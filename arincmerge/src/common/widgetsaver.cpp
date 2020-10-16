#include "widgetsaver.h"

void SaveState(QWidget *object)
{
    QSettings settings;
    Q_ASSERT(!object->objectName().isEmpty());
    settings.beginGroup(object->objectName());
    settings.setValue("geometry", object->saveGeometry().toHex());
    QList<QSplitter *> splitters = object->findChildren<QSplitter *>();
    foreach(const QSplitter* splitter, splitters)
    {
        QString name = splitter->objectName();
        if (!name.isEmpty())
            settings.setValue(name, splitter->saveState().toHex());
    }
    settings.endGroup();
}

void RestoreState(QWidget *object)
{
    QSettings settings;
    Q_ASSERT(!object->objectName().isEmpty());
    settings.beginGroup(object->objectName());
    object->restoreGeometry(QByteArray::fromHex(settings.value("geometry").toByteArray()));
    QList<QSplitter *> splitters = object->findChildren<QSplitter *>();
    foreach(QSplitter* splitter, splitters)
    {
        QString name = splitter->objectName();
        if (!name.isEmpty())
            splitter->restoreState(QByteArray::fromHex(settings.value(name).toByteArray()));
    }
    settings.endGroup();
}
