#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include "main.h"

#include "models/FileListModel.h"
#include "models/FileListFilterModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Init();    
    
public slots:
    void onFileDirectory(bool exists);
    void onFlashDirectory(bool exists);
    void onScriptPath(bool exists);

private slots:
    void onBtnRun();
    void onBtnCopy();
    void onBtnRename();
    void onBtnDelete();
    void onBtnRefresh();
    void onBtnClose();

private:
    void Translate();

    Ui::MainWindow *ui;

    CFileListModel mModel;

    QString mLang;
    QString mFileDir;
    QString mFlashDir;
    QString mScript;
    QString mMask;
};

#endif // MAINWINDOW_H
