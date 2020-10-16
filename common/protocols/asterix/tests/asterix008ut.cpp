#include "asterixFuncs.h"
#include "helperFuncs.h"
#include "src/asterixdata.h"
#include "src/asterix008.h"

#include <QtTest/QtTest>
#include <QtCore/QTime>
#include <QtCore/QDebug>


static Asterix008::EMessageType messageTypes[] = { Asterix008::PolarVector,
                                                   Asterix008::CartesianVector,
                                                   Asterix008::ContourPoints,
                                                   Asterix008::WeatherVector,
                                                   Asterix008::SOPMessage,
                                                   Asterix008::EOPMessage
};


void generateProcessingStatus(AsterixData &ad8)
{
    Asterix008::SProcessingStatus ps;
    ps.scaleFactor = (rand() % 16) * (((rand() % 100) < 50) ? -1 : 1);
    ps.reductionStage = rand() % 8;
    ps.parameters = rand() % 32768;

    ad8.insert(Asterix008::ProcessingStatus, QVariant::fromValue(ps));
}

static void generateStationConfigurationStatus(AsterixData &ad8)
{
    Asterix008::TStationConfigurationStatusList scs;
    int countScs = (rand() % 3) + 1;
    for ( int i = 0; i < countScs; i++ )
        scs.append( (rand() % 128) << 1 );

    ad8.insert(Asterix008::StationConfigurationStatus, QVariant::fromValue( scs ));
}

void generateVectorQualifier(AsterixData &ad8)
{
    Asterix008::SVectorQualifier vq;
    vq.org = (rand() % 100) < 50 ? Asterix008::LocalCoordinates : Asterix008::SystemCoordinates;
    vq.intensity = rand() % 8;
    vq.shadingOrientation = 22.5 * (rand() % 8);

    vq.isTestVector = (rand() % 100) < 50;

    ad8.insert(Asterix008::VectorQualifier, QVariant::fromValue( vq ));
}

void generateCartesianVectors(AsterixData &ad8)
{
    const qreal lsbFactor = qPow(2, -6+Asterix008::scaleFactor);

    Asterix008::TSequenceCartesianVectorList list;
    int count = (rand() % 5) + 1;
    for ( int i = 0; i < count; i++ )
    {
        Asterix008::SCartesianVector cv;
        cv.x = lsbFactor * (rand() % 8);
        cv.y = lsbFactor * (rand() % 8);
        cv.length = lsbFactor * (rand() % 8);
        list.append( QVariant::fromValue( cv ) );
    }
    ad8.insert(Asterix008::SequenceCartesianVectors, list);
}

void generatePolarVectors(AsterixData &ad8)
{
    const qreal lsbFactor = qPow(2, -7+Asterix008::scaleFactor);

    Asterix008::TSequencePolarVectorList list;
    int count = (rand() % 5) + 1;
    for ( int i = 0; i < count; i++ )
    {
        Asterix008::SPolarVector pv;
        pv.startRange = lsbFactor * (rand() % 8);
        pv.endRange = lsbFactor * (rand() % 8);
        pv.azimuth = 0.0055 * (rand() % 8);
        list.append( QVariant::fromValue( pv ) );
    }
    ad8.insert(Asterix008::SequencePolarVectors, list);
}

void generateWeatherVectors(AsterixData &ad8)
{
    const qreal lsbFactor = qPow(2, -6+Asterix008::scaleFactor);

    Asterix008::TSequenceWeatherVectorList list;
    int count = (rand() % 5) + 1;
    for ( int i = 0; i < count; i++ )
    {
        Asterix008::SWeatherVector wv;
        wv.x1 = lsbFactor * (rand() % 8);
        wv.y1 = lsbFactor * (rand() % 8);
        wv.x2 = lsbFactor * (rand() % 8);
        wv.y2 = lsbFactor * (rand() % 8);
        list.append( QVariant::fromValue(wv) );
    }
    ad8.insert(Asterix008::SequenceWeatherVectors, list);
}

void generateContourPoints(AsterixData &ad8)
{
    const qreal lsbFactor = qPow(2, -6+Asterix008::scaleFactor);

    Asterix008::TSequenceContourPointsList list;
    int count = (rand() % 5) + 1;
    for ( int i = 0; i < count; i++ )
    {
        Asterix008::SPoint point;
        point.x = lsbFactor * (rand() % 8);
        point.y = lsbFactor * (rand() % 8);
        list.append( QVariant::fromValue( point ) );
    }
    ad8.insert(Asterix008::SequenceContourPoints, list);
}

