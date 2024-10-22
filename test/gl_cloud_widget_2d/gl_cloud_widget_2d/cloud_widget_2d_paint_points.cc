#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include <qpen.h>
#include <qpoint.h>
#include "cloud_widget_2d_paint_points.h"
#include "gl_cloud_widget_2d.h"

namespace test::gl_painter {

CloudWidget2DPaintPoints::CloudWidget2DPaintPoints(GLCloudWidget2D* widget) : CloudWidget2DPaintInterface(widget) {}

void CloudWidget2DPaintPoints::setData(const std::vector<point_double_t>& pts) {
  pts_ = pts;
}
void CloudWidget2DPaintPoints::setPhyPointSize(double size) {
  pt_phy_size_ = size;
}

void CloudWidget2DPaintPoints::paint(QPainter* painter, QPaintEvent* event) {
  if (pts_.empty() || !widget_) return;
  if (clr_idx_ >= widget_->colors_.size()) return;

  const auto phy_width = event->rect().width(), phy_height = event->rect().height();
  const auto phy_minx = event->rect().x(), phy_miny = event->rect().y();
  // const auto phy_maxx = phy_minx + phy_width, phy_maxy = phy_miny + phy_height;
  const auto lg_minx = rng_->axis_x_[0].value_, lg_miny = rng_->axis_y_[0].value_;
  const auto lg_maxx = rng_->axis_x_[1].value_, lg_maxy = rng_->axis_y_[1].value_;
  const auto lg_width = lg_maxx - lg_minx, lg_height = lg_maxy - lg_miny;

  std::vector<QPointF> pts_phy(pts_.size());
  for (size_t i = 0; i < pts_.size(); i++) {
    pts_phy[i] = QPointF((pts_[i].x_ - lg_minx) / lg_width * phy_width + phy_minx,
                         (pts_[i].y_ - lg_miny) / lg_height * phy_height + phy_miny);
  }

  QBrush brush(widget_->colors_[clr_idx_]);
  QPen pen(widget_->colors_[clr_idx_], pt_phy_size_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  painter->save();
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawPoints(pts_phy.data(), (int)(pts_phy.size()));

  painter->restore();
}

}  // namespace test::gl_painter