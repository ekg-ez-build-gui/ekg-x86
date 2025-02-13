#include "ekg/layout/docknize.hpp"
#include "ekg/ui/display.hpp"
#include "ekg/ekg.hpp"
#include "ekg/ui/frame/ui_frame_widget.hpp"
#include "ekg/layout/extentnize.hpp"

float ekg::layout::offset {2.0f};

void ekg::layout::mask::preset(
  ekg::vec3_t<float> mask_offset,
  ekg::axis mask_axis,
  float initial_respective_size
) {
  this->axis = mask_axis;
  this->offset = mask_offset;
  this->respective_all = initial_respective_size;

  ekg::layout::extent_t::v_rect_descriptor = {};
  ekg::layout::extent_t::h_rect_descriptor = {};
}

void ekg::layout::mask::insert(
  ekg::rect_descriptor_t rect_descriptor
) {
  if (ekg::has(rect_descriptor.flags, ekg::dock::none)) {
    rect_descriptor.p_rect->w = 0.0f;
    rect_descriptor.p_rect->h = 0.0f;
    return;
  }

  this->rect_descriptor_list.push_back(rect_descriptor);
}

void ekg::layout::mask::docknize() {
  int64_t count {};
  float dimensional_extent {};
  float rect_height {};
  float rect_width {};
  float dimension_width {};
  float dimension_height {};
  float dimension_bind {};

  bool is_left {};
  bool is_right {};
  bool is_top {};
  bool is_bottom {};
  bool is_not_bind {};
  bool is_bind_dimension_not_zero {};

  /**
   * All dock rects within left corner (ekg::dock::left)
   * starts from left to right.
   **/
  ekg::rect_t<float> left_corner {};

  /**
   * All dock rects within right corner (ekg::dock::right)
   * starts from right (dimension width based) to left.
   **/
  ekg::rect_t<float> right_corner {};

  /**
   * All dock rects within center left corner (ekg::dock::right & ekg::dock::center)
   * starts from right (dimension width divided by 2 based) to left.
   **/
  ekg::rect_t<float> center_left_corner {};

  /**
   * All dock rects within center right corner (ekg::dock::right & ekg::dock::center)
   * starts from left (dimension width divided by 2 based) to right.
   **/
  ekg::rect_t<float> center_right_corner {};

  switch (this->axis) {
  case ekg::axis::horizontal:
    if (this->rect_descriptor_list.empty()) {
      this->mask.w = this->respective_all;
      this->mask.h = this->offset.z;
      return;
    }

    this->mask.w = this->offset.x;
    this->mask.h = this->offset.z;

    dimension_width = this->respective_all;
    dimension_height = this->offset.z;

    left_corner.w += this->offset.x;
    right_corner.w += this->offset.x;

    center_left_corner.x = dimension_width / 2.0f;
    center_left_corner.w = this->offset.x;

    center_right_corner.x = dimension_width / 2.0f;
    center_right_corner.w = this->offset.x;

    for (uint64_t it {}; it < this->rect_descriptor_list.size(); it++) {
      ekg::rect_descriptor_t &rect_descriptor {this->rect_descriptor_list.at(it)};
      if (rect_descriptor.p_rect == nullptr) {
        continue;
      }

      is_left = ekg::has(rect_descriptor.flags, ekg::dock::left);
      is_right = ekg::has(rect_descriptor.flags, ekg::dock::right);
      is_bottom = ekg::has(rect_descriptor.flags, ekg::dock::bottom);
      is_top = ekg::has(rect_descriptor.flags, ekg::dock::top);
      is_not_bind = !ekg::has(rect_descriptor.flags, ekg::dock::bind);
      is_bind_dimension_not_zero = (dimension_bind > 0.0f);

      rect_width = rect_descriptor.p_rect->w;
      rect_height = rect_descriptor.p_rect->h;

      if (ekg::has(rect_descriptor.flags, ekg::dock::fill)) {
        count = 0;
        ekg::layout::extentnize_rect_descriptor(
          this->rect_descriptor_list,
          dimensional_extent,
          ekg::dock::fill,
          ekg::dock::none,
          count,
          ekg::axis::horizontal
        );

        rect_width = ekg::min_clamp(
          ekg::layout::transform_dimension_from_extent(
            this->respective_all,
            dimensional_extent,
            this->offset.x,
            count
          ),
          1.0f
        );
      }

      if (is_left) {
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + left_corner.w;
        rect_descriptor.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        rect_descriptor.p_rect->w = rect_width;
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + dimension_width - right_corner.w - rect_descriptor.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_left) {
        rect_descriptor.p_rect->w = rect_width;
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_left_corner.x - center_left_corner.w - rect_descriptor.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        center_left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_right_corner.x + center_right_corner.w;
        rect_descriptor.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        center_right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (rect_descriptor.flags == ekg::dock::center) {
        rect_descriptor.p_rect->w = rect_width;
        rect_descriptor.p_rect->x = (dimension_width / 2.0f) - (rect_descriptor.p_rect->w / 2.0f);
      }

      if (is_top | is_bottom) {
        rect_descriptor.p_rect->y = (
          is_top ? (this->offset.y) : (dimension_height - rect_height - this->offset.y)
        );
      } else {
        rect_descriptor.p_rect->y = (
          (dimension_height / 2.0f) - (rect_height / 2.0f)
        );
      }

      if (!is_not_bind && rect_descriptor.p_rect->w > dimension_bind) {
        dimension_bind = rect_descriptor.p_rect->w;
      } else if (is_not_bind) {
        dimension_bind = 0.0f;
      }

      this->mask.h = dimension_height;
    }

    this->mask.w = ekg::min_clamp(
      this->respective_all,
      this->mask.w
    );
    break;
  case ekg::axis::vertical:
    if (this->rect_descriptor_list.empty()) {
      this->mask.w = this->offset.z;
      this->mask.h = this->respective_all;
      return;
    }

    this->mask.w = this->offset.z;
    this->mask.h = this->offset.y;
    break;
  }

  this->rect_descriptor_list.clear();
}

ekg::rect_t<float> &ekg::layout::mask::get_rect() {
  return this->mask;
}

void ekg::layout::docknize_widget(
  ekg::ui::abstract *p_widget_parent
) {
  if (p_widget_parent == nullptr) {
    return;
  }

  ekg::type type {p_widget_parent->properties.type};
  bool is_group {type == ekg::type::frame};
  ekg::rect_t<float> &abs_parent_rect {p_widget_parent->get_abs_rect()};

  if (!is_group || abs_parent_rect.w == 0 || abs_parent_rect.h == 0) {
    return;
  }

  if (p_widget_parent->states.is_targeting_absolute_parent) {
    p_widget_parent->states.is_targeting_absolute_parent = false;

    if (p_widget_parent->properties.p_abs_parent) {
      ekg::layout::docknize_widget(static_cast<ekg::ui::abstract*>(p_widget_parent->properties.p_abs_parent));
      return;
    }
  }

  float initial_offset {};
 
  bool has_scroll_embedded {};
  bool is_vertical_enabled {};

  ekg::rect_t<float> container_rect {p_widget_parent->dimension};

  switch (type) {
    case ekg::type::frame: {
      ekg::service::theme_scheme_t &theme_scheme {ekg::current_theme_scheme()};
      ekg::ui::frame_widget *p_frame {(ekg::ui::frame_widget *) p_widget_parent};

      initial_offset = static_cast<float>(theme_scheme.scrollbar_pixel_thickness);
      has_scroll_embedded = p_frame->p_scroll_embedded != nullptr;

      if (has_scroll_embedded) {
        p_frame->p_scroll_embedded->check_axis_states();
        is_vertical_enabled = p_frame->p_scroll_embedded->is_vertical_enabled;
        initial_offset *= static_cast<float>(!theme_scheme.symmetric_layout);

        container_rect.w -= initial_offset * static_cast<float>(is_vertical_enabled);
        container_rect.h -= initial_offset * static_cast<float>(p_frame->p_scroll_embedded->is_horizontal_enabled);
      }

      initial_offset = static_cast<float>(theme_scheme.scrollbar_pixel_thickness) * static_cast<float>(theme_scheme.symmetric_layout);
      break;
    }

    default: {
      break;
    }
  }

  float container_size_offset {(initial_offset + ekg::layout::offset) * 2.0f};
  container_rect.w -= container_size_offset;
  container_rect.h -= container_size_offset;

  ekg::ui::abstract *p_widgets {};
  ekg::flags flags {};

  int64_t it {};
  float dimensional_extent {};
  int64_t count {};

  ekg::rect_t<float> parent_offset {ekg::layout::offset + initial_offset, ekg::layout::offset + initial_offset, 0.0f, 0.0f};
  ekg::rect_t<float> prev_widget_layout {};

  bool should_reload_widget {};
  bool should_estimate_extent {};
  float max_previous_height {};

  ekg::layout::extent_t::h_widget = {};
  ekg::layout::extent_t h_extent_backup {};
  ekg::layout::fill_align_t fill_align {};

  bool is_left {};
  bool is_right {};
  bool is_top {};
  bool is_bottom {};
  bool is_fill {};
  bool is_next {};

  ekg::rect_t<float> corner_top_left {parent_offset};
  ekg::rect_t<float> corner_top_right {0.0f, parent_offset.y, 0.0f, 0.0f};
  ekg::rect_t<float> corner_bottom_left {parent_offset.x, 0.0f, 0.0f, 0.0f};
  ekg::rect_t<float> corner_bottom_right {};

  float highest_top {};
  float highest_bottom {};

  for (ekg::properties_t *&p_properties : p_widget_parent->properties.children) {
    if (p_properties == nullptr || p_properties->p_widget == nullptr) {
      continue;
    }

    p_widgets = static_cast<ekg::ui::abstract>(p_properties);
    flags = p_properties->dock;

    // @TODO Prevent useless scrolling reload.
    p_widgets->on_reload();
    type = p_widgets->properties.type;

    if (type == ekg::type::scrollbar) {
      it++;
      continue;
    }

    should_estimate_extent = true;

    is_right  = ekg::has(flags, ekg::dock::right);
    is_left   = ekg::has(flags, ekg::dock::left) || !is_right;
    is_bottom = ekg::has(flags, ekg::dock::bottom);
    is_top    = ekg::has(flags, ekg::dock::top) || !is_bottom;
    is_fill   = ekg::has(flags, ekg::dock::fill);
    is_next   = ekg::has(flags, ekg::dock::next);

    if (is_fill) {
      count = it;
      ekg::layout::extentnize_widget(
        p_widget_parent,
        ekg::dock::fill,
        ekg::dock::next | (is_top ? ekg::dock::bottom : ekg::dock::top),
        ekg::axis::horizontal,
        fill_align,
        dimensional_extent,
        count
      );

      dimensional_extent = ekg::min_clamp(
        ekg::layout::transform_dimension_from_extent(
          container_rect.w,
          dimensional_extent,
          ekg::layout::offset,
          count
        ),
        p_widgets->min_size.x
      );

      p_widgets->rect.w = dimensional_extent;
      should_reload_widget = true;
      should_estimate_extent = false;
    }

    switch (flags & ekg::dock::bottom) {
    case ekg::dock::bottom:
      if (ekg::fequalsf(corner_bottom_right.y, 0.0f)) {
        corner_bottom_right.y += highest_bottom + ekg::layout::offset;
        corner_bottom_left.y = corner_bottom_right.y;
      }

      if (is_next && is_left) {
        corner_bottom_left.x = parent_offset.x;
        corner_bottom_right.x = 0.0f;

        corner_bottom_left.y += highest_bottom + ekg::layout::offset;
        corner_bottom_right.y = corner_bottom_left.y;
        highest_bottom = 0.0f;
      }

      if (is_left) {
        p_widgets->rect.x = corner_bottom_left.x;
        p_widgets->rect.y = (
          ekg::min_clamp(
            ekg::layout::transform_to_pixel_perfect_position(
              corner_top_right.y,
              corner_bottom_right.y,
              container_rect.h,
              ekg::layout::offset
            ),
            corner_top_right.y + ekg::layout::offset + highest_bottom
          )
        );

        corner_bottom_left.x += p_widgets->rect.w + ekg::layout::offset;
      }

      if (is_next && is_right) {
        corner_bottom_left.x = parent_offset.x;
        corner_bottom_right.x = 0.0f;

        corner_bottom_right.y += highest_bottom + ekg::layout::offset;
        corner_bottom_left.y = corner_bottom_right.y;
        highest_bottom = 0.0f;
      }

      if (is_right) {
        corner_bottom_right.x += p_widgets->rect.w;
        p_widgets->rect.x = (
          ekg::layout::transform_to_pixel_perfect_position(
            corner_bottom_left.x,
            corner_bottom_right.x,
            container_rect.w,
            ekg::layout::offset
          )
        );

        p_widgets->rect.y = (
          ekg::min_clamp(
            ekg::layout::transform_to_pixel_perfect_position(
              corner_top_right.y,
              corner_bottom_right.y,
              container_rect.h,
              ekg::layout::offset
            ),
            corner_top_right.y + ekg::layout::offset + highest_bottom
          )
        );

        corner_bottom_right.x += ekg::layout::offset;
      }

      highest_bottom = ekg::min_clamp(highest_bottom, p_widgets->rect.h);
      break;
    default:
      if (is_next && is_left) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_left.y += highest_top + ekg::layout::offset;
        corner_top_right.y = corner_top_left.y;
        highest_top = 0.0f;
      }

      if (is_left) {
        p_widgets->rect.x = corner_top_left.x;
        p_widgets->rect.y = corner_top_left.y;
  
        corner_top_left.x += p_widgets->rect.w + ekg::layout::offset;
      }

      if (is_next && is_right) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_right.y += highest_top + ekg::layout::offset;
        corner_top_left.y = corner_top_right.y;
        highest_top = 0.0f;
      }

      if (is_right) {
        corner_top_right.x += p_widgets->rect.w;
        p_widgets->rect.x = (
          ekg::layout::transform_to_pixel_perfect_position(
            corner_top_left.x,
            corner_top_right.x,
            container_rect.w,
            ekg::layout::offset
          )
        );

        corner_top_right.x += ekg::layout::offset;
        p_widgets->rect.y = corner_top_right.y;
      }

      highest_top = ekg::min_clamp(highest_top, p_widgets->rect.h);
      break;
    }

    if (should_estimate_extent) {
      count = it;
      ekg::layout::extentnize_widget(
        p_widget_parent,
        ekg::dock::fill,
        ekg::dock::next,
        ekg::axis::horizontal,
        fill_align,
        dimensional_extent,
        count
      );
    }

    if (
      !fill_align.was_pixel_perfect_calculated
      &&
      fill_align.must_calculate_pixel_perfect
    ) {
      fill_align.align = container_rect.w - (p_widgets->rect.x + p_widgets->rect.w);
      fill_align.was_pixel_perfect_calculated = true;
      
      corner_top_right.x = fill_align.align;
      corner_bottom_right.x = fill_align.align + ekg::layout::offset;
    } else if (
      is_fill
      &&
      fill_align.was_last_fill_found
      &&
      fill_align.was_pixel_perfect_calculated
    ) {
      p_widgets->rect.w = (
        (container_rect.w - p_widgets->rect.x) - fill_align.align
      );
    }

    max_previous_height = p_widgets->rect.h > max_previous_height ? p_widgets->rect.h : max_previous_height;
    if (should_reload_widget) {
      p_widgets->on_reload();
    }

    h_extent_backup = ekg::layout::extent_t::h_widget;
    if (p_properties->is_docknizable && !p_properties->children.empty()) {
      ekg::layout::docknize_widget(p_widgets);
    }

    ekg::layout::extent_t::h_widget = h_extent_backup;
    prev_widget_layout = layout;
    it++;
  }

  if (has_scroll_embedded && !is_vertical_enabled && type == ekg::type::frame) {
    ekg::ui::frame_widget *p_frame {(ekg::ui::frame_widget *) p_widget_parent};
    has_scroll_embedded = p_frame->p_scroll_embedded != nullptr;

    if (has_scroll_embedded && !is_vertical_enabled && p_frame->p_scroll_embedded->is_vertical_enabled) {
      ekg::layout::docknize_widget(p_widget_parent);
    }
  }
}
