#pragma once

#include <QOpenGLWidget>

class Helper;

class GLWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  GLWidget(QWidget* parent);

 public slots:
  void animate();

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  int elapsed;
};