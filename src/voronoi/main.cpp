#include "mainwindow.h"
#include <QApplication>
#include "../cg/widget.h"

using namespace std;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow m;
  Widget w;

  w.show();
  m.show();
  return a.exec();
}

