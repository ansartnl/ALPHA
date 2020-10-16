#include "asterixtest.h"
#include "asterixdata.h"
#include "asterix001.h"
#include "asterix002.h"
#include "asterix008.h"
#include "asterix021.h"
#include "asterix034.h"
#include "asterix048.h"
#include "asterix062.h"
#include "asterix063.h"
#include "asterix.h"
#include "tests/asterixFuncs.h"

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QBitArray>
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtTest/QtTest>

Q_DECLARE_METATYPE(AsterixData);

AsterixTest::AsterixTest(QObject *parent) : QObject(parent)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    setRandomTestChance();
}

void AsterixTest::setRandomTestChance()
{
    chance = rnd() % 101;
}

bool AsterixTest::testChance() const
{
    return rnd() % 100 < chance;
}

QString AsterixTest::translate(const QByteArray& array)
{
    QString result;
    for (int i = 0; i < array.size(); ++i)
        result.append(QString("%1 ").arg(int(array[i]), 2, 16, QChar('0')).right(3));
    return result;
}

quint32 AsterixTest::rnd()
{
    quint32 result = qrand() ^ qrand();
    result <<= 16;
    result |= qrand() ^ qrand();
    return result;
}

qint32 AsterixTest::rndS(int i)
{
    if (i >= 31)
        return rnd();

    quint32 mask = (0x01 << (i - 1)) - 1;
    qint32 result = rnd() & mask;
    return result * (rnd() % 2 ? -1 : 1);
}

quint32 AsterixTest::rndU(int i)
{
    if (i >= 32)
        return rnd();

    quint32 mask = (0x01 << i) - 1;
    return rnd() & mask;
}

QByteArray AsterixTest::rndArray(int size, int bit)
{
    QByteArray result;
    for (int i = 0; i < size; ++i)
        result.push_back(rndU(bit));
    return result;
}

QTime AsterixTest::rndTime()
{
    return QTime().addMSecs(rnd());
}

static inline bool myFuzzyCompare(double p1, double p2)
{
    if (p1 == p2)
        return true;
    const static double capacity = 10;
    double dif = log10(qAbs(p1 - p2));
    double d1 = qAbs(log10(qAbs(p1)) - dif);
    double d2 = qAbs(log10(qAbs(p2)) - dif);
    bool b1 = d1 > capacity;
    bool b2 = d2 > capacity;
//    if (!b1 || !b2)
//        __asm("int $0x03");
    return b1 && b2;
}

template <class T>
static bool operator== (const T &a, const T &b)
{
    unsigned int size = sizeof(a);
    return size == sizeof(b) && 0 == memcmp(&a, &b, size);
}

static bool operator== (const Asterix021::I021_110_2 &a, const Asterix021::I021_110_2 &b)
{
    bool result = a.TCA == b.TCA;
    result = result && a.NC == b.NC;
    result = result && a.TCPnumber == b.TCPnumber;
    result = result && myFuzzyCompare(a.altitude, b.altitude);
    result = result && myFuzzyCompare(a.latitude, b.latitude);
    result = result && myFuzzyCompare(a.longitude, b.longitude);
    result = result && a.pointType == b.pointType;
    result = result && a.TD == b.TD;
    result = result && a.TRA == b.TRA;
    result = result && a.TOA == b.TOA;
    result = result && myFuzzyCompare(a.TOV, b.TOV);
    result = result && myFuzzyCompare(a.TTR, b.TTR);
    return result;
}

//template <>
//bool operator== (const Asterix048::I048_250 &a, const Asterix048::I048_250 &b)
//{
//    return true;
//}

template<class T>
static inline bool compare(const QVariant &a, const QVariant &b)
{
    return a.type() == b.type() && a.canConvert<T>() && b.canConvert<T>() && a.value<T>() == b.value<T>();
}

static inline bool compare(const QVariant &a, const QVariant &b)
{
    return compare<Asterix001::I001_020_FX>(a, b)
            || compare<Asterix048::I048_120_2>(a, b)
            || compare<Asterix048::I048_170_FX>(a, b)
            || compare<Asterix048::I048_250>(a, b)
            || compare<Asterix048::I048_020_FX>(a, b)
            || compare<Asterix021::I021_090_FX>(a, b)
            || compare<Asterix021::I021_110_1>(a, b)
            || compare<Asterix021::I021_110_2>(a, b)
            || compare<Asterix021::I048_250>(a, b)
            || compare<Asterix063::I063_060_FX>(a, b)
            || compare<Asterix002::I002_070>(a, b)
            || compare<Asterix034::I034_070>(a, b)
            ;
}

namespace QTest
{
    template <>
            inline bool qCompare(AsterixData const &t1, AsterixData const &t2,
                                 const char *actual, const char *expected, const char *file, int line)
    {
        bool result = true;

        QMap<QString, QVariant> ad1 = t1.data();
        QMap<QString, QVariant> ad2 = t2.data();

        QList<QString> keyList = ad1.keys();
        foreach (const QString &key, keyList)
        {
            if (ad2.contains(key))
            {
                QVariant val1 = ad1[key];
                QVariant val2 = ad2[key];
                ad1.remove(key);
                ad2.remove(key);
                if (val1 != val2)
                {
                    switch (val2.type())
                    {
                    case QVariant::Time:
                        if (qAbs(val1.toTime().msecsTo(val2.toTime())) > 1000)
                            result = result && compare_helper(false, qPrintable(key), toString(val1), toString(val2), actual, expected, file, line);
                        break;
                    case QVariant::ByteArray:
                        result = result && compare_helper(false, qPrintable(key), toString(val1), toString(val2), actual, expected, file, line);
                        break;
                    case QVariant::Double:
                        if (!myFuzzyCompare(val1.toDouble(), val2.toDouble()))
                            result = result && compare_helper(false, qPrintable(key), toString(val1), toString(val2), actual, expected, file, line);
                        break;
                    case QVariant::List:
                        {
                            QVariantList l1 = val1.toList();
                            QVariantList l2 = val2.toList();
                            if (l1.size() != l2.size())
                            {
                                result = result && compare_helper(false, qPrintable(key), toString(val1), toString(val2), actual, expected, file, line);
                                break;
                            }
                            for (int i = 0; i < l1.size(); ++i)
                            {
                                if (compare(l1[i], l2[i]))
                                    continue;
                                result = result && compare_helper(false, qPrintable(key), toString(val1), toString(val2), actual, expected, file, line);
                                break;
                            }
                        }
                        break;
                    default:
                        if (!compare(val1, val2))
                            result = result && compare_helper(false, qPrintable(key), toString(val1), toString(val2), actual, expected, file, line);
                        break;
                    }
                }
            }
        }

        if (!ad1.isEmpty())
        {
            result = false;
            QMap<QString, QVariant>::iterator it = ad1.begin();
            while (it != ad1.end())
            {
                qFail(qPrintable(QString("Stayed for %1: %2 %3").arg(actual).arg(it.key()).arg(toString(it.value()))), file, line);
                ++it;
            }
        }

        if (!ad2.isEmpty())
        {
            result = false;
            QMap<QString, QVariant>::iterator it = ad2.begin();
            while (it != ad2.end()) {
                qFail(qPrintable(QString("Stayed for %1: %2 %3").arg(expected).arg(it.key()).arg(toString(it.value()))), file, line);
                ++it;
            }
        }

        return result;
    }

}

void AsterixTest::testAsterix(Asterix *asterix)
{
    QStringList fields = asterix->fields();
    QVERIFY2(0 == fields.removeDuplicates(), qPrintable(QString("Asterix%1: There are duplicates in fields!").arg(asterix->category(), 3, 10, QLatin1Char('0'))));
    QString prefix = QString("a%1.").arg(asterix->category(), 3, 10, QLatin1Char('0'));
    foreach (const QString &str, fields)
        QVERIFY2(str.startsWith(prefix) || str == Asterix::SAC || str == Asterix::SIC, qPrintable(QString("Invalid prefix for %1. (Should be %2)").arg(str).arg(prefix)));
}

void AsterixTest::testAsterixFieldColision()
{
    QList<Asterix *> parsers = asterixFactory.createAll();
    foreach (Asterix *asterix, parsers)
    {
        testAsterix(asterix);
        delete asterix;
    }
}

