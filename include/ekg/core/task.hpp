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

#ifndef EKG_CORE_TASK_H
#define EKG_CORE_TASK_H

#include <iostream>
#include <functional>
#include <cstdint>
#include <string_view>

namespace ekg {
  extern bool poll_io_event;

  enum event_type {
    unique = 0,
    alloc = 2,
    allocated = 4,
    shared = 8
  };

  struct info {
  public:
    std::string_view tag {};
    void *p_ui {};
    void *p_data {};
  };

  struct task {
  public:
    ekg::info info {};
    std::function<void(ekg::info&)> function {};
    bool is_dispatched {};
    bool unsafe_is_allocated {};
  public:
    /**
     * EKG free tasks automatically by mapping `new` keywords calls.
     * User-side is off from memory management when direct explicity invoke `new`,
     * as behavior of allocating randomly and not freeing after the widget
     * was destroyed --- EKG must make memory safe as maximum possible
     * in the case of bad management from user-side.
     * 
     * A good practice for using EKG tasks is not using new, but addressing
     * stacked tasks.
     * 
     * ```c++
     * class/struct your_gui_scene {
     * public:
     *  ekg::task meow_task; // <- stacked
     * public:
     *  your_gui_scene() {
     *    meow_task = ekg::task {
     *      // values go here
     *    };
     *  }
     * };
     * ```
     * 
     * The main point is using `ekg::task` in stack variables
     * and not direct allocating using `new` if you want address
     * in different(s) widgets.
     * 
     * As option, you can disable EKG tasks memory managment by setting
     * `EKG_MEMORY_MUST_FREE_TASKS_AUTOMATICALLY` to false,
     * so EKG will not care about all of this.
     **/
    void *operator new (size_t mem_alloc_size) {
      ekg::task *p_instance {
        (ekg::task*)
        ::operator new(mem_alloc_size)
      };
      
      p_instance->unsafe_is_allocated = true;
      return p_instance;
    }
  };
}

#endif