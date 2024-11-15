#include "ekg/layout/docknize.hpp"
#include "ekg/ui/display.hpp"
#include "ekg/ekg.hpp"
#include "ekg/ui/frame/ui_frame_widget.hpp"

float ekg::layout::offset {2.0f};
ekg::layout::extent_t ekg::layout::v_extent = {};
ekg::layout::extent_t ekg::layout::h_extent = {};

void ekg::layout::extentnize(
  float &extent,
  ekg::layout::fill_align_t *p_fill_align,
  ekg::ui::abstract_widget *p_widget,
  ekg::flags flag_ok,
  ekg::flags flag_stop,
  int64_t &begin_and_count,
  ekg::axis axis
) {
  extent = 0.0f;
  if (p_widget == nullptr) {
    return;
  }

  int64_t begin_index {begin_and_count};
  switch (axis) {
    case ekg::axis::horizontal: {
      ekg::flags flags {};
      int32_t ids {};
      int64_t flag_ok_count {};
      int64_t it {begin_index};

      if (
          begin_and_count > static_cast<int64_t>(ekg::layout::h_extent.begin_index)
          &&
          begin_and_count < static_cast<int64_t>(ekg::layout::h_extent.end_index)
        ) {

        begin_and_count = static_cast<int64_t>(ekg::layout::h_extent.count);
        extent = ekg::layout::h_extent.extent;
        break;
      }

      ekg::layout::h_extent.begin_index = static_cast<float>(it);
      ekg::ui::abstract_widget *p_widgets {};
      std::vector<int32_t> &child_id_list {p_widget->p_data->get_child_id_list()};

      int64_t size {static_cast<int64_t>(child_id_list.size())};
      int64_t latest_index {size - (!child_id_list.empty())};

      bool is_scrollbar {};
      bool is_last_index {};
      bool is_ok_flag {};
      bool is_last_index_but {};
      bool is_ok {};
      bool is_stop {};

      /**
       * The last index does not check if contains a next flag,
       * so it is needed to brute-check to stop at end of index. 
       *
       * The extent data store the previous bounding indices,
       * in simply words, prevent useless iteration.
       *
       * The min offset is added for extent, because we need count
       * the offset position when split the fill width, but the
       * last extent space is not necessary, so we need to subtract.
       **/
      for (it = it; it < size; it++) {
        if ((p_widgets = ekg::core->get_fast_widget_by_id(child_id_list.at(it))) == nullptr) {
          continue;
        }

        flags = p_widgets->p_data->get_place_dock();
        is_scrollbar = p_widgets->p_data->get_type() == ekg::type::scrollbar;
        is_last_index = it == latest_index;
        is_ok = ekg_bitwise_contains(flags, flag_ok);
        is_stop = ekg_bitwise_contains(flags, flag_stop);

        if (
            (is_stop && it != begin_and_count) || is_last_index || is_scrollbar
          ) {

          extent -= (ekg::layout::offset) * (extent > 0.0f);

          is_last_index_but = (
            is_ok && is_last_index
          );

          flag_ok_count += (
            !is_stop
            &&
            is_last_index_but
          );

          is_last_index_but = (
            is_last_index
            &&
            !is_ok
            &&
            !is_stop
            &&
            !is_scrollbar
          );

          /**
           * Basically if the container/frame mother ends with any non flag ok (ekg::dock::fill)
           * it MUST add the width size to extend.
           * But the point is the scrollbar, he is not docknized here, then just bypass with `!is_scrollbar`.
           *
           * :blush:
           **/
          extent += (
            p_widgets->dimension.w
            *
            is_last_index_but
          );

          /**
           * There are two glitches fixed here.
           * 
           * The first one happens when the widget contains `next | fill` flag but the previous row
           * count as one extra index, so the last widget does not fill with 1 but with the previous
           * amount of fill count.
           * 
           * The second one happens when the widget contains `next | fill` and `fill`, making the
           * previous row do not count as 1, so the widget is complete filled.
           * 
           * To fix this, we consider the two options:
           * NOT STOP or OK but NOT STOP.
           * 
           * Both case must fix this, if soon happens, we can already now the context of issue.
           **/
          is_last_index_but = (
            is_last_index
            &&
            (!is_stop || (is_ok && !is_stop))
            &&
            !is_scrollbar
          );

          ekg::layout::h_extent.end_index = it + (is_last_index * is_last_index_but);
          ekg::layout::h_extent.extent = extent;
          ekg::layout::h_extent.count = flag_ok_count + (flag_ok_count == 0);
          p_fill_align->index = is_stop ? it - (it > 0) : -1;
          break;
        }

        if (is_ok) {
          flag_ok_count++;
          continue;
        }

        extent += p_widgets->dimension.w + ekg::layout::offset;
      }

      begin_and_count = flag_ok_count + (flag_ok_count == 0);
      if (!p_fill_align->was_found && is_stop) {
        p_fill_align->was_found = true;
      }

      break;
    }

    case ekg::axis::vertical: {
      break;
    }
  }

  p_fill_align->was_last_fill_found = p_fill_align->index == begin_index;
  if (p_fill_align->was_found && !p_fill_align->must_calculate_pixel_perfect && p_fill_align->was_last_fill_found) {
    p_fill_align->must_calculate_pixel_perfect = true;
  }
}

