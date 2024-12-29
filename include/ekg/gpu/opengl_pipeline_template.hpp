#ifndef EKG_GPU_OPENGL_PIPELINE_TEMPLATE_HPP
#define EKG_GPU_OPENGL_PIPELINE_TEMPLATE_HPP

#include "ekg/os/ekg_opengl.hpp"

namespace ekg::gpu {
  void get_standard_vertex_shader(
    std::string glsl_version,
    ekg::os::opengl_version opengl_version,
    std::string &output_kernel_source
  );

  void get_standard_fragment_shader(
    std::string glsl_version,
    ekg::os::opengl_version opengl_version,
    std::string &output_kernel_source
  );
}

#endif