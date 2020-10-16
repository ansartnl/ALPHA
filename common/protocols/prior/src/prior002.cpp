#include "prior002.h"
#include "shareutility.h"
#include "priordata.h"
#include "presentdata.h"

#include <QtCore/QVector>
#include <QtCore/QTime>

#include <math.h>

/*!
    ПРИМЕЧАНИЯ (сообщение «Пеленг») :

    1.  Авария  канала  (9-й  байт) характеризует    качество  работы  данного  канала
        и  качество  сигнала  данного  канала   на  выходе  АРП.

    2.	Авария  ОА  (9-й  байт)  -  признак  аварии  общеканальной    аппаратуры  АРП
        или  источника  электропитания.

    Кодирование  аварии  ОА :
    ———————————————————————
       4   3      бит   9-го  байта
       0   0      Норма
       0   1      Авария  общеканальной  аппаратуры  АРП
       1   0      Авария  источника  питания  АРП
       1   1      Авария  ОА  и  источника  питания  АРП

    3.  Тип  сообщения  (9-й  байт)  кодируется   следующим  образом :
    ————————————————————————
           2   1     бит   9-го  байта
    ————————————————————————
           0   0     Промежуточное  значение  пеленга
           0   1     Начало  сеанса  радиосвязи
           1   0     Конец  сеанса  радиосвязи
           1   1     Контрольный  пеленг

    4.  Разряды  номера   частотного  канала  (10-й  байт)   содержат  двоичный код,  который
        на единицу меньше  реального номера  канала  АРП.

    5.  Значение  пеленга  (10 - 11  байты) отсчитывается от направления  на  “Север”
        относительно   точки  стояния  АРП  по  часовой  стрелке.

    6.  Временной  интервал  между  промежуточными  пеленгами  может   составлять  по
        согласованию   1 / 2 / 5  сек.

ПРИМЕЧАНИЕ (сообщение «Режим / Состояние») :

    1.  Байты  технического  состояния содержат
        биты - признаки  режимов  работы  и  технического  состояния
        модулей  ПРЛ, ВРЛ, АПОИ,  модемов и каналов связи.
        Все байты  техсостояния  передаются только  при  использовании  БПРД.
        При  отсутствии  БПРД передаются  только  байты  1 и 2 техсостояния .

    2. Количество  и  состав  байтов  технического  состояния
        уточняются  для  конкретных  ПРЛ,  ВРЛ  и  АПОИ.
        При  передаче  информации  техсостояния  от  КА  РЛП
        “Радуга-2”  через  БПРД  используются  следующие  биты -
        признаки :

        ТС  1   ЦУ  ПРЛ	ТС 33  ЦУ  Контр.  ответчика
        ТС  2   ВКЛ  ПРМ–ПРД-1	ТС 34  Норма  Контр.  ответч.
        ТС  3   Норма  ПРМ–ПРД-1	ТС 35  Предупр. Контр. ответч.
        ТС  4   Авария  ПРМ-ПРД-1	ТС 36  Авария  Контр. ответч.
        ТС  5   ( резерв )	ТС 37  Пожар
        ТС  6   ВКЛ  ПРМ-ПРД-2	ТС 38  Охрана
        ТС  7   Норма  ПРМ-ПРД-2	ТС 39  Подогрев
        ТС  8   Авария  ПРМ-ПРД-2	ТС 40  Авария  фидера питания
        ТС  9   ВКЛ  Привода	ТС 41  ЦУ  ПРД-2
        ТС 10  Норма  Привода	ТС 42  ЦУ  Привода-2
        ТС 11  Авария  Привода	ТС 43  ВКЛ  ПРД-2
        ТС 12  Работа  2-х  двигателей	ТС 44  ВКЛ  Привода-2
        ТС 13  ( резерв )	ТС 45  Норма  ПРД-2
        ТС 14  ( резерв )	ТС 46  Норма  Привода-2
        ТС 15  ( резерв )	ТС 47  Норма  АОИ  Перв.-2
        ТС 16  ( резерв )	ТС 48  Норма  АОИ  Втор.-2
        ТС 17  ЦУ  ПРД-1	ТС 49  Предупрежд.  ПРД-2
        ТС 18  ЦУ  Привода-1	ТС 50  Авария  ПРД-2
        ТС 19  ВКЛ  ПРД-1	ТС 51  Авария  Привода-2
        ТС 20  ВКЛ  Проивода-1	ТС 52  Авария  АОИ  Перв.-2
        ТС 21  Норма  ПРД-1	ТС 53  Авария  АОИ  Втор.-2
        ТС 22  Норма  Привода-1	ТС 54  Пред. по накоп. Ошибки
        ТС 23  Норма  АОИ  Перв.-1	ТС 55  Авария  внешнего зап.
        ТС 24  Норма  АОИ  Втор.-1	ТС 56  Рабочее место КДП
        ТС 25  Предупрежд.  ПРД-1	ТС 57  Запросный  код  1
        ТС 26  Авария  ПРД-1	ТС 58  Запросный  код  2
        ТС 27  Авария  Привода-1	ТС 59  Запросный  код  3
        ТС 28  Авария  АОИ  Перв.-1	ТС 60  Запросный  код  4
        ТС 29  Авария  АОИ  Втор.-1	ТС 61  Запросный  код  А
        ТС 30  ВКЛ  Заградит.  Огни	ТС 62  Запросный  код  С
        ТС 31  Авария Заград. Огней	ТС 63  Запросный  код  I
        ТС 32  ДУ	ТС 64  Запросный  код  II

*/

