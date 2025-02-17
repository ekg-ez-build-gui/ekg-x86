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
  ekg::flags modes
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
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::arrow)]      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::ibeam)]      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::wait)]       = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::crosshair)]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::wait_arrow)] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::size_nwse)]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::size_nesw)]  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::size_we)]    = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::size_ns)]    = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::size_all)]   = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::no)]         = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
  this->loaded_system_cursor_list[static_cast<uint64_t>(ekg::system_cursor::hand)]       = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

  this->system_cursor = ekg::system_cursor::arrow;
  this->update_cursor();

  SDL_GetWindowSize(this->p_sdl_win, &ekg::viewport.w, &ekg::viewport.h);

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
          this->modes,
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
        ekg::p_core->update_size_changed();

        break;
    }
    break;
  case SDL_KEYDOWN:
    this->serialized_input_event.event_type = ekg::input_event_type::key_down;
    this->serialized_input_event.key.key = static_cast<int32_t>(sdl_event.key.keysym.sym);
    must_poll_events = true;
    break;
  case SDL_KEYUP:
    this->serialized_input_event.event_type = ekg::input_event_type::key_up;
    this->serialized_input_event.key.key = static_cast<int32_t>(sdl_event.key.keysym.sym);
    must_poll_events = true;
    break;
  case SDL_TEXTINPUT:
    this->serialized_input_event.event_type = ekg::input_event_type::text_input;
    this->serialized_input_event.text_input = sdl_event.text.text;
    must_poll_events = true;
    break;
  case SDL_MOUSEBUTTONUP:
    this->serialized_input_event.event_type = ekg::input_event_type::mouse_button_up;
    this->serialized_input_event.mouse_button = sdl_event.button.button;
    must_poll_events = true;
    break;
  case SDL_MOUSEBUTTONDOWN:
    this->serialized_input_event.event_type = ekg::input_event_type::mouse_button_down;
    this->serialized_input_event.mouse_button = sdl_event.button.button;
    must_poll_events = true;
    break;
  case SDL_MOUSEWHEEL:
    this->serialized_input_event.event_type = ekg::input_event_type::mouse_wheel;
    this->serialized_input_event.mouse_wheel_x = sdl_event.wheel.x;
    this->serialized_input_event.mouse_wheel_y = sdl_event.wheel.y;
    this->serialized_input_event.mouse_wheel_precise_x = sdl_event.wheel.preciseX;
    this->serialized_input_event.mouse_wheel_precise_y = sdl_event.wheel.preciseY;
    must_poll_events = true;
    break;
  case SDL_MOUSEMOTION:
    this->serialized_input_event.event_type = ekg::input_event_type::mouse_motion;
    this->serialized_input_event.mouse_motion_x = sdl_event.motion.x;
    this->serialized_input_event.mouse_motion_y = sdl_event.motion.y;
    must_poll_events = true;
    break;
  case SDL_FINGERUP:
    this->serialized_input_event.event_type = ekg::input_event_type::finger_up;
    this->serialized_input_event.finger_x = sdl_event.tfinger.x;
    this->serialized_input_event.finger_y = sdl_event.tfinger.y;
    must_poll_events = true;
    break;
  case SDL_FINGERDOWN:
    this->serialized_input_event.event_type = ekg::input_event_type::finger_down;
    this->serialized_input_event.finger_x = sdl_event.tfinger.x;
    this->serialized_input_event.finger_y = sdl_event.tfinger.y;
    must_poll_events = true;
    break;
  case SDL_FINGERMOTION:
    this->serialized_input_event.event_type = ekg::input_event_type::finger_motion;
    this->serialized_input_event.finger_x = sdl_event.tfinger.x;
    this->serialized_input_event.finger_y = sdl_event.tfinger.y;
    this->serialized_input_event.finger_dx = sdl_event.tfinger.dx;
    this->serialized_input_event.finger_dy = sdl_event.tfinger.dy;
    must_poll_events = true;
    break;
  }

  if (must_poll_events) {
    this->system_cursor = ekg::system_cursor::arrow;
    ekg::p_core->poll_events();
    must_poll_events = false;
  }
}
