#ifndef _WACCOUNTS_H_
#define _WACCOUNTS_H_

#include "WBase.h"

namespace Ui
{
class WAccounts;
}

class AccountsModel;
class AccountNode;

class WAccounts : public WBase
{
  Q_OBJECT

public:
  WAccounts(QWidget *parent = nullptr);
  virtual ~WAccounts(void);

private:
  void setupHandlers(void);

  void showAccountsContextMenu(const QPoint &point);
  void editAccount(const QModelIndex &index);
  void saveAccount(void);

  void importFromJson(void);
  void importJsonArray(const QJsonArray &array, const QString &parentId = QString());

  Ui::WAccounts *m_ui;
  AccountsModel *m_model;

  AccountNode *m_currentAccount;
  bool m_isCurrentModified;
};

#endif // _WACCOUNTS_H_
