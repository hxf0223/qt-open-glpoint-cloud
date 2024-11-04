#include <algorithm>

#include <QDebug>
#include <QPainter>

#include "gl_widget_2d/cloud_widget_2d_paint_dash.h"
#include "gl_widget_2d/cloud_widget_2d_paint_points.h"
#include "gl_widget_2d/cloud_widget_2d_paint_rect.h"

#include <qchar.h>
#include "chart_tooltip.h"
#include "gl_widget.h"

namespace test::gl_painter {

GLWidget::GLWidget(QWidget* parent) : GLCloudWidget2D(parent) {
  init_painters();
  init_points();
  init_callout();
}

void GLWidget::resizeEvent(QResizeEvent* event) {
  GLCloudWidget2D::resizeEvent(event);
  forceUpdate();
}
void GLWidget::showEvent(QShowEvent* event) {
  GLCloudWidget2D::showEvent(event);
  forceUpdate();
}

void GLWidget::init_painters() {
  this->setContentsMargins(QMargins(0, 0, 0, 0));
  this->enablePaintTrace(true);
  this->setRightToLeft(true);

  {
    paint_points_ = new CloudWidget2DPaintPoints(this);
    paint_points_->setEnableMouseTrace(true);
    paint_points_->setPhyPointSize(4.0);
    paint_points_->colors() << QColor(255, 0, 0);

    paint_dash_line_ = new CloudWidget2DPaintHorizontalDashLine(this);
    paint_dash_line_->colors() << QColor(0, 0, 255);

    paint_rect_ = new CloudWidget2DPaintRect(this);
    paint_rect_->colors() << QColor(Qt::magenta);

    this->addPaint(paint_points_);
    this->addPaint(paint_dash_line_);
    this->addPaint(paint_rect_);
  }
}

void GLWidget::init_points() {
  constexpr size_t kNumPoints = 20;
  std::vector<point_double_t> points(kNumPoints);
  std::vector<paint_rect_data_t> rects(kNumPoints);
  for (size_t i = 0; i < kNumPoints; i++) {
    const point_double_t cp{i * 4.0, i * 4.0};
    rects[i].rect = rect_double_t{cp - 3.0, cp + 3.0};
    rects[i].label_ = tr("测试 ").append(QString::number(i + 1));
    points[i] = cp;
  }

  const auto predx = [](const point_double_t& p1, const point_double_t& p2) { return p1.x_ < p2.x_; };
  const auto predy = [](const point_double_t& p1, const point_double_t& p2) { return p1.y_ < p2.y_; };
  const auto rng_minx = std::min_element(points.begin(), points.end(), predx)->x_;
  const auto rng_maxx = std::max_element(points.begin(), points.end(), predx)->x_;
  const auto rng_miny = std::min_element(points.begin(), points.end(), predy)->y_;
  const auto rng_maxy = std::max_element(points.begin(), points.end(), predy)->y_;

  paint_points_->setRange(rng_minx, rng_maxx, rng_miny, rng_maxy);
  paint_points_->setData(std::move(points));

  paint_dash_line_->setRange(rng_minx, rng_maxx, rng_miny, rng_maxy);
  paint_dash_line_->setY((rng_miny + rng_maxy) / 4);

  paint_rect_->setRange(rng_minx, rng_maxx, rng_miny, rng_maxy);
  paint_rect_->setData(rects);
}

void GLWidget::init_callout() {
  callout_ = new Callout(this, 0);

  connect(this, &GLCloudWidget2D::signalPaintHit, [this](int pid, int eid, double x, double y) {
    qDebug() << "paint id " << pid << " data id " << eid << " lpos " << x << ", " << y;
    hit_info_ = paint_hit_info_t{pid, eid, QPointF(x, y)};
    if (hit_info_.paint_id_ >= 0) {
      QString str1 = QString(tr("paint id %1, data id %2")).arg(hit_info_.paint_id_).arg(hit_info_.eid_);
      QString str2 = QString(tr("logic xy: %3, %4")).arg(x).arg(y);
      callout_->setText(str1 + "\n" + str2);
      update();
    } else {
      update();
    }
  });
}

void GLWidget::paintEvent(QPaintEvent* event) {
  GLCloudWidget2D::paintEvent(event);

  if (enable_paint_trace_) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (hit_info_.paint_id_ >= 0) {
      callout_->setAnchor(hit_info_.lg_pt_);
      callout_->paint(&painter, event);
    }

    painter.end();
  }
}

}  // namespace test::gl_painter