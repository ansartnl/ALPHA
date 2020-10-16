#include "OperationsLog.h"

#include <QtCore/QDateTime>

OperationsLog & operationsLog(const QString &path, int maxFileSize, int maxFilesIndex, const QString &owner)
{
    static OperationsLog mLog(path, maxFileSize, maxFilesIndex, owner);
    return mLog;
}

void logOperation(const QString &text)
{
    operationsLog().add(text);
}

OperationsLog::OperationsLog(const QString &path, int maxFileSize, int maxFilesIndex, const QString &owner) :
    mFile(path), mMaxFileSize(maxFileSize), mMaxFilesIndex(maxFilesIndex), mOwner(owner)
{
}

bool OperationsLog::add(const QString &text)
{
    QMutexLocker locker(&mutex);

    if (isFileTooBig())
        cycleFiles();

    if (!mFile.open(QFile::Append))
        return false;
    mFile.write(
                QString("[%1] - %2 - %3\n")
                .arg(QDateTime::currentDateTimeUtc().toString("yyyy.MM.dd hh:mm:ss"))
                .arg(mOwner)
                .arg(text)
                .toUtf8()
                );
    mFile.close();
    return true;
}

bool OperationsLog::isFileTooBig() const
{
    return (mFile.size() >= mMaxFileSize);
}

void OperationsLog::cycleFiles()
{
    if (QFile::exists(QString("%1.%2").arg(mFile.fileName()).arg(mMaxFilesIndex)))
        QFile::remove(QString("%1.%2").arg(mFile.fileName()).arg(mMaxFilesIndex));

    for (int i = mMaxFilesIndex - 1; i > 0; --i) {
        if (QFile::exists(QString("%1.%2").arg(mFile.fileName()).arg(i)))
            QFile::rename(QString("%1.%2").arg(mFile.fileName()).arg(i)
                          , QString("%1.%2").arg(mFile.fileName()).arg(i + 1));
    }

    QFile::rename(mFile.fileName(), QString("%1.1").arg(mFile.fileName()));
}
