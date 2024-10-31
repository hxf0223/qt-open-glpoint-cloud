#include <QDebug>
#include <QGridLayout>
#include <QTimer>

#include "gl_widget.h"

#include "main_window.h"

using namespace test::gl_painter;

MainWindow::MainWindow() {
  setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));

  glcw_2d_ = new GLWidget(this);

  auto* layout = new QGridLayout;
  layout->setContentsMargins(QMargins(0, 0, 0, 0));
  layout->setMargin(0);

  layout->addWidget(glcw_2d_, 0, 0);
  setLayout(layout);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, glcw_2d_, &GLCloudWidget2D::animate);
  timer->start(30);
}

void MainWindow::showEvent(QShowEvent* event) {
  // painter 要求在resize事件触发之前，设置好logic -> phy映射关系
  // 但是实际应用不一定满足这个顺序，可以在显示时使用forceUpdate强制刷新
  glcw_2d_->forceUpdate();
  QWidget::showEvent(event);
}