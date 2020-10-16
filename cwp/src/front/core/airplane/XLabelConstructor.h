#ifndef XLABELCONSTRUCTOR_H
#define XLABELCONSTRUCTOR_H

#include <QDialog>
#include "ui_LabelConstructor.h"
#include "front/common/XAppSettings.h"
#include "XLabel.h"

//! Class for graphic create and edit labels
class CXLabelConstructor
		: public QDialog
		, public Ui::LabelConstructor
		, public AppSettings
		, public CXLabelItem::ILabelHandler
{
Q_OBJECT

#ifdef Q_OS_LINUX
static const int VIEW_SIZE_X	= 940;
static const int VIEW_SIZE_Y	= 450;
static const int SEPARATOR_POS	= 200;
#else
static const int VIEW_SIZE_X	= 870;
static const int VIEW_SIZE_Y	= 450;
static const int SEPARATOR_POS	= 170;
#endif

static const int MAX_SCALE = 10;

public:
	//! Constructor
	/*! \param parent a parent widget */
    explicit CXLabelConstructor(QWidget *parent = 0);
	//! Destructor
	virtual ~CXLabelConstructor();

	//! Interface CXLabelItem::ILabelHandler
	virtual void SetPos(CXLabelItem* pItem, QPointF scenePos);

protected:
	//@{
	//! Inherited from QWidget
	virtual void closeEvent(QCloseEvent* pEvent);
	virtual void resizeEvent(QResizeEvent* pEvent);
	virtual void wheelEvent(QWheelEvent* pEvent);
    virtual bool eventFilter(QObject* watched, QEvent* event);
	//@}
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}

private slots:
	void OnRuleButtons(QAbstractButton* pButton);
	void OnComboType(int index);
	void OnComboDimension(int index);
	void OnButForeground();
	void OnButBackground();
	void OnSpinTransparent(int iValue);
    void OnPullScene(QPointF CenterPoint);

private:
	void SetItems(const XGridLabel& GridLabel);
	void SetDimension(int iDimension);
	void SetItemPos(CXLabelItem* pItem, const XGridItem& pLabelItem);

	void CalcRowSpan(const int row, const int column, const int dimension, const XGridLabel::TMapItem& map, qint32& span);
	void CalcColumnSpan(const int row, const int column, const int dimension, const XGridLabel::TMapItem& map, qint32& span);
    void CalcSpan(XGridLabel& label);

	void SetModify(bool bModify);
	void Save();

    void SetDefaultLabel();

    //@{
    //! Size functions
    int get_VIEW_SIZE_X() const;
    int get_VIEW_SIZE_Y() const;
    int get_SEPARATOR_POS() const;
    double getSizeFactor() const;
    //@}

private:
	typedef QList<QSharedPointer<QGraphicsItem> > TGridItems;
	typedef QMap<XGridItem::LabelItemType, QSharedPointer<CXLabelItem> > TLabelItems;

	double			m_dScale;
	int				m_iDimension;
	int				m_iColX;
	int				m_iColY;
	bool			m_bPosChange;
	bool			m_bModify;
	TGridItems		m_listGridItems;
	TLabelItems		m_listLabelItems;
	XGridLabel::TMapGrid	m_mapGrig;

    bool            m_bPull;
    QPointF         m_PullPoint;
};

#endif // XLABELCONSTRUCTOR_H
