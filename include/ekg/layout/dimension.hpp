#ifndef EKG_LAYOUT_DIMENSION_HPP
#define EKG_LAYOUT_DIMENSION_HPP

#include "ekg/ui/abstract.hpp"

namespace ekg {
  /**
   * Estimate height from a container children list.
   * Note: Recursive.
   **/
  float estimate_docknizable_height(
    ekg::ui::abstract *p_parent_widget
  );
}

#endif
