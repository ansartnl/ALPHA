#ifndef LIST_APP_H
#define LIST_APP_H

#include "front/common/XAppSettings.h"
#include "ListAC.h"

//! FPL's list APP
class CListAPP : public CListAC,
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
	explicit CListAPP( CMaster* pMaster = 0, QWidget* pParent = 0);
	~CListAPP();

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}
private slots:
    void OnItemEdit(const QModelIndex index, bool& bEdit);
    void OnDbClickItemEdit(const QModelIndex& index);
private:
	//@{
	//! Inherited from CListAC
	virtual int  InsertRow(const QSharedPointer<CAirplane>& pAirplane);
	virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane);
	virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& pAirplane);
	//@}
};

#endif // LIST_APP_H
