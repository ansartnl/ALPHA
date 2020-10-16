#ifndef GUILABELTEXT_H
#define GUILABELTEXT_H

#include <QGraphicsSimpleTextItem>
#include <QGraphicsLayoutItem>
#include "GUIControlDlg.h"
#include "front/common/XAppSettings.h"

//! Class for showing and editing any single data item
class CGUILabelText : public QObject, public QGraphicsSimpleTextItem
        , public QGraphicsLayoutItem, public AppSettings
{
Q_OBJECT
Q_INTERFACES(QGraphicsLayoutItem)

public:
	//! Constructor
	CGUILabelText(QGraphicsItem * parent = 0);
	//! Destructor
	virtual ~CGUILabelText();

    void SetACID(const QString &v) {m_ACID = v;}
    void SetCODE(const QString &v) {m_code = v;}
    //! Initialization function
	/*!
		\param sValues1 a first set of values
		\param sValues2 a second set of values
		\param sCurSel a current value
	*/
	void SetValues(const QStringList &sValues1, const QStringList &sValues2, const QString &sCurSel)
					{ m_sValues1 = sValues1; m_sValues2 = sValues2; m_sCurSel = sCurSel; }
	//! Set editable or not
	void SetReadOnly(const bool readOnly) { m_bReadOnly = readOnly; }
	//! Is editable or not
	const bool IsReadOnly() const { return m_bReadOnly; }
	//! Get type CGUIControlDlg for edit
	CGUIControlDlg::Type GetTypeDlg() const { return m_type; }
	//! Set type CGUIControlDlg for edit
	void SetTypeDlg(const CGUIControlDlg::Type type) { m_type = type; }
	//! Set color for text
	void SetTextColor(const QColor &color);
    void setBackgroundColor(const QColor &background_color) { background_color_ = background_color; };
    void setBoldFont(bool bold_font) { bold_font_ = bold_font; };
    void setFrame();
    void setBoundingRect(const QRectF &rect);

	//! Is item visible or not
	
public:
    const bool &GetVisible() const { return m_Visible; }
    void SetVisible(const bool &V) { m_Visible = V; }
private:
    bool m_Visible;

public:
	//@{
	//! Inherited from QGraphicsLayoutItem
	void setGeometry(const QRectF &geom);
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
	void updateGeometry();
	//@}

	//@{
	//! Inherited from QGraphicsSimpleTextItem
	void setText(const QString& text);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
	//@}

    //{@
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}

signals:
	//! Sends after item has been edited
	/*!
	  \param pWnd a pointer to sender, usually this
	  \param sValues a set of values
	*/
	void endEdit(const CGUILabelText *pWnd, const QStringList sValues);
    void dialogOpened();
    void dialogClosed();

public:
	//! Start edit
	void OnChoose();

protected slots:
	//! Call after edit
	/*! \param sValues a set of values */
	void OnItemChanged(const QStringList sValues);

protected:
    //@{
    //! Inherited from QGraphicsSimpleTextItem
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);
    //@}

private:
	bool					m_bReadOnly;
    QString                 m_ACID;
    QString                 m_code;
	QStringList				m_sValues1;
	QStringList				m_sValues2;
	QString					m_sCurSel;
	CGUIControlDlg::Type	m_type;
    QColor background_color_;
    QColor background_color_old;
    bool bold_font_;
    bool isFrame;
    QRectF m_boundingRect;
};

#endif // GUILABELTEXT_H
