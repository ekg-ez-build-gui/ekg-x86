#include "ekg/layout/dimension.hpp"
#include "ekg/ekg.hpp"

float ekg::layout::estimate_height(
  ekg::ui::abstract *p_parent_widget
) {
  if (
      p_parent_widget == nullptr
      ||
      !p_parent_widget->properties.is_docknizable
    ) {
    return 0.0f;
  }

  ekg::theme_t &current_global_theme {ekg::p_core->service_theme.get_current_theme()};
  ekg::ui::abstract *p_widgets {};
  ekg::flags_t flags {};

  float total_height {};
  float height {};

  for (ekg::properites_t *&p_properties : p_widget_parent->properties.children) {
    if (
        p_properties == nullptr
        ||
        p_properties->p_widget == nullptr
      ) {
      continue;
    }

    p_widgets = static_cast<ekg::ui::abstract*>(p_properties);
    p_widgets->on_reload();

    flags = p_widgets->properties.dock;
    height = p_widgets->rect.h;

    if (
        p_widgets->properties.is_docknizable
        &&
        p_widgets->properties.children.empty()
      ) {
      height = ekg::layout::estimate_height(p_widgets); 
    }

    total_height += (
      height
      *
      (
        ekg::fequalsf(total_height, 0.0f)
        ||
        ekg::has(flags, ekg::dock::next)
      )
      +
      current_global_theme.layout_offset
    );
  }

  total_height += (
    current_global_theme.layout_offset // top
    +
    current_global_theme.layout_offset // top
    +
    current_global_theme.layout_offset // bottom
    +
    current_global_theme.layout_offset // bottom
  );

  return total_height;
}
