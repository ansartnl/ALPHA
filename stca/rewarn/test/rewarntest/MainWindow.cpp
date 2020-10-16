#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "WarnNotifier.h"
#include "orm/ReFir.h"
#include "cs/CoordSystemStereo.h"

#include <QPainter>
#include <QPolygonF>
#include <QMouseEvent>
#include <QMessageBox>

// Area shape - polygon
static QPolygonF gPolygon;
static QPolygonF gPolygonModel;
// Area shape - circle
static QPointF gCenterCircle(-1., -1.);
static double gRadiusCircle = 0;
// Center of screen
static QPointF gScreenCenter(500., 500.);

static TCoordSystem gCoordSys;

static double latc = 54, lonc = 28;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    warnNotifier(0)
{
    ui->setupUi(this);
    setMouseTracking(true);

    mAirPlane = QPair<QPointF, QPointF>(QPointF(-1., -1.), QPointF(-1., -1.));

    warnNotifier = new rewarn::WarnNotifier();
    warnNotifier->defineCSCenterPoint(latc, lonc);

    gCoordSys = TCoordSystem(new CCoordSystemStereo());
    CCoordSystemStereo *cs = static_cast<CCoordSystemStereo *>(gCoordSys.data());
    cs->defineCenterPoint(latc, lonc);

    // Signals retranslation.
    connect(warnNotifier, SIGNAL(signalInsideZone(quint16, int, quint32)),
            SLOT(onPlaneInsideZone(quint16, int, quint32)));
    connect(warnNotifier, SIGNAL(signalZoneOnCourse(quint16, int, quint32)),
            SLOT(onPlaneZoneOnCourse(quint16, int, quint32)));
}

MainWindow::~MainWindow()
{
    delete warnNotifier;
    delete ui;
}

void MainWindow::paintEvent ( QPaintEvent * event )
{
    QPainter p(this);

    int radiusPoint = 1;
    if ( ui->radioButtonPickArea->isChecked() )
        radiusPoint = 2;

    p.drawText(10, 120, mStatusMessage);
    p.drawText(10, 130, QString("x: %1  y: %2").arg(mMousePos.x()).arg(mMousePos.y()));

    p.setPen(Qt::blue);
    p.setBrush(Qt::blue);

    foreach ( const QPointF &pnt, mPickPoints )
    {
        p.drawEllipse(QPointF(pnt.x(), pnt.y()), radiusPoint, radiusPoint);
    }

    p.setBrush(Qt::NoBrush);

    // Draw shape
    {
        // Polygon
        if ( ui->radioButtonAreaPolygon->isChecked() )
        {
            if ( gPolygonModel.size() > 2 )
                p.drawPolygon( gPolygonModel );
        }
        // Circle
        else
        {
            if ( gRadiusCircle > 0. )
            {
                QVector3D vecModel = gCoordSys->toModelDegree( QVector3D(gCenterCircle.x(), gCenterCircle.y(), 0.) );
                p.drawEllipse(QPointF(vecModel.x(), vecModel.y()), gRadiusCircle, gRadiusCircle);
            }
        }
    }

    p.setPen(Qt::red);
    p.setBrush(Qt::red);

    // Draw airplane
    if ( ui->radioButtonPickPlane->isChecked() && ui->spinBoxInterval->value() > 0 )
    {
        if ( mAirPlane.first.x() > -1. )
        {
            QVector3D vecModel = gCoordSys->toModelDegree( QVector3D(mAirPlane.first.x(), mAirPlane.first.y(), 0.) );
            p.drawEllipse(QPointF(vecModel.x(),vecModel.y()), radiusPoint+2, radiusPoint+2);
        }
        if ( mAirPlane.second.x() > -1. )
        {
            QVector3D vecModel1 = gCoordSys->toModelDegree( QVector3D(mAirPlane.first.x(), mAirPlane.first.y(), 0.) );
            QVector3D vecModel2 = gCoordSys->toModelDegree( QVector3D(mAirPlane.second.x(), mAirPlane.second.y(), 0.) );
            QPointF pnt1(vecModel1.x(), vecModel1.y());
            QPointF pnt2(vecModel2.x(), vecModel2.y());
            p.drawEllipse(pnt2, radiusPoint, radiusPoint);

            QLineF unitVel = QLineF(pnt1, pnt2).unitVector();
            QPointF unitPoint(unitVel.dx(), unitVel.dy());
            QPointF p2 = pnt1 + unitPoint * (5.*ui->spinBoxInterval->value());

            p.drawLine(pnt1, p2);
        }
    }
}

