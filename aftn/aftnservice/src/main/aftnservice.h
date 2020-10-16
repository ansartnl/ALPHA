#ifndef AFTNSERVICE_H
#define AFTNSERVICE_H

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QSettings>

#include "telnetappender.h"

#include "aftnworkerbase.h"

#include "qtservice.h"

class Pulsar;
class PulsarStatus;


namespace Log4Qt
{
class PatternLayout;
class ColorConsoleLayout;
class ColorConsoleAppender;
class RollingFileAppender;
class SystemLogAppender;
class ColorHtmlLayout;
class TelnetAppender;
}

class PulsarStatus: public QObject
{
    Q_OBJECT

public:
    explicit PulsarStatus(QObject* parent = 0);

public slots:

    virtual void started();
    virtual void stopped();

    void setAftnStatusChanged(bool connected);

signals:
    void aftnStatusChanged(const QString& status);

private:
    QList<AFTNWorkerBase*> workers;
};

/**
 * @brief Crossplatform service, controlled all threads
 *
 * @class AFTNService aftnservice.h
 */
class AFTNService: public QtService<QCoreApplication> , QObject
{

    void init();
public:
    AFTNService(int argc, char** argv);
    virtual ~AFTNService();

protected:
    void start();
    void stop();
    void processCommand(int code);
    void loadConfig();

    void initLog4qt();
    void deinitLog4qt();
private:
    void loadDefaultSettings();

    // Log engine objects
    Log4Qt::PatternLayout *patternLayout;
    Log4Qt::ColorConsoleLayout *colorConsoleLayout;
    Log4Qt::ColorConsoleAppender *colorConsoleAppender;
    Log4Qt::RollingFileAppender *fileAppender;
    Log4Qt::SystemLogAppender *syslogAppender;
    Log4Qt::TelnetAppender *telnetAppender;
    Log4Qt::ColorHtmlLayout *htmlLayout;

    QSettings *settings;

    int pingTimerId;

    Pulsar *pulsar;
    PulsarStatus *status;
};
#endif // AFTNSERVICE_H
