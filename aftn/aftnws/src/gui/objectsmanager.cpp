#include "objectsmanager.h"
#include "objectsmanager_p.h"
#include "objectsmanagerhelper.h"

#include "standardtreemodel.h"

#include "fplbrowser.h"
#include "fplhistorybrowser.h"
#include "fplonfuturebrowser.h"
#include "sendtelegrambrowser.h"
#include "receivedtelegrambrowser.h"
#include "zonebrowser.h"

#include "globalhistorybrowser.h"
#include "notambrowser.h"
#include "notamhistorybrowser.h"
#include "rplbrowser.h"
#include "zonebrowser.h"

#include "useraccountsdialog.h"
#include "useractionsbrowser.h"
#include "aftnservicelog.h"

#include "manualeditor.h"

#include "main.h"

#include <QtDebug>

#include <QtCore/QStringList>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QSignalMapper>


//! Add your cpp object helper here
void ObjectsManager::registerBuiltinObjects()
{
#if defined(MICROKOD_BUILD)
    registerObject(new SendTelegramBrowserHelper(this));
    registerObject(new ReceivedTelegramBrowserHelper(this));
    registerObject(new ManualTelegramEditorHelper(this));
#else
    registerObject(new FplBrowserHelper(this));
    registerObject(new FplHistoryBrowserHelper(this));
    registerObject(new GlobalHistoryBrowserHelper(this));

    registerObject(new ManualEditorHelper(this));

    registerObject(new FplOnFutureBrowserHelper(this));
    registerObject(new NotamBrowserHelper(this));
    registerObject(new NotamHistoryBrowserHelper(this));
    registerObject(new RplBrowserHelper(this));

    registerObject(new ZoneBrowserHelper(this));
    registerObject(new AirplaneTypesTableHelper(this));

    registerObject(new AFTNServiceLogHelper(this));
#endif

    registerObject(new UserAccountsDialogHelper(this));
    registerObject(new UserActionsBrowserHelper(this));
}


void ObjectsManager::init()
{
    Q_D(ObjectsManager);

    d->mapper = new QSignalMapper(this);
    connect(d->mapper, SIGNAL(mapped(QString)), this, SIGNAL(objectTriggered(QString)));

    registerBuiltinObjects();

    d->model = new StandardTreeModel(this);
    buildModel();
}

ObjectsManager::ObjectsManager(QObject* parent)
    :QObject(parent)
    , d_ptr(new ObjectsManagerPrivate)
{
    init();
}

ObjectsManager::ObjectsManager(ObjectsManagerPrivate& dd, QObject* parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    init();
}

ObjectsManager::~ObjectsManager()
{

}

QStringList ObjectsManager::objects() const
{
    Q_D(const ObjectsManager);
    return d->helpers.keys();
}

bool ObjectsManager::contains(const QString& id) const
{
    Q_D(const ObjectsManager);
    return d->helpers.contains(id);
}

bool ObjectsManager::allow(const QString& id) const
{
    if (currentUserId == SUPERUSER_ID)
        return true;
    return (currentPermissions.value(id, PermissionDisabled) != PermissionDisabled);
}

ObjectsManagerHelper* ObjectsManager::helperFor(const QString& id) const
{
    Q_D(const ObjectsManager);
    return d->helpers.value(id, 0);
}

ObjectsManagerHelper* ObjectsManager::helperFor(const QModelIndex& index) const
{
    Q_D(const ObjectsManager);
    QModelIndex firstColumnIndex = index.sibling(index.row(), 0);
    QStandardItem* item = d->model->itemFromIndex(firstColumnIndex);
    QString path = d->model->pathFromItem(item);

    QHash<QString, ObjectsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
    QHash<QString, ObjectsManagerHelper*>::ConstIterator end = d->helpers.constEnd();

    for (; it != end; ++it) {
        if (it.value()->path() == path)
            return it.value();
    }

    return 0;
}

QAbstractItemModel* ObjectsManager::model() const
{
    Q_D(const ObjectsManager);
    return d->model;
}

void ObjectsManager::buildModel()
{
    Q_D(ObjectsManager);

    // clear model
    QHash<QString, ObjectsManagerHelper*>::iterator it = d->helpers.begin();
    QHash<QString, ObjectsManagerHelper*>::iterator end = d->helpers.end();
    for (; it != end; ++it)
        it.value()->item = 0;

    d->model->clear();

    it = d->helpers.begin();

    QStandardItem* item;
    QStandardItem* parent;
    QStandardItem* descriptionItem;

    /// superuser rights
    if (currentUserId == SUPERUSER_ID) {
        for (; it != end; ++it) {
            item = d->model->itemFromPath(it.value()->path());
            it.value()->item = item;
            item->setIcon(it.value()->icon());
            item->setToolTip(it.value()->description());
            item->setStatusTip(it.value()->description());

            parent = item->parent();

            if (!it.value()->description().isEmpty()) {
                descriptionItem = new QStandardItem(it.value()->description());
                descriptionItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                parent->setChild(item->row(), 1, descriptionItem);
            }
        }

        ///
    } else {
        for (; it != end; ++it) {
            switch  (currentPermissions.value(it.value()->id(), PermissionDisabled)) {
            case PermissionDisabled:
                continue;
                break;
            case PermissionEnabled:
            default:
                item = d->model->itemFromPath(it.value()->path());
                it.value()->item = item;

                item->setIcon(it.value()->icon());
                item->setToolTip(it.value()->description());
                item->setStatusTip(it.value()->description());

                parent = item->parent();

                if (!it.value()->description().isEmpty()) {
                    descriptionItem = new QStandardItem(it.value()->description());
                    descriptionItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                    parent->setChild(item->row(), 1, descriptionItem);
                }

                break;
            };
        }
    }

    QStringList labels = QStringList() << tr("Name") << tr("Description");
    d->model->setHorizontalHeaderLabels(labels);
}

