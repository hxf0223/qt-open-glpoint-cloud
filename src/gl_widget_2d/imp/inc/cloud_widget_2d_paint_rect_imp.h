#pragma once

#include <qchar.h>
#include <qpoint.h>
#include <vector>

#include <QPoint>
#include <QPointF>
#include <QRect>

#include "cloud_widget_2d_paint_interface_imp.h"
#include "gl_widget_2d/point_typedef.h"

namespace test::gl_painter::imp {

class CloudWidget2DPaintRectImp : public CloudWidget2DPaintInterfaceImp {
 public:
  CloudWidget2DPaintRectImp(test::gl_painter::GLCloudWidget2D* glWidget);
  ~CloudWidget2DPaintRectImp() override = default;

  void setPenStyle(Qt::PenStyle style);
  void setData(const std::vector<paint_rect_data_t>& rects);

  void paint(QPainter* painter, QPaintEvent* event) override;
  mouse_trace_t mouseTrace(const QPoint& pos) override;
  void process_widget_resize(QSize size) override;
  QPointF lg_pt_to_phy_pt(const QPointF& pt) const override;

 protected:
  void update_phy_points(const QRect& rect);

 private:
  Qt::PenStyle pen_style_ = Qt::SolidLine;
  std::vector<paint_rect_data_t> lg_rects_;
  std::vector<QRectF> phy_rects_;
};

}  // namespace test::gl_painter::imp