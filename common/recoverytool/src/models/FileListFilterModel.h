#ifndef FILELISTFILTERMODEL_H
#define FILELISTFILTERMODEL_H

#include <QSortFilterProxyModel>

class CFileListFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CFileListFilterModel(QObject *parent = 0);

    void setMask(QRegExp &mask){mMask = mask;}
    
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    
public slots:

private:
    QRegExp mMask;
    
};

#endif // FILELISTFILTERMODEL_H
