#pragma once

namespace test::gl_painter {

template <typename T>
struct gl_cw_rng_axis {
  T value_{};
  double mp_{-1.0};
};
template <typename T>
struct gl_cw_rng_xy {
  gl_cw_rng_axis<T> axis_x_[2];
  gl_cw_rng_axis<T> axis_y_[2];
};

using gl_cw_rng_xyd = gl_cw_rng_xy<double>;

}  // namespace test::gl_painter