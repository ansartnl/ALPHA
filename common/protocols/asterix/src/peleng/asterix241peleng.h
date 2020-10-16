#ifndef PELENG_ASTERIX241_H
#define PELENG_ASTERIX241_H

#include "asterix.h"


//! Peleng standard of Asterix protocol
namespace Peleng {

class ASTERIX_EXPORT Asterix241 : public Asterix
{
    void parse_000(QDataStream &stream, AsterixData &map);
    void parse_010(QDataStream &stream, AsterixData &map);
    void parse_030(QDataStream &stream, AsterixData &map);
    void parse_105(QDataStream &stream, AsterixData &map);
    void parse_600(QDataStream &stream, AsterixData &map);
    void parse_601(QDataStream &stream, AsterixData &map);
    void parse_602(QDataStream &stream, AsterixData &map);
    void parse_603(QDataStream &stream, AsterixData &map);
    void parse_604(QDataStream &stream, AsterixData &map);
    void parse_605(QDataStream &stream, AsterixData &map);
    void parse_606(QDataStream &stream, AsterixData &map);
    void parse_607(QDataStream &stream, AsterixData &map);
    void parse_608(QDataStream &stream, AsterixData &map);
    void parse_609(QDataStream &stream, AsterixData &map);
    void parse_610(QDataStream &stream, AsterixData &map);
    void parse_611(QDataStream &stream, AsterixData &map);
    void parse_612(QDataStream &stream, AsterixData &map);
    void parse_613(QDataStream &stream, AsterixData &map);
    void parse_614(QDataStream &stream, AsterixData &map);
    void parse_615(QDataStream &stream, AsterixData &map);
    void parse_616(QDataStream &stream, AsterixData &map);
    void parse_617(QDataStream &stream, AsterixData &map);
    void parse_618(QDataStream &stream, AsterixData &map);

    bool generate_000(AsterixData &map, QDataStream &stream);
    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_030(AsterixData &map, QDataStream &stream);
    bool generate_600(AsterixData &map, QDataStream &stream);
    bool generate_601(AsterixData &map, QDataStream &stream);
    bool generate_603(AsterixData &map, QDataStream &stream);
    bool generate_604(AsterixData &map, QDataStream &stream);
    bool generate_605(AsterixData &map, QDataStream &stream);
    bool generate_606(AsterixData &map, QDataStream &stream);
    bool generate_607(AsterixData &map, QDataStream &stream);
    bool generate_608(AsterixData &map, QDataStream &stream);
    bool generate_609(AsterixData &map, QDataStream &stream);
    bool generate_610(AsterixData &map, QDataStream &stream);
    bool generate_611(AsterixData &map, QDataStream &stream);
    bool generate_612(AsterixData &map, QDataStream &stream);
    bool generate_613(AsterixData &map, QDataStream &stream);
    bool generate_614(AsterixData &map, QDataStream &stream);
    bool generate_615(AsterixData &map, QDataStream &stream);
    bool generate_616(AsterixData &map, QDataStream &stream);
    bool generate_617(AsterixData &map, QDataStream &stream);
    bool generate_618(AsterixData &map, QDataStream &stream);

public:
    //! Structure: Contour information
    struct SContourInfo
    {
        quint8      contNumber;
        quint8      codeEvent;
        qint16      HRadioecho;
        qint16      VelRadioecho;
        qint16      CourseRadioecho;
    };

    //! Structure: Contour Point
    struct SPoint
    {
        qreal   b;      // degree
        qreal   l;      // degree
        qreal   h;      // meter
    };
    // Sequence of Contour Points
    typedef QVariantList        TSequenceContourPointsList;

    enum EMessageType
    {
        ContourPoints   = 1,
        Info            = 2,
        Storm           = 3
    };

public:
    Asterix241();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;


    // Data Item I241/000, Message Type
    static const QString MessageType;                   // EMessageType

    // Data Item I241/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC;
    static const QString DataSourceIdentifier_SIC;

    // Data Item I241/090, Time of Day
    static const QString TruncatedTimeOfDay;            // QTime

    // Data Item I241/105, Sequense of contour points
    static const QString SequenceContourPoints;         // TSequenceContourPointsList

    // Data Item I241/600, Message Number
    static const QString MessageNumber;                 // qint16

    // Data Item I241/601, Time Stop
    static const QString TimeStop;                      // QTime

    // Data Item I241/602
    static const QString ContourInfo;                   // SContourInfo

    // Data Item I241/603, ICAO identifier
    static const QString ICAO;                          // quint32

    // Data Item I241/604, RWName
    static const QString RWName;                        // QString

    // Data Item I241/605, Магнитное направление ветра (градусы)
    static const QString WindCourse;

    // Data Item I241/606, Средняя скорость ветра (м/с)
    static const QString WindEstm;

    // Data Item I241/607, Максимальная скорость ветра (м/с)
    static const QString WindMax;

    // Data Item I241/608, Боковая состовляющая ветра относительно ВПП (м/с)
    static const QString WindSide;

    // Data Item I241/609, Temperature
    static const QString Temp;

    // Data Item I241/610, Точка росы
    static const QString DewPoint;

    // Data Item I241/611, Метеорологическая дальность видимости (м/с)
    static const QString Vis;

    // Data Item I241/612, Метеорологическая дальность видимости ОВИ в начале ВПП (м/с)
    static const QString DistanceVisBeg;

    // Data Item I241/613, Метеорологическая дальность видимости ОВИ в середине ВПП (м/с)
    static const QString DistanceVisMdl;

    // Data Item I241/614, Метеорологическая дальность видимости ОВИ в конце ВПП (м/с)
    static const QString DistanceVisEnd;

    // Data Item I241/615, Высота нижней границы облачности (метры)
    static const QString HCloud;

    // Data Item I241/616, Давление на рабочем пороге ВПП (мм.рт.ст.)
    static const QString QFE;

    // Data Item I241/617, Давление на рабочем пороге ВПП (гПа)
    static const QString QFEh;

    // Data Item I241/618, Давление, приведенное к уровню моря (гПа)
    static const QString QNHh;
};

}   // Eurocontrol

Q_DECLARE_METATYPE(Peleng::Asterix241::SContourInfo)
Q_DECLARE_METATYPE(Peleng::Asterix241::SPoint)

#endif // PELENG_ASTERIX241_H
