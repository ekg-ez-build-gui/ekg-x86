#include "ekg/core/runtime.hpp"

void ekg::runtime::do_init() {
  ekg::log() << "Initializing task-handler service...";

  this->swap_target_collector->unique_id = ekg::memory::invalid_unique_id;
  this->handler.allocate() = new ekg::task_t {
    .info = ekg::info_t {
      .tag = "swap",
      .p_properties = nullptr,
      .p_data = nullptr
    },
    .function = [this](ekg::info_t &info) {
      if (this->swap_target_collector->unique_id == ekg::memory::invalid_unique_id) {
        return;
      }

      ekg::flags result {};
      std::vector<ekg::ui::abstract*> top_level_widget_list {};
      this->context_widget_list.clear();

      for (std::unique_ptr<ekg::ui::abstract> &p_widget : this->loaded_widget_list) {
        if (
            p_widget->properties.p_abs_parent != nullptr
            ||
            /**
             * TODO: check this right
             **/
            p_widget->properties.p_abs_parent->p_widget == nullptr
          ) {
          continue;
        }

        this->swap_target_collector.was_target_found = false;
        this->swap_target_collector.storage.clear();

        result = ekg::io::push_back_widget_tree_recursively(
          &target_collector,
          static_cast<ekg::ui::abstract*>(p_widget->properties.p_abs_parent->p_widget)
        );

        if (result != ekg::result::success) {
          continue;
        }

        if (this->swap_target_collector.was_target_found) {
          top_level_widget_list = storage; // idk i be brain dead
        } else {
          this->context_widget_list.insert(
            this->context_widget_list.end(),
            this->swap_target_collector.storage.begin(),
            this->swap_target_collector.storage.end()
          );
        }
      }
    }

    this->context_widget_list.insert(
      this->context_widget_list.end(),
      top_level_widget_list.begin(),
      top_level_widget_list.end()
    );

    this->swap_target_collector.storage.clear();
    this->swap_target_collector->unique_id = ekg::memory::invalid_unique_id;
  };
}

void ekg::runtime::do_quit() {

}

void ekg::runtime::do_update() {

}

void ekg::runtime::do_render() {

}

ekg::ui::abstract *ekg::runtime::push_back_new_widget_safety(
  ekg::ui::abstract *p_widget
) {
  return this->loaded_widget_list.emplace_back(
    std::unique_ptr<ekg::ui::abstract>(p_widget)
  ).get();
}

ekg::id ekg::runtime::generate_unique_id() {
  return ++this->global_id;
}