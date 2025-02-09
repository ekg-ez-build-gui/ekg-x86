#include "ekg/layout/dimension.hpp"

float ekg::layout::estimate_height(
  ekg::ui::abstract_widget *p_parent_widget
) {
  if (p_parent_widget == nullptr) {
    return 0.0f;
  }

  ekg::ui::abstract_widget *p_widgets {};
  ekg::flags flags {};
  float total_height {};
  float height {};

  for (int32_t &ids : p_parent_widget->p_data->get_child_id_list()) {
    if (ids == 0 || (p_widgets = ekg::core->get_fast_widget_by_id(ids)) == nullptr) {
      continue;
    }

    p_widgets->on_reload();

    flags = p_widgets->p_data->get_place_dock();
    height = p_widgets->dimension.h;

    if (p_widgets->p_data->has_children()) {
      height = ekg::layout::height(p_widgets); 
    }

    total_height += (
      height
      *
      (ekg_equals_float(total_height, 0.0f) || ekg_bitwise_contains(flags, ekg::dock::next))
      +
      ekg::layout::offset
    );
  }

  total_height += (
    ekg::layout::offset // top
    +
    ekg::layout::offset // top
    +
    ekg::layout::offset // bottom
    +
    ekg::layout::offset // bottom
  );
  return total_height;
}
