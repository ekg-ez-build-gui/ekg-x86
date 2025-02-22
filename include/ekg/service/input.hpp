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

#ifndef EKG_SERVICE_INPUT_HPP
#define EKG_SERVICE_INPUT_HPP

#include <vector>
#include <unordered_map>
#include <array>

#include "ekg/io/log.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/os/platform.hpp"

namespace ekg::service {
  class input {
  protected:
    std::unordered_map<std::string, std::vector<bool*>> input_bindings_map {};
    std::unordered_map<std::string, ekg::io::input_bind_t> input_bind_map {};
    std::unordered_map<std::string, bool> input_map {};
    std::array<char[8], 7> special_keys {};

    std::vector<std::string> special_keys_unit_pressed {};
    std::vector<std::string> double_click_mouse_buttons_pressed {};
    std::vector<std::string> input_released_list {};
    std::vector<std::string> just_fired_input_bind {};

    bool finger_hold_event {};
    bool finger_swipe_event {};
    bool is_special_keys_released {};

    ekg::timing_t double_interact {};
    ekg::timing_t last_time_wheel_was_fired {};
  public:
    ekg::input_t input {};
  protected:
    void complete_with_units(
      std::string &string_builder,
      std::string_view key_name
    );

    bool contains_unit(
      std::string_view label
    );

    bool is_special_key(
      int32_t sdl_key_code
    );
  public:
    void init();

    void quit();

    void on_event();

    void on_update();

    void insert_input_bind(
      std::string_view tag,
      std::string_view input
    );

    void erase_input_bind(
      std::string_view tag,
      std::string_view input_to_erase
    );

    void erase_input_bind(
      std::string_view tag
    );

    void set_input_bind_state(
      std::string_view tag
    );

    bool get_input_bind_state(
      std::string_view tag
    );

    void set_input_state(
      std::string_view input,
      bool state
    );

    bool get_input_state(
      std::string_view tag
    );
  };
}

#endif
