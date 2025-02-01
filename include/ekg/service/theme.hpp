
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

#ifndef EKG_SERVICE_THEME_H
#define EKG_SERVICE_THEME_H

#include <vector>
#include <string_view>
#include <map>

#include "ekg/ui/button/button.hpp"
#include "ekg/ui/checkbox/checkbox.hpp"
#include "ekg/ui/combobox/combobox.hpp"
#include "ekg/ui/frame/frame.hpp"
#include "ekg/ui/label/label.hpp"
#include "ekg/ui/listbox/listbox.hpp"
#include "ekg/ui/menu/menu.hpp"
#include "ekg/ui/popup/popup.hpp"
#include "ekg/ui/scrollbar/scrollbar.hpp"
#include "ekg/ui/slider/slider.hpp"
#include "ekg/ui/textbox/textbox.hpp"

namespace ekg {
  struct theme_t {
  public:
    std::string_view name {};
    std::string_view author {};
    std::string_view description {};
    std::string_view path {};
  public:
    bool symmetric_layout {};
    float min_widget_size {5};

    ekg::button_theme_t button {};
    ekg::checkbox_theme_t checkbox {};
    ekg::combobox_theme_t combobox {};
    ekg::frame_theme_t frame {};
    ekg::label_theme_t label {};
    ekg::listbox_theme_t listbox {};
    ekg::menu_theme_t menu {};
    ekg::popup_theme_t popup {};
    ekg::scrollbar_theme_t scrollbar {};
    ekg::slider_theme_t slider {};
    ekg::textbox_theme_t textbox {};
  };
}

namespace ekg::service {
  class theme {
  protected:
    std::map<std::string_view, ekg::theme_t> theme_map {};
    ekg::theme_t current_theme {};
  public:
    /**
     * Initialize default themes (dark, light, pinky etc) and update global theme scheme.
     **/
    void init();

    void quit();

    /**
     * Returns all mapped schemes from theme service.
     * Note: Use property register/deregister methods; may be unsafe.
     **/
    std::map<std::string_view, ekg::theme_t> &get_theme_map();

    /**
     * Set the current theme global.
     * Note: You must set a registered theme.
     * 
     * Returns true if exists, else false.
     **/
    bool set_current_theme(std::string_view name);

    /**
     * Returns the current theme scheme global loaded.
     **/
    ekg::theme_t &get_current_theme();

    /**
     * Dynamic registry one theme scheme on memory, you must not repeat themes name.
     * Note: May you want save theme scheme in a file, use `ekg::service::theme::save`
     * method to do that.
     **/
    void add(ekg::theme_t theme);

    /**
     * Local save one theme scheme to a file.
     * You need to register a theme before use this.
     **/
    void save(std::string_view name, std::string_view path);

    /**
     * Read one theme scheme from a file and load it to memory; may replacing one
     * if already exists.
     **/
    void read(std::string_view path, ekg::theme_t *p_theme);
  };
}

#endif