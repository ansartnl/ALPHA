#ifndef LISTDAILYPLAN_H
#define LISTDAILYPLAN_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"
#include "planeditor.h"
#include <QRadioButton>
#include <QtSql/QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QtSql/QSqlRecord>
#include "DailyDlgSID.h"
#include "DailyDlgTime.h"


class QCheckBox;

class ActWidget;
class ActTableModel;

class ANOProxyModel;
class FixedWidthDelegate;
class CListDailyPlan : public CListAC,
    public AppSettings
{
Q_OBJECT

public:
    explicit CListDailyPlan(CMaster* master = 0, QWidget* parent = 0);
    explicit CListDailyPlan(QStringList _lst, CMaster* master = 0, QWidget* parent = 0);
    ~CListDailyPlan();

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}

private slots:
    void onFilterAllChecked(bool checked);
    void onFilterChanged();
    void onButtonViewPlan();
    void onButtonEditPlan();
    void onButtonActPlan();
    void onButtonDeActPlan();
    void OnClickItemEdit(const QModelIndex& index, QString str);
    void OnDblClickItemEdit(const QModelIndex& index);
    void OnMouseEvent(const QModelIndex& index, QMouseEvent* event);
    void OnEditDevice();
    void OnRouteDevice();
    void OnSidDevice();
    void OnStarDevice();
    void OnChangeSelect(const QItemSelection &selected, const QItemSelection &deselected);
    void onFplPlanFirstClicked();
    void onFplPlanPrevClicked();
    void onFplPlanNextClicked();
    void onFplPlanLastClicked();
    void onANOToggle(bool checked);
    void onCWPToggle(bool checked);
    void onAct();
    void _OnTimer();
    void OnAutoActFpl();
    void OnThread();
    void OnTimerRefreshFpl();
    void RefreshFpl();
    void RefreshCwp();
    void CurrentIndex(QModelIndex ind);
    void onHeaderClicked(int n);
    void onScrollChanged(bool scrolled);
    void afterSleep();

private:
    //@{
    //! Inherited from CListAC
    virtual int  InsertRow(const QSharedPointer<CAirplane>& airplane);
    virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& airplane);
    virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& airplane);
    virtual void TimerSlot();
    //@}

    void setFilterVisible(bool visible);
    void CheckIndexNavigation(QModelIndex & index, int row);
    void PlanWidgetShow();
    void setInSector(QSharedPointer<CSFPL> &sfpl);
    void setInSector2(QSharedPointer<CSFPL> &sfpl);
    void AdaptateRoute(QString route);
    QString ReadOrcam();
    QString FreeCodeOrcam();
    void Init();
    quint32 CalcMinWidth(void);

private:
    ActTableModel *act_in_model_;
    ActWidget *act_in_widget_;
    PlanEditor *plan_widget;
    QLabel      *mRowCount;
    QCheckBox   *mFilterAll;
    QCheckBox   *mFilterAlternative;
    QCheckBox   *mFilterArrival;
    QCheckBox   *mFilterDeparture;
    QPushButton *but_actplan;
    QPushButton *but_deactplan;
    QPushButton *but_viewplan;
    QPushButton *but_editplan;
    QUuid uid;
    QSharedPointer<CSFPL> sfpl_;
    QSharedPointer<CSFPL> sfpl_MouseEvent;
    QModelIndex sourceIndex_MouseEvent;
    QWidget *_pParent;
    int last_colomn_clicked;
    int last_row_clicked;

    QRadioButton *ano_button;
    QRadioButton *cwp_button;
    QRadioButton *ano_button_s;
    QRadioButton *cwp_button_s;
    QHBoxLayout *layout_max_line;
    QHBoxLayout *layout_min_line;
    QVBoxLayout *layout_lines;
    QSqlQueryModel *model_fpl;
    ANOProxyModel *sqlproxy;
    FixedWidthDelegate  *mFixedWidthDelegate;
    QSqlRecord record;

    QTimer* timer;
    QTimer* timerAutoActFpl;
    QThread *mThread;
    QString newRoute;
    //CGUIAirplane*	m_pAirplane;
    int clickColumn;
    int selectedIdFpl;
    QUuid selectedUid;
    QModelIndex indexProxy;
    FPL mFpl;
    QStringList lst_orcamrange;
    bool finish;
    QString cod;
    QModelIndex indexMouseEvent;
    QDateTime lastTimeRefresh;
    QTimer *timerRefreshFpl;
};


#endif // LISTDAILYPLAN_H
