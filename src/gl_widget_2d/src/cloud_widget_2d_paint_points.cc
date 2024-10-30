#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include "cloud_widget_2d_paint_points_imp.h"

#include "gl_widget_2d/cloud_widget_2d_paint_points.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter {

using namespace test::gl_painter::imp;

CloudWidget2DPaintPoints::CloudWidget2DPaintPoints(GLCloudWidget2D* widget) : CloudWidget2DPaintInterface(widget) {
  impl_ = new CloudWidget2DPaintPointsImp(widget);
}
CloudWidget2DPaintPoints::~CloudWidget2DPaintPoints() {
  delete impl_;
}

void CloudWidget2DPaintPoints::setData(const std::vector<point_double_t>& pts) {
  auto* impl = dynamic_cast<CloudWidget2DPaintPointsImp*>(impl_);
  CHECK2(impl, "impl is nullptr");
  impl->setData(pts);
}
void CloudWidget2DPaintPoints::setPhyPointSize(double size) {
  auto* impl = dynamic_cast<CloudWidget2DPaintPointsImp*>(impl_);
  CHECK2(impl, "impl is nullptr");
  impl->setPhyPointSize(size);
}

}  // namespace test::gl_painter