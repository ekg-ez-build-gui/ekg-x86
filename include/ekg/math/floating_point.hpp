#ifndef EKG_MATH_FLOATING_POINT_HPP
#define EKG_MATH_FLOATING_POINT_HPP

#include <cfloat>
#include <cmath>

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
}

#endif
