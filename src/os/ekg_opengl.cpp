#include "ekg/os/ekg_opengl.hpp"
#include "ekg/gpu/opengl_pipeline_template.hpp"
#include "ekg/ekg.hpp"
#include "ekg/gpu/api.hpp"
#include "ekg/util/image.hpp"

#include <cstdio>
#include <unordered_map>
#include <iostream>
#include <regex>

ekg::opengl::opengl(std::string_view set_glsl_version) {
  if (set_glsl_version.empty()) {
    ekg::log() << "[GPU][API] not viable glsl version, empty, must: 330 higher for core-profile or 300 higher for ES";
    return;
  }

  std::string glsl_version {set_glsl_version};
  std::regex es_re {"es"};

  if (std::regex_search(glsl_version, es_re)) {
    this->opengl_version = ekg::os::opengl_version::es;
  } else {
    this->opengl_version = ekg::os::opengl_version::core_profile;
  }

  std::regex number_re {"\\d+"};  
  if (std::regex_search(glsl_version, number_re)) {
    std::sregex_iterator next(glsl_version.begin(), glsl_version.end(), number_re);
    std::sregex_iterator end {};
    std::string version {};
    while (next != end) {
      std::smatch match = *next;
      version = match.str();
      next++;
    }

    if (version.empty() || version.size() != 3) {
      ekg::log() << "[GPU][API] not viable glsl version, invalid number, must: 330 higher for core-profile or 300 higher for ES";
      return;
    }
  } else {
    ekg::log() << "[GPU][API] not viable glsl version, unknown number, must: 330 higher for core-profile or 300 higher for ES";
    return; 
  }

  this->glsl_version = set_glsl_version;
}

void ekg::opengl::log_vendor_details() {
  ekg::log() << glGetString(GL_VENDOR);
}

void ekg::opengl::init() {
  std::string no_view_glsl_version {
    this->glsl_version
  };

  std::string vsh_src {};
  ekg::gpu::get_standard_vertex_shader(
    no_view_glsl_version,
    this->opengl_version,
    vsh_src
  );

  std::string fsh_src {};
  ekg::gpu::get_standard_fragment_shader(
    no_view_glsl_version,
    this->opengl_version,
    fsh_src
  );

  if (!this->rendering_shader_fragment_source.empty()) {
    fsh_src = this->rendering_shader_fragment_source;
  }

  ekg::log() << "Loading internal shaders...";

  /* Create main shading program using two basic shaders (vertex & fragment). */
  this->create_pipeline_program(this->pipeline_program, {
    {vsh_src, GL_VERTEX_SHADER},
    {fsh_src, GL_FRAGMENT_SHADER}
  });

  glGenVertexArrays(1, &this->vbo_array);
  glGenBuffers(1, &this->geometry_buffer);
  glGenBuffers(1, &this->ebo_simple_shape);

  /* Generate base shape rendering. */
  uint8_t simple_shape_mesh_indices[6] {
      0, 2, 3,
      3, 1, 0
  };

  glBindVertexArray(this->vbo_array);

  /* Start of geometry resources buffer attributes. */
  glBindBuffer(GL_ARRAY_BUFFER, this->geometry_buffer);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *) (sizeof(float) * 2));
  /* End of geometry resources buffer attributes. */

  /* Start of simple shape indexing buffer bind to VAO. */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_simple_shape);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(simple_shape_mesh_indices), simple_shape_mesh_indices, GL_STATIC_DRAW);
  glBindVertexArray(0);
  /* End  of simple shape indexing buffer bind to VAO. */

  /* reduce glGetLocation calls when rendering the batch */
  this->uniform_active_texture = glGetUniformLocation(this->pipeline_program, "uActiveTexture");
  this->uniform_active_tex_slot = glGetUniformLocation(this->pipeline_program, "uTextureSampler");
  this->uniform_content = glGetUniformLocation(this->pipeline_program, "uContent");
  this->uniform_rect = glGetUniformLocation(this->pipeline_program, "uRect");
  this->uniform_line_thickness = glGetUniformLocation(this->pipeline_program, "uLineThickness");
  this->uniform_scissor = glGetUniformLocation(this->pipeline_program, "uScissor");
  this->uniform_viewport_height = glGetUniformLocation(this->pipeline_program, "uViewportHeight");
  this->uniform_projection = glGetUniformLocation(this->pipeline_program, "uProjection");

  ekg::log() << "GPU shaders, pipeline program, and uniforms done";
}

void ekg::opengl::quit() {

}

