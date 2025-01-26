#include "ekg/io/input.hpp"
#include "ekg/ekg.hpp"

bool ekg::fire(std::string_view tag) {
  return ekg::core->service_input.get_input_bind_state(tag);
}

bool ekg::input(std::string_view input) {
  return ekg::core->service_input.get_input_state(input);
}

void ekg::bind(std::string_view tag, std::string_view input) {
  ekg::core->service_input.register_input_bind(tag, input);
}

void ekg::bind(std::string_view tag, std::vector<std::string_view> inputs) {
  for (std::string_view &input : inputs) {
    ekg::bind(tag, inputs);
  }
}

void ekg::io::register_standard_input_bindings() {
  ekg::bind("frame-drag-activity", "mouse-1");
  ekg::bind("frame-drag-activity", "finger-click");
  ekg::bind("frame-resize-activity", "mouse-1");
  ekg::bind("frame-resize-activity", "finger-click");

  ekg::bind("button-activity", "mouse-1");
  ekg::bind("button-activity", "finger-click");

  ekg::bind("checkbox-activity", "mouse-1");
  ekg::bind("checkbox-activity", "finger-click");

  ekg::bind("popup-activity", "mouse-1");
  ekg::bind("popup-activity", "finger-click");

  ekg::bind("textbox-activity", "mouse-1");
  ekg::bind("textbox-activity", "finger-click");
  ekg::bind("textbox-action-activity", "return");
  ekg::bind("textbox-action-activity", "keypad enter");

  ekg::bind("textbox-action-select-all", "lctrl+a");
  ekg::bind("textbox-action-select-all", "rctrl+a");

  ekg::bind("textbox-action-select-all-inline", "mouse-1");
  ekg::bind("textbox-action-select", "lshift");
  ekg::bind("textbox-action-select", "rshift");

  ekg::bind("textbox-action-select-word", "mouse-1-double");
  ekg::bind("textbox-action-select-word", "finger-hold");

  ekg::bind("textbox-action-delete-left", "abs-backspace");
  ekg::bind("textbox-action-delete-right", "abs-delete");
  ekg::bind("textbox-action-break-line", "return");
  ekg::bind("textbox-action-break-line", "keypad enter");
  ekg::bind("textbox-action-break-line", "lshift+return");
  ekg::bind("textbox-action-break-line", "rshift+return");
  ekg::bind("textbox-action-tab", "tab");
  ekg::bind("textbox-action-modifier", "lctrl");
  ekg::bind("textbox-action-modifier", "rctrl");

  ekg::bind("textbox-action-up", "abs-up");
  ekg::bind("textbox-action-down", "abs-down");
  ekg::bind("textbox-action-right", "abs-right");
  ekg::bind("textbox-action-left", "abs-left");

  ekg::bind("clipboard-copy", "lctrl+c");
  ekg::bind("clipboard-copy", "rctrl+c");
  ekg::bind("clipboard-copy", "copy");
  ekg::bind("clipboard-paste", "lctrl+v");
  ekg::bind("clipboard-paste", "rctrl+v");
  ekg::bind("clipboard-paste", "paste");
  ekg::bind("clipboard-cut", "lctrl+x");
  ekg::bind("clipboard-cut", "rctrl+x");
  ekg::bind("clipboard-cut", "cut");

  ekg::bind("listbox-activity-open", "mouse-1-double");
  ekg::bind("listbox-activity-open", "finger-hold");

  ekg::bind("listbox-activity-select", "mouse-1");
  ekg::bind("listbox-activity-select", "finger-click");
  ekg::bind("listbox-activity-select-many", "lctrl+mouse-1");
  ekg::bind("listbox-activity-select-many", "rctrl+mouse-1");

  ekg::bind("slider-drag-activity", "mouse-1");
  ekg::bind("slider-drag-activity", "finger-click");
  ekg::bind("slider-bar-increase", "mouse-wheel-up");
  ekg::bind("slider-bar-decrease", "mouse-wheel-down");
  ekg::bind("slider-bar-modifier", "lctrl");
  ekg::bind("slider-bar-modifier", "rctrl");

  ekg::bind("scrollbar-drag", "mouse-1");
  ekg::bind("scrollbar-drag", "finger-click");
  ekg::bind("scrollbar-scroll", "mouse-wheel");
  ekg::bind("scrollbar-scroll", "finger-swipe");

  // I think it is a severe problem with EKG input-bind system, but I will fix soon.
  ekg::bind("scrollbar-scroll", "lshift+mouse-wheel");
  ekg::bind("scrollbar-scroll", "rshift+mouse-wheel");
  ekg::bind("scrollbar-horizontal-scroll", "lshift+mouse-wheel");
  ekg::bind("scrollbar-horizontal-scroll", "rshift+mouse-wheel");
}