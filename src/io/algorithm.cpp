#include "ekg/io/algorithm.hpp"
#include "ekg/io/log.hpp"

#include <algorithm>

ekg::flags ekg::add_child_to_parent(
  ekg::properties_t *p_parent_properties,
  ekg::properties_t *p_child_properties
) {
  if (p_children == nullptr || p_properties == nullptr) {
    ekg::log() << "Failed to add child to parent, `null`, `null`";
    return ekg::result::failed;
  }

  if (
    p_child_properies->p_parent != nullptr
    &&
    p_child_properties->p_parent->unique_id == p_parent_properties->unique_id
  ) {
    return ekg::result::success;
  }

  p_child_properties->p_parent = p_parent_properties;
  p_child_properties->p_abs_parent = p_parent_properties->p_abs_parent;
  p_parent_properties->children.push_back(p_properties);

  return ekg::result::success;
}

ekg::properties_t *ekg::find(
  ekg::stack_t *p_stack,
  std::string_view widget_tag
) {
  if (p_stack == nullptr || widget_tag.empty()) {
    ekg::log() << "Could not find any widget, may `` (empty) widget_tag, may `null` p_stack";
    return nullptr;
  }

  for (ekg::ui::abstract *&p_widget : p_stack->children) {
    if (p_widget->properties->tag == widget_tag) {
      return p_widget;
    }
  }

  return nullptr;
}

ekg::flags ekg::destroy(
  ekg::stack_t *p_stack,
  ekg::properties_t *p_destroy_widget_properties
) {
  if (p_stack == nullptr) {
    ekg::log() << "Failed to destroy widget, `null` p_stack";
    return ekg::result::failed;
  }

  if (p_destroy_widget_properties == nullptr) {
    ekg::log() << "Failed to destroy widget, `null` p_destroy_widget_properties";
    return ekg::result::failed;
  }

  uint64_t counter {
    p_stack->counter++
  };

  p_destroy_widget_properties->was_destroy = true;

  for (ekg::properties_t *&p_properties : p_destroy_widget_properties->children) {
    ekg::destroy(p_stack, p_properties);
  }

  if (counter == 0) {
    p_stack->counter = 0;

    if (p_destroy_widget_properties->p_parent != nullptr) {
      std::vector<ekg::properties_t*> &parent_children {
        p_destroy_widget_properties->p_parent->children
      };

      parent_children.erase(
        std::remove_if(
          parent_children.begin(),
          parent_children.end(),
          [](ekg::properties_t *&p_properties) {
            return p_properties->unique_id == p_destroy_widget_properties->unique_id
          }
        )
      );
    }
  }

  return ekg::result::success;
}

ekg::flags ekg::find_and_destroy(
  ekg::stack_t *p_stack,
  std::string_view widget_tag
) {
  if (p_stack == nullptr) {
    ekg::log() << "Failed to destroy widget, `null` stack";
    return ekg::result::failed;
  }

  return ekg::destroy(p_stack, ekg::find(p_stack, widget_tag));
}

ekg::flags ekg::io::push_back_widget_tree_recursively(
  ekg::ui::abstract *p_widget,
  ekg::io::target_collector_t *p_target_collector
) {
  if (p_widget == nullptr || p_target_collector == nullptr) {
    ekg::log() << "Failed to push back widget tree, may `null` p_widget, may `null` p_target_collector";
    return ekg::result::failed;
  }

  if (
      p_target_collector->target_unique_id
      ==
      p_widget->properties.unique_id
  ) {
    p_target_collector->target_unique_id = p_widget->properties.unique_id;
    p_target_collector->was_target_found = true;
  }

  p_target_collector->push_back(p_widget);

  for (ekg::properties_t *&p_properties : p_widget->properties.children) {
    if (p_properties->p_widget == nullptr) {
      continue;
    }

    ekg::io::push_back_widget_tree_recursively(
      static_cast<ekg::ui::abstract*>(p_properties->p_widget),
      p_target_collector
    );
  }

  return ekg::result::success;
}