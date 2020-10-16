#ifndef QMTK5TEXTCODEC_H
#define QMTK5TEXTCODEC_H

#include <QtCore/QTextCodec>

//! Implementation of 'MTK-5' telegraph codec.
class MTK5Codec : public QTextCodec
{
public:
    MTK5Codec();
    virtual ~MTK5Codec();

    virtual QByteArray name() const;
    virtual int mibEnum() const;

protected:
    virtual QString convertToUnicode(const char *in, int length, ConverterState *state) const;
    virtual QByteArray convertFromUnicode(const QChar *in, int length, ConverterState *state) const;

private:
    /** One of 3 telegraph states */
    mutable char    _state;
    mutable char    _state5;
};

#endif // QMTK5TEXTCODEC_H
