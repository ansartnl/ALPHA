#ifndef SSAMODEL_H
#define SSAMODEL_H

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRelationalTableModel>

class RunwayModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit RunwayModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    //@{
    //! Inherited from QSqlTableModel
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QStringList mimeTypes() const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);
    virtual Qt::DropActions supportedDropActions() const;
    //@}

signals:
    void changedRunwaySSA(int newRunwayID, int oldRunwayID, QList<int> listSSA);
};

class SSAModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit SSAModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    //@{
    //! Inherited from QSqlRelationalTableModel
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    //@}
};

#endif // SSAMODEL_H
