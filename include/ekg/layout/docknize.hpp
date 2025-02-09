#ifndef EKG_LAYOUT_DOCKNIZE_H
#define EKG_LAYOUT_DOCKNIZE_H

#include "ekg/ui/abstract/ui_abstract_widget.hpp"
#include "ekg/util/geometry.hpp"

/**
 * Returns the dimensional extent based in count and the offset (space between rects). 
 * 
 * The pixel imperfect issue was solved here...
 * For a long time I did not know what was going on with the pixels,
 * some solutions I used did not work, then I discovered that all the time
 * was this dimension extent with float imprecision loss.
 * 
 * Each pixels represent 1.0f, if the GPU receives pixels with
 * (n + f) `n` a non-floating point number and `f` a floating point;
 * the rasterizer will jump between pixels, resulting in pixel-imperfect.
 * 
 * The following formula make you understand:
 * ( (g - d) - (c * o) ) / c
 * 
 * g = group rect
 * d = dimensional extent
 * c = amount of widgets with fill property flag until any flag next
 * o = UI offset setting
 * 
 * Float-only without the int32_t cast may results in pixel-imperfect
 * due the influence of dimensional size of parent rect, font height, font width etc.
 * 
 * - Rina.
 **/
#define ekg_layout_get_dimensional_extent(dimension, extent, offset, count) \
( \
  (static_cast<int32_t>(dimension) - static_cast<int32_t>(extent) - static_cast<int32_t>(count * offset)) \
) / static_cast<int32_t>(count) \

/**
 * Pixel imperfection is a problem for UI widget placements, this macro provides
 * correction position for right/bottom based on left/top position.
 * 
 * It is important to understand that is impossible to remove all the pixel
 * imperfections, but there are ways to round it, as example, you can place widget
 * from a side (left or right, top or bottom) and align with an offset. That is how EKG fix it.
 * 
 * This method calculate the minimum possible position based on left until the pixel
 * escape for two or more offsets distance. Instead you use the container width directly
 * to calculate right widgets positions, EKG must use the left consistency to get the
 * real container (width/height) from the side of left (when using the container width directly
 * the position is pixel imperfect).
 * 
 * - Rina.
 **/
#define ekg_layout_get_pixel_perfect_position(side_a, side_b, container_dimension, offset) \
  ekg_min( \
    ( \
      (side_a + (container_dimension - side_a) + offset) \
      - \
      side_b \
    ), \
    side_a \
  ) \

namespace ekg::layout {
  class mask {
  protected:
    std::vector<ekg::rect_descriptor_t> rect_descriptor_list {};
    float respective_all {};
    float respective_center {};
    ekg::flags_t axis {};
    ekg::vec3_t<float> offset {};
    ekg::rect_t<float> mask {};
  public:
    void preset(
      ekg::vec3_t<float> offset,
      ekg::flags_t axis,
      float initial_respective_size = 0.0f
    );
    
    void insert(
      ekg::rect_descriptor_t rect_descriptor
    );

    void docknize();
    ekg::rect_t<float> &get_rect();
  };

  /**
   * A mid-functional feature to process dock position from widgets.
   * Note: Recursive.
   **/
  void docknize(
    ekg::ui::abstract_widget *p_parent_widget
  );
}

#endif
