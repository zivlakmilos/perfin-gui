#include <QApplication>

#include "Database.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName(QObject::tr("Perfin"));
  QCoreApplication::setApplicationVersion(QObject::tr("1.0.0"));

  setupDatabase();

  MainWindow mainWindow;
  mainWindow.show();

  app.exec();
  return 0;
}
