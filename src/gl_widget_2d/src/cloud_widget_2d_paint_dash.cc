#include <QColor>
#include <QList>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>

#include "3rd_utils.h"

#include "cloud_widget_2d_paint_dash_imp.h"

#include "gl_widget_2d/cloud_widget_2d_paint_dash.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

using namespace test::gl_painter::imp;

namespace test::gl_painter {

CloudWidget2DPaintHorizontalDashLine::CloudWidget2DPaintHorizontalDashLine(GLCloudWidget2D* widget)
    : CloudWidget2DPaintInterface(widget) {
  impl_ = new CloudWidget2DPaintHorizontalDashLineImp(widget);
}

void CloudWidget2DPaintHorizontalDashLine::setY(double y) {
  auto* impl = dynamic_cast<CloudWidget2DPaintHorizontalDashLineImp*>(impl_);
  CHECK2(impl, "Invalid implementation");
  impl->setY(y);
}

}  // namespace test::gl_painter