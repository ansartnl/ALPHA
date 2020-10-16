#ifndef COMBOBOXDELEGATE_P_H
#define COMBOBOXDELEGATE_P_H

#include "comboboxdelegate.h"

#include <QtSql/QSqlDatabase>

class QRegExpValidator;
class ComboBoxDelegatePrivate
{
public:
  ComboBoxDelegatePrivate();
  virtual ~ComboBoxDelegatePrivate();

  bool editable;
  bool autoPopup;
  bool enableCompleter;
  ComboBoxDelegate::EndEditHint endEditHint;

  QAbstractItemModel *model;
  QPersistentModelIndex rootIndex;
  int keyColumn;
  int valueColumn;

  bool hideKeyColumn;

  QRegExpValidator *rexpValidator;
  QString inputMask;

  bool hideHorizontalHeader;
};

#endif

