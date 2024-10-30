#pragma once

#include <QPoint>
#include <QPointF>
#include <QRect>

#include "cloud_widget_2d_paint_interface_imp.h"

namespace test::gl_painter::imp {

class CloudWidget2DPaintDashImp : public CloudWidget2DPaintInterfaceImp {
 public:
  CloudWidget2DPaintDashImp(test::gl_painter::GLCloudWidget2D* glWidget);
  ~CloudWidget2DPaintDashImp() override = default;

  void paint(QPainter* painter, QPaintEvent* event) override;
  mouse_trace_t mouseTrace(const QPoint& pos) override;
  void process_widget_resize(QSize size) override;

 protected:
  QPoint start_pt_, end_pt_;
};

class CloudWidget2DPaintHorizontalDashLineImp : public CloudWidget2DPaintDashImp {
 public:
  CloudWidget2DPaintHorizontalDashLineImp(test::gl_painter::GLCloudWidget2D* glWidget);
  ~CloudWidget2DPaintHorizontalDashLineImp() override = default;

  void setY(double y);

  void process_widget_resize(QSize size) override;

 protected:
  void update_phy_points(const QRect& rect);

 protected:
  double lg_y_{}, phy_y_{};
};

}  // namespace test::gl_painter::imp