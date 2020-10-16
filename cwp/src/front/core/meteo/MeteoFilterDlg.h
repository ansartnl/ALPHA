#ifndef METEO_FILTER_DLG_H
#define METEO_FILTER_DLG_H

#include <QDialog>
#include "ui_MeteoFilterDlg.h"

//! Class for filtering runways
class MeteoFilterDlg : public QDialog, public Ui::MeteoFilterDlg
{
Q_OBJECT

public:
    //! Constructor
    /*!
     \param pParent a parent widget
     \param f a widget's flags
     */
    MeteoFilterDlg(QWidget* pParent = 0, Qt::WindowFlags f = 0);
    //! Destructor
    virtual ~MeteoFilterDlg();

    //! Set filter or not
    bool isFilter();
    //! Include runway or not
    bool isRunwayEnable(QString sAirport, QString sRunway);
    //! Build tree
    void init(QString sAirport);
    //! Current airport
    QString GetAirport();

private slots:
    void accept();
    void on_comboBox_currentIndexChanged(int index);

private:
    Qt::CheckState RunwayCheckState(QString sFIR, QString sAirport, QString sRunway);

private:
    typedef QSet<QString>               TRunwayList;
    typedef QMap<QString, TRunwayList>  TAirportMap;
    typedef QMap<QString, TAirportMap>  TFirMap;

    TFirMap m_FilterMap;
};

#endif // METEO_FILTER_DLG_H
