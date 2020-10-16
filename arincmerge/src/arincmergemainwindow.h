#ifndef ARINGMERGEMAINWINDOW_H
#define ARINGMERGEMAINWINDOW_H

#include "ui_arincmergemainwindow.h"

#include "ArincStatusStorage.h"

#include <QLabel>
#include <QMainWindow>

class ArincMergeMainWindow : public QMainWindow, public Ui::ArincMergeMainWindow
{
    Q_OBJECT
public:
    ArincMergeMainWindow(QWidget *parent = 0);
    virtual ~ArincMergeMainWindow();

signals:
    void firChanged(int id, const QString &name);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent* pEvent);

private:
    template<class T> void showEditor();

    void saveSettings();
    void loadSettings();

    QScopedPointer<ArincStatusStorage> arinc;
    QLabel *labelCurrentArincFile;

    int mCurrentFirId;
    QString mCurrentFirName;

private slots:
    void closeMdiTab(int index);

    void on_editFir_triggered();
    void on_editSector_triggered();
    void on_editAirport_triggered();
    void on_editHolding_triggered();
    void on_editPoint_triggered();
    void on_editRestrict_triggered();
    void on_editRestrictUser_triggered();
    void on_editRoute_triggered();
    void on_editSsa_triggered();
    void on_importArinc_triggered();
    void on_actionAbout_triggered();

    void importFir();
    void releseArinc();
    void arincChanges();
    void onSynchronize();

    void onChangeCurrentFir();
};

#endif // ARINGMERGEMAINWINDOW_H
