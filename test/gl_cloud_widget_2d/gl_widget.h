#pragma once

#include <qpoint.h>
#include "gl_widget_2d/gl_cloud_widget_2d.h"

namespace test::gl_painter {

class GLCloudWidget2D;
class CloudWidget2DPaintPoints;
class CloudWidget2DPaintHorizontalDashLine;
class CloudWidget2DPaintRect;

class Callout;

}  // namespace test::gl_painter

namespace test::gl_painter {

class GLWidget : public GLCloudWidget2D {
  Q_OBJECT
 public:
  GLWidget(QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;

 protected:
  void init_painters();
  void init_points();
  void init_callout();

 protected:
  struct paint_hit_info_t {
    int32_t paint_id_{-1}, eid_{};
    QPointF lg_pt_{};
  };

 protected:
  test::gl_painter::CloudWidget2DPaintPoints* paint_points_{};
  test::gl_painter::CloudWidget2DPaintHorizontalDashLine* paint_dash_line_{};
  test::gl_painter::CloudWidget2DPaintRect* paint_rect_{};

  paint_hit_info_t hit_info_{};
  Callout* callout_{};
};

}  // namespace test::gl_painter