void ekg::layout::docknize(
  ekg::ui::abstract_widget *p_widget_parent
) {
  if (p_widget_parent == nullptr || p_widget_parent->p_data == nullptr) {
    return;
  }

  ekg::type type {p_widget_parent->p_data->get_type()};
  bool is_group {type == ekg::type::frame};
  ekg::rect &abs_parent_rect {p_widget_parent->get_abs_rect()};

  if (!is_group || abs_parent_rect.w == 0 || abs_parent_rect.h == 0) {
    return;
  }

  if (p_widget_parent->is_targeting_absolute_parent) {
    p_widget_parent->is_targeting_absolute_parent = false;

    if (p_widget_parent->p_abs_parent_widget) {
      ekg::layout::docknize(p_widget_parent->p_abs_parent_widget);
    } else {
      ekg::layout::docknize(p_widget_parent);
    }

    return;
  }

  float group_top_offset {ekg::layout::offset};
  float initial_offset {};
 
  bool has_scroll_embedded {};
  bool is_vertical_enabled {};

  ekg::rect container_rect {p_widget_parent->dimension};

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

  /**
   * Container size (scaled container size), works right for left docknization
   * but right dockinization no, due the precision and difference between
   * sides where is calculated the positions of widget.
   * So there is a special size number to use as container rect.
   **/
  ekg::vec2 side_container_fixed {
    0.0f,
    container_rect.h - ((initial_offset + ekg::layout::offset))
  };

  float container_size_offset {(initial_offset + ekg::layout::offset) * 2.0f};
  container_rect.w -= container_size_offset;
  container_rect.h -= container_size_offset;

  ekg::ui::abstract_widget *p_widgets {};
  ekg::flags flags {};
  int64_t it {};
  float dimensional_extent {};
  int64_t count {};

  ekg::rect widget_rect {};
  ekg::rect parent_offset {ekg::layout::offset + initial_offset, ekg::layout::offset + initial_offset, 0.0f, 0.0f};
  ekg::rect prev_widget_layout {};

  ekg::flags prev_flags {};
  bool should_reload_widget {};
  bool skip_widget {};
  bool should_estimated_extentinize {};
  float max_previous_height {};

  ekg::layout::h_extent = {};
  ekg::layout::extent_t h_extent_backup {};
  ekg::layout::extent_t v_extent_backup {};
  ekg::layout::fill_align_t fill_align {};

  bool is_none {};
  bool is_free {};
  bool is_left {};
  bool is_right {};
  bool is_top {};
  bool is_bottom {};
  bool is_fill {};
  bool is_next {};
  bool was_first_fill_right_align_found {};

  ekg::rect corner_top_left {parent_offset};
  ekg::rect corner_top_right {0.0f, parent_offset.y, 0.0f, 0.0f};
  ekg::rect corner_bottom_left {parent_offset.x, 0.0f, 0.0f, 0.0f};
  ekg::rect corner_bottom_right {};

  float highest_top {};
  float highest_bottom {};

  for (int32_t &ids: p_widget_parent->p_data->get_child_id_list()) {
    if (ids == 0 || (p_widgets = ekg::core->get_fast_widget_by_id(ids)) == nullptr) {
      continue;
    }

    ekg::rect &layout {p_widgets->dimension};
    flags = p_widgets->p_data->get_place_dock();

    // @TODO Prevent useless scrolling reload.
    p_widgets->on_reload();
    type = p_widgets->p_data->get_type();

    if (type == ekg::type::scrollbar) {
      it++;
      continue;
    }

    should_estimated_extentinize = true;

    is_none   = ekg_bitwise_contains(flags, ekg::dock::none);
    is_free   = ekg_bitwise_contains(flags, ekg::dock::free);
    is_right  = ekg_bitwise_contains(flags, ekg::dock::right);
    is_left   = (!is_right && ekg_bitwise_contains(flags, ekg::dock::left)) || !is_right;
    is_bottom = ekg_bitwise_contains(flags, ekg::dock::bottom);
    is_top    = (!is_bottom && ekg_bitwise_contains(flags, ekg::dock::top)) || !is_bottom;
    is_fill   = ekg_bitwise_contains(flags, ekg::dock::fill);
    is_next   = ekg_bitwise_contains(flags, ekg::dock::next);

    if (is_fill) {
      count = it;
      ekg::layout::extentnize(
        dimensional_extent,
        &fill_align,
        p_widget_parent,
        ekg::dock::fill,
        ekg::dock::next | (is_top ? ekg::dock::bottom : ekg::dock::top),
        count,
        ekg::axis::horizontal
      );

      dimensional_extent = ekg_min(
        ekg_layout_get_dimensional_extent(
          container_rect.w,
          dimensional_extent,
          ekg::layout::offset,
          count
        ),
        p_widgets->min_size.x
      );

      layout.w = dimensional_extent;
      should_reload_widget = true;
      should_estimated_extentinize = false;
    }

    switch (flags & ekg::dock::bottom) {
    case ekg::dock::bottom:
      highest_bottom = ekg_min(highest_bottom, layout.h);

      if (ekg_equals_float(corner_bottom_right.y, 0.0f)) {
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
        layout.x = corner_bottom_left.x;
        layout.y = (
          ekg_min(
            ekg_layout_get_pixel_perfect_position(
              corner_top_right.y,
              corner_bottom_right.y,
              container_rect.h,
              ekg::layout::offset
            ),
            corner_top_right.y + ekg::layout::offset + highest_bottom
          )
        );

        corner_bottom_left.x += layout.w + ekg::layout::offset;
      }

      if (is_next && is_right) {
        corner_bottom_left.x = parent_offset.x;
        corner_bottom_right.x = 0.0f;

        corner_bottom_right.y += highest_bottom + ekg::layout::offset;
        corner_bottom_left.y = corner_bottom_right.y;
        highest_bottom = 0.0f;
      }

      if (is_right) {
        corner_bottom_right.x += layout.w;
        layout.x = (
          ekg_layout_get_pixel_perfect_position(
            corner_bottom_left.x,
            corner_bottom_right.x,
            container_rect.w,
            ekg::layout::offset
          )
        );

        layout.y = (
          ekg_min(
            ekg_layout_get_pixel_perfect_position(
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

      break;
    default:
      highest_top = ekg_min(highest_top, layout.h);

      if (is_next && is_left) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_left.y += highest_top + ekg::layout::offset;
        corner_top_right.y = corner_top_left.y;
        highest_top = 0.0f;
      }

      if (is_left) {
        layout.x = corner_top_left.x;
        layout.y = corner_top_left.y;
  
        corner_top_left.x += layout.w + ekg::layout::offset;
      }

      if (is_next && is_right) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_right.y += highest_top + ekg::layout::offset;
        corner_top_left.y = corner_top_right.y;
        highest_top = 0.0f;
      }

      if (is_right) {
        corner_top_right.x += layout.w;
        layout.x = (
          ekg_layout_get_pixel_perfect_position(
            corner_top_left.x,
            corner_top_right.x,
            container_rect.w,
            ekg::layout::offset
          )
        );

        corner_top_right.x += ekg::layout::offset;
        layout.y = corner_top_right.y;
      }
      break;
    }

    if (should_estimated_extentinize) {
      count = it;
      ekg::layout::extentnize(
        dimensional_extent,
        &fill_align,
        p_widget_parent,
        ekg::dock::fill,
        ekg::dock::next,
        count,
        ekg::axis::horizontal
      );
    }

    if (!fill_align.was_pixel_perfect_calculated && fill_align.must_calculate_pixel_perfect) {
      fill_align.align = container_rect.w - (layout.x + layout.w);
      fill_align.was_pixel_perfect_calculated = true;
      corner_top_right.x = fill_align.align;
      corner_bottom_right.x = fill_align.align + ekg::layout::offset;
    } else if (is_fill && fill_align.was_last_fill_found && fill_align.was_pixel_perfect_calculated) {
      layout.w = (
        (container_rect.w - layout.x) - fill_align.align
      );
    }

    max_previous_height = layout.h > max_previous_height ? layout.h : max_previous_height;
    if (should_reload_widget) {
      p_widgets->on_reload();
    }

    h_extent_backup = ekg::layout::h_extent;
    if (p_widgets->p_data->has_children()) {
      ekg::layout::docknize(p_widgets);
    }

    ekg::layout::h_extent = h_extent_backup;
    prev_widget_layout = layout;
    prev_flags = flags;
    it++;
  }

  if (has_scroll_embedded && !is_vertical_enabled && type == ekg::type::frame) {
    ekg::ui::frame_widget *p_frame {(ekg::ui::frame_widget *) p_widget_parent};
    has_scroll_embedded = p_frame->p_scroll_embedded != nullptr;

    if (has_scroll_embedded && !is_vertical_enabled && p_frame->p_scroll_embedded->is_vertical_enabled) {
      ekg::layout::docknize(p_widget_parent);
    }
  }
}

float ekg::layout::height(
  ekg::ui::abstract_widget *p_parent_widget
) {
  if (p_parent_widget == nullptr) {
    return 0.0f;
  }

  ekg::ui::abstract_widget *p_widgets {};
  ekg::flags flags {};
  float total_height {};
  float height {};

  for (int32_t &ids : p_parent_widget->p_data->get_child_id_list()) {
    if (ids == 0 || (p_widgets = ekg::core->get_fast_widget_by_id(ids)) == nullptr) {
      continue;
    }

    p_widgets->on_reload();

    flags = p_widgets->p_data->get_place_dock();
    height = p_widgets->dimension.h;

    if (p_widgets->p_data->has_children()) {
      height = ekg::layout::height(p_widgets); 
    }

    total_height += (
      height
      *
      (ekg_equals_float(total_height, 0.0f) || ekg_bitwise_contains(flags, ekg::dock::next))
      +
      ekg::layout::offset
    );
  }

  return total_height;
}

void ekg::layout::mask::extentnize(
  float &extent,
  ekg::flags flag_ok,
  ekg::flags flag_stop,
  int64_t &begin_and_count,
  ekg::axis axis
) {
  extent = 0.0f;
  switch (axis) {
    case ekg::axis::horizontal: {
      ekg::flags flags {};
      int32_t ids {};
      int64_t flag_ok_count {};
      int64_t it {begin_and_count};

      if (
          begin_and_count > this->h_extent.begin_index
          &&
          begin_and_count < this->h_extent.end_index
        ) {
        begin_and_count = this->h_extent.count;
        extent = this->h_extent.extent;
        return;
      }

      ekg::layout::h_extent.begin_index = static_cast<float>(it);
      uint64_t size {this->dock_rect_list.size()};
      uint64_t latest_index {size - (!this->dock_rect_list.empty())};
      int32_t should_skip_next {};

      bool is_last_index {};
      bool is_ok_flag {};
 
      extent += this->offset.x;

      /**
       * The last index does not check if contains a next flag,
       * so it is needed to brute-check to stop at end of index. 
       *
       * The extent data store the previous bounding indices,
       * in simply words, prevent useless iteration.
       *
       * The min offset is added for extent, because we need count
       * the offset position when split the fill width, but the
       * last extent space is not necessary, so we need to subtract.
       **/
      for (it = it; it < size; it++) {
        ekg::layout::mask::rect &dock_rect {this->dock_rect_list.at(it)};
        if (dock_rect.p_rect == nullptr) {
          continue;
        }

        is_last_index = it == latest_index;

        if (
            (ekg_bitwise_contains(dock_rect.flags, flag_stop) && it != begin_and_count) || is_last_index
          ) {
          extent -= this->offset.x;
          flag_ok_count += (
            (is_ok_flag = (!ekg_bitwise_contains(dock_rect.flags, flag_stop) && (ekg_bitwise_contains(dock_rect.flags, flag_ok)) && is_last_index))
          );

          /**
           * Basically if the container/frame mother ends with any non flag ok (ekg::dock::fill)
           * it MUST add the width size to extend.
           *
           * :blush:
           **/
          extent += ( 
            (dock_rect.p_rect->w + this->offset.x) * (is_last_index && (!ekg_bitwise_contains(dock_rect.flags, flag_ok) && should_skip_next == 0))
          );

          this->h_extent.end_index = it + is_last_index;
          this->h_extent.extent = extent;
          this->h_extent.count = flag_ok_count + (flag_ok_count == 0);
          break;
        }

        should_skip_next += static_cast<bool>(ekg_bitwise_contains(dock_rect.flags, ekg::dock::bind));

        if (should_skip_next > 0) {
          should_skip_next = (should_skip_next + 1) * (should_skip_next < 2);
          flag_ok_count += static_cast<bool>(ekg_bitwise_contains(dock_rect.flags, flag_ok));
          continue;
        }

        if (ekg_bitwise_contains(dock_rect.flags, flag_ok)) {
          flag_ok_count++;
          continue;
        }

        extent += dock_rect.p_rect->w + this->offset.x;
      }

      begin_and_count = flag_ok_count + (flag_ok_count == 0);
      break;
    }

    case ekg::axis::vertical: {
      break;
    }
  }
}

void ekg::layout::mask::preset(const ekg::vec3 &mask_offset, ekg::axis mask_axis, float initial_respective_size) {
  this->axis = mask_axis;
  this->offset = mask_offset;
  this->respective_all = initial_respective_size;
  this->v_extent = {};
  this->h_extent = {};
}

void ekg::layout::mask::insert(const ekg::layout::mask::rect &dock_rect) {
  if (ekg_bitwise_contains(dock_rect.flags, ekg::dock::none)) {
    dock_rect.p_rect->w = 0.0f;
    dock_rect.p_rect->h = 0.0f;
    return;
  }

  this->dock_rect_list.push_back(dock_rect);
}

void ekg::layout::mask::docknize() {
  float left_or_right {};
  float centered_dimension {this->offset.z / 2};
  float opposite {};
  float uniform {};
  float clamped_offset {};

  int64_t count {};
  float dimensional_extent {};
  float rect_height {};
  float rect_width {};
  float dimension_width {};
  float dimension_height {};
  float dimension_bind {};

  bool is_left {};
  bool is_right {};
  bool is_center {};
  bool is_top {};
  bool is_bottom {};
  bool is_not_bind {};
  bool is_bind_dimension_not_zero {};

  /**
   * All dock rects within left corner (ekg::dock::left)
   * starts from left to right.
   **/
  ekg::rect left_corner {};

  /**
   * All dock rects within right corner (ekg::dock::right)
   * starts from right (dimension width based) to left.
   **/
  ekg::rect right_corner {};

  /**
   * All dock rects within center left corner (ekg::dock::right & ekg::dock::center)
   * starts from right (dimension width divided by 2 based) to left.
   **/
  ekg::rect center_left_corner {};

  /**
   * All dock rects within center right corner (ekg::dock::right & ekg::dock::center)
   * starts from left (dimension width divided by 2 based) to right.
   **/
  ekg::rect center_right_corner {};

  ekg::rect center {};

  switch (this->axis) {
  case ekg::axis::horizontal:
    if (this->dock_rect_list.empty()) {
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

    for (uint64_t it {}; it < this->dock_rect_list.size(); it++) {
      ekg::layout::mask::rect &dock_rect {this->dock_rect_list.at(it)};
      if (dock_rect.p_rect == nullptr) {
        continue;
      }

      is_left = ekg_bitwise_contains(dock_rect.flags, ekg::dock::left);
      is_right = ekg_bitwise_contains(dock_rect.flags, ekg::dock::right);
      is_center = ekg_bitwise_contains(dock_rect.flags, ekg::dock::center);
      is_bottom = ekg_bitwise_contains(dock_rect.flags, ekg::dock::bottom);
      is_top = ekg_bitwise_contains(dock_rect.flags, ekg::dock::top);
      is_not_bind = !ekg_bitwise_contains(dock_rect.flags, ekg::dock::bind);
      is_bind_dimension_not_zero = (dimension_bind > 0.0f);

      rect_width = dock_rect.p_rect->w;
      rect_height = dock_rect.p_rect->h;

      if (ekg_bitwise_contains(dock_rect.flags, ekg::dock::fill)) {
        count = 0;
        this->extentnize(
          dimensional_extent,
          ekg::dock::fill,
          ekg::dock::none,
          count,
          ekg::axis::horizontal
        );

        rect_width = ekg_min(
          ekg_layout_get_dimensional_extent(
            this->respective_all,
            dimensional_extent,
            this->offset.x,
            count
          ),
          1.0f
        );
      }

      if (is_left) {
        dock_rect.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + left_corner.w;
        dock_rect.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((dock_rect.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        dock_rect.p_rect->w = rect_width;
        dock_rect.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + dimension_width - right_corner.w - dock_rect.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((dock_rect.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_left) {
        dock_rect.p_rect->w = rect_width;
        dock_rect.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_left_corner.x - center_left_corner.w - dock_rect.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((dock_rect.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        center_left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        dock_rect.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_right_corner.x + center_right_corner.w;
        dock_rect.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((dock_rect.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_bind
        );

        center_right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (dock_rect.flags == ekg::dock::center) {
        dock_rect.p_rect->w = rect_width;
        dock_rect.p_rect->x = (dimension_width / 2.0f) - (dock_rect.p_rect->w / 2.0f);
      }

      if (is_top | is_bottom) {
        dock_rect.p_rect->y = (
          is_top ? (this->offset.y) : (dimension_height - rect_height - this->offset.y)
        );
      } else {
        dock_rect.p_rect->y = (
          (dimension_height / 2.0f) - (rect_height / 2.0f)
        );
      }

      if (!is_not_bind && dock_rect.p_rect->w > dimension_bind) {
        dimension_bind = dock_rect.p_rect->w;
      } else if (is_not_bind) {
        dimension_bind = 0.0f;
      }

      this->mask.h = dimension_height;
    }

    this->mask.w = ekg_min(
      this->respective_all,
      this->mask.w
    );
    break;
  case ekg::axis::vertical:
    if (this->dock_rect_list.empty()) {
      this->mask.w = this->offset.z;
      this->mask.h = this->respective_all;
      return;
    }

    this->mask.w = this->offset.z;
    this->mask.h = this->offset.y;
    break;
  }

  this->dock_rect_list.clear();
}

ekg::rect &ekg::layout::mask::get_rect() {
  return this->mask;
}