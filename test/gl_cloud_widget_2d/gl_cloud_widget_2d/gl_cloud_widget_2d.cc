#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include "cloud_widget_2d_paint_interface.h"
#include "gl_cloud_widget_2d.h"

namespace test::gl_painter {

GLCloudWidget2D::GLCloudWidget2D(QWidget* parent) : QOpenGLWidget(parent) {
  setMinimumSize(640, 480);
  setAutoFillBackground(false);

  background_ = QBrush(QColor(0, 0, 0));
}
GLCloudWidget2D::~GLCloudWidget2D() {
  for (auto& p : paints_) {
    delete p;
  }
}

void GLCloudWidget2D::setBackColor(QColor color) {
  background_ = QBrush(color);
}
const QList<QColor>& GLCloudWidget2D::colors() const {
  return colors_;
}
QList<QColor>& GLCloudWidget2D::colors() {
  return colors_;
}

void GLCloudWidget2D::setRightToLeft(bool value) {
  right_to_left_ = value;
}
void GLCloudWidget2D::setBottomToTop(bool value) {
  bottom_to_top_ = value;
}

void GLCloudWidget2D::addPaint(CloudWidget2DPaintInterface* paint) {
  paints_.push_back(paint);
}
void GLCloudWidget2D::animate() {
  update();
}

void GLCloudWidget2D::paintEvent(QPaintEvent* event) {
  // const auto r = event->rect();
  // qDebug() << "paintEvent: " << r;

  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(event->rect(), background_);

  for (auto& p : paints_) {
    p->paint(&painter, event);
  }

  painter.end();
}

}  // namespace test::gl_painter