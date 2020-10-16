#ifndef CONTROLDLG_H
#define CONTROLDLG_H

#include <QFrame>
#include "front/common/XAppSettings.h"
#include <QStringListModel>

class QListView;
class QLineEdit;
class QTreeWidgetItem;
class QTreeWidget;

//! Class for editing in lists airplane's data
class CControlDlg : public QFrame,
	public AppSettings
{
Q_OBJECT
public:
    enum Type { FPLType, CODEType, CFLType, ETDType, STARType };

	//! Costructor
    explicit CControlDlg(const Type type, QWidget* parent = 0, Qt::WindowFlags wFlags = Qt::Popup);
	//! Destructor
	virtual ~CControlDlg();

	//! Initialization function
	/*! \param sVal a current value */
	void Init(const QString sVal);
	//! Initialization function
	/*!
		\param sValues1 a first set of values
		\param sValues2 a second set of values
	*/
	void SetValues(const QStringList& sValues1, const QStringList& sValues2) { m_sValues1 = sValues1; m_sValues2 = sValues2; }
	//! Values after edit
	const QStringList& GetValues() { return m_sValues; }

signals:
	//! Signal is sent after editing
	void endEdit();

protected slots:
    //! Selected item on list widget
    void SelectListItem(const QModelIndex& item);
    //! Selected item on tree widget
    void SelectTreeItem(QTreeWidgetItem* item, int column);
    //! Selected item on edit widget
	void SelectEditItem();
    void OnResizeColumns(QTreeWidgetItem* pItem);

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate() {}
	virtual void UpdateXFont();
	virtual void SI() {}
	//@}

private:
	Type				m_type;
	QStringList			m_sValues1;
	QStringList			m_sValues2;
	QStringList			m_sValues;
	QStringListModel	m_sModel;
	QListView*			m_pListView;
    QTreeWidget*        m_pTreeWidget;
	QLineEdit*			m_pEdit;
};

#endif // CONTROLDLG_H
