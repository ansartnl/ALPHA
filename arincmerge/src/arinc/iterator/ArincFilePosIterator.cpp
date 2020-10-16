#include "ArincFilePosIterator.h"
#include <QtCore/QFileInfo>

ArincFilePosIterator::ArincFilePosIterator(const QString &filePath, QTextCodec *codec, QObject *parent): QObject(parent), ArincFilePrimitiveIterator(filePath, codec)
{}

ArincFilePosIterator::~ArincFilePosIterator()
{}

bool ArincFilePosIterator::next()
{
    bool result = ArincFilePrimitiveIterator::next();
    if (result)
        emit posChanged(file.pos());
    return result;
}

int ArincFilePosIterator::size() const
{
    return file.size();
}

int ArincFilePosIterator::pos() const
{
    return file.pos();
}
