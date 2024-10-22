#include <QGridLayout>
#include <QTimer>

#include <qboxlayout.h>
#include "gl_cloud_widget_2d.h"
#include "main_window.h"

using namespace test::gl_painter;

Window::Window() {
  setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

  auto* openGL = new GLCloudWidget2D(this);

  auto* layout = new QGridLayout;
  layout->addWidget(openGL, 0, 0);
  setLayout(layout);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, openGL, &GLCloudWidget2D::animate);
  timer->start(200);
}