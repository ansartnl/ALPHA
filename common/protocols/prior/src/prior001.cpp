#include "prior001.h"
#include "shareutility.h"
#include "priordata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>

/*!
ПРИМЕЧАНИЕ:
    1.  Во  всех  сообщениях   идентификационный  код  ( байт # 6 )  содержит  две  двоично - десятичные  цифры  идентификационного кода  страны,  в  которой  размещен источник  информации.
        Для  России  и  большинства  других стран  СНГ  старшая  цифра   -  5.
        По  согласованию  поставщиков  взаимодействующего  оборудования  в  определенных случаях  идентификационный код  страны может  не передаваться.

    2.	Во  всех  сообщениях  байт # 7,  идентифицирующий   код  источника,    формируется  следующим  образом :

             ХХХХХ001      ВРЛ
             ХХХХХ010      ПРЛ
             ХХХХХ011      комбинированный
             ХХХХХ000      АРП

        Первые  пять  разрядов  предствляют  конкретную  РЛП  и  определяются Администрацией  Заказчика.  По  согласованию  поставщиков  взаимодействующего  оборудования  в  определенных  случаях  идентифицирующий  код  источника может  не  передаваться.

    3.  В  координатных  сообщениях  данные  о  местоположении  воздушного  судна передаются  в  прямоугольных  координатах  правой  системы  координат   относительно  точки  стояния  ПРЛ / ВРЛ :
           ось   OX   направлена   на    восток;
           ось   OY   направлена   на   север.

    4.  В координатных сообщениях отрицательные значения координат передаются в дополнительном коде.

    5.   В случае, если от источника координатной информации не  поступают данные о сокращенном времени суток, соответствующие байты  (## 14 и 15) могут  заполняться  нулями.

    6.   Бортовой  номер  международного  канала  в  режиме  З/А  (байты  ## 16 и 17) передается  4-мя цифрами  (A,  B,  C,  D)  двоично - восьмеричного  кода.

    7.	 Кодирование расширения (байты ## 16,  18):  0 - нет расширения;
                                                     1 - есть расширение.

    8.   Кодирование бита знака (байт # 18):      0 - положительный;
                                                  1 – отрицательный.

    9.   Кодирование достоверности (байты ## 16, 18):   0 - достоверный;
                                                        1 -  достоверный

ПРИМЕЧАНИЕ:
    1. Бортовой  номер  отечественного  канала  в  режиме  номера  борта  (байты  ## 16 - 18)  передается  5-ю  цифрами  (A,  B,  C,  D,  E)   двоично-десятичного  кода.

    2.  Остаток топлива (байт  # 19) кодируется следующим образом :

        5    %	0001
        10  %	0010
        15  %	0011
        20  %	0100
        25  %	0101
        30  %	0110
        35  %	0111
        40  %	1000
        45  %	1001
        50  %	1010
        60  %	1011
        70  %	1100
        80  %	1101
        90  %	1110
        100 %	1111
*/

const int Prior001::mCategory = 1;
const QString Prior001::mEdition = "1.0";

const QString Prior001::DataSourceIdentifier_SAC = PRIOR_SAC;
const QString Prior001::DataSourceIdentifier_SIC = PRIOR_SIC;

const QString Prior001::TargetReportDescriptor = "pr001.TargetReportDescriptor";

const QString Prior001::CalculatedPositionInCartesianCoordinates_X = "pr001.Coordinate.X";
const QString Prior001::CalculatedPositionInCartesianCoordinates_Y = "pr001.Coordinate.Y";

const QString Prior001::Mode3ACode = "pr001.Mode3ACode";

const QString Prior001::ModeCCode = "pr001.ModeCCode";

const QString Prior001::TruncatedTimeOfDay = "pr001.TruncatedTimeOfDay";

const QString Prior001::BoardNumber = "pr001.BoardNumber";

const QString Prior001::FlightLevel = "pr001.FlightLevel";

const QString Prior001::FuelReserve = "pr001.FuelReserve";


Prior001::Prior001()
{}

int Prior001::category() const
{
    return mCategory;
}

QString Prior001::edition() const
{
    return mEdition;
}

