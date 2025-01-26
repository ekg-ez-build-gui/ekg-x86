#ifndef EKG_IO_INPUT_HPP
#define EKG_IO_INPUT_HPP

#include <cstdint>
#include <string_view>

namespace ekg {
  enum class system_cursor_type {
    arrow,
    ibeam,
    wait,
    crosshair,
    wait_arrow,
    size_nwse,
    size_nesw,
    size_we,
    size_ns,
    size_all,
    no,
    hand
  };

  enum class special_key_type {
    left_shift,
    right_shift,
    left_ctrl,
    right_ctrl,
    left_alt,
    right_alt,
    tab,
    unknown
  };

  enum internal_behavior {
    no_auto_set_viewport_when_resize = 2 << 1
  };
}

namespace ekg::io {
  enum class input_event_type {
    none,
    text_input,
    mouse_button_up,
    mouse_button_down,
    mouse_motion,
    mouse_wheel,
    finger_up,
    finger_down,
    finger_motion,
    key_down,
    key_up
  };

  struct input_key_t {
  public:
    int32_t key {};
    int32_t scancode {};
  };

  struct serialized_input_event_t {
  public:
    ekg::platform_event_type event_type {};
    std::string_view text_input {};
    uint8_t mouse_button {};

    int32_t mouse_motion_x {};
    int32_t mouse_motion_y {};

    int32_t mouse_wheel_x {};
    int32_t mouse_wheel_y {};

    float mouse_wheel_precise_x {};
    float mouse_wheel_precise_y {};

    ekg::io::input_key key {};

    float finger_x {};
    float finger_y {};

    float finger_dx {};
    float finger_dy {};
  };
}

#endif