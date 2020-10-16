#ifndef OPERATIONSLOG_H
#define OPERATIONSLOG_H

#include <QtCore/QFile>
#include <QtCore/QMutex>

class OperationsLog
{
public:
    OperationsLog(const QString &path, int maxFileSize, int maxFilesIndex, const QString &owner);

    bool add(const QString &text);

private:
    bool isFileTooBig() const;
    void cycleFiles();

private:
    mutable QMutex mutex;

    QFile mFile;
    const int mMaxFileSize;
    const int mMaxFilesIndex;
    const QString mOwner;
};

OperationsLog & operationsLog(
        const QString &path = QString()
        , int maxFileSize = 10000000
        , int maxFilesIndex = 10
        , const QString &owner = "owner"
        );

void logOperation(const QString &text);

#endif // OPERATIONSLOG_H
