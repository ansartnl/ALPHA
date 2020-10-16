#include "ListACItemDelegate.h"
#include "ControlDlg.h"
#include "back/core/Airplane.h"
#include "ListACData.h"

#include "EchelonsWidget.h"

#include "main.h"

#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>

#include <QCompleter>
#include <QLineEdit>
#include <QTimeEdit>
#include <QApplication>
#include <QDesktopWidget>

#include "utils/ProfileSettings.h"

CListACItemDelegate::CListACItemDelegate(const QMap<int, int>& colType, QObject *parent) :
    QStyledItemDelegate(parent), m_mapColType(colType), m_bSignalEmitted(false)
{
    m_HighTextColor = QColor(0,127,0,255);
    UserProfile::CSettings& stg = UserProfile::CSettings::Instance();
    m_HighTextColor = stg.GetColor(UserProfile::CtListSelectedText);
}

QWidget* CListACItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    const_cast<CListACItemDelegate*>(this)->m_bSignalEmitted = false;
    if (m_mapColType.contains(index.column()))
    {
        switch (m_mapColType[index.column()])
        {
        case CControlDlg::FPLType:
            {
                //bool bEdit = false;
                //rem for dbclick editing emit const_cast<CListACItemDelegate*> (this)->itemEdit(index, bEdit);
                return 0;
            }
            break;
        case CControlDlg::CODEType:
        {
            QString sText;
            /*
            QStringList sValues;
            QSharedPointer<CAirplane> pPlane = index.data(Qt::UserRole).value<CListACData> ().GetAirplane();
            if (pPlane)
                sValues << QString::number(pPlane->GetMode3AReplyInit(), 8).rightJustified(4, '0');
            else
                sValues << index.data().toString();
            sValues << CAirplane::sListSpecCodes;
            */
            sText = index.data().toString();
            QLineEdit* pEdit = new QLineEdit(parent);
            pEdit->setText(sText);
            QRegExp regExp("[0-7]{1,4}");
            pEdit->setValidator(new QRegExpValidator(regExp, pEdit));
            /*
            QCompleter* pCompleter = new QCompleter(sValues, pEdit);
            pCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
            pEdit->setCompleter(pCompleter);
            */
            connect(pEdit, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
            //ShowEditCompleter(pEdit);
            return pEdit;
        }
            break;
        case CControlDlg::CFLType:
            {
                bool bEdit = false;
                emit const_cast<CListACItemDelegate*> (this)->itemEdit(index, bEdit);
                if(!bEdit) return 0;

                int levelMetersMax = INT_MAX;
                QSharedPointer<CSFPL> pSFPL = index.data(Qt::UserRole).value<CListACData> ().GetSFPL();
                if(pSFPL && pSFPL->GetAirplanePtr())
                    levelMetersMax = pSFPL->GetAirplanePtr()->GetLevelMax();
                EchelonsWidget::Measure measure = EchelonsWidget::Feet;
                bool bButtons = false;
                QRegExp rexp("([F,M]{1,1})([F,M]{0,1})");
                if(rexp.exactMatch(networkSettings()->value("airplane/FL_SI", "F").toString()))
                {
                    if(!rexp.cap(1).isEmpty())
                    {
                        if("F" == rexp.cap(1))
                            measure = EchelonsWidget::Feet;
                        else if("M" == rexp.cap(1))
                            measure = EchelonsWidget::Meters;

                        if(!rexp.cap(2).isEmpty())
                            bButtons = true;
                    }
                }
                EchelonsWidget *levels = new EchelonsWidget(measure,
                //EchelonsWidget *levels = new EchelonsWidget(EchelonsWidget::Measure(AppSettings::IsSISet(AppSettings::SIFeet)),
                    levelMetersMax, QSqlDatabase::database(), parent);
                levels->setVisibleButtons(bButtons);
                levels->setRemainderShown(false);
                QVariant val = index.data(Qt::EditRole);
                if (levels && !val.isNull())
                {
                    //levels->setValue(val.value<FLVal>().toString());

                    FLVal flval(val.value<FLVal>());
                    if(bButtons)
                        levels->setValue(flval.toString());
                    else
                    {
                        if(levels->measure() == EchelonsWidget::Feet && flval.isFeet())
                            levels->setValue(flval.GetF());
                        if(levels->measure() == EchelonsWidget::Meters && !flval.isFeet())
                            levels->setValue(flval.GetM());
                    }
                }
                connect(levels, SIGNAL(valueChanged(double, double, int)), this, SLOT(commitAndCloseEditor()));
                return levels;
            }
            break;
        case CControlDlg::ETDType:
            {
                bool bEdit = false;
                emit const_cast<CListACItemDelegate*> (this)->itemEdit(index, bEdit);
                if(bEdit)
                {
                    QTimeEdit* timeEdit = new QTimeEdit(parent);
                    timeEdit->setDisplayFormat("hh:mm");
                    QString sTime = index.data().toString();
                    timeEdit->setTime(QTime::fromString(sTime, "hh:mm"));
                    connect(timeEdit, SIGNAL(editingFinished()),
                            this, SLOT(commitAndCloseEditor()));
                    return timeEdit;
                }
                return 0;
            }
            break;
        case CControlDlg::STARType:
            {
                QSharedPointer<CSFPL> pSFPL = index.data(Qt::UserRole).value<CListACData>().GetSFPL();
                if(pSFPL)
                {
                    QStringList sValues1, sValues2;
                    QString sText;
                    sValues1.append(pSFPL->GetDestAerodrome());
                    sText = index.data().toString();
                    CControlDlg* pWnd = new CControlDlg(CControlDlg::STARType, parent);
                    pWnd->SetValues(sValues1, sValues2);
                    pWnd->Init(sText);
                    connect(pWnd, SIGNAL(endEdit()), this, SLOT(commitAndCloseEditor()));
                    return pWnd;
                }
            }
            break;
        }
        return 0;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void CListACItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    if (m_mapColType.contains(index.column()))
    {
        switch (m_mapColType[index.column()])
        {

        case CControlDlg::CFLType:
        {
            EchelonsWidget *levels = qobject_cast<EchelonsWidget*> (editor);
            //levels->move(editor->parentWidget()->mapToGlobal(option.rect.topLeft()));
            QRect rect;
            rect.setWidth(editor->frameGeometry().width());
            rect.setHeight(editor->frameGeometry().height());
            levels->move(isRectVisible(editor->parentWidget()->mapToGlobal(option.rect.topLeft()), editor->parentWidget()->mapToGlobal(option.rect.bottomLeft()), rect));
        }
            break;
        case CControlDlg::STARType:
        {
            editor->resize(130, 200);
            //editor->move(editor->parentWidget()->mapToGlobal(option.rect.topLeft()));
            QRect rect;
            rect.setWidth(editor->frameGeometry().width());
            rect.setHeight(editor->frameGeometry().height());
            editor->move(isRectVisible(editor->parentWidget()->mapToGlobal(option.rect.topLeft()), editor->parentWidget()->mapToGlobal(option.rect.bottomLeft()), rect));
        }
            break;
        default:
            QStyledItemDelegate::updateEditorGeometry(editor, option, index);
            break;
        }
    }
    else
    {
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);
    }
}

void CListACItemDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
    if (m_mapColType.contains(index.column()))
    {
        switch (m_mapColType[index.column()])
        {
        case CControlDlg::CFLType:
        {
                /*
            EchelonsWidget *levels = qobject_cast<EchelonsWidget*> (editor);
            QVariant val = index.data(Qt::EditRole);
//            if (levels)
//                levels->setValue(levels->measure() == EchelonsWidget::Meters ? val.toDouble()*AppSettings::CFL_METER_DIVIDER : val.toDouble());
            if (levels && !val.isNull())
            {
                //levels->setValue(val.value<FLVal>().toString());

                FLVal flval(val.value<FLVal>());
                if(levels->measure() == EchelonsWidget::Feet && flval.isFeet())
                    levels->setValue(flval.GetF());
                if(levels->measure() == EchelonsWidget::Meters && !flval.isFeet())
                    levels->setValue(flval.GetM());
            }
            */
        }
            break;
        case CControlDlg::ETDType:
            {

            }
            break;
        }
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void CListACItemDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (m_mapColType.contains(index.column()))
    {
        switch (m_mapColType[index.column()])
        {
        case CControlDlg::CODEType:
        {
            QLineEdit* pEdit = qobject_cast<QLineEdit*> (editor);
            if (pEdit)
            {
                QString sText = pEdit->text();
                if (sText != index.data().toString())
                {
                    model->setData(index, sText, Qt::EditRole);
                }
            }
        }
            break;
        case CControlDlg::CFLType:
        {
            EchelonsWidget *levels = qobject_cast<EchelonsWidget*> (editor);
            if(levels && m_bSignalEmitted)
            {
                FLVal val(levels->value(), levels->measure());
                if(val)
                {
                    QVariant var;
                    var.setValue(val);
                    model->setData(index, var);
                }
            }
        }
            break;
        case CControlDlg::ETDType:
            {
                QTimeEdit* pEdit = qobject_cast<QTimeEdit*> (editor);
                if (pEdit)
                {
                    QTime time = pEdit->time();
                    if(time != QTime(0,0,0,0))
                    {
                        if(time.toString("hh:mm") != index.data().toString())
                            model->setData(index, time, Qt::EditRole);
                    }
                }
            }
            break;
        case CControlDlg::STARType:
            {
                CControlDlg* pWnd = qobject_cast<CControlDlg*>(editor);
                if(pWnd)
                {
                    QStringList sValues = pWnd->GetValues();
                    if(sValues.size())
                    {
                        QString sText = sValues.first();
                        if(sText != index.data().toString())
                            model->setData(index, sText, Qt::EditRole);
                    }
                }
            }
            break;
        }
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void CListACItemDelegate::commitAndCloseEditor()
{
    m_bSignalEmitted = true;
    QWidget* editor = qobject_cast<QWidget*> (sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void CListACItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (QStyleOptionViewItemV4* v4 = qstyleoption_cast<QStyleOptionViewItemV4*>(option))
    {
        bool selected = v4->state & QStyle::State_Selected;
        if (selected)
        {
            v4->state &= ~QStyle::State_Selected;
            //v4->palette.setBrush(QPalette::Text, v4->palette.brush(QPalette::HighlightedText));
            v4->palette.setBrush(QPalette::Text, QBrush(m_HighTextColor));
            if (v4->backgroundBrush.style() == Qt::NoBrush)
            {
                v4->backgroundBrush = QBrush(QColor(210, 210, 210, 255));
            }
            else
            {
                QColor color(v4->backgroundBrush.color());
                color.setAlpha(255);
                v4->backgroundBrush.setColor(color);
            }
        }
    }
}

void CListACItemDelegate::ShowEditCompleter(QWidget* edit) const
{
    QKeyEvent* pEvent1 = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::ControlModifier);
    QKeyEvent* pEvent2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Down, Qt::ControlModifier);
    QApplication::postEvent(edit, pEvent1);
    QApplication::postEvent(edit, pEvent2);
}

QPoint CListACItemDelegate::isRectVisible(QPoint tl, QPoint bl, QRect rect) const
{
    QRect screenRect = qApp->desktop()->availableGeometry();
    if(tl.y() + rect.height() > screenRect.bottom())
    {
        return QPoint(bl.x(), bl.y() - rect.height());
    }
    else
    {
        return tl;
    }
}

void CListACItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
