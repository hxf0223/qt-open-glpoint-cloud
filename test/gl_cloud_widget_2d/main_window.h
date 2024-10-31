#pragma once

#include <QWidget>

namespace test::gl_painter {

class GLWidget;

}  // namespace test::gl_painter

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow();

 protected:
  void showEvent(QShowEvent* event) override;
  void init_points();

 protected:
  test::gl_painter::GLWidget* glcw_2d_;
};