#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class PulsReceiver;

class QTimer;

class QPlainTextEdit;
class QStackedLayout;

class QHostAddress;
class QTcpSocket;
class QHostInfo;

#include "Pulsar.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

    void showEvent(QShowEvent *e);

protected slots:

    void pulsarChanged(const QHostAddress& pulsar, const QString& data, const QHostAddress& oldpulsar);
    void status(const QHostAddress& pulsar, const QString& status);

    void on_serversView_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

    void on_about_triggered();
    void on_aboutQt_triggered();

    void on_activate_clicked();

    void timeout();

    void telnetReadyRead();

    void on_save_clicked();
    void on_clear_clicked();

    void hostFounded(const QHostInfo& info);

    // -----------------------------------

    void wsStatus(const QHostAddress& pulsar, const QString& data);
    void on_wsView_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

    void wsTimeout();

    void wsTelnetReadyRead();

    void on_wsSave_clicked();
    void on_wsClear_clicked();


    void on_vsplitter1_splitterMoved(int pos, int index);
    void on_vsplitter2_splitterMoved(int pos, int index);


protected:

    PulsReceiver *receiver;

    QMap<QTimer*, QListWidgetItem*> hosts;
    QMap<QListWidgetItem*, QTcpSocket*> telnets;
    QMap<QListWidgetItem*, QPlainTextEdit*> logs;

    QStackedLayout *logLayout;

    PulsReceiver *wsReceiver;

    QMap<QTimer*, QListWidgetItem*> wsHosts;
    QMap<QListWidgetItem*, QTcpSocket*> wsTelnets;
    QMap<QListWidgetItem*, QPlainTextEdit*> wsLogs;

    QStackedLayout *wsLogLayout;
};

#endif // MAINWINDOW_H
