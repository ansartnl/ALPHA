#include "qtinisettings.h"

#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>

static bool readIniFile(QIODevice &device, QSettings::SettingsMap &map);
static bool writeIniFile(QIODevice &device, const QSettings::SettingsMap &map);

const QSettings::Format QtIniSettings::ini_format = QSettings::registerFormat(
            "ini", readIniFile, writeIniFile);

bool readIniFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QRegExp rx("\\[([\\w\\s/]+)\\]");

    QString key, value;

    QTextStream stream(&device);
    while ( !stream.atEnd() )
    {
        QString line = stream.readLine();

        if ( rx.indexIn(line) != -1 )
        {
            QString keyPrev = key;
            key = rx.cap(1).trimmed();

            value = value.trimmed();
            if ( !keyPrev.isEmpty() )
                map[keyPrev] = value;

            value = QString::null;
            continue;
        }

        value += QString("\n%1").arg(line);
    }

    value = value.trimmed();
    if ( !key.isEmpty() )
        map[key] = value;

    return true;
}

bool writeIniFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    QTextStream stream(&device);

    QMapIterator<QString, QVariant> it(map);
    while ( it.hasNext() )
    {
        it.next();

        stream << QString("[%1]").arg(it.key()) << endl;

        // Write value ...

        QVariant value = it.value();
        if ( value.type() == QVariant::String )
        {
            QStringList stringList = value.toString().split(QRegExp("(\\n|\\r\\n)"), QString::SkipEmptyParts);
            foreach ( const QString &s, stringList )
                stream << s << endl;
            stream << endl;
        }
        else
        {
            stream << value.toString() << endl << endl;
        }
    };

    return true;
}
