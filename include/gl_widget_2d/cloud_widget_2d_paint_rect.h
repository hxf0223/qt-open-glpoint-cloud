#pragma once

#include <qchar.h>
#include <vector>

#include <QPoint>
#include <QPointF>
#include <QRect>

#include "cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d_exp_def.h"
#include "point_typedef.h"

namespace test::gl_painter {

class GL_WIDGET_2D_API CloudWidget2DPaintRect : public CloudWidget2DPaintInterface {
 public:
  CloudWidget2DPaintRect(GLCloudWidget2D* widget);
  ~CloudWidget2DPaintRect() override = default;

  void setPenStyle(Qt::PenStyle style);
  void setData(const std::vector<paint_rect_data_t>& rects);
};

}  // namespace test::gl_painter