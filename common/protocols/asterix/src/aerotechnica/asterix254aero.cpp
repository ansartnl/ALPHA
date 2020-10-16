#include "asterix254aero.h"
#include "shareutility.h"
#include "asterixdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>


namespace Aerotechnica {

const int Asterix254::mCategory = 254;
const QString Asterix254::mEdition = "A1000";

const QString Asterix254::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix254::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix254::MessageType = "MessageType";


Asterix254::Asterix254()
{}

int Asterix254::category() const
{
    return mCategory;
}

QString Asterix254::edition() const
{
    return mEdition;
}

void Asterix254::parse(QDataStream &stream, AsterixData &asterix)
{
    try
    {
        quint8 type = read<quint8>(stream);
        asterix.insert(MessageType, type);

        // Message type is always 1 or 2.
        if ( type != 1 || type != 2 )
            throw AsterixErrors::Error_InvalidInput;

        asterix.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
        asterix.insert(DataSourceIdentifier_SIC, read<quint8>(stream));

        // spare byte read
        if ( type == 2 )
            read<quint8>(stream);
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

QByteArray Asterix254::generate(AsterixData &asterix)
{
    QByteArray result;

    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        quint8 type = 1;
        if (asterix.contains(MessageType))
        {
            type = asterix.value(MessageType).toUInt();
            if ( !(type == 1 || type == 2) )
                throw AsterixErrors::Error_InvalidInput;
        }
        else
            asterix.appendError(mandatory("Message Type"));

        // Message type
        write8(stream, type);

        if (asterix.contains(DataSourceIdentifier_SAC) && asterix.contains(DataSourceIdentifier_SIC))
        {
            write8(stream, asterix.value(DataSourceIdentifier_SAC).toUInt());
            write8(stream, asterix.value(DataSourceIdentifier_SIC).toUInt());
        }
        else
            asterix.appendError(mandatory("Data Source Identifier"));

        // write spare byte
        if (type == 2)
            write8(stream, 0);
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

QStringList Asterix254::fields() const
{
    static QStringList result = QStringList()

        << DataSourceIdentifier_SAC
        << DataSourceIdentifier_SIC;

    return result;
}

}   // namespace Aerotechnica {
