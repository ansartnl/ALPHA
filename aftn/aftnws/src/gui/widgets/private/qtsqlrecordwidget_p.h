#ifndef QTSQLRECORDWIDGET_P_H
#define QTSQLRECORDWIDGET_P_H

#include "qtsqlrecordwidget.h"

#include <QtCore/QHash>

#include <QtSql/QSqlRecord>

class QUiLoader;

class QtSqlRecordWidgetPrivate
{

public:
   QtSqlRecordWidgetPrivate();
   virtual ~QtSqlRecordWidgetPrivate();

   QString uiFileName;
   QSqlRecord originalRecord;
   QSqlRecord dirtyRecord;

   QUiLoader *loader;
   QWidget *loadedWidget;

   QHash<QString, QWidget*> children;
};

#endif //QTSQLRECORDWIDGET_P_H
