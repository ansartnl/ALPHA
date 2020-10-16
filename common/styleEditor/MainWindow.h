#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void updateStyle();

protected slots:
    void onStyleChanged(const QString &newStyle);
    void onPaletteChange(const QPalette &p);

    void onSave();
    void onLoad();

protected:
    QLabel *styleLabel;
    QString defaultStyle;
};

#endif // MAINWINDOW_H
