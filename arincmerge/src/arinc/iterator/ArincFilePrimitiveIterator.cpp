#include "ArincFilePrimitiveIterator.h"
#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>
#include <QtCore/QDebug>

const int ArincFilePrimitiveIterator::PACKET_SIZE = 132;

ArincFilePrimitiveIterator::ArincFilePrimitiveIterator(const QString &filePath, QTextCodec *codec)
    : file(filePath), mLine(0), codec(codec)
{
    if (!file.open(QIODevice::ReadOnly))
        qWarning() << "Unable to open arinc file" << filePath;
    else
        next();
    mFileName = QFileInfo(file).fileName();
    mFullFileName = QFileInfo(file).filePath();
}

ArincFilePrimitiveIterator::~ArincFilePrimitiveIterator()
{}

void ArincFilePrimitiveIterator::begin()
{
    file.reset();
    mLine = 0;
    next();
}

bool ArincFilePrimitiveIterator::next()
{
    Q_FOREVER
    {
        if (file.atEnd())
        {
            buffer.clear();
            return false;
        }
        buffer = file.readLine();
        if(buffer.endsWith("\r\n"))
            buffer.chop(2);
        else if(buffer.endsWith('\n'))
            buffer.chop(1);
        ++mLine;
        if (buffer.size() != PACKET_SIZE)
            qWarning() << desctiption() << ": have invalid size (" << buffer.size() << "symbols)";
        if (buffer.size() >= PACKET_SIZE)
            break;
    }
    return true;
}

bool ArincFilePrimitiveIterator::isEnd()
{
    return file.atEnd() && buffer.isEmpty();
}

QString ArincFilePrimitiveIterator::desctiption() const
{
    return QString("arinc file '%1' line %2").arg(QFileInfo(file).absoluteFilePath()).arg(mLine);
}

char ArincFilePrimitiveIterator::operator[](int i) const
{
    return buffer[i - 1];
}

QString ArincFilePrimitiveIterator::str(int i, int j) const
{
    return codec->toUnicode(buffer.mid(i - 1, j)).trimmed();
    return QTextCodec::codecForName("Windows-1251")->toUnicode(buffer.mid(i - 1, j)).trimmed();
    return QString::fromAscii(buffer.mid(i - 1, j)).trimmed();
}

QByteArray ArincFilePrimitiveIterator::arr(int i, int j) const
{
    return buffer.mid(i - 1, j);
}
