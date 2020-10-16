#ifndef METEORUNWAYWIDGET_H
#define METEORUNWAYWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include "front/common/XAppSettings.h"

namespace Ui {
    class MeteoRunwayWidget;
}
class MeteoRunway;

class MeteoRunwayWidget : public QWidget, public AppSettings
{
    Q_OBJECT

public:
    explicit MeteoRunwayWidget(QWidget *parent = 0);
    ~MeteoRunwayWidget();

    const QSize GetVisibleSize() const;
    void SizeToContents(bool bSend = true);
    void Refresh();

signals:
    void sizeChanged();

protected:
    virtual void showEvent(QShowEvent* event);
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}

public slots:
    void setModeldataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);

private slots:
    void on_comboBoxRunways_currentIndexChanged(int index);
    void on_toolButtonApply_clicked();
    void on_toolButtonReset_clicked();
    void on_toolButtonFilter_clicked();
    void on_editRow_clicked();

    void OnTimerSize();

private:
    void setupUi();
    void init();
    bool isDataChanged() const;
    bool isAirportOnFIR(const QString& sAirport) const;
    QString GetAirportName();

    typedef QList<MeteoRunway>  TMeteoList;

    Ui::MeteoRunwayWidget *ui;
    /** Old meteo information */
    TMeteoList  mMeteoOld;
    /** Old current runway */
    QString     mCurrentRunwayOld;
};

#endif // METEORUNWAYWIDGET_H
