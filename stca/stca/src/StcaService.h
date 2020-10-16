#ifndef STCASERVICE_H
#define STCASERVICE_H

#include "qtservice.h"
#include "qtdbautoconnector.h"
#include "RedisDBHelper.h"

class Analyzer;
namespace rewarn {
    class WarnNotifier;
}
class AirplaneThread;

class StcaService : public QtService<QCoreApplication>
{
public:
    StcaService(int argc, char** argv, const QString &name);
    virtual ~StcaService();

protected:
    virtual void createApplication(int &argc, char **argv);
    virtual void start();
    virtual void stop();

private:
    /** Path to configuration file */
    QString mPathConfig;
    /** True if regular application is scheduled */
    bool    mIsConsole;

    Analyzer *analyzer;
    rewarn::WarnNotifier *warnNotifier;
    AirplaneThread *airplaneThread;

    QtDbAutoConnector mDBAutoconnector;
    RedisDBHelper     mRedisAutoconnector;
};

#endif // STCASERVICE_H
