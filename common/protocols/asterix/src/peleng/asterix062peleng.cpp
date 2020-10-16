#include "asterix062peleng.h"

#include "asterixdata.h"
#include "presentdata.h"
#include "shareutility.h"

namespace Peleng
{

const QString Asterix062::SP_BoardNumber = "a62.SP_BoardNumber";
const QString Asterix062::SP_FuelHeight = "a62.SP_FuelHeight";
const QString Asterix062::SP_Warning = "a62.SP_Warning";

Asterix062::Asterix062() {}

QStringList Asterix062::fields() const
{
    static QStringList result = Asterix062Proto::fields()
            << SP_BoardNumber
            << SP_FuelHeight
            << SP_Warning;

    return result;
}

void Asterix062::parse_sp(QDataStream &stream, AsterixData &map)
{
    quint8 len = read<quint8>(stream);
    if ( len < 2 )
        return;

    PresentData fspec(stream);

    // Read "Board Number"
    if (fspec.isSet(6))
    {
        const quint8 data = read<quint8>(stream);

        BoardNumber bn;
        bn.V = data & 0x80;
        bn.G = data & 0x40;
        bn.L = data & 0x20;
        bn.number = quint32(data & 0x0F) << 16 | read<quint16>(stream);

        map.insert(SP_BoardNumber, QVariant::fromValue(bn));
    }

    // Read "Fuel and Height"
    if (fspec.isSet(7))
    {
        quint8 data = read<quint8>(stream);

        FuelHeight fh;
        fh.A = data & 0x20;
        fh.B = data & 0x10;
        fh.fuel = data & 0x0F;

        data = read<quint8>(stream);

        fh.V = data & 0x80;
        fh.G = data & 0x40;

        if (data & 0x20)
            fh.height = quint16(0xE0 | (data & 0x1F)) << 8 | read<quint8>(stream);
        else
            fh.height = quint16(data & 0x1F) << 8 | read<quint8>(stream);

        map.insert(SP_FuelHeight, QVariant::fromValue(fh));
    }

    // Read "Warning Message"
    if (fspec.isSet(8))
    {
        Warning w;
        w.type = read<quint8>(stream);

        int count = read<quint8>(stream);
        for (int i = 0; i < count; ++i)
        {
            w.tracks.append(read<quint16>(stream));
        }

        if (w.type == 4)
        {
            w.time = read<quint8>(stream);
            w.distance = read<quint16>(stream);
        }

        map.insert(SP_Warning, QVariant::fromValue(w));
    }
}

bool Asterix062::generate_sp(AsterixData &map, QDataStream &stream)
{
    bool isBoardNumber = map.contains(SP_BoardNumber);
    bool isFuelHeight = map.contains(SP_FuelHeight);
    bool isWarning = map.contains(SP_Warning);

    if ( !(isBoardNumber || isFuelHeight || isWarning) )
        return false;

    QByteArray result;
    {
        PresentData mainMask;
        QDataStream streamSP(&result, QIODevice::WriteOnly);

        // Save "Board Number"
        if (isBoardNumber)
        {
            BoardNumber bn = map.value(SP_BoardNumber).value<BoardNumber>();

            quint8 data = 0;
            if (bn.V)
                data |= 0x80;

            if (bn.G)
                data |= 0x40;

            if (bn.L)
                data |= 0x20;

            data |= (bn.number >> 16) & 0x0F;

            write8(streamSP, data);
            write16(streamSP, quint16(bn.number));

            mainMask.set(6);
        }

        // Save "Fuel and Height"
        if (isFuelHeight)
        {
            FuelHeight fh = map.value(SP_FuelHeight).value<FuelHeight>();

            quint8 data = 0;
            if (fh.A)
                data |= 0x20;

            if (fh.B)
                data |= 0x10;

            data |= fh.fuel;

            write8(streamSP, data);

            data = 0;
            if (fh.V)
                data |= 0x80;

            if (fh.G)
                data |= 0x40;

            if (fh.height < 0)
                data |= 0x20;

            data |= (fh.height >> 8) & 0x1F;

            write8(streamSP, data);
            write8(streamSP, quint8(fh.height));

            mainMask.set(7);
        }

        // Save "Warning Message"
        if (isWarning)
        {
            Warning w = map.value(SP_Warning).value<Warning>();
            quint8 count = w.tracks.count();

            write8(streamSP, w.type);
            write8(streamSP, count);

            for (int i = 0; i < count; ++i)
            {
                write16(streamSP, w.tracks[i]);
            }

            if (w.type == 4)
            {
                write8(streamSP, w.time);
                write16(streamSP, w.distance);
            }

            mainMask.set(8);
        }

        result.prepend(mainMask.data());
    }

    write8(stream, result.size() + 1);
    writeArray(stream, result);

    return true;
}

}
