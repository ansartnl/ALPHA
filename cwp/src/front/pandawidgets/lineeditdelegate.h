#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QStyledItemDelegate>

class QCompleter;

class LineEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LineEditDelegate(QSharedPointer<QValidator> validator, QObject *parent = 0, bool ReadOnly = false);

    void setCompleter(const QStringList& completeList);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:

    void toUpper(const QString &text);

private:
    QSharedPointer<QValidator> validator_;
    QCompleter* completer_;
    bool m_read_only;
};

#endif
