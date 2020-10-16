#ifndef LISTACITEMDELEGATE_H
#define LISTACITEMDELEGATE_H

#include <QStyledItemDelegate>

//! Class extends QStyledItemDelegate for lists AC
class CListACItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT
    public:
        //! Constructor
        /*!
         \param colType a table column type for edit
         \param parent a parent object
         */
        explicit CListACItemDelegate(const QMap<int, int>& colType, QObject *parent = 0);

        //@{
        //! Inherited from QStyledItemDelegate */
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        //@}

        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    signals:
        //! Some of the special edit
        void itemEdit(const QModelIndex index, bool& bEdit);

    private slots:
        void commitAndCloseEditor();

    private:
        void ShowEditCompleter(QWidget* edit) const;
        QPoint isRectVisible(QPoint tl, QPoint bl, QRect rect) const;

    private:
        QMap<int, int>  m_mapColType;
        bool            m_bSignalEmitted;
        QColor          m_HighTextColor;
};

#endif // LISTACITEMDELEGATE_H
