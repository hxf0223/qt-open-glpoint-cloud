#pragma once

#include <qobject.h>
#include <qobjectdefs.h>
#include <qsize.h>
#include <tuple>

#include <QObject>
#include <QPoint>

#include "gl_cloud_widget_rng.h"

class QPainter;
class QPaintEvent;

namespace test::gl_painter {

class GLCloudWidget2D;

class CloudWidget2DPaintInterface : public QObject {
  //Q_OBJECT
 public:
  constexpr static double kEPS = 1e-6;
  CloudWidget2DPaintInterface(GLCloudWidget2D* widget) : QObject{}, widget_(widget) {}
  virtual ~CloudWidget2DPaintInterface() = default;

  virtual void paint(QPainter* painter, QPaintEvent* event) = 0;

 public:
  struct mouse_trace_t {
    double x_{}, y_{};
    int eid_{};
    bool flag_{};
  };

 public:
  virtual void setColorIdx(size_t idx) { clr_idx_ = idx; }
  virtual void setRange(gl_cw_rng_xyd rng) { rng_ = rng; }

  void setEnableMouseTrace(bool enable) { enable_mouse_trace_ = enable; }
  bool isMouseTraceEnabled() const { return enable_mouse_trace_; }
  virtual mouse_trace_t mouseTrace(const QPoint& pos) = 0;

 public slots:
  void onWidgetResized(QSize size) { process_widget_resize(size); }

 protected:
  virtual void process_widget_resize(QSize size) = 0;

 protected:
  bool enable_mouse_trace_{false};

 protected:
  gl_cw_rng_xyd rng_{};
  size_t clr_idx_{};

 protected:
  GLCloudWidget2D* widget_{};
};

}  // namespace test::gl_painter