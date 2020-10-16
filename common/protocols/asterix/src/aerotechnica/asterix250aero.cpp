#include "asterix250aero.h"
#include "shareutility.h"
#include "asterixdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


namespace Aerotechnica {

const int Asterix250::mCategory = 250;
const QString Asterix250::mEdition = "A1000";

const QString Asterix250::RawVideo = "RawVideo";


Asterix250::Asterix250()
{}

int Asterix250::category() const
{
    return mCategory;
}

QString Asterix250::edition() const
{
    return mEdition;
}

void Asterix250::parse(QDataStream &stream, AsterixData &asterix)
{
    try
    {
        if ( mLength < 4 )
            throw AsterixErrors::Error_InvalidInput;

        // 3 - 1 byte - category, 2 bytes - length
        asterix.insert(RawVideo, readArray(stream, mLength-3));
    }
    catch (AsterixErrors::ErrorCode error)
    {
        asterix.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        asterix.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            asterix.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Asterix250::generate(AsterixData &asterix)
{
    QByteArray result;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (asterix.contains(RawVideo))
            writeArray(stream, asterix.value(RawVideo).toByteArray());
        else
            asterix.appendError(mandatory("Raw Video"));
    }
    catch (AsterixErrors::ErrorCode error)
    {
        asterix.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        asterix.appendError(error);
    }
    return result;
}

QStringList Asterix250::fields() const
{
    static QStringList result = QStringList()

        << RawVideo;

    return result;
}

}   // namespace Aerotechnica {