void AsterixTest::testAsterix001_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix001::category;

        aDataSource.insert(Asterix001::DataSourceIdentifier_SAC, rndU(8));
        aDataSource.insert(Asterix001::DataSourceIdentifier_SIC, rndU(8));

        aDataSource.insert(Asterix001::TargetReportDescriptor_TYP, rndU(1));
        aDataSource.insert(Asterix001::TargetReportDescriptor_SIM, rndU(1));
        aDataSource.insert(Asterix001::TargetReportDescriptor_SSR, rndU(1));
        aDataSource.insert(Asterix001::TargetReportDescriptor_PSR, rndU(1));
        aDataSource.insert(Asterix001::TargetReportDescriptor_ANT, rndU(1));
        aDataSource.insert(Asterix001::TargetReportDescriptor_SPI, rndU(1));
        aDataSource.insert(Asterix001::TargetReportDescriptor_RAB, rndU(1));

        if (testChance())
        {
            QVariantList data;
            int amount = qrand() % 100 + 1;
            for (int i = 0; i < amount; ++i)
            {
                Asterix001::I001_020_FX ext = {0};
                ext.TST = rndU(1);
                ext.DS1DS2 = rndU(2);
                ext.ME = rndU(1);
                ext.MI = rndU(1);
                data.push_back(QVariant::fromValue(ext));
            }
            aDataSource.insert(Asterix001::TargetReportDescriptor_FX, data);
        }

        if (aDataSource.value(Asterix001::TargetReportDescriptor_TYP).toBool()) // track
        {
            aDataSource.insert(Asterix001::TrackPlotNumber, rndU(16));

            if (testChance())
            {
                aDataSource.insert(Asterix001::MeasuredPositionInPolarCoordinates_RHO, rndS(16) / 128.);
                aDataSource.insert(Asterix001::MeasuredPositionInPolarCoordinates_THETA, rndS(16) * 360. / 65536 );
            }

            if (testChance())
            {
                aDataSource.insert(Asterix001::CalculatedPositionInCartesianCoordinates_X, rndS(16) / 64.);
                aDataSource.insert(Asterix001::CalculatedPositionInCartesianCoordinates_Y, rndS(16) / 64.);
            }

            if (testChance())
            {
                aDataSource.insert(Asterix001::CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED, rndU(16) / 16384.);
                aDataSource.insert(Asterix001::CalculatedTrackVelocityInPolarCoordinates_HEADING, rndU(16) * 360. / 65536.);
            }

            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_V, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_G, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_L, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_A4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_A2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_A1, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_B4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_B2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_B1, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_C4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_C2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_C1, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_D4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_D2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_D1, rndU(1));

            aDataSource.insert(Asterix001::ModeCCodeInBinaryRepresentation_V, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeInBinaryRepresentation_G, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeInBinaryRepresentation_HEIGHT, rndS(14) * 25);

            aDataSource.insert(Asterix001::TrackStatus_CON, rndU(1));
            aDataSource.insert(Asterix001::TrackStatus_RAD, rndU(1));
            aDataSource.insert(Asterix001::TrackStatus_MAN, rndU(1));
            aDataSource.insert(Asterix001::TrackStatus_DOU, rndU(1));
            aDataSource.insert(Asterix001::TrackStatus_RDPC, rndU(1));
            aDataSource.insert(Asterix001::TrackStatus_GHO, rndU(1));

            if (testChance())
                aDataSource.insert(Asterix001::TrackStatus_TRE, rndU(1));

            if (testChance())
            {
                QVariantList data;
                int amount = qrand() % 100 + 1;
                for (int i = 0; i < amount; ++i)
                    data.push_back(rndU(7));
                aDataSource.insert(Asterix001::TrackQuality, data);
            }
        }
        else
        {
            //plot
            bool isSSR = aDataSource.value(Asterix001::TargetReportDescriptor_SSR).toBool();

            aDataSource.insert(Asterix001::MeasuredPositionInPolarCoordinates_RHO, rndS(16) / 128.);
            aDataSource.insert(Asterix001::MeasuredPositionInPolarCoordinates_THETA, rndS(16) * 360. / 65536 );

            if (isSSR || testChance())
            {
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_V, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_G, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_L, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_A4, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_A2, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_A1, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_B4, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_B2, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_B1, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_C4, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_C2, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_C1, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_D4, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_D2, rndU(1));
                aDataSource.insert(Asterix001::Mode3ACodeInOctalRepresentation_D1, rndU(1));
            }

            if (isSSR || testChance())
            {
                aDataSource.insert(Asterix001::ModeCCodeInBinaryRepresentation_V, rndU(1));
                aDataSource.insert(Asterix001::ModeCCodeInBinaryRepresentation_G, rndU(1));
                aDataSource.insert(Asterix001::ModeCCodeInBinaryRepresentation_HEIGHT, rndS(14) * 25);
            }

        } // end of track-plot separation

        if (testChance())
        {
            QVariantList data;
            int amount = qrand() % 100 + 1;
            for (int i = 0; i < amount; ++i)
                data.push_back(rndU(7));
            aDataSource.insert(Asterix001::RadarPlotCharacteristics, data);
        }

        if (testChance())
            aDataSource.insert(Asterix001::TruncatedTimeOfDay, rndU(16) / 128.);

        if (testChance())
        {
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_V, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_G, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_L, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_A4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_A2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_A1, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_B4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_B2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_B1, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_C4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_C2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_C1, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_D4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_D2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeInOctalRepresentation_D1, rndU(1));
        }

        if (testChance())
            aDataSource.insert(Asterix001::MeasuredRadialDopplerSpeed, rndS(8) / 16384.);

        if (testChance())
            aDataSource.insert(Asterix001::ReceivedPower, rndS(8));

        if (testChance())
        {
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QB4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QB1, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QC4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QC2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QC1, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QD4, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QD2, rndU(1));
            aDataSource.insert(Asterix001::Mode3ACodeConfidenceIndicator_QD1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_V, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_G, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_C1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_A1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_C2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_A2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_C4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_A4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_B1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_D1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_B2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_D2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_B4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_D4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QC1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QC2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QC4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QB1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QD1, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QD2, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QB4, rndU(1));
            aDataSource.insert(Asterix001::ModeCCodeAndCodeConfidenceIndicator_QD4, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QB4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QB1, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QC4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QC2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QC1, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QD4, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QD2, rndU(1));
            aDataSource.insert(Asterix001::Mode2CodeConfidenceIndicator_QD1, rndU(1));
        }

        if (testChance())
        {
            QVariantList data;
            int amount = qrand() % 100 + 1;
            for (int i = 0; i < amount; ++i)
                data.push_back(rndU(7));
            aDataSource.insert(Asterix001::WarningErrorConditions, data);
        }

        if (testChance())
        {
            aDataSource.insert(Asterix001::PresenceOfXPulse_XA, rndU(1));
            aDataSource.insert(Asterix001::PresenceOfXPulse_XC, rndU(1));
            aDataSource.insert(Asterix001::PresenceOfXPulse_X2, rndU(1));
        }

        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data))
        {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;
    }
}

void AsterixTest::testAsterix001()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}

void AsterixTest::testAsterix002_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix002::category;

        aDataSource.insert(Asterix002::MessageType, rndU(8));

        aDataSource.insert(Asterix002::DataSourceIdentifier_SAC, rndU(8));
        aDataSource.insert(Asterix002::DataSourceIdentifier_SIC, rndU(8));

        aDataSource.insert(Asterix002::SectorAzimuth, rndU(8) * 360. / 256.);

        if (testChance())
            aDataSource.insert(Asterix002::TimeOfDay, rndTime());

        if (testChance())
            aDataSource.insert(Asterix002::AntennaRotationSpeed, rndU(16) / 128.);

        if (testChance())
        {
            QVariantList data;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
                data.push_back(rndU(7));
            aDataSource.insert(Asterix002::StationConfigurationStatus, data);
        }

        if (testChance())
        {
            QVariantList data;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
                data.push_back(rndU(7));
            aDataSource.insert(Asterix002::StationProcessingMode, data);
        }

        if (testChance())
        {
            QVariantList data;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
            {
                Asterix002::I002_070 val = {0};
                val.A = rndU(1);
                val.IDENT = rndU(5);
                val.COUNTER = rndU(10);
                data.push_back(QVariant::fromValue(val));
            }
            aDataSource.insert(Asterix002::PlotCountValues, data);
        }

        if (testChance())
        {
            QVariantList data;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
                data.push_back(rndU(7));
            aDataSource.insert(Asterix002::WarningErrorConditions, data);
        }

        if (testChance())
        {
            aDataSource.insert(Asterix002::Collimation_ErrorRangeError, rndS(8) / 128.);
            aDataSource.insert(Asterix002::Collimation_ErrorAzimuthError, rndS(8) * 360. / 65536.);
        }

        if (testChance() || Asterix002::ActivationBlindZoneFiltering == Asterix002::EMessageType(aDataSource.value(Asterix002::MessageType).toUInt()))
        {
            aDataSource.insert(Asterix002::DynamicWindow_RhoStart, rndU(16) / 128.);
            aDataSource.insert(Asterix002::DynamicWindow_RhoEnd, rndU(16) / 128.);
            aDataSource.insert(Asterix002::DynamicWindow_ThetaStart, rndU(16) * 360. / 65536.);
            aDataSource.insert(Asterix002::DynamicWindow_ThetaEnd, rndU(16) * 360. / 65536.);
        }

        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data)) {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;
    }
}

