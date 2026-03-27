#include "WAccounts.h"

#include "ui_WAccounts.h"

WAccounts::WAccounts(QWidget *parent) :
    WBase(parent),
    m_ui(new Ui::WAccounts)
{
  m_ui->setupUi(this);
  setupHandlers();
}

WAccounts::~WAccounts(void)
{
  if (m_ui)
  {
    delete m_ui;
  }
}

void WAccounts::setupHandlers(void)
{
}
