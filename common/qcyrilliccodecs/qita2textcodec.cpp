#include "qita2textcodec.h"

#include <QtDebug>
#define InRange(c, lower, upper)  (((c) >= (lower)) && ((c) <= (upper)))
#define IsLatin(c)        ((c) <= 0x7F)

QByteArray QITA2TextCodec::name() const
{
    return QByteArray("ITA2");
}

QList<QByteArray> QITA2TextCodec::aliases() const
{
    QList<QByteArray> result;
    result += QByteArray("CCITT-2");
    return result;
}

int QITA2TextCodec::mibEnum() const
{
    return -1;
}

static unsigned short ITA2LatinTable[] = {
/*NULL*/0x0000
/*E*/, 0x0045
/*carriage return*/, 0x000D
/*A   */, 0x0041
/*space*/, 0x0020
/*S*/, 0x0053
/*I*/, 0x0049
/*U   */, 0x0055
/*line feed*/, 0x000A
/*D   */, 0x0044
/*R   */, 0x0052
/*J   */, 0x004A
/*N   */, 0x004E
/*F   */, 0x0046
/*C   */, 0x0043
/*K   */, 0x004B
/*T   */, 0x0054
/*Z   */, 0x005A
/*L   */, 0x004C
/*W   */, 0x0057
/*H   */, 0x0048
/*Y   */, 0x0059
/*P   */, 0x0050
/*Q   */, 0x0051
/*O   */, 0x004F
/*B   */, 0x0042
/*G   */, 0x0047
/*switch to figures*/, 0x0000
/*M   */, 0x004D
/*X   */, 0x0058
/*V   */, 0x0056
/*switch to latin*/, 0x0000 };

static unsigned short ITA2DigitTable[] = {
    /* NULL */0x0000
/*3   */, 0x0033
/*carriage return*/, 0x000D
/*-   */, 0x002D
/*space*/, 0x0020
/*'   */, 0x0027
/*8   */, 0x0038
/*7   */, 0x0037
/* line feed*/, 0x000A
/*ENQ   */, 0x0005
/*4   */, 0x0034
/*BELL    */, 0x0007
/*,   */, 0x002C
/*!   */, 0x0021
/*:   */, 0x003A
/*(   */, 0x0028
/*5   */, 0x0035
/*+   */, 0x002B
/*)   */, 0x0029
/*2   */, 0x0032
/*$   */, 0x0024
/*6   */, 0x0036
/*0   */, 0x0030
/*1   */, 0x0031
/*9   */, 0x0039
/*?   */, 0x003F
/*&   */, 0x0026
/*switch to figures*/, 0x0000
/*.   */, 0x002E
/*/   */, 0x002F
/*;   */, 0x003B
/*switch to latin*/, 0x0000 };

#define ITA2MASK (0x1F)
#define SWITCHTOLATIN 0x1F
#define SWITCHTOFIGURES 0x1B
#define QUESTIONMARK 0x13
#define CARRIAGERETURNINDEX 0x02
#define LINEFEEDINDEX 0x08
#define SPACEINDEX 0x04

QString QITA2TextCodec::convertToUnicode(const char *in, int length,
                                         ConverterState */*state*/) const
{
    QString result;
    unsigned char currentChar;
    unsigned short *table = ITA2LatinTable;
    for (int i = 0; i < length; ++i) {
        currentChar = in[i];
        currentChar &= ITA2MASK;
        if (currentChar == SWITCHTOLATIN) {
            table = ITA2LatinTable;
        } else if (currentChar == SWITCHTOFIGURES) {
            table = ITA2DigitTable;
        } else if (currentChar == 0) {
            table = ITA2LatinTable;
        } else {
            // it's character
            result.append(table[currentChar]);
        }
    }
    return result;
}

static int indexIn(unsigned short *array, int size, unsigned short value)
{
    for (int i = 0; i < size; ++i)
        if (array[i] == value)
            return i;
    return -1;
}

QByteArray QITA2TextCodec::convertFromUnicode(const QChar *in, int length,
                                              ConverterState */*state*/) const
{
    QByteArray result;

    unsigned short current;
    unsigned short *table = ITA2LatinTable;

    for (int i = 0; i < length; ++i) {
        current = in[i].unicode();
        if (IsLatin(current)) {
            int index = indexIn(ITA2LatinTable, sizeof(ITA2LatinTable)
                                / sizeof(unsigned short), current);
            if (index == LINEFEEDINDEX || index == CARRIAGERETURNINDEX || index
                == SPACEINDEX) {
                result.append(index);
                continue;
            }
            if (index == LINEFEEDINDEX || index == CARRIAGERETURNINDEX) {
                if (table != ITA2LatinTable) {
                    table = ITA2LatinTable;
                }
            }
            if (index != -1) {
                if (table != ITA2LatinTable) {
                    result.append(SWITCHTOLATIN);
                    table = ITA2LatinTable;
                }
                result.append(index);
                continue;
            }

            index = indexIn(ITA2DigitTable, sizeof(ITA2DigitTable)
                            / sizeof(unsigned short), current);
            if (index != -1) {
                if (table != ITA2DigitTable) {
                    result.append(SWITCHTOFIGURES);
                    table = ITA2DigitTable;
                }
                result.append(index);
                continue;
            }
        } else {

        }
    }

    return result;
}

QITA2TextCodec::QITA2TextCodec() :
        QTextCodec()
{
}
