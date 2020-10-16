#ifndef RESTROUTECURRENTITEM_H
#define RESTROUTECURRENTITEM_H

#include <QListWidgetItem>

#include "Restriction.h"

class RestRouteCurrentItem : public QListWidgetItem
{
public:
    RestRouteCurrentItem(CRestrictionUser *item, int index);

    int index();

private:
    CRestrictionUser *m_item;
    int m_index;
};

#endif // RESTROUTECURRENTITEM_H
