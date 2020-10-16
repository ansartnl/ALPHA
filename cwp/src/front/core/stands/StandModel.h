#ifndef STAND_MODEL_H
#define STAND_MODEL_H

#include <QAbstractTableModel>
#include "front/common/XAppSettings.h"
#include "GroundStand.h"

//! Model for stands
class StandModel : public QAbstractTableModel
		, public AppSettings
{
	Q_OBJECT
	
    static const int COL_COUNT = 3;

public:
    typedef QList<QSharedPointer<GroundStand> >	TListData;

    //! Constructor
    explicit StandModel(QObject *parent = 0);
    ~StandModel();
    //! Clear model
    void Clear();
    //! Set data in model
    void SetModelData(const TListData& pListData);
    void SetModelData(const QSharedPointer<GroundStand>& pData);

    //@{
    //! Inherited from QAbstractTableModel
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void sort(int column, Qt::SortOrder order);
    //@}

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}

private:
    QVariant GetItemText(const QModelIndex &index) const;

private:
    TListData				m_listData;
};

class StandLessThan
{
public:
    StandLessThan(int column, Qt::SortOrder order);
    bool operator ()(const QSharedPointer<GroundStand>& left, const QSharedPointer<GroundStand>& right) const;
private:
    int             mColumn;
    Qt::SortOrder   mOrder;
};

#endif // STAND_MODEL_H
