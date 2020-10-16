#include "ExtendedXmlSettings.h"

ExtendedXmlSettings::~ExtendedXmlSettings()
{
}

void ExtendedXmlSettings::setAlternativeConfig(const QtXmlSettings *config)
{
    mAltConfig = config;
}

void ExtendedXmlSettings::setVolatileConfig(QtXmlSettings *config)
{
    mVolatileConfig = config;
}

QVariant ExtendedXmlSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (mVolatileConfig && mVolatileConfig->contains(key))
        return mVolatileConfig->value(key, defaultValue);

    if(mAltConfig && mAltConfig->contains(key))
        return mAltConfig->value(key, defaultValue);

    return QtXmlSettings::value(key, defaultValue);
}

void ExtendedXmlSettings::setValue(const QString &key, const QVariant &value)
{
    if (mVolatileConfig)
        mVolatileConfig->setValue(key, value);
    else
        QtXmlSettings::setValue(key, value);
}

void ExtendedXmlSettings::beginGroup(const QString &prefix)
{
    if (mVolatileConfig)
        mVolatileConfig->beginGroup(prefix);

    QtXmlSettings::beginGroup(prefix);
}

void ExtendedXmlSettings::endGroup()
{
    if (mVolatileConfig)
        mVolatileConfig->endGroup();

    QtXmlSettings::endGroup();
}