void MainWindow::mousePressEvent ( QMouseEvent * event )
{
    // Add point or plane
    if ( event->button() == Qt::LeftButton )
    {
        // Pick area
        if ( ui->radioButtonPickArea->isChecked() )
        {
            mPickPoints.push_back(event->pos());

            if ( ui->radioButtonAreaPolygon->isChecked() )
            {
                QPointF pos = event->pos();
                QVector3D vec = gCoordSys->toGeoDegree( QVector3D(pos.x(), pos.y(), 0.) );
                gPolygon << QPointF(vec.x(), vec.y());
                QVector3D vecModel = gCoordSys->toModelDegree( QVector3D(vec.x(), vec.y(), 0.) );
                gPolygonModel << QPointF(vecModel.x(), vecModel.y());
            }
            else
            {
                if ( mPickPoints.size() > 1 )
                {
                    int i0 = mPickPoints.size()-2,
                        i1 = mPickPoints.size()-1;
                    gCenterCircle = mPickPoints[i0] + (mPickPoints[i1] - mPickPoints[i0]) / 2.;
                    gRadiusCircle = QLineF(mPickPoints[i1], mPickPoints[i0]).length() / 2.;

                    QVector3D vec = gCoordSys->toGeoDegree( QVector3D(gCenterCircle.x(), gCenterCircle.y(), 0.) );
                    gCenterCircle = QPointF(vec.x(), vec.y());
                }
            }
        }
        // Pick airplane
        else
        {
            mStatusMessage = QString();

            if ( mAirPlane.first.x() < 0. )
            {
                mAirPlane.first = event->pos();
                QVector3D vec = gCoordSys->toGeoDegree( QVector3D(mAirPlane.first.x(), mAirPlane.first.y(), 0.) );
                mAirPlane.first = QPointF(vec.x(), vec.y());
            }
            else
            {
                mAirPlane.second = event->pos();
                QVector3D vec = gCoordSys->toGeoDegree( QVector3D(mAirPlane.second.x(), mAirPlane.second.y(), 0.) );
                mAirPlane.second = QPointF(vec.x(), vec.y());

                QVector3D vec1 = gCoordSys->toModelDegree( QVector3D(mAirPlane.first.x(), mAirPlane.first.y(), 0.) );
                QVector3D vec2 = gCoordSys->toModelDegree( QVector3D(mAirPlane.second.x(), mAirPlane.second.y(), 0.) );
                QLineF unitVel = QLineF(QPointF(vec1.x(), vec1.y()), QPointF(vec2.x(), vec2.y())).unitVector();

                rewarn::AirplaneInfo airInfo;
                airInfo.x = mAirPlane.first.x();
                airInfo.y = mAirPlane.first.y();
                airInfo.height = 0.;

                airInfo.dx = unitVel.dx()*5.;
                airInfo.dy = unitVel.dy()*5.;
                airInfo.dz = 0.;

                warnNotifier->addAirplane(airInfo);
            }
        }

        update();
    }
    // Clear
    else if ( event->button() == Qt::RightButton )
    {
        if ( ui->radioButtonPickArea->isChecked() )
        {
            mPickPoints.clear();
            gPolygon.clear();
            gPolygonModel.clear();
            gCenterCircle = QPointF(-1., -1.);
            gRadiusCircle = 0;
        }
        else
        {
            mAirPlane.first = QPointF(-1., -1.);
            mAirPlane.second = QPointF(-1., -1.);
        }

        update();
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent ( QMouseEvent * event )
{
    mMousePos = event->pos();
    update();

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::on_radioButtonPickArea_clicked()
{
    mAirPlane.first = QPointF(-1., -1.);
    mAirPlane.second = QPointF(-1., -1.);
    on_radioButtonAreaPolygon_clicked();

    warnNotifier->stop();
}

void MainWindow::on_radioButtonPickPlane_clicked()
{
    warnNotifier->removeAirplanes();

    using namespace rewarn;

    ReFir fir;
    fir.mCenterLatitude = latc;
    fir.mCenterLongitude = lonc;
    fir.mAreas.append(ReArea());
    ReArea &area = fir.mAreas.back();
    area.mType = enDanger;
    area.mID = 20;

    area.mHeightMin = ui->spinBoxHeightMin->value();
    area.mHeightMax = ui->spinBoxHeightMax->value();

    // Create new polygon area.
    if ( ui->radioButtonAreaPolygon->isChecked() )
    {
        if ( gPolygon.size() < 3 )
        {
            QMessageBox::warning(this, "Polygon", "Polygon has less than 3 points");
            return;
        }

        int i = 0;
        foreach ( const QPointF &pnt, gPolygon )
        {
            RePoint rePoint;
            rePoint.mType = RePoint::enLine;
            rePoint.mCoord = CCoord(pnt.y(), pnt.x());

            area.mPoints[i++] = rePoint;
        }
    }
    // Create new circle area.
    else
    {
        if ( gRadiusCircle <= 0. )
        {
            QMessageBox::warning(this, "Circle", "Circle is not defined");
            return;
        }

        RePoint rePoint;
        rePoint.mType = RePoint::enCircle;
        rePoint.mArcCoord = CCoord(gCenterCircle.y(), gCenterCircle.x());
        rePoint.mArcDistance = gRadiusCircle;
        area.mPoints[0] = rePoint;
    }

    warnNotifier->addFir("UMMV", fir);
    warnNotifier->setParams(ui->spinBoxInterval->value(), true);

    warnNotifier->start();
}

void MainWindow::on_radioButtonAreaPolygon_clicked()
{
    if ( ui->radioButtonPickArea->isChecked() )
    {
        mPickPoints.clear();
        gPolygon.clear();
        gPolygonModel.clear();
        gCenterCircle = QPointF(-1., -1.);
        gRadiusCircle = 0;

        update();
    }
}

void MainWindow::on_radioButtonAreaCircle_clicked()
{
    on_radioButtonAreaPolygon_clicked();
}

void MainWindow::on_spinBoxInterval_valueChanged( int i )
{
    warnNotifier->setParams(i, true);
    update();
}

void MainWindow::onPlaneInsideZone(quint16 boardNumber, int typeZone, quint32 idZone)
{
    mStatusMessage = QString("Plane inside zone. Board number: %1  Zone type: %2  Zone id: %3")
            .arg(boardNumber).arg(typeZone).arg(idZone);

    update();
}

void MainWindow::onPlaneZoneOnCourse(quint16 boardNumber, int typeZone, quint32 idZone)
{
    mStatusMessage = QString("Plane on course. Board number: %1  Zone type: %2  Zone id: %3")
            .arg(boardNumber).arg(typeZone).arg(idZone);

    update();
}
