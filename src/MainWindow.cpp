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

  m_twCentralWidget = new QTabWidget(this);
  m_twCentralWidget->setTabsClosable(true);
  setCentralWidget(m_twCentralWidget);

  m_lblStatus = new QLabel(this);
  m_lblStatus->setText(tr("Status:"));
  statusBar()->addWidget(m_lblStatus);
}
