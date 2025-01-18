#ifndef EKG_CORE_RUNTIME_HPP
#define EKG_CORE_RUNTIME_HPP

#include "ekg/ui/abstract.hpp"
#include "ekg/service/handler.hpp"

#include <memory>

namespace ekg {
  class runtime {
  public:
    std::vector<std::unique_ptr<ekg::ui::abstract>> loaded_widget_list {};
    ekg::id global_id {};

    std::vector<ekg::ui::abstract*> context_widget_list {};
    ekg::io::target_collector_t swap_target_collector {}; 
  public:
    ekg::service::handler handler {};
  public:
    void do_init();
    void do_quit();
    void do_update();
    void do_render();

    ekg::ui::abstract *push_back_new_widget_safety(
      ekg::ui::abstract *p_widget
    );

    ekg::id generate_unique_id();
  };
}

#endif