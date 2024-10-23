#pragma once

// #include <memory>
#include <qlist.h>
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

class GLCloudWidget2D : public QOpenGLWidget {
  friend class CloudWidget2DPaintPoints;
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