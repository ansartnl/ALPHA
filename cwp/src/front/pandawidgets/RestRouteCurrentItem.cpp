#include "RestRouteCurrentItem.h"

RestRouteCurrentItem::RestRouteCurrentItem(CRestrictionUser *item, int index)
{
    m_item = item;
    m_index = index;
    setData(Qt::DisplayRole, QString::fromStdString(m_item->route_name) + QString(" ") + QString::fromStdString(m_item->segment.first)
            + QString(" - ") + QString::fromStdString(m_item->segment.second));
}

int RestRouteCurrentItem::index()
{
    return m_index;
}

