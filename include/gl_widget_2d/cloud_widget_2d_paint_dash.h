#pragma once

#include <QPoint>

#include "cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d_exp_def.h"

namespace test::gl_painter {

class GL_WIDGET_2D_API CloudWidget2DPaintDash : public CloudWidget2DPaintInterface {
  using CloudWidget2DPaintInterface::mouse_trace_t;

 public:
  CloudWidget2DPaintDash(GLCloudWidget2D* parent = nullptr);
  ~CloudWidget2DPaintDash() override = default;

  void paint(QPainter* painter, QPaintEvent* event) override;
  mouse_trace_t mouseTrace(const QPoint&) override { return mouse_trace_t(); }

 protected:
  void process_widget_resize(QSize) override{};

 protected:
  QPoint start_pt_, end_pt_;
};

class GL_WIDGET_2D_API CloudWidget2DPaintHorizontalDashLine : public CloudWidget2DPaintDash {
 public:
  CloudWidget2DPaintHorizontalDashLine(GLCloudWidget2D* parent = nullptr);
  ~CloudWidget2DPaintHorizontalDashLine() override = default;

  void setY(double y);

 protected:
  void process_widget_resize(QSize size) override;
  void update_phy_points(const QRect& rect);

 protected:
  double lg_y_{}, phy_y_{};
};

}  // namespace test::gl_painter