#ifndef EKG_MATH_FLOATING_POINT_HPP
#define EKG_MATH_FLOATING_POINT_HPP

#include <cfloat>

namespace ekg {
  constexpr float pi {3.141592653589793238462643383279502884f};
  constexpr float pixel {1.000000f};
  constexpr float pixel_div_2 {0.500000f};

  constexpr bool fequalsf(float compare, float compared) {
    return (
      fabsf(compare - compared)
      <=
      (
        FLT_EPSILON
        *
        fmaxf(1.0f, fmaxf(fabsf(compare), fabsf(compared)))
      )
    );
  }

  template<typename t>
  constexpr float min_clamp(t a, t b) {
    return a < b ? b : a;
  }

  template<typename t>
  constexpr float max_clamp(t a, t b) {
    return a > b ? b : a;
  }

  template<typename t>
  constexpr float clamp(t a, t b, t c) {
    return ekg::min_clamp(ekg::max_clamp(a, c), b);
  }
}

#endif
