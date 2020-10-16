#ifndef KRAMS_4_DUMP_H
#define KRAMS_4_DUMP_H

#include "krams.h"

#include <QtCore/QPair>
#include <QtCore/QStringList>
#include <QtCore/QVariant>


//! Информационное сообщение КРАМС-4, полученное из дампа Душанбе.
class KRAMS_EXPORT Krams4dump : public Krams
{
    Q_DECLARE_TR_FUNCTIONS(Krams4dump)
protected:
    void parseData(const QStringList &dataList, QList<TKramsError> &errors);
    void calculateData();

public:
    //! Код состояния ВПП
    struct SVPPState
    {
        SVPPState() : isCleanDry(false), cond(0xFF), level(0xFF), thickness(0), number(0), coef(0xFF) {}

        bool        isCleanDry; /** чисто - условия покрытия ВПП */
        quint8      cond;       /** условия покрытия ВПП
                                    0 - чисто и сухо; 1 - влажно; 2 - мокро или вода местами
                                    3 - иней или изморозь; 4 - сухой снег; 5 - мокрый снег
                                    6 - слякоть; 7 - лед; 8 - уплотненный или укатанный снег
                                    9 - замерзшая или неровная поверхность
                                    0xFF - уловия покрытия не указаны
                                */
        quint8      level;      /** степень покрытия ВПП
                                    1 - менее 10%; 2 - от 11-25%; 5 - от 26-50%; 9 - 51-100%
                                    0xFF - данных нет
                                */
        quint8      thickness;  /** толщина покрытия ВПП
                                    00 - менее 1 мм; 01 - 1 мм; 02 - 2мм; ... 90 - 90мм;
                                    91 - не используется; 92 - 10см; 93 - 15см; 94 - 20см;
                                    96 - 25см; 97 -30см; 98 - 35см
                                */
        quint8      number;     /** номер ВПП */
        quint8      coef;       /** эффективность торможения
                                    91 - плохая; 92 - плохая/средняя; 93 - средняя; 94 - средняя/хорошая;
                                    95 - хорошая; 99 - ненадежное измерение
                                    0xFF - эффективность торможения не сообщена, ВПП не работает, аэродром закрыт
                                */
    };

public:
    Krams4dump();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;

    //! Check whether parsed map contains a key.
    bool contains(int key) const;
    //! Retrieve value by the key.
    QVariant value(int key) const;



    //! 01 - Date time of this message.
    static const int DateTime;                      // QDateTime

    //! 02 - курс
    static const int Course;                        // int
    //! 04 - код состояния ВПП
    static const int VPPState;                      // SVPPState
    //! 05 - время суток
    static const int TimeOfDay;                     // QString
    //! 10 - имя ВПП
    static const int VPPName;                       // QString
    //! 11 - Идентификатор аэропорта
    static const int AirportId;                     // QString
    //! 12 - имя аэропорта
    static const int AirportName;                   // QString
    //! 13 - идентификатор ВПП с начала
    static const int VPPidBegin;                    // QString
    //! 14 - идентификатор ВПП с конца
    static const int VPPidEnd;                      // QString
    //! 36 - QNH, гПа
    static const int QNHhPa;                        // double
    //! 37 - Температура, градусы
    static const int Temperature;                   // double
    //! 38 - Влажность, %
    static const int Humidity;                      // double
    //! 39 - Температура точки росы, градусы
    static const int DewPoint;                      // double
    //! 42 - Видимость в начале ВПП, метры
    static const int VisBegin;                      // double
    //! 43 - Видимость в середине ВПП, метры
    static const int VisMiddle;                     // double
    //! 44 - Видимость в конце ВПП, метры
    static const int VisEnd;                        // double
    //! 46 - высота нижней границы облачности, метры
    static const int CloudsLow;                     // int
    //! 48 - направление ветра, градусы
    static const int WindDirectionBegin;            // int
    //! 49 - средняя скорость ветра, м/с
    static const int WindSpeedAverageBegin;         // int
    //! 51 - направление ветра, градусы
    static const int WindDirectionEnd;              // int
    //! 52 - средняя скорость ветра, м/с
    static const int WindSpeedAverageEnd;           // int
    //! 54 - QFE, мм
    static const int QFEmmBegin;                    // double
    //! 55 - QFE, гПа
    static const int QFEhPaBegin;                   // double
    //! 56 - QFE, мм
    static const int QFEmmEnd;                      // double
    //! 57 - QFE, гПа
    static const int QFEhPaEnd;                     // double
    //! 61 - Видимость в начале ВПП, метры
    static const int VisOVIBegin;                   // double
    //! 62 - Видимость в середине ВПП, метры
    static const int VisOVIMiddle;                  // double
    //! 63 - Видимость в конце ВПП, метры
    static const int VisOVIEnd;                     // double

    // Calculated fields.

    //! 201 - Боковая составляющая ветра в начале полосы, м/с
    static const int WindSpeedSideBegin;            // int
    //! 202 - Попутная составляющая ветра в начале полосы, м/с
    static const int WindSpeedFollowBegin;          // int
    //! 203 - Встречная составляющая ветра в начале полосы, м/с
    static const int WindSpeedOppositeBegin;        // int
    //! 211 - Боковая составляющая ветра в конце полосы, м/с
    static const int WindSpeedSideEnd;              // int
    //! 212 - Попутная составляющая ветра в конце полосы, м/с
    static const int WindSpeedFollowEnd;            // int
    //! 213 - Встречная составляющая ветра в конце полосы, м/с
    static const int WindSpeedOppositeEnd;          // int

private:
    typedef QPair<QString, QVariant>    TDataPair;
    typedef QList<TDataPair>            TDataList;

    typedef QMap<int, QVariant>         TDataMap;

    TDataMap    mValues;
};

Q_DECLARE_METATYPE(Krams4dump::SVPPState)

#endif // KRAMS_4_H
