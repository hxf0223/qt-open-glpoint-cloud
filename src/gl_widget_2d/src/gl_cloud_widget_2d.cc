#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include "cloud_widget_2d_paint_interface_imp.h"

#include <qopenglwidget.h>
#include "gl_widget_2d/cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter {

GLCloudWidget2D::GLCloudWidget2D(QWidget* parent) : QOpenGLWidget(parent) {
  setMinimumSize(640, 480);
  setAutoFillBackground(false);
  setMouseTracking(true);

  brsh_background_ = QBrush(QColor(0, 0, 0));
  paint_area_ = this->rect();
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
void GLCloudWidget2D::enablePaintTrace(bool enable) {
  enable_paint_trace_ = enable;
}
void GLCloudWidget2D::forceUpdate() {
  const auto sz = this->size();
  for (auto& p : paints_) {
    p->getImpl()->process_widget_resize(sz);
  }

  this->update();
}

void GLCloudWidget2D::addPaint(CloudWidget2DPaintInterface* paint) {
  connect(this, &GLCloudWidget2D::signalSizeChanged, paint, &CloudWidget2DPaintInterface::onWidgetResized);
  paints_.push_back(paint);
}
void GLCloudWidget2D::animate() {
  forceUpdate();
}

QPointF GLCloudWidget2D::mapToPosition(const QPointF& pos, const CloudWidget2DPaintInterface* paint) const {
  const auto paint_it = std::find(paints_.begin(), paints_.end(), paint);
  if (paint_it == paints_.end()) return QPointF();

  const auto paint_imp = (*paint_it)->getImpl();
  return paint_imp->lg_pt_to_phy_pt(pos);
}
QPointF GLCloudWidget2D::mapToPosition(const QPointF& pos, int paintIdx) const {
  if (paintIdx < 0 || paintIdx >= paints_.size()) return QPointF();
  const auto paint_imp = paints_[paintIdx]->getImpl();
  return paint_imp->lg_pt_to_phy_pt(pos);
}

void GLCloudWidget2D::setMargin(int left, int top, int right, int bottom) {
  if (left < 0 || top < 0 || right < 0 || bottom < 0) return;
  if ((left + right) >= this->width()) return;
  if ((top + bottom) >= this->height()) return;

  margin_left_ = left, margin_top_ = top;
  margin_right_ = right, margin_bottom_ = bottom;
  paint_area_ = QRect(margin_left_, margin_top_, this->width(), this->height());
  paint_area_.setWidth(paint_area_.width() - margin_left_ - margin_right_);
  paint_area_.setHeight(paint_area_.height() - margin_top_ - margin_bottom_);

  setMinimumSize(left + right + 1, top + bottom + 1);
  forceUpdate();
}

void GLCloudWidget2D::paintEvent(QPaintEvent* event) {
  // const auto r = event->rect();
  // qDebug() << "paintEvent: " << r;

  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(event->rect(), brsh_background_);

  for (auto& p : paints_) {
    if (!p->isEnabled()) continue;
    auto* impl = p->getImpl();
    impl->paint(&painter, event);
  }

  painter.end();
}

void GLCloudWidget2D::mousePressEvent(QMouseEvent* event) {
  QOpenGLWidget::mousePressEvent(event);
  // const auto pos = event->pos();
}

void GLCloudWidget2D::mouseMoveEvent(QMouseEvent* event) {
  QOpenGLWidget::mouseMoveEvent(event);
  const auto pos = event->pos();
  // qDebug() << "mouseMoveEvent: " << pos;

  if (enable_paint_trace_) {
    bool hit_flag{false};
    for (int i = 0; i < paints_.size() && !hit_flag; i++) {
      auto* impl = paints_[i]->getImpl();
      if (!impl->isMouseTraceEnabled()) continue;
      const auto mt = impl->mouseTrace(pos);
      if (mt.flag_) {
        hit_flag = true;
        emit signalPaintHit(i, (int)(mt.eid_), mt.x_, mt.y_);
      }
    }

    if (!hit_flag) {
      emit signalPaintHit(-1, -1, 0.0, 0.0);
    }
  }
}

void GLCloudWidget2D::resizeEvent(QResizeEvent* event) {
  const auto size = event->size();
  paint_area_ = QRect(margin_left_, margin_top_, size.width(), size.height());
  paint_area_.setWidth(size.width() - margin_left_ - margin_right_);
  paint_area_.setHeight(size.height() - margin_top_ - margin_bottom_);

  const auto paint_sz = paint_area_.size();
  QOpenGLWidget::resizeEvent(event);
  emit signalSizeChanged(paint_sz);
}

}  // namespace test::gl_painter
