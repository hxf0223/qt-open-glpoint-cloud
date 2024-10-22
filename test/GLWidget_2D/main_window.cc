#include <QGridLayout>
#include <QTimer>

#include "glwidget.h"
#include "main_window.h"

Window::Window() {
  setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

  GLWidget* openGL = new GLWidget(this);

  QGridLayout* layout = new QGridLayout;
  layout->addWidget(openGL, 0, 0);
  setLayout(layout);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
  timer->start(50);
}