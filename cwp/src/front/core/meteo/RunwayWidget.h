#ifndef RUNWAY_WIDGET_H
#define RUNWAY_WIDGET_H

#include <QWidget>
#include "front/common/XAppSettings.h"
#include "RunwayWidgetData.h"
#include "AudioPlayer.h"
#include <QtSql/QSqlQuery>

namespace Ui {
class RunwayWidget;
}
class QToolButton;

class RunwayWidget : public QWidget, public AppSettings
{
    Q_OBJECT

public:
    enum RunwayType
    {
        RW1, RW2
    };

    explicit RunwayWidget(RunwayType type, QWidget *parent = 0);
    ~RunwayWidget();

    static QString currentRunway1() {return mCurrentRunway1;}
    static QString currentRunway2() {return mCurrentRunway2;}
signals:
    void sizeChanged();
    void UpdateMeteoToMaster();

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont();
    virtual void SI();
    //@}

private slots:
    void OnChangeRunway(int index, bool bFromRedis = false);
    void OnChangeRunwayState(bool bFromRedis = false);
    void OnChangeILS(bool bFromRedis = false);
    void OnChangeNDB(bool bFromRedis = false);
    void OnChangePress(int iValue, bool bFromRedis = false);
    void OnChangeEchelon(int index, bool bFromRedis = false);
    void OnChangeATIS(bool bFromRedis = false);
    void OnClickedUseMeteo(bool checked);
    void OnToggledUseMeteo(bool checked);

    void OnQNHEditingFinished();

    void onChangedRWF1(QString idFIR, RunwayWidgetDataFIR rwf, QString operType);
    void onChangedRWF2(QString idFIR, RunwayWidgetDataFIR rwf, QString operType);
    void onChangedRWF(QString idFIR, RunwayWidgetDataFIR rwf, QString operType);
    void onChangedRWFVPP(QString idFIR, RunwayWidgetDataVPP rwf, QString operType);
    void onSaveRWF();
    void onSaveRWFVPP();

    void toUpper(const QString &text);
    void _OnTimer();
    void OnClickedBut1(bool checked);
    void OnClickedBut2(bool checked);
    void OnClickedBut3(bool checked);
    void OnClickedBut4(bool checked);
    void OnToggledBut1(bool checked);
    void OnToggledBut2(bool checked);
    void OnToggledBut3(bool checked);
    void OnToggledBut4(bool checked);
    void setBorder(bool free);


private:
    int GetEchelonValue(bool bFeet) const;
    void SetEchelonString();
    int GetEchelonComboIndex(int FL) const;
    void SetButtonState(QToolButton* but, int state);
    QString GetButtonStateText(int state) const;

    void SetRunway(bool bFromRedis = false);
    void SaveRWF();
    void SaveRWFVPP();
    void UpdateQNH();
    bool CheckDataBase();
    void loadRunwaysFromConfig();

    static QString mCurrentRunway1;
    static QString mCurrentRunway2;
private:
    Ui::RunwayWidget *ui;
    RunwayWidgetDataFIR mRWF;
    RunwayWidgetDataVPP mRWFVPP;
    bool mInvokeSave;
    bool mInvokeSave2;
    QTimer* timer;
    QColor color_notoperativ;
    QString lifetime;
    QString ourAirport;
    AudioPlayer *player;
    RunwayType _type;
};

#endif // RUNWAY_WIDGET_H
