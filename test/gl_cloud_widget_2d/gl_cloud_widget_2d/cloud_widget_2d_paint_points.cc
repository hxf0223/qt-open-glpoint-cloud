#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include "cloud_widget_2d_paint_points.h"
#include "gl_cloud_widget_2d.h"

namespace test::gl_painter {

CloudWidget2DPaintPoints::CloudWidget2DPaintPoints(GLCloudWidget2D* widget) : CloudWidget2DPaintInterface(widget) {}

void CloudWidget2DPaintPoints::setData(const std::vector<point_double_t>& pts) {
  phy_pts_ = std::vector<QPointF>(pts.size());
  phy_xs_ = std::vector<double>(pts.size());
  phy_ys_ = std::vector<double>(pts.size());
  pts_ = pts;
}
void CloudWidget2DPaintPoints::setPhyPointSize(double size) {
  pt_phy_size_ = size;
}

void CloudWidget2DPaintPoints::paint(QPainter* painter, QPaintEvent* event) {
  if (pts_.empty() || !widget_) return;
  if ((int)(clr_idx_) >= widget_->colors_.size()) return;

  update_phy_points(event->rect());

  const auto& clr = widget_->colors_.at((int)clr_idx_);
  QPen pen(clr, pt_phy_size_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  QBrush brush(clr);

  painter->save();
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawPoints(phy_pts_.data(), (int)phy_pts_.size());

  painter->restore();
}

void CloudWidget2DPaintPoints::update_phy_points(const QRect& rect) {
  const auto phy_width = rect.width(), phy_height = rect.height();
  const auto phy_minx = rect.x(), phy_miny = rect.y();
  // const auto phy_maxx = phy_minx + phy_width, phy_maxy = phy_miny + phy_height;
  const auto lg_minx = rng_.axis_x_[0].value_, lg_miny = rng_.axis_y_[0].value_;
  const auto lg_maxx = rng_.axis_x_[1].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_width = lg_maxx - lg_minx, lg_height = lg_maxy - lg_miny;

  CHECK2(phy_xs_.size() == pts_.size(), "phy_xs_.size() != pts_.size()");
  CHECK2(phy_ys_.size() == pts_.size(), "phy_ys_.size() != pts_.size()");
  CHECK2(phy_pts_.size() == pts_.size(), "phy_pts_.size() != pts_.size()");

  for (size_t i = 0; i < pts_.size(); i++) {
    phy_xs_[i] = (pts_[i].x_ - lg_minx) / lg_width * phy_width + phy_minx;
    phy_ys_[i] = (pts_[i].y_ - lg_miny) / lg_height * phy_height + phy_miny;
  }
  if (widget_->right_to_left_) {
    for (size_t i = 0; i < pts_.size(); i++) {
      phy_xs_[i] = phy_width - phy_xs_[i];
    }
  }
  if (widget_->bottom_to_top_) {
    for (size_t i = 0; i < pts_.size(); i++) {
      phy_ys_[i] = phy_height - phy_ys_[i];
    }
  }

  for (size_t i = 0; i < pts_.size(); i++) {
    phy_pts_[i] = QPointF(phy_xs_[i], phy_ys_[i]);
  }
}

}  // namespace test::gl_painter