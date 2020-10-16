#ifndef ASTERIXTEST_H
#define ASTERIXTEST_H

#include <QtCore>

class Asterix;

class AsterixTest : public QObject
{
    Q_OBJECT

public:
    AsterixTest(QObject *parent = 0);

private slots:
    void testAsterixFieldColision();

    void testAsterix001_data();
    void testAsterix001();

    void testAsterix002_data();
    void testAsterix002();

    void testAsterix008();

    void testAsterix021_data();
    void testAsterix021();

    void testAsterix034_data();
    void testAsterix034();

    void testAsterix048_data();
    void testAsterix048();

    void testAsterix062_data();
    void testAsterix062();

    void testAsterix063_data();
    void testAsterix063();

private:
    void setRandomTestChance();
    bool testChance() const;
    static QString translate(const QByteArray& array);
    static quint32 rnd();
    static qint32 rndS(int i = 32);
    static quint32 rndU(int i = 32);
    static QByteArray rndArray(int size, int bit = 8);
    static QTime rndTime();

    quint32 chance;

    void testAsterix(Asterix *asterix);
};

#endif // ASTERIXTEST_H