void Prior001::parse(QDataStream &stream, PriorData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_DataSourceIdentifier(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_TargetReportDescriptor(stream, map);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (mainMask.isSet(3))
            parse_Coords(stream, map);
        else
            map.appendError(mandatory("Calculated Position in Cartesian Coordinates"));

        parse_TimeOfDay(stream, map);

        if (mainMask.isSet(4))
            parse_Mode3ACode(stream, map);

        if (mainMask.isSet(5))
            parse_ModeCCode(stream, map);

        if (mainMask.isSet(6))
            parse_BoardNumber(stream, map);

        if (mainMask.isSet(7))
        {
            parse_FuelReserve(stream, map);
            parse_FlightLevel(stream, map);
        }

    }
    catch (PriorErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (PriorErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(PriorErrors::Error_UnexpectedEndOfData);
    }
}

QByteArray Prior001::generate(PriorData &map)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        if (generate_DataSourceIdentifier(map, stream))
            mainMask.set(1);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (generate_TargetReportDescriptor(map, stream))
            mainMask.set(2);
        else
            map.appendError(mandatory("Target Report Descriptor"));

        if (generate_Coords(map, stream))
            mainMask.set(3);
        else
            map.appendError(mandatory("Calculated Position in Cartesian Coordinates"));

        generate_TimeOfDay(map, stream);

        if (generate_Mode3ACode(map, stream))
            mainMask.set(4);

        if (generate_ModeCCode(map, stream))
            mainMask.set(5);

        if (generate_BoardNumber(map, stream))
            mainMask.set(6);

        if (map.contains(FuelReserve) && map.contains(FlightLevel))
        {
            generate_FuelReserve(map, stream);
            generate_FlightLevel(map, stream);
            mainMask.set(7);
        }

    }
    catch (PriorErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (PriorErrors *error)
    {
        map.appendError(error);
    }
    return result.prepend(mainMask.data());
}

QStringList Prior001::fields() const
{
    static QStringList result = QStringList()

        << DataSourceIdentifier_SAC
        << DataSourceIdentifier_SIC

        << TargetReportDescriptor

        << CalculatedPositionInCartesianCoordinates_X
        << CalculatedPositionInCartesianCoordinates_Y

        << Mode3ACode

        << ModeCCode

        << TruncatedTimeOfDay

        << BoardNumber

        << FlightLevel;

    return result;
}

