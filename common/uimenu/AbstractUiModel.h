#ifndef _ABSTRACT_UI_MODEL_H__
#define _ABSTRACT_UI_MODEL_H__

#include <QtCore/QStringList>
#include <QtGui/QIcon>
#include <QtGui/QAction>

namespace Uim {

//! Abstract interface for user defined actions and menus.
class AbstractUiModel
{
public:
    //! Returns list of category names.
    virtual QStringList categories() const = 0;
    //! Returns list of action class names.
    virtual QStringList actions(const QString &categoryName = QString()) const = 0;

    //! Finds Qt action associated with the class name of action.
    virtual QAction* findAction(const QString &actionClassName) const = 0;

    //! Returns list of prebuilt menu class names.
    virtual QStringList menus() const = 0;

    //! Find prebuilt Qt menu object associated with the input class name.
    virtual QMenu* findMenu(const QString &menuClassName) const = 0;
};

}   // namespace Uim

#endif // _ABSTRACT_UI_MODEL_H__
