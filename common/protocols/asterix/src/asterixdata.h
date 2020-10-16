#ifndef ASTERIXDATA_H
#define ASTERIXDATA_H

#include "asterixexport.h"
#include "asterixvariety.h"
#include "asterixerrors.h"

#include <QtCore/QMap>
#include <QtCore/QVariant>

class ASTERIX_EXPORT AsterixData
{
public:
    AsterixData(int category = 0, AsterixVariety variety = AsterixEurocontrol);
    AsterixData(const AsterixData &other);
    virtual ~AsterixData();

    static int categoryFromArray(const QByteArray &source);

    quint8 category;
    AsterixVariety variety;

    bool contains(const QString &field) const;
    bool contains(const QStringList &fields) const;
    QVariant value(const QString &field) const;
    void insert(const QString &field, const QVariant &value);

    //! Parse raw buffer data.
    /*!
        \param[in] data Buffer with data
        \param[out] readed Number of readed bytes
    */
    AsterixErrors::ErrorCode fromArray(const QByteArray &source, int *readed = 0);
    AsterixErrors::ErrorCode toArray(QByteArray &result);

    const AsterixErrors *firstError() const;
    const AsterixErrors *lastError() const;
    const QList<AsterixErrors *> &errors() const;
    void clearErrors();

    AsterixErrors::ErrorCode appendError(AsterixErrors *error);
    AsterixErrors::ErrorCode appendError(AsterixErrors::ErrorCode code);

    const QMap<QString, QVariant> &data() const;
    void setData(const QMap<QString, QVariant> &data);

    //! Enable/disable CRC check summ in the data buffer.
    /*! When CRC check summ is enabled then input data buffer will be read 16bit more
        and CRC check summ will be validated.
        When CRC check summ is enabled in the case of output buffer - CRC check summ
        will be generated and added into the resulting data buffer.
    */
    void setCRCCheckEnabled(bool on) { mIsCheckCRC = on; }

private:
    //! Returns true if byte array contains valid data.
    bool testCRC(quint16 crc, const QByteArray &data, quint16 length);

private:
    QMap<QString, QVariant> mData;
    QList<AsterixErrors *>  mErrors;
    /** True when CRC check summ should be used in test procedure and data buffer */
    bool                    mIsCheckCRC;

    static const AsterixErrors okError;
};

#endif // ASTERIXDATA_H
