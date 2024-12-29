#ifndef EKG_UTIL_IMAGE_HPP
#define EKG_UTIL_IMAGE_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

namespace ekg {
  enum format_convert_result {
    success,
    failed
  };

  ekg::format_convert_result image_src_r8_convert_to_r8g8b8a8(
    FT_Vector size,
    const unsigned char *p_src,
    std::vector<unsigned char> &dst
  );  
}

#endif