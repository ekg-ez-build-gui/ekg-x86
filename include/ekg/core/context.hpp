#ifndef EKG_CORE_CONTEXT_HPP
#define EKG_CORE_CONTEXT_HPP

#include "ekg/io/typography.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/ui/properties.hpp"

namespace ekg {
  extern FT_Library freetype_library;

  extern struct viewport_t {
    float x {};
    float y {};
    float w {};
    float h {};

    bool auto_scale {};
    ekg::vec4_t<float> scale {};
    bool redraw {};
  } viewport;

  extern struct current_t {
    ekg::id_t pressed {};
    ekg::type pressed_type {};
    ekg::id_t released {};
    ekg::type released_type {};
    ekg::id_t last {};
    ekg::id_t unique_id {};
    ekg::type type {};
  } current;
}

#endif
