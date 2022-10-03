#ifndef EKG_UTIL_ENV_H
#define EKG_UTIL_ENV_H

#include <iostream>

namespace ekg {
    enum class env {
        swap, refresh, reset, update, redraw
    };

    struct timing {
        uint64_t elapsed_ticks {};
        uint64_t current_ticks {};
        uint64_t ticks_going_on {};
    };

    void log(const std::string &log_message);
    bool reach(ekg::timing &timing, uint64_t ms);
    void reset(ekg::timing &timing);

    bool file_to_string(std::string &string_builder, const std::string &path);
    bool set(bool &var_mutable, bool predicate);
    std::string &set(std::string &var_mutable, const std::string &predicate);

    bool get_task_state(const std::string &action_tag);

    bool was_motion();
    bool was_released();
    bool was_pressed();
    bool was_wheel();
};

#endif