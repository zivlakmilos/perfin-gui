#include "Database.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

void runMigrations(void);
void showFatalError(const QString &error);

void setupDatabase(void)
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

#if PRODUCTION
#else  // PRODUCTION
  db.setDatabaseName("database.db");
#endif // PRODUCTION

  if (!db.open())
  {
    QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Cannot Open Database"));
    qFatal() << "cannot open database";
    qApp->exit();
    showFatalError(QObject::tr("Cannot Open Database"));
  }

  runMigrations();
}

QSqlDatabase getDatabase()
{
  return QSqlDatabase::database();
}

void runMigrations(void)
{
  QSqlDatabase db = QSqlDatabase::database();
  QSqlQuery query(db);
  QString sql;

  sql = "CREATE TABLE IF NOT EXISTS accounts ("
        "  id TEXT PRIMARY KEY,"
        "  accountType TEXT,"
        "  parentId TEXT,"
        "  title TEXT"
        ")";

  if (!query.exec(sql))
  {
    showFatalError("Creating table 'accounts' failed");
  }
}

void showFatalError(const QString &error)
{
  QMessageBox::critical(nullptr, QObject::tr("Error"), error);
  qFatal() << error;
  qApp->exit();
}
