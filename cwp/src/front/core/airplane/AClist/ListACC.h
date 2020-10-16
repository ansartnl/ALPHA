#ifndef LIST_ACC_H
#define LIST_ACC_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"

class QPushButton;

//! FPL's list ACC
class CListACC : public CListAC,
	public AppSettings
{
Q_OBJECT

public:
	//! Constructor
	/*!
	  \param pBase a pointer to Dao
	  \param pMaster a pointer to CMaster
	  \param pParent a parent widget
	*/
	explicit CListACC( CMaster* pMaster = 0, QWidget* pParent = 0);
	~CListACC();

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}
private slots:
    void OnButtonACT();
    void OnButtonMes();
    //void OnScrollItem(int index);
    void OnItemEdit(const QModelIndex index, bool& bEdit);
    void OnDbClickItemEdit(const QModelIndex& index);

private:
	//@{
	//! Inherited from CListAC
	virtual int  InsertRow(const QSharedPointer<CAirplane>& pAirplane);
	virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane);
	virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& pAirplane);
	virtual void TimerSlot();
	//@}

private:
    QPushButton*			m_butACT;
    QPushButton*			m_butMes;
};

#endif // LIST_ACC_H
