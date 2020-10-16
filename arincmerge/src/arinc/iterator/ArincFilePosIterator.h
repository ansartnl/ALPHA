#ifndef ARINCFILEPOSITERATOR_H
#define ARINCFILEPOSITERATOR_H

#include "ArincFilePrimitiveIterator.h"
#include <QtCore/QObject>

class ArincFilePosIterator : public QObject, public ArincFilePrimitiveIterator
{
    Q_OBJECT
public:
    explicit ArincFilePosIterator(const QString& filePath, QTextCodec* codec = QTextCodec::codecForName("UTF-8"), QObject *parent = 0);
    virtual ~ArincFilePosIterator();
    virtual bool next();
    virtual int size() const;
    virtual int pos() const;

signals:
    void posChanged(int pos);
};

#endif // ARINCFILEPOSITERATOR_H