void ekg::opengl::pre_re_alloc() {
  this->protected_texture_active_index = 0;
}

void ekg::opengl::update_viewport(int32_t w, int32_t h) {
  this->viewport.x = 0.0f;
  this->viewport.y = 0.0f;
  this->viewport.w = static_cast<float>(w);
  this->viewport.h = static_cast<float>(h);

  ekg::ortho(
    this->projection_matrix,
    0,
    this->viewport.w,
    this->viewport.h,
    0
  );

  glUseProgram(this->pipeline_program);
  glUniformMatrix4fv(this->uniform_projection, GL_TRUE, 0, this->projection_matrix);
  glUniform1f(this->uniform_viewport_height, this->viewport.h);
  glUseProgram(0);
}

bool ekg::opengl::create_pipeline_program(uint32_t &program, const std::unordered_map<std::string_view, uint32_t> &resources) {
  if (resources.empty()) {
    ekg::log() << "Error: Invalid shader, empty resources";
    return true;
  }

  std::string shader_src {};
  std::vector<uint32_t> compiled_shader_list {};
  int32_t status {};
  program = glCreateProgram();

  std::string msg {};
  for (auto &[value, key]: resources) {
    uint32_t shader {glCreateShader(key)};
    const char *p_src {value.data()};
    glShaderSource(shader, 1, &p_src, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
      msg.resize(status);
      glGetShaderInfoLog(shader, status, nullptr, msg.data());
      ekg::log() << "Error: Failed to compile shader '" << p_src  << "' stage: \n" << msg;
      break;
    }

    compiled_shader_list.push_back(shader);
  }

  bool keep {compiled_shader_list.size() == resources.size()};

  for (uint32_t &shaders: compiled_shader_list) {
    if (keep) {
      glAttachShader(program, shaders);
    }

    glDeleteShader(shaders);
  }

  if (keep) {
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status);
      msg.resize(status);
      glGetProgramInfoLog(program, status, nullptr, msg.data());
      ekg::log() << "Error: Failed to link program: \n" << msg;
    }
  }

  return false;
}

void ekg::opengl::re_alloc_geometry_resources(
  const float *p_data,
  uint64_t size
) {
  // TODO: add capacity to gbuffers instead buffer data directly
  // also use mapped buffers to get ptr directly.

  glBindVertexArray(this->vbo_array);

  glBindBuffer(GL_ARRAY_BUFFER, this->geometry_buffer);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(float)*size,
    p_data,
    GL_STATIC_DRAW
  );

  glBindVertexArray(0);
}

ekg::flags_t ekg::opengl::allocate_sampler(
  ekg::gpu::sampler_allocate_info *p_sampler_allocate_info,
  ekg::sampler_t *p_sampler
) {
  if (p_sampler == nullptr) {
    return ekg::result::failed;
  }

  if (!p_sampler->gl_id) {
    glGenTextures(1, &p_sampler->gl_id);
  }

  glBindTexture(
    GL_TEXTURE_2D,
    p_sampler->gl_id
  );

  if (p_sampler_allocate_info->gl_unpack_alignment) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }

  p_sampler->w = p_sampler_allocate_info->w;
  p_sampler->h = p_sampler_allocate_info->h;

  if (p_sampler_allocate_info->gl_wrap_modes[0]) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_sampler_allocate_info->gl_wrap_modes[0]);
  }

  if (p_sampler_allocate_info->gl_wrap_modes[1]) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_sampler_allocate_info->gl_wrap_modes[1]);
  }

  if (p_sampler_allocate_info->gl_parameter_filter[0]) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p_sampler_allocate_info->gl_parameter_filter[0]);
  }

  if (p_sampler_allocate_info->gl_parameter_filter[1]) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p_sampler_allocate_info->gl_parameter_filter[1]);
  }

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    p_sampler_allocate_info->gl_internal_format,
    p_sampler_allocate_info->w,
    p_sampler_allocate_info->h,
    0,
    p_sampler_allocate_info->gl_format,
    p_sampler_allocate_info->gl_type,
    p_sampler_allocate_info->p_data
  );

  if (p_sampler_allocate_info->gl_generate_mipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  p_sampler->p_tag = p_sampler_allocate_info->p_tag;
  return ekg::result::success;
}

