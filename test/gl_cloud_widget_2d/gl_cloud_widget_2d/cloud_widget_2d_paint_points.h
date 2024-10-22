#pragma once

#include <vector>

#include "cloud_widget_2d_paint_interface.h"
#include "point_typedef.h"

namespace test::gl_painter {

class CloudWidget2DPaintPoints : public CloudWidget2DPaintInterface {
 public:
  CloudWidget2DPaintPoints(GLCloudWidget2D* widget);
  ~CloudWidget2DPaintPoints() override = default;

  void setData(const std::vector<point_double_t>& pts);
  void setPhyPointSize(double size);

  void paint(QPainter* painter, QPaintEvent* event) override;

 protected:
  double pt_phy_size_{1.0};
  std::vector<point_double_t> pts_;
};

}  // namespace test::gl_painter