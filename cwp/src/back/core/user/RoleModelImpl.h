#ifndef ROLEMODELIMPL_H
#define ROLEMODELIMPL_H

#include "RoleModel.h"
#include "core/User.h"
#include "Dao.h"

//! Tree model
class CRoleModel
    : public QStandardItemModel
{
    Q_OBJECT
public:
    // Menu
    static const char MENU_NAME[];
    static const char MENU_DESCR[];
    static const char MENU_UNIQUE[];
    // Operations submenu
    static const char MENUOPER_NAME[];
    static const char MENUOPER_DESCR[];
    static const char MENUOPER_UNIQUE[];
    // User admin submenu
    static const char MENUUSER_NAME[];
    static const char MENUUSER_DESCR[];
    static const char MENUUSER_UNIQUE[];
    // Other submenus
    static const char MENUOTHER_NAME[];
    static const char MENUOTHER_DESCR[];
    static const char MENUOTHER_UNIQUE[];
    // Sectoring
    static const char SECTORING_NAME[];
    static const char SECTORING_DESCR[];
    static const char SECTORING_UNIQUE[];

    CRoleModel(QObject* parent = 0);
    virtual ~CRoleModel();

    void BuildTree();
    void retranslate();

private:
    void translateItem(QStandardItem* item);
    void _SetHeader();
};

#endif // ROLEMODELIMPL_H
