#ifndef PGTABLEVIEW_H
#define PGTABLEVIEW_H

#include "ociqueryview.h"

/*!
*/
class OciTableViewPrivate;
/**
* @brief Table view for OciTableModel
*
* @class OciTableView ocitableview.h
*/
class OciTableView : public OciQueryView
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(OciTableView)

  void init();
protected:
  OciTableView(OciTableViewPrivate &dd, QWidget* parent);
public:
  explicit OciTableView(QWidget *parent = 0);
};

#endif // PGTABLEVIEW_H
