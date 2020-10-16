#include "MeteoRunwayDelegate.h"

#include "import/MeteoRunway.h"

#include <QtGui/QRegExpValidator>
#include <QTimeEdit>
#include <QLineEdit>
#include <QSpinBox>

MeteoRunwayDelegate::MeteoRunwayDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget* MeteoRunwayDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option,
                                           const QModelIndex & index) const
{
    if ( index.column() < 0 || index.column() > 6 )
        return 0;

    int column = index.column();

    QWidget *editor = 0;

    if ( column == 0 )
    {
        QTimeEdit *timeEdit = new QTimeEdit(parent);
        timeEdit->setFrame(false);
        timeEdit->setDisplayFormat("HH:mm");

        editor = timeEdit;
    }
    else if ( column == 1 )
    {
        QRegExp rx("M\\d{0,3}-\\d*G\\d*/[\\d,.]*/[+-]?\\d*");
        QLineEdit *lineEdit = new QLineEdit(parent);
        QValidator *validator = new QRegExpValidator(rx, lineEdit);
        lineEdit->setValidator( validator );
        lineEdit->setInputMask("M009-009G009/009.09/#99;_");
        lineEdit->setFrame(false);

        editor = lineEdit;
    }
    else if ( column == 2 )
    {

        QRegExp rx("\\d{,4}");
        QLineEdit *lineEdit = new QLineEdit(parent);
        QValidator *validator = new QRegExpValidator(rx, lineEdit);
        lineEdit->setValidator( validator );
        lineEdit->setInputMask("0009;_");
        lineEdit->setFrame(false);

        editor = lineEdit;
    }
    else if ( column == 3 )
    {
        QRegExp rx("\\*\\d*/\\d*/\\d*");
        QLineEdit *lineEdit = new QLineEdit(parent);
        QValidator *validator = new QRegExpValidator(rx, lineEdit);
        lineEdit->setValidator( validator );
        lineEdit->setInputMask("*9999/9999/9999;_");
        lineEdit->setFrame(false);

        editor = lineEdit;
    }
    else if ( column == 4 )
    {
        QRegExp rx("\\d{,5}");
        QLineEdit *lineEdit = new QLineEdit(parent);
        QValidator *validator = new QRegExpValidator(rx, lineEdit);
        lineEdit->setValidator( validator );
        lineEdit->setInputMask("00009;_");
        lineEdit->setFrame(false);

        editor = lineEdit;
    }
    else if ( column == 5 )
    {
        QRegExp rx("\\d*\\(\\d*\\)");
        QLineEdit *lineEdit = new QLineEdit(parent);
        QValidator *validator = new QRegExpValidator(rx, lineEdit);
        lineEdit->setValidator( validator );
        lineEdit->setInputMask("009/0009(0009);_");
        lineEdit->setFrame(false);

        editor = lineEdit;
    }
    else if ( column == 6 )
    {
        QRegExp rx("[\\+\\-\\d\\.,]*/[\\+\\-\\d\\.,]*");
        QLineEdit *lineEdit = new QLineEdit(parent);
        QValidator *validator = new QRegExpValidator(rx, lineEdit);
        lineEdit->setValidator( validator );
        lineEdit->setInputMask("#00.0/#00.0;_");
        lineEdit->setFrame(false);

        editor = lineEdit;
    }

    return editor;
}

void MeteoRunwayDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ( index.column() < 0 || index.column() > 6 )
        return;

    int column = index.column();

    Q_ASSERT( index.internalPointer() );
    const MeteoRunway &meteo = *reinterpret_cast<const MeteoRunway *>( index.internalPointer() );

    if ( column == 0 )
    {
        QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>( editor );
        if ( meteo.contains(MeteoRunway::ControlTime) )
            timeEdit->setTime( meteo.value(MeteoRunway::ControlTime).toDateTime().time() );
    }
    else if ( column == 1 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>( editor );
        QString dataStr = index.model()->data(index).toString();
        // Replace "-" to "K" in order to save delimiter and minus sign.
        dataStr.replace(dataStr.indexOf("-"), 1, "K");
//        int posLastDash = dataStr.lastIndexOf("/");
//        if ( dataStr.mid(posLastDash+1).toInt() < 0 )
//            dataStr.replace(dataStr.indexOf("-", posLastDash+1), 1, "K");
//        dataStr.remove("-").replace("K", "-");
        dataStr.remove("---").remove("--").replace("K", "-");
        lineEdit->setText( dataStr );
    }
    else if ( column == 3 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>( editor );
        QString dataStr = index.model()->data(index).toString();
        dataStr.remove("-");
        lineEdit->setText( dataStr );
    }
    else if ( column == 2 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>( editor );
        QString dataStr = index.model()->data(index).toString();
        lineEdit->setText( dataStr );
    }
    else if ( column == 4 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>( editor );
        QString dataStr = index.model()->data(index).toString();
        lineEdit->setText( dataStr );
    }
    else if ( column == 5 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>( editor );
        QString dataStr = index.model()->data(index).toString();
        dataStr.remove("-");
        int pos1 = dataStr.indexOf("/");
        int pos2 = dataStr.indexOf("(");
        if ( pos1 > -1 && pos2 > -1 )
            dataStr.remove(pos1, pos2-pos1);
        lineEdit->setText( dataStr );
    }
    else if ( column == 6 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>( editor );
        QString dataStr = index.model()->data(index).toString();
        dataStr.remove("---").remove("--");
        lineEdit->setText( dataStr );
    }
}

void MeteoRunwayDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const
{
    int column = index.column();

    if ( column == 0 )
    {
        QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor);
        Q_ASSERT(timeEdit);

        model->setData(index, timeEdit->time());
    } else if (column == 2)
    {
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(editor);

        model->setData(index, lineedit->text());
    }
    else if ( column == 1 || column == 3 || column == 5 || column == 6 )
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        Q_ASSERT(lineEdit);

        model->setData(index, lineEdit->text());
    }
    else if ( column == 4 )
    {
        QLineEdit *lineedit = qobject_cast<QLineEdit *>(editor);

        model->setData(index, lineedit->text());
    }
}
