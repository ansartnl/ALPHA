#include "qmtk2textcodec.h"
#include <QMap>

typedef QMap<unsigned char, QChar>       TTelegraphCodecMap;
static TTelegraphCodecMap g_enSymbols, g_ruSymbols, g_digits;
typedef QMap<QChar, unsigned char>       TTelegraphDecoderMap;
static TTelegraphDecoderMap d_enSymbols, d_ruSymbols, d_digits;

static unsigned char MTK2_RUS = 0x00;
static unsigned char MTK2_ENG = 0x1F;
static unsigned char MTK2_DIG = 0x1B;

static unsigned char MTK2_SPACE  = 0x04;
static unsigned char MTK2_CR     = 0x08;
static unsigned char MTK2_LF     = 0x02;

MTK2Codec::MTK2Codec()
    : _state(MTK2_ENG)
{
    // Init english conversion table.
    if ( g_enSymbols.empty() )
    {
        g_enSymbols[0x03] = 'A';
        g_enSymbols[0x19] = 'B';
        g_enSymbols[0x0E] = 'C';
        g_enSymbols[0x09] = 'D';
        g_enSymbols[0x01] = 'E';
        g_enSymbols[0x0D] = 'F';
        g_enSymbols[0x1A] = 'G';
        g_enSymbols[0x14] = 'H';
        g_enSymbols[0x06] = 'I';
        g_enSymbols[0x0B] = 'J';
        g_enSymbols[0x0F] = 'K';
        g_enSymbols[0x12] = 'L';
        g_enSymbols[0x1C] = 'M';
        g_enSymbols[0x0C] = 'N';
        g_enSymbols[0x18] = 'O';
        g_enSymbols[0x16] = 'P';
        g_enSymbols[0x17] = 'Q';
        g_enSymbols[0x0A] = 'R';
        g_enSymbols[0x05] = 'S';
        g_enSymbols[0x10] = 'T';
        g_enSymbols[0x07] = 'U';
        g_enSymbols[0x1E] = 'V';
        g_enSymbols[0x13] = 'W';
        g_enSymbols[0x1D] = 'X';
        g_enSymbols[0x15] = 'Y';
        g_enSymbols[0x11] = 'Z';

        QList<unsigned char> keys = g_enSymbols.keys();
        // Init english decoder map.
        foreach ( unsigned char k, keys )
        {
            d_enSymbols[ g_enSymbols[k] ] = k;
        }
    }

    // Init russian conversion table.
    if ( g_ruSymbols.empty() )
    {
        g_ruSymbols[0x03] = 0x0410;
        g_ruSymbols[0x19] = 0x0411;
        g_ruSymbols[0x0E] = 0x0426;
        g_ruSymbols[0x09] = 0x0414;
        g_ruSymbols[0x01] = 0x0415;
        g_ruSymbols[0x0D] = 0x0424;
        g_ruSymbols[0x1A] = 0x0413;
        g_ruSymbols[0x14] = 0x0425;
        g_ruSymbols[0x06] = 0x0418;
        g_ruSymbols[0x0B] = 0x0419;
        g_ruSymbols[0x0F] = 0x041A;
        g_ruSymbols[0x12] = 0x041B;
        g_ruSymbols[0x1C] = 0x041C;
        g_ruSymbols[0x0C] = 0x041D;
        g_ruSymbols[0x18] = 0x041E;
        g_ruSymbols[0x16] = 0x041F;
        g_ruSymbols[0x17] = 0x042F;
        g_ruSymbols[0x0A] = 0x0420;
        g_ruSymbols[0x05] = 0x0421;
        g_ruSymbols[0x10] = 0x0422;
        g_ruSymbols[0x07] = 0x0423;
        g_ruSymbols[0x1E] = 0x0416;
        g_ruSymbols[0x13] = 0x0412;
        g_ruSymbols[0x1D] = 0x042C;
        g_ruSymbols[0x15] = 0x042B;
        g_ruSymbols[0x11] = 0x0417;

        QList<unsigned char> keys = g_ruSymbols.keys();
        // Init russian decoder map.
        foreach ( unsigned char k, keys )
        {
            d_ruSymbols[ g_ruSymbols[k] ] = k;
        }
    }

    // Init digits table.
    if ( g_digits.empty() )
    {
        g_digits[0x03] = '-';
        g_digits[0x19] = '?';
        g_digits[0x0E] = ':';
        g_digits[0x09] = '@';
        g_digits[0x01] = '3';
        g_digits[0x0D] = 0x042D;
        g_digits[0x1A] = 0x0428;
        g_digits[0x14] = 0x0429;
        g_digits[0x06] = '8';
        g_digits[0x0B] = 0x042E;
        g_digits[0x0F] = '(';
        g_digits[0x12] = ')';
        g_digits[0x1C] = '.';
        g_digits[0x0C] = ',';
        g_digits[0x18] = '9';
        g_digits[0x16] = '0';
        g_digits[0x17] = '1';
        g_digits[0x0A] = '4';
        g_digits[0x05] = '\'';
        g_digits[0x10] = '5';
        g_digits[0x07] = '7';
        g_digits[0x1E] = '=';
        g_digits[0x13] = '2';
        g_digits[0x1D] = '/';
        g_digits[0x15] = '6';
        g_digits[0x11] = '+';

        QList<unsigned char> keys = g_digits.keys();
        // Init digits decoder map.
        foreach ( unsigned char k, keys )
        {
            d_digits[ g_digits[k] ] = k;
        }
    }
}

