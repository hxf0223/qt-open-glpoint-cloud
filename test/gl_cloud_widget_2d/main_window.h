#pragma once

#include <QWidget>

namespace test::gl_painter {

class GLCloudWidget2D;
class CloudWidget2DPaintPoints;
class CloudWidget2DPaintHorizontalDashLine;
class CloudWidget2DPaintRect;

}  // namespace test::gl_painter

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow();

 protected:
  void showEvent(QShowEvent* event) override;
  void init_points();

 protected:
  test::gl_painter::GLCloudWidget2D* glcw_2d_;
  test::gl_painter::CloudWidget2DPaintPoints* paint_points_;
  test::gl_painter::CloudWidget2DPaintHorizontalDashLine* paint_dash_line_;
  test::gl_painter::CloudWidget2DPaintRect* paint_rect_;
};