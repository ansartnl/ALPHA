#ifndef _UIM_ACTION_UI_GROUP_H__
#define _UIM_ACTION_UI_GROUP_H__

#include <QtGui/QAction>


namespace Uim {

//! Does translation to current language for all actions.
void retranslateUi();

class CustomizeCmd : public QObject
{
    Q_OBJECT
public:
    static QAction* action(QObject *parent = 0);
};

}

#endif // _UIM_ACTION_UI_GROUP_H__
