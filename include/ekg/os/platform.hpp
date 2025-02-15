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

#ifndef EKG_OS_PLATFORM_HPP
#define EKG_OS_PLATFORM_HPP

#include "ekg/io/input.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg::os {
  class platform {
  public:
    ekg::rect<int32_t> display_size {};
    ekg::system_cursor_type system_cursor {};
    ekg::io::serialized_input_event_t serialized_input_event {};
  public:
    virtual void init() {}
    virtual void quit() {}
    virtual void update_monitor_resolution() {}
    virtual void update_cursor() {}
    virtual void get_key_name(ekg::io::input_key_t &key, std::string &name) {}
    virtual void get_special_key(ekg::io::input_key_t &key, ekg::special_key_type special_key) {}
    virtual const char *get_clipboard_text() { return nullptr; };
    virtual void set_clipboard_text(const char *p_text) {};
    virtual bool has_clipboard_text() { return false; }
    virtual uint64_t get_ticks() { return 0; }
  };
}

#endif
