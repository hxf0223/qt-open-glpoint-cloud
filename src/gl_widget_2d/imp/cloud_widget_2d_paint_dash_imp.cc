#include <QDebug>
#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include <qpoint.h>
#include "cloud_widget_2d_paint_dash_imp.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter::imp {

CloudWidget2DPaintDashImp::CloudWidget2DPaintDashImp(test::gl_painter::GLCloudWidget2D* glWidget)
    : CloudWidget2DPaintInterfaceImp(glWidget) {}

void CloudWidget2DPaintDashImp::paint(QPainter* painter, QPaintEvent* /*event*/) {
  if ((int)(clr_idx_) >= colors_.size()) return;

  const auto& clr = colors_.at((int)clr_idx_);
  QPen pen(clr, 2, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin);
  QBrush brush(clr);

  painter->save();
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawLine(start_pt_, end_pt_);

  painter->restore();
}

mouse_trace_t CloudWidget2DPaintDashImp::mouseTrace(const QPoint&) {
  return mouse_trace_t();
}
void CloudWidget2DPaintDashImp::process_widget_resize(QSize) {}
QPointF CloudWidget2DPaintDashImp::lg_pt_to_phy_pt(const QPointF& /*pt*/) const {
  return QPointF();
}

////////////////////////////////////////////////////////////////////////////////////

CloudWidget2DPaintHorizontalDashLineImp::CloudWidget2DPaintHorizontalDashLineImp(test::gl_painter::GLCloudWidget2D* glWidget)
    : CloudWidget2DPaintDashImp(glWidget) {}

void CloudWidget2DPaintHorizontalDashLineImp::process_widget_resize(QSize /*size*/) {
  CHECK2(widget_, "widget_ is null");
  const QRect rect = widget_->paint_area_;
  update_phy_points(rect);
}

void CloudWidget2DPaintHorizontalDashLineImp::update_phy_points(const QRect& rect) {
  if (!widget_) return;

  const auto phy_height = rect.height(), phy_miny = rect.y();
  const auto lg_miny = rng_.axis_y_[0].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_height = lg_maxy - lg_miny;

  phy_y_ = (lg_y_ - lg_miny) / lg_height * phy_height;
  if (widget_->bottom_to_top_) phy_y_ = phy_height - phy_y_;
  phy_y_ += phy_miny;

  start_pt_ = QPoint(rect.x(), phy_y_);
  end_pt_ = QPoint(rect.right(), phy_y_);
}

void CloudWidget2DPaintHorizontalDashLineImp::setY(double y) {
  lg_y_ = y;
  CHECK1(widget_, "widget_ is null");
  const auto rect = widget_->paint_area_;
  update_phy_points(rect);
}

}  // namespace test::gl_painter::imp