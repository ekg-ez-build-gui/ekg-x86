#ifndef EKG_LAYOUT_EXTENTNIZE_HPP
#define EKG_LAYOUT_EXTENTNIZE_HPP

namespace ekg::layout {
  struct fill_align_t {
  public:
    bool was_last_fill_found {};
    bool was_found {};
    bool must_calculate_pixel_perfect {};
    bool was_pixel_perfect_calculated {};

    float align {static_cast<float>(UINT32_MAX)}; // idk may i duwmb
    int64_t index {};
    int64_t end_fill_index {};
  };

  struct extent_t {
  public:
    static ekg::layout::extent_t v_widget;
    static ekg::layout::extent_t h_widget;
    static ekg::layout::extent_t v_rect_descriptor;
    static ekg::layout::extent_t h_rect_descriptor;
  public:
    int64_t end_index {};
    int64_t begin_index {};
    int64_t count {};
    float extent {};
  };

  /**
   * Obtain the remain extent size, from the latest rect descriptor list index.
   **/
  void extentnize_rect_descriptor(
    std::vector<ekg::rect_descriptor_t> &rect_descriptor_list,
    float &extent,
    ekg::flags flag_ok,
    ekg::flags flag_stop,
    int64_t &begin_and_count,
    ekg::axis axis
  );

  /**
   * Obtain the remain extent size, from the latest widget index.
   **/
  void extentnize_widget(
    ekg::ui::abstract_widget *p_widget,
    float &extent,
    fill_align_t *p_fill_align,
    ekg::flags flag_ok,
    ekg::flags flag_stop,
    int64_t &begin_and_count,
    ekg::axis axis
  );
}

#endif