void Prior001::parse_DataSourceIdentifier(QDataStream &stream, PriorData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

void Prior001::parse_TargetReportDescriptor(QDataStream &stream, PriorData &map)
{
    STargetReportDescriptor trd;
    trd.part = read<quint8>(stream);
    if ( trd.part & 0x01 )
    {
        quint8 byte = read<quint8>(stream);
        trd.extents.append(byte);
    }
    map.insert(TargetReportDescriptor, QVariant::fromValue(trd));
}

void Prior001::parse_Coords(QDataStream &stream, PriorData &map)
{
    map.insert(CalculatedPositionInCartesianCoordinates_X, read<qint16>(stream) * 15);
    map.insert(CalculatedPositionInCartesianCoordinates_Y, read<qint16>(stream) * 15);
}

void Prior001::parse_TimeOfDay(QDataStream &stream, PriorData &map)
{
    map.insert(TruncatedTimeOfDay, QTime(0,0).addMSecs(double(read<quint16>(stream)) / 128. * 1000.));
}

void Prior001::parse_Mode3ACode(QDataStream &stream, PriorData &map)
{
    SMode3ACode mode3A;
    mode3A.code = read<quint16>(stream);
    map.insert(Mode3ACode, QVariant::fromValue(mode3A));
}

void Prior001::parse_ModeCCode(QDataStream &stream, PriorData &map)
{
    SModeCCode modeC;
    modeC.code = read<quint16>(stream);
    map.insert(Mode3ACode, QVariant::fromValue(modeC));
}

void Prior001::parse_BoardNumber(QDataStream &stream, PriorData &map)
{
    SBoardNumber number;
    number.code = read24(stream);
    map.insert(BoardNumber, QVariant::fromValue(number));
}

void Prior001::parse_FuelReserve(QDataStream &stream, PriorData &map)
{
    quint8 byte = read<quint8>(stream) & 0x0F;
    // Convert 4-bit enumeration -> %
    quint8 fuel = byte * 5;
    if ( fuel > 50 )
        fuel = 50 + (byte - 10)*10;
    map.insert(FuelReserve, fuel);
}

void Prior001::parse_FlightLevel(QDataStream &stream, PriorData &map)
{
    SFlightLevel flightLevel;
    flightLevel.code = read<quint16>(stream);
    map.insert(BoardNumber, QVariant::fromValue(flightLevel));
}

bool Prior001::generate_DataSourceIdentifier(PriorData &map, QDataStream &stream)
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

bool Prior001::generate_TargetReportDescriptor(PriorData &map, QDataStream &stream)
{
    if (map.contains(TargetReportDescriptor))
    {
        STargetReportDescriptor trd =
                map.value(TargetReportDescriptor).value<STargetReportDescriptor>();
        write8(stream, trd.part);
        if ( !trd.extents.empty() )
            write8(stream, trd.extents[0]);
        return true;
    }
    return false;
}

bool Prior001::generate_Coords(PriorData &map, QDataStream &stream)
{
    if (map.contains(CalculatedPositionInCartesianCoordinates_X)
        && map.contains(CalculatedPositionInCartesianCoordinates_Y))
    {
        write16<qint16>(stream, map.value(CalculatedPositionInCartesianCoordinates_X).toInt() / 15);
        write16<qint16>(stream, map.value(CalculatedPositionInCartesianCoordinates_Y).toInt() / 15);
        return true;
    }
    return false;
}

bool Prior001::generate_TimeOfDay(PriorData &map, QDataStream &stream)
{
    if (map.contains(TruncatedTimeOfDay))
    {
        write16(stream, double(QTime(0,0).msecsTo(map.value(TruncatedTimeOfDay).toTime())) / 1000. * 128.);
        return true;
    }
    return false;
}

bool Prior001::generate_Mode3ACode(PriorData &map, QDataStream &stream)
{
    if (map.contains(Mode3ACode))
    {
        SMode3ACode mode3A = map.value(Mode3ACode).value<SMode3ACode>();
        write16(stream, mode3A.code);
        return true;
    }
    return false;
}

bool Prior001::generate_ModeCCode(PriorData &map, QDataStream &stream)
{
    if (map.contains(ModeCCode))
    {
        SModeCCode modeC = map.value(ModeCCode).value<SModeCCode>();
        write16(stream, modeC.code);
        return true;
    }
    return false;
}

bool Prior001::generate_BoardNumber(PriorData &map, QDataStream &stream)
{
    if (map.contains(BoardNumber))
    {
        SBoardNumber number = map.value(BoardNumber).value<SBoardNumber>();
        write24(stream, number.code);
        return true;
    }
    return false;
}

bool Prior001::generate_FuelReserve(PriorData &map, QDataStream &stream)
{
    if (map.contains(FuelReserve))
    {
        quint8 fuel = map.value(FuelReserve).toUInt();
        // Convert % -> 4-bit enumeration
        quint8 byte = fuel / 5;
        if ( byte > 10 )
            byte = 10 + (fuel-50)/10;
        write8(stream, byte);
        return true;
    }
    return false;
}

bool Prior001::generate_FlightLevel(PriorData &map, QDataStream &stream)
{
    if (map.contains(FlightLevel))
    {
        SFlightLevel flightLevel = map.value(FlightLevel).value<SFlightLevel>();
        write16(stream, flightLevel.code);
        return true;
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////////
// SModeCCode implementation

qint32 Prior001::SModeCCode::height() const
{
    return (restoreSign(code, 14) * 10);
}

void Prior001::SModeCCode::setHeight(qint32 v)
{
    code |= qint16(v / 10) & 0x3FFF;
}


///////////////////////////////////////////////////////////////////////////////////
// SFlightLevel implementation

qint32 Prior001::SFlightLevel::altitude() const
{
    return (restoreSign(code, 14) * 10);
}

void Prior001::SFlightLevel::setAltitude(qint32 v)
{
    code |= qint16(v / 10) & 0x3FFF;
}


///////////////////////////////////////////////////////////////////////////////////
// SFlightLevel implementation

quint32 Prior001::SBoardNumber::number() const
{
    return (code & 0x000FFFFF);
}

void Prior001::SBoardNumber::setNumber(quint32 v)
{
    code |= v & 0x000FFFFF;
}
