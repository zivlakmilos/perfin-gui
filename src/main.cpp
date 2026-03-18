#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName(QObject::tr("Perfin"));
  QCoreApplication::setApplicationVersion(QObject::tr("1.0.0"));

  MainWindow mainWindow;
  mainWindow.show();

  app.exec();
  return 0;
}
