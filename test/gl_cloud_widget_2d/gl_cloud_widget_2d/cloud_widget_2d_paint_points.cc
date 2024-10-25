#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include <qdebug.h>
#include <qglobal.h>
#include <qregion.h>
#include "cloud_widget_2d_paint_points.h"
#include "gl_cloud_widget_2d.h"

namespace test::gl_painter {

CloudWidget2DPaintPoints::CloudWidget2DPaintPoints(GLCloudWidget2D* widget) : CloudWidget2DPaintInterface(widget) {}

void CloudWidget2DPaintPoints::setData(const std::vector<point_double_t>& pts) {
  phy_pts_ = std::vector<QPointF>(pts.size());
  phy_xs_ = std::vector<double>(pts.size());
  phy_ys_ = std::vector<double>(pts.size());
  pts_ = pts;

  const auto rect = QRect(QPoint(0, 0), widget_->size());
  update_phy_points(rect);
}
void CloudWidget2DPaintPoints::setPhyPointSize(double size) {
  pt_phy_size_ = size;
}

void CloudWidget2DPaintPoints::paint(QPainter* painter, QPaintEvent* event) {
  if (pts_.empty() || !widget_) return;
  if ((int)(clr_idx_) >= widget_->colors_.size()) return;

  Q_UNUSED(event);
  //update_phy_points(event->rect());

  const auto& clr = widget_->colors_.at((int)clr_idx_);
  QPen pen(clr, pt_phy_size_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  QBrush brush(clr);

  painter->save();
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawPoints(phy_pts_.data(), (int)phy_pts_.size());

  painter->restore();
}

QPointF CloudWidget2DPaintPoints::widget_pt_to_logic_pt(const QPoint& pt) const {
  const auto lg_minx = rng_.axis_x_[0].value_, lg_miny = rng_.axis_y_[0].value_;
  const auto lg_maxx = rng_.axis_x_[1].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_width = lg_maxx - lg_minx, lg_height = lg_maxy - lg_miny;
  QRect phy_rect(QPoint(0, 0), widget_->size());

  constexpr double eps = CloudWidget2DPaintInterface::kEPS;
  QPointF logic_pt(0.0, 0.0);
  if (lg_width < eps || lg_height < eps) {
    return logic_pt;
  }

  CHECK2(widget_, "widget_ is nullptr");
  const auto right_to_left = widget_->right_to_left_;
  const auto bottom_to_top = widget_->bottom_to_top_;
  const double phy_x = right_to_left ? phy_rect.width() - pt.x() : pt.x();
  const double phy_y = bottom_to_top ? phy_rect.height() - pt.y() : pt.y();

  logic_pt.setX(lg_minx + (phy_x / phy_rect.width()) * lg_width);
  logic_pt.setY(lg_miny + (phy_y / phy_rect.height()) * lg_height);

  return logic_pt;
}

void CloudWidget2DPaintPoints::update_phy_points(const QRect& rect) {
  if (pts_.empty() || !widget_) return;

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

CloudWidget2DPaintPoints::mouse_trace_t CloudWidget2DPaintPoints::mouseTrace(const QPoint& pos) {
  mouse_trace_t trace;
  if (phy_pts_.empty()) return trace;

  const double dist_err = pt_phy_size_ * 0.5;
  for (size_t i = 0; i < phy_pts_.size(); i++) {
    const auto& pt = phy_pts_[i];
    const auto delta_x = std::abs(pt.x() - pos.x());
    const auto delta_y = std::abs(pt.y() - pos.y());
    if (delta_x <= dist_err && delta_y <= dist_err) {
      trace = mouse_trace_t{pts_[i].x_, pts_[i].y_, (int)i, true};
      break;
    }
  }

  return trace;
}

void CloudWidget2DPaintPoints::process_widget_resize(QSize size) {
  QRect rect(QPoint(0, 0), size);
  update_phy_points(rect);
}

}  // namespace test::gl_painter