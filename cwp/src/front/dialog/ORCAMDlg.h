#ifndef ORCAMDLG_H
#define ORCAMDLG_H

#include <QDialog>

#include "back/common/AirplaneState.h"

namespace Ui {
class ORCAMDlg;
}

class ORCAMModel;

class ORCAMDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit ORCAMDlg(const QStringList &lst, int minutes, QWidget *parent = 0);
    ~ORCAMDlg();

    void setExpireTimeout(int minutes);
    void airplaneData(const CAirplaneState& data);
    void reset();
    void ChangedOrcamRedis(QString code, QString assigned_time, QString otType);

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void onFindNext();
    void _OnTimerOccupied();

private:
    Ui::ORCAMDlg *ui;
    int mTimer;
    int mExpireTimeout;
    ORCAMModel *mModel;
    int mOrcamLifetime;
    QTimer* mTimerOccupied;
};

#endif // ORCAMDLG_H
