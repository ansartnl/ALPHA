#include "kramstest.h"

#include "kramsproto.h"
#include "kramstickets.h"
#include "kramsping.h"
#include "krams4.h"

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

KramsTest::KramsTest(QObject *parent) : QObject(parent)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

void KramsTest::testKrams4Ping()
{
    TKramsMessage message(new KramsPing);

    QCOMPARE(message->type(), KramsPing::classType());

    KramsProtocol proto;
    QByteArray ba = proto.toArray(message);

    QVERIFY( proto.lastErrors().empty() );

    TKramsMessage messageTest = proto.fromArray(ba);

    QVERIFY( messageTest );

    QCOMPARE(message->type(), messageTest->type());
    QCOMPARE(message->version(), messageTest->version());
}

void KramsTest::testKrams4Tickets()
{
    TKramsMessage messageOk(new KramsTicketOk);
    TKramsMessage messageEr(new KramsTicketEr);
    TKramsMessage messageRq(new KramsTicketRq);

    QCOMPARE(messageOk->type(), KramsTicketOk::classType());
    QCOMPARE(messageEr->type(), KramsTicketEr::classType());
    QCOMPARE(messageRq->type(), KramsTicketRq::classType());

    static char ticketMessageOk[] = { 1, 'O', 'K', 4 };
    static char ticketMessageEr[] = { 1, 'E', 'R', 4 };
    static char ticketMessageRq[] = { 1, 'R', 'Q', 4 };

    KramsProtocol proto;

    // Ticket Ok test
    QByteArray ba = proto.toArray(messageOk);

    QVERIFY( proto.lastErrors().empty() );
    QVERIFY( memcmp(ticketMessageOk, ba.constData(), 4) == 0 );

    // Ticket Error test
    ba = proto.toArray(messageEr);

    QVERIFY( proto.lastErrors().empty() );
    QVERIFY( memcmp(ticketMessageEr, ba.constData(), 4) == 0 );

    // Ticket Request test
    ba = proto.toArray(messageRq);

    QVERIFY( proto.lastErrors().empty() );
    QVERIFY( memcmp(ticketMessageRq, ba.constData(), 4) == 0 );
}

