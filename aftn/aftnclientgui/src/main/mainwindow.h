#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QtCore/QFile>
#include <QtCore/QTimer>

#include <QtGui/QMainWindow>

#include "aftnclient.h"

class QSettings;

class MainWindow: public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT
public:
  MainWindow (QWidget * parent = 0, Qt::WindowFlags flags = 0 );
  virtual ~MainWindow();

protected:
  void customEvent(QEvent *event);
private:
  void refreshUi();
  void loadSettingsFromUi();

  AFTNClient *client;

  QScopedPointer<QFile> rawdata;
  QScopedPointer<QFile> logdata;

  QTimer timer;

private slots:
  void on_toolButton_2_clicked();
  void on_interrupt_clicked();
  void on_connectBtn_clicked(bool);
  void on_toolButton_clicked();

  void clientRawMessage(const QByteArray& message);

  void clientAftnStateChanged(AFTNClient::AftnState state);

  void connectTimer();
};

#endif //#ifndef MAINWINDOW_H
