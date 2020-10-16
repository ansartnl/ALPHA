#include "ArincParserThread.h"
#include "ArincFilePrimitiveIterator.h"
#include "ArincEmptyStorage.h"
#include "ArincParser.h"

ArincParserThread::ArincParserThread(QObject *parent) : QThread(parent), bCancel(false), it(0), st(0)
{}

ArincParserThread::~ArincParserThread()
{
    if (isRunning())
    {
        cancel();
        wait();
    }
}

void ArincParserThread::cancel()
{
    bCancel = true;
}

bool ArincParserThread::startParser(ArincFilePrimitiveIterator *iterator, ArincEmptyStorage *storage)
{
    if (isRunning())
        return false;

    it = iterator;
    st = storage;
    bCancel = false;

    start();
    return true;
}

void ArincParserThread::run()
{
    if (!it || !st)
        return;

    while (!it->isEnd() && !bCancel)
        ArincParser::parse(*it, *st);
}
