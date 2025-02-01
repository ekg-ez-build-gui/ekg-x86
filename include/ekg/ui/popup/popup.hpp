#ifndef EKG_UI_POPUP_HPP
#define EKG_UI_POPUP_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct popup_theme_t {
  public:
    ekg::vec4<float> background {};
    ekg::vec4<float> string {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> highlight {};
    ekg::vec4<float> separator {};
    int64_t drop_animation_delay {};
  };

  struct popup_t {
  public:
  };
}

#endif