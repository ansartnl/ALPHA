#ifndef MAIN_H
#define MAIN_H

#include <QtCore/QSettings>

//! Retrieve project xml settings.
QSettings & xmlSettings(const QString &pathConfig = QString());

#endif // MAIN_H
