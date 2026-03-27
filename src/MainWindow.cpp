#include "MainWindow.h"

#include <QtWidgets>

#include "DWRibbon.h"
#include "WAccounts.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  setupUi();
  setupHandlers();
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

void MainWindow::setupHandlers(void)
{
  connect(m_twCentralWidget, &QTabWidget::tabCloseRequested,
          [&](int index)
          {
            m_twCentralWidget->removeTab(index);

            for (auto it = m_tabs.begin(); it != m_tabs.end(); it++)
            {
              if (it->index == index)
              {
                m_tabs.remove(it.key());
                break;
              }
            }

            for (auto it = m_tabs.begin(); it != m_tabs.end(); it++)
            {
              if (it->index > index)
              {
                it->index--;
              }
            }
          });

  connect(m_dwRibbon, &DWRibbon::quitClicked, qApp, &QCoreApplication::quit);

  connect(m_dwRibbon, &DWRibbon::accountsClicked,
          [&]()
          {
            if (m_tabs.find("accounts") != m_tabs.end())
            {
              m_twCentralWidget->setCurrentIndex(m_tabs["accounts"].index);
              return;
            }

            WAccounts *frm = new WAccounts(this);
            int index = m_twCentralWidget->addTab(frm, tr("Accounts"));
            m_twCentralWidget->setCurrentIndex(index);
            m_tabs["accounts"] = {index, frm};
          });
}
