#pragma once

#include <qchar.h>
#include <vector>

#include <QPoint>
#include <QPointF>
#include <QRect>

#include "cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d_exp_def.h"
#include "point_typedef.h"

namespace test::gl_painter {

struct paint_rect_data_t {
  rect_double_t rect;
  QString label_;
};

class GL_WIDGET_2D_API CloudWidget2DPaintRect : public CloudWidget2DPaintInterface {
 public:
  CloudWidget2DPaintRect(GLCloudWidget2D* widget);
  ~CloudWidget2DPaintRect() override = default;

  void setPenStyle(Qt::PenStyle style);
  void setData(const std::vector<paint_rect_data_t>& rects);
  void paint(QPainter* painter, QPaintEvent* event) override;

 public:
  using CloudWidget2DPaintInterface::mouse_trace_t;
  mouse_trace_t mouseTrace(const QPoint&) override { return mouse_trace_t(); }

 protected:
  void process_widget_resize(QSize size) override;
  void update_phy_points(const QRect& rect);

 private:
  Qt::PenStyle pen_style_ = Qt::SolidLine;
  std::vector<paint_rect_data_t> lg_rects_;
  std::vector<QRectF> phy_rects_;
};

}  // namespace test::gl_painter