const int Prior002::mCategory = 2;
const QString Prior002::mEdition = "1.0";

const QString Prior002::MessageType = "pr002.MessageType";

const QString Prior002::DataSourceIdentifier_SAC = PRIOR_SAC;
const QString Prior002::DataSourceIdentifier_SIC = PRIOR_SIC;

const QString Prior002::SectorNumber = "pr002.SectorNumber";

const QString Prior002::TimeOfDay = "pr002.TimeOfDay";

const QString Prior002::ChannelNumber = "pr002.ChannelNumber";
const QString Prior002::PelengARP = "pr002.PelengARP";

const QString Prior002::TechnicalState = "pr002.TechnicalState";


Prior002::Prior002()
{}

int Prior002::category() const
{
    return mCategory;
}

QString Prior002::edition() const
{
    return mEdition;
}

void Prior002::parse(QDataStream &stream, PriorData &map)
{
    try
    {
        PresentData mainMask(stream);

        if (mainMask.isSet(1))
            parse_DataSourceIdentifier(stream, map);
        else
            map.appendError(mandatory("Data Source Identifier"));

        if (mainMask.isSet(2))
            parse_MessageType(stream, map);
        else
            throw mandatory("Message Type");

        uint typeMessage = map.value(MessageType).toUInt();

        if (typeMessage == uint(Sector))
            parse_Sector(mainMask, stream, map);
        else if (typeMessage == uint(North))
            parse_North(mainMask, stream, map);
        else if (typeMessage == uint(Peleng))
            parse_Peleng(mainMask, stream, map);
        else if (typeMessage == uint(ModeState))
            parse_ModeState(mainMask, stream, map);
        else
            throw PriorErrors::Error_InvalidInput;

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

void Prior002::parse_North(const PresentData &mainMask, QDataStream &stream, PriorData &map)
{
    if (mainMask.isSet(4))
        parse_TimeOfDay(stream, map);
}

void Prior002::parse_Sector(const PresentData &mainMask, QDataStream &stream, PriorData &map)
{
    if (mainMask.isSet(3))
        parse_SectorNumber(stream, map);
    else
        map.appendError(mandatory("Sector Number"));

    if (mainMask.isSet(4))
        parse_TimeOfDay(stream, map);
}

void Prior002::parse_Peleng(const PresentData &mainMask, QDataStream &stream, PriorData &map)
{
    if (mainMask.isSet(6))
    {
        parse_PelengARP(stream, map);
        parse_ChannelAzimuth(stream, map);
    }
    else
        map.appendError(mandatory("Peleng Items"));
}

void Prior002::parse_ModeState(const PresentData &mainMask, QDataStream &stream, PriorData &map)
{
    if (mainMask.isSet(4))
        parse_TimeOfDay(stream, map);

    if (mainMask.isSet(5))
        parse_TechnicalState(stream, map);
    else
        map.appendError(mandatory("Technical state"));
}

QByteArray Prior002::generate(PriorData &map)
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

        if (generate_MessageType(map, stream))
            mainMask.set(2);
        else
            throw mandatory("Message Type");

        uint typeMessage = map.value(MessageType).toUInt();

        if (typeMessage == uint(Sector))
            generate_Sector(mainMask, map, stream);
        else if (typeMessage == uint(North))
            generate_North(mainMask, map, stream);
        else if (typeMessage == uint(Peleng))
            generate_Peleng(mainMask, map, stream);
        else if (typeMessage == uint(ModeState))
            generate_ModeState(mainMask, map, stream);
        else
            throw PriorErrors::Error_InvalidInput;

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

void Prior002::generate_North(PresentData &mainMask, PriorData &map, QDataStream &stream)
{
    if (generate_TimeOfDay(map, stream))
        mainMask.set(4);
}

void Prior002::generate_Sector(PresentData &mainMask, PriorData &map, QDataStream &stream)
{
    if (generate_SectorNumber(map, stream))
        mainMask.set(3);
    else
        map.appendError(mandatory("Sector Number"));

    if (generate_TimeOfDay(map, stream))
        mainMask.set(4);
}

void Prior002::generate_Peleng(PresentData &mainMask, PriorData &map, QDataStream &stream)
{
    if (map.contains(ChannelNumber) && map.contains(SectorNumber))
    {
        generate_PelengARP(map, stream);
        generate_ChannelAzimuth(map, stream);
        mainMask.set(6);
    }
    else
        map.appendError(mandatory("Peleng Items"));
}

void Prior002::generate_ModeState(PresentData &mainMask, PriorData &map, QDataStream &stream)
{
    if (generate_TimeOfDay(map, stream))
        mainMask.set(4);

    if (generate_TechnicalState(map, stream))
        mainMask.set(5);
    else
        map.appendError(mandatory("Technical state"));
}

QStringList Prior002::fields() const
{
    static QStringList result = QStringList()

    << MessageType

    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    << SectorNumber

    << TimeOfDay

    << ChannelNumber
    << PelengARP

    << TechnicalState;

    return result;
}

void Prior002::parse_MessageType(QDataStream &stream, PriorData &map)
{
    map.insert(MessageType, read<quint8>(stream));
}

bool Prior002::generate_MessageType(PriorData &map, QDataStream &stream)
{
    if (map.contains(MessageType))
    {
        write8(stream, map.value(MessageType).toUInt());
        return true;
    }
    return false;
}

void Prior002::parse_DataSourceIdentifier(QDataStream &stream, PriorData &map)
{
    map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
    map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
}

bool Prior002::generate_DataSourceIdentifier(PriorData &map, QDataStream &stream)
{
    if (map.contains(DataSourceIdentifier_SAC) && map.contains(DataSourceIdentifier_SIC))
    {
        write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
        write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        return true;
    }
    return false;
}

void Prior002::parse_SectorNumber(QDataStream &stream, PriorData &map)
{
    quint8 byte = read<quint8>(stream) & 0x07;
    // Convert 5-bit code -> angle (degree)
    double angle = 0.;
    for ( int i = 7; i >= 0; i-- )
        angle += 180.*(byte >> i)/double(8-i);

    map.insert(SectorNumber, angle);
}

bool Prior002::generate_SectorNumber(PriorData &map, QDataStream &stream)
{
    if (map.contains(SectorNumber))
    {
        quint8 byte = 0;
        // Convert angle (degree) -> 5-bit code
        double angle = map.value(SectorNumber).toDouble();
        for ( int i = 7; i >= 0; i-- )
        {
            double angleCur = angle - 180./double(8-i);
            if ( angleCur >= 0. || qFuzzyIsNull(angleCur) )
            {
                angle = angleCur;
                byte |= 1 << i;
            }
        }

        write8(stream, byte);
        return true;
    }
    return false;
}

void Prior002::parse_TimeOfDay(QDataStream &stream, PriorData &map)
{
    map.insert(TimeOfDay, QTime(0,0).addMSecs(ceil(double(read24(stream)) / 128. * 1000.)));
}

bool Prior002::generate_TimeOfDay(PriorData &map, QDataStream &stream)
{
    if (map.contains(TimeOfDay))
    {
        write24(stream, double(QTime(0,0).msecsTo(map.value(TimeOfDay).toTime())) * 128. / 1000.);
        return true;
    }
    return false;
}

void Prior002::parse_PelengARP(QDataStream &stream, PriorData &map)
{
    SPelengARP peleng;
    peleng.code = read<quint8>(stream);
    map.insert(PelengARP, QVariant::fromValue(peleng));
}

bool Prior002::generate_PelengARP(PriorData &map, QDataStream &stream)
{
    if (map.contains(PelengARP))
    {
        SPelengARP peleng = map.value(PelengARP).value<SPelengARP>();
        write8(stream, peleng.code);
        return true;
    }
    return false;
}

void Prior002::parse_ChannelAzimuth(QDataStream &stream, PriorData &map)
{
    quint16 byte2 = read<quint16>(stream);
    quint8 channelNumber = quint8(byte2 >> 12);

    byte2 &= 0x0FFF;
    // Convert 12-bit code -> angle (degree)
    double angle = 0.;
    for ( int i = 11; i >= 0; i-- )
        angle += 180.*(byte2 >> i)/double(12-i);

    map.insert(SectorNumber, angle);
    map.insert(ChannelNumber, channelNumber);
}

bool Prior002::generate_ChannelAzimuth(PriorData &map, QDataStream &stream)
{
    if (map.contains(SectorNumber) && map.contains(ChannelNumber))
    {
        quint16 byte2 = quint16(map.value(ChannelNumber).toUInt()) << 12;

        // Convert angle (degree) -> 12-bit code
        double angle = map.value(SectorNumber).toDouble();
        for ( int i = 11; i >= 0; i-- )
        {
            double angleCur = angle - 180./double(12-i);
            if ( angleCur >= 0. || qFuzzyIsNull(angleCur) )
            {
                angle = angleCur;
                byte2 |= 1 << i;
            }
        }

        write16(stream, byte2);
        return true;
    }
    return false;
}

void Prior002::parse_TechnicalState(QDataStream &stream, PriorData &map)
{
    quint8 byte = 0;
    QList<quint8> techList;
    do
    {
        byte = read<quint8>(stream);
        techList.append(byte >> 1);
    } while (byte & 0x01);

    map.insert(TechnicalState, QVariant::fromValue(techList));
}

bool Prior002::generate_TechnicalState(PriorData &map, QDataStream &stream)
{
    if (map.contains(TechnicalState))
    {
        const QList<quint8> techList = map.value(TechnicalState).value< QList<quint8> >();
        if (techList.isEmpty())
            return false;

        QList<quint8>::const_iterator itc = techList.begin();
        quint8 byte = *itc << 1;
        for ( ++itc; itc != techList.end(); ++itc )
        {
            write8(stream, byte | 0x01);
            byte = *itc << 1;
        }
        write8(stream, byte);
        return true;
    }
    return false;
}
