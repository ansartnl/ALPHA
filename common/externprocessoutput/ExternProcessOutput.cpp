#include "ExternProcessOutput.h"

#include <QtGui>

ExternProcessOutput::ExternProcessOutput(const QString &command, const QStringList &args, QWidget *parent)
    : QDialog(parent)
    , command(command)
    , args(args)
    , process(new QProcess(this))
{
    setupUi(this);

    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(process, SIGNAL(started()), this, SLOT(started()));

    process->start(command, args);
}

void ExternProcessOutput::changeEvent(QEvent *e)
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

void ExternProcessOutput::clicked(QAbstractButton *button)
{
    QAbstractButton *b = buttonBox->button(QDialogButtonBox::Retry);
    QAbstractButton *ok= buttonBox->button(QDialogButtonBox::Cancel);
    if (button == b)
        process->start(command, args);
    else if (button == ok)
        accept();
}
