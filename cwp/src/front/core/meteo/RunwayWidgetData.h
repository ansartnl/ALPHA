#ifndef RUNWAY_WIDGET_DATA_H
#define RUNWAY_WIDGET_DATA_H

#include <QString>
#include <QVariantMap>

class RunwayWidgetData
{
public:
    RunwayWidgetData();

    QVariantMap toVariantMap() const;
    void fromVariantMap(const QVariantMap& map);

    QString name() const;
    void setName(const QString& s);

    int state() const;
    void setState(int s);

    int ILS() const;
    void setILS(int s);

    int NDB() const;
    void setNDB(int s);

private:
    QString mName;
    int     mState;
    int     mILS;
    int     mNDB;
};


class RunwayWidgetDataFIR
{
public:
    typedef QMap<QString, RunwayWidgetData> TRunways;

    RunwayWidgetDataFIR();
    RunwayWidgetDataFIR(const QString& sFIR);

    QVariantMap toVariantMap() const;
    void fromVariantMap(const QVariantMap& map);

    QString FIR() const;
    void setFIR(const QString& s);

    int QNH() const;
    void setQNH(int s);

    int TL() const;
    void setTL(int s);

    QString ATIS() const;
    void setATIS(const QString& s);

    QString currentRunway() const;
    void setCurrentRunway(const QString& s);

    bool isValidRunway() const;
    RunwayWidgetData& runway(const QString& runwayName);
    void addRunway(const RunwayWidgetData& s);
    TRunways& runwaysList();
    bool UseMeteo() const;
    void setUseMeteo(bool ch);
    bool Serv1() const;
    void setServ1(bool ch);
    bool Vehicle() const;
    void setVehicle(bool ch);
    bool Serv2() const;
    void setServ2(bool ch);
    bool Other() const;
    void setOther(bool ch);

private:
    QString mFIR;
    //int     mQNH;
    //int     mTL;
    //QString mATIS;
    QString mCurrentRunway;
    RunwayWidgetData mEmptyRunway;
    TRunways mRunways;
    //bool mUseMeteo;
    bool mServ1;
    bool mVehicle;
    bool mServ2;
    bool mOther;
};

Q_DECLARE_METATYPE(RunwayWidgetDataFIR)

class RunwayWidgetDataVPP
{
public:
    //typedef QMap<QString, RunwayWidgetData> TRunways;

    RunwayWidgetDataVPP();
    RunwayWidgetDataVPP(const QString& sFIR);

    QVariantMap toVariantMap() const;
    void fromVariantMap(const QVariantMap& map);

    QString FIR() const;
    void setFIR(const QString& s);

    int QNH() const;
    void setQNH(int s);

    int TL() const;
    void setTL(int s);

    QString ATIS() const;
    void setATIS(const QString& s);

    bool UseMeteo() const;
    void setUseMeteo(bool ch);

private:
    QString mFIR;
    int     mQNH;
    int     mTL;
    QString mATIS;
    bool mUseMeteo;
};

Q_DECLARE_METATYPE(RunwayWidgetDataVPP)


#endif // RUNWAY_WIDGET_DATA_H
