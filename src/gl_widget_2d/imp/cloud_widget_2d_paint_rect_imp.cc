#include <QDebug>
#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include "cloud_widget_2d_paint_rect_imp.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter::imp {

CloudWidget2DPaintRectImp::CloudWidget2DPaintRectImp(test::gl_painter::GLCloudWidget2D* glWidget)
    : CloudWidget2DPaintInterfaceImp(glWidget) {}

void CloudWidget2DPaintRectImp::setData(const std::vector<paint_rect_data_t>& rects) {
  lg_rects_ = rects;
  phy_rects_ = std::vector<QRectF>(lg_rects_.size());
  CHECK2(widget_, "widget_ is nullptr");
  const auto phy_rect = widget_->paint_area_;
  update_phy_points(phy_rect);
}

void CloudWidget2DPaintRectImp::setPenStyle(Qt::PenStyle style) {
  pen_style_ = style;
}

void CloudWidget2DPaintRectImp::paint(QPainter* painter, QPaintEvent* /*event*/) {
  if (phy_rects_.empty() || ((int)(clr_idx_) >= colors_.size())) return;

  const auto& clr = colors_.at((int)clr_idx_);
  QPen pen(clr, 1, pen_style_, Qt::RoundCap, Qt::RoundJoin);
  QBrush brush(clr);

  painter->save();
  painter->setPen(pen);
  // painter->setBrush(brush);
  QFontMetrics metrics(painter->font());
  const auto txt_height = metrics.height();

  painter->drawRects(phy_rects_.data(), (int)(phy_rects_.size()));
  for (size_t i = 0; i < phy_rects_.size(); i++) {
    if (lg_rects_[i].label_.isEmpty()) continue;
    QPointF lt(phy_rects_[i].topLeft().x(), phy_rects_[i].topLeft().y() - txt_height - 3);
    const auto& text = lg_rects_[i].label_;
    const auto txt_width = metrics.horizontalAdvance(text);
    QRectF txt_rect(lt, QSizeF(txt_width, txt_height));
    painter->drawText(txt_rect, Qt::AlignVCenter | Qt::AlignLeft, text);
  }

  painter->restore();
}

mouse_trace_t CloudWidget2DPaintRectImp::mouseTrace(const QPoint& /*pos*/) {
  return mouse_trace_t();
}
QPointF CloudWidget2DPaintRectImp::lg_pt_to_phy_pt(const QPointF& /*pt*/) const {
  return QPointF();
}

void CloudWidget2DPaintRectImp::process_widget_resize(QSize /*size*/) {
  CHECK2(widget_, "widget_ is nullptr");
  const QRect rect = widget_->paint_area_;
  update_phy_points(rect);
}

void CloudWidget2DPaintRectImp::update_phy_points(const QRect& rect) {
  if (lg_rects_.empty() || !widget_) return;

  const auto phy_width = rect.width(), phy_height = rect.height();
  const auto phy_minx = rect.x(), phy_miny = rect.y();
  const auto lg_minx = rng_.axis_x_[0].value_, lg_miny = rng_.axis_y_[0].value_;
  const auto lg_maxx = rng_.axis_x_[1].value_, lg_maxy = rng_.axis_y_[1].value_;
  const auto lg_width = lg_maxx - lg_minx, lg_height = lg_maxy - lg_miny;

  CHECK2(lg_rects_.size() == phy_rects_.size(), "phy_xs_.size() != pts_.size()");
  std::vector<double> phy_lt_xs(lg_rects_.size()), phy_lt_ys(lg_rects_.size());
  std::vector<double> phy_rb_xs(lg_rects_.size()), phy_rb_ys(lg_rects_.size());

  for (size_t i = 0; i < lg_rects_.size(); i++) {
    const auto& lg_rect = lg_rects_[i].rect;
    phy_lt_xs[i] = (lg_rect.top_left_.x_ - lg_minx) / lg_width * phy_width;
    phy_lt_ys[i] = (lg_rect.top_left_.y_ - lg_miny) / lg_height * phy_height;
    phy_rb_xs[i] = (lg_rect.bottom_right_.x_ - lg_minx) / lg_width * phy_width;
    phy_rb_ys[i] = (lg_rect.bottom_right_.y_ - lg_miny) / lg_height * phy_height;
  }
  if (widget_->right_to_left_) {
    for (size_t i = 0; i < lg_rects_.size(); i++) {
      phy_lt_xs[i] = phy_width - phy_lt_xs[i];
      phy_rb_xs[i] = phy_width - phy_rb_xs[i];
      std::swap(phy_lt_xs[i], phy_rb_xs[i]);
    }
  }
  if (widget_->bottom_to_top_) {
    for (size_t i = 0; i < lg_rects_.size(); i++) {
      phy_lt_ys[i] = phy_height - phy_lt_ys[i];
      phy_rb_ys[i] = phy_height - phy_rb_ys[i];
      std::swap(phy_lt_ys[i], phy_rb_ys[i]);
    }
  }
  for (size_t i = 0; i < lg_rects_.size(); i++) {
    phy_lt_xs[i] += phy_minx, phy_lt_ys[i] += phy_miny;
    phy_rb_xs[i] += phy_minx, phy_rb_ys[i] += phy_miny;
  }

  for (size_t i = 0; i < lg_rects_.size(); i++) {
    const auto ptlt = QPointF(phy_lt_xs[i], phy_lt_ys[i]);
    const auto ptrb = QPointF(phy_rb_xs[i], phy_rb_ys[i]);
    phy_rects_[i] = QRectF(ptlt, ptrb);
  }
}

}  // namespace test::gl_painter::imp