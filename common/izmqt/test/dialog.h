#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonSend_clicked();

    void onPatternToggled(bool);

    void onConnected();

    void onAutoConnected();
    void onAutoDisconnected();

    void onMessageReceived(QVariant);
    void onMessageReceived(QVariant, QString);
    void onMessageReceived(QString, QVariant);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
