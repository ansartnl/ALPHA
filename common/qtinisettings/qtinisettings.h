#ifndef QTINISETTINGS_H
#define QTINISETTINGS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QSettings>

#if defined(QTINISETTINGS_EXPORTS)
#  define QTINISETTINGS_EXPORT Q_DECL_EXPORT
#elif defined(QTINISETTINGS_IMPORTS)
#  define QTINISETTINGS_EXPORT  Q_DECL_IMPORT  /**/
#else
#  define QTINISETTINGS_EXPORT   /**/
#endif

/**
 * @brief Class for storing application setting in ini file. This class implements QSettings interface.
 *
 * @class QtIniSettings QtIniSettings.h
 */
class QTINISETTINGS_EXPORT QtIniSettings: public QSettings
{
    Q_OBJECT
public:
    QtIniSettings(const QString &organization, const QString &application,
                  QObject *parent = 0) :
        QSettings(ini_format, QSettings::UserScope, organization, application,
                  parent)
    {
    }

    QtIniSettings(Scope scope, const QString &organization,
                  const QString &application = QString(), QObject *parent = 0) :
        QSettings(ini_format, scope, organization, application, parent)
    {
    }

    QtIniSettings(const QString &fileName, QObject *parent = 0) :
        QSettings(fileName, ini_format, parent)
    {
    }

    QtIniSettings(QObject * parent = 0) :
        QSettings(ini_format, QSettings::UserScope,
                  QCoreApplication::organizationName(),
                  QCoreApplication::applicationName(), parent)
    {
    }

private:
    static const QSettings::Format ini_format;
};

#endif // QTINISETTINGS_H
