#pragma once

template <typename T>
struct point_t {
  T x_{}, y_{};

  point_t operator+(const point_t& p) const { return {x_ + p.x_, y_ + p.y_}; }
  point_t operator-(const point_t& p) const { return {x_ - p.x_, y_ - p.y_}; }
  template <typename U>
  point_t operator*(U s) const {
    return {x_ * s, y_ * s};
  }
};

using point_double_t = point_t<double>;