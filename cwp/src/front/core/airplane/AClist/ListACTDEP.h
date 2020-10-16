#ifndef LISTACT_DEP_H
#define LISTACT_DEP_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"

class ActWidget;
class ActTableModel;

class CListActDep : public CListAC,
    public AppSettings
{
Q_OBJECT

public:
    explicit CListActDep(CMaster* master = 0, QWidget* parent = 0);
    explicit CListActDep(QStringList _lst, CMaster* master = 0, QWidget* parent = 0);
    //virtual const QSize GetVisibleSize() const;

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}

private slots:
    void onButtonActIn();
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
    void setInSector(QSharedPointer<CSFPL>& sfpl);
    QString ReadOrcam();
    //QString ReadOrcamRedis();
    QString FreeCodeOrcam();
    void Init();

private:
    QPushButton *but_act_;

	ActTableModel *edit_model_;
	ActWidget *act_in_widget_;

	QSharedPointer<CSFPL> sfpl_;
    QStringList lst_orcamrange;
};

#endif // LISTACT_DEP_H
