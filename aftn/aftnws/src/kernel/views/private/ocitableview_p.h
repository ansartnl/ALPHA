#ifndef OCITABLEVIEW_P_H
#define OCITABLEVIEW_P_H

#include "ocitableview.h"
#include "private/ociqueryview_p.h"

class OciTableModel;

class OciTableViewPrivate : public OciQueryViewPrivate
{
  Q_DECLARE_PUBLIC(OciTableView)
public:
  OciTableViewPrivate();
};


#endif // OCITABLEVIEW_P_H
