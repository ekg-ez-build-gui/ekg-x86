/**
 * MIT License
 *
 * Copyright (c) 2022-2024 Rina Wilk / vokegpu@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EKG_UI_DISPLAY_HPP
#define EKG_UI_DISPLAY_HPP

#include "ekg/util/geometry.hpp"

namespace ekg {
  /**
   * EKG does not has support to dynamic font sizes,
   * due the performance loss while handling it.
   * EKG provides 3 basics sizes:
   * - Small
   * - Normal
   * - Big
   * But it depends of current display scale.
   */
  enum class font {
    small,
    normal,
    big
  };
}

namespace ekg::ui {
  /**
   * The current display-viewport width.
   */
  extern int32_t width;

  /**
   * The current display-viewport height.
   */
  extern int32_t height;

  /**
   * Current pipeline viewport (note may be diff in Vulkan).
   **/
  extern ekg::rect viewport;

  /**
   * Delta time value, usable by the widgets to perform animations.
   * Not automatically set, application must set the delta-time value.
   */
  extern float dt;

  /**
   * The scale-base of GUI,
   * by default value is set as 1920x1080 (full high-definition).
   */
  extern ekg::vec2 scale;

  /**
   * Auto-scale boolean do the GUI `ekg::scale` be set as the monitor
   * resolution.
   * Smartphones displays may need manual set the scale-base.
   */
  extern bool auto_scale;

  /**
   * The scale interval, likely delta-time but for display tiles size.
   * Default value and recommend: 25.0f
   */
  extern float scale_interval;

  /**
   * Display latency for dispatching hover (motion)
   * task. Default is 500.
   * Android must have a higher latency due the battery.
   */
  extern int64_t latency;

  /**
   * The display scroll value. Default is 0.8.
   * Note: It is a normalized acceleration speed.
   */
  extern float scroll;

  /**
   * The technology algorithm used here needs a tiling size to fix
   * the DPI pixel inperfect.
   * Default: 2.0f
   */
  extern float dpi_tiling;

  /**
   * Min frame width & height.
   **/
  extern float min_frame_size;

  /**
   * Redraw the entire GUI.
   **/
  extern bool redraw;
}

#endif
