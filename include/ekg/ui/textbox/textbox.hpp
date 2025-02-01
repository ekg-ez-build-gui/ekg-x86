#ifndef EKG_UI_TEXTBOX_HPP
#define EKG_UI_TEXTBOX_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct textbox_theme_t {
  public:
    ekg::vec4<float> string {};
    ekg::vec4<float> background {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> cursor {};
    ekg::vec4<float> select {};
  };

  struct textbox_t {
  public:
  };
}

#endif