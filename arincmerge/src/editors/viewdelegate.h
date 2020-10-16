#ifndef VIEWDELEGATE_H
#define VIEWDELEGATE_H

#include <QItemDelegate>
#include <QtSql/QSqlRelationalDelegate>

//! Class extends QItemDelegate
class ModifyDelegate
    : public QItemDelegate
{
    Q_OBJECT
public:
    ModifyDelegate(QObject* pObject = 0);

    //! Inherited from QItemDelegate
    virtual QWidget* createEditor(
            QWidget* parent,
            const QStyleOptionViewItem & option,
            const QModelIndex & index ) const;

signals:
    void change() const;

};

//! Class extends QSqlRelationalDelegate
class RelModifyDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    typedef QMap<int, QVariant::Type> TColMap;

    RelModifyDelegate(QObject* pObject = 0);

    void setReadOnly(bool readOnly);

    //! Set item's type for column
    /*!
        \param colMap a map QVariant::Type for columns
    */
    inline void SetColsType(const TColMap& colMap) { m_mapColType = colMap; }
    inline void setColumn(int col, QVariant::Type type) { m_mapColType.insert(col, type); }

    //! Inherited from QSqlRelationalDelegate
    virtual QWidget* createEditor(
            QWidget* parent,
            const QStyleOptionViewItem & option,
            const QModelIndex & index ) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    TColMap m_mapColType;
    bool mReadOnly;
};

//! Class extends CRelModifyDelegate
class NullDelegate:
        public RelModifyDelegate
{
public:
    NullDelegate(QObject* pObject = 0);

    //! Inherited from CRelModifyDelegate
    virtual QWidget* createEditor(
            QWidget* parent,
            const QStyleOptionViewItem & option,
            const QModelIndex & index ) const;


};

//! Class extends CRelModifyDelegate
class OctetDelegate:
        public RelModifyDelegate
{
Q_OBJECT
public:
    OctetDelegate(QObject* pObject = 0);

    //! Inherited from CRelModifyDelegate
    virtual QWidget* createEditor(
            QWidget* parent,
            const QStyleOptionViewItem & option,
            const QModelIndex & index ) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();
};

class UpperCaseDelegate : public RelModifyDelegate
{
    Q_OBJECT
public:
    explicit UpperCaseDelegate(QObject *parent = 0);
    explicit UpperCaseDelegate(int length, QObject *parent = 0);
    explicit UpperCaseDelegate(int minLength, int maxLength, QObject *parent = 0);

    virtual QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option
                                   , const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private slots:
    void onEdit(const QString &text);
    void commitAndCloseEditor();

private:
    const QString mRegExpPattern;
};

class YesNoDelegate : public RelModifyDelegate
{
    Q_OBJECT
public:
    explicit YesNoDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option
                                   , const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();

private:
    static const int kYes = 0;
    static const int kNo = 1;
};

#endif // VIEWDELEGATE_H
