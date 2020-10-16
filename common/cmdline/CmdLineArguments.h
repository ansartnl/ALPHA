#ifndef _CMD_LINE_ARGUMENTS_H__
#define _CMD_LINE_ARGUMENTS_H__

#include <QtCore/QStringList>

/*!
  * \class CmdLineArguments
  * \brief Используется для формирования и парсинга командной строки
  *
  * Возможны следующие опции:
  * - имя пользователя = -u
  * - пароль = -p
  * - файл конфигурации = -c
  * - файл логирования = -l
  * - Для АДП. добавление ко всем конфигам суффикса = -x
  */

class CmdLineArguments
{
public:
    //! Constructor used to build command line arguments.
    CmdLineArguments();

    //! Reference to the singleton object.
    static CmdLineArguments& instance(const QStringList &args = QStringList());

    //! Set/get user name.
    QString username() const;
    void setUserName(const QString &name);

    //! Set/get password.
    QString password() const;
    void setPassword(const QString &pswd);

    //! Set/get log file.
    QString log() const;
    void setLog(const QString &l);

    //! Set/get config file.
    QString config() const;
    void setConfig(const QString &c);

    //! Set/get xml file.
    QString xml() const;
    void setXml(const QString &x);

    //! Set/get host address.
    QString host() const;
    void setHost(const QString &h);

    //! Set/get path to extended style file.
    QString extendedStyle() const;
    void setExtendedStyle(const QString &es);

    //! Set/get port number.
    quint16 port() const;
    void setPort(quint16 p);

    //! Retrieve list of filled arguments.
    QStringList arguments() const { return mArguments; }

    //! Help string.
    static QString usage();

private:
    CmdLineArguments(const QStringList &args);

    /** Command line arguments */
    QStringList     mArguments;
};

#endif // _CMD_LINE_ARGUMENTS_H__
