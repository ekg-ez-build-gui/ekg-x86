#include "ekg/core/user_input_register.hpp"
#include "ekg/ekg.hpp"
#include "ekg/service/input.hpp"

void ekg::user_input_register() {
  ekg::input::bind("frame-drag-activity", "mouse-1");
  ekg::input::bind("frame-drag-activity", "finger-click");
  ekg::input::bind("frame-resize-activity", "mouse-1");
  ekg::input::bind("frame-resize-activity", "finger-click");

  ekg::input::bind("button-activity", "mouse-1");
  ekg::input::bind("button-activity", "finger-click");

  ekg::input::bind("checkbox-activity", "mouse-1");
  ekg::input::bind("checkbox-activity", "finger-click");

  ekg::input::bind("popup-activity", "mouse-1");
  ekg::input::bind("popup-activity", "finger-click");

  ekg::input::bind("textbox-activity", "mouse-1");
  ekg::input::bind("textbox-activity", "finger-click");
  ekg::input::bind("textbox-action-activity", "return");
  ekg::input::bind("textbox-action-activity", "keypad enter");

  ekg::input::bind("textbox-action-select-all", "lctrl+a");
  ekg::input::bind("textbox-action-select-all", "rctrl+a");

  ekg::input::bind("textbox-action-select-all-inline", "mouse-1");
  ekg::input::bind("textbox-action-select", "lshift");
  ekg::input::bind("textbox-action-select", "rshift");

  ekg::input::bind("textbox-action-select-word", "mouse-1-double");
  ekg::input::bind("textbox-action-select-word", "finger-hold");

  ekg::input::bind("textbox-action-delete-left", "abs-backspace");
  ekg::input::bind("textbox-action-delete-right", "abs-delete");
  ekg::input::bind("textbox-action-break-line", "return");
  ekg::input::bind("textbox-action-break-line", "keypad enter");
  ekg::input::bind("textbox-action-break-line", "lshift+return");
  ekg::input::bind("textbox-action-break-line", "rshift+return");
  ekg::input::bind("textbox-action-tab", "tab");
  ekg::input::bind("textbox-action-modifier", "lctrl");
  ekg::input::bind("textbox-action-modifier", "rctrl");

  ekg::input::bind("textbox-action-up", "abs-up");
  ekg::input::bind("textbox-action-down", "abs-down");
  ekg::input::bind("textbox-action-right", "abs-right");
  ekg::input::bind("textbox-action-left", "abs-left");

  ekg::input::bind("clipboard-copy", "lctrl+c");
  ekg::input::bind("clipboard-copy", "rctrl+c");
  ekg::input::bind("clipboard-copy", "copy");
  ekg::input::bind("clipboard-paste", "lctrl+v");
  ekg::input::bind("clipboard-paste", "rctrl+v");
  ekg::input::bind("clipboard-paste", "paste");
  ekg::input::bind("clipboard-cut", "lctrl+x");
  ekg::input::bind("clipboard-cut", "rctrl+x");
  ekg::input::bind("clipboard-cut", "cut");

  ekg::input::bind("listbox-activity-open", "mouse-1-double");
  ekg::input::bind("listbox-activity-open", "finger-hold");

  ekg::input::bind("listbox-activity-select", "mouse-1");
  ekg::input::bind("listbox-activity-select", "finger-click");
  ekg::input::bind("listbox-activity-select-many", "lctrl+mouse-1");
  ekg::input::bind("listbox-activity-select-many", "rctrl+mouse-1");

  ekg::input::bind("slider-drag-activity", "mouse-1");
  ekg::input::bind("slider-drag-activity", "finger-click");
  ekg::input::bind("slider-bar-increase", "mouse-wheel-up");
  ekg::input::bind("slider-bar-decrease", "mouse-wheel-down");
  ekg::input::bind("slider-bar-modifier", "lctrl");
  ekg::input::bind("slider-bar-modifier", "rctrl");

  ekg::input::bind("scrollbar-drag", "mouse-1");
  ekg::input::bind("scrollbar-drag", "finger-click");
  ekg::input::bind("scrollbar-scroll", "mouse-wheel");
  ekg::input::bind("scrollbar-scroll", "finger-swipe");

  // I think it is a severe problem with EKG input-bind system, but I will fix soon.
  ekg::input::bind("scrollbar-scroll", "lshift+mouse-wheel");
  ekg::input::bind("scrollbar-scroll", "rshift+mouse-wheel");
  ekg::input::bind("scrollbar-horizontal-scroll", "lshift+mouse-wheel");
  ekg::input::bind("scrollbar-horizontal-scroll", "rshift+mouse-wheel");
}