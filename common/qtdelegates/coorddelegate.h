#ifndef COORD_DELEGATE_H
#define COORD_DELEGATE_H

#include <QStyledItemDelegate>
#include "coordedit.h"

//! Class extends QStyledItemDelegate for coordinates
class CoordDelegate
    : public QStyledItemDelegate
{
    Q_OBJECT
public:
    //! Constructor
    /*!
        \param type a type of coordinate
        \param parent a parent object
    */
    CoordDelegate(CoordEdit::CoordType type, QObject *parent = 0);
    CoordDelegate(CoordEdit::CoordType type, bool readOnly, QObject *parent = 0);

    bool isReadOnly() const;
    void setReadOnly(bool readOnly);

    //@{
    //! Inherited from QStyledItemDelegate
public:
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;
protected:
    virtual QWidget *createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const;
    //@}

private slots:
    void commitAndCloseEditor();

private:
    CoordEdit::CoordType m_CoordType;
    bool m_ReadOnly;
};

#endif // COORD_DELEGATE_H
