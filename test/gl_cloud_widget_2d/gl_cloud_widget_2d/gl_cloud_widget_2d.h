#pragma once

#include <memory>
#include <vector>

#include <QBrush>
#include <QColor>
#include <QFont>
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
  void setBackColor(QColor color);

  void addPaint(std::shared_ptr<CloudWidget2DPaintInterface> paint);

 public slots:
  void animate();

 protected:
  void paintEvent(QPaintEvent* event) override;

 protected:
  std::vector<std::shared_ptr<CloudWidget2DPaintInterface>> paints_;
  std::vector<QColor> colors_;

 protected:
  QBrush background_;
  QBrush circleBrush;
  QFont textFont;
  QPen circlePen;
  QPen textPen;
};

}  // namespace test::gl_painter