#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include "cloud_widget_2d_paint_interface_imp.h"

#include "gl_widget_2d/cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter {

GLCloudWidget2D::GLCloudWidget2D(QWidget* parent) : QOpenGLWidget(parent) {
  setMinimumSize(640, 480);
  setAutoFillBackground(false);
  setMouseTracking(true);

  brsh_background_ = QBrush(QColor(0, 0, 0));
}
GLCloudWidget2D::~GLCloudWidget2D() {
  for (auto& p : paints_) {
    delete p;
  }
}

void GLCloudWidget2D::setBackColor(QColor color) {
  brsh_background_ = QBrush(color);
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
  connect(this, &GLCloudWidget2D::signalSizeChanged, paint, &CloudWidget2DPaintInterface::onWidgetResized);
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

  painter.fillRect(event->rect(), brsh_background_);

  for (auto& p : paints_) {
    auto* impl = p->getImpl();
    impl->paint(&painter, event);
  }

  painter.end();
}

void GLCloudWidget2D::mousePressEvent(QMouseEvent* event) {
  const auto pos = event->pos();
  qDebug() << "mousePressEvent: " << pos;
}

void GLCloudWidget2D::mouseMoveEvent(QMouseEvent* event) {
  const auto pos = event->pos();
  // qDebug() << "mouseMoveEvent: " << pos;

  for (int i = 0; i < paints_.size(); i++) {
    auto* impl = paints_[i]->getImpl();
    if (!impl->isMouseTraceEnabled()) continue;
    const auto mt = impl->mouseTrace(pos);
    if (mt.flag_) {
      signalPaintHit(i, (int)(mt.eid_), mt.x_, mt.y_);
      break;
    }
  }
}

void GLCloudWidget2D::resizeEvent(QResizeEvent* event) {
  const auto size = event->size();
  QOpenGLWidget::resizeEvent(event);
  emit signalSizeChanged(size);
}

}  // namespace test::gl_painter
