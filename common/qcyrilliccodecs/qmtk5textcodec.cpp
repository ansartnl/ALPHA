#include "qmtk5textcodec.h"
#include <QMap>

typedef QMap<unsigned char, QChar>       TTelegraphCodecMap;
static TTelegraphCodecMap g_enSymbols5, g_ruSymbols5;//, g_digits5;
typedef QMap<QChar, unsigned char>       TTelegraphDecoderMap;
static TTelegraphDecoderMap d_enSymbols5, d_ruSymbols5;//, d_digits5;

static unsigned char MTK5_RUS = 0x0F;
static unsigned char MTK5_ENG = 0x0E;

static unsigned char MTK5_SPACE  = 0x20;
static unsigned char MTK5_CR     = 0x0D;
static unsigned char MTK5_LF     = 0x0A;
static unsigned char MTK5_BEL    = 0x07;

MTK5Codec::MTK5Codec()
    : _state5(MTK5_ENG)
{
    // Init english conversion table.
    if ( g_enSymbols5.empty() )
    {
        // special symbol
        g_enSymbols5[0x21] = '1';
        g_enSymbols5[0x22] = '"';
        g_enSymbols5[0x23] = '#';
        //g_enSymbols5[0x24] = '';
        g_enSymbols5[0x25] = '%';
        g_enSymbols5[0x26] = '&';
        g_enSymbols5[0x27] = '/';
        g_enSymbols5[0x28] = '(';
        g_enSymbols5[0x29] = ')';
        g_enSymbols5[0x2A] = '*';
        g_enSymbols5[0x2B] = '+';
        g_enSymbols5[0x2C] = '`';
        g_enSymbols5[0x2D] = '-';
        g_enSymbols5[0x2E] = '.';
        g_enSymbols5[0x2F] = '/';
        g_enSymbols5[0x30] = '0';
        g_enSymbols5[0x31] = '1';
        g_enSymbols5[0x32] = '2';
        g_enSymbols5[0x33] = '3';
        g_enSymbols5[0x34] = '4';
        g_enSymbols5[0x35] = '5';
        g_enSymbols5[0x36] = '6';
        g_enSymbols5[0x37] = '7';
        g_enSymbols5[0x38] = '8';
        g_enSymbols5[0x39] = '9';

        g_enSymbols5[0x3B] = '`';
        g_enSymbols5[0x3C] = '<';
        g_enSymbols5[0x3D] = '=';
        g_enSymbols5[0x3E] = '>';
        g_enSymbols5[0x3F] = '?';

        // big
        g_enSymbols5[0x41] = 'A';
        g_enSymbols5[0x42] = 'B';
        g_enSymbols5[0x43] = 'C';
        g_enSymbols5[0x44] = 'D';
        g_enSymbols5[0x45] = 'E';
        g_enSymbols5[0x46] = 'F';
        g_enSymbols5[0x47] = 'G';
        g_enSymbols5[0x48] = 'H';
        g_enSymbols5[0x49] = 'I';
        g_enSymbols5[0x4A] = 'J';
        g_enSymbols5[0x4B] = 'K';
        g_enSymbols5[0x4C] = 'L';
        g_enSymbols5[0x4D] = 'M';
        g_enSymbols5[0x4E] = 'N';
        g_enSymbols5[0x4F] = 'O';
        g_enSymbols5[0x50] = 'P';
        g_enSymbols5[0x51] = 'Q';
        g_enSymbols5[0x52] = 'R';
        g_enSymbols5[0x53] = 'S';
        g_enSymbols5[0x54] = 'T';
        g_enSymbols5[0x55] = 'U';
        g_enSymbols5[0x56] = 'V';
        g_enSymbols5[0x57] = 'W';
        g_enSymbols5[0x58] = 'X';
        g_enSymbols5[0x59] = 'Y';
        g_enSymbols5[0x5A] = 'Z';
        g_enSymbols5[0x5B] = '[';
        //g_enSymbols5[0x5C] = '';
        g_enSymbols5[0x5D] = ']';
        g_enSymbols5[0x5E] = '^';
        g_enSymbols5[0x5F] = '_';

        // small
        g_enSymbols5[0x60] = 0x005C;   // "\"
        g_enSymbols5[0x61] = 'a';
        g_enSymbols5[0x62] = 'b';
        g_enSymbols5[0x63] = 'c';
        g_enSymbols5[0x64] = 'd';
        g_enSymbols5[0x65] = 'e';
        g_enSymbols5[0x66] = 'f';
        g_enSymbols5[0x67] = 'g';
        g_enSymbols5[0x68] = 'h';
        g_enSymbols5[0x69] = 'i';
        g_enSymbols5[0x6A] = 'j';
        g_enSymbols5[0x6B] = 'k';
        g_enSymbols5[0x6C] = 'l';
        g_enSymbols5[0x6D] = 'm';
        g_enSymbols5[0x6E] = 'n';
        g_enSymbols5[0x6F] = 'o';
        g_enSymbols5[0x70] = 'p';
        g_enSymbols5[0x71] = 'q';
        g_enSymbols5[0x72] = 'r';
        g_enSymbols5[0x73] = 's';
        g_enSymbols5[0x74] = 't';
        g_enSymbols5[0x75] = 'u';
        g_enSymbols5[0x76] = 'v';
        g_enSymbols5[0x77] = 'w';
        g_enSymbols5[0x78] = 'x';
        g_enSymbols5[0x79] = 'y';
        g_enSymbols5[0x7A] = 'z';
        g_enSymbols5[0x7E] = '.';


        QList<unsigned char> keys = g_enSymbols5.keys();
        // Init english decoder map.
        foreach ( unsigned char k, keys )
        {
            d_enSymbols5[ g_enSymbols5[k] ] = k;
        }
    }

    // Init russian conversion table.
    if ( g_ruSymbols5.empty() )
    {
        // special symbol
        g_ruSymbols5[0x21] = '!';
        g_ruSymbols5[0x22] = '"';
        g_ruSymbols5[0x23] = '#';
        g_ruSymbols5[0x24] = '$';
        g_ruSymbols5[0x25] = '%';
        g_ruSymbols5[0x26] = '&';
        g_ruSymbols5[0x27] = '/';
        g_ruSymbols5[0x28] = '(';
        g_ruSymbols5[0x29] = ')';
        g_ruSymbols5[0x2A] = '*';
        g_ruSymbols5[0x2B] = '+';
        g_ruSymbols5[0x2C] = '`';
        g_ruSymbols5[0x2D] = '-';
        g_ruSymbols5[0x2E] = '.';
        g_ruSymbols5[0x2F] = '/';
        g_ruSymbols5[0x30] = '0';
        g_ruSymbols5[0x31] = '1';
        g_ruSymbols5[0x32] = '2';
        g_ruSymbols5[0x33] = '3';
        g_ruSymbols5[0x34] = '4';
        g_ruSymbols5[0x35] = '5';
        g_ruSymbols5[0x36] = '6';
        g_ruSymbols5[0x37] = '7';
        g_ruSymbols5[0x38] = '8';
        g_ruSymbols5[0x39] = '9';

        g_ruSymbols5[0x3B] = '`';
        g_ruSymbols5[0x3C] = '<';
        g_ruSymbols5[0x3D] = '=';
        g_ruSymbols5[0x3E] = '>';
        g_ruSymbols5[0x3F] = '?';

        // small
        g_ruSymbols5[0x40] = 0x044E;    // ю
        g_ruSymbols5[0x41] = 0x0430;    // а
        g_ruSymbols5[0x42] = 0x0431;    // б
        g_ruSymbols5[0x43] = 0x0446;    // ц
        g_ruSymbols5[0x44] = 0x0434;    // д
        g_ruSymbols5[0x45] = 0x0435;    // е
        g_ruSymbols5[0x46] = 0x0444;    // ф
        g_ruSymbols5[0x47] = 0x0433;    // г
        g_ruSymbols5[0x48] = 0x0445;    // х
        g_ruSymbols5[0x49] = 0x0438;    // и
        g_ruSymbols5[0x4A] = 0x0439;    // й
        g_ruSymbols5[0x4B] = 0x043A;    // к
        g_ruSymbols5[0x4C] = 0x043B;    // л
        g_ruSymbols5[0x4D] = 0x043C;    // м
        g_ruSymbols5[0x4E] = 0x043D;    // н
        g_ruSymbols5[0x4F] = 0x043E;    // о
        g_ruSymbols5[0x50] = 0x043F;    // п
        g_ruSymbols5[0x51] = 0x044F;    // я
        g_ruSymbols5[0x52] = 0x0440;    // р
        g_ruSymbols5[0x53] = 0x0441;    // с
        g_ruSymbols5[0x54] = 0x0442;    // т
        g_ruSymbols5[0x55] = 0x0443;    // у
        g_ruSymbols5[0x56] = 0x0436;    // ж
        g_ruSymbols5[0x57] = 0x0432;    // в
        g_ruSymbols5[0x58] = 0x044C;    // ь
        g_ruSymbols5[0x59] = 0x044B;    // ы
        g_ruSymbols5[0x5A] = 0x0437;    // з
        g_ruSymbols5[0x5B] = 0x0448;    // ш
        g_ruSymbols5[0x5C] = 0x044D;    // э
        g_ruSymbols5[0x5D] = 0x0449;    // щ
        g_ruSymbols5[0x5E] = 0x0447;    // ч
        g_ruSymbols5[0x5F] = '_';   // ъ?

        // big
        g_ruSymbols5[0x60] = 0x042E;    // Ю
        g_ruSymbols5[0x61] = 0x0410;    // А
        g_ruSymbols5[0x62] = 0x0411;    // Б
        g_ruSymbols5[0x63] = 0x0426;    // Ц
        g_ruSymbols5[0x64] = 0x0414;    // Д
        g_ruSymbols5[0x65] = 0x0415;    // Е
        g_ruSymbols5[0x66] = 0x0424;    // Ф
        g_ruSymbols5[0x67] = 0x0413;    // Г
        g_ruSymbols5[0x68] = 0x0425;    // Х
        g_ruSymbols5[0x69] = 0x0418;    // И
        g_ruSymbols5[0x6A] = 0x0419;    // Й
        g_ruSymbols5[0x6B] = 0x041A;    // К
        g_ruSymbols5[0x6C] = 0x041B;    // Л
        g_ruSymbols5[0x6D] = 0x041C;    // М
        g_ruSymbols5[0x6E] = 0x041D;    // Н
        g_ruSymbols5[0x6F] = 0x041E;    // О
        g_ruSymbols5[0x70] = 0x041F;    // П
        g_ruSymbols5[0x71] = 0x042F;    // Я
        g_ruSymbols5[0x72] = 0x0420;    // Р
        g_ruSymbols5[0x73] = 0x0421;    // С
        g_ruSymbols5[0x74] = 0x0422;    // Т
        g_ruSymbols5[0x75] = 0x0423;    // У
        g_ruSymbols5[0x76] = 0x0416;    // Ж
        g_ruSymbols5[0x77] = 0x0412;    // В
        g_ruSymbols5[0x78] = 0x042C;    // Ь
        g_ruSymbols5[0x79] = 0x042B;    // Ы
        g_ruSymbols5[0x7A] = 0x0417;    // З
        g_ruSymbols5[0x7B] = 0x0428;    // Ш
        g_ruSymbols5[0x7C] = 0x042D;    // Э
        g_ruSymbols5[0x7D] = 0x0429;    // Щ
        g_ruSymbols5[0x7E] = 0x0427;    // Ч
        //g_ruSymbols5[0x7F] = '_'; // Ъ ?

        QList<unsigned char> keys = g_ruSymbols5.keys();
        // Init russian decoder map.
        foreach ( unsigned char k, keys )
        {
            d_ruSymbols5[ g_ruSymbols5[k] ] = k;
        }
    }
}

