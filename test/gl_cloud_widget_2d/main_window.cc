#include <algorithm>

#include <QGridLayout>
#include <QTimer>

#include "cloud_widget_2d_paint_points.h"
#include "gl_cloud_widget_2d.h"

#include "main_window.h"

using namespace test::gl_painter;

MainWindow::MainWindow() {
  setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

  glcw_2d_ = new GLCloudWidget2D(this);
  glcw_2d_->colors() << QColor(255, 0, 0) << QColor(0, 255, 0) << QColor(0, 0, 255) << QColor(255, 255, 0) << QColor(255, 0, 255);
  glcw_2d_->setRightToLeft(true);

  paint_points_ = new CloudWidget2DPaintPoints(glcw_2d_);
  paint_points_->setPhyPointSize(3.0);
  paint_points_->setColorIdx(0);
  init_points();

  glcw_2d_->addPaint(paint_points_);

  auto* layout = new QGridLayout;
  layout->addWidget(glcw_2d_, 0, 0);
  setLayout(layout);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, glcw_2d_, &GLCloudWidget2D::animate);
  timer->start(200);
}

void MainWindow::init_points() {
  std::vector<point_double_t> points;
  for (int i = 0; i < 100; i++) {
    points.emplace_back(point_double_t{i * 4.0, i * 4.0});
  }

  const auto predx = [](const point_double_t& p1, const point_double_t& p2) { return p1.x_ < p2.x_; };
  const auto predy = [](const point_double_t& p1, const point_double_t& p2) { return p1.y_ < p2.y_; };
  const auto rng_minx = std::min_element(points.begin(), points.end(), predx)->x_;
  const auto rng_maxx = std::max_element(points.begin(), points.end(), predx)->x_;
  const auto rng_miny = std::min_element(points.begin(), points.end(), predy)->y_;
  const auto rng_maxy = std::max_element(points.begin(), points.end(), predy)->y_;

  gl_cw_rng_xyd rng{{{rng_minx, -1.0}, {rng_maxx, 1.0}}, {{rng_miny, -1.0}, {rng_maxy, -1.0}}};
  paint_points_->setRange(rng);
  paint_points_->setData(points);
}