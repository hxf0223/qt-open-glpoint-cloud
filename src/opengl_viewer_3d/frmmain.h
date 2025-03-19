#pragma once

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class frmMain;
}
QT_END_NAMESPACE

/***********************************************
 应用程序主窗口类：
***********************************************/
class frmMain : public QWidget {
  Q_OBJECT
 public:
  frmMain(QWidget* parent = nullptr);
  ~frmMain();

 private:
  Ui::frmMain* ui;
};
