#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <QSqlDatabase>
#include <QString>

constexpr const char *AccountTypeAsset = "asset";
constexpr const char *AccountTypeLiability = "liability";
constexpr const char *AccountTypeIncome = "income";
constexpr const char *AccountTypeExpense = "expense";
constexpr const char *AccountTypeEquity = "equity";

void setupDatabase(void);
QSqlDatabase getDatabase(void);

#endif // _DATABASE_H_
