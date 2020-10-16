#include "qtdatabaseconnectiondialog.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtGui/QPushButton>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>

QtDatabaseConnectionDialog::QtDatabaseConnectionDialog(const QString &connectionName, QWidget *parent)
    : QDialog(parent), dbConnectionName(connectionName)
{
    connectorThread = new QtDatabaseConnector(connectionName, this);
    connect(connectorThread, SIGNAL(finished()), this, SLOT(threadFinished()));
    setupUi(this);
    Qt::WindowFlags flags = windowFlags() | Qt::CustomizeWindowHint;
    flags &= ~Qt::WindowCloseButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);
    driverEdit->addItems(QSqlDatabase::drivers());
    errorLable->hide();
    errorLable->clear();
    portEdit->setToolTip(tr("default") + QString("\n3306   MySQL\n1521   Oracle\n5432   PostgreSQL"));
    setupCursor();
}

QtDatabaseConnectionDialog::~QtDatabaseConnectionDialog()
{}

void QtDatabaseConnectionDialog::loadSettings(const QString &savePath)
{
    settingsPath = savePath;
    QSettings settings;
    if (!settingsPath.isEmpty())
        settings.beginGroup(settingsPath);
    if (settings.contains("driver") && driverEdit->isEnabled())
    {
        const QString driverName = settings.value("driver").toString();
        for (int i = 0; i < driverEdit->count(); ++i)
        {
            if (driverEdit->itemText(i) == driverName)
            {
                driverEdit->setCurrentIndex(i);
                break;
            }
        }
    }
    databaseNameEdit->setText(settings.value("databasename").toString());
    hostEdit->setText(settings.value("host").toString());
    portEdit->setValue(settings.value("port").toInt());
    loginEdit->setText(settings.value("login").toString());
    setupCursor();
}

void QtDatabaseConnectionDialog::saveFieldsIntoSettings()
{
    QSettings settings;
    if (!settingsPath.isEmpty())
        settings.beginGroup(settingsPath);
    if (driverEdit->count())
        settings.setValue("driver", driverEdit->currentText());
    settings.setValue("databasename", databaseNameEdit->text());
    settings.setValue("host", hostEdit->text());
    settings.setValue("port", portEdit->value());
    settings.setValue("login", loginEdit->text());
}

void QtDatabaseConnectionDialog::setSettingsPath(const QString &savePath)
{
    settingsPath = savePath;
}

void QtDatabaseConnectionDialog::setDriver(const QString &driverName)
{
    driverEdit->clear();
    if (driverName.isEmpty())
    {
        driverEdit->addItems(QSqlDatabase::drivers());
        driverEdit->setEnabled(true);
    }
    else
    {
        driverEdit->addItem(driverName);
        driverEdit->setDisabled(true);
    }
}

void QtDatabaseConnectionDialog::setDriversFilter(const QStringList &permittedList)
{
    driverEdit->clear();
    driverEdit->setEnabled(true);
    QStringList drivers(QSqlDatabase::drivers());
    for(QStringList::iterator itDriver = drivers.begin(); itDriver != drivers.end();)
    {
        if(!permittedList.contains(*itDriver))
            itDriver = drivers.erase(itDriver);
        else
            ++itDriver;
    }
    driverEdit->addItems(drivers);
}

void QtDatabaseConnectionDialog::accept()
{
    if (!driverEdit->count())
    {
        errorLable->setText(tr("There are no available SQL drivers."));
        errorLable->show();
        return;
    }

    errorLable->hide();
    errorLable->clear();
    operButtons->setDisabled(true);
    if (connectorThread->isRunning())
        return;

    connectorThread->driverName = driverEdit->currentText();
    connectorThread->databaseName = databaseNameEdit->text();
    connectorThread->host = hostEdit->text();
    connectorThread->port = portEdit->value();
    connectorThread->login = loginEdit->text();
    connectorThread->password = passwordEdit->text();
    progressBar->setRange(0, 0);
    connectorThread->start();
}

void QtDatabaseConnectionDialog::reject()
{
    if (connectorThread->isRunning())
        operButtons->setDisabled(true);
    else
        QDialog::reject();
}

void QtDatabaseConnectionDialog::threadFinished()
{
    progressBar->setRange(0, 1);
    progressBar->update();
    if (connectorThread->db.isOpen())
    {
        QSqlDatabase::cloneDatabase(connectorThread->db, dbConnectionName);
        connectorThread->clear();
        QDialog::accept();
    }
    else
    {
        errorLable->setText(connectorThread->db.lastError().text());
        errorLable->show();
    }
    operButtons->setEnabled(true);
}

void QtDatabaseConnectionDialog::setupCursor()
{
    QList<QLineEdit *> list;
    list << databaseNameEdit << hostEdit << loginEdit << passwordEdit;
    foreach (QLineEdit *widget, list)
    {
        if (widget->text().isEmpty())
        {
            widget->setFocus();
            return;
        }
    }
}

void QtDatabaseConnectionDialog::closeEvent(QCloseEvent *e)
{
    if (connectorThread->isFinished())
        QDialog::closeEvent(e);
    else
        e->ignore();
}
