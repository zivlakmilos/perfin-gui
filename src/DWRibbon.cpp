#include "DWRibbon.h"
#include "ui_DWRibbon.h"

DWRibbon::DWRibbon(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::DWRibbon)
{
  m_ui->setupUi(this);
}

DWRibbon::~DWRibbon(void)
{
  if (m_ui)
  {
    delete m_ui;
  }
}
