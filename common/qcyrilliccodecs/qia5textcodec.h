#ifndef QIA5TEXTCODEC_H
#define QIA5TEXTCODEC_H

#include <QtCore/QTextCodec>

class QKOI7TextCodec :public QTextCodec {
public:
  QKOI7TextCodec();

  virtual QByteArray name() const;
  virtual QList<QByteArray> aliases() const;
  virtual int mibEnum() const;

protected:
  virtual QString convertToUnicode(const char *in, int length, ConverterState *state) const;
  virtual QByteArray convertFromUnicode(const QChar *in, int length, ConverterState *state) const;

};

#endif
