#ifndef LISTCOST_H
#define LISTCOST_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"

class ActWidget;
class ActTableModel;
class CGUIAirplane;

class CListCost : public CListAC,
    public AppSettings
{
Q_OBJECT

public:
    explicit CListCost(CMaster* master = 0, QWidget* parent = 0);

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}

private slots:
    void onButtonCostShow();
    void OnDblClickItemEdit(const QModelIndex& index);
    void OnClicked(const QModelIndex& index);
	void onAct();

private:
    //@{
    //! Inherited from CListAC
    virtual int  InsertRow(const QSharedPointer<CAirplane>& airplane);
    virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& airplane);
    virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& airplane);
    virtual void TimerSlot();
    //@}
    void setInSector(QSharedPointer<CSFPL>& sfpl);
    void InitActIn();
    void InitActDep();

private:
    QPushButton *but_cost_show;

    ActTableModel *show_model_;
	ActWidget *act_in_widget_;

	QSharedPointer<CSFPL> sfpl_;
};

#endif // LISTCOST_H
