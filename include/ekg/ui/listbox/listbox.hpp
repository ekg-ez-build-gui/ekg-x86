#ifndef EKG_UI_LISTBOX_HPP
#define EKG_UI_LISTBOX_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct listbox_theme_t {
  public:
    ekg::vec4<float> header_background {};
    ekg::vec4<float> header_highlight_outline {};
    ekg::vec4<float> header_highlight {};
    ekg::vec4<float> header_outline {};
    ekg::vec4<float> header_string {};
    ekg::vec4<float> item_background {};
    ekg::vec4<float> item_highlight_outline {};
    ekg::vec4<float> item_highlight {};
    ekg::vec4<float> item_focused {};
    ekg::vec4<float> item_focused_outline {};
    ekg::vec4<float> item_string {};
    ekg::vec4<float> item_outline {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> background {};
    ekg::vec4<float> line_separator {};
    ekg::vec4<float> drag_background {};
    ekg::vec4<float> drag_outline {};
    
    float subitem_offset_space {4.0f};
  };

  struct listbox_t {
  public:
  };
}

#endif