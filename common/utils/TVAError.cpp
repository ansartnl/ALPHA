#include "TVAError.h"

#include <QtDebug>

Error::Error(const Error &ref) :
    mFile(ref.mFile), mLine(ref.mLine), mReason(ref.mReason), mCode(ref.mCode)
{
}

Error::Error(const char* file, int line, const QString& reason, int code) :
    mFile(file), mLine(line), mReason(reason), mCode(code)
{
    qDebug() << "Error exception at file:" << file << "line:" << line << "reason" << reason;
}

Error::~Error(void)
{
}

const QString& Error::reason() const
{
    return mReason;
}

int Error::code() const
{
    return mCode;
}

const QString& Error::file() const
{
    return mFile;
}

int Error::line() const
{
    return mLine;
}

void Error::setReason(const QString& reason)
{
    if (!reason.isEmpty())
    {
        if (mReason.isEmpty())
            mReason = reason;
        else
            //mReason.insert(0, reason + "%1");
            mReason.insert(0, reason + ". ");
    }
}
