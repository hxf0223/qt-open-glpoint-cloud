
#include <QApplication>
#include <QSurfaceFormat>

#include "main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QSurfaceFormat fmt;
  fmt.setSamples(4);
  QSurfaceFormat::setDefaultFormat(fmt);

  MainWindow window;
  window.show();
  return app.exec();
}