ekg::flags_t ekg::opengl::fill_sampler(
  ekg::gpu::sampler_fill_info *p_sampler_fill_info,
  ekg::sampler_t *p_sampler
) {
  if (p_sampler == nullptr || !p_sampler->gl_id) {
    return ekg::result::failed;
  }

  glBindTexture(
    GL_TEXTURE_2D,
    p_sampler->gl_id
  );
  
  glTexSubImage2D(
    GL_TEXTURE_2D,
    0,
    p_sampler_fill_info->offset[0],
    p_sampler_fill_info->offset[1],
    p_sampler_fill_info->w,
    p_sampler_fill_info->h,
    p_sampler_fill_info->gl_format,
    p_sampler_fill_info->gl_type,
    p_sampler_fill_info->p_data
  );

  glBindTexture(GL_TEXTURE_2D, 0);
  return ekg::result::success;
}

ekg::flags_t ekg::opengl::gen_font_atlas_and_map_glyph(
  ekg::sampler_t *p_sampler,
  ekg::draw::font_face_t *p_font_face_text,
  ekg::draw::font_face_t *p_font_face_emoji,
  ekg::draw::font_face_t *p_font_face_kanjis,
  ekg::rect_t<int32_t> &atlas_rect,
  std::vector<char32_t> &char_to_gen_sampler_list,
  std::unordered_map<char32_t, ekg::draw::glyph_char_t> &mapped_gpu_data_char_glyph,
  float &non_swizzlable_range
) {
  constexpr enum_count_font_face_type {3};
  ekg::draw::font_face_t *faces[3] {
    p_font_face_text,
    p_font_face_emoji,
    p_font_face_kanjis
  };

  float highest_square {};
  float square {};
  FT_Vector highest_glyph_size {};

  for (size_t it {}; it < 3; it++) {
    ekg::draw::font_face_t *&p_font_face {
      faces[it]
    };

    square = (
      p_font_face->highest_glyph_size.x
      *
      p_font_face->highest_glyph_size.y
    );
    
    if (square > highest_square) {
      highest_square = square;
      highest_glyph_size = p_font_face_emoji->highest_glyph_size;
    } 
  }

  GLint sub_image_format {
    GL_RED
  };

  bool is_current_opengl_version_gl_es {
    this->opengl_version == ekg::opengl_version::es
  };

  std::vector<unsigned char> r8_to_r8g8b8a8_swizzled_image {};
  unsigned char *p_current_image_buffer {};

  if (is_current_opengl_version_gl_es) {
    sub_image_format = GL_RGBA;

    r8_to_r8g8b8a8_swizzled_image.resize(
      highest_glyph_size.x
      *
      highest_glyph_size.y
      *
      4
    );
  }
  
  if (!p_sampler->gl_id) {
    glGenTextures(1, &p_sampler->gl_id);
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, p_sampler->gl_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    atlas_rect.w,
    atlas_rect.h,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    nullptr
  );

  FT_GlyphSlot ft_glyph_slot {};
  FT_Face ft_face {};
  FT_Vector char_size {};

  ekg::flags_t flags {};
  float offset {};

  for (char32_t &char32 : p_loaded_sampler_generate_list) {
    switch (char32 < 256 || !p_font_face_emoji->font_face_loaded) {
      case true: {
        ft_face = p_font_face_text->ft_face;
        ft_glyph_slot = p_font_face_text->ft_face->glyph;
        non_swizzlable_range = offset;
        flags = FT_LOAD_RENDER;
        break;
      }

      default: {
        ft_face = p_font_face_emoji->ft_face;
        ft_glyph_slot = p_font_face_emoji->ft_face->glyph;
        flags = FT_LOAD_RENDER | FT_LOAD_COLOR;
        break;
      }
    }

    if (FT_Load_Char(ft_face, char32, flags)) {
      continue;
    }

    ekg::draw::glyph_char_t &char_data {mapped_glyph_char_data[char32]};
    char_data.x = offset / static_cast<float>(atlas_width);

    p_current_image_buffer = ft_glyph_slot->bitmap.buffer;

    if (
        is_current_opengl_version_gl_es
        &&
        !ekg::has(flags, FT_LOAD_COLOR)
      ) {

      const unsigned char *p_src_copy {
        ft_glyph_slot->bitmap.buffer
      };

      char_size.x = char_data.w;
      char_size.y = char_data.h;

      ekg::format_convert_result result {
        ekg::image_src_r8_convert_to_r8g8b8a8(
          char_size,
          p_src_copy,
          r8_to_r8g8b8a8_swizzled_image
        )
      };

      if (result == ekg::format_convert_result::failed) {
        ekg::log() << "Warning: could not convert character '" << char32 << "' from r8 to r8g8b8a8 on OpenGL ES3";
      } else {
        p_current_image_buffer = r8_to_r8g8b8a8_swizzled_image.data();
      }
    }

    glTexSubImage2D(
      GL_TEXTURE_2D,
      0,
      static_cast<GLint>(offset),
      0,
      static_cast<GLsizei>(char_data.w),
      static_cast<GLsizei>(char_data.h),
      (
        ekg::has(flags, FT_LOAD_COLOR) ? GL_BGRA : sub_image_format
      ),
      GL_UNSIGNED_BYTE,
      p_current_image_buffer
    );

    offset += char_data.w;
    char_data.was_sampled = true;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  non_swizzlable_range = (
    non_swizzlable_range / static_cast<float>(atlas_width)
  );

  return ekg::result::success;
}

ekg::flags_t ekg::opengl::bind_sampler(ekg::sampler_t *p_sampler) {
  uint64_t size {this->bound_sampler_list.size()};
  for (uint64_t it {}; it < this->bound_sampler_list.size(); it++) {
    ekg::sampler_t *p_in_list_sampler {this->bound_sampler_list.at(it)};
    if (p_in_list_sampler->gl_id == p_sampler->gl_id) {
      return it;
    }
  }

  if (ekg_is_sampler_protected(p_sampler->gl_protected_active_index)) {
    p_sampler->gl_protected_active_index = this->protected_texture_active_index++;
  }

  this->bound_sampler_list.emplace_back() = p_sampler;
  return size;
}

void ekg::opengl::draw(
  ekg::gpu::data_t *p_gpu_data,
  uint64_t loaded_gpu_data_size
) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(this->pipeline_program);
  glBindVertexArray(this->vbo_array);

  /**
   * Protected samplers such as font, icons, and default EKG samplers,
   * should not overhead the glBindTexture calls,
   * this reason the active index only increase
   * when THIS is a protected sampler.
   **/
  for (ekg::sampler_t *&p_sampler : this->bound_sampler_list) {
    if (p_sampler->gl_protected_active_index == -1) continue;

    glActiveTexture(
      GL_TEXTURE0 + p_sampler->gl_protected_active_index
    );

    glBindTexture(GL_TEXTURE_2D, p_sampler->gl_id);
  }

  glActiveTexture(GL_TEXTURE0 + this->protected_texture_active_index);

  int32_t previous_sampler_bound {};
  bool sampler_going_on {};

  for (uint64_t it {}; it < loaded_gpu_data_size-1; it++) {
    ekg::gpu::data_t &data {p_gpu_data[it]};
    sampler_going_on = data.sampler_index > -1;

    if (
        sampler_going_on
        &&
        (
          previous_sampler_bound != data.sampler_index
          ||
          !ekg_is_sampler_protected(this->bound_sampler_list.at(data.sampler_index)->gl_protected_active_index)
        )
      ) {

      ekg::sampler_t *&p_sampler {
        this->bound_sampler_list.at(data.sampler_index)
      };

      if (ekg_is_sampler_protected(p_sampler->gl_protected_active_index)) {
        glUniform1i(this->uniform_active_tex_slot, p_sampler->gl_protected_active_index);
        glUniform1i(this->uniform_active_texture, EKG_ENABLE_TEXTURE_PROTECTED);
      } else {
        glBindTexture(GL_TEXTURE_2D, p_sampler->gl_id);
        glUniform1i(this->uniform_active_tex_slot, this->protected_texture_active_index);
        glUniform1i(this->uniform_active_texture, EKG_ENABLE_TEXTURE);
      }

      previous_sampler_bound = data.sampler_index;
    } else if (!sampler_going_on && previous_sampler_bound > -1) {
      glUniform1i(this->uniform_active_texture, EKG_DISABLE_TEXTURE);
      previous_sampler_bound = -1;
    }

    glUniform1i(this->uniform_line_thickness, data.line_thickness);
    glUniform4fv(this->uniform_rect, GL_TRUE, data.buffer_content);
    glUniform1fv(this->uniform_content, 8, &data.buffer_content[4]);

    switch (data.begin_stride) {
      case 0: {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
        break;
      }

      default: {
        glDrawArrays(GL_TRIANGLES, data.begin_stride, data.end_stride);
        break;
      }
    }
  }

  glDisable(GL_BLEND);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, 0);

  glUseProgram(0);
}

ekg::opengl *ekg::os::get_opengl() {
  return static_cast<ekg::opengl*>(ekg::core->p_gpu_api);
}