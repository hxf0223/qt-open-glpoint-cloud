#pragma once

#include <QPoint>

#include "cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d_exp_def.h"

namespace test::gl_painter {

class GL_WIDGET_2D_API CloudWidget2DPaintHorizontalDashLine : public CloudWidget2DPaintInterface {
 public:
  CloudWidget2DPaintHorizontalDashLine(GLCloudWidget2D* parent = nullptr);
  ~CloudWidget2DPaintHorizontalDashLine() override = default;

  void setY(double y);
};

}  // namespace test::gl_painter