#include "priordata.h"
#include "prior.h"

#include <QtCore/QDebug>

const PriorErrors PriorData::okError = PriorErrors::OK;

PriorData::PriorData(int cat)
    : category(cat)
{}

PriorData::PriorData(const PriorData &other)
    : category(other.category), mData(other.mData)
{}

PriorData::~PriorData()
{
    clearErrors();
}

bool PriorData::contains(const QString& field) const
{
    return mData.contains(field);
}

bool PriorData::contains(const QStringList &fields) const
{
    foreach (const QString &field, fields)
        if (!mData.contains(field))
            return false;
    return true;
}

QVariant PriorData::value(const QString& field) const
{
    return mData.value(field);
}

void PriorData::insert(const QString& field, const QVariant &value)
{
    if (value.isValid())
        mData.insert(field, value);
}

PriorErrors::ErrorCode PriorData::fromArray(const QByteArray& source, int *readed)
{
    clearErrors();
    mData.clear();
    category = 0;

    if (readed)
        *readed = 0;

    int dataSize = source.size();
    if (dataSize < 3)
        return appendError(PriorErrors::Error_MessageIsShort);

    QDataStream stream(source);
    stream.setByteOrder(QDataStream::BigEndian);

    stream >> category;
    QScopedPointer<Prior> parser(priorFactory.create(category));

    quint8 length;
    stream >> length;

    if (length < 3)
    {
        if (readed)
            *readed = 2;
        return appendError(PriorErrors::Error_InputCorrupted);
    }

    if (readed)
        *readed = length;

    if (!parser)
        return appendError(PriorErrors::Error_ProtocolNotSupported);

    parser->setLength(length);
    parser->parse(stream, *this);
    return firstError()->code();
}

PriorErrors::ErrorCode PriorData::toArray(QByteArray& output)
{
    clearErrors();

    QByteArray result;
    QScopedPointer<Prior> generator(priorFactory.create(category));
    if (generator)
        result = generator->generate(*this);
    else
        appendError(PriorErrors::Error_ProtocolNotSupported);

    output.clear();
    QDataStream stream(&output, QIODevice::WriteOnly);
    stream << quint8(category);
    stream << quint8(2 + result.size());
    stream.writeRawData(result.data(), result.size());

    return firstError()->code();
}

const QMap<QString, QVariant> &PriorData::data() const
{
    return mData;
}

void PriorData::setData(const QMap<QString, QVariant> &data)
{
    mData = data;
}

const PriorErrors *PriorData::firstError() const
{
    return mErrors.isEmpty() ? &okError : mErrors.first();
}

const PriorErrors *PriorData::lastError() const
{
    return mErrors.isEmpty() ? &okError : mErrors.last();
}

const QList<PriorErrors *> &PriorData::errors() const
{
    return mErrors;
}

void PriorData::clearErrors()
{
    qDeleteAll(mErrors);
    mErrors.clear();
}

PriorErrors::ErrorCode PriorData::appendError(PriorErrors::ErrorCode code)
{
    mErrors.append(new PriorErrors(code));
    return code;
}

PriorErrors::ErrorCode PriorData::appendError(PriorErrors *error)
{
    mErrors.append(error);
    return error->code();
}
