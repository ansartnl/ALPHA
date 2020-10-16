#ifndef TABLEDELEGATE_P_H
#define TABLEDELEGATE_P_H
#include "tabledelegate.h"

class TableDelegatePrivate
{
  public:
    TableDelegatePrivate();
    virtual ~TableDelegatePrivate();

    bool hideHorizontalHeader;
    bool hideKeyColumn;
    QAbstractItemDelegate::EndEditHint endEditHint;

    struct ModelInfo{
      ModelInfo():keyColumn(0){}
      ModelInfo(const ModelInfo& other):tag(other.tag),keyColumn(other.keyColumn),rootIndex(other.rootIndex){}
      ModelInfo &operator = (const ModelInfo& other) {
        tag = other.tag;keyColumn = other.keyColumn;rootIndex = other.rootIndex;
        return *this;
      }

      bool operator == (const ModelInfo& other) {return tag == other.tag
                                                 && keyColumn == other.keyColumn
                                                 && rootIndex == other.rootIndex;}
      QString tag;
      int keyColumn;
      QPersistentModelIndex rootIndex;
    };

    QList<QAbstractItemModel*> modelsOrder;
    QHash<QAbstractItemModel*, ModelInfo> models;

    void addModel(QAbstractItemModel* model);
    void insertModel(QAbstractItemModel* before, QAbstractItemModel* model);
    void removeModel(QAbstractItemModel* model);

    ModelInfo& modelInfo(QAbstractItemModel* model);
    ModelInfo modelInfo(QAbstractItemModel *model) const;
};

#endif //TABLEDELEGATE_P_H
