#ifndef OCIQUERYVIEW_P_H
#define OCIQUERYVIEW_P_H

#include "qtsqltableview_p.h"
#include "ociqueryview.h"

#include <QtSql/QSqlIndex>

class OciQueryModel;
class OciQueryViewPrivate : public QtSqlTableViewPrivate
{
  Q_DECLARE_PUBLIC(OciQueryView)

public:
  OciQueryViewPrivate();
  virtual ~OciQueryViewPrivate();

  OciQueryModel *model;

  QSqlIndex lastUsed;
  int lastUsedColumn;
  QPoint lastIndexPos;
};

#endif // OCIQUERYVIEW_P_H
