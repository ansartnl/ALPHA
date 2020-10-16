#ifndef LISTACT_OUT_H
#define LISTACT_OUT_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"

class ActWidget;
class ActTableModel;

class CListActOut : public CListAC,
    public AppSettings
{
Q_OBJECT

public:
    explicit CListActOut(CMaster* master = 0, QWidget* parent = 0);

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}

private slots:
    void OnDblClickItemEdit(const QModelIndex& index);
    void onAct();

private:
    //@{
    //! Inherited from CListAC
    virtual int  InsertRow(const QSharedPointer<CAirplane>& airplane);
    virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& airplane);
    virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& airplane);
    virtual void TimerSlot();
    //@}

private:
	ActTableModel *edit_model_;
	ActWidget *act_in_widget_;

	QSharedPointer<CSFPL> sfpl_;
};

#endif // LISTACT_OUT_H
