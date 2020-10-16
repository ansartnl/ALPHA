#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include <QtCore/QProcess>
#include <QtCore/QTextCodec>
#include <QtCore/QThread>
#include <QtCore/QVariant>

/*!
  * \class ScriptRunner
  * \brief Запуск консольных скриптов
  *
  * Данный класс предназначен для запуска консольных скриптов и возврата строки вывода.
  */

class ScriptRunner : public QThread
{
    Q_OBJECT

public:
    explicit ScriptRunner(const QString &command, const QStringList &args = QStringList());
    explicit ScriptRunner(const QString &command, const QString &args);

    void setStartString(const QString &str);
    void setFinishString(const QString &str);
    void setCrashString(const QString &str);

    inline QStringList outputList() const { return mResult; }
    inline QString outputString() const { return mResult.join("\n"); }

    inline QString lastError() const { return mLastError; }

public slots:
    void stop();

signals:
    void outputString(const QString &str);
    void scriptStarted();
    void scriptFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void error(const QString &str);

protected slots:
    void onStarted();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError();
    void readyReadStandardOutput();
    void onError(QProcess::ProcessError err);

protected:
    void run();
    void insertString(const QString &str);
    void insertString(const QByteArray &str);

    QString mCommand;
    bool hasArgs;
    QStringList mArgs;
    QProcess *process;
    QString startString;
    QString finishString;
    QString crashString;
    QStringList mResult;
    QString mLastError;
    bool isRunning;
};

Q_DECLARE_METATYPE(QProcess::ExitStatus)

#endif // SCRIPTRUNNER_H
