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

#ifndef EKG_UI_LISTBOX_WIDGET_H
#define EKG_UI_LISTBOX_WIDGET_H

#include "ekg/ui/abstract/ui_abstract_widget.hpp"
#include "ekg/util/io.hpp"
#include "ekg/ui/scrollbar/ui_scrollbar_embedded_widget.hpp"
#include "ekg/ui/listbox/ui_listbox.hpp"

namespace ekg::ui {
  class listbox_widget : public ekg::ui::abstract_widget {
  public:
    ekg::ui::scrollbar_embedded_widget embedded_scroll {};
    bool was_selected {};
    ekg::item item_rendering_cache {};
    ekg::rect rect_content_abs {};
    ekg::rect rect_content_place {};
    float column_header_height {};
    bool must_update_items {};
  public:
    void on_create() override;

    void on_reload() override;

    void on_update() override;

    void on_post_event(ekg::os::io_event_serial &io_event_serial) override;
    
    void on_event(ekg::os::io_event_serial &io_event_serial) override;

    void on_pre_event(ekg::os::io_event_serial &io_event_serial) override;

    void on_draw_refresh() override;
  };
}

namespace ekg::ui {
  void listbox_template_reload(
    ekg::item &parent,
    ekg::rect &ui_rect,
    ekg::rect &header_rect,
    ekg::font &item_font,
    ekg::rect &relative_rect,
    int32_t item_scaled_height,
    uint64_t header_index,
    uint64_t &arbitrary_index_pos,
    bool &opened,
    ekg::mode mode,
    bool *p_semaphore
  );

  void listbox_template_on_event(
    ekg::os::io_event_serial &io_event_serial,
    uint64_t &arbitrary_index_pos,
    ekg::item &rendering_cache,
    bool motion,
    bool released,
    bool pressed_select_many,
    bool pressed_select,
    bool pressed_open,
    bool &was_selected,
    ekg::item &parent,
    ekg::vec2 &ui_pos,
    ekg::rect &ui_rect,
    ekg::rect &relative_rect,
    ekg::mode mode
  );
}

#endif