void generateAsterix008(AsterixData &ad8)
{
    Asterix008::scaleFactor = (rand() % 8) * ((rand() % 100) < 50 ? -1 : 1);
    Asterix008::EMessageType messageType = messageTypes[rand() % 6];

    qDebug() << "Message type is \"" << messageType << "\"";

    ad8.insert(Asterix008::DataSourceIdentifier_SAC, rndU(8));
    ad8.insert(Asterix008::DataSourceIdentifier_SIC, rndU(8));
    ad8.insert(Asterix008::MessageType, messageType);

    // Asterix008::SOPMessage restrictions.
    if ( messageType == Asterix008::SOPMessage )
    {
        ad8.insert(Asterix008::TimeOfDay, QTime(0,0).addSecs(7200));

        generateProcessingStatus(ad8);

        generateStationConfigurationStatus(ad8);
    }
    // Asterix008::EOPMessage restrictions.
    else if ( messageType == Asterix008::EOPMessage )
    {
        ad8.insert(Asterix008::TimeOfDay, QTime(0,0).addSecs(7200));

        generateStationConfigurationStatus(ad8);

        ad8.insert(Asterix008::TotalNumberItems, rand() % 65536);
    }
    // Asterix008::CartesianVector restrictions.
    else if ( messageType == Asterix008::CartesianVector )
    {
        generateVectorQualifier(ad8);
        generateCartesianVectors(ad8);
    }
    // Asterix008::WeatherVector restrictions.
    else if ( messageType == Asterix008::WeatherVector )
    {
        generateVectorQualifier(ad8);
        generateWeatherVectors(ad8);
    }
    // Asterix008::PolarVector restrictions.
    else if ( messageType == Asterix008::PolarVector )
    {
        generateVectorQualifier(ad8);
        generatePolarVectors(ad8);
    }
    // Asterix008::ContourPoints restrictions.
    else if ( messageType == Asterix008::ContourPoints )
    {
        Asterix008::SContourId cid;
        cid.org = (rand() % 100) < 50 ? Asterix008::LocalCoordinates : Asterix008::SystemCoordinates;
        cid.intensity = rand() % 8;
        cid.fstLst = Asterix008::SContourId::FST_LST( rand() % 4 );
        cid.serialNumber = rand() % 256;
        ad8.insert(Asterix008::ContourIdentifier, QVariant::fromValue( cid ));

        generateContourPoints(ad8);
    }
}

template<class T>
static bool compare(const QVariant &a, const QVariant &b)
{
    return ( a.type() == b.type() &&
             a.canConvert<T>() && b.canConvert<T>() &&
             a.value<T>() == b.value<T>() );
}

// SVectorQualifier
static bool operator == (const Asterix008::SVectorQualifier &a, const Asterix008::SVectorQualifier &b)
{
    return ( a.org == b.org && a.intensity == b.intensity &&
            myFuzzyCompare(a.shadingOrientation, b.shadingOrientation) &&
            a.isTestVector == b.isTestVector && a.isErrorCondition == b.isErrorCondition );
}

// SPolarVector
static bool operator == (const Asterix008::SPolarVector &a, const Asterix008::SPolarVector &b)
{
    return ( myFuzzyCompare(a.endRange, b.endRange) &&
            myFuzzyCompare(a.startRange, b.startRange) &&
            myFuzzyCompare(a.azimuth, b.azimuth) );
}

// SCartesianVector
static bool operator == (const Asterix008::SCartesianVector &a, const Asterix008::SCartesianVector &b)
{
    return ( myFuzzyCompare(a.x, b.x) && myFuzzyCompare(a.y, b.y) &&
            myFuzzyCompare(a.length, b.length) );
}

// SWeatherVector
static bool operator == (const Asterix008::SWeatherVector &a, const Asterix008::SWeatherVector &b)
{
    return ( myFuzzyCompare(a.x1, b.x1) && myFuzzyCompare(a.y1, b.y1) &&
            myFuzzyCompare(a.x1, b.x1) && myFuzzyCompare(a.y1, b.y1) );
}

// SContourId
static bool operator == (const Asterix008::SContourId &a, const Asterix008::SContourId &b)
{
    return ( a.org == b.org && a.intensity == b.intensity && a.fstLst == b.fstLst &&
             a.serialNumber == b.serialNumber );
}

