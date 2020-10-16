#ifndef METEO_WIDGET_H
#define METEO_WIDGET_H

#include <QWidget>
#include <QtSql/QSqlQueryModel>

#include "front/common/XAppSettings.h"
#include "ui_MeteoWidget.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableView>
#include <QtCore/QTime>
#include <QtSql/QSqlQuery>



namespace Ui {
class MeteoWidget;
}
class QToolButton;

class MeteoWidget : public QWidget, public AppSettings
{
    Q_OBJECT

public:
    explicit MeteoWidget(QString privileges, QWidget *parent = 0);
    ~MeteoWidget();

    const QSize GetVisibleSize() const;
    void SizeToContents(bool bSend = true);

signals:
    void sizeChanged();

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont();
    virtual void SI();
    void UpdateModelAirports();
    void ClearMetar();
    void ClearMetreport();
    bool CheckDataBase();
    void SetColor(QString color);
     void SetColorMetreport(QString color);
    //@}

private:
    void DefaultSelectOurAirport();


private slots:
    void UpdateModelTaf();
    void UpdateModelMetar();
    void UpdateModelSigmet();
    void UpdateModelMetreport();
    void UpdateModelGamet();
    //void onButtonHistory();
    void onActualToggle(bool toggle);
    void onAllToggle(bool toggle);
    void OnChangeSelect(const QItemSelection &itsnew, const QItemSelection & its);
    void OnChangeSelect2(const QItemSelection &itsnew, const QItemSelection & its);
    void _OnTimer();
    void onUpdateMeteo();
    void OnClicked(QModelIndex index);

private:
    Ui::MeteoWidget *ui;
    QHBoxLayout *layoutMetar;
    QHBoxLayout *layoutMetreport;
    QVBoxLayout *layoutVert;
    QVBoxLayout *layoutVert2;
    QWidget *widgetMetar;
    QGridLayout *layoutGrid;
    QGridLayout *layoutGrid2;

    QTableView *view_taf;
    QTableView view_metar;
    QTableView view_airports;
    QTableView view_airports2;
    QTableView *view_sigmet;
    QTableView view_metreport;
    QTableView *view_gamet;

    QSqlQueryModel *model_taf;
    QSqlQueryModel *model_metar;
    QSqlQueryModel *model_airports;
    QSqlQueryModel *model_sigmet;
    QSqlQueryModel *model_metreport;
    QSqlQueryModel *model_gamet;

    QLineEdit *w_direct;
    QLineEdit *w_speed;
    QLineEdit *qnh;
    QLineEdit *w_max;
    QLineEdit *w_direct_lateral;
    QLineEdit *w_direct_axial;
    QTextEdit *text;
    QLineEdit *w_direct2;
    QLineEdit *w_speed2;
    QLineEdit *qnh2;
    QLineEdit *w_max2;
    QLineEdit *w_direct_lateral2;
    QLineEdit *w_direct_axial2;
    QTextEdit *text2;
    int idAirport;
    int idAirportStore;

    QTimer* timer;
    QTime time_actualmetar;
    QDateTime now;
    QColor color_notoperativ;
    QString lifetime;

    QString airportsName;
    QStringList airportsList;
    QString queryStr;
    QString airportOur;

    QString privileges;
    QWidget* tabTaf;
    QWidget* tabMetar;
    QWidget* tabSigmet;
    QWidget* tabGamet;
    QWidget* tabMetreport;
    QString firs;
};

#endif // METEO_WIDGET_H
