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
#include <memory>
#include <typeinfo>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#include "geometry.hpp"
#include "ekg/ui/display.hpp"

#define ekg_bitwise_contains(flags, flag) (flags & flag)
#define ekg_bitwise_remove(flags, flag)   (flags &= ~flag)
#define ekg_bitwise_add(flags, flag)      (flags |= flag)

/**
 * LOG NO-TRACE dev-case reserved.
 * Note: this macro call console out (`std::cout`) directly, not safe for official log trace.
 **/
#define ekg_log(msg) std::cout << msg << std::endl;
#define ekg_log_assert(assert, msg) if (!assert) ekg_log(msg);

namespace ekg {
  typedef uint32_t flags;
  typedef uint32_t id;
}

namespace ekg {
  class item : public std::vector<ekg::item> {
  protected:
    std::string value {};
    ekg::flags attr_bits {};
    ekg::placement placement {};
    uint64_t visible_count {};
    bool *p_semaphore {nullptr};
    ekg::item *p_addressed {};
  public:
    explicit item() = default;

    item(
      std::string_view insert_value,
      ekg::flags insert_attr_bits = 0
    );

    item(
      std::string_view insert_value,
      std::vector<ekg::item> insert_item_list,
      ekg::flags insert_attr_bits = 0
    );

    /**
     * Idk my brain buuuobmbobm obmormobmmbrmfdf
     **/
    item(std::initializer_list<ekg::item> __l) : std::vector<ekg::item>(__l) {};

    ~item();
  public:
    /**
     * Set the addressed item which point to the primordial item.
     * Unsafe due the nullptr crash possibility.
     **/
    void unsafe_set_addressed(ekg::item *p_set_addressed);

    /**
     * Get the primordial item, used in rendering cache.
     **/
    ekg::item *unsafe_get_addressed();

    /**
     * Set the semaphore used in rendering-cache, to invoke changes on
     * widget rendering.
     * Unsafe by the performance directly effect.
     **/
    void unsafe_set_semaphore(bool *p_set_semaphore);
    
    /**
     * Get the item-placement used for rendering.
     * Unsafe due the return as reference, bypassing the rendering states.
     **/
    ekg::placement &unsafe_get_placement();

    /**
     * Set the visible count used in rendering cache.
     * Unsafe due the performanceless if visible count is higher than the truly-visible.
     **/
    void unsafe_set_visible_count(uint64_t count);
  public:
    void set_value(std::string_view new_value);

    std::string get_value();
  
    void set_attr(ekg::flags bits);

    ekg::flags get_attr();

    void set_text_align(ekg::flags dock);

    ekg::flags get_text_align();

    /**
     * Get the amount of visible items.
     * Note: Reserved purpose-only for rendering cache.
     **/
    uint64_t get_visible_count();
  };

  /**
   * Number range types, used in specific-features.
   **/
  enum class number {
    float64,
    float32,
    int64,
    uint64,
    int32,
    uint32,
    int16,
    uint16,
    int8,
    uint8,
  };

  /**
   * Base class for specific-features.
   * Note: None.
   **/
  struct feature {};

  template<typename t>
  ekg::number retreive_number_type_from_var_type() {
    if (typeid(t) == typeid(double)) {
      return ekg::number::float64;
    } else if (typeid(t) == typeid(float)) {
      return ekg::number::float32;
    } else if (typeid(t) == typeid(int64_t)) {
      return ekg::number::int64;
    } else if (typeid(t) == typeid(uint64_t)) {
      return ekg::number::uint64;
    } else if (typeid(t) == typeid(int32_t)) {
      return ekg::number::int32;
    } else if (typeid(t) == typeid(uint32_t)) {
      return ekg::number::uint32;
    } else if (typeid(t) == typeid(int16_t)) {
      return ekg::number::int16;
    } else if (typeid(t) == typeid(uint16_t)) {
      return ekg::number::uint16;
    } else if (typeid(t) == typeid(int8_t)) {
      return ekg::number::int8;
    }

    // typeid(t) == typeid(uint8_t)
    return ekg::number::uint8;
  }

  template<typename t, typename s>
  struct value_t {
  protected:
    bool changed {};
    bool not_self_ownership {};
  protected:
    s *p_mommy_s {};
  public:
    t self {};
    t *p_value {nullptr};
  public:
    value_t<t, s>() {
      this->p_value = &this->self;
    };

    operator bool() {
      return this->changed;
    }
  public:
    void registry(s *p_mommy) {
      if (this->p_mommy_s == nullptr) {
        this->p_mommy_s = p_mommy;
      }
    }

    s *box(t val) {
      this->self = val;
      return this->p_mommy_s;
    }

    s *transfer_ownership(t *p_address) {
      this->p_value = p_address;
      this->not_self_ownership = true;
      return this->p_mommy_s;
    }

    s *reset_ownership() {
      this->p_value = &this->self;
      this->not_self_ownership = false;
      return this->p_mommy_s;
    }

    s *set_value(t val) {
      this->changed = *this->p_value != val;
      *this->p_value = val;

      if (this->changed) {
        ekg::ui::redraw = true;
      }

      return this->p_mommy_s;
    }

    t &get_value() {
      return *this->p_value;
    }

    bool was_changed() {
      bool was {this->changed};
      this->changed = false;
      return was;
    }

    bool was_not_ownership_transfered() {
      return this->not_self_ownership;
    }

    void align_ownership_mem_if_necessary() {
      if (!this->not_self_ownership) {
        this->p_value = &this->self;
      }
    }
  };

  enum attr {
    disabled  = 2 << 1,
    focused   = 2 << 2,
    hovering  = 2 << 3,
    locked    = 2 << 4,
    opened    = 2 << 5,

    contains_separator = 2 << 6,
    contains_icon      = 2 << 7,

    // legacy compatibility
    separator = 2 << 7
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
    bool was_hovered {};
    bool scrolling {};
  };

  /**
   * Deprecated!
   *
   **/
  enum class clipboard {
    copy = 1073741948, // SDLK_COPY
    cut = 1073741947, // SDLK_CUT
    paste = 1073741949  // SDLK_PASTE
  };
}

namespace ekg {
  bool file_to_string(std::string &file_content, std::string_view path);

  bool reach(ekg::timing &timing, int64_t ms);

  bool reset(ekg::timing &timing);

  bool extend(ekg::timing &timing, int64_t ms);

  int64_t interval(ekg::timing &timing);
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
