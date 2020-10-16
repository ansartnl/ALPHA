#include "asterix241peleng.h"
#include "shareutility.h"
#include "asterixdata.h"
#include "presentdata.h"

#include <QtCore/QTime>
#include <math.h>


namespace Peleng {

const int Asterix241::mCategory = 241;
const QString Asterix241::mEdition = "1.0";

const QString Asterix241::MessageType = "a241.MessageType";
const QString Asterix241::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix241::DataSourceIdentifier_SIC = ASTERIX_SIC;
const QString Asterix241::TruncatedTimeOfDay = "a241.TruncatedTimeOfDay";
const QString Asterix241::SequenceContourPoints = "a241.SequenceContourPoints";
const QString Asterix241::MessageNumber = "a241.MessageNumber";
const QString Asterix241::TimeStop = "a241.TimeStop";
const QString Asterix241::ContourInfo = "a241.ContourInfo";
const QString Asterix241::ICAO = "a241.ICAO";
const QString Asterix241::RWName = "a241.RWName";
const QString Asterix241::WindCourse = "a241.WindCourse";
const QString Asterix241::WindEstm = "a241.WindEstm";
const QString Asterix241::WindMax = "a241.WindMax";
const QString Asterix241::WindSide = "a241.WindSide";
const QString Asterix241::Temp = "a241.Temp";
const QString Asterix241::DewPoint = "a241.DewPoint";
const QString Asterix241::Vis = "a241.Vis";
const QString Asterix241::DistanceVisBeg = "a241.DistanceVisBeg";
const QString Asterix241::DistanceVisMdl = "a241.DistanceVisMdl";
const QString Asterix241::DistanceVisEnd = "a241.DistanceVisEnd";
const QString Asterix241::HCloud = "a241.HCloud";
const QString Asterix241::QFE = "a241.QFE";
const QString Asterix241::QFEh = "a241.QFEh";
const QString Asterix241::QNHh = "a241.QNHh";


Asterix241::Asterix241()
{}

int Asterix241::category() const
{
    return mCategory;
}

QString Asterix241::edition() const
{
    return mEdition;
}

void Asterix241::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_010(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_000(stream, map);
        else
            throw mandatory("Message Type");

        if (mainMask.isSet(3))
            parse_600(stream, map);

        if (mainMask.isSet(4))
            parse_030(stream, map);

        if (mainMask.isSet(5))
            parse_601(stream, map);


        EMessageType messageType = (EMessageType) map.value(MessageType).toUInt();

        // Contour points.
        if ( messageType == ContourPoints )
        {
            if (mainMask.isSet(6))
                parse_602(stream, map);

            if (mainMask.isSet(7))
                parse_105(stream, map);
        }
        // Information about meteo environment.
        else if ( messageType == Info || messageType == Storm )
        {
            if (mainMask.isSet(8))
                parse_603(stream, map);

            if (mainMask.isSet(9))
                parse_604(stream, map);

            if (mainMask.isSet(10))
                parse_605(stream, map);

            if (mainMask.isSet(11))
                parse_606(stream, map);

            if (mainMask.isSet(12))
                parse_607(stream, map);

            if (mainMask.isSet(13))
                parse_608(stream, map);

            if (mainMask.isSet(14))
                parse_609(stream, map);

            if (mainMask.isSet(15))
                parse_610(stream, map);

            if (mainMask.isSet(16))
                parse_611(stream, map);

            if (mainMask.isSet(17))
                parse_612(stream, map);

            if (mainMask.isSet(18))
                parse_613(stream, map);

            if (mainMask.isSet(19))
                parse_614(stream, map);

            if (mainMask.isSet(20))
                parse_615(stream, map);

            if (mainMask.isSet(21))
                parse_616(stream, map);

            if (mainMask.isSet(22))
                parse_617(stream, map);

            if (mainMask.isSet(23))
                parse_618(stream, map);
        }
        else
            throw AsterixErrors::Error_InvalidInput;

    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Asterix241::generate(AsterixData &map)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (generate_010(map, stream))
            mainMask.set(1);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (generate_000(map, stream))
            mainMask.set(2);
        else
            throw mandatory("Message Type");

        if (generate_600(map, stream))
            mainMask.set(3);

        if (generate_030(map, stream))
            mainMask.set(4);

        if (generate_601(map, stream))
            mainMask.set(5);


        EMessageType messageType = (EMessageType) map.value(MessageType).toUInt();

        // Information about meteo environment.
        if ( messageType == Info )
        {
            if (generate_603(map, stream))
                mainMask.set(8);

            if (generate_604(map, stream))
                mainMask.set(9);

            if (generate_605(map, stream))
                mainMask.set(10);

            if (generate_606(map, stream))
                mainMask.set(11);

            if (generate_607(map, stream))
                mainMask.set(12);

            if (generate_608(map, stream))
                mainMask.set(13);

            if (generate_609(map, stream))
                mainMask.set(14);

            if (generate_610(map, stream))
                mainMask.set(15);

            if (generate_611(map, stream))
                mainMask.set(16);

            if (generate_612(map, stream))
                mainMask.set(17);

            if (generate_613(map, stream))
                mainMask.set(18);

            if (generate_614(map, stream))
                mainMask.set(19);

            if (generate_615(map, stream))
                mainMask.set(20);

            if (generate_616(map, stream))
                mainMask.set(21);

            if (generate_617(map, stream))
                mainMask.set(22);

            if (generate_618(map, stream))
                mainMask.set(23);
        }
        // Contour points and storm message type are not supported.
        else
            throw AsterixErrors::Error_InvalidInput;

    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
    return result.prepend(mainMask.data());
}

void Asterix241::parse_000(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageType, int(read<quint8>(stream)));
}

