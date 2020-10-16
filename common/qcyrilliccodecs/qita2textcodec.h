#ifndef QTIA2TEXTCODEC_H
#define QTIA2TEXTCODEC_H

#include <QtCore/QTextCodec>

/*
 * Text codec for ITA5 5-bit encoding
 * \warning NULL byte interpret as switch to latin. It's create for russian!@#$%.
 */
class QITA2TextCodec : public QTextCodec {

public:
  QITA2TextCodec();

  virtual QByteArray name() const;
  virtual QList<QByteArray> aliases() const;
  virtual int mibEnum() const;

protected:

  virtual QString convertToUnicode(const char *in, int length, ConverterState *state) const;
  virtual QByteArray convertFromUnicode(const QChar *in, int length, ConverterState *state) const;
};

#endif
