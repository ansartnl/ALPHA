#include "ocitableview.h"
#include "private/ocitableview_p.h"

OciTableView::OciTableView(QWidget *parent)
  :OciQueryView(*new OciTableViewPrivate, parent)
{
  init();
}

OciTableView::OciTableView(OciTableViewPrivate &dd, QWidget *parent)
  :OciQueryView(dd, parent)
{
  init();
}

void OciTableView::init()
{
  //Q_D(PGTableView);
  setObjectName("pgTableView");

  // Настройка таблицы
  EditTriggers triggers = AnyKeyPressed
                          | EditKeyPressed
                          | SelectedClicked
                          | DoubleClicked;
  setEditTriggers(triggers); // Редактирование если выделена только одна ячейка
  // А также вызываем редактирование в обработке MouseRelease

  setAcceptDrops(true);
  setDropIndicatorShown(true);
}

OciTableViewPrivate::OciTableViewPrivate()
  :OciQueryViewPrivate(){}