MTK5Codec::~MTK5Codec()
{
}

QByteArray MTK5Codec::name() const
{
    return "MTK-5";
}

int MTK5Codec::mibEnum() const
{
    return 3;
}

QString MTK5Codec::convertToUnicode(const char *in, int length, ConverterState *state) const
{
    Q_UNUSED(state);

    QString unicodeString;
    for ( int i = 0; i < length; i++ )
    {
        unsigned char code = in[i] & 0x7F;

        // Check switching symbols.
        if ( code == MTK5_RUS )
        {
            _state5 = MTK5_RUS;
            continue;
        }
        else if ( code == MTK5_ENG )
        {
            _state5 = MTK5_ENG;
            continue;
        }

        // Check for special symbols.
        if ( code == MTK5_SPACE )
        {
            unicodeString += " ";
            continue;
        }
        else if ( code == MTK5_CR )
        {
            unicodeString += "\r";
            continue;
        }
        else if ( code == MTK5_LF )
        {
            unicodeString += "\n";
            continue;
        }
        else if ( code == MTK5_BEL )
        {
            unicodeString += "\a";
            continue;
        }
        // Convert to unicode.
        else
        {
            const TTelegraphCodecMap &convTable = _state5 == MTK5_RUS
                    ? g_ruSymbols5 : g_enSymbols5;
            unicodeString += convTable[code];
        }
    }

    return unicodeString;
}

QByteArray MTK5Codec::convertFromUnicode(const QChar *in, int length, ConverterState *state) const
{
    Q_UNUSED(state);

    unsigned char statePrev = 0x0E;

    QByteArray byteArray;
    for ( int i = 0; i < length; i++ )
    {
        QChar sym = in[i];

        unsigned char code = 0xFF;
        unsigned char state = 0x0E;

        // Check for special symbols.
        if ( sym == ' ' )
        {
            state = statePrev;
            code = MTK5_SPACE;
        }
        else if ( sym == '\n' )
        {
            state = statePrev;
            code = MTK5_LF;
        }
        else if ( sym == '\r' )
        {
            state = statePrev;
            code = MTK5_CR;
        }
        else if ( sym == '\a' )
        {
            state = statePrev;
            code = MTK5_BEL;
        }
        // Convert from unicode.
        else
        {
            if ( d_enSymbols5.contains(sym) )
            {
                code = d_enSymbols5[sym];
                state = MTK5_ENG;
            }
            else if ( d_ruSymbols5.contains(sym) )
            {
                code = d_ruSymbols5[sym];
                state = MTK5_RUS;
            }
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
