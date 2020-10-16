#ifndef LIST_SIL_APP_ARR_H
#define LIST_SIL_APP_ARR_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"

class QPushButton;

//! FPL's list SIL APP ARR
class CListSIL_APP_ARR : public CListAC,
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
	explicit CListSIL_APP_ARR( CMaster* pMaster = 0, QWidget* pParent = 0);
	~CListSIL_APP_ARR();

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}
private slots:
    void OnButtonACT();
    void OnItemEdit(const QModelIndex index, bool& bEdit);
    void OnDbClickItemEdit(const QModelIndex& index);
private:
	//@{
	//! Inherited from CListAC
	virtual int  InsertRow(const QSharedPointer<CAirplane>& pAirplane);
	virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane);
	virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& pAirplane);
	//@}
private:
    QPushButton*			m_butACT;
};

#endif // LIST_SIL_APP_ARR_H
