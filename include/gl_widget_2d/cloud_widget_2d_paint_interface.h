#pragma once

#include <QObject>
#include <QPoint>
#include <QSize>

#include "gl_widget_2d_exp_def.h"

class QPainter;
class QPaintEvent;

namespace test::gl_painter::imp {
class CloudWidget2DPaintInterfaceImp;
}  // namespace test::gl_painter::imp

namespace test::gl_painter {

class GLCloudWidget2D;

class GL_WIDGET_2D_API CloudWidget2DPaintInterface : public QObject {
  Q_OBJECT
 public:
  constexpr static double kEPS = 1e-6;
  CloudWidget2DPaintInterface(GLCloudWidget2D* widget);
  virtual ~CloudWidget2DPaintInterface() = default;

 public:
  virtual void setColorIdx(size_t idx);
  virtual void setRange(double minX, double maxX, double minY, double maxY);
  test::gl_painter::imp::CloudWidget2DPaintInterfaceImp* getImpl();

  void setEnableMouseTrace(bool enable);

 public slots:
  void onWidgetResized(QSize size);

 protected:
  test::gl_painter::imp::CloudWidget2DPaintInterfaceImp* impl_{};
};

}  // namespace test::gl_painter