#include "WAccounts.h"
#include "ui_WAccounts.h"

#include <QAbstractItemModel>
#include <QtWidgets>

#include "Database.h"

class AccountsModel : public QAbstractItemModel
{
public:
  AccountsModel(QObject *parent = nullptr);
  virtual ~AccountsModel() override;

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = {}) const override;
  int columnCount(const QModelIndex &parent = {}) const override;

private:
};

AccountsModel::AccountsModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

AccountsModel::~AccountsModel()
{
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || role != Qt::DisplayRole)
  {
    return QVariant();
  }

  return "test";
}

Qt::ItemFlags AccountsModel::flags(const QModelIndex &index) const
{
  return index.isValid() ? QAbstractItemModel::flags(index) : Qt::NoItemFlags;
}

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    return "header";
  }

  return QVariant();
}

QModelIndex AccountsModel::index(int row, int column, const QModelIndex &parent) const
{
  return QModelIndex();
}

QModelIndex AccountsModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
  return 10;
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

WAccounts::WAccounts(QWidget *parent) :
    WBase(parent),
    m_ui(new Ui::WAccounts)
{
  m_ui->setupUi(this);
  setupHandlers();

  m_ui->treeView->setModel(new AccountsModel(this));
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
  connect(m_ui->btnImport, &QPushButton::clicked, this, &WAccounts::importFromJson);
}

void WAccounts::importFromJson(void)
{
  QString filePath =
      QFileDialog::getOpenFileName(this, tr("Import Accounts"), QDir::homePath(), tr("json (*.json); All Files (*)"));

  if (filePath.isEmpty())
  {
    return;
  }

  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    return;
  }

  QJsonParseError err;
  QJsonDocument json = QJsonDocument::fromJson(file.readAll(), &err);
  if (err.error != QJsonParseError::NoError)
  {
    qDebug() << "parsing json failed: " << err.errorString();
    QMessageBox::warning(this, tr("Import Accounts"), tr("Parsing json failed"));
  }

  if (!json.isObject() || !json.object()["accounts"].isArray())
  {
    qDebug() << "parsing json failed: missing accounts property";
    return;
  }

  QJsonArray accountsArray = json.object()["accounts"].toArray();
  importJsonArray(accountsArray);

  file.close();
}

void WAccounts::importJsonArray(const QJsonArray &array, const QString &parentId)
{
  for (auto el : array)
  {
    if (!el.isObject())
    {
      continue;
    }

    QJsonObject obj = el.toObject();
    QString id = obj["id"].toString();
    QString accountType = obj["accountType"].toString();
    QString title = obj["title"].toString();

    QString sql;
    if (id != "")
    {
      sql = "UPDATE accounts SET"
            "  accountType=:accountType,"
            "  parentId=:parentId,"
            "  title=:title"
            " WHERE id=:id";
    }
    else
    {
      id = generateId();

      sql = "INSERT INTO accounts ("
            "  id,"
            "  accountType,"
            "  parentId,"
            "  title"
            ") VALUES ("
            "  :id,"
            "  :accountType,"
            "  :parentId,"
            "  :title"
            ")";
    }
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":accountType", accountType);
    query.bindValue(":parentId", parentId);
    query.bindValue(":title", title);
    if (!query.exec())
    {
      qDebug() << "import accounts sql error: " << query.lastError().text();
      continue;
    }

    if (obj["childrens"].isArray())
    {
      importJsonArray(obj["childrens"].toArray(), id);
    }
  }
}
