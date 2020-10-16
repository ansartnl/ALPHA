#ifndef EXTERNPROCESSOUTPUT_H
#define EXTERNPROCESSOUTPUT_H

#include "ui_ExternProcessOutput.h"

#include <QtCore/QProcess>
#include <QtCore/QTextCodec>
#include <QtGui/QScrollBar>

/*!
  * \class ExternProcessOutput
  * \brief Диалог консольного вывода
  *
  * Данный диалог предназначен для отображения консольного вывода сторонней программы.
  */

class ExternProcessOutput : public QDialog, private Ui::ExternProcessOutput
{
    Q_OBJECT

public:
    explicit ExternProcessOutput(const QString& command, const QStringList& args = QStringList(), QWidget *parent = 0);

protected slots:
    void clicked(QAbstractButton *button);

    void finished(int exitCode, QProcess::ExitStatus exitStatus )
    {
        log->insertPlainText("--------------------\n");
        log->insertPlainText(QString("Finished. Exit code: %1, status: %2\n").arg(exitCode).arg(exitStatus));
        log->insertPlainText("--------------------\n");
        log->verticalScrollBar()->setValue(log->verticalScrollBar()->maximum());
    }

    void readyReadStandardError()
    {
        QByteArray buf = process->readAllStandardError();

#if defined(Q_OS_WIN)
        QTextCodec *codec = QTextCodec::codecForName("cp866");
        QString string = codec->toUnicode(buf);
#else
        QString string(buf);
#endif

        log->insertPlainText(string.trimmed());
        log->insertPlainText("\n");
        log->verticalScrollBar()->setValue(log->verticalScrollBar()->maximum());
    }

    void readyReadStandardOutput()
    {
        QByteArray buf = process->readAllStandardOutput();

#if defined(Q_OS_WIN)
        QTextCodec *codec = QTextCodec::codecForName("cp866");
        QString string = codec->toUnicode(buf);
#else
        QString string(buf);
#endif
        log->insertPlainText(string.trimmed());
        log->insertPlainText("\n");
        log->verticalScrollBar()->setValue(log->verticalScrollBar()->maximum());
    }

    void started()
    {
        log->insertPlainText("--------------------\n");
        log->insertPlainText("Started\n");
        log->insertPlainText("--------------------\n");
        log->insertPlainText(command);
        log->insertPlainText(" ");
        log->insertPlainText(args.join(" "));
        log->insertPlainText("\n");
        log->verticalScrollBar()->setValue(log->verticalScrollBar()->maximum());
    }

protected:
    void changeEvent(QEvent *e);

    QString command;
    QStringList args;
    QProcess *process;
};

#endif // EXTERNPROCESSOUTPUT_H
