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

#include <cinttypes>
#include <iostream>
#include <fstream>

#include "ekg/ekg.hpp"
#include "ekg/os/ekg_sdl.hpp"

ekg::sdl::sdl(
  SDL_Window *p_sdl_win,
  ekg::flags_t modes
) {
  this->p_sdl_win = p_sdl_win;
  this->modes = modes;
  this->update_display_size();

  int32_t w {}, h {};

  /**
   * Seems like an unnecessary resize, but it is invoke a complete swapchain redo,
   * to fix the orthographic matrix neededs calc.
   */

  SDL_GetWindowSize(this->p_sdl_win, &w, &h);
  SDL_SetWindowSize(this->p_sdl_win, w--, h--);
  SDL_SetWindowSize(this->p_sdl_win, w++, h++);
}

void ekg::sdl::set_clipboard_text(const char *p_text) {
  SDL_SetClipboardText(p_text);
}

bool ekg::sdl::has_clipboard_text() {
  return SDL_HasClipboardText();
}

const char *ekg::sdl::get_clipboard_text() {
  return SDL_GetClipboardText();
}

void ekg::sdl::init() {
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::arrow)]      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::ibeam)]      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::wait)]       = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::crosshair)]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::wait_arrow)] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_nwse)]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_nesw)]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_we)]    = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_ns)]    = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::size_all)]   = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::no)]         = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor_type::hand)]       = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

  this->system_cursor = ekg::system_cursor_type::arrow;
  this->update_cursor();

  ekg::vec2_t<int32_t> window_size {};
  SDL_GetWindowSize(this->p_sdl_win, &window_size.x, &window_size.y);

  ekg::viewport.w = static_cast<float>(window_size.x);
  ekg::viewport.h = static_cast<float>(window_size.y);

  SDL_version sdl_version {};
  SDL_GetVersion(&sdl_version);

  ekg::log() << "SDL version: "
             << static_cast<int32_t>(sdl_version.major)
             << '.'
             << static_cast<int32_t>(sdl_version.minor)
             << '.'
             << static_cast<int32_t>(sdl_version.patch);
}

void ekg::sdl::quit() {
  
}

void ekg::sdl::update_cursor() {
  SDL_SetCursor(
    this->loaded_system_cursor_list[static_cast<uint64_t>(this->system_cursor)]
  );
}

void ekg::sdl::update_display_size() {
  SDL_DisplayMode sdl_display_mode {};
  SDL_GetDisplayMode(0, 0, &sdl_display_mode);

  this->display_size.w = static_cast<int32_t>(sdl_display_mode.w);
  this->display_size.h = static_cast<int32_t>(sdl_display_mode.h);
}

uint64_t ekg::sdl::get_ticks() {
  return SDL_GetTicks64();
}

void ekg::sdl::get_key_name(ekg::io::input_key_t &key, std::string &name) {
  switch (key.key) {
    case SDLK_LCTRL:
      name = "lctrl";
      break;
    case SDLK_RCTRL:
      name = "rctrl";
      break;
    case SDLK_LSHIFT:
      name = "lshift";
      break;
    case SDLK_RSHIFT:
      name = "rshift";
      break;
    case SDLK_LALT:
      name = "alt";
      break;
    case SDLK_RALT:
      name = "altgr";
      break;
    case SDLK_TAB:
      name = "tab";
      break;
    default:
      name = SDL_GetKeyName(key.key);
      break;
  }
}

void ekg::sdl::get_special_key(ekg::io::input_key_t &key, ekg::special_key_type &special_key) {
  switch (key.key) {
    case SDLK_LCTRL:
      special_key = ekg::special_key_type::left_ctrl;
      break;
    case SDLK_RCTRL:
      special_key = ekg::special_key_type::right_ctrl;
      break;
    case SDLK_LSHIFT:
      special_key = ekg::special_key_type::left_shift;
      break;
    case SDLK_RSHIFT:
      special_key = ekg::special_key_type::right_shift;
      break;
    case SDLK_LALT:
      special_key = ekg::special_key_type::left_alt;
      break;
    case SDLK_RALT:
      special_key = ekg::special_key_type::right_alt;
      break;
    case SDLK_TAB:
      special_key = ekg::special_key_type::tab;
      break;
    default:
      special_key = ekg::special_key_type::unknown;
      break;
  }
}

