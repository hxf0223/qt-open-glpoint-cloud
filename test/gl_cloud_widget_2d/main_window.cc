#include <algorithm>

#include <QGridLayout>
#include <QTimer>

#include "gl_widget_2d/cloud_widget_2d_paint_dash.h"
#include "gl_widget_2d/cloud_widget_2d_paint_points.h"
#include "gl_widget_2d/cloud_widget_2d_paint_rect.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

#include "main_window.h"

using namespace test::gl_painter;

MainWindow::MainWindow() {
  setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

  glcw_2d_ = new GLCloudWidget2D(this);
  glcw_2d_->colors() << QColor(255, 0, 0) << QColor(0, 255, 0) << QColor(0, 0, 255) << QColor(255, 255, 0) << QColor(255, 0, 255);
  glcw_2d_->setRightToLeft(true);

  {
    paint_points_ = new CloudWidget2DPaintPoints(glcw_2d_);
    paint_points_->setEnableMouseTrace(true);
    paint_points_->setPhyPointSize(4.0);
    paint_points_->setColorIdx(0);

    paint_dash_line_ = new CloudWidget2DPaintHorizontalDashLine(glcw_2d_);
    paint_dash_line_->setColorIdx(1);

    paint_rect_ = new CloudWidget2DPaintRect(glcw_2d_);
    paint_rect_->setColorIdx(2);

    glcw_2d_->addPaint(paint_points_);
    glcw_2d_->addPaint(paint_dash_line_);
    glcw_2d_->addPaint(paint_rect_);

    init_points();
  }

  auto* layout = new QGridLayout;
  layout->addWidget(glcw_2d_, 0, 0);
  setLayout(layout);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, glcw_2d_, &GLCloudWidget2D::animate);
  timer->start(30);
}

void MainWindow::init_points() {
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
  paint_points_->setData(points);

  paint_dash_line_->setRange(rng_minx, rng_maxx, rng_miny, rng_maxy);
  paint_dash_line_->setY((rng_miny + rng_maxy) / 3);

  paint_rect_->setRange(rng_minx, rng_maxx, rng_miny, rng_maxy);
  paint_rect_->setData(rects);
}