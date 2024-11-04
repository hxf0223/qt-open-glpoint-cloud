#pragma once

#include <QObject>
#include <QPoint>
#include <QSize>

#include "cloud_widget_2d_paint_interface_imp.h"
#include "gl_widget_2d/cloud_widget_2d_paint_points.h"

class QPainter;
class QPaintEvent;

namespace test::gl_painter {
class GLCloudWidget2D;
}

namespace test::gl_painter::imp {

class CloudWidget2DPaintPointsImp : public CloudWidget2DPaintInterfaceImp {
 public:
  CloudWidget2DPaintPointsImp(test::gl_painter::GLCloudWidget2D* glWidget);
  virtual ~CloudWidget2DPaintPointsImp() = default;

  void setData(const std::vector<point_double_t>& pts);
  void setData(std::vector<point_double_t>&& pts);
  void setPhyPointSize(double size);

  void paint(QPainter* painter, QPaintEvent* event) override;
  mouse_trace_t mouseTrace(const QPoint& pos) override;
  void process_widget_resize(QSize size) override;
  QPointF lg_pt_to_phy_pt(const QPointF& pt) const override;

 protected:
  QPointF widget_pt_to_logic_pt(const QPoint& pt) const;
  void update_phy_points(const QRect& rect);

 protected:
  double pt_phy_size_{1.0};
  std::vector<point_double_t> pts_;
  std::vector<QPointF> phy_pts_;
};

}  // namespace test::gl_painter::imp