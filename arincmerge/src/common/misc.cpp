#include "misc.h"

#include <QApplication>
#include <QClipboard>
#include <QtCore/QMimeData>
#include <QMessageBox>
#include <QtCore/QObject>
#include <QtCore/QTextStream>


QList<QPointF> getPointsFromClipboard(QWidget *parent)
{
    typedef QList<QPointF>  TCoordList;
    TCoordList coords;

    QClipboard *clipboard = QApplication::clipboard();
    if ( !clipboard->mimeData() || !clipboard->mimeData()->hasText() )
    {
        QMessageBox::information(parent, QObject::tr("Paste from Clipboard"),
                                 QObject::tr("Unknown data in the clipboard"));
        return coords;
    }

    QString textClp = clipboard->text();
    QTextStream stream(&textClp);
    while ( !stream.atEnd() )
    {
        QString lat, lon;
        stream >> lat >> lon;
        if ( QTextStream::Ok != stream.status() )
            continue;

        coords << QPointF(lat.toDouble(), lon.toDouble());
    }

    return coords;
}
