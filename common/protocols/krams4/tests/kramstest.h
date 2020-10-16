#ifndef KRAMSTEST_H
#define KRAMSTEST_H

#include <QtCore>

class KramsTest : public QObject
{
    Q_OBJECT

public:
    KramsTest(QObject *parent = 0);

private slots:
    void testKrams4Ping();
    void testKrams4Tickets();

    void testKrams4InfoMessage();
};

#endif // KRAMSTEST_H
