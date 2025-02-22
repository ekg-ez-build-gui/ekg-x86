#include "ekg/io/log.hpp"

int64_t ekg::timing_t::second {};
int64_t ekg::timing_t::ticks {};

bool ekg::reach(ekg::timing_t *p_timing, int64_t ms) {
  p_timing->ticks_going_on = ekg::timing_t::ticks;
  p_timing->current_ticks = p_timing->ticks_going_on - p_timing->elapsed_ticks;
  return p_timing->current_ticks > ms;
}

bool ekg::reset(ekg::timing_t *p_timing) {
  p_timing->elapsed_ticks = p_timing->ticks_going_on;
  return true;
}

bool ekg::extend(ekg::timing_t *p_timing, int64_t ms) {
  p_timing->elapsed_ticks = p_timing->ticks_going_on - ms;
  return true;
}

int64_t ekg::interval(ekg::timing_t *p_timing) {
  p_timing->ticks_going_on = ekg::timing_t::ticks;
  return (
    p_timing->current_ticks = p_timing->ticks_going_on - p_timing->elapsed_ticks
  );
}
