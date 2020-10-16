#ifndef VPP_H
#define VPP_H

#include <QWidget>

#include "front/common/XAppSettings.h"

namespace Ui {
class Vpp;
}

class Vpp : public QWidget, public AppSettings
{
    Q_OBJECT    

public:
    explicit Vpp(QWidget *parent = 0);
    ~Vpp();

    inline void setRunwayName(const QString& name)
    {
        mRunwayName = name;
        updateVpp();
    }
    inline const QString& runwayName() const
    {return mRunwayName;}

    inline void setDisabledWidget(bool isDisabled)
    {
        mIsDisabled = isDisabled;
        updateVpp();
        updateButtons();
    }
    inline bool isDisabledWidget() const
    {return mIsDisabled;}
    
public slots:
    void reset();
    void onNewData(QString runway, int numPeople, int numAuto, int numOther);

signals:
    void peopleCount(int count);
    void autoCount(int count);
    void otherCount(int count);

protected:
    void updateVpp();
    void updateButtons();

    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}

private slots:
    void onRemovePeople();
    void onAddPeople();
    void onRemoveAuto();
    void onAddAuto();
    void onRemoveOther();
    void onAddOther();

private:
    Ui::Vpp *ui;
    int mStatCount;
    int *mStats;
    QString mRunwayName;
    bool mIsDisabled;

    int mPendingPeople;
    int mPendingAuto;
    int mPendingOther;
};

#endif // VPP_H
