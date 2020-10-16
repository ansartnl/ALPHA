#ifndef ARINCFILEPRIMITIVEITERATOR_H
#define ARINCFILEPRIMITIVEITERATOR_H

#include <QtCore/QFile>
#include <QtCore/QTextCodec>

class ArincFilePrimitiveIterator
{
public:
    explicit ArincFilePrimitiveIterator(const QString& filePath, QTextCodec* codec = QTextCodec::codecForName("UTF-8")); // \param filePath the name of a file
    virtual ~ArincFilePrimitiveIterator();
    virtual void begin(); // Reset iterator
    virtual bool next(); // Select next record
    virtual bool isEnd(); // Check the end of arinc file

    static const int PACKET_SIZE;

    virtual char operator[](int i) const;
    virtual QString str(int i, int j) const;
    virtual QByteArray arr(int i, int j = 1) const;

    virtual QString desctiption() const;

    inline quint32 line() const { return mLine; }

    inline QString fileName() const { return mFileName; }
    inline QString fullFileName() const { return mFullFileName; }

protected:
    QFile file;
    quint32 mLine;
    QByteArray buffer;
    QString mFileName;
    QString mFullFileName;
    QTextCodec *codec;
};

#endif // ARINCFILEPRIMITIVEITERATOR_H
