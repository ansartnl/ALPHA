#ifndef QMTK2TEXTCODEC_H
#define QMTK2TEXTCODEC_H

#include <QtCore/QTextCodec>

//! Implementation of 'MTK-2' telegraph codec.
class MTK2Codec : public QTextCodec
{
public:
    MTK2Codec();
    virtual ~MTK2Codec();

    virtual QByteArray name() const;
    virtual int mibEnum() const;

protected:
    virtual QString convertToUnicode(const char *in, int length, ConverterState *state) const;
    virtual QByteArray convertFromUnicode(const QChar *in, int length, ConverterState *state) const;

private:
    /** One of 3 telegraph states */
    mutable char    _state;
};

#endif // QMTK2TEXTCODEC_H