void AsterixTest::testAsterix002()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}

void AsterixTest::testAsterix008()
{
    asterix008UnitTest();
}

void AsterixTest::testAsterix021_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix021::category;

        if (testChance())
        {
            // Data Item I021/008, Aircraft Operational Status
            aDataSource.insert(Asterix021::AircraftOperationalStatus_RA, rndU(1));
            aDataSource.insert(Asterix021::AircraftOperationalStatus_TC, rndU(2));
            aDataSource.insert(Asterix021::AircraftOperationalStatus_TS, rndU(1));
            aDataSource.insert(Asterix021::AircraftOperationalStatus_ARV, rndU(1));
            aDataSource.insert(Asterix021::AircraftOperationalStatus_CDTIA, rndU(1));
            aDataSource.insert(Asterix021::AircraftOperationalStatus_NotTCAS, rndU(1));
        }

        if (true)
        {
            // Data Item I021/010, Data Source Identification
            aDataSource.insert(Asterix021::DataSourceIdentification_SAC, rndU(8));
            aDataSource.insert(Asterix021::DataSourceIdentification_SIC, rndU(8));
        }

        if (testChance())
        {
            // Data Item I021/015, Service Identification
            aDataSource.insert(Asterix021::ServiceIdentification, rndU(8));
        }

        if (testChance())
        {
            // Data Item I021/016, Service Management
            aDataSource.insert(Asterix021::ServiceManagement, rndU(8) * 0.5);
        }

        if (testChance())
        {
            // Data Item I021/020, Emitter Category
            aDataSource.insert(Asterix021::EmitterCategory, rndU(8));
        }

        if (true)
        {
            // Data Item I021/040, Target Report Descriptor
            aDataSource.insert(Asterix021::TargetReportDescriptor_ATP, rndU(3));
            aDataSource.insert(Asterix021::TargetReportDescriptor_ARC, rndU(2));
            aDataSource.insert(Asterix021::TargetReportDescriptor_RC, rndU(1));
            aDataSource.insert(Asterix021::TargetReportDescriptor_RAB, rndU(1));

            if (testChance())
            {
                aDataSource.insert(Asterix021::TargetReportDescriptor_DCR, rndU(1));
                aDataSource.insert(Asterix021::TargetReportDescriptor_GBS, rndU(1));
                aDataSource.insert(Asterix021::TargetReportDescriptor_SIM, rndU(1));
                aDataSource.insert(Asterix021::TargetReportDescriptor_TST, rndU(1));
                aDataSource.insert(Asterix021::TargetReportDescriptor_SAA, rndU(1));
                aDataSource.insert(Asterix021::TargetReportDescriptor_CL, rndU(2));

                if (testChance())
                {
                    aDataSource.insert(Asterix021::TargetReportDescriptor_EC7, rndU(1));
                    aDataSource.insert(Asterix021::TargetReportDescriptor_EC6, rndU(1));
                    aDataSource.insert(Asterix021::TargetReportDescriptor_IPC, rndU(1));
                    aDataSource.insert(Asterix021::TargetReportDescriptor_NOGO, rndU(1));
                    aDataSource.insert(Asterix021::TargetReportDescriptor_CPR, rndU(1));
                    aDataSource.insert(Asterix021::TargetReportDescriptor_LDPJ, rndU(1));
                    aDataSource.insert(Asterix021::TargetReportDescriptor_RCF, rndU(1));
                }
            }
        }

        if (testChance())
        {
            // Data Item I021/070, Mode 3/A Code in Octal Representation
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_A4, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_A2, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_A1, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_B4, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_B2, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_B1, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_C4, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_C2, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_C1, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_D4, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_D2, rndU(1));
            aDataSource.insert(Asterix021::Mode3ACodeInOctalRepresentation_D1, rndU(1));
        }

        if (testChance())
        {
            // Data Item I021/071, Time of Applicability for Position
            aDataSource.insert(Asterix021::TimeOfApplicabilityForPosition, rndTime());
        }

        if (testChance())
        {
            // Data Item I021/072, Time of Applicability for Velocity
            aDataSource.insert(Asterix021::TimeOfApplicabilityForVelocity, rndTime());
        }

        if (testChance())
        {
            // Data Item I021/073, Time of Message Reception for Position
            aDataSource.insert(Asterix021::TimeOfMessageReceptionForPosition, rndTime());
        }

        if (testChance())
        {
            // Data Item I021/074, Time of Message Reception of Position–High Precision
            aDataSource.insert(Asterix021::TimeOfMessageReceptionOfPositionHighPrecision, rndU(30) / 1073741824.);
            aDataSource.insert(Asterix021::TimeOfMessageReceptionOfPositionHighPrecision_FSI, rndU(2));
        }

        if (testChance())
        {
            // Data Item I021/075, Time of Message Reception for Velocity
            aDataSource.insert(Asterix021::TimeOfMessageReceptionForVelocity, rndTime());
        }

        if (testChance())
        {
            // Data Item I021/076, Time of Message Reception of Velocity–High Precision
            aDataSource.insert(Asterix021::TimeOfMessageReceptionOfVelocityHighPrecision, rndU(30) / 1073741824.);
            aDataSource.insert(Asterix021::TimeOfMessageReceptionOfVelocityHighPrecision_FSI, rndU(2));
        }

        if (testChance())
        {
            // Data Item I021/077, Time of ASTERIX Report Transmission
            aDataSource.insert(Asterix021::TimeOfAsterixReportTransmission, rndTime());
        }

        if (true)
        {
            // Data Item I021/080, Target Address
            aDataSource.insert(Asterix021::TargetAddress, rndU(24));
        }

        if (true)
        {
            // Data Item I021/090, Quality Indicators
            aDataSource.insert(Asterix021::QualityIndicators_NUCrNACv, rndU(3));
            aDataSource.insert(Asterix021::QualityIndicators_NUCpNIC, rndU(4));
            if (testChance())
            {
                QVariantList ext;
                for (int amount = rnd() % 100 + 1; amount != 0; --amount)
                {
                    Asterix021::I021_090_FX val = {0};
                    val.NACp = rndU(4);
                    val.NICbaro = rndU(1);
                    val.SIL = rndU(2);
                    ext.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix021::QualityIndicators_FX, ext);
            }
        }

        if (testChance())
        {
            // Data Item I021/110, Trajectory Intent
            if (testChance())
            {
                QVariantList ext;
                for (int amount = rnd() % 100 + 1; amount != 0; --amount)
                {
                    Asterix021::I021_110_1 val = {0};
                    val.NAV = rndU(1);
                    val.NVB = rndU(1);
                    ext.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix021::TrajectoryIntent_TIS, ext);
            }

            if (testChance())
            {
                QVariantList ext;
                for (int amount = rnd() % 100 + 1; amount != 0; --amount)
                {
                    Asterix021::I021_110_2 val = {0};
                    val.TCA = rndU(1);
                    val.NC = rndU(1);
                    val.TCPnumber = rndU(6);
                    val.altitude = rndS(16) * 10.;
                    val.latitude = rndS(24) * 180. / 8388608.;
                    val.longitude = rndS(24) * 180. / 8388608.;
                    val.pointType = rndU(4);
                    val.TD = rndU(2);
                    val.TRA = rndU(2);
                    val.TOA = rndU(1);
                    val.TOV = rndU(24);
                    val.TTR = rndU(16) / 100.;
                    ext.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix021::TrajectoryIntent_TID, ext);
            }
        }

        if (testChance())
        {
            // Data Item I021/130, Position in WGS-84 Co-ordinates
            aDataSource.insert(Asterix021::PositionInWGS84Coordinates_Latitude, rndS(24) * 180. / 8388608.);
            aDataSource.insert(Asterix021::PositionInWGS84Coordinates_Longitude, rndS(24) * 180. / 8388608.);
        }

        if (testChance())
        {
            // Data Item I021/131, High-Resolution Position in WGS-84 Co-ordinates
            aDataSource.insert(Asterix021::HighResolutionPositionInWGS84Coordinates_Latitude, rndS(32) * 180. / 1073741824.);
            aDataSource.insert(Asterix021::HighResolutionPositionInWGS84Coordinates_Longitude, rndS(32) * 180. / 1073741824.);
        }

        if (testChance())
        {
            // Data Item I021/132, Message Amplitude
            aDataSource.insert(Asterix021::MessageAmplitude, rndS(8));
        }

        if (testChance())
        {
            // Data Item I021/140, Geometric Height
            aDataSource.insert(Asterix021::GeometricHeight, rndS(16) * 6.25);
        }

        if (testChance())
        {
            // Data Item I021/145, Flight Level
            aDataSource.insert(Asterix021::FlightLevel, rndS(16) / 4.);
        }

        if (testChance())
        {
            // Data Item I021/146, Intermediate State Selected Altitude
            aDataSource.insert(Asterix021::IntermediateStateSelectedAltitude_SAS, rndU(1));
            aDataSource.insert(Asterix021::IntermediateStateSelectedAltitude_Source, rndU(2));
            aDataSource.insert(Asterix021::IntermediateStateSelectedAltitude_Altitude, rndS(13) * 25.);
        }

        if (testChance())
        {
            // Data Item I021/148, Final State Selected Altitude
            aDataSource.insert(Asterix021::FinalStateSelectedAltitude_MV, rndU(1));
            aDataSource.insert(Asterix021::FinalStateSelectedAltitude_AH, rndU(1));
            aDataSource.insert(Asterix021::FinalStateSelectedAltitude_AM, rndU(1));
            aDataSource.insert(Asterix021::FinalStateSelectedAltitude_Altitude, rndS(13) * 25.);
        }

        if (testChance())
        {
            // Data Item I021/150, Air Speed
            bool isMach = rndU(1);
            aDataSource.insert(Asterix021::AirSpeed_IM, isMach);
            aDataSource.insert(Asterix021::AirSpeed, rndU(15) / (isMach ? 1000. : 16384.));
        }

        if (testChance())
        {
            // Data Item I021/151 True Airspeed
            aDataSource.insert(Asterix021::TrueAirspeed_RE, rndU(1));
            aDataSource.insert(Asterix021::TrueAirspeed, rndU(15));
        }

        if (testChance())
        {
            // Data Item I021/152, Magnetic Heading
            aDataSource.insert(Asterix021::MagneticHeading, rndU(16) * 360. / 65536.);
        }

        if (testChance())
        {
            // Data Item I021/155, Barometric Vertical Rate
            aDataSource.insert(Asterix021::BarometricVerticalRate_RE, rndU(1));
            aDataSource.insert(Asterix021::BarometricVerticalRate, rndS(15) * 6.25);
        }

        if (testChance())
        {
            // Data Item I021/157, Geometric Vertical Rate
            aDataSource.insert(Asterix021::GeometricVerticalRate_RE, rndU(1));
            aDataSource.insert(Asterix021::GeometricVerticalRate, rndS(15) * 6.25);
        }

        if (testChance())
        {
            // Data Item I021/160, Ground Vector
            aDataSource.insert(Asterix021::GroundVector_RE, rndU(1));
            aDataSource.insert(Asterix021::GroundVector_Speed, rndU(15) / 16384.);
            aDataSource.insert(Asterix021::GroundVector_Angle, rndU(16) * 360. / 65536.);
        }

        if (testChance())
        {
            // Data Item I021/161, Track Number
            aDataSource.insert(Asterix021::TrackNumber, rndU(12));
        }

        if (testChance())
        {
            // Data Item I021/165, Track Angle Rate
            aDataSource.insert(Asterix021::TrackAngleRate, rndU(10) / 32.);
        }

        if (testChance())
        {
            // Data Item I021/170, Target Identification
            aDataSource.insert(Asterix021::TargetIdentification, rndArray(8, 6));
        }

        if (testChance())
        {
            // Data Item I021/200, Target Status
            aDataSource.insert(Asterix021::TargetStatus_PS, rndU(3));
            aDataSource.insert(Asterix021::TargetStatus_SS, rndU(2));
        }

        if (testChance())
        {
            // Data Item I021/210, MOPS Version
            aDataSource.insert(Asterix021::MopsVersion_VNS, rndU(1));
            aDataSource.insert(Asterix021::MopsVersion_VN, rndU(3));
            aDataSource.insert(Asterix021::MopsVersion_LTT, rndU(3));
        }

        if (testChance())
        {
            // Data Item I021/220, Met Information
            if (testChance())
                aDataSource.insert(Asterix021::MetInformation_WindSpeed, rndU(16));
            if (testChance())
                aDataSource.insert(Asterix021::MetInformation_WindDirection, rndU(16));
            if (testChance())
                aDataSource.insert(Asterix021::MetInformation_Temperature, rndS(16) * 0.25);
            if (testChance())
                aDataSource.insert(Asterix021::MetInformation_Turbulence, rndU(8));
        }

        if (testChance())
        {
            // Data Item I021/230, Roll Angle
            aDataSource.insert(Asterix021::RollAngle, rndS(16) * 0.01);
        }

        if (true)
        {
            // Data Item I021/250, Mode S MB Data
            QVariantList extension;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
            {
                Asterix021::I048_250 val = {{0}};
                for (unsigned int i = 0; i < sizeof(val.MbData); ++i)
                    val.MbData[i] = rndU(8);
                val.BDS1 = rndU(4);
                val.BDS2 = rndU(4);
                extension.push_back(QVariant::fromValue(val));
            }
            aDataSource.insert(Asterix021::ModeSMbData, extension);
        }

        if (testChance())
        {
            // Data Item I021/260, ACAS Resolution Advisory Report
            aDataSource.insert(Asterix021::AcasResolutionAdvisoryReport, rndArray(7));
        }

        if (testChance())
        {
            // Data Item I021/271, Surface Capabilities and Characteristics
            aDataSource.insert(Asterix021::SurfaceCapabilitiesAndCharacteristics_POA, rndU(1));
            aDataSource.insert(Asterix021::SurfaceCapabilitiesAndCharacteristics_CDTIS, rndU(1));
            aDataSource.insert(Asterix021::SurfaceCapabilitiesAndCharacteristics_B2low, rndU(1));
            aDataSource.insert(Asterix021::SurfaceCapabilitiesAndCharacteristics_RAS, rndU(1));
            aDataSource.insert(Asterix021::SurfaceCapabilitiesAndCharacteristics_IDENT, rndU(1));
            if (testChance())
                aDataSource.insert(Asterix021::SurfaceCapabilitiesAndCharacteristics_LengthAndWidth, rndU(4));
        }

        if (testChance())
        {
            // Data Item I021/295, Data Ages
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_AircraftOperationalStatusAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_TargetReportDescriptorAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_Mode3AAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_QualityIndicatorsAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_TrajectoryIntentAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_MessageAmplitudeAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_GeometricHeightAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_FlightLevelAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_IntermediateStateSelectedAltitudeAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_FinalStateSelectedAltitudeAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_AirSpeedAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_TrueAirSpeedAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_MagneticHeadingAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_BarometricVerticalRateAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_GeometricVerticalRateAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_GroundVectorAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_TrackRateAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_TargetIdentificationAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_TargetStatusAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_MetInformationAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_RollAngleAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_AcasResolutionAdvisoryAge, rndU(8) * 0.1);
            if (testChance())
                aDataSource.insert(Asterix021::DataAges_SurfaceCapabilitiesAndCharacteristicsAge, rndU(8) * 0.1);
        }

        if (testChance())
        {
            // Data Item I021/400, Receiver ID
            aDataSource.insert(Asterix021::ReceiverId, rndU(8));
        }

        if (testChance())
            aDataSource.insert(Asterix021::SpecialPurposeField, rndArray(rnd() % 254 + 1));

        if (testChance())
            aDataSource.insert(Asterix021::ReservedExpansionField, rndArray(rnd() % 254 + 1));

        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data)) {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;
    }
}

