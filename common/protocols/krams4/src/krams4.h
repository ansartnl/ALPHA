#ifndef KRAMS_4_H
#define KRAMS_4_H

#include "krams.h"

#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QVariant>


//! Информационное сообщение КРАМС-4.
class KRAMS_EXPORT Krams4 : public Krams
{
    Q_DECLARE_TR_FUNCTIONS(Krams4)
protected:
    void parseHeader(const QString &header, QList<TKramsError> &errors);
    void parseData(const QString &data, QList<TKramsError> &errors);

    char makeCRC(const QString &data) const;

    QString generateHeader(QList<TKramsError> &errors) const;
    QString generateData(QList<TKramsError> &errors) const;

public:
    //! Код состояния ВПП
    struct SVPPState
    {
        SVPPState() : isCleanDry(false), a(0), b(0), mm(0) {}

        bool        isCleanDry; /** True - ВПП сухая и чистая */
        quint8      a;          /** код состояния поверхности ВПП */
        quint8      b;          /** процент ВПП, покрытый указанными осадками */
        quint8      mm;         /** толщина слоя осадков в мм */
    };

public:
    Krams4();

    static Type classType();
    static QString classVersion();

    virtual Type type() const;
    virtual QString version() const;

    virtual QList<TKramsError> parse(const QByteArray &input);
    virtual QList<TKramsError> generate(QByteArray &result) const;


    //! Append new key-value pair to the list.
    void append(const QString &key, const QVariant &value);
    //! Moves the iterator to the front of the container.
    void toFront();
    //! Iterate to the next data item in the container.
    bool next();
    //! Jumps to the next data item with the "key" from current position.
    bool next(const QString &key);
    //! Retrieve current data parameter number (001, 010, ..)
    QString key() const;
    //! Retrieve associated with the parameter value.
    QVariant value() const;


    //! Date time of this message.
    static const QString DateTime;                  // QDateTime

