#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H

#include "aftnworkerbase.h"
#include "aftnclient.h"

class AFTNMessageProcessor;
class NetworkWorkerBase;
class QSettings;
class QITA2TextCodec;

namespace Log4Qt {
  class TelnetAppender;
}
class NetworkWorker: public AFTNWorkerBase
{
    Q_OBJECT
public:
  explicit NetworkWorker(QObject* parent = 0);
  virtual ~NetworkWorker();
  virtual void run();

signals:
    void aftnStatusChanged(bool connected);
};

/* --------------------------------------------------
 * Internal useonly. We create this class, because we need signals,slots,events for timer and tcpsocket in thread.
 */
class NetworkWorkerBase: public WorkerBase
{
Q_OBJECT
public:
  NetworkWorkerBase(QObject* parent = 0);
  virtual ~NetworkWorkerBase();

  bool start();
  void stop();

signals:
  void aftnStatusChanged(bool connected);

public slots:
  void processMessages(const QStringList& messages);
  void clientStateChanged(AFTNClient::AftnState state);

protected:
  void loadSettings();
  void initDefaultSettings();

  virtual void timerEvent(QTimerEvent *event);

private:
  int timerId;
  int interval;

  QString mHost1;
  QString mHost2;
  int mPort1;
  int mPort2;

  QString codecName;
  QTextCodec* codec;
  QScopedPointer<QITA2TextCodec> ita2codec;

  QByteArray mAftnName;
  int mPingTimeout;

  AFTNClient *mClient;
  AFTNMessageProcessor *mProcessor;
  Log4Qt::TelnetAppender *telnetAppender;
};
#endif //#ifndef NETWORKWORKER_H