QString ObjectsManager::objectIdFromIndex(const QModelIndex& index) const
{
    Q_D(const ObjectsManager);
    QModelIndex firstColumnIndex = index.sibling(index.row(), 0);

    QStandardItem* item = d->model->itemFromIndex(firstColumnIndex);

    QHash<QString, ObjectsManagerHelper*>::ConstIterator it = d->helpers.constBegin();
    QHash<QString, ObjectsManagerHelper*>::ConstIterator end = d->helpers.constEnd();

    for (; it != end; ++it) {
        if (it.value()->item == item)
            return it.key();
    }

    return QString::null;
}

QModelIndex ObjectsManager::indexFromId(const QString& id) const
{
    Q_D(const ObjectsManager);

    if (d->helpers.contains(id))
        if (d->helpers.value(id)->item)
            return d->helpers.value(id)->item->index();

    return QModelIndex();
}

QWidget* ObjectsManager::widget(const QString& id, QWidget* parent) const
{
    ObjectsManagerHelper* helper = helperFor(id);

    if (helper)
        return helper->widget(parent);

    return 0;
}

QWidget* ObjectsManager::widget(const QModelIndex& index, QWidget* parent) const
{
    ObjectsManagerHelper* helper = helperFor(index);

    if (helper)
        return helper->widget(parent);

    return 0;
}

QString ObjectsManager::objectName(const QString& id) const
{
    Q_D(const ObjectsManager);

    if (d->helpers.contains(id)) {
        return d->helpers.value(id)->name();
    }

    return QString::null;
}

QString ObjectsManager::objectDescription(const QString& id)
{
    Q_D(const ObjectsManager);

    if (d->helpers.contains(id)) {
        return d->helpers.value(id)->description();
    }

    return QString::null;
}

void ObjectsManager::registerObject(ObjectsManagerHelper* helper)
{
    Q_D(ObjectsManager);

    if (validateObject(helper)) {
        d->helpers.insert(helper->id(), helper);
        QAction* action = helper->action();
        connect(action, SIGNAL(triggered()), d->mapper, SLOT(map()));
        d->mapper->setMapping(action, helper->id());
    }
}

bool ObjectsManager::validateObject(ObjectsManagerHelper* helper)
{
    bool result = true;

    if (helper->id().isEmpty()) {
        qWarning() << helper->metaObject()->className() << "-" << helper->objectName()
                << "have not valid id";
        result = false;
    }

    if (helper->name().isEmpty()) {
        qWarning() << helper->metaObject()->className() << "-" << helper->objectName()
                << "have not valid name";
        result = false;
    }

    if (helper->path().isEmpty()) {
        qWarning() << helper->metaObject()->className() << "-" << helper->objectName()
                << "have not valid path";
        result = false;
    }

    return result;
}

void ObjectsManager::retranslateUi()
{
    Q_D(ObjectsManager);
    foreach(ObjectsManagerHelper *helper, d->helpers.values())
        helper->retranslateUi();
    buildModel();
}

QAction* ObjectsManager::action(const QString& id) const
{
    Q_D(const ObjectsManager);

    if (d->helpers.contains(id))
        return d->helpers.value(id)->action();

    return 0;
}

QList<QAction*> ObjectsManager::actions() const
{
    Q_D(const ObjectsManager);
    QList<QAction*> result;

    if (currentUserId == SUPERUSER_ID) {
        foreach (ObjectsManagerHelper *helper, d->helpers.values())
            result.append(helper->action());
    } else {
        foreach (ObjectsManagerHelper *helper, d->helpers.values()) {
            switch  (currentPermissions.value(helper->id(), PermissionDisabled)) {
            case PermissionDisabled:
                continue;
                break;
            case PermissionEnabled:
            default:
                result.append(helper->action());
                break;
            };
        }
    }

    return result;
}


void ObjectsManager::refresh()
{
    buildModel();
}

ObjectsManager* ObjectsManager::instance()
{
    static ObjectsManager instance;

    return &instance;
}

ObjectsManagerPrivate::ObjectsManagerPrivate() {}
ObjectsManagerPrivate::~ObjectsManagerPrivate() {}

