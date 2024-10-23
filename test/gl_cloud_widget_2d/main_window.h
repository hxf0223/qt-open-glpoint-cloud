#pragma once

#include <QWidget>

namespace test::gl_painter {

class GLCloudWidget2D;
class CloudWidget2DPaintPoints;

}  // namespace test::gl_painter

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow();

 protected:
  void init_points();

 protected:
  test::gl_painter::GLCloudWidget2D* glcw_2d_;
  test::gl_painter::CloudWidget2DPaintPoints* paint_points_;
};