void ekg::sdl_poll_event(SDL_Event &sdl_event) {
  bool must_poll_events {};

  switch (sdl_event.type) {
  default:
    break;
  case SDL_WINDOWEVENT:
    if (
        ekg::has(
          ekg::p_core->p_os_platform->modes,
          ekg::internal_behavior::no_auto_set_viewport_when_resize
        )
      ) {
      break;
    }

    switch (sdl_event.window.event) {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        ekg::viewport.w = sdl_event.window.data1;
        ekg::viewport.h = sdl_event.window.data2;

        ekg::p_core->p_gpu_api->update_viewport(ekg::viewport.w, ekg::viewport.h);
        ekg::io::dispatch(ekg::io::operation::scale_update);

        break;
    }
    break;
  case SDL_KEYDOWN:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::key_down;
    ekg::p_core->p_os_platform->serialized_input_event.key.key = static_cast<int32_t>(sdl_event.key.keysym.sym);
    must_poll_events = true;
    break;
  case SDL_KEYUP:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::key_up;
    ekg::p_core->p_os_platform->serialized_input_event.key.key = static_cast<int32_t>(sdl_event.key.keysym.sym);
    must_poll_events = true;
    break;
  case SDL_TEXTINPUT:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::text_input;
    ekg::p_core->p_os_platform->serialized_input_event.text_input = sdl_event.text.text;
    must_poll_events = true;
    break;
  case SDL_MOUSEBUTTONUP:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_button_up;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_button = sdl_event.button.button;
    must_poll_events = true;
    break;
  case SDL_MOUSEBUTTONDOWN:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_button_down;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_button = sdl_event.button.button;
    must_poll_events = true;
    break;
  case SDL_MOUSEWHEEL:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_wheel;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_x = sdl_event.wheel.x;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_y = sdl_event.wheel.y;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_precise_x = sdl_event.wheel.preciseX;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_wheel_precise_y = sdl_event.wheel.preciseY;
    must_poll_events = true;
    break;
  case SDL_MOUSEMOTION:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::mouse_motion;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_motion_x = sdl_event.motion.x;
    ekg::p_core->p_os_platform->serialized_input_event.mouse_motion_y = sdl_event.motion.y;
    must_poll_events = true;
    break;
  case SDL_FINGERUP:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::finger_up;
    ekg::p_core->p_os_platform->serialized_input_event.finger_x = sdl_event.tfinger.x;
    ekg::p_core->p_os_platform->serialized_input_event.finger_y = sdl_event.tfinger.y;
    must_poll_events = true;
    break;
  case SDL_FINGERDOWN:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::finger_down;
    ekg::p_core->p_os_platform->serialized_input_event.finger_x = sdl_event.tfinger.x;
    ekg::p_core->p_os_platform->serialized_input_event.finger_y = sdl_event.tfinger.y;
    must_poll_events = true;
    break;
  case SDL_FINGERMOTION:
    ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::finger_motion;
    ekg::p_core->p_os_platform->serialized_input_event.finger_x = sdl_event.tfinger.x;
    ekg::p_core->p_os_platform->serialized_input_event.finger_y = sdl_event.tfinger.y;
    ekg::p_core->p_os_platform->serialized_input_event.finger_dx = sdl_event.tfinger.dx;
    ekg::p_core->p_os_platform->serialized_input_event.finger_dy = sdl_event.tfinger.dy;
    must_poll_events = true;
    break;
  }

  if (must_poll_events) {
    ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
    ekg::p_core->poll_events();
    must_poll_events = false;
  }
}
