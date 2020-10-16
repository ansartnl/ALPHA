#ifndef EXTENDEDXMLSETTINGS_H
#define EXTENDEDXMLSETTINGS_H

#include "qtxmlsettings.h"

class ExtendedXmlSettings : public QtXmlSettings
{
    Q_OBJECT
public:
    ExtendedXmlSettings(const QString &organization, const QString &application,
                  QObject *parent = 0) :
        QtXmlSettings(organization, application, parent), mAltConfig(0), mVolatileConfig(0)
    {
    }

    ExtendedXmlSettings(Scope scope, const QString &organization,
                  const QString &application = QString(), QObject *parent = 0) :
        QtXmlSettings(scope, organization, application, parent), mAltConfig(0), mVolatileConfig(0)
    {
    }

    ExtendedXmlSettings(const QString &fileName, QObject *parent = 0) :
        QtXmlSettings(fileName, parent), mAltConfig(0), mVolatileConfig(0)
    {
    }

    ExtendedXmlSettings(QObject * parent = 0) :
        QtXmlSettings(parent), mAltConfig(0), mVolatileConfig(0)
    {
    }

    ~ExtendedXmlSettings();

    void setAlternativeConfig(const QtXmlSettings *config);
    void setVolatileConfig(QtXmlSettings *config);

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);

    void beginGroup(const QString &prefix);
    void endGroup();

private:
    const QtXmlSettings* mAltConfig;
    QtXmlSettings *mVolatileConfig;
};

#endif // EXTENDEDXMLSETTINGS_H
