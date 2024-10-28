#pragma once

#include <vector>

#include <QPoint>
#include <QPointF>
#include <QRect>

#include "cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d_exp_def.h"
#include "point_typedef.h"

namespace test::gl_painter {

template <typename T, typename ET = double>
struct point_adapter_t {
  point_adapter_t(T* const ptr, size_t size) : ptr_(ptr), size_(size) {}
  point_t<ET> operator[](size_t i) const {
    const ET x = ptr_[i].getX();
    const ET y = ptr_[i].getY();
    return point_t<ET>{x, y};
  }

 private:
  T* const ptr_;
  const size_t size_;
};

class GL_WIDGET_2D_API CloudWidget2DPaintPoints : public CloudWidget2DPaintInterface {
 public:
  CloudWidget2DPaintPoints(GLCloudWidget2D* widget);
  ~CloudWidget2DPaintPoints() override = default;

  void setData(const std::vector<point_double_t>& pts);
  void setPhyPointSize(double size);
};

}  // namespace test::gl_painter