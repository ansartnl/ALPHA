#include "StandModel.h"
#include <QBrush>

StandLessThan::StandLessThan(int column, Qt::SortOrder order)
    : mColumn(column), mOrder(order)
{

}

bool StandLessThan::operator ()(const QSharedPointer<GroundStand>& left, const QSharedPointer<GroundStand>& right) const
{
    if(left && right)
    {
        const QSharedPointer<GroundStand>& p1 = mOrder == Qt::AscendingOrder ? left : right;
        const QSharedPointer<GroundStand>& p2 = mOrder == Qt::AscendingOrder ? right : left;
        switch(mColumn)
        {
        case 0: return p1->mStandName < p2->mStandName;
        case 1: return p1->mStateId < p2->mStateId;
        case 2: return p1->mFplName < p2->mFplName;
        default: return p1->mId < p2->mId;
        }
    }
    return false;
}

StandModel::StandModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

StandModel::~StandModel()
{
    Clear();
}

void StandModel::Clear()
{
    beginResetModel();
    m_listData.clear();
    endResetModel();
}

void StandModel::SetModelData(const TListData& pListData)
{
    beginResetModel();
    m_listData = pListData;
    endResetModel();
}

void StandModel::SetModelData(const QSharedPointer<GroundStand>& pData)
{
    if(pData)
    {
        emit layoutAboutToBeChanged();
        bool bFound = false;
        for(int i = 0; i < m_listData.size(); ++i)
        {
            if(m_listData[i]->mId == pData->mId)
            {
                bFound = true;
                m_listData[i] = pData;
                break;
            }
        }
        if(!bFound)
            m_listData.append(pData);
        emit layoutChanged();
    }
}

int StandModel::rowCount(const QModelIndex &parent) const
{
    return m_listData.size();
}

int StandModel::columnCount(const QModelIndex &parent) const
{
    return COL_COUNT;
}

void StandModel::sort(int column, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();
    qSort(m_listData.begin(), m_listData.end(), StandLessThan(column, order));
    emit layoutChanged();
}

QVariant StandModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();

    switch(role)
    {
    case Qt::TextAlignmentRole:
        return int(Qt::AlignLeft|Qt::AlignVCenter);
    case Qt::DisplayRole:
        return GetItemText(index);
    case Qt::BackgroundRole:
    {
        QBrush backBrush(Qt::NoBrush);
        QSharedPointer<GroundStand> pData;
        if(index.row() >= 0 && index.row() < m_listData.size())
        {
            pData = m_listData[index.row()];
            if(pData && pData->mStateId)
                backBrush = QBrush(QColor(255,0,0,120));
        }
        return backBrush;
    }
    }
    return QVariant();
}

bool StandModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QAbstractTableModel::setData(index, value, role);
}

QVariant StandModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        if((section >= 0) && (section < COL_COUNT))
        {
            switch(section)
            {
            case 0: return tr("Stand");
            case 1: return tr("State");
            case 2: return tr("Aircraft");
            }
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant StandModel::GetItemText(const QModelIndex &index) const
{
    if(!index.isValid()) return QVariant();
    QSharedPointer<GroundStand> pData;
    if(index.row() >= 0 && index.row() < m_listData.size())
        pData = m_listData[index.row()];
    switch(index.column())
    {
    case 0:
        return pData->mStandName;
    case 1:
    {
        QString sText;
        if(pData->mStateId == 0)
            sText = tr("Free");
        else if(pData->mStateId == 1)
            sText = tr("Occupied");
        else if(pData->mStateId == 2)
            sText = tr("Closed");
        return sText;
    }
    case 2:
        return pData->mFplName;
    }
    return QVariant();
}

void StandModel::Translate()
{
    emit headerDataChanged(Qt::Horizontal, 0, COL_COUNT-1);
}
