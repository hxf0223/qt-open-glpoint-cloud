#pragma once

namespace test::gl_painter {

template <typename T>
struct point_t {
  T x_{}, y_{};

  point_t operator+(const point_t& p) const { return {x_ + p.x_, y_ + p.y_}; }
  point_t operator-(const point_t& p) const { return {x_ - p.x_, y_ - p.y_}; }
  point_t operator+(double delta) const { return {x_ + delta, y_ + delta}; }
  point_t operator-(double delta) const { return {x_ - delta, y_ - delta}; }

  template <typename U>
  point_t operator*(U s) const {
    return {x_ * s, y_ * s};
  }
};

using point_double_t = point_t<double>;

struct rect_double_t {
  point_double_t top_left_;
  point_double_t bottom_right_;
};

}  // namespace test::gl_painter