void KramsTest::testKrams4InfoMessage()
{
    TKramsMessage messageInfo(new Krams4);

    QCOMPARE(messageInfo->type(), Krams4::classType());
    QCOMPARE(messageInfo->version(), Krams4::classVersion());

    Krams4 *krams4 = static_cast<Krams4 *>(messageInfo.data());

    krams4->append(Krams4::DateTime, QDateTime::currentDateTime());
    krams4->append(Krams4::Data001, qrand() % 360);
    krams4->append(Krams4::Data002, "06RR");
    krams4->append(Krams4::Data010, 1280.0);
    Krams4::SVPPState state;
    if ( (qrand() % 2) < 1 )
        state.isCleanDry = true;
    {
        state.a = qrand() % 10;
        state.b = qrand() % 10;
        state.mm = qrand() % 100;
    }
    krams4->append(Krams4::Data011, QVariant::fromValue(state));
    krams4->append(Krams4::Data020, qrand() % 4);
    krams4->append(Krams4::Data021, qrand() % 999999);
    krams4->append(Krams4::Data022, qrand() % 999999);
    krams4->append(Krams4::Data023, qrand() % 999999);
    krams4->append(Krams4::Data024, qrand() % 999999);
    krams4->append(Krams4::Data025, qrand() % 999999);
    krams4->append(Krams4::Data030, (qrand() % 100) < 50 ? 0 : 2);
    krams4->append(Krams4::Data031, qrand() % 999999);
    krams4->append(Krams4::Data032, qrand() % 999999);
    krams4->append(Krams4::Data033, qrand() % 999999);
    krams4->append(Krams4::Data034, qrand() % 999999);
    krams4->append(Krams4::Data040, (qrand() % 100) < 50 ? 0 : 2);
    krams4->append(Krams4::Data042, qrand() % 999999);
    krams4->append(Krams4::Data043, qrand() % 999999);
    krams4->append(Krams4::Data050, qrand() % 2);
    krams4->append(Krams4::Data051, qrand() % 999999);
    krams4->append(Krams4::Data060, qrand() % 999999);
    krams4->append(Krams4::Data061, (qrand() % 100) < 50 ? 1 : 2);
    static int cloudiness[] = { 0, 1, 2, 3, 9 };
    krams4->append(Krams4::Data100, cloudiness[qrand() % 5]);
    krams4->append(Krams4::Data101, qrand() % 999999);
    krams4->append(Krams4::Data102, qrand() % 999999);
    krams4->append(Krams4::Data103, (qrand() % 100) < 50 ? 1 : 2);
    krams4->append(Krams4::Data104, qrand() % 999999);
    krams4->append(Krams4::Data111, qrand() % 999999);
    krams4->append(Krams4::Data112, qrand() % 999999);
    krams4->append(Krams4::Data113, (qrand() % 100) < 50 ? 1 : 2);
    krams4->append(Krams4::Data121, qrand() % 999999);
    krams4->append(Krams4::Data122, qrand() % 999999);
    krams4->append(Krams4::Data123, (qrand() % 100) < 50 ? 1 : 2);
    krams4->append(Krams4::Data140, qrand() % 360);
    krams4->append(Krams4::Data141, qrand() % 360);
    krams4->append(Krams4::Data150, qrand() % 999999);
    krams4->append(Krams4::Data151, qrand() % 999999);
    krams4->append(Krams4::Data152, qrand() % 999999);
    krams4->append(Krams4::Data160, qrand() % 4);
    krams4->append(Krams4::Data161, qrand() % 999999);
    krams4->append(Krams4::Data162, qrand() % 999999);
    krams4->append(Krams4::Data170, qrand() % 4);
    krams4->append(Krams4::Data171, qrand() % 999999);
    krams4->append(Krams4::Data172, qrand() % 999999);
    krams4->append(Krams4::Data173, qrand() % 3 + 1);
    krams4->append(Krams4::Data180, qrand() % 4);
    krams4->append(Krams4::Data181, qrand() % 360);
    krams4->append(Krams4::Data182, qrand() % 360);
    krams4->append(Krams4::Data183, qrand() % 360);
    krams4->append(Krams4::Data184, qrand() % 999999);
    krams4->append(Krams4::Data185, qrand() % 999999);
    krams4->append(Krams4::Data186, qrand() % 16 + 1);
    krams4->append(Krams4::Data187, qrand() % 999999);
    krams4->append(Krams4::Data188, qrand() % 3);
    krams4->append(Krams4::Data190, qrand() % 2);
    krams4->append(Krams4::Data200, " +RA FG=");
    krams4->append(Krams4::Data201, " BECMG 3000=");
    krams4->append(Krams4::Data210, qrand() % 2);
    krams4->append(Krams4::Data220, qrand() % 2);
    krams4->append(Krams4::Data230, qrand() % 2);

    KramsProtocol proto;

    QByteArray ba = proto.toArray(messageInfo);

    QVERIFY( proto.lastErrors().empty() );

    int readedLength = 0;
    TKramsMessage messageInfoTest = proto.fromArray(ba, &readedLength);

    QVERIFY(messageInfoTest->type() == Krams4::classType());

    Krams4 *krams4Test = static_cast<Krams4 *>(messageInfoTest.data());

    krams4->toFront();
    krams4Test->toFront();

    do
    {
        QCOMPARE(krams4->key(), krams4Test->key());

        QVariant value = krams4->value();
        QVariant valueTest = krams4Test->value();

        if ( value.canConvert<Krams4::SVPPState>() )
        {
            Krams4::SVPPState state = value.value<Krams4::SVPPState>();
            Krams4::SVPPState stateTest = valueTest.value<Krams4::SVPPState>();

            QCOMPARE(state.isCleanDry, state.isCleanDry);
            QCOMPARE(state.a, state.a);
            QCOMPARE(state.b, state.b);
            QCOMPARE(state.mm, state.mm);
        }
        else if ( value.canConvert<double>() )
            QVERIFY( qFuzzyCompare(value.toDouble(), valueTest.toDouble()) );
        else if ( value.canConvert<int>() )
            QCOMPARE(value, valueTest);
        else if ( value.canConvert<QDateTime>() )
        {
            QCOMPARE(value.toDateTime().date(), valueTest.toDateTime().date());
            QTime time = value.toDateTime().time();
            QTime timeTest = valueTest.toDateTime().time();
            QCOMPARE(time.hour(), timeTest.hour());
            QCOMPARE(time.minute(), timeTest.minute());
            QCOMPARE(time.second(), timeTest.second());
        }
        else if ( value.canConvert<QString>() )
            QCOMPARE(value.toString().trimmed(), valueTest.toString().trimmed());

    } while ( krams4->next() && krams4Test->next() );
}
