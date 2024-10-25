#pragma once

// #include <memory>
#include <qlist.h>
#include <qsize.h>
#include <vector>

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QList>
#include <QOpenGLWidget>
#include <QPen>
#include <QWidget>

namespace test::gl_painter {

class CloudWidget2DPaintInterface;
class CloudWidget2DPaintDash;
class CloudWidget2DPaintRect;
class CloudWidget2DPaintHorizontalDashLine;

class GLCloudWidget2D : public QOpenGLWidget {
  friend class CloudWidget2DPaintHorizontalDashLine;
  friend class CloudWidget2DPaintPoints;
  friend class CloudWidget2DPaintDash;
  friend class CloudWidget2DPaintRect;

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

 public slots:
  void animate();

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

 signals:
  void signalPaintHit(int pid, int eid, double x, double y);
  void signalSizeChanged(QSize);

 protected:
  QList<CloudWidget2DPaintInterface*> paints_;
  QList<QColor> colors_;

 protected:
  bool right_to_left_{false}, bottom_to_top_{false};

 protected:
  QBrush background_;
  QBrush circleBrush;
  QFont textFont;
  QPen circlePen;
  QPen textPen;
};

}  // namespace test::gl_painter