void AsterixTest::testAsterix021()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}

void AsterixTest::testAsterix062_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix062::category;
        aDataSource.insert(Asterix062::DataSourceIdentifier_SAC, rndU(8));
        aDataSource.insert(Asterix062::DataSourceIdentifier_SIC, rndU(8));
        aDataSource.insert(Asterix062::TimeOfTrackInformation, QTime::currentTime());
        aDataSource.insert(Asterix062::TrackNumber, rndU(16));
        aDataSource.insert(Asterix062::TrackStatus_MON, rndU(1));
        aDataSource.insert(Asterix062::TrackStatus_SPI, rndU(1));
        aDataSource.insert(Asterix062::TrackStatus_MRH, rndU(1));
        aDataSource.insert(Asterix062::TrackStatus_SRC, rndU(3));
        aDataSource.insert(Asterix062::TrackStatus_CNF, rndU(1));

        if (testChance())
            aDataSource.insert(Asterix062::ServiceIdentification, rndU(8));
        if (testChance()) {
            aDataSource.insert(Asterix062::CalculatedTrackPositionWGS_Latitude, rndS(32) * 180. / 33554432.);
            aDataSource.insert(Asterix062::CalculatedTrackPositionWGS_Longitude, rndS(32) * 180. / 33554432.);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::CalculatedTrackPositionCartesian_x, rndU(24) / 2);
            aDataSource.insert(Asterix062::CalculatedTrackPositionCartesian_y, rndU(24) / 2);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::CalculatedTrackVelocityCartesian_Vx, rndU(16) / 4);
            aDataSource.insert(Asterix062::CalculatedTrackVelocityCartesian_Vy, rndU(16) / 4);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::CalculatedAccelerationCartesian_Ax, rndS(8) / 4);
            aDataSource.insert(Asterix062::CalculatedAccelerationCartesian_Ay, rndS(8) / 4);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::TrackMode3ACode_hasChanged, rndU(1));
            aDataSource.insert(Asterix062::TrackMode3ACode_reply, rndU(12));
        }
        if (testChance()){
            aDataSource.insert(Asterix062::TargetIdentification_STI, rndU(2));
            aDataSource.insert(Asterix062::TargetIdentification_Character1, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character2, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character3, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character4, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character5, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character6, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character7, rndU(6));
            aDataSource.insert(Asterix062::TargetIdentification_Character8, rndU(6));
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_ADR, rndU(24));
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character1, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character2, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character3, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character4, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character5, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character6, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character7, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_ID_Character8, rndU(6));
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_MHG, rndU(16) * 360. / 65536.);
        if (testChance())
        {
            bool b = rndU(1);
            aDataSource.insert(Asterix062::AircraftDerivedData_IAS_IM, b);
            aDataSource.insert(Asterix062::AircraftDerivedData_IAS, rndU(15) / (b ? 1000. : 16384.));
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_TAS, rndU(16));
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_SAL_SAS, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_SAL_Source, rndU(2));
            aDataSource.insert(Asterix062::AircraftDerivedData_SAL_Altitude, rndS(13) * 25);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_FSS_MV, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_FSS_AH, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_FSS_AM, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_FSS_Altitude, rndS(13) * 25);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_TIS_NAV, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_TIS_NVB, rndU(1));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP, rndS(8));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_TCA, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_NC, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_TCP, rndU(6));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_Altitude, rndU(16) * 10.);
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_Latitude, rndS(24) * 180. / 8388608.);
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_Longitude, rndS(24) * 180. / 8388608.);
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_PointType, rndU(4));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_TD, rndU(2));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_TRA, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_TOA, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_TOV, rndU(24));
            aDataSource.insert(Asterix062::AircraftDerivedData_TID_REP_TTR, rndU(16) / 100.);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_COM, rndU(3));
            aDataSource.insert(Asterix062::AircraftDerivedData_COM_STAT, rndU(3));
            aDataSource.insert(Asterix062::AircraftDerivedData_COM_SSC, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_COM_ARC, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_COM_AIC, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_COM_B1A, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_COM_B1B, rndU(4));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_SAB_AC, rndU(2));
            aDataSource.insert(Asterix062::AircraftDerivedData_SAB_MN, rndU(2));
            aDataSource.insert(Asterix062::AircraftDerivedData_SAB_DC, rndU(2));
            aDataSource.insert(Asterix062::AircraftDerivedData_SAB_GBS, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_SAB_STAT, rndU(3));
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_ACS, rndArray(7));
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_BVR, rndU(16) * 6.25);
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_GVR, rndU(16) * 6.25);
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_RAN, rndU(16) / 100);
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_TAR_TI, rndU(2));
            aDataSource.insert(Asterix062::AircraftDerivedData_TAR_RateOfTurn, rndS(7) / 4.);
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_TAN, rndU(16) * 360. / 65536.);
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_GSP, rndU(16) / 16384);
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_VUN, rndS(8));

        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_WS, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_WD, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_TMP, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_TRB, rndU(1));
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_WindSpeed, rndU(16));
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_WindDirection, rndU(16));
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_Temperature, rndS(16) / 4);
            aDataSource.insert(Asterix062::AircraftDerivedData_MET_Turbulence, rndU(8));
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_EMC, rndU(8));
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_POS_Latitude, rndS(24) * 180. / 8388608.);
            aDataSource.insert(Asterix062::AircraftDerivedData_POS_Longitude, rndS(24) * 180. / 8388608.);
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_GAL, rndU(16) * 6.25);
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_PUN, rndU(4));
        if (testChance()) {
            aDataSource.insert(Asterix062::AircraftDerivedData_MB_REP, rndS(8));
            aDataSource.insert(Asterix062::AircraftDerivedData_MB_data, rndArray(7));
            aDataSource.insert(Asterix062::AircraftDerivedData_MB_BDS1, rndU(4));
            aDataSource.insert(Asterix062::AircraftDerivedData_MB_BDS2, rndU(4));
        }
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_IAR, rndU(16));
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_MAC, rndU(16) * 0.008);
        if (testChance())
            aDataSource.insert(Asterix062::AircraftDerivedData_BPS, rndU(12) / 10);

        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_TRK, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_PSR, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_SSR, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_MDS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_ADS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_ES, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_VDL, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_UAT, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_LOP, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::SystemTrackUpdateAges_MLT, rndU(8) / 4.);
        if (testChance()) {
            aDataSource.insert(Asterix062::ModeOfMovement_TRANS, rndU(2));
            aDataSource.insert(Asterix062::ModeOfMovement_LONG, rndU(2));
            aDataSource.insert(Asterix062::ModeOfMovement_VERT, rndU(2));
            aDataSource.insert(Asterix062::ModeOfMovement_ADF, rndU(1));
        }

        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_FML, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MD1, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MD2, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MDA, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MD4, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MD5, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MHG, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_IAS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_TAS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_SAL, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_FSS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_TID, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_COM, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_SAB, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_ACS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_BVR, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_GVR, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_RAN, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_TAR, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_TAN, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_GSP, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_VUN, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MET, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_EMC, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_POS, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_GAL, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_PUN, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MB, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_IAR, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_MAC, rndU(8) / 4.);
        if (testChance())
            aDataSource.insert(Asterix062::TrackDataAges_BPS, rndU(8) / 4.);

        if (testChance())
            aDataSource.insert(Asterix062::MeasuredFlightLevel, rndU(16) / 4);
        if (testChance())
            aDataSource.insert(Asterix062::CalculatedTrackGeometricAltitude, rndU(16) * 6.25);
        if (testChance()) {
            aDataSource.insert(Asterix062::CalculatedTrackBarometricAltitude_QNH, rndU(1));
            aDataSource.insert(Asterix062::CalculatedTrackBarometricAltitude, rndS(15) / 4.);
        }
        if (testChance())
            aDataSource.insert(Asterix062::CalculatedRateOfClimbDescent, rndU(16) * 6.25);

        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TAG_SAC, rndU(8));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TAG_SIC, rndU(8));
        }
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_CSN, rndArray(7));
        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_IFI_TYP, rndU(2));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_IFI_NBR, rndU(27));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_FCT_GAT_OAT, rndU(2));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_FCT_FR1_FR2, rndU(2));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_FCT_RVSM, rndU(2));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_FCT_HPR, rndU(1));
        }
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TAC, rndArray(4));
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_WTC, rndU(8));
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_DEP, rndArray(4));
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_DST, rndArray(4));
        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_RDS_NU1, rndU(8));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_RDS_NU2, rndU(8));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_RDS_LTR, rndU(8));
        }
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_CFL, rndU(16) / 4);
        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_CTL_Centre, rndU(8));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_CTL_Position, rndU(8));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TOD_REP, rndS(8));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TOD_TYP, rndU(5));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TOD_DAY, rndU(2));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TOD_HOR, rndU(5));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TOD_MIN, rndU(6));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_TOD_SEC, rndU(6));
        }
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_AST, rndArray(6));
        if (testChance()) {
            aDataSource.insert(Asterix062::FlightPlanRelatedData_STS_EMP, rndU(2));
            aDataSource.insert(Asterix062::FlightPlanRelatedData_STS_AVL, rndU(2));
        }
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_STD, rndArray(7));
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_STA, rndArray(7));
        if (testChance()) {
            bool bVA = rndU(1);
            aDataSource.insert(Asterix062::FlightPlanRelatedData_PEM_VA, bVA);
            if (bVA)
                aDataSource.insert(Asterix062::FlightPlanRelatedData_PEM_reply, rndU(12));
        }
        if (testChance())
            aDataSource.insert(Asterix062::FlightPlanRelatedData_PEC, rndArray(7));

        if (testChance()) {
            aDataSource.insert(Asterix062::TargetSizeAndOrientation_Length, rndU(7));
            if (testChance()) {
                aDataSource.insert(Asterix062::TargetSizeAndOrientation_Orientation, rndU(7) / 128 * 360);
                if (testChance())
                    aDataSource.insert(Asterix062::TargetSizeAndOrientation_Width, rndU(7));
            }
        }
        if (testChance())
            aDataSource.insert(Asterix062::VehicleFleetIdentification, rndU(8));
        if (testChance()) {
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_M5, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_ID, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_DA, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_M1, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_M2, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_M3, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_MC, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_SUM_X, rndU(1));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_PMN_PIN, rndU(14));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_PMN_NAT, rndU(6));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_PMN_MIS, rndU(6));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_POS_Latitude, rndS(24) * 180. / 8388608.);
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_POS_Longitude, rndS(24) * 180. / 8388608.);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_GA_RES, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_GA, rndS(14) * 25);
        }
        if (testChance())
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_EM1, rndU(12));
        if (testChance())
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_TOS, rndU(8) / 128.);
        if (testChance()) {
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_XP_X5, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_XP_XC, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_XP_X3, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_XP_X2, rndU(1));
            aDataSource.insert(Asterix062::Mode5DataReportsAndExtendedMode1Code_XP_X1, rndU(1));
        }
        if (testChance())
            aDataSource.insert(Asterix062::TrackMode2Code, rndU(12));
        if (testChance()) {
            aDataSource.insert(Asterix062::ComposedTrackNumber_SystemUnitIdentification.arg(0), rndU(8));
            aDataSource.insert(Asterix062::ComposedTrackNumber_SystemTrackNumber.arg(0), rndU(15));
            if (testChance()) {
                aDataSource.insert(Asterix062::ComposedTrackNumber_SystemUnitIdentification.arg(1), rndU(8));
                aDataSource.insert(Asterix062::ComposedTrackNumber_SystemTrackNumber.arg(1), rndU(15));
                if (testChance()) {
                    aDataSource.insert(Asterix062::ComposedTrackNumber_SystemUnitIdentification.arg(2), rndU(8));
                    aDataSource.insert(Asterix062::ComposedTrackNumber_SystemTrackNumber.arg(2), rndU(15));
                }
            }
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::EstimatedAccuracies_APC_X, rndU(16) / 2);
            aDataSource.insert(Asterix062::EstimatedAccuracies_APC_Y, rndU(16) / 2);
        }
        if (testChance())
            aDataSource.insert(Asterix062::EstimatedAccuracies_COV, rndU(16) / 2);
        if (testChance()) {
            aDataSource.insert(Asterix062::EstimatedAccuracies_APW_Latitude, rndU(16) * 180 / 33554432);
            aDataSource.insert(Asterix062::EstimatedAccuracies_APW_Longitude, rndU(16) * 180 / 33554432);
        }
        if (testChance())
            aDataSource.insert(Asterix062::EstimatedAccuracies_AGA, rndS(8) * 6.25);
        if (testChance())
            aDataSource.insert(Asterix062::EstimatedAccuracies_ABA, rndS(8) / 4);
        if (testChance()) {
            aDataSource.insert(Asterix062::EstimatedAccuracies_ATV_X, rndS(8) / 4);
            aDataSource.insert(Asterix062::EstimatedAccuracies_ATV_Y, rndS(8) / 4);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::EstimatedAccuracies_AA_X, rndS(8) / 4);
            aDataSource.insert(Asterix062::EstimatedAccuracies_AA_Y, rndS(8) / 4);
        }
        if (testChance())
            aDataSource.insert(Asterix062::EstimatedAccuracies_ARC, rndS(8) * 6.25);
        if (testChance()) {
            aDataSource.insert(Asterix062::MeasuredInformation_SID_SAC, rndU(8));
            aDataSource.insert(Asterix062::MeasuredInformation_SID_SIC, rndU(8));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::MeasuredInformation_POS_RHO, rndU(16) / 256.);
            aDataSource.insert(Asterix062::MeasuredInformation_POS_THETA, rndS(16) * 360. / 65536.);
        }
        if (testChance())
            aDataSource.insert(Asterix062::MeasuredInformation_HEI, rndU(16) * 25);
        if (testChance()) {
            aDataSource.insert(Asterix062::MeasuredInformation_MDC_V, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_MDC_G, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_MDC, rndS(14) / 4);
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::MeasuredInformation_MDA_V, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_MDA_G, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_MDA_L, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_MDA, rndU(12));
        }
        if (testChance()) {
            aDataSource.insert(Asterix062::MeasuredInformation_TYP, rndU(3));
            aDataSource.insert(Asterix062::MeasuredInformation_TYP_SIM, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_TYP_RAB, rndU(1));
            aDataSource.insert(Asterix062::MeasuredInformation_TYP_TST, rndU(1));
        }


        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data)) {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;

    }
}

