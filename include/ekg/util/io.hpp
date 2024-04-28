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

#ifndef EKG_UTIL_IO_H
#define EKG_UTIL_IO_H

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <map>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#include "geometry.hpp"

#define ekg_bitwise_contains(flags, flag) (flags & flag)
#define ekg_bitwise_remove(flags, flag)   (flags &= ~flag)
#define ekg_bitwise_add(flags, flag)      (flags |= flag)

namespace ekg {
  class item : public std::vector<ekg::item> {
  protected:
    std::string value {};
    uint16_t attr_bits {};
  public:
    item(std::string_view insert_value);
    item(std::string_view insert_value, uint16_t insert_attr_bits);
    ~item();
  };

  struct log {
  public:
    static std::ostringstream buffer;
    static bool buffered;
    static bool tracked;
  public:
    static void flush() {
      if (ekg::log::buffered) {
        std::string p_log {ekg::log::buffer.str()};

        #if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_VERBOSE, "EKG", "%s", p_log.c_str());
        #else
        std::cout << p_log;
        #endif

        ekg::log::buffer = std::ostringstream {};
        ekg::log::buffered = false;
      }
    }

    template<typename t>
    static void trace(bool should, t trace, bool interrupt_runtime = false) {
      if (!should) {
        return;
      }

      std::cout << "\n[EKG-TRACE] " << trace << std::endl;
      if (interrupt_runtime) {
        std::terminate();
      }
    }

    explicit log() {
      ekg::log::buffered = true;
      ekg::log::buffer << "\n[EKG-INFO] ";
    }

    template<typename t>
    log &operator<<(const t &value) {
      ekg::log::buffer << value;
      return *this;
    }
  };

  /*
   * Attributes are used by item,
   * they can be dynamically converted by string symbols:
   * \t separator
   * \\ box
   * \1 category
   * \2 row
   * \3 row_member
   * \4 unselectable
   */
  enum attr {
    separator = 2 << 2,
    box = 2 << 3,
    category = 2 << 4,
    row = 2 << 5,
    row_member = 2 << 6,
    unselectable = 2 << 7
  };

  struct timing {
  public:
    /**
     * The 1 second counter in ms.
     */
    static int64_t second;

    /**
     * The total running ticks since the application was started.
     */
    static int64_t ticks;
  public:
    int64_t elapsed_ticks {};
    int64_t current_ticks {};
    int64_t ticks_going_on {};
  };

  struct flag {
    bool highlight {};
    bool hovered {};
    bool activity {};
    bool focused {};
    bool state {};
    bool extra_state {};
    bool absolute {};
  };

  enum class clipboard {
    copy = 1073741948, // SDLK_COPY
    cut = 1073741947, // SDLK_CUT
    paste = 1073741949  // SDLK_PASTE
  };
}

namespace ekg {
  bool file_to_string(std::string &file_content, std::string_view path);

  bool reach(ekg::timing &, uint64_t);

  bool reset(ekg::timing &);
}

namespace ekg::input {
  bool action(std::string_view action_key);

  bool receive(std::string_view input_key);

  void clipboard(ekg::clipboard clipboard_op);

  void fire(std::string_view action_key);

  void bind(std::string_view action_key, std::string_view input_key);

  void bind(std::string_view action_key, const std::vector<std::string_view> &bind_list);

  bool motion();

  bool released();

  bool pressed();

  bool wheel();

  bool typed();

  ekg::vec4 &interact();
}

#endif