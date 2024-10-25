#include <QColor>
#include <QList>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>

#include "3rd_utils.h"

#include "gl_widget_2d/cloud_widget_2d_paint_dash.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter {

CloudWidget2DPaintDash::CloudWidget2DPaintDash(GLCloudWidget2D* widget) : CloudWidget2DPaintInterface(widget) {}

void CloudWidget2DPaintDash::paint(QPainter* painter, QPaintEvent*) {
  if (!widget_) return;
  if ((int)(clr_idx_) >= widget_->colors_.size()) return;

  const auto& clr = widget_->colors_.at((int)clr_idx_);
  QPen pen(clr, 2, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin);
  QBrush brush(clr);

  painter->save();
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawLine(start_pt_, end_pt_);

  painter->restore();
}

///////////////////////////////////////////////////////////////

CloudWidget2DPaintHorizontalDashLine::CloudWidget2DPaintHorizontalDashLine(GLCloudWidget2D* widget)
    : CloudWidget2DPaintDash(widget) {}

void CloudWidget2DPaintHorizontalDashLine::setY(double y) {
  lg_y_ = y;
  CHECK1(widget_, "widget_ is null");
  const auto rect = widget_->rect();
  update_phy_points(rect);
}

void CloudWidget2DPaintHorizontalDashLine::process_widget_resize(QSize size) {
  const auto rect = QRect(0, 0, size.width(), size.height());
  update_phy_points(rect);
}

void CloudWidget2DPaintHorizontalDashLine::update_phy_points(const QRect& rect) {
  if (!widget_) return;

  const auto phy_height = rect.height(), phy_miny = rect.y();
  const auto lg_miny = rng_.axis_y_[0].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_height = lg_maxy - lg_miny;

  phy_y_ = (lg_y_ - lg_miny) / lg_height * phy_height + phy_miny;
  if (widget_->bottom_to_top_) {
    phy_y_ = phy_height - phy_y_;
  }

  start_pt_ = QPoint(0, phy_y_);
  end_pt_ = QPoint(rect.width(), phy_y_);
}

}  // namespace test::gl_painter