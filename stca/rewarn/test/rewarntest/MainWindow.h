#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
    class MainWindow;
}
namespace rewarn {
    class WarnNotifier;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void paintEvent ( QPaintEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );

private slots:
    void on_radioButtonPickArea_clicked();
    void on_radioButtonPickPlane_clicked();

    void on_radioButtonAreaPolygon_clicked();
    void on_radioButtonAreaCircle_clicked();

    void on_spinBoxInterval_valueChanged( int i );

    void onPlaneInsideZone(quint16 boardNumber, int typeZone, quint32 idZone);
    void onPlaneZoneOnCourse(quint16 boardNumber, int typeZone, quint32 idZone);

private:
    Ui::MainWindow *ui;
    rewarn::WarnNotifier *warnNotifier;

    QVector<QPointF>        mPickPoints;
    QPair<QPointF, QPointF> mAirPlane;

    QPointF mMousePos;

    QString mStatusMessage;
};

#endif // MAINWINDOW_H
