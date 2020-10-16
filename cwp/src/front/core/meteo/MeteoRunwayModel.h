#ifndef _METEO_RUNWAY_MODEL_H__
#define _METEO_RUNWAY_MODEL_H__

#include "import/MeteoRunway.h"

#include <QtCore/QAbstractItemModel>


//! Meteo information representation for one airport.
class MeteoRunwayModel : public QAbstractItemModel
{
public:
    typedef QList<MeteoRunway>  TMeteoList;

    explicit MeteoRunwayModel(const QString &currentRunway,
                              const TMeteoList &meteo,
                              QObject *parent = 0);

    //! Set new selected row (runway).
    void setCurrentRunway(const QString &name);
    //! Set new meteo information list.
    void setMeteo(const TMeteoList &meteo);

    //! Retrieve current meteo information.
    TMeteoList meteoList() const { return mMeteo; }

    //! Returns the data stored under the given role for the item referred to by the index.
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //! Returns the item flags for the given index.
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Returns the index of the item in the model specified by the given row, column and parent index.
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    //! Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned.
    virtual QModelIndex parent(const QModelIndex &child) const;

    /*! Returns the number of rows under the given parent. When the parent is valid it
        means that rowCount is returning the number of children of parent.
    */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //! Returns the number of columns for the children of the given parent.
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setEditEnabled(bool on) { mIsEditable = on; }

private:
    /** Airport name */
    QString     mAirportName;
    /** Current runway name */
    QString     mCurrentRunway;
    /** List about meteo information on all runways for specified airport */
    TMeteoList  mMeteo;
    /** True if editing is possible */
    bool        mIsEditable;
};

#endif // _METEO_RUNWAY_MODEL_H__
