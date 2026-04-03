#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <QString>
#include <QUuid>
#include <QtSql>

constexpr const char *AccountTypeAsset = "asset";
constexpr const char *AccountTypeLiability = "liability";
constexpr const char *AccountTypeIncome = "income";
constexpr const char *AccountTypeExpense = "expense";
constexpr const char *AccountTypeEquity = "equity";

void setupDatabase(void);
QSqlDatabase getDatabase(void);

inline QString generateId(void)
{
  return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

#endif // _DATABASE_H_
