#include "MainWindow.h"

#include <QtWidgets>

#include "DWRibbon.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  setupUi();
}

MainWindow::~MainWindow(void)
{
}

void MainWindow::setupUi(void)
{
  m_dwRibbon = new DWRibbon(this);
  addDockWidget(Qt::TopDockWidgetArea, m_dwRibbon);
}
