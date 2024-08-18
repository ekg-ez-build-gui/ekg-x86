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

#ifndef EKG_UI_SLIDER_WIDGET_H
#define EKG_UI_SLIDER_WIDGET_H

#include "ekg/ui/abstract/ui_abstract_widget.hpp"

namespace ekg::ui {
  class slider_widget : public ekg::ui::abstract_widget {
  public:
    struct range {
    public:
      ekg::rect rect {};
      ekg::vec2 target {};
    };
  public:
    std::vector<ekg::ui::slider_widget::range> range_list {};
    uint64_t targetted_range_index {UINT64_MAX};
  public:
    void on_reload() override;

    void on_pre_event(ekg::os::io_event_serial &io_event_serial) override;

    void on_event(ekg::os::io_event_serial &io_event_serial) override;

    void on_draw_refresh() override;
  };
}

namespace ekg::ui {
  uint64_t slider_widget_get_range_count(ekg::feature *&p_feature, ekg::number number);
}

#endif