#include <QApplication>
#include <QSurfaceFormat>

//#include "spdlog/spdlog.h"

#include "main_window.h"

int main(int argc, char* argv[]) {

#if 0
#ifdef NDEBUG
  spdlog::set_level(spdlog::level::warn);  // disable spdlog for performance test
#else
  spdlog::set_level(spdlog::level::info);
  spdlog::set_pattern("%H:%M:%S.%e %t %s %! %v");
#endif
#endif

  QApplication app(argc, argv);

  QSurfaceFormat fmt;
  fmt.setSamples(4);
  QSurfaceFormat::setDefaultFormat(fmt);

  MainWindow window;
  window.show();
  return app.exec();
}
