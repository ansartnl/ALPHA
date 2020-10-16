#ifndef WIDGETSAVER_H
#define WIDGETSAVER_H

#include <QtCore/QSettings>
#include <QSplitter>

void SaveState(QWidget *object);
void RestoreState(QWidget *object);

#endif // WIDGETSAVER_H
