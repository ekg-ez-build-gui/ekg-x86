#include "ekg/layout/extentnize.hpp"

ekg::layout::extent_t ekg::layout::extent_t::v_widget {};
ekg::layout::extent_t ekg::layout::extent_t::h_widget {};
ekg::layout::extent_t ekg::layout::extent_t::v_rect_descriptor {};
ekg::layout::extent_t ekg::layout::extent_t::h_rect_descriptor {};

void ekg::layout::extentnize_rect_descriptor(
  std::vector<ekg::rect_descriptor_t> &rect_descriptor_list,
  float &extent,
  ekg::flags flag_ok,
  ekg::flags flag_stop,
  int64_t &begin_and_count,
  ekg::axis axis
) {
  extent = 0.0f;
  switch (axis) {
    case ekg::axis::horizontal: {
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
      int64_t size {static_cast<int64_t>(this->dock_rect_list.size())};
      int64_t latest_index {static_cast<int64_t>(size - (!this->dock_rect_list.empty()))};
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

void ekg::layout::extentnize_widget(
  ekg::ui::abstract_widget *p_widget,
  float &extent,
  ekg::layout::fill_align_t &fill_align,
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

          fill_align.index = is_last_index_but ? it - (it > 0) : -1;

          ekg::layout::h_extent.end_index = it + (is_last_index * is_last_index_but);
          ekg::layout::h_extent.extent = extent;
          ekg::layout::h_extent.count = flag_ok_count + (flag_ok_count == 0);

          break;
        }

        if (is_ok) {
          flag_ok_count++;
          continue;
        }

        extent += p_widgets->dimension.w + ekg::layout::offset;
      }

      begin_and_count = flag_ok_count + (flag_ok_count == 0);
      if (!fill_align.was_found && is_stop) {
        fill_align.was_found = true;
      }

      break;
    }

    case ekg::axis::vertical: {
      break;
    }
  }

  fill_align.was_last_fill_found = fill_align.index == begin_index;
  if (
      fill_align.was_found
      &&
      !fill_align.must_calculate_pixel_perfect
      &&
      fill_align.was_last_fill_found
  ) {
    fill_align.must_calculate_pixel_perfect = true;
  }
}
