#ifndef QTXMLSETTINGS_H
#define QTXMLSETTINGS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QSettings>

class QDomDocument;
class QDomElement;
class QIODevice;

#if defined(QTXMLSETTINGS_EXPORTS)
#  define QTXMLSETTINGS_EXPORT Q_DECL_EXPORT
#elif defined(QTXMLSETTINGS_IMPORTS)
#  define QTXMLSETTINGS_EXPORT  Q_DECL_IMPORT  /**/
#else
#  define QTXMLSETTINGS_EXPORT   /**/
#endif

/**
 * @brief Class for storing application setting in xml file. This class implements QSettings interface.
 *
 * @class QtXmlSettings qtxmlsettings.h
 */
class QTXMLSETTINGS_EXPORT QtXmlSettings: public QSettings
{
    Q_OBJECT
public:
    QtXmlSettings(const QString &organization, const QString &application,
                  QObject *parent = 0) :
        QSettings(xml_format, QSettings::UserScope, organization, application,
                  parent)
    {
    }

    QtXmlSettings(Scope scope, const QString &organization,
                  const QString &application = QString(), QObject *parent = 0) :
        QSettings(xml_format, scope, organization, application, parent)
    {
    }

    QtXmlSettings(const QString &fileName, QObject *parent = 0) :
        QSettings(fileName, xml_format, parent)
    {
    }

    QtXmlSettings(QObject * parent = 0) :
        QSettings(xml_format, QSettings::UserScope,
                  QCoreApplication::organizationName(),
                  QCoreApplication::applicationName(), parent)
    {
    }

    //! Returns true if specified key is an attribute.
    static bool isAttribute(const QString &key);

    //! Make attribute name for the specified key.
    static QString makeAttributeName(const QString &key);

    static const QSettings::Format xml_format;
private:
    static bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map);
    static bool writeXmlFile(QIODevice &device,
                             const QSettings::SettingsMap &map);
    static void processWriteKey(QDomDocument& doc, QDomElement& domElement,
                                QString key, const QVariant& value);
    static void processReadKey(QString key, QSettings::SettingsMap &map,
                               QDomElement& domElement);
};

#endif //QTXMLSETTINGS_H
