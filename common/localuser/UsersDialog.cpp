#include "UsersDialog.h"

#include "Users.h"

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

QVariantHash users;
QVariantHash currentUser;
QString currentUserLogin;
QStringList groups;

UsersDialog::UsersDialog(QWidget *parent) :
    QDialog(parent), model(new QStandardItemModel(this))
{
    setupUi(this);

    QStandardItem *root = model->invisibleRootItem();
    QList<QStandardItem *> row;
    foreach (const QString& userName, users.keys())
    {
        row.append(new QStandardItem(userName));
        row.append(new QStandardItem(users.value(userName).toHash().value("fullname").toString()));
        row.append(new QStandardItem(users.value(userName).toHash().value("type").toString()));
        root->appendRow(row);
        row.clear();
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Login") << tr("Full name")
                                     << tr("User type"));

    tableView->setModel(model);
}

void UsersDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void UsersDialog::on_add_clicked()
{
    QString userName = QInputDialog::getText(this, tr("New user"), tr("Enter login for new user:"));
    if (userName.isEmpty())
        return;
    QVariantHash newUser;
    newUser.insert("fullname", userName);
    newUser.insert("type", "user");

    if (users.find(userName) != users.end())
    {
        QMessageBox::critical(this, tr("Error"), tr("User with this name already exists"));
        qUserLogError(tr("UsersDialog")) << tr(" Attempted to create user '"
                                               ) << userName << tr("', which already exists");
        return;
    }
    users.insert(userName, newUser);
    qUserLogInfo(tr("UsersDialog")) << tr(" Created user: ") << userName;

    QList<QStandardItem *> row;
    row.append(new QStandardItem(userName));
    row.append(new QStandardItem(newUser.value("fullname").toString()));
    row.append(new QStandardItem(newUser.value("type").toString()));

    model->invisibleRootItem()->appendRow(row);

    tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
    on_changePassword_clicked();
    on_changeType_clicked();
}

void UsersDialog::on_remove_clicked()
{
    if (tableView->currentIndex().isValid())
    {
        QString userName = model->index(tableView->currentIndex().row(), 0).data().toString();
        if (QMessageBox::question(this, tr("Warning")
                                  , tr("Do you realy want to remove user ") + userName + "?"
                                  , QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Ok)
        {
            users.remove(userName);
            qUserLogInfo(tr("UsersDialog")) << tr(" Removed user: '") << userName << "'";
            model->removeRow(tableView->currentIndex().row());
        }
    }
}

void UsersDialog::accept()
{
    int atLeast = 0;
    foreach(const QVariant& user, users.values())
        if (user.toHash().value("type").toString() == "admin")
            ++atLeast;

    if (!atLeast)
        QMessageBox::warning(this, tr("Warning")
                             , tr("You must create one user with type 'admin' at least"));
    else
        QDialog::accept();
}

void UsersDialog::on_changeFullname_clicked()
{
    if (tableView->currentIndex().isValid())
    {
        bool ok;
        QString currentName = model->index(tableView->currentIndex().row(), 0).data().toString();
        QVariantHash user = users.value(currentName).toHash();
        QString value = QInputDialog::getText(this, tr("New full name"), tr("Enter full name:")
                                              , QLineEdit::Normal, user.value("fullname").toString()
                                              , &ok);
        if (ok)
        {
            user.insert("fullname", value);
            users.insert(currentName, user);
            qUserLogInfo(tr("UsersDialog")) << tr(" Changed user's '") << currentName
                                            << tr("' full name to ") << value;
            model->setData(model->index(tableView->currentIndex().row(), 1), value);
        }
    }
}

void UsersDialog::on_changePassword_clicked()
{
    if (tableView->currentIndex().isValid())
    {
        bool ok = true;
        QString currentName = model->index(tableView->currentIndex().row(), 0).data().toString();
        QVariantHash user = users.value(currentName).toHash();
        QString value = "";
        while (ok && (value.size() < 6))
        {
            value = QInputDialog::getText(this, tr("New password"), tr("Enter password:")
                                          , QLineEdit::Normal, QString(""), &ok);
            if (ok && (value.size() < 6))
                QMessageBox::warning(this, tr("Warning")
                                     , tr("Password should be at least 6 symbols long"));
        }
        if (ok)
        {
            QMessageBox::information(this, tr("Information")
                                     , tr("Password was changed succesfully"));
            user.insert("password", passwordHash(value));
            users.insert(currentName, user);
            qUserLogInfo(tr("UsersDialog")) << tr(" Changed user's '") << currentName
                                            << tr("' password");
        }
    }
}

void UsersDialog::on_changeType_clicked()
{
    if (tableView->currentIndex().isValid())
    {
        bool ok;
        QString currentName = model->index(tableView->currentIndex().row(), 0).data().toString();
        QVariantHash user = users.value(currentName).toHash();

        QStringList types = QStringList() << "admin" << "user" << "guest";
        if (!groups.isEmpty())
            types = groups;

        QString value = QInputDialog::getItem(this, tr("New type"), tr("Select type:")
                                              , types, types.indexOf(user.value("type").toString())
                                              , false, &ok);
        if (ok)
        {
            user.insert("type", value);
            users.insert(currentName, user);
            qUserLogInfo(tr("UsersDialog")) << tr(" Changed user's '") << currentName
                                            << tr("' type to '") << value << "'";
            model->setData(model->index(tableView->currentIndex().row(), 2), value);
        }
    }
}
