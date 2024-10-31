#pragma once

#include <qpoint.h>
#include <QColor>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QSize>


#include "gl_cloud_widget_rng.h"

class QPainter;
class QPaintEvent;

namespace test::gl_painter {
class GLCloudWidget2D;
}

namespace test::gl_painter::imp {

struct mouse_trace_t {
  double x_{}, y_{};
  int eid_{};
  bool flag_{};
};

class CloudWidget2DPaintInterfaceImp : public QObject {
 public:
  CloudWidget2DPaintInterfaceImp(test::gl_painter::GLCloudWidget2D* glWidget);
  virtual ~CloudWidget2DPaintInterfaceImp() = default;

  virtual void paint(QPainter* painter, QPaintEvent* event) = 0;
  virtual mouse_trace_t mouseTrace(const QPoint& pos) = 0;
  virtual void process_widget_resize(QSize size) = 0;
  virtual QPointF lg_pt_to_phy_pt(const QPointF& pt) const = 0;

 public:
  virtual void setColorIdx(size_t idx) { clr_idx_ = idx; }
  virtual void setRange(gl_cw_rng_xyd rng) { rng_ = rng; }

  void setEnableMouseTrace(bool enable) { enable_mouse_trace_ = enable; }
  bool isMouseTraceEnabled() const { return enable_mouse_trace_; }

  const QList<QColor>& colors() const;
  QList<QColor>& colors();

  void setEnabled(bool enable);
  bool isEnabled() const;

 protected:
  QList<QColor> colors_;

 protected:
  bool enable_mouse_trace_{false};
  bool enabled_{true};

 protected:
  gl_cw_rng_xyd rng_{};
  size_t clr_idx_{};

 protected:
  GLCloudWidget2D* widget_{};
};

}  // namespace test::gl_painter::imp