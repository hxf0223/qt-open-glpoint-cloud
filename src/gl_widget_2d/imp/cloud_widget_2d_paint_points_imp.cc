#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include <qdebug.h>
#include "cloud_widget_2d_paint_points_imp.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter::imp {

CloudWidget2DPaintPointsImp::CloudWidget2DPaintPointsImp(test::gl_painter::GLCloudWidget2D* glWidget)
    : CloudWidget2DPaintInterfaceImp(glWidget) {}

void CloudWidget2DPaintPointsImp::setData(const std::vector<point_double_t>& pts) {
  CHECK2(widget_, "widget_ is nullptr");
  phy_pts_ = std::vector<QPointF>(pts.size());
  pts_ = pts;

  const auto rect = widget_->paint_area_;
  update_phy_points(rect);
}
void CloudWidget2DPaintPointsImp::setPhyPointSize(double size) {
  pt_phy_size_ = size;
}

void CloudWidget2DPaintPointsImp::paint(QPainter* painter, QPaintEvent* /*event*/) {
  if (pts_.empty() || ((int)(clr_idx_) >= colors_.size())) return;

  const auto& clr = colors_.at((int)clr_idx_);
  QPen pen(clr, pt_phy_size_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  QBrush brush(clr);

  painter->save();
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawPoints(phy_pts_.data(), (int)phy_pts_.size());

  painter->restore();
}

QPointF CloudWidget2DPaintPointsImp::widget_pt_to_logic_pt(const QPoint& pt) const {
  const auto lg_minx = rng_.axis_x_[0].value_, lg_miny = rng_.axis_y_[0].value_;
  const auto lg_maxx = rng_.axis_x_[1].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_width = lg_maxx - lg_minx, lg_height = lg_maxy - lg_miny;

  CHECK2(widget_, "widget_ is nullptr");
  const QRect phy_rect = widget_->paint_area_;
  const auto phy_pt = QPointF(pt.x() - phy_rect.x(), pt.y() - phy_rect.y());

  constexpr double eps = CloudWidget2DPaintInterface::kEPS;
  QPointF logic_pt(0.0, 0.0);
  if (lg_width < eps || lg_height < eps) {
    return logic_pt;
  }

  const auto right_to_left = widget_->right_to_left_;
  const auto bottom_to_top = widget_->bottom_to_top_;
  const double phy_x = right_to_left ? phy_rect.width() - phy_pt.x() : phy_pt.x();
  const double phy_y = bottom_to_top ? phy_rect.height() - phy_pt.y() : phy_pt.y();

  logic_pt.setX(lg_minx + (phy_x / phy_rect.width()) * lg_width);
  logic_pt.setY(lg_miny + (phy_y / phy_rect.height()) * lg_height);

  return logic_pt;
}

void CloudWidget2DPaintPointsImp::update_phy_points(const QRect& rect) {
  if (pts_.empty() || !widget_) return;

  const auto phy_width = rect.width(), phy_height = rect.height();
  const auto phy_minx = rect.x(), phy_miny = rect.y();
  // const auto phy_maxx = phy_minx + phy_width, phy_maxy = phy_miny + phy_height;
  const auto lg_minx = rng_.axis_x_[0].value_, lg_miny = rng_.axis_y_[0].value_;
  const auto lg_maxx = rng_.axis_x_[1].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_width = lg_maxx - lg_minx, lg_height = lg_maxy - lg_miny;

  if (lg_width < CloudWidget2DPaintInterface::kEPS || lg_height < CloudWidget2DPaintInterface::kEPS) {
    qDebug() << "lg_width < CloudWidget2DPaintInterface::kEPS || lg_height < CloudWidget2DPaintInterface::kEPS";
    for (auto& pt : phy_pts_) {
      pt = QPointF(0.0, 0.0);
    }

    return;
  }

  CHECK2(phy_pts_.size() == pts_.size(), "phy_pts_.size() != pts_.size()");

  for (size_t i = 0; i < pts_.size(); i++) {
    const auto phy_x = (pts_[i].x_ - lg_minx) / lg_width * phy_width;
    const auto phy_y = (pts_[i].y_ - lg_miny) / lg_height * phy_height;
    phy_pts_[i] = QPointF(phy_x, phy_y);
  }
  if (widget_->right_to_left_) {
    for (size_t i = 0; i < pts_.size(); i++) {
      phy_pts_[i].setX(phy_width - phy_pts_[i].x());
    }
  }
  if (widget_->bottom_to_top_) {
    for (size_t i = 0; i < pts_.size(); i++) {
      phy_pts_[i].setY(phy_height - phy_pts_[i].y());
    }
  }

  for (auto& pt : phy_pts_) {
    pt.setX(pt.x() + phy_minx);
    pt.setY(pt.y() + phy_miny);
  }
}

mouse_trace_t CloudWidget2DPaintPointsImp::mouseTrace(const QPoint& pos) {
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

void CloudWidget2DPaintPointsImp::process_widget_resize(QSize /*size*/) {
  CHECK2(widget_, "widget_ is nullptr");
  const QRect rect = widget_->paint_area_;
  update_phy_points(rect);
}

QPointF CloudWidget2DPaintPointsImp::lg_pt_to_phy_pt(const QPointF& pt) const {
  CHECK2(widget_, "widget_ is nullptr");
  const auto lg_width = rng_.axis_x_[1].value_ - rng_.axis_x_[0].value_;
  const auto lg_height = rng_.axis_y_[1].value_ - rng_.axis_y_[0].value_;

  const auto phy_rect = widget_->paint_area_;
  const auto phy_width = phy_rect.width(), phy_height = phy_rect.height();

  constexpr double eps = CloudWidget2DPaintInterface::kEPS;
  if (lg_width < eps || lg_height < eps) {
    qDebug() << "lg_width < eps || lg_height < eps";
    return QPointF(0.0, 0.0);
  }

  double phyx = (pt.x() - rng_.axis_x_[0].value_) / lg_width * phy_width;
  double phyy = (pt.y() - rng_.axis_y_[0].value_) / lg_height * phy_height;
  if (widget_->right_to_left_) phyx = phy_width - phyx;
  if (widget_->bottom_to_top_) phyy = phy_height - phyy;

  phyx += phy_rect.x(), phyy += phy_rect.y();
  return QPointF(phyx, phyy);
}

}  // namespace test::gl_painter::imp