#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QtCore>

#include "BaseToolDialog.h"
#include "back/core/airplane/BearingPoint.h"

namespace Ui
{
class ControlWindow;
}

class QMenu;
class QPushButton;
class QSqlTableModel;
class QTreeWidgetItem;
class QxtSpanSliderRestricted;


class ControlWindow : public BaseToolDialog
{
    Q_OBJECT

public:

    enum HeightMeasure {
        Meters  = 0,    //<
        Feet            //<
    };

    enum Orientation
    {
        horizontal = 0,
        vertical
    };

    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();

Q_SIGNALS:
    void logOffButtonClicked();

    void speedClicked(int value);
    void distanceClicked(int value);

    void angleClicked(int value);
    void autoAntiOverlapClicked(bool);

    void sectorComboIndexChanged(int index);

    void sectorButtonClicked();
    void restrictionsButtonClicked(bool);
    void freqButtonClicked(bool);
    void settingsButtonClicked();

    void normalButtonClicked();

    void deflexionValueChanged(int value);

    void heightLowerPositionChanged(int position);
    void heightUpperPositionChanged(int position);
    void heightSpanChanged(int min, int max);
    void zoomChanged(int zoom);
    void layersTriggered();

    void heightCurrentIndexChanged(int index);
    void speedCurrentIndexChanged(int index);

    void labelAngleChanged(int azimuth);

    void routesClicked(bool checked);
    void HidePlainsClicked(bool checked);
    void pointsClicked(bool checked);
    void BpointsClicked(bool checked);
    void sectorsClicked(bool checked);
    void remindersClicked(bool checked);
    void meteoClicked(bool checked);

    void vectorMeasurementButtonClicked(bool checked);
    void ShortButtonClicked(bool checked);

    void bearingIdChanged();

public:
    bool isRestrictionButtonChecked() const;
    void CreateOldButtons();
    void CreateNewButtons();
    int heightLowerValue() const;
    int heightUpperValue() const;

    int zoom() const;
    int angle() const;

    int sectorCurrentIndex();
    QString sectorCurrentText();
    void sectorBlockSignals(bool block);
    void MeteoVisible(bool visible);
    void MeteoCheck();

    void setBearingPoints(const QList<CBearingPoint> &points);
    void setBearingId(const QVariant &id);
    TBearingIdSet getBearingId() const;

public Q_SLOTS:
    void setCurrentDate(const QString &date);
    void setCurrentTime(const QString &time);
    void setCurrentUser(const QString &user);

    void setHeightLowerValue(int value);
    void setHeightUpperValue(int value);

    void setSpeedVectorValue(int value);
    void setDistanceVectorValue(int value);

    void setMainFrameVisible(bool visible);
    void setHeightFilterFrameVisible(bool visible);
    //void setPressureLevelFrameVisible(bool visible);
    void setAirplaneLengthFrameVisible(bool visible);
    void setRestrictionsEditorVisible(bool visible);

    //void setFlightLevel(const QString &text);
    //void setPressure(const QString &text);

    void setAutoAntiOverlapChecked(bool checked);
    void setRestrictionButtonChecked(bool checked);

    void setSectorModel(QSqlTableModel *model, int column);
    void setSectorCurrentIndex(int index);
    void setSettingsMenu(QMenu *menu);
    //void setFlightLevelMenu(QMenu *menu);
    //void setPressureMenu(QMenu *menu);

    void setHeightRange(int min, int max);
    void setSpanRange(int min, int max);
    void setRestrictedSpanRange(int min, int max);

    void setZoomRange(int min, int max);
    void setZoom(int current);

    void setHeightMinText(const QString &text);
    void setHeightMaxText(const QString &text);

    void setOrientation(Orientation orientation);

    void onPointsClicked(bool checked);
    void onBPointsClicked(bool checked);
    void onRoutesClicked(bool checked);
    void onSectorsClicked(bool checked);

    void setHeightCurrentIndex(int index);
    void setSpeedCurrentIndex(int index);
    void _OnShortCW(bool checked);
    void onLastBearing(const QString& lastBearing);

private Q_SLOTS:

    //void onHeightRangeChanged(int min, int max);
    void onLowerPositionChanged(int value);
    void onUpperPositionChanged(int value);
    //void onHeightSpanChanged(int min, int max);

    void on_slZoom_valueChanged(int value);

    void updateLanguage();

    void onAngleClicked(int value);
    void onBearingIdChanged(const QStringList &items);
protected:

    virtual void changeEvent(QEvent *event);

private:
    void connectButtonToMapper(QHash<int, QPushButton *> &bindings, QSignalMapper *mapper, QPushButton *button, int value);
    void disconectButtonFromMapper(QHash<int, QPushButton *> &bindings, QSignalMapper *mapper, QPushButton *button, int value);
private:
    Q_DISABLE_COPY(ControlWindow)

    Ui::ControlWindow *ui;

    QxtSpanSliderRestricted *height_slider_;

    QSignalMapper *speed_mapper_;
    QSignalMapper *distance_mapper_;
    QHash<int, QPushButton *> speed_buttons_;
    QHash<int, QPushButton *> distance_buttons_;

    QSignalMapper *angle_mapper_;

    int angle_;
};

Q_DECLARE_METATYPE(ControlWindow::HeightMeasure)

#endif // CONTROLWINDOW_H
