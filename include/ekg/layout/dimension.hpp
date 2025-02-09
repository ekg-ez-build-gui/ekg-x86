#ifndef EKG_LAYOUT_DIMENSION_HPP
#define EKG_LAYOUT_DIMENSION_HPP

namespace ekg {
  /**
   * Estimate height from a container children list.
   * Note: Recursive.
   **/
  float estimate_height(
    ekg::ui::abstract_widget *p_parent_widget
  );
}

#endif
