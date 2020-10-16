#include "ExternProcessRun.h"

#include <QtGui>

ExternProcessRun::ExternProcessRun(const QString &command, const QStringList &args, QWidget *parent)
    : QDialog(parent)
    , command(command)
    , args(args)
    , process(new QProcess(this))
{
    setupUi(this);

    connect(closeButton, SIGNAL(clicked()), SLOT(accept()));

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(process, SIGNAL(started()), this, SLOT(started()));

    startString = tr("Program started");
    finishString = tr("Program finished");
    crashString = tr("Program crashed");
}

void ExternProcessRun::setStartString(const QString &str)
{
    startString = str;
}

void ExternProcessRun::setFinishString(const QString &str)
{
    finishString = str;
}

void ExternProcessRun::setCrashString(const QString &str)
{
    crashString = str;
}

int ExternProcessRun::start()
{
    process->start(command, args);
    return exec();
}

void ExternProcessRun::started()
{
    insertString(startString.trimmed());
}

void ExternProcessRun::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    switch (exitStatus)
    {
    case QProcess::NormalExit:
        insertString(finishString.trimmed());
        break;
    case QProcess::CrashExit:
        insertString(crashString.trimmed());
        break;
    }
}

void ExternProcessRun::readyReadStandardError()
{
    insertString(process->readAllStandardError());
}

void ExternProcessRun::readyReadStandardOutput()
{
    insertString(process->readAllStandardOutput());
}

void ExternProcessRun::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void ExternProcessRun::insertString(const QString &str)
{
    log->insertPlainText(str.trimmed());
    log->insertPlainText("\n");
    log->verticalScrollBar()->setValue(log->verticalScrollBar()->maximum());
}

void ExternProcessRun::insertString(const QByteArray &str)
{
#if defined(Q_OS_WIN)
    QTextCodec *codec = QTextCodec::codecForName("cp866");
    QString string = codec->toUnicode(str);
#else
    QString string(str);
#endif
    insertString(string);
}
