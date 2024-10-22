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

void GLCloudWidget2D::setBackColor(QColor color) {
  background_ = QBrush(color);
}
void GLCloudWidget2D::addPaint(std::shared_ptr<CloudWidget2DPaintInterface> paint) {
  paints_.push_back(paint);
}
void GLCloudWidget2D::animate() {
  update();
}

void GLCloudWidget2D::paintEvent(QPaintEvent* event) {
  const auto r = event->rect();
  qDebug() << "paintEvent: " << r;

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