#ifndef INITMAIN_H
#define INITMAIN_H

#include <QtCore/QVariant>

/*!
  * \namespace InitMain
  * \brief Инициализация глобальных переменных
  *
  * Содержит фуннкции глобальной инициализации.
  */

class QSettings;

namespace InitMain {
void setupSearchPath(const QString& applicationDirPath);
void setupSearchPath();
void setupCodec(const QByteArray& codecName = "UTF-8");
QVariant initValue(QSettings& settings, const QString& key, const QVariant& defaultValue = QVariant(), const QString& comment = QString::null);

//! Searching of configuration file on disk.
/*! Function searching in the next order:
    1. Searches file in the same folder as executable and the same name as executable
       application name
    2. Searches file on the path defined as "etc" with name of executable.
    3. Searches file on the path defined as "etc" with name "config".

    \return Path to file or null QString if file is not found.

    \note setupSearchPath() function should be called first.
*/
QString findConfigFilePath(const QString& applicationDirPath, const QString& applicationName);
QString findConfigFilePath();
//! for QtService
QString findConfigFilePath(int argc, char** argv);

QString findXmlFilePath(const QString& applicationDirPath, const QString& typeConfig);

//! Usage-help string where config file can be located.
QString configUsageString();
}

#endif // INITMAIN_H