// SPoint
static bool operator == (const Asterix008::SPoint &a, const Asterix008::SPoint &b)
{
    return ( myFuzzyCompare(a.x, b.x) && myFuzzyCompare(a.y, b.y) );
}

// SProcessingStatus
static bool operator == (const Asterix008::SProcessingStatus &a, const Asterix008::SProcessingStatus &b)
{
    return ( a.scaleFactor == b.scaleFactor && a.reductionStage == b.reductionStage &&
             a.parameters == b.parameters );
}

void parseAsterix008(const QByteArray &ba, const AsterixData &ad8)
{
    AsterixData ad;
    QVERIFY2( ad.fromArray(ba), qPrintable(QString(ad.lastError())) );

    QMap<QString, QVariant> ad_data = ad.data();
    QMap<QString, QVariant> ad8_data = ad8.data();
    QList<QString> keyList = ad_data.keys();
    QList<QString> keyList34 = ad8_data.keys();

    QCOMPARE( ad_data.count(), ad8_data.count() );

    foreach (const QString &key, keyList)
    {
        if (ad8_data.contains(key))
        {
            QVariant val = ad_data[key];
            QVariant val2 = ad8_data[key];
            ad_data.remove(key);
            ad8_data.remove(key);

            if ( val2.type() == QVariant::List )
            {
                QVariantList list = val.toList();
                QVariantList list2 = val2.toList();

                QCOMPARE(list.size(), list2.size());
                for (int i = 0; i < list.size(); ++i)
                {
                    const QVariant &v = list[i];
                    const QVariant &v2 = list2[i];

                    if ( v.canConvert<Asterix008::SVectorQualifier>() )
                        QVERIFY( compare<Asterix008::SVectorQualifier>(v, v2) );
                    else if ( v.canConvert<Asterix008::SPolarVector>() )
                        QVERIFY( compare<Asterix008::SPolarVector>(v, v2) );
                    else if ( v.canConvert<Asterix008::SCartesianVector>() )
                        QVERIFY( compare<Asterix008::SCartesianVector>(v, v2) );
                    else if ( v.canConvert<Asterix008::SWeatherVector>() )
                        QVERIFY( compare<Asterix008::SWeatherVector>(v, v2) );
                    else if ( v.canConvert<Asterix008::SContourId>() )
                        QVERIFY( compare<Asterix008::SContourId>(v, v2) );
                    else if ( v.canConvert<Asterix008::SPoint>() )
                        QVERIFY( compare<Asterix008::SPoint>(v, v2) );
                    else if ( v.canConvert<Asterix008::SProcessingStatus>() )
                        QVERIFY( compare<Asterix008::SProcessingStatus>(v, v2) );
                }
            }
            else if ( val2.canConvert<Asterix008::SVectorQualifier>() )
                QVERIFY( compare<Asterix008::SVectorQualifier>(val, val2) );
            else if ( val2.canConvert<Asterix008::SContourId>() )
                QVERIFY( compare<Asterix008::SContourId>(val, val2) );
            else if ( val2.canConvert<Asterix008::SProcessingStatus>() )
                QVERIFY( compare<Asterix008::SProcessingStatus>(val, val2) );
            else
            {
                if (val != val2)
                {
                    if ( val2.type() == QVariant::Time )
                    {
                        if (qAbs(val2.toTime().msecsTo(val.toTime())) > 100)
                            QFAIL( QString("Dif: %1 ms").arg(qAbs(val.toTime().msecsTo(val2.toTime()))).toLocal8Bit().constData() );
                    }
                    else if ( val2.type() == QVariant::Double )
                    {
                        QVERIFY2( myFuzzyCompare(val.toDouble(), val2.toDouble()),
                                  QString("Key - %1 \"%2\" != \"%3\"").arg(key)
                                  .arg(val.toDouble()).arg(val2.toDouble()).toLocal8Bit().constData() );
                    }
                    else
                    {
                        QFAIL( qPrintable( QString("Key - %1 \"%2\" != \"%3\"").arg(key).arg(val.toString())
                               .arg(val2.toString()) ) );
                    }
                }
            }
        }
    }
}

void asterix008UnitTest()
{
    AsterixData ad8;
    ad8.category = Asterix008::category;

    generateAsterix008(ad8);

    QByteArray ba;
    QVERIFY2( ad8.toArray(ba), qPrintable(QString(ad8.lastError())) );

    parseAsterix008(ba, ad8);
}
