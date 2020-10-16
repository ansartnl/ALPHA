#ifndef MAIN_H
#define MAIN_H

/*!
  Application for aftn planning.

  For create new mdi child window or dialog with anything function implement ObjectsManagerHelper interface. Return your widget in ObjectsManagerHelper::widget. Than register your object into
  ObjectsManager::registerBuiltinObjects.
  For fast develop access to any sql table you can create something like this:
  \code
class AirplaneTypesTableHelper : public ObjectsManagerHelper
{
  Q_OBJECT
#define AirplaneTypesTableHelperId "airplaneTypesTable"
public:
  AirplaneTypesTableHelper(QObject* parent = 0):ObjectsManagerHelper(parent){}

  virtual QString id() const {
    return AirplaneTypesTableHelperId;
  }
  virtual QString path() const  {
    return tr("/View/Airplane types");
  }

  virtual QString name() const {
    return tr("Airplane types");
  }

  virtual QString description() const {
    return tr("Description of airplane types");
  }

  virtual QIcon icon() const {
    return QIcon();
  }

  virtual QWidget* widget(QWidget* parent = 0) const {
    return new SqlTableBrowser("airplane_type", parent);
  };
};
  \endcode

  Main application sql connection is default sql connection. QSqlDatabase::database().

  Other connection can be used in 3rdparty libraries (AringToDB), or in thread.

  At now application does not use thread.

  \mainpage
  */

#include <QtCore/QSettings>
#include <QtCore/QHash>
#include <QtCore/QEvent>

#include <QtNetwork/QTcpSocket>

class QHostAddress;
class MainWindow;
class QTimer;
class LoginDialog;

class PulsarStatus : public QObject
{
    Q_OBJECT
public:
    PulsarStatus(QObject* parent = 0);

public slots:
    void pulsarChanged(const QHostAddress& newpulsar, const QString& status, const QHostAddress& oldpulsar);

    void statusChanged(const QHostAddress& pulsar, const QString& status);

    void telnetStateChanged(QAbstractSocket::SocketState socketState);
    void telnetReadyRead();

    void initConnection();

protected:

    void disconnectFromOld(const QHostAddress& pulsar);
    void connectToNew(const QHostAddress& pulsar, const QString& data);

    QTcpSocket *telnet;

    bool alreadyLogged;

    MainWindow *mainWindow;

    QTimer *initTimer;

    QString lastDbHost;
};

#include "aftnmessageprocessor.h"

#define SUPERUSER_ID 1

/*!
  Rights system for users

  \struct PermissionsMap
  */
typedef enum {
  PermissionDisabled = 0,
  PermissionEnabled
} PermissionEnum;

typedef QHash<QString, int> Permissions;
/*!
  Settings with network connection params
  */
QSettings* networkSettings(const QString &pathFile = QString());

/*!
  Settings for user dialogs, forms, etc options
  */
QSettings* globalSettings();

/*!
  Curernt user name is storage here
  */
extern QString currentUser;
extern QString currentUserFullName;
extern int currentUserId;
extern QString currentPassword;


/**
* @brief return md5 hash for password string
*
* @fn passwordHash
* @param password
*/
QString passwordHash(QString password);

/**
* @brief Function for internal account system in database. Tables users, login, users_rights

*/
QString login(const QString &username, const QString &password);
QString setLogin_out(int userid, const QString &username, int logintype);

LoginDialog* getLoginDialog(QWidget *parent);

/**
* @brief Current user permissions is storage here
*
* @var currentPermissions
*/
extern Permissions currentPermissions;

/*!
  Class from "aftnmessageprocessor" static library for aftn message processing (validating, parsing, and database inserting)
  */
extern AFTNMessageProcessor* aftnMessageProcessor;

/*!
  Gui classes presents in this namespace
  \namespace gui
  */

/*!

  \namespace core
  */
#endif //MAIN_H
