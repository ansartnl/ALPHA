#ifndef LISTACVIEW_H
#define LISTACVIEW_H

#include "front/common/XAppSettings.h"
//#include <QTableView>
#include <QTableView>


#define DEFAULT_MAX_COLUMN_WIDTH  100
#define DEFAULT_MAX_ROW_HEIGHT  20
#define DEFAULT_MAX_TABLE_HEIGHT  500

class CListACView
        : public QTableView, public AppSettings
{
Q_OBJECT
public:
    explicit CListACView(QWidget *parent = 0);

    void setPlanType(QString type);
    void setPlanHeaders(QStringList headers);
    void scroll(bool scrolled);
    bool isScrolled() const;

signals:
    void scrollChanged(bool scrolled);
    void enter_leaveEvent(bool bEnter);
    void keyPressedEvent(const QModelIndex &index, QKeyEvent *event);
    void mouseEvent(const QModelIndex &index, QMouseEvent *event);

protected:
    //@{
    //! Inherited from QTableView
    virtual void leaveEvent(QEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    //@}
    //@{
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}
};

#endif // LISTACVIEW_H
