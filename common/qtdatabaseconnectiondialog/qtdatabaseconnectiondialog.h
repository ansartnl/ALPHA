#ifndef QTDATABASECONNECTIONDIALOG_H
#define QTDATABASECONNECTIONDIALOG_H

#include "ui_qtdatabaseconnectiondialog.h"
#include "qtdatabaseconnector.h"

/*!
  * \class QtDatabaseConnectionDialog
  * \brief Диалог подключения к базе данных
  *
  * Данный класс реализует функциональную часть диалога подключения к базе данных.
  * Он предназначен для однократной попытки подключения к базе, на основе данных,
  * введенных пользователем в диалоге. В случае ошибки подключения, данный класс
  * отображает возникшую проблему на форме диалога.
  */

class QtDatabaseConnectionDialog : public QDialog, private Ui::QtDatabaseConnectionDialog
{
    Q_OBJECT
public:
    explicit QtDatabaseConnectionDialog(const QString &connectionName = QString(QSqlDatabase::defaultConnection), QWidget *parent = 0);
    virtual ~QtDatabaseConnectionDialog();
    void setDriver(const QString &driverName = QString::null); // push type of driver or disable rigidly setted driver name if driverName is QString::null
    void setDriversFilter(const QStringList &permittedList); // filtered drivers list
    void loadSettings(const QString &savePath = QString::null); // load setting from settings by savePath and store savePath for next saves and loads
    void setSettingsPath(const QString &savePath); // just change save path

public slots:
    virtual void accept(); // try to open connectin to DB
    virtual void reject(); // close the dialog
    virtual void saveFieldsIntoSettings(); // save entered field excep password into settings

private slots:
    void threadFinished();

protected:
    void closeEvent(QCloseEvent *e);

private:
    const QString dbConnectionName;
    QString settingsPath;
    QtDatabaseConnector *connectorThread;
    void setupCursor();
};

//    HOW TO USE EXAMPLE:

//    QtDatabaseConnectionDialog dialog("MyNewConnection");
//    dialog.setDriver("QOCI");
//    dialog.loadSettings("db");
//    if (QDialog::Rejected == dialog.exec())
//    {
//        a.processEvents();
//        return 1;
//    }
//    else
//        dialog.saveFieldsIntoSettings();


#endif // QTDATABASECONNECTIONDIALOG_H
