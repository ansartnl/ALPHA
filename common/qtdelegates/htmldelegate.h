#ifndef _HTML_DELEGATE_H__
#define _HTML_DELEGATE_H__

#include <QPainter>
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QFontMetrics>
#include <QLineEdit>

#include <QLabel>
#include <QTreeView>

//! Html delegate - draw html text in the view's cell.
class HtmlDelegate : public QStyledItemDelegate
{
public:
    HtmlDelegate(QObject *parent = 0)
        : QStyledItemDelegate(parent)
    {}

    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        if ( !index.isValid() )
            return;

        QStyleOptionViewItemV4 options = option;

        initStyleOption(&options, index);

        painter->save();

        QTextDocument doc;
        doc.setHtml(options.text);

        // Draw simple, disabled text
        if ( !(QStyle::State_Enabled & option.state) )
        {
            options.text = doc.toPlainText();
            options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);
        }
        else
        {
            options.text = "";
            options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

            painter->translate(options.rect.left(), options.rect.top());
            QRect clip(0, 0, options.rect.width(), options.rect.height());

            // Draw html text
            if ( QStyle::State_Enabled & option.state )
            {
                doc.setTextWidth(options.rect.width());
                doc.drawContents(painter, clip);
            }
        }

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItemV4 optionV4 = option;
        initStyleOption(&optionV4, index);

        QTextDocument doc;
        doc.setHtml(optionV4.text);

        if ( !(QStyle::State_Enabled & option.state) )
            doc.setPlainText( doc.toPlainText() );

        doc.setTextWidth(optionV4.rect.width());
        return QSize(doc.idealWidth(), doc.size().height());
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QTextDocument doc;
        doc.setHtml(index.data().toString());

        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
        if ( lineEdit )
            lineEdit->setText( doc.toPlainText() );
    }
};


#endif // _HTML_DELEGATE_H__