    //! 001 – содержит информацию о рабочем курсе ВПП.
    static const QString Data001;       // degree - double
    //! 002 - Идентификатор ВПП.
    static const QString Data002;       // QString
    //! 010 - коэффициент сцепления покрытия ВПП
    static const QString Data010;       // uint
    //! 011 - код состояния ВПП
    static const QString Data011;       // SVPPState
    //! 020 - характеристика приземного ветра, цифра кода (*);
    /*!
        0- значительных изменений направления нет, могут использоваться группы 021, 022, 023
        1- значительные изменения направления есть, используются группы  021, 022, 023, 024, 025
        2- ветер неустойчивый
    */
    static const QString Data020;       // uint
    //! 021 - направление ветра у земли, осредненное за 2 мин
    static const QString Data021;       // degree - double
    //! 022 - скорость  ветра у земли, осредненная за 2 мин
    static const QString Data022;       // meter/sec - double
    //! 023 - максимальная скорость ветра у земли
    static const QString Data023;       // meter/sec - double
    //! 024 - минимальное (первое) направление скорости ветра
    static const QString Data024;       // degree - double
    //! 025 - максимальное (второе) направление скорости ветра
    static const QString Data025;       // degree - double
    //! 030 - характеристика ветра на высоте, цифра кода (возможно несколько групп 030)
    /*!
        0 - значительных изменений направления нет, могут использоваться группы 031, 032, 033
        2 - ветер неустойчивый
    */
    static const QString Data030;       // uint
    //! 031 - значение высоты для ветра на высоте
    /*! ( возможно несколько групп 031, 032, 033, 034) */
    static const QString Data031;       // meters
    //! 032 - направление ветра на высоте
    /*! ( возможно несколько групп 031, 032, 033, 034) */
    static const QString Data032;       // degree - double
    //! 033 - скорость ветра на высоте
    /*! ( возможно несколько групп 031, 032, 033, 034) */
    static const QString Data033;       // meter/sec
    //! 034 - максимальная скорость ветра на высоте
    /*! ( возможно несколько групп 031, 032, 033, 034) */
    static const QString Data034;       // meter/sec
    //! 040 - характеристика ветра на высоте круга
    /*!
        0 - значительных изменений направления нет, могут использоваться группы 042, 043
        2 - ветер неустойчивый
    */
    static const QString Data040;       // uint
    //! 042 - направление ветра на высоте круга
    static const QString Data042;       // degree
    //! 043 - скорость ветра на высоте круга
    static const QString Data043;       // meter/sec
    //! 050 - характеристика условий погоды
    /*!
        0 – CAVOK нет
        1 – CAVOK есть
    */
    static const QString Data050;       // uint
    //! 051 - минимальная метеорологическая видимость, выбранная из двух значений MOR (торца и середины ВПП)
    static const QString Data051;       // meters
    //! 060 - минимальная видимость на ВПП, выбранная из двух значений RVR (торца и середины ВПП)
    static const QString Data060;       // meters
    //! 061 - признак предельного измеряемого значения RVR
    /*!
        1 – более указанного значения
        2 – менее указанного значения
    */
    static const QString Data061;       // uint
    //! 100 - характеристика облачности
    /*!
        0 – ясно, облачности нет
        1 – есть 1 слой облачности
        2 – есть 2 слоя облачности
        3 – есть 3 слоя облачности
        9 – состояние неба неизвестно, есть значение вертикальной видимости (используется группа 104)
    */
    static const QString Data100;       // uint
    //! 101 - количество облаков первого слоя
    static const QString Data101;       // octants
    //! 102 - высота нижней границы облаков первого слоя
    static const QString Data102;       // meters
    //! 103 - наличие кучево-дождевой облачности для первого слоя
    /*!
        1 -  первый слой облачности является кучево-дождевым
        2 -  первый слой облачности является мощно-кучевым
    */
    static const QString Data103;       // uint
    //! 104 - вертикальная видимость
    static const QString Data104;       // meters
    //! 111 - количество облаков второго слоя
    static const QString Data111;       // octants
    //! 112 - высота нижней границы облаков второго слоя
    static const QString Data112;       // meters
    //! 113 - наличие кучево-дождевой облачности для второго слоя
    /*!
        1 - слой облачности является кучево-дождевым
        2 - слой облачности является мощно-кучевым
    */
    static const QString Data113;       // uint
    //! 121 - количество облаков третьего слоя
    static const QString Data121;       // octants
    //! 122 - высота нижней границы облаков третьего слоя
    static const QString Data122;       // meters
    //! 123 - наличие кучево-дождевой облачности для третьего слоя
    /*!
        1 - слой облачности является кучево-дождевым
        2 - слой облачности является мощно-кучевым
    */
    static const QString Data123;       // uint
    //! 140 - температура воздуха
    static const QString Data140;       // degree
    //! 141 - температура точки росы
    static const QString Data141;       // degree
    //! 150 - давление QFE (давление приведенное к уровню порога ВПП)
    static const QString Data150;       // мм рт. ст.
    //! 151 - давление QFE (давление приведенное к уровню порога ВПП)
    static const QString Data151;       // гПа
    //! 152 - давление QNH (давление приведенное к уровню моря)
    static const QString Data152;       // гПа
    //! 160 - наличие турбулентности
    /*!
        0 - явления нет
        1 - явление есть, интенсивность умеренная
        2 - явление есть, интенсивность умеренная
        3 - явление есть, интенсивность сильная
    */
    static const QString Data160;       // uint
    //! 161 - нижняя граница слоя турбулентности
    static const QString Data161;       // meters
    //! 162 - верхняя граница слоя турбулентности
    static const QString Data162;       // meters
    //! 170 - наличие обледенения
    /*!
        0 - явления нет
        1 - явление есть, интенсивность слабая
        2 - явление есть, интенсивность умеренная
        3 - явление есть, интенсивность сильная
    */
    static const QString Data170;       // uint
    //! 171 - нижняя граница слоя обледенения
    static const QString Data171;       // meters
    //! 172 - верхняя граница слоя обледенения
    static const QString Data172;       // meters
    //! 173 - местонахождение явления обледенения
    /*!
        1 – в облаках
        2 – в осадках
        3 – в облаках в осадках
    */
    static const QString Data173;       // uint
    //! 180 - наличие грозовых, ливневых или градовых очагов, цифра кода;
    //! возможно несколько очагов одинаковых или разных типов, задаваемых
    //! группами 180. Группы 181-188 относятся к очагу, определяемому
    //! предшествующей группой 180
    /*!
        0 – явления нет
        1 – наличие грозовых очагов
        2 – наличие ливневых очагов
        3 – наличие градовых очагов
    */
    static const QString Data180;       // uint
    //! 181 - азимут местонахождения очага
    static const QString Data181;       // degree
    //! 182 - сектор местонахождения очага (минимальное значение угла)
    static const QString Data182;       // degree
    //! 183 - сектор местонахождения очага (максимальное значение угла)
    static const QString Data183;       // degree
    //! 184 - удаление очага
    static const QString Data184;       // meters
    //! 185 - удаление очага максимальное
    static const QString Data185;       // meters
    //! 186 - направление движения очага
    /*!
        1 – восток
        2 – северо-восток
        3 – север
        4 – северо-запад
        5 – запад
        6 – юго-запад
        7 – юг
        8 – юго-восток
        9 – восток-северо-восток
        10 – северо-северо-восток
        11 – северо-северо-запад
        12 – запад-северо-запад
        13 – запад-юго-запад
        14 – юго-юго-запад
        15 – юго-юго-восток
        16 – восток-юго-восток
    */
    static const QString Data186;       // uint
    //! 187 - скорость движения очага
    static const QString Data187;       // meter/sec
    //! 188 - тенденция изменения интенсивности
    /*!
        0 – без изменений
        1 – усиливается
        2 – ослабевает
    */
    static const QString Data188;       // uint
    //! 190 - информация о сдвиге ветра
    /*!
        0 – явления нет
        1 – явление есть
    */
    static const QString Data190;       // uint
    //! 200 - строка явлений погоды, закодированных в соответствии с WMO Doc.306
    //! (Manual on codes) FM-15/16. Строка переменной длины заканчивается символом = (равно)
    static const QString Data200;       // QString
    //! 201 - строка прогноза типа TREND, закодированных в соответствии с WMO Doc.306 FM-15/16.
    //! Строка переменной длины заканчивается символом = (равно)
    static const QString Data201;       // QString
    //! 210 - информация о закрытии облаками гор/мачт/препятствий
    /*!
        0 – явления нет
        1 – явление есть
    */
    static const QString Data210;       // uint
    //! 220 - информация об электризации в облаках
    /*!
        0 – явления нет
        1 – явление есть
    */
    static const QString Data220;       // QString
    //! 230 - информация о зарницах
    /*!
        0 – явления нет
        1 – явление есть
    */
    static const QString Data230;       // QString

private:
    typedef QPair<QString, QVariant>    TDataPair;
    typedef QList<TDataPair>            TDataList;

    TDataList                           mData;
    /** Simple list iterator */
    TDataList::const_iterator           mIterator;
};

Q_DECLARE_METATYPE(Krams4::SVPPState)

#endif // KRAMS_4_H
