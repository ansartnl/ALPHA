#include "restarter.h"
#include "aftnworkerbase.h"

#include <QtCore/QtDebug>
#include <QtCore/QProcess>
#include <QtCore/QTimerEvent>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

////////////////////////////////////////////////////////////////////////////////
// Restarter thread implementation.

Restarter::Restarter(QObject* parent)
    : QThread(parent), mTimerInterval(30)
{
    if ( objectName().isEmpty() )
        setObjectName("restarter");
}

void Restarter::setInterval(int interval)
{
    mTimerInterval = interval < 10 ? 10 : interval;
}

void Restarter::run()
{
    qloginfo("Restarter process started ...");

    RestarterObject restarterObject(this);
    restarterObject.start(mTimerInterval);

    exec();

    restarterObject.stop();

    qloginfo("Stop restarter process");
}


////////////////////////////////////////////////////////////////////////////////
// RestarterObject implementation.

RestarterObject::RestarterObject(Restarter *restarter, QObject *parent)
    : QObject(parent), mRestarter(restarter), mTimerId(-1)
{
    Q_ASSERT(mRestarter);
}

void RestarterObject::start(int interval)
{
    if ( mTimerId == -1 )
        mTimerId = startTimer(interval *1000);
}

void RestarterObject::stop()
{
    if ( mTimerId != -1 )
        killTimer(mTimerId);
}

void RestarterObject::timerEvent(QTimerEvent * event)
{
    if ( event->timerId() == mTimerId )
    {
        bool isRestartRequired = false;

        foreach ( AFTNWorkerBase* w, mRestarter->mWorkers )
        {
            // Do not process disabled workers.
            if ( !w->isRunning() )
                continue;

            if ( w->isActive() )
                w->setActive(false);
            else
            {
                qloginfo( tr("Worker '%1' does not reply (possibly hangs)").arg(w->objectName()) );
                isRestartRequired = true;
                break;
            }
        }

        // Check existence of 'rrr' file for 'test' purpose in order to do manual restart.
        QString pathRRRFile = QDir(qApp->applicationDirPath()).absoluteFilePath("rrr");
        bool isRRRFileExists = QFileInfo(pathRRRFile).exists();
        if ( isRRRFileExists )
        {
            qloginfo( tr("Manual restart requested ('rrr' file found). File path: '%1'").arg(pathRRRFile) );
            QFile::remove(pathRRRFile);
        }

        if ( isRestartRequired || isRRRFileExists )
        {
            qloginfo( tr("Restart is about to begin as one of the workers does not reply (possibly hangs)") );
#ifdef Q_OS_LINUX
            if ( !QProcess::startDetached( "bash", QStringList() << "-c" << mRestarter->mScriptPath ) )
#else
            if ( !QProcess::startDetached( mRestarter->mScriptPath ) )
#endif
                qlogwarn( tr("Restart process did not start. Please check config for 'restarter/script_path' value ('%1')").arg(mRestarter->mScriptPath) );

        }
    }

    QObject::timerEvent(event);
}
