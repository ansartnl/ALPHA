#ifndef PRIOR_DATA_H
#define PRIOR_DATA_H

#include "priorexport.h"
#include "priorerrors.h"

#include <QtCore/QMap>
#include <QtCore/QVariant>

class PRIOR_EXPORT PriorData
{
public:
    PriorData(int category = 0);
    PriorData(const PriorData &other);

    virtual ~PriorData();

    quint8 category;

    bool contains(const QString &field) const;
    bool contains(const QStringList &fields) const;
    QVariant value(const QString &field) const;
    void insert(const QString &field, const QVariant &value);

    //! Parse raw buffer data.
    /*!
        \param[in] data Buffer with data
        \param[out] readed Number of readed bytes
    */
    PriorErrors::ErrorCode fromArray(const QByteArray &data, int *readed = 0);
    PriorErrors::ErrorCode toArray(QByteArray &result);

    const PriorErrors *firstError() const;
    const PriorErrors *lastError() const;
    const QList<PriorErrors *> &errors() const;
    void clearErrors();

    PriorErrors::ErrorCode appendError(PriorErrors *error);
    PriorErrors::ErrorCode appendError(PriorErrors::ErrorCode code);

    const QMap<QString, QVariant> &data() const;
    void setData(const QMap<QString, QVariant> &data);

private:
    QMap<QString, QVariant> mData;
    QList<PriorErrors *>  mErrors;
    /** True when CRC check summ should be used in test procedure and data buffer */
    bool                    mIsCheckCRC;

    static const PriorErrors okError;
};

#endif // PRIOR_DATA_H
