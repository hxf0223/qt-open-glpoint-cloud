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

  void paint(QPainter* painter, QPaintEvent* event) override;

 public:
  using CloudWidget2DPaintInterface::mouse_trace_t;
  mouse_trace_t mouseTrace(const QPoint& pos) override;

 protected:
  void process_widget_resize(QSize size) override;

  QPointF widget_pt_to_logic_pt(const QPoint& pt) const;
  void update_phy_points(const QRect& rect);

 protected:
  double pt_phy_size_{1.0};
  std::vector<point_double_t> pts_;
  std::vector<QPointF> phy_pts_;
  std::vector<double> phy_xs_, phy_ys_;
};

}  // namespace test::gl_painter