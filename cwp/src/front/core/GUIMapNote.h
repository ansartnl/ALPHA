#ifndef GUIMAPNOTE_H
#define GUIMAPNOTE_H

#include <QGraphicsItem>
#include "import/MapNote.h"
#include "GUIInterface.h"

class QAction;
class QMenu;

class GUIMapNote
        : public QObject,
        public MapNote,
        public QGraphicsItem,
        public IGUIItemHandler
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    GUIMapNote(QGraphicsItem* parent = 0);

    //@{
    //! Inherited from QGraphicsItem
    virtual QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget);
    //@}

    //@{
    //! Interface IGUIItemHandler
    virtual void SetPosition(qint64 qllXMin, qint64 qllYMax, double dScale, const CCoord& centralCoord);
    virtual void SetSI(qint16 SI) {}
    //@}

signals:
    void deleteNote(quint32 id);
    void editNote(quint32 id);

protected:
//    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private slots:
    void OnRemove();
    void OnEdit();

private:
    QColor  mTextColor;
    QRectF mTextZone;
    int mPenWidth;
    QAction*    mRemoveAction;
    QAction*    mEditAction;
    QMenu*      mDropDownMenu;
};

#endif // GUIMAPNOTE_H
