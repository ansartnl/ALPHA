#ifndef LISTACHEADERVIEW_H
#define LISTACHEADERVIEW_H

#include <QHeaderView>

class CListACHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CListACHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

    //! Sets header belonging to some list type
    void setType(QString type){
        sPlanType = type;
    }

    void setHeaders(QStringList headers);

    bool isScrolled;
protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const;

    virtual bool event(QEvent* event);
    virtual QSize sectionSizeFromContents ( int logicalIndex ) const;
private:
    QString sPlanType;
    QStringList* m_pHeaders;
    QList<int> m_ColumnSizes;
    int hoverIndex;
};

#endif // LISTACHEADERVIEW_H
