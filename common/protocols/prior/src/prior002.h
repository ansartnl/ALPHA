#ifndef PRIOR002_H
#define PRIOR002_H

#include "priorexport.h"
#include "priorerrors.h"
#include "prior.h"

#include <QtCore/QMap>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class PriorData;
class PresentData;

//! Сообщение "Приор" категории 2.
/*!
    Сообщение этой категории может содержать следующие типы сообщений:
        - Сообщение  «Сектор»
        - Сообщение  «Север»
        - Сообщение  «Пеленг»
        - Сообщение  «Режим / Состояние»
*/
class PRIOR_EXPORT Prior002 : public Prior
{
protected:
    void parse_MessageType(QDataStream &stream, PriorData &map);
    void parse_DataSourceIdentifier(QDataStream &stream, PriorData &map);
    void parse_SectorNumber(QDataStream &stream, PriorData &map);
    void parse_TimeOfDay(QDataStream &stream, PriorData &map);
    void parse_ChannelAzimuth(QDataStream &stream, PriorData &map);
    void parse_PelengARP(QDataStream &stream, PriorData &map);
    void parse_TechnicalState(QDataStream &stream, PriorData &map);

    void parse_North(const PresentData &mainMask, QDataStream &stream, PriorData &map);
    void parse_Sector(const PresentData &mainMask, QDataStream &stream, PriorData &map);
    void parse_Peleng(const PresentData &mainMask, QDataStream &stream, PriorData &map);
    void parse_ModeState(const PresentData &mainMask, QDataStream &stream, PriorData &map);

    bool generate_MessageType(PriorData &map, QDataStream &stream);
    bool generate_DataSourceIdentifier(PriorData &map, QDataStream &stream);
    bool generate_SectorNumber(PriorData &map, QDataStream &stream);
    bool generate_TimeOfDay(PriorData &map, QDataStream &stream);
    bool generate_ChannelAzimuth(PriorData &map, QDataStream &stream);
    bool generate_PelengARP(PriorData &map, QDataStream &stream);
    bool generate_TechnicalState(PriorData &map, QDataStream &stream);

    void generate_North(PresentData &mainMask, PriorData &map, QDataStream &stream);
    void generate_Sector(PresentData &mainMask, PriorData &map, QDataStream &stream);
    void generate_Peleng(PresentData &mainMask, PriorData &map, QDataStream &stream);
    void generate_ModeState(PresentData &mainMask, PriorData &map, QDataStream &stream);

public:
    enum EMessageType
    {
        Peleng      = 0x80,
        North       = 0x81,
        Sector      = 0x82,
        ModeState   = 0x84
    };

    //! Peleng ARP data structure
    struct SPelengARP
    {
        SPelengARP() : code(0) {}

        //! Признак  аварии  общеканальной    аппаратуры  АРП  или  источника  электропитания
        enum DamageCE
        {
            Default,    //!< Норма
            DCE,        //!< Авария  общеканальной  аппаратуры  АРП
            DPS,        //!< Авария  источника  питания  АРП
            DCE_DPS     //!< Авария  ОА  и  источника  питания  АРП
        };

        //! Тип  сообщения
        enum Type
        {
            Intermediate,   //!< Промежуточное  значение  пеленга
            Begin,          //!< Начало  сеанса  радиосвязи
            End,            //!< Конец  сеанса  радиосвязи
            Control         //!< Контрольный  пеленг
        };

        enum
        {
            CD = 0x10   /** Channel damage
                            Авария канала характеризует качество работы данного канала и
                            качество сигнала данного канала на выходе АРП */
        };

        DamageCE damageCE() const { return DamageCE((code >> 2) & 0x03); }
        Type type() const { return Type(code & 0x03); }

        void setDamageCE(DamageCE v) { code |= (quint8(v) & 0x03) << 2; }
        void setType(Type v) { code |= quint8(v) & 0x03; }

    public:
        quint8 code;
    };

public:
    Prior002();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, PriorData &asterix);
    virtual QByteArray generate(PriorData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Message Type
    static const QString MessageType;

    // Data Source Identifier
    static const QString DataSourceIdentifier_SAC;  // System Area Code
    static const QString DataSourceIdentifier_SIC;  // System Identification Code

    // Time of Day
    static const QString TimeOfDay;         // QTime

    // Sector data items:

    // Sector Number or Azimuth
    static const QString SectorNumber;      // degree

    // Peleng data items:

    // Frequency channel number
    static const QString ChannelNumber;     // quint8

    // Peleng ARP - secondary fields
    static const QString PelengARP;         // SPelengARP

    // Technical state
    static const QString TechnicalState;    // QList<quint8>
};

Q_DECLARE_METATYPE(Prior002::SPelengARP)

#endif // PRIOR002_H
