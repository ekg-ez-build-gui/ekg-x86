#ifndef EKG_OS_GLFW_HPP
#define EKG_OS_GLFW_HPP

#include "platform.hpp"

#if defined(__ANDROID__)
// there is no glfw... for android I guess idk i do not give a fuck
#else
#include <GLFW/glfw3.h>
#endif

namespace ekg {
  class glfw : public ekg::os::platform {
  protected:
    GLFWcursor *loaded_system_cursor_list[12] {};
    GLFWwindow *p_glfw_win {};
  public:
    explicit glfw(
      GLFWwindow *p_glfw_win,
      ekg::flags_t modes = static_cast<ekg::flags_t>(0)
    );
  public:
    void init() override;
    void quit() override;
    void update_display_size() override;
    void update_cursor() override;
    void get_key_name(ekg::io::input_key_t &key, std::string &name) override;
    void get_special_key(ekg::io::input_key_t &key, ekg::special_key_type &special_key) override;
    const char *get_clipboard_text() override;
    void set_clipboard_text(const char *p_text) override;
    bool has_clipboard_text() override;
    uint64_t get_ticks() override;
  };

  void glfw_window_size_callback(
    int32_t w,
    int32_t h
  );

  void glfw_char_callback(
    uint32_t codepoint
  );

  void glfw_key_callback(
    int32_t key,
    int32_t scancode,
    int32_t action,
    int32_t mods
  );

  void glfw_cursor_pos_callback(
    double x,
    double y
  );

  void glfw_scroll_callback(
    double dx,
    double dy
  );
  
  void glfw_mouse_button_callback(
    int32_t button,
    int32_t action,
    int32_t mods
  );
}

#endif
