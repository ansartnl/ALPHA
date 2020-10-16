#ifndef EXTERNPROCESSRUN_H
#define EXTERNPROCESSRUN_H

#include "ui_ExternProcessRun.h"

#include <QtCore/QProcess>
#include <QtCore/QTextCodec>
#include <QtGui/QScrollBar>

/*!
  * \class ExternProcessRun
  * \brief Диалог запуска программ
  *
  * Данный диалог предназначен для запуска сторонней программы и контроля ее исполнения.
  */

class ExternProcessRun : public QDialog, private Ui::ExternProcessRun
{
    Q_OBJECT

public:
    explicit ExternProcessRun(const QString& command, const QStringList& args = QStringList(), QWidget *parent = 0);

    void setStartString(const QString &str);
    void setFinishString(const QString &str);
    void setCrashString(const QString &str);

public slots:
    int start();

protected slots:
    void started();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError();
    void readyReadStandardOutput();

protected:
    void changeEvent(QEvent *e);
    void insertString(const QString &str);
    void insertString(const QByteArray &str);

    QString command;
    QStringList args;
    QProcess *process;
    QString startString;
    QString finishString;
    QString crashString;
};

#endif // EXTERNPROCESSOUTPUT_H