void Asterix241::parse_010(QDataStream &stream, AsterixData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Asterix241::parse_030(QDataStream &stream, AsterixData &map)
{
    quint32 timeOfDay = read24(stream) / 128;
    map.insert(TruncatedTimeOfDay, QTime().addMSecs(timeOfDay*1000));
}

void Asterix241::parse_105(QDataStream &stream, AsterixData &map)
{
    quint8 rep = read<quint8>(stream);

    TSequenceContourPointsList list;
    const qreal lsbFactor = 180.0/(1024*1024*32);
    const double h = map.contains(ContourInfo) ?
                map.value(ContourInfo).value<SContourInfo>().HRadioecho : 0.;

    for ( int i = 0; i < rep; i++ )
    {
        quint8 b = read<qint32>(stream);
        quint8 l = read<qint32>(stream);

        SPoint point;
        point.b = lsbFactor * b;
        point.l = lsbFactor * l;
        point.h = h;

        list.append( QVariant::fromValue(point) );
    }

    if ( !list.empty() )
        map.insert(SequenceContourPoints, list);
}

void Asterix241::parse_600(QDataStream &stream, AsterixData &map)
{
    map.insert(MessageNumber, read<qint16>(stream));
}

void Asterix241::parse_601(QDataStream &stream, AsterixData &map)
{
    QTime timeStop = map.contains(TruncatedTimeOfDay) ?
                map.value(TruncatedTimeOfDay).toTime() : QTime::currentTime();

    quint8 hourMs = read<quint8>(stream)*3600*1000;
    timeStop.addMSecs( hourMs );

    map.insert(TimeStop, timeStop);
}

void Asterix241::parse_602(QDataStream &stream, AsterixData &map)
{
    SContourInfo ci;
    ci.contNumber = read<quint8>(stream);
    ci.codeEvent = read<quint8>(stream);

    qint32 i32 = read<qint32>(stream);
    qint16 i16 = 0;

    i16 = (qint16)(i32>>22) & 0x3FF;
    ci.HRadioecho = i16;
    i16=(qint16)(i32>>12) & 0x3FF;
    ci.VelRadioecho = i16;
    i16=(qint16)(i32>>3) & 0x1FF;
    ci.CourseRadioecho = i16;

    map.insert(ContourInfo, QVariant::fromValue(ci));
}

void Asterix241::parse_603(QDataStream &stream, AsterixData &map)
{
    map.insert(ICAO, read<quint32>(stream));
}

void Asterix241::parse_604(QDataStream &stream, AsterixData &map)
{
    QString rwName = "RW";
    quint8 byte = read<quint8>(stream);

    quint8 b;
    int a1, a2;
    b=(byte>>2)&0x3F;
    a1=(int)b;
    a2=a1/10;
    a1=a1-a2*10;
    b=quint8(a2) + 0x30;
    rwName += QChar(b);
    b=quint8(a1) + 0x30;
    rwName += QChar(b);
    if((byte & 0x03)==0x03)
        rwName += QChar('M');
    if((byte & 0x03)==0x02)
        rwName += QChar('R');
    if((byte & 0x03)==0x01)
        rwName += QChar('L');

    map.insert(RWName, rwName);
}

void Asterix241::parse_605(QDataStream &stream, AsterixData &map)
{
    map.insert(WindCourse, read<qint16>(stream));
}

void Asterix241::parse_606(QDataStream &stream, AsterixData &map)
{
    map.insert(WindEstm, read<quint8>(stream));
}

void Asterix241::parse_607(QDataStream &stream, AsterixData &map)
{
    map.insert(WindMax, read<quint8>(stream));
}

void Asterix241::parse_608(QDataStream &stream, AsterixData &map)
{
    map.insert(WindSide, (double)read<qint16>(stream) / 10.);
}

void Asterix241::parse_609(QDataStream &stream, AsterixData &map)
{
    map.insert(Temp, (double)read<qint16>(stream) / 10.);
}

void Asterix241::parse_610(QDataStream &stream, AsterixData &map)
{
    map.insert(DewPoint, (double)read<qint16>(stream) / 10.);
}

void Asterix241::parse_611(QDataStream &stream, AsterixData &map)
{
    map.insert(Vis, read<qint16>(stream));
}

void Asterix241::parse_612(QDataStream &stream, AsterixData &map)
{
    map.insert(DistanceVisBeg, read<qint16>(stream));
}

void Asterix241::parse_613(QDataStream &stream, AsterixData &map)
{
    map.insert(DistanceVisMdl, read<qint16>(stream));
}

void Asterix241::parse_614(QDataStream &stream, AsterixData &map)
{
    map.insert(DistanceVisEnd, read<qint16>(stream));
}

void Asterix241::parse_615(QDataStream &stream, AsterixData &map)
{
    map.insert(HCloud, read<qint16>(stream));
}

void Asterix241::parse_616(QDataStream &stream, AsterixData &map)
{
    map.insert(QFE, read<qint16>(stream));
}

void Asterix241::parse_617(QDataStream &stream, AsterixData &map)
{
    map.insert(QFEh, read<qint16>(stream));
}

void Asterix241::parse_618(QDataStream &stream, AsterixData &map)
{
    map.insert(QNHh, read<qint16>(stream));
}

bool Asterix241::generate_000(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_010(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DataSourceIdentifier_SAC)
        && map.contains(DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_030(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TruncatedTimeOfDay))
    {
        int secs = QTime(0,0,0,0).msecsTo(map.value(TruncatedTimeOfDay).toTime()) * 128 / 1000;
        write24(stream, secs);
        return true;
    }
    return false;
}

bool Asterix241::generate_600(AsterixData &map, QDataStream &stream)
{
    if (map.contains(MessageNumber))
    {
        write16<qint16>(stream, map.value(MessageNumber).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_601(AsterixData &map, QDataStream &stream)
{
    if (map.contains(TimeStop) && map.contains(TruncatedTimeOfDay))
    {
        QTime timeOfDay = map.value(TimeStop).toTime();
        QTime timeStop = map.value(TruncatedTimeOfDay).toTime();

        int dtGet, dtStop, dt;
        dtGet=timeOfDay.hour();
        dtStop=timeStop.hour();
        if(dtGet>dtStop)
            dtStop=dtStop+24;
        dt=dtStop-dtGet;

        write8(stream, dt);
        return true;
    }
    return false;
}

bool Asterix241::generate_603(AsterixData &map, QDataStream &stream)
{
    if (map.contains(ICAO))
    {
        write32(stream, map.value(ICAO).toUInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_604(AsterixData &map, QDataStream &stream)
{
    if (map.contains(RWName))
    {
        QString rwName = map.value(RWName).toString();
        QString str="00";
        int cours;
        if ( rwName.length()>=4 )
            str = rwName.mid(2,3);
        cours = (str[0].toLatin1()-0x30)*10 + (str[1].toLatin1()-0x30);
        quint8 b;
        b = (quint8(cours)<<2) & 0xFC;
        if(str[2]=='L') b = b | 0x01;
        if(str[2]=='R') b = b | 0x02;
        if(str[2]=='M') b = b | 0x03;

        write8(stream, b);
        return true;
    }
    return false;
}

bool Asterix241::generate_605(AsterixData &map, QDataStream &stream)
{
    if (map.contains(WindCourse))
    {
        write16<qint16>(stream, map.value(WindCourse).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_606(AsterixData &map, QDataStream &stream)
{
    if (map.contains(WindEstm))
    {
        write8(stream, map.value(WindEstm).toUInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_607(AsterixData &map, QDataStream &stream)
{
    if (map.contains(WindMax))
    {
        write8(stream, map.value(WindMax).toUInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_608(AsterixData &map, QDataStream &stream)
{
    if (map.contains(WindSide))
    {
        write16<qint16>(stream, map.value(WindSide).toDouble()*10);
        return true;
    }
    return false;
}

bool Asterix241::generate_609(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Temp))
    {
        write16<qint16>(stream, map.value(Temp).toDouble()*10);
        return true;
    }
    return false;
}

bool Asterix241::generate_610(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DewPoint))
    {
        write16<qint16>(stream, map.value(DewPoint).toDouble()*10);
        return true;
    }
    return false;
}

bool Asterix241::generate_611(AsterixData &map, QDataStream &stream)
{
    if (map.contains(Vis))
    {
        write16<qint16>(stream, map.value(Vis).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_612(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DistanceVisBeg))
    {
        write16<qint16>(stream, map.value(DistanceVisBeg).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_613(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DistanceVisMdl))
    {
        write16<qint16>(stream, map.value(DistanceVisMdl).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_614(AsterixData &map, QDataStream &stream)
{
    if (map.contains(DistanceVisEnd))
    {
        write16<qint16>(stream, map.value(DistanceVisEnd).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_615(AsterixData &map, QDataStream &stream)
{
    if (map.contains(HCloud))
    {
        write16<qint16>(stream, map.value(HCloud).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_616(AsterixData &map, QDataStream &stream)
{
    if (map.contains(QFE))
    {
        write16<qint16>(stream, map.value(QFE).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_617(AsterixData &map, QDataStream &stream)
{
    if (map.contains(QFEh))
    {
        write16<qint16>(stream, map.value(QFEh).toInt());
        return true;
    }
    return false;
}

bool Asterix241::generate_618(AsterixData &map, QDataStream &stream)
{
    if (map.contains(QNHh))
    {
        write16<qint16>(stream, map.value(QNHh).toInt());
        return true;
    }
    return false;
}

QStringList Asterix241::fields() const
{
    static QStringList result = QStringList()

            << MessageType
            << DataSourceIdentifier_SAC
            << DataSourceIdentifier_SIC
            << TruncatedTimeOfDay
            << SequenceContourPoints
            << MessageNumber
            << TimeStop
            << ContourInfo
            << ICAO
            << RWName
            << WindCourse
            << WindEstm
            << WindMax
            << WindSide
            << Temp
            << DewPoint
            << Vis
            << DistanceVisBeg
            << DistanceVisMdl
            << DistanceVisEnd
            << HCloud
            << QFE
            << QFEh
            << QNHh;

    return result;
}

}   // Eurocontrol