void AsterixTest::testAsterix062()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}

void AsterixTest::testAsterix034_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix034::category;

        int messageType = i % 4 + 1;

        aDataSource.insert(Asterix034::MessageType, messageType);

        aDataSource.insert(Asterix034::DataSourceIdentifier_SAC, rndU(8));
        aDataSource.insert(Asterix034::DataSourceIdentifier_SIC, rndU(8));

        if (messageType == 1)
        {
            aDataSource.insert(Asterix034::TimeOfDay, rndTime()); // 030

            if (testChance())
                aDataSource.insert(Asterix034::AntennaRotationPeriod, rndU(16) / 128.);

            if (testChance()) // 050
            {
                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_NOGO, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_RDPC, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_RDPR, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_OVLRDP, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_OVLXMT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_MSC, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_TSV, rndU(1));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_ANT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_CHAB, rndU(2));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_OVL, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_MSC, rndU(1));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_ANT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_CHAB, rndU(2));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_OVL, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_MSC, rndU(1));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_ANT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_CHAB, rndU(2));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_OVLSUR, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_MSC, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_SCF, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_DLF, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_OVLSCF, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_OVLDLF, rndU(1));
                }
            }

            if (testChance()) // 060
            {
                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationProcessingMode_COM_REDRDP, rndU(3));
                    aDataSource.insert(Asterix034::StationProcessingMode_COM_REDXMT, rndU(3));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationProcessingMode_PSR_POL, rndU(1));
                    aDataSource.insert(Asterix034::StationProcessingMode_PSR_REDRAD, rndU(3));
                    aDataSource.insert(Asterix034::StationProcessingMode_PSR_STC, rndU(2));
                }

                if (testChance())
                    aDataSource.insert(Asterix034::StationProcessingMode_SSR_REDRAD, rndU(3));

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationProcessingMode_MDS_REDRAD, rndU(3));
                    aDataSource.insert(Asterix034::StationProcessingMode_MDS_CLU, rndU(1));
                }
            }

            if (testChance()) // 070
            {
                QVariantList extension;
                for (int amount = rand() % 100 + 1; amount != 0; --amount)
                {
                    Asterix034::I034_070 val = {0};
                    val.TYP = rndU(5);
                    val.COUNTER = rndU(11);
                    extension.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix034::MessageCountValues, QVariant::fromValue(extension));
            }

            if (testChance()) // 120
            {
                aDataSource.insert(Asterix034::PositionOfSource3D_Height, rndU(16));
                aDataSource.insert(Asterix034::PositionOfSource3D_Latitude, rndS(24) * 180. / 8388608.);
                aDataSource.insert(Asterix034::PositionOfSource3D_Longitude, rndS(24) * 180. / 8388608.);
            }

            if (testChance()) // 090
            {
                aDataSource.insert(Asterix034::CollimationError_RangeError, rndS(8) / 128.);
                aDataSource.insert(Asterix034::CollimationError_AzimuthError, rndS(8) * 360. / 16384.);
            }
        }
        else if (messageType == 2)
        {
            aDataSource.insert(Asterix034::TimeOfDay, rndTime()); // 030

            aDataSource.insert(Asterix034::SectorAzimuth, rndU(8) * 360. / 256.); // 020

            if (testChance()) // 050
            {
                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_NOGO, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_RDPC, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_RDPR, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_OVLRDP, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_OVLXMT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_MSC, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_COM_TSV, rndU(1));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_ANT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_CHAB, rndU(2));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_OVL, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_PSR_MSC, rndU(1));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_ANT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_CHAB, rndU(2));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_OVL, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_SSR_MSC, rndU(1));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_ANT, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_CHAB, rndU(2));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_OVLSUR, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_MSC, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_SCF, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_DLF, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_OVLSCF, rndU(1));
                    aDataSource.insert(Asterix034::StationConfigurationStatus_MDS_OVLDLF, rndU(1));
                }
            }

            if (testChance()) // 060
            {
                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationProcessingMode_COM_REDRDP, rndU(3));
                    aDataSource.insert(Asterix034::StationProcessingMode_COM_REDXMT, rndU(3));
                }

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationProcessingMode_PSR_POL, rndU(1));
                    aDataSource.insert(Asterix034::StationProcessingMode_PSR_REDRAD, rndU(3));
                    aDataSource.insert(Asterix034::StationProcessingMode_PSR_STC, rndU(2));
                }

                if (testChance())
                    aDataSource.insert(Asterix034::StationProcessingMode_SSR_REDRAD, rndU(3));

                if (testChance())
                {
                    aDataSource.insert(Asterix034::StationProcessingMode_MDS_REDRAD, rndU(3));
                    aDataSource.insert(Asterix034::StationProcessingMode_MDS_CLU, rndU(1));
                }
            }

            if (testChance()) // 070
            {
                QVariantList extension;
                for (int amount = rand() % 100 + 1; amount != 0; --amount)
                {
                    Asterix034::I034_070 val = {0};
                    val.TYP = rndU(5);
                    val.COUNTER = rndU(11);
                    extension.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix034::MessageCountValues, QVariant::fromValue(extension));
            }

            if (testChance()) // 090
            {
                aDataSource.insert(Asterix034::CollimationError_RangeError, rndS(8) / 128.);
                aDataSource.insert(Asterix034::CollimationError_AzimuthError, rndS(8) * 360. / 16384.);
            }
        }
        else if (messageType == 3)
        {
            if (testChance())
                aDataSource.insert(Asterix034::TimeOfDay, rndTime()); // 030

            if (testChance()) // 100
            {
                aDataSource.insert(Asterix034::GenericPolarWindow_RhoStart, rndU(16) / 256.);
                aDataSource.insert(Asterix034::GenericPolarWindow_RhoEnd, rndU(16) / 256.);
                aDataSource.insert(Asterix034::GenericPolarWindow_ThetaStart, rndU(16) * 360. / 65536.);
                aDataSource.insert(Asterix034::GenericPolarWindow_ThetaEnd, rndU(16) * 360. / 65536.);
            }

            aDataSource.insert(Asterix034::DataFilter, rndU(8)); // 110
        }
        else if (messageType == 4)
        {
            if (testChance())
                aDataSource.insert(Asterix034::TimeOfDay, rndTime()); // 030

            // 100
            aDataSource.insert(Asterix034::GenericPolarWindow_RhoStart, rndU(16) / 256.);
            aDataSource.insert(Asterix034::GenericPolarWindow_RhoEnd, rndU(16) / 256.);
            aDataSource.insert(Asterix034::GenericPolarWindow_ThetaStart, rndU(16) * 360. / 65536.);
            aDataSource.insert(Asterix034::GenericPolarWindow_ThetaEnd, rndU(16) * 360. / 65536.);
        }

        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data))
        {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;
    }
}

