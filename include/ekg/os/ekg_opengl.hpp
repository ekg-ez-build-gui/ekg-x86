/**
 * MIT License
 * 
 * Copyright (c) 2022-2024 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// TODO: fix pattern to new-project rules
// TODO: remove useless macros
// TODO: add some comments if necessary

#ifndef EKG_OS_OPENGL_HPP
#define EKG_OS_OPENGL_HPP

#if defined(__ANDROID__)
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <vector>
#include <unordered_map>

#include "ekg/gpu/api.hpp"

#define ekg_is_sampler_protected(sampler_protected_index) (sampler_protected_index > -1)
#define EKG_DISABLE_TEXTURE 0
#define EKG_ENABLE_TEXTURE_PROTECTED 1
#define EKG_ENABLE_TEXTURE 2

namespace ekg {
  enum class opengl_version {
    es,
    core_profile
  };

  class opengl : public ekg::gpu::api {
  protected:
    ekg::os::opengl_version opengl_version {};
    std::vector<ekg::gpu::sampler_t*> bound_sampler_list {};
    std::string_view glsl_version {};
    
    int32_t uniform_active_texture {};
    int32_t uniform_active_tex_slot {};
    int32_t uniform_content {};
    int32_t uniform_rect {};
    int32_t uniform_line_thickness {};
    int32_t uniform_scissor {};
    int32_t uniform_viewport_height {};
    int32_t uniform_projection {};

    uint32_t geometry_buffer {};
    uint32_t vbo_array {};
    uint32_t ebo_simple_shape {};
    uint32_t pipeline_program {};
    uint8_t protected_texture_active_index {};
  public:
    bool create_pipeline_program(
      uint32_t &program,
      const std::unordered_map<std::string_view, uint32_t> &resources
    );
  public:
    /**
     * OpenGL API wrapper abstraction constructor;
     * `set_glsl_version` must be 330 higher, if not, the version is auto-initialized as `450`.
     * OpenGL ES 3 needs explicit set to the GLSL ES version.
     */
    explicit opengl(std::string_view set_glsl_version = "#version 450");
  public:
    ekg::os::opengl_version get_version();

    void log_vendor_details() override;

    void init() override;
    void quit() override;
    void pre_re_alloc() override;
    void update_viewport(int32_t w, int32_t h) override;
    void re_alloc_geometry_resources(const float *p_data, uint64_t size) override;
    
    void draw(
      ekg::gpu_data_t *p_gpu_data,
      uint64_t loaded_gpu_data_size
    ) override;

    ekg::flags_t allocate_sampler(
      ekg::gpu::sampler_allocate_info *p_sampler_allocate_info,
      ekg::gpu::sampler_t *p_sampler
    ) override;

    ekg::flags_t fill_sampler(
      ekg::gpu::sampler_fill_info *sampler_fill_info,
      ekg::gpu::sampler_t *p_sampler
    ) override;

    ekg::flags_t gen_font_atlas_and_map_glyph(
      ekg::sampler_t *p_sampler,
      ekg::draw::font_face_t *p_font_face_text,
      ekg::draw::font_face_t *p_font_face_emoji,
      ekg::draw::font_face_t *p_font_face_kanjis,
      ekg::rect<int32_t> &atlas_rect,
      std::vector<char32_t> &char_to_gen_sampler_list,
      std::unordered_map<char32_t, ekg::draw::glyph_char_t> &mapped_gpu_data_char_glyph,
      float &non_swizzlable_range
    ) override;

    ekg::flags_t bind_sampler(
      ekg::gpu::sampler_t *p_sampler
    ) override;
  };
}

#endif
