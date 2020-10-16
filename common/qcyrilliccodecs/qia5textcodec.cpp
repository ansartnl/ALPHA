#include "qia5textcodec.h"

#define IsLatin(c)        ((c) <= 0x7F)


QKOI7TextCodec::QKOI7TextCodec()
  :QTextCodec()
{

}

QByteArray QKOI7TextCodec::name() const
{
  return QByteArray("KOI7-switched");
}

QList<QByteArray> QKOI7TextCodec::aliases() const
{
  return QList<QByteArray>();
}

int QKOI7TextCodec::mibEnum() const
{
  return 2105;
}

QString QKOI7TextCodec::convertToUnicode(const char *in, int length, ConverterState *state) const
{
  QString result;
  unsigned short currentShort(0);
  unsigned char currentChar(0);
  for (int i = 0; i < length; ++i) {
    currentChar = in[i];

    currentChar &= ~0x80;
    switch (currentChar) {
    case 0x60:
      currentShort = 0x042E;
      break;
    case 0x61:
      currentShort = 0x0410;
      break;
    case 0x62:
      currentShort = 0x0411;
      break;
    case 0x63:
      currentShort = 0x0426;
      break;
    case 0x64:
      currentShort = 0x0414;
      break;
    case 0x65:
      currentShort = 0x0415;
      break;
    case 0x66:
      currentShort = 0x0424;
      break;
    case 0x67:
      currentShort = 0x0413;
      break;
    case 0x68:
      currentShort = 0x0425;
      break;
    case 0x69:
      currentShort = 0x0418;
      break;
    case 0x6A:
      currentShort = 0x0419;
      break;
    case 0x6B:
      currentShort = 0x041A;
      break;
    case 0x6C:
      currentShort = 0x041B;
      break;
    case 0x6D:
      currentShort = 0x041C;
      break;
    case 0x6E:
      currentShort = 0x041D;
      break;
    case 0x6F:
      currentShort = 0x041E;
      break;
    case 0x70:
      currentShort = 0x041F;
      break;
    case 0x71:
      currentShort = 0x042F;
      break;
    case 0x72:
      currentShort = 0x0420;
      break;
    case 0x73:
      currentShort = 0x0421;
      break;
    case 0x74:
      currentShort = 0x0422;
      break;
    case 0x75:
      currentShort = 0x0423;
      break;
    case 0x76:
      currentShort = 0x0416;
      break;
    case 0x77:
      currentShort = 0x0412;
      break;
    case 0x78:
      currentShort = 0x042C;
      break;
    case 0x79:
      currentShort = 0x042B;
      break;
    case 0x7A:
      currentShort = 0x0417;
      break;
    case 0x7B:
      currentShort = 0x0428;
      break;
    case 0x7C:
      currentShort = 0x042D;
      break;
    case 0x7D:
      currentShort = 0x0429;
      break;
    case 0x7E:
      currentShort = 0x0427;
      break;
    case 0x7F:
      break;
    default:
      currentShort = (0x0000) | currentChar;
      break;
    }

    result.append(QChar(currentShort));
  }
  return result;
}

QByteArray QKOI7TextCodec::convertFromUnicode(const QChar *in, int length, ConverterState *state) const
{
  QByteArray result;
  unsigned short currentShort(0);
  unsigned char resultChar(0);
  for (int i = 0; i < length; ++i) {
    currentShort = in[i].unicode();
    if (currentShort < 0x60) {
      resultChar = currentShort;
    } else {
      switch (currentShort) {
      case 0x042E:
        resultChar = 0x60;
        break;
      case 0x0410:
        resultChar = 0x61;
        break;
      case 0x0411:
        resultChar = 0x62;
        break;
      case 0x0426:
        resultChar = 0x63;
        break;
      case 0x0414:
        resultChar = 0x64;
        break;
      case 0x0415:
        resultChar = 0x65;
        break;
      case 0x0424:
        resultChar = 0x66;
        break;
      case 0x0413:
        resultChar = 0x67;
        break;
      case 0x0425:
        resultChar = 0x68;
        break;
      case 0x0418:
        resultChar = 0x69;
        break;
      case 0x0419:
        resultChar = 0x6A;
        break;
      case 0x041A:
        resultChar = 0x6B;
        break;
      case 0x041B:
        resultChar = 0x6C;
        break;
      case 0x041C:
        resultChar = 0x6D;
        break;
      case 0x041D:
        resultChar = 0x6E;
        break;
      case 0x041E:
        resultChar = 0x6F;
        break;
      case 0x041F:
        resultChar = 0x70;
        break;
      case 0x042F:
        resultChar = 0x71;
        break;
      case 0x0420:
        resultChar = 0x72;
        break;
      case 0x0421:
        resultChar = 0x73;
        break;
      case 0x0422:
        resultChar = 0x74;
        break;
      case 0x0423:
        resultChar = 0x75;
        break;
      case 0x0416:
        resultChar = 0x76;
        break;
      case 0x0412:
        resultChar = 0x77;
        break;
      case 0x042C:
        resultChar = 0x78;
        break;
      case 0x042B:
        resultChar = 0x79;
        break;
      case 0x0417:
        resultChar = 0x7A;
        break;
      case 0x0428:
        resultChar = 0x7B;
        break;
      case 0x042D:
        resultChar = 0x7C;
        break;
      case 0x0429 :
        resultChar = 0x7D;
        break;
      case 0x0427:
        resultChar = 0x7E;
        break;
      case 0x7F:
        resultChar = 0x7F;
        break;
      default:
        resultChar = currentShort;
        break;
      }
    }
    result.append(resultChar);
  }
  result.append("\0", 1);
  return result;
}


