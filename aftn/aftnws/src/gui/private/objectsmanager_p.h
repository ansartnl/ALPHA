#ifndef OBJECTSMANAGER_P_H
#define OBJECTSMANAGER_P_H

#include "objectsmanager.h"

#include <QtCore/QHash>
class QSignalMapper;

class StandardTreeModel;
class ObjectsManagerPrivate {
public:
  ObjectsManagerPrivate();
  virtual ~ObjectsManagerPrivate();

  QHash<QString, ObjectsManagerHelper*> helpers;
  StandardTreeModel *model;

  QSignalMapper* mapper;
};

#endif //#ifndef OBJECTSMANAGER_P_H
