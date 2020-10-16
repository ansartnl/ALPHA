#include "ssamodel.h"

#include <QDataStream>
#include <QSqlRecord>
#include <QStringList>
#include <QMimeData>
#include <QDebug>

const QString SSA_DRAG_AND_DROP_TYPE("arincmerge/ssa.id.list");
const QString SSA_TABLE_ID("id");
const QString SSA_TABLE_RUNWAY_ID("runway_id");
const QString RUNWAY_TABLE_ID("id");

// RunwayModel
RunwayModel::RunwayModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

Qt::ItemFlags RunwayModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSqlTableModel::flags(index);
    if(index.isValid())
    {
        //bool bOk = ("*" != headerData(index.row(), Qt::Vertical));
        bool bOk = (0 != record(index.row()).value(RUNWAY_TABLE_ID).toInt());
        if(bOk)
            defaultFlags |= Qt::ItemIsDropEnabled;
    }
    return defaultFlags;
}

QStringList RunwayModel::mimeTypes() const
{
    QStringList types;
    types << SSA_DRAG_AND_DROP_TYPE;
    return types;
}

bool RunwayModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action)
    Q_UNUSED(column)
    int iRow = row;
    if(parent.isValid())
        iRow = parent.row();
    if(iRow < 0 || iRow >= rowCount())
        return false;
    if(!data || !data->hasFormat(SSA_DRAG_AND_DROP_TYPE))
        return false;
    QByteArray encodedData = data->data(SSA_DRAG_AND_DROP_TYPE);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    int newRunwayID = record(iRow).value(RUNWAY_TABLE_ID).toInt();
    int oldRunwayID = 0;
    stream >> oldRunwayID;

    QList<int> listSSA;
    while(!stream.atEnd())
    {
        int iVal(0);
        stream >> iVal;
        listSSA.append(iVal);
    }
    emit changedRunwaySSA(newRunwayID, oldRunwayID, listSSA);
    return true;
}

Qt::DropActions RunwayModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}


// SSAModel
SSAModel::SSAModel(QObject *parent, QSqlDatabase db)
    : QSqlRelationalTableModel(parent, db)
{

}

Qt::ItemFlags SSAModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSqlRelationalTableModel::flags(index);
    if(index.isValid())
    {
        //bool bOk = ("*" != headerData(index.row(), Qt::Vertical));
        bool bOk = (0 != record(index.row()).value(SSA_TABLE_ID).toInt());
        if(bOk)
            defaultFlags |= Qt::ItemIsDragEnabled;
    }
    return defaultFlags;
}

QMimeData* SSAModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = 0;
    if(indexes.size())
    {
        mimeData = new QMimeData;
        QByteArray encodedData;
        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        stream << record(indexes.first().row()).value(SSA_TABLE_RUNWAY_ID).toInt();

        foreach(QModelIndex index, indexes)
        {
            if(index.isValid())
            {
                stream << record(index.row()).value(SSA_TABLE_ID).toInt();
            }
        }

        mimeData->setData(SSA_DRAG_AND_DROP_TYPE, encodedData);
    }
    return mimeData;
}
