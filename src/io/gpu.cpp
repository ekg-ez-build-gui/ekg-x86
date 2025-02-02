#include "ekg/ekg.hpp"

ekg::flags_t ekg::gpu_allocate_sampler(
  ekg::sampler_allocate_info_t *p_sampler_allocate_info,
  ekg::sampler_t *p_sampler
) {
  return ekg::core->p_gpu_api->gpu_allocate_sampler(
    p_sampler_allocate_info,
    p_sampler
  );
}

ekg::flags_t ekg::gpu_fill_sampler(
  ekg::sampler_fill_info_t *p_sampler_fill_info,
  ekg::sampler_t *p_sampler
) {
  return ekg::core->p_gpu_api->fill_sampler(
    p_sampler_fill_info,
    p_sampler
  );
}