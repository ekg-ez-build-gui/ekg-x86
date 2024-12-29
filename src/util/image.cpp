#include "ekg/util/image.hpp"
#include <cstdint>

#include "ekg/util/io.hpp"

ekg::format_convert_result ekg::image_src_r8_convert_to_r8g8b8a8(
  FT_Vector size,
  const unsigned char *p_src,
  std::vector<unsigned char> &dst
) {
  if (size.x == 0 || size.y == 0 || p_src == nullptr || dst.empty()) {
    return ekg::format_convert_result::failed;
  }

  size_t index {};
  for (size_t it {}; it < (size.x * size.y); it++) {
    const unsigned char &char8_red_color {
      p_src[it]
    };

    index = it * 4 + 0; 

    if (index == dst.size()) {
      break;
    }

    dst.at(it * 4 + 0) = char8_red_color;
    dst.at(it * 4 + 1) = char8_red_color;
    dst.at(it * 4 + 2) = char8_red_color;
    dst.at(it * 4 + 3) = char8_red_color;
  }

  return ekg::format_convert_result::success;
}