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

#include "ekg/ui/checkbox/ui_checkbox_widget.hpp"
#include "ekg/ekg.hpp"
#include "ekg/draw/draw.hpp"

void ekg::ui::checkbox_widget::on_reload() {
  abstract_widget::on_reload();

  ekg::ui::checkbox *p_ui {(ekg::ui::checkbox *) this->p_data};
  ekg::rect &rect {this->get_abs_rect()};
  ekg::draw::font_renderer &f_renderer {ekg::f_renderer(p_ui->get_font_size())};
  int32_t scaled_height {p_ui->get_scaled_height()};

  float text_width {f_renderer.get_text_width(p_ui->get_text())};
  float text_height {f_renderer.get_text_height()};
  float box_size {text_height};

  float dimension_offset = text_height / 2;
  float offset {ekg::find_min_offset(text_width, dimension_offset)};
  float dimension_height {(text_height + dimension_offset) * static_cast<float>(scaled_height)};

  if (p_ui->is_auto_initial_dimension()) {
    this->dimension.w = ekg_min(this->dimension.w, text_width);
    this->min_size.x = ekg_min(this->min_size.x, text_height);
    p_ui->set_auto_initial_dimension(false);
  }

  this->dimension.h = dimension_height;
  this->min_size.y = ekg_min(this->min_size.y, dimension_height);

  this->rect_box.w = box_size;
  this->rect_box.h = box_size;

  this->rect_text.w = text_width;
  this->rect_text.h = text_height;

  ekg::layout::mask &mask {ekg::core->mask};
  mask.preset({offset, offset, dimension_height}, ekg::axis::horizontal, this->dimension.w);
  mask.insert({&this->rect_box, p_ui->get_box_align()});
  mask.insert({&this->rect_text, p_ui->get_text_align()});
  mask.docknize();

  ekg::rect &layout_mask {mask.get_rect()};
  this->dimension.h = ekg_min(this->dimension.h, layout_mask.h);
}

void ekg::ui::checkbox_widget::on_event(ekg::os::io_event_serial &io_event_serial) {
  bool pressed {ekg::input::pressed()};
  bool released {ekg::input::released()};
  bool motion {ekg::input::motion()};

  if (motion || pressed || released) {
    ekg::set(this->flag.highlight, this->flag.hovered);
    ekg::set(
      this->flag.focused,
      (
        this->flag.hovered &&
        ekg::rect_collide_vec(this->rect_box + (this->dimension + *this->p_parent), ekg::input::interact())
      )
    );

    ekg_action_dispatch(
      motion && this->flag.hovered && ekg::timing::second > ekg::ui::latency,
      ekg::action::motion
    );
  }

  if (pressed && !this->flag.activity && this->flag.hovered && ekg::input::action("checkbox-activity")) {
    ekg_action_dispatch(
      true,
      ekg::action::press
    );

    ekg::set(this->flag.activity, true);
  } else if (released && this->flag.activity) {
    ekg_action_dispatch(
      this->flag.hovered,
      ekg::action::release
    );

    if (this->flag.hovered) {
      ekg_action_dispatch(
        true,
        ekg::action::activity
      );

      ekg::ui::checkbox *p_ui {(ekg::ui::checkbox *) this->p_data};
      if (p_ui->set_value(!p_ui->get_value())) {
        ekg::reload(this);
      }
    }

    ekg::set(this->flag.activity, false);
  }
}

void ekg::ui::checkbox_widget::on_draw_refresh() {
  ekg::ui::checkbox *p_ui {(ekg::ui::checkbox *) this->p_data};
  ekg::rect &rect {this->get_abs_rect()};
  ekg::service::theme_scheme_t &theme_scheme {ekg::current_theme_scheme()};
  ekg::draw::font_renderer &f_renderer {ekg::f_renderer(p_ui->get_font_size())};
  ekg::rect box {this->rect_box + rect};

  if (p_ui->was_changed()) {
    ekg::reload(this);
  }

  ekg::draw::sync_scissor(this->scissor, rect, this->p_parent_scissor);
  ekg_draw_assert_scissor();

  ekg::draw::rect(
    rect,
    theme_scheme.checkbox_background,
    ekg::draw_mode::filled,
    ekg_layer(ekg::layer::background)
  );

  if (this->flag.highlight) {
    ekg::draw::rect(
      rect,
      theme_scheme.checkbox_highlight,
      ekg::draw_mode::filled,
      ekg_layer(ekg::layer::highlight)
    );
  }

  if (this->flag.focused && !p_ui->get_value()) {
    ekg::draw::rect(
      box,
      theme_scheme.checkbox_highlight,
      ekg::draw_mode::filled,
      ekg_layer(ekg::layer::highlight)
    );
  }

  ekg::draw::rect(
    box,
    theme_scheme.checkbox_highlight,
    ekg::draw_mode::filled,
    ekg_layer(ekg::layer::highlight)
  );

  if (this->flag.activity) {
    ekg::draw::rect(
      this->flag.focused ? box : rect,
      theme_scheme.checkbox_activity,
      ekg::draw_mode::filled,
      ekg_layer(ekg::layer::activity)
    );
  }

  if (p_ui->get_value()) {
    ekg::draw::rect(
      box,
      theme_scheme.checkbox_activity,
      ekg::draw_mode::filled,
      ekg_layer(ekg::layer::activity)
    );
  }

  if (p_ui->get_value() && this->flag.highlight) {
    ekg::draw::rect(
      box,
      ekg::vec4 {theme_scheme.checkbox_activity.x, theme_scheme.checkbox_activity.y, theme_scheme.checkbox_activity.z, theme_scheme.checkbox_outline.w},
      ekg::draw_mode::outline
    );
  }

  f_renderer.blit(
    p_ui->get_text(),
    rect.x + this->rect_text.x,
    rect.y + this->rect_text.y,
    theme_scheme.checkbox_string
  );

  ekg::draw::rect(
    rect,
    theme_scheme.checkbox_outline,
    ekg::draw_mode::outline
  );
}