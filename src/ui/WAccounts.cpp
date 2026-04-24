#include "WAccounts.h"
#include "ui_WAccounts.h"

#include <QAbstractItemModel>
#include <QtWidgets>

#include "Database.h"

struct AccountNode
{
  QString m_id;
  QString m_accountType;
  QString m_title;
  AccountNode *m_parent;
  QVector<AccountNode *> m_childs;
};

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

  void reloadData(void);
  void clearData(void);
  void clearNode(AccountNode *node);
  AccountNode *findNode(AccountNode *node, QString id);
  void insertNode(AccountNode *node, QString parentId);

private:
  AccountNode *m_data;
};

AccountsModel::AccountsModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_data(nullptr)
{
}

AccountsModel::~AccountsModel()
{
  clearData();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || role != Qt::DisplayRole)
  {
    return QVariant();
  }

  AccountNode *node = static_cast<AccountNode *>(index.internalPointer());
  return node->m_title;
}

Qt::ItemFlags AccountsModel::flags(const QModelIndex &index) const
{
  return index.isValid() ? QAbstractItemModel::flags(index) : Qt::NoItemFlags;
}

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    return "Account";
  }

  return QVariant();
}

QModelIndex AccountsModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
  {
    return QModelIndex();
  }

  AccountNode *parentNode = m_data;
  if (parent.isValid())
  {
    parentNode = static_cast<AccountNode *>(parent.internalPointer());
  }

  if (row < parentNode->m_childs.length())
  {
    return createIndex(row, column, parentNode->m_childs[row]);
  }

  return QModelIndex();
}

QModelIndex AccountsModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return QModelIndex();
  }

  AccountNode *childNode = static_cast<AccountNode *>(index.internalPointer());
  AccountNode *parentNode = childNode->m_parent;

  if (parentNode == m_data)
  {
    return QModelIndex();
  }

  int row = parentNode->m_childs.indexOf(childNode);
  return createIndex(row, 0, parentNode);
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
  AccountNode *node = m_data;
  if (parent.isValid())
  {
    node = static_cast<AccountNode *>(parent.internalPointer());
  }

  return node->m_childs.length();
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

void AccountsModel::reloadData(void)
{
  beginResetModel();

  clearData();

  m_data = new AccountNode;
  m_data->m_id = "root";
  m_data->m_accountType = "";
  m_data->m_title = "";

  m_data->m_childs.append(new AccountNode{
      .m_id = "equity",
      .m_accountType = "equity",
      .m_title = "Equity",
      .m_parent = m_data,
  });
  m_data->m_childs.append(new AccountNode{
      .m_id = "asset",
      .m_accountType = "asset",
      .m_title = "Assets",
      .m_parent = m_data,
  });
  m_data->m_childs.append(new AccountNode{
      .m_id = "liability",
      .m_accountType = "liability",
      .m_title = "Liability",
      .m_parent = m_data,
  });
  m_data->m_childs.append(new AccountNode{
      .m_id = "income",
      .m_accountType = "income",
      .m_title = "Incomes",
      .m_parent = m_data,
  });
  m_data->m_childs.append(new AccountNode{
      .m_id = "expense",
      .m_accountType = "expense",
      .m_title = "Expenses",
      .m_parent = m_data,
  });

  QSqlDatabase db = QSqlDatabase::database();
  QSqlQuery query;
  QString sql = "SELECT * FROM accounts";
  if (!query.exec(sql))
  {
    qDebug() << "error reloading accounts data";
    return;
  }

  while (query.next())
  {
    AccountNode *node = new AccountNode{
        .m_id = query.value(0).toString(),
        .m_accountType = query.value(1).toString(),
        .m_title = query.value(3).toString(),
    };
    QString parentId = query.value(2).toString();
    if (parentId == "")
    {
      parentId = node->m_accountType;
    }
    AccountNode *parent = findNode(m_data, parentId);
    if (!parent)
    {
      qDebug() << "error reloading accounts data; missing parent account";
      delete node;
      continue;
    }

    node->m_parent = parent;
    parent->m_childs.append(node);
  }

  endResetModel();
}

void AccountsModel::clearData(void)
{
  clearNode(m_data);
  m_data = nullptr;
}

void AccountsModel::clearNode(AccountNode *node)
{
  if (!node)
  {
    return;
  }

  for (auto child : node->m_childs)
  {
    clearNode(child);
  }

  delete node;
}

AccountNode *AccountsModel::findNode(AccountNode *node, QString id)
{
  if (!node)
  {
    return nullptr;
  }

  if (id == node->m_id)
  {
    return node;
  }

  for (auto child : node->m_childs)
  {
    AccountNode *n = findNode(child, id);
    if (n)
    {
      return n;
    }
  }

  return nullptr;
}

