#ifndef GUIAZIMUTH_H
#define GUIAZIMUTH_H

#include <QWidget>

//! Class for graphic editing azimuth
class CGUIAzimuth : public QWidget
{
	Q_OBJECT
public:
	//! Constructor
	CGUIAzimuth(QWidget* pParent = 0, Qt::WindowFlags f = 0);
	//! Destructor
	virtual ~CGUIAzimuth();

	void setAzimuth1(int degrees);
	void setAzimuth2(int degrees);
	int  getAzimuth1() const;
	int  getAzimuth2() const;

signals:
	//! Signal emitted when azimuth is changed
	void changedAzimuth(int degrees);

public:
	//! Set editable or not
	
public:
    const bool &GetReadOnly() const { return m_ReadOnly; }
    void SetReadOnly(const bool &V) { m_ReadOnly = V; }
private:
    bool m_ReadOnly;

protected:
	//@{
	//! Inherited from QWidget
	bool event(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void draw(QPainter *painter);
	//@}

private:
	int m_iAzimuth1;
	int m_iAzimuth2;
};

#endif // GUIAZIMUTH_H
