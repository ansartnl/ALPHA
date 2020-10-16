#include "RoleModelImpl.h"
#include "Sectoring.h"

#include "main.h"

const char CRoleModel::MENU_NAME[] = "Menu";
const char CRoleModel::MENU_DESCR[] = "Main menu";
const char CRoleModel::MENU_UNIQUE[] = "main menu";
const char CRoleModel::MENUOPER_NAME[] = "Operations";
const char CRoleModel::MENUOPER_DESCR[] = "Operation submenu";
const char CRoleModel::MENUOPER_UNIQUE[] = "operation menu";
const char CRoleModel::MENUUSER_NAME[] = "Users";
const char CRoleModel::MENUUSER_DESCR[] = "User administration";
const char CRoleModel::MENUUSER_UNIQUE[] = "user administration";
const char CRoleModel::MENUOTHER_NAME[] = "Other";
const char CRoleModel::MENUOTHER_DESCR[] = "Other menus";
const char CRoleModel::MENUOTHER_UNIQUE[] = "other menu";
const char CRoleModel::SECTORING_NAME[] = "Sectors group";
const char CRoleModel::SECTORING_DESCR[] = "";
const char CRoleModel::SECTORING_UNIQUE[] = "Sectoring";

static void unusedTrWorkaround()
{
    CRoleModel::tr("Menu");
    CRoleModel::tr("Main menu");
    CRoleModel::tr("main menu");
    CRoleModel::tr("Operations");
    CRoleModel::tr("Operation submenu");
    CRoleModel::tr("operation menu");
    CRoleModel::tr("Users");
    CRoleModel::tr("User administration");
    CRoleModel::tr("user administration");
    CRoleModel::tr("Other");
    CRoleModel::tr("Other menus");
    CRoleModel::tr("other menu");
    CRoleModel::tr("Sectors group");
    CRoleModel::tr("");
    CRoleModel::tr("Sectoring");
}

CRoleModel::CRoleModel(QObject *parent) :
    QStandardItemModel(parent)
{
    unusedTrWorkaround();
    BuildTree();
}

CRoleModel::~CRoleModel()
{
}

void CRoleModel::BuildTree()
{
    _SetHeader();
    QStandardItem *parentItem = invisibleRootItem();
    // 1 Level
    // Menu
    QStandardItem* pDescr = new QStandardItem();
    CRoleItem* pMenuItem = new CRoleItem(pDescr, MENU_UNIQUE, MENU_NAME,
        MENU_DESCR);
    parentItem->setChild(0, 0, pMenuItem);
    parentItem->setChild(0, 1, pDescr);

    // Sectoring
    pDescr = new QStandardItem();
    QStandardItem* pSectorItem = new QStandardItem(tr(SECTORING_NAME));
    parentItem->setChild(1, 0, pSectorItem);
    parentItem->setChild(1, 1, pDescr);
    Dao::TSectoringList SectoringList;
    TVA_CATCHALL_TRY
    {
        dao()->LoadSectoringAll(SectoringList);
    }TVA_CATCHALL(err) { LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err); }
    Dao::TSectoringList::const_iterator itSectoring = SectoringList.begin();
    for (int iInd = 0; itSectoring != SectoringList.end(); ++itSectoring, ++iInd)
    {
        QStandardItem* pDescr = new QStandardItem();
        CRoleItem* pSItem =
                new CRoleItem(pDescr, SECTORING_UNIQUE,
                    (*itSectoring)->GetName(), SECTORING_DESCR,
                    (*itSectoring)->GetID());
        pSectorItem->setChild(iInd, 0, pSItem);
        pSectorItem->setChild(iInd, 1, pDescr);
    }
    retranslate();
}

void CRoleModel::retranslate()
{
    _SetHeader();
    QStandardItem* item = invisibleRootItem();
    for (int i = 0; i < item->rowCount(); ++i)
        translateItem(item->child(i));
}

void CRoleModel::translateItem(QStandardItem* item)
{
    CRoleItem* roleItem = dynamic_cast<CRoleItem *>(item);
    if (roleItem)
    {
        roleItem->setText(tr(roleItem->GetName().toLatin1()));
        if (roleItem->m_pDescr != NULL && !roleItem->GetDescription().isEmpty())
            roleItem->m_pDescr->setText(tr(roleItem->GetDescription().toLatin1()));
    }

    for (int i = 0; i < item->rowCount(); ++i)
        translateItem(item->child(i));
}

void CRoleModel::_SetHeader()
{
    QStringList sHeader;
    sHeader << tr("Permission") << tr("Description");
    setHorizontalHeaderLabels(sHeader);
}
