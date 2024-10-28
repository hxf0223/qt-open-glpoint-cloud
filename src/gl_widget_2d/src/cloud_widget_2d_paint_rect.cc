#include <QDebug>
#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

#include "3rd_utils.h"

#include "cloud_widget_2d_paint_rect_imp.h"

#include "gl_widget_2d/cloud_widget_2d_paint_rect.h"
#include "gl_widget_2d/gl_cloud_widget_2d.h"

using namespace test::gl_painter::imp;

namespace test::gl_painter {

CloudWidget2DPaintRect::CloudWidget2DPaintRect(GLCloudWidget2D* parent) : CloudWidget2DPaintInterface(parent) {
  impl_ = new CloudWidget2DPaintRectImp(parent);
}

void CloudWidget2DPaintRect::setData(const std::vector<paint_rect_data_t>& rects) {
  auto* impl = dynamic_cast<CloudWidget2DPaintRectImp*>(impl_);
  CHECK2(impl, "impl is null");
  impl->setData(rects);
}

void CloudWidget2DPaintRect::setPenStyle(Qt::PenStyle style) {
  auto* impl = dynamic_cast<CloudWidget2DPaintRectImp*>(impl_);
  CHECK2(impl, "impl is null");
  impl->setPenStyle(style);
}

}  // namespace test::gl_painter