#pragma once

// #include <memory>
//#include <vector>

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QList>
#include <QOpenGLWidget>
#include <QPen>
#include <QPointF>
#include <QWidget>

#include "gl_widget_2d_exp_def.h"

namespace test::gl_painter::imp {
class CloudWidget2DPaintInterfaceImp;
class CloudWidget2DPaintPointsImp;
class CloudWidget2DPaintRectImp;
class CloudWidget2DPaintDashImp;
class CloudWidget2DPaintHorizontalDashLineImp;
}  // namespace test::gl_painter::imp

namespace test::gl_painter {

class CloudWidget2DPaintInterface;

class GL_WIDGET_2D_API GLCloudWidget2D : public QOpenGLWidget {
  friend class test::gl_painter::imp::CloudWidget2DPaintInterfaceImp;
  friend class test::gl_painter::imp::CloudWidget2DPaintPointsImp;
  friend class test::gl_painter::imp::CloudWidget2DPaintRectImp;
  friend class test::gl_painter::imp::CloudWidget2DPaintDashImp;
  friend class test::gl_painter::imp::CloudWidget2DPaintHorizontalDashLineImp;

  Q_OBJECT

 public:
  GLCloudWidget2D(QWidget* parent);
  ~GLCloudWidget2D() override;
  void setBackColor(QColor color);

  void addPaint(CloudWidget2DPaintInterface* paint);
  const QList<QColor>& colors() const;
  QList<QColor>& colors();

  void setRightToLeft(bool value);
  void setBottomToTop(bool value);
  void enablePaintTrace(bool enable);

  QPointF mapToPosition(const QPointF& pos, const CloudWidget2DPaintInterface* paint) const;
  QPointF mapToPosition(const QPointF& pos, int paintIdx) const;

  // the paint's resize update need set logic -> phy map first,
  // otherwise the resize update will be wrong.
  // so a new API provided to force update logic -> phy map.
  void forceUpdate();

 public Q_SLOTS:
  void animate();

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

 Q_SIGNALS:
  void signalPaintHit(int pid, int eid, double x, double y);
  void signalSizeChanged(QSize);

 protected:
  QList<CloudWidget2DPaintInterface*> paints_;
  QList<QColor> colors_;

 protected:
  bool right_to_left_{false}, bottom_to_top_{false};
  bool enable_paint_trace_{false};

 protected:
  QBrush brsh_background_;
};

}  // namespace test::gl_painter