void AccountsModel::insertNode(AccountNode *node, QString parentId)
{
  AccountNode *parent = findNode(m_data, parentId);
  if (!parent)
  {
    return;
  }

  parent->m_childs.append(node);
  node->m_parent = parent;
}

WAccounts::WAccounts(QWidget *parent) :
    WBase(parent),
    m_ui(new Ui::WAccounts),
    m_currentAccount(nullptr),
    m_isCurrentModified(false)
{
  m_ui->setupUi(this);
  setupHandlers();

  m_model = new AccountsModel(this);
  m_model->reloadData();

  m_ui->twAccounts->setModel(m_model);
  m_ui->twAccounts->expandAll();
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

  connect(m_ui->twAccounts, &QTreeView::customContextMenuRequested, this, &WAccounts::showAccountsContextMenu);
  connect(m_ui->twAccounts, &QTreeView::doubleClicked, this, &WAccounts::editAccount);

  connect(m_ui->txtTitle, &QLineEdit::textChanged, this, [&]() { m_isCurrentModified = true; });
  connect(m_ui->btnSave, &QPushButton::clicked, this, &WAccounts::saveAccount);
}

void WAccounts::showAccountsContextMenu(const QPoint &point)
{
  QModelIndex index = m_ui->twAccounts->indexAt(point);
  if (!index.isValid())
  {
    return;
  }
  m_ui->twAccounts->setCurrentIndex(index);

  QMenu *menu = new QMenu(this);

  menu->addAction(tr("Edit account"), this, [&]() { editAccount(index); });

  menu->addAction(tr("Create child account"), this,
                  [&]()
                  {
                    // TODO: check if modify flag is set and display yes/no modal
                    AccountNode *node = static_cast<AccountNode *>(index.internalPointer());
                    m_currentAccount = new AccountNode{
                        .m_id = "",
                        .m_accountType = node->m_accountType,
                        .m_title = "",
                        .m_parent = node,
                    };
                    if (node->m_id == node->m_accountType)
                    {
                      m_currentAccount->m_parent = nullptr;
                    }
                    m_isCurrentModified = false;

                    m_ui->txtTitle->setText(m_currentAccount->m_title);
                  });

  QPoint pos = m_ui->twAccounts->viewport()->mapToGlobal(point);
  menu->exec(pos);
}

void WAccounts::editAccount(const QModelIndex &index)
{
  // TODO: check if modify flag is set and display yes/no modal
  AccountNode *node = static_cast<AccountNode *>(index.internalPointer());
  m_currentAccount = node;
  m_isCurrentModified = false;
  m_ui->txtTitle->setText(m_currentAccount->m_title);
}

void WAccounts::saveAccount(void)
{
  if (!m_isCurrentModified || !m_currentAccount)
  {
    return;
  }

  m_currentAccount->m_title = m_ui->txtTitle->text();

  QSqlDatabase db = QSqlDatabase::database();
  QSqlQuery query;

  QString parentId;
  if (m_currentAccount->m_parent)
  {
    parentId = m_currentAccount->m_parent->m_id;
  }

  if (m_currentAccount->m_id == "")
  {
    QString id = generateId();
    QString sql = "INSERT INTO accounts ("
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
    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":accountType", m_currentAccount->m_accountType);
    query.bindValue(":parentId", parentId);
    query.bindValue(":title", m_currentAccount->m_title);
    if (!query.exec())
    {
      QMessageBox::warning(this, tr("Create Account"), tr("Fail to create account"));
      qDebug() << "create account sql error: " << query.lastError().text();
      return;
    }

    m_currentAccount->m_id = id;
    if (parentId == "")
    {
      parentId = m_currentAccount->m_accountType;
    }
    m_model->insertNode(m_currentAccount, parentId);

    m_model->layoutChanged();
  }
  else
  {
    QString sql = "UPDATE accounts SET"
                  "  title=:title"
                  " WHERE id=:id";
    query.prepare(sql);
    query.bindValue(":id", m_currentAccount->m_id);
    query.bindValue(":title", m_currentAccount->m_title);
    if (!query.exec())
    {
      QMessageBox::warning(this, tr("Update Account"), tr("Fail to update account"));
      qDebug() << "update account sql error: " << query.lastError().text();
      return;
    }

    m_model->layoutChanged();
  }

  m_isCurrentModified = false;
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

  m_model->reloadData();
  m_ui->twAccounts->expandAll();
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
