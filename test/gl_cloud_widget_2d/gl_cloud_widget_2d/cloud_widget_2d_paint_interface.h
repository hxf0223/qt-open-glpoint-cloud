#pragma once

#include <memory>

#include "gl_cloud_widget_rng.h"

class QPainter;
class QPaintEvent;

namespace test::gl_painter {

class GLCloudWidget2D;

class CloudWidget2DPaintInterface {
 public:
  CloudWidget2DPaintInterface(GLCloudWidget2D* widget) : widget_(widget) {}
  virtual ~CloudWidget2DPaintInterface() = default;

  virtual void paint(QPainter* painter, QPaintEvent* event) = 0;

 public:
  virtual void setColorIdx(size_t idx) { clr_idx_ = idx; }
  virtual void setRange(gl_cw_rng_xyd rng) { rng_ = rng; }

 protected:
  gl_cw_rng_xyd rng_{};
  size_t clr_idx_{};

 protected:
  GLCloudWidget2D* widget_{};
};

}  // namespace test::gl_painter