#ifndef EKG_IO_ALGORITHM_HPP
#define EKG_IO_ALGORITHM_HPP

#include "ekg/io/memory.hpp"
#include "ekg/ui/stack.hpp"

#include <memory>
#include <string_view>

namespace ekg {
  ekg::flags add_child_to_parent(
    ekg::properties_t *p_parent_properties,
    ekg::properties_t *p_child_properties
  );

  ekg::properties_t *find(
    ekg::stack_t *p_stack,
    std::string_view widget_tag
  );

  ekg::flags destroy(
    ekg::stack_t *p_stack,
    ekg::properties_t *p_destroy_widget_properties
  );

  ekg::flags find_and_destroy(
    ekg::stack_t *p_stack,
    std::string_view widget_tag
  );
}

namespace ekg::io {
  struct target_collector_t {
  public:
    ekg::id unique_id {};
    std::vector<ekg::ui::abstract*> storage {};
    bool was_target_found {};
    size_t count {};
  };

  ekg::flags push_back_widget_tree_recursively(
    ekg::io::target_collector_t *p_target_collector,
    ekg::ui::abstract *p_widget
  );
}

#endif