MTK2Codec::~MTK2Codec()
{
}

QByteArray MTK2Codec::name() const
{
    return "MTK-2";
}

int MTK2Codec::mibEnum() const
{
    return 3;
}

QString MTK2Codec::convertToUnicode(const char *in, int length, ConverterState *state) const
{
    Q_UNUSED(state);

    QString unicodeString;
    for ( int i = 0; i < length; i++ )
    {
        unsigned char code = in[i] & 0x1F;

        // Check switching symbols.
        if ( code == MTK2_RUS )
        {
            _state = MTK2_RUS;
            continue;
        }
        else if ( code == MTK2_ENG )
        {
            _state = MTK2_ENG;
            continue;
        }
        else if ( code == MTK2_DIG )
        {
            _state = MTK2_DIG;
            continue;
        }

        // Check for special symbols.
        if ( code == MTK2_SPACE )
        {
            unicodeString += " ";
            continue;
        }
        else if ( code == MTK2_CR )
        {
            unicodeString += "\r";
            continue;
        }
        else if ( code == MTK2_LF )
        {
            unicodeString += "\n";
            continue;
        }
        // Convert to unicode.
        else
        {
            const TTelegraphCodecMap &convTable = _state == MTK2_RUS
                    ? g_ruSymbols : (_state == MTK2_DIG ? g_digits : g_enSymbols);
            unicodeString += convTable[code];
        }
    }

    return unicodeString;
}

QByteArray MTK2Codec::convertFromUnicode(const QChar *in, int length, ConverterState *state) const
{
    Q_UNUSED(state);

    // One of 3 telegraph states.
    unsigned char statePrev = 0xFF;

    QByteArray byteArray;
    for ( int i = 0; i < length; i++ )
    {
        QChar sym = in[i];

        unsigned char code = 0xFF;
        unsigned char state = 0xFF;

        // There is no 'Ч' russian symbol in the MTK-2 codepage. Used '4' instead.
        if ( sym == 0x0427 )
            sym = '4';

        // Check for special symbols.
        if ( sym == ' ' )
        {
            state = statePrev;
            code = MTK2_SPACE;
        }
        else if ( sym == '\r' )
        {
            state = statePrev;
            code = MTK2_CR;
        }
        else if ( sym == '\n' )
        {
            state = statePrev;
            code = MTK2_LF;
        }
        // Convert from unicode.
        else
        {
            if ( d_enSymbols.contains(sym) )
            {
                code = d_enSymbols[sym];
                state = MTK2_ENG;
            }
            else if ( d_ruSymbols.contains(sym) )
            {
                code = d_ruSymbols[sym];
                state = MTK2_RUS;
            }
            else if ( d_digits.contains(sym) )
            {
                code = d_digits[sym];
                state = MTK2_DIG;
            }
        }

        if ( code == -1 )
        {
            sym = d_digits['?'];
            state = MTK2_DIG;
        }

        // Add special symbol if necessary.
        if ( state != statePrev )
        {
            statePrev = state;
            byteArray += state;
        }

        byteArray += code;
    }

    return byteArray;
}
