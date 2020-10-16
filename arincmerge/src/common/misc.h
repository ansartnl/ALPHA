#ifndef _MISC_H__
#define _MISC_H__

#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QWidget>


//! Retrieve points from clipboard.
QList<QPointF> getPointsFromClipboard(QWidget *parent);

#endif // _MISC_H__
