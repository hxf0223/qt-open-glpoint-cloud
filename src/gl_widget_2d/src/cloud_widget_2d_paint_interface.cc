
#include "cloud_widget_2d_paint_interface_imp.h"

#include "gl_cloud_widget_rng.h"
#include "gl_widget_2d/cloud_widget_2d_paint_interface.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter {

CloudWidget2DPaintInterface::CloudWidget2DPaintInterface(GLCloudWidget2D* widget) : QObject(widget) {}

void CloudWidget2DPaintInterface::setColorIdx(size_t idx) {
  impl_->setColorIdx(idx);
}
void CloudWidget2DPaintInterface::setRange(double minX, double maxX, double minY, double maxY) {
  imp::gl_cw_rng_xyd rnd = {{{minX, -1.0}, {maxX, -1.0}}, {{minY, -1.0}, {maxY, -1.0}}};
  impl_->setRange(rnd);
}

void CloudWidget2DPaintInterface::setEnableMouseTrace(bool enable) {
  impl_->setEnableMouseTrace(enable);
}

void CloudWidget2DPaintInterface::onWidgetResized(QSize size) {
  impl_->process_widget_resize(size);
}

void CloudWidget2DPaintInterface::setEnabled(bool enable) {
  impl_->setEnabled(enable);
}
bool CloudWidget2DPaintInterface::isEnabled() const {
  return impl_->isEnabled();
}

test::gl_painter::imp::CloudWidget2DPaintInterfaceImp* CloudWidget2DPaintInterface::getImpl() {
  return impl_;
}

}  // namespace test::gl_painter