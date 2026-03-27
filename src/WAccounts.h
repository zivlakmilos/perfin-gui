#ifndef _WACCOUNTS_H_
#define _WACCOUNTS_H_

#include "WBase.h"

namespace Ui
{
class WAccounts;
}

class WAccounts : public WBase
{
  Q_OBJECT

public:
  WAccounts(QWidget *parent = nullptr);
  virtual ~WAccounts(void);

private:
  void setupHandlers(void);

  Ui::WAccounts *m_ui;
};

#endif // _WACCOUNTS_H_
