#ifndef OLDIWORKER_H
#define OLDIWORKER_H

#include "aftnworkerbase.h"

#include "OLDIMessages.h"

#include <QtCore/QSharedPointer>

class OldiWorker: public AFTNWorkerBase
{
    Q_OBJECT
public:
    explicit OldiWorker(QObject* parent = 0);
    virtual void run();
};

class OLDIProc;
class OldiWorkerBase : public WorkerBase
{
    Q_OBJECT
public:
    OldiWorkerBase(QObject* parent = 0);

    bool start();
    void stop();
    bool isDisabled() const { return disable; }

protected slots:
    void checkFplToSend();
    void oldiMessageReceived(QSharedPointer<OLDIMessage>);

    void onOldiMessageSended(const OLDIMessage* message, const QStringList& errors);
    void messageTimeOut(const OLDIMessage*);

    void onTimerClearData();

private:
    void defaultSettings();

private:
    QString owner;
    int timeout;
    OLDIProc *pOLDIProc;
    bool bAppend_ENTRY_FIR_EET;
    bool disable;
    static const QString sSIMLName;
};

#endif // OLDIWORKER_H
