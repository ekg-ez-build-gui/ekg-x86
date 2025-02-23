#ifndef EKG_TEST_APPLICATION_HPP
#define EKG_TEST_APPLICATION_HPP

#include <ekg/ekg.hpp>
#include <ekg/os/ekg_sdl.hpp>

extern struct application_t {
  SDL_Window *p_sdl_win {};
  bool vsync {true};
  bool is_running {};
} app;

namespace laboratory {
  struct shape {
  protected:
    float mat4x4_projection[16] {};
    uint32_t program {};
    uint32_t gbuffer {};
    uint32_t buffer_collection {};
    uint32_t uniform_matrix_projection {};
    uint32_t uniform_rect {};
    uint32_t uniform_color {};
  public:
    shape();
  public:
    void on_resize();
    void invoke();
    void draw(const ekg::rect_t<float> &rect, const ekg::vec4_t<float> &color);
    void revoke();
  };
}

#endif
