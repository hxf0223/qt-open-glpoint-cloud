
#include "cloud_widget_2d_paint_interface_imp.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter::imp {

CloudWidget2DPaintInterfaceImp::CloudWidget2DPaintInterfaceImp(test::gl_painter::GLCloudWidget2D* glWidget)
    : QObject(glWidget), widget_(glWidget) {}

void CloudWidget2DPaintInterfaceImp::setEnabled(bool enable) {
  enabled_ = enable;
}
bool CloudWidget2DPaintInterfaceImp::isEnabled() const {
  return enabled_;
}

}  // namespace test::gl_painter::imp