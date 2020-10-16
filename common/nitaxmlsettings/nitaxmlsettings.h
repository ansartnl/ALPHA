#ifndef NITAXMLSETTINGS_H
#define NITAXMLSETTINGS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>

class QDomDocument;
class QDomElement;
class QIODevice;

class NitaXmlSettings : public QSettings
{
    Q_OBJECT
public:
    static const QSettings::Format kXmlFormat;

    static const QString kSeparator;

    static const QString kName;
    static const QString kValue;
    static const QString kDescription;
    static const QString kUnit;
    static const QString kRange;

    NitaXmlSettings(const QString &organization, const QString &application, QObject *parent = 0) :
        QSettings(kXmlFormat, QSettings::UserScope, organization, application, parent)
    {}

    NitaXmlSettings(Scope scope, const QString &organization, const QString &application = QString()
                    , QObject *parent = 0) :
        QSettings(kXmlFormat, scope, organization, application, parent)
    {}

    NitaXmlSettings(const QString &fileName, QObject *parent = 0) :
        QSettings(fileName, kXmlFormat, parent)
    {}

    NitaXmlSettings(QObject * parent = 0) :
        QSettings(kXmlFormat, QSettings::UserScope
                  , QCoreApplication::organizationName()
                  , QCoreApplication::applicationName()
                  , parent)
    {}

    static const QString description(const QString key);
    static const QString unit(const QString key);
    static const QString range(const QString key);

private:
    static bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map);
    static void processReadKey(const QString &key, QSettings::SettingsMap &map, QDomElement &domElement);

    static bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map);
    static void processWriteKey(QDomDocument &doc, QDomElement &domElement
                                , const QString &key, const QVariant& value);
    static void insertAttribute(QDomDocument& doc, QDomElement &domElement
                                , const QString &key, const QString &attrKey, const QVariant& value);
    static void changeTagName(QDomElement &domElement);
};

#endif // NITAXMLSETTINGS_H
