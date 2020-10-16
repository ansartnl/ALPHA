#include "ScriptRunner.h"

#include "qcorecmdlineargs_p.h"

#include <QtCore/QTimer>

ScriptRunner::ScriptRunner(const QString &command, const QStringList &args) :
    QThread(0)
{
    moveToThread(this);
    qRegisterMetaType<QProcess::ExitStatus>("ExitStatus");
#ifdef Q_OS_WIN
    mCommand = "cmd";
    mArgs << "/C";
#else
    mCommand = "bash";
#endif
    mArgs << command << args;

    startString = tr("Started");
    finishString = tr("Finished");
    crashString = tr("Crashed");
    mLastError = tr("No error");
    isRunning = false;
    hasArgs = true;
}

ScriptRunner::ScriptRunner(const QString &command, const QString &args) :
    QThread (0)
{
    moveToThread(this);
    qRegisterMetaType<QProcess::ExitStatus>("ExitStatus");
#ifdef Q_OS_WIN
    mCommand = QString("cmd /C %1 %2").arg(command).arg(args);
#else
    mCommand = QString("bash %1 %2").arg(command).arg(args);
#endif

    startString = tr("Started");
    finishString = tr("Finished");
    crashString = tr("Crashed");
    mLastError = tr("No error");
    isRunning = false;
    hasArgs = false;
}

void ScriptRunner::setStartString(const QString &str)
{
    startString = str;
}

void ScriptRunner::setFinishString(const QString &str)
{
    finishString = str;
}

void ScriptRunner::setCrashString(const QString &str)
{
    crashString = str;
}

void ScriptRunner::stop()
{
    if (currentThread() != this)
    {
        // can't quit from another thread
        QObject::staticMetaObject.invokeMethod(this
                                               , "stop"
                                               , Qt::QueuedConnection);
    } else
    {
        quit();
    }
}

void ScriptRunner::onStarted()
{
    isRunning = true;
    insertString(startString.trimmed());
    emit scriptStarted();
}

void ScriptRunner::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    isRunning = false;
    switch (exitStatus)
    {
    case QProcess::NormalExit:
        insertString(finishString.trimmed());
        break;
    case QProcess::CrashExit:
        insertString(crashString.trimmed());
        break;
    }
    emit scriptFinished(exitCode, exitStatus);
}

void ScriptRunner::readyReadStandardError()
{
    insertString(process->readAllStandardError());
}

void ScriptRunner::readyReadStandardOutput()
{
    insertString(process->readAllStandardOutput());
}

void ScriptRunner::onError(QProcess::ProcessError err)
{
    mLastError = "";
    switch (err)
    {
    case QProcess::FailedToStart:
        mLastError = tr("The process failed to start");
        break;
    case QProcess::Crashed:
        mLastError = tr("The process crashed some time after starting successfully");
        break;
    case QProcess::Timedout:
        mLastError = tr("The last waitFor...() function timed out");
        break;
    case QProcess::WriteError:
        mLastError = tr("An error occurred when attempting to write to the process");
        break;
    case QProcess::ReadError:
        mLastError = tr("An error occurred when attempting to read from the process");
        break;
    case QProcess::UnknownError:
        mLastError = tr("An unknown error occurred");
        break;
    }
    insertString(mLastError);
    emit error(mLastError);
}

void ScriptRunner::run()
{
    process = new QProcess;

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus))
            , SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(process, SIGNAL(readyReadStandardError()), SLOT(readyReadStandardError()));
    connect(process, SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));
    connect(process, SIGNAL(started()), SLOT(onStarted()));
    connect(process, SIGNAL(error(QProcess::ProcessError)), SLOT(onError(QProcess::ProcessError)));

    connect(this, SIGNAL(scriptFinished(int,QProcess::ExitStatus)), SLOT(quit()));

    if (hasArgs)
    {
        process->start(mCommand, mArgs);
    } else
    {
#ifdef Q_OS_WIN
        QString arguments(mCommand.data(), mCommand.length());
        QStringList al = qWinCmdArgs(arguments);
        QString command = al[0];
        al.removeAt(0);
        process->start(command, al);
#else
        process->start(mCommand);
#endif

    }

    exec();

    disconnect(this, SIGNAL(scriptFinished(int,QProcess::ExitStatus)), this, SLOT(quit()));

    if (isRunning)
    {
        process->terminate();
        if (!process->waitForFinished(5000))
        {
            insertString(crashString);
            process->kill();
        }
        isRunning = false;
    }
    disconnect(process);
    process->deleteLater();
}

void ScriptRunner::insertString(const QString &str)
{
    mResult.append(str.trimmed());
    emit outputString(str);
}

void ScriptRunner::insertString(const QByteArray &str)
{
#if defined(Q_OS_WIN)
    QTextCodec *codec = QTextCodec::codecForName("cp866");
    QString string = codec->toUnicode(str);
#else
    QString string(str);
#endif
    insertString(string);
}
