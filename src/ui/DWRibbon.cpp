#include "DWRibbon.h"
#include "ui_DWRibbon.h"

DWRibbon::DWRibbon(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::DWRibbon)
{
  m_ui->setupUi(this);
  setupHandlers();
}

DWRibbon::~DWRibbon(void)
{
  if (m_ui)
  {
    delete m_ui;
  }
}

void DWRibbon::setupHandlers(void)
{
  connect(m_ui->btnAccounts, &QToolButton::clicked, this, &DWRibbon::accountsClicked);
  connect(m_ui->btnQuit, &QToolButton::clicked, this, &DWRibbon::quitClicked);
}