void AsterixTest::testAsterix034()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}

void AsterixTest::testAsterix048_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix048::category;

        aDataSource.insert(Asterix048::DataSourceIdentifier_SAC, rndU(8));
        aDataSource.insert(Asterix048::DataSourceIdentifier_SIC, rndU(8));

        aDataSource.insert(Asterix048::TargetReportDescriptor_TYP, rndU(3));
        aDataSource.insert(Asterix048::TargetReportDescriptor_SIM, rndU(1));
        aDataSource.insert(Asterix048::TargetReportDescriptor_RDP, rndU(1));
        aDataSource.insert(Asterix048::TargetReportDescriptor_SPI, rndU(1));
        aDataSource.insert(Asterix048::TargetReportDescriptor_RAB, rndU(1));

        if (testChance())
        {
            QVariantList extension;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
            {
                Asterix048::I048_020_FX val = {0};
                val.TST = rndU(1);
                val.ME = rndU(1);
                val.MI = rndU(1);
                val.FOEFRI = rndU(2);
                extension.push_back(QVariant::fromValue(val));
            }
            aDataSource.insert(Asterix048::TargetReportDescriptor_FX, QVariant::fromValue(extension));
        }

        if (testChance())
        {
            QVariantList extension;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
                extension.push_back(rndU(7));
            aDataSource.insert(Asterix048::WarningErrorConditions, QVariant::fromValue(extension));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::MeasuredPositionInPolarCoordinates_RHO, rndU(16) / 256.);
            aDataSource.insert(Asterix048::MeasuredPositionInPolarCoordinates_THETA, rndU(16) * 360. / 65536.);
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::CalculatedPositionInCartesianCoordinates_X, rndU(16) / 128.);
            aDataSource.insert(Asterix048::CalculatedPositionInCartesianCoordinates_Y, rndU(16) / 128.);
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_V, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_G, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_L, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_A4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_A2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_A1, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_B4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_B2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_B1, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_C4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_C2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_C1, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_D4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_D2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeInOctalRepresentation_D1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_V, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_G, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_L, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_A4, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_A2, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_A1, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_B2, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeInOctalRepresentation_B1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QB4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QB1, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QC4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QC2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QC1, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QD4, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QD2, rndU(1));
            aDataSource.insert(Asterix048::Mode2CodeConfidenceIndicator_QD1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::Mode1CodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix048::Mode1CodeConfidenceIndicator_QB1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_V, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_G, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_L, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_A4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_A2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_A1, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_B4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_B2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_B1, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_C4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_C2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_C1, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_D4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_D2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeInOctalRepresentation_D1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QB4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QB1, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QC4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QC2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QC1, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QD4, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QD2, rndU(1));
            aDataSource.insert(Asterix048::Mode3ACodeConfidenceIndicator_QD1, rndU(1));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::FlightLevelInBinaryRepresentation_V, rndU(1));
            aDataSource.insert(Asterix048::FlightLevelInBinaryRepresentation_G, rndU(1));
            aDataSource.insert(Asterix048::FlightLevelInBinaryRepresentation_FlightLevel, rndU(14));
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_V, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_G, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_C1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_A1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_C2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_A2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_C4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_A4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_B1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_D1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_B2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_D2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_B4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_D4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QC1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QA1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QC2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QA2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QC4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QA4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QB1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QD1, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QB2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QD2, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QB4, rndU(1));
            aDataSource.insert(Asterix048::ModeCCodeAndCodeConfidenceIndicator_QD4, rndU(1));
        }

        if (testChance())
            aDataSource.insert(Asterix048::HeightMeasuredBy3DRadar, rndU(14) * 25.);

        if (testChance())
        {
            aDataSource.insert(Asterix048::RadialDopplerSpeed_CalculatedDopplerSpeed_D, rndU(1));
            aDataSource.insert(Asterix048::RadialDopplerSpeed_CalculatedDopplerSpeed_CAL, rndU(10));

            QVariantList extension;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
            {
                Asterix048::I048_120_2 val = {0};
                val.DopplerSpeed = rndU(16);
                val.AmbiguityRange = rndU(16);
                val.TransmitterFrequency = rndU(16);
                extension.push_back(QVariant::fromValue(val));
            }
            aDataSource.insert(Asterix048::RadialDopplerSpeed_RawDopplerSpeed, QVariant::fromValue(extension));
        }

        if (testChance())
        {
            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_SsrPlotRunlength, rndU(8) * 360. / 8192);

            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_NumberOfReceivedRepliesForMSsr, rndU(8));

            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_AmplitudeOfMSSRReply, rndU(8));

            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_PrimaryPlotRunlength, rndU(8) * 360. / 8192);

            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_AmplitudeOfPrimaryPlot, rndU(8));

            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_DifferenceInRangeBetweenPsrAndSsrPlot, rndS(8) / 256.);

            if (testChance())
                aDataSource.insert(Asterix048::RadarPlotCharacteristics_DifferenceInAzimuthBetweenPsrSndSsrPlot, rndS(8) * 360. / 16384.);
        }

        if (testChance())
            aDataSource.insert(Asterix048::TimeOfDay, QTime::currentTime());

        if (testChance())
            aDataSource.insert(Asterix048::TrackNumber, rndU(12));

        if (testChance())
        {
            aDataSource.insert(Asterix048::TrackStatus_CNF, rndU(1));
            aDataSource.insert(Asterix048::TrackStatus_RAD, rndU(2));
            aDataSource.insert(Asterix048::TrackStatus_DOU, rndU(1));
            aDataSource.insert(Asterix048::TrackStatus_MAH, rndU(1));
            aDataSource.insert(Asterix048::TrackStatus_CDM, rndU(2));

            if (testChance())
            {
                QVariantList extension;
                for (int amount = rand() % 100 + 1; amount != 0; --amount)
                {
                    Asterix048::I048_170_FX val = {0};
                    val.TRE = rndU(1);
                    val.GHO = rndU(1);
                    val.SUP = rndU(1);
                    val.TCC = rndU(1);
                    extension.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix048::TrackStatus_FX, QVariant::fromValue(extension));
            }
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::CalculatedTrackVelocityInPolarCoordinates_GROUNDSPEED, rndU(16) / 16384.);
            aDataSource.insert(Asterix048::CalculatedTrackVelocityInPolarCoordinates_HEADING, rndU(16) * 360. / 65536.);
        }

        if (testChance())
        {
            aDataSource.insert(Asterix048::TrackQuality_X, rndU(8) / 128.);
            aDataSource.insert(Asterix048::TrackQuality_Y, rndU(8) / 128.);
            aDataSource.insert(Asterix048::TrackQuality_V, rndU(8) / 16384.);
            aDataSource.insert(Asterix048::TrackQuality_H, rndU(8) * 360. / 4096.);
        }

        if (testChance())
            aDataSource.insert(Asterix048::AircraftAddress, rndU(24));

        if (testChance())
        {
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_COM, rndU(3));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_STAT, rndU(3));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_SI, rndU(1));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_MSSC, rndU(1));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_ARC, rndU(1));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_AIC, rndU(1));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_B1A, rndU(1));
            aDataSource.insert(Asterix048::CommunicationsAcasCapabilityAndFlightStatus_B1B, rndU(4));
        }

        if (testChance())
            aDataSource.insert(Asterix048::AircraftIdentification, rndArray(8, 6));

        if (testChance())
        {
            QVariantList extension;
            for (int amount = rand() % 100 + 1; amount != 0; --amount)
            {
                Asterix048::I048_250 val = {{0}};
                for (unsigned int i = 0; i < sizeof(val.MbData); ++i)
                    val.MbData[i] = rndU(8);
                val.BDS1 = rndU(4);
                val.BDS2 = rndU(4);
                extension.push_back(QVariant::fromValue(val));
            }
            aDataSource.insert(Asterix048::ModeSMbData, extension);
        }

        if (testChance())
            aDataSource.insert(Asterix048::AcasResolutionAdvisoryReport, rndArray(7));

        if (testChance())
            aDataSource.insert(Asterix048::SpecialPurposeField, rndArray(rnd() % 254 + 1));

        if (testChance())
            aDataSource.insert(Asterix048::ReservedExpansionField, rndArray(rnd() % 254 + 1));

        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data))
        {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;
    }
}

