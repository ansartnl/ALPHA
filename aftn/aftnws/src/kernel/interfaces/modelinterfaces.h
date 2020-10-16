#ifndef MODELINTERFACES_H
#define MODELINTERFACES_H

#include <QtCore/QObject>

#include <QtGui/QUndoStack>

/**
* @brief Интерфейс для модели позволяющей обновление
*
* @class IRefreshModel modelinterfaces.h
*/
class IRefreshModel
{
public :
		virtual void refresh() = 0;
};

Q_DECLARE_INTERFACE(IRefreshModel, "com.filonenko.IRefreshModel/1.0")

/**
* @brief Интерфейс для модели позволяющей undo-redo
*
* @class IUndoRedoModel modelinterfaces.h
*/
class IUndoRedoModel
{
public:
  // can not be null
  virtual QUndoStack * undoStack() const = 0;
};

Q_DECLARE_INTERFACE(IUndoRedoModel, "com.filonenko.IUndoRedoModel/1.0")

#endif // #ifndef MODELINTERFACES_H
