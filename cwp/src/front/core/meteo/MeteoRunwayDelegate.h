#ifndef _METEO_RUNWAY_DELEGATE_H__
#define _METEO_RUNWAY_DELEGATE_H__

#include <QItemDelegate>


//! Tree widget delegate.
class MeteoRunwayDelegate : public QItemDelegate
{
public:
    MeteoRunwayDelegate(QObject *parent);

    QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option,
        const QModelIndex & index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

#endif // _METEO_RUNWAY_DELEGATE_H__