void AsterixTest::testAsterix048()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}

void AsterixTest::testAsterix063_data()
{
    QTest::addColumn<AsterixData>("source");
    QTest::addColumn<AsterixData>("expected");

    for (int i = 0; i < 100; ++i)
    {
        setRandomTestChance();

        AsterixData aDataSource;
        aDataSource.category = Asterix063::category;

        if (true)
        {
            // Data Item I063/010, Data Source Identifier
            aDataSource.insert(Asterix063::DataSourceIdentifier_SAC, rndU(8));
            aDataSource.insert(Asterix063::DataSourceIdentifier_SIC, rndU(8));
        }

        if (testChance())
        {
            // Data Item I063/015, Service Identification
            aDataSource.insert(Asterix063::ServiceIdentification, rndU(8));
        }

        if (true)
        {
            // Data Item I063/030, Time of Message
            aDataSource.insert(Asterix063::TimeOfMessage, rndTime());
        }

        if (true)
        {
            // Data Item I063/050, Sensor Identifier
            aDataSource.insert(Asterix063::SensorIdentifier_SAC, rndU(8));
            aDataSource.insert(Asterix063::SensorIdentifier_SIC, rndU(8));
        }

        if (testChance())
        {
            // Data Item I063/060, Sensor Configuration and Status
            aDataSource.insert(Asterix063::SensorConfigurationAndStatus_CON, rndU(2));
            aDataSource.insert(Asterix063::SensorConfigurationAndStatus_PSR, rndU(1));
            aDataSource.insert(Asterix063::SensorConfigurationAndStatus_SSR, rndU(1));
            aDataSource.insert(Asterix063::SensorConfigurationAndStatus_MDS, rndU(1));
            aDataSource.insert(Asterix063::SensorConfigurationAndStatus_ADS, rndU(1));
            aDataSource.insert(Asterix063::SensorConfigurationAndStatus_MLT, rndU(1));

            if (testChance())
            {
                QVariantList extension;
                for (int amount = rand() % 100 + 1; amount != 0; --amount)
                {
                    Asterix063::I063_060_FX val = {0};
                    val.OPS = rndU(1);
                    val.ODP = rndU(1);
                    val.OXT = rndU(1);
                    val.MSC = rndU(1);
                    val.TSV = rndU(1);
                    val.NPW = rndU(1);
                    extension.push_back(QVariant::fromValue(val));
                }
                aDataSource.insert(Asterix063::SensorConfigurationAndStatus_FX, extension);
            }
        }

        if (testChance())
        {
            // Data Item I063/070, Time Stamping Bias
            aDataSource.insert(Asterix063::TimeStampingBias, rndU(16));
        }

        if (testChance())
        {
            // Data Item I063/080, SSR / Mode S Range Gain and Bias
            aDataSource.insert(Asterix063::SsrModeSRangeGainAndBias_SRG, rndS(16) / 100000.);
            aDataSource.insert(Asterix063::SsrModeSRangeGainAndBias_SRB, rndS(16) / 128.);
        }

        if (testChance())
        {
            // Data Item I063/081, SSR / Mode S Azimuth Bias
            aDataSource.insert(Asterix063::SsrModeSAzimuthBias, rndS(16) * 360. / 65536.);
        }

        if (testChance())
        {
            // Data Item I063/090, PSR Range Gain and Bias
            aDataSource.insert(Asterix063::PsrRangeGainSndBias_PRG, rndS(16) / 100000.);
            aDataSource.insert(Asterix063::PsrRangeGainSndBias_PRB, rndS(16) / 128.);
        }

        if (testChance())
        {
            // Data Item I063/091, PSR Azimuth Bias
            aDataSource.insert(Asterix063::PsrAzimuthBias, rndS(16) * 360. / 65536.);
        }

        if (testChance())
        {
            // Data Item I063/092, PSR Elevation Bias
            aDataSource.insert(Asterix063::PsrElevationBias, rndS(16) * 360. / 65536.);
        }

        if (testChance())
            aDataSource.insert(Asterix063::SpecialPurposeField, rndArray(rnd() % 254 + 1));

        if (testChance())
            aDataSource.insert(Asterix063::ReservedExpansionField, rndArray(rnd() % 254 + 1));

        QByteArray data;
        if (!aDataSource.toArray(data))
        {
            QWARN(qPrintable(QString("ad1: %1").arg(QString(aDataSource.lastError()))));
            continue;
        }

        AsterixData aDataResult;
        if (!aDataResult.fromArray(data))
        {
            QWARN(qPrintable(QString("ad2: %1").arg(QString(aDataResult.lastError()))));
            continue;
        }

        QTest::newRow(QByteArray::number(i).data()) << aDataSource << aDataResult;
    }
}

void AsterixTest::testAsterix063()
{
    QFETCH(AsterixData, source);
    QFETCH(AsterixData, expected);

    QCOMPARE(source.category, expected.category);

    QCOMPARE(source, expected);
}
