#include "GUIControlDlg.h"

#include "main.h"
#include "relativedlg.h"
#include "fpldlg.h"
#include "back/core/Airplane.h"
#include "front/core/reminder/GUIAzimuth.h"

#include "EchelonsWidget.h"
#include "FlightLevelWidget.h"
#include "SpeedWidget.h"
#include "VerticalSpeedWidget.h"
#include "FLVal.h"
#include "DailyPlanWidget.h"

#include <QTimer>

#include <QMessageBox>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QTabBar>
#include <QTimeEdit>
#include <QGraphicsGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QToolTip>
#include <QTableView>
#include <QGraphicsSceneWheelEvent>

CGUIControlDlg::CGUIControlDlg(const Type type, QWidget *parent, Qt::WindowFlags wFlags/* = Qt::Popup*/) :
    QWidget(parent, wFlags),
    m_type(type),
    m_butState(false),
    m_pListView(0)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

CGUIControlDlg::~CGUIControlDlg()
{
}

void CGUIControlDlg::Init(const QString &sVal)
{
    m_butState = false;
    QWidget* pWnd = 0;
    switch (m_type)
    {
    case RelativeType:
        {
            //		pWnd = new CGUIAzimuth(0, Qt::Popup);
            //		static_cast<CGUIAzimuth*>(pWnd)->SetReadOnly(false);
            //		static_cast<CGUIAzimuth*>(pWnd)->setAzimuth1(m_sValues1[0].toInt());
            //		static_cast<CGUIAzimuth*>(pWnd)->setAzimuth2(m_sValues1[1].toInt());
            //		connect(pWnd, SIGNAL( changedAzimuth(int) ), this, SLOT( OnAzimuth(int) ));
            pWnd = new CRelativeDlg(0, Qt::Popup);
            static_cast<CRelativeDlg*> (pWnd)->spinBox->setFocus();
            connect(static_cast<CRelativeDlg*> (pWnd)->butExecute, SIGNAL( clicked() ), this, SLOT( SelectExecBut() ));

            QPalette pal(static_cast<CRelativeDlg*> (pWnd)->comboBox->palette());
            pal.setBrush(QPalette::Base, Qt::NoBrush);
            static_cast<CRelativeDlg*> (pWnd)->comboBox->setPalette(pal);

            static_cast<CRelativeDlg*> (pWnd)->spinBox->setFrame(false);
        }
        break;
    case FPLType:
        {
            CAirplane *pPlane = reinterpret_cast<CAirplane*>(m_sValues1.first().toULong());
            //pWnd = new CFPLDlg(pPlane->GetSFPL(), 0, Qt::Popup);
            pWnd = new CFPLDlg(pPlane->GetSFPL(), 0, 0);
            static_cast<CFPLDlg*> (pWnd)->SetAirplane(pPlane);
            //static_cast<CFPLDlg*> (pWnd)->buttonActivate->setEnabled(pPlane && pPlane->IsOur());
            //static_cast<CFPLDlg*> (pWnd)->buttonActivate->setEnabled(false);
            connect(static_cast<CFPLDlg*>(pWnd)->buttonActivate, SIGNAL( clicked() ),
                    this, SLOT( SelectExecBut() ));
            connect(static_cast<CFPLDlg*> (pWnd), SIGNAL( getControlDlg(CGUIControlDlg*&, CGUIControlDlg::Type) ), this,
                    SLOT( OnGetControlDlg(CGUIControlDlg*&, CGUIControlDlg::Type) ));

            QWidget* pTempWnd = new QWidget(0, Qt::Popup);
            QGridLayout* pGrid = new QGridLayout(pTempWnd);
            pGrid->setSpacing(1);
            pGrid->setContentsMargins(0, 0, 0, 0);
            pGrid->addWidget(pWnd);
            pTempWnd->setLayout(pGrid);
            pTempWnd->adjustSize();
            pWnd = pTempWnd;
        }
        break;
    case EchelonsWidgetType:
        {
            QString type = "CFL";
            if (!m_sValues2.isEmpty())
                type = m_sValues2.first();

            FlightLevelWidget::Measure measure = FlightLevelWidget::FEET;
            bool bButtons = false;
            QRegExp rexp("([F,M]{1,1})([F,M]{0,1})");
            if (rexp.exactMatch(networkSettings()->value("airplane/FL_SI", "F").toString()))
            {
                if (!rexp.cap(1).isEmpty())
                {
                    if ("F" == rexp.cap(1))
                        measure = FlightLevelWidget::FEET;
                    else if ("M" == rexp.cap(1))
                        measure = FlightLevelWidget::METERS;

                    if (!rexp.cap(2).isEmpty())
                        bButtons = true;
                }
            }


            QString sValue;
            FLVal flval(sVal);
            if (bButtons) {
                if(flval.isFeet() /*&& !(flval.GetF() == 0)*/)
                {
                    sValue = 'F'+  QString::number(flval.GetF());
                    pWnd = new FlightLevelWidget(m_ACID, type);
                }
                if(!flval.isFeet() /*&& !(flval.GetM() == 0)*/)
                {
                    sValue = 'C' + QString::number(flval.GetM());
                    pWnd = new FlightLevelWidget(m_ACID, type);
                }
            }
            else
            {
                pWnd = new FlightLevelWidget(m_ACID, type);
                sValue = sVal;
            }

            qobject_cast < FlightLevelWidget * >(pWnd)->setTflMarkerVisible(false);
            qobject_cast < FlightLevelWidget * >(pWnd)->setVisibleValue(sValue);
            qobject_cast < FlightLevelWidget * >(pWnd)->setValueColor(sValue, Qt::green, Qt::white);
            connect(pWnd, SIGNAL(accepted(double, double, int)), this, SLOT(onEchelonValueChanged(double, double, int)));
            connect(pWnd, SIGNAL(rejected()), this, SLOT(onRejected()));
        }
        break;
    case speedWidget:
        {
            QString sValnew = "";

            int i = m_sValues1[0].toInt();
            if( i >= 240 && sVal == "spd")
            {
                sValnew = 'M' + sVal;
                pWnd = new SpeedWidget(m_ACID, sValnew);
            }
            else
                pWnd = new SpeedWidget(m_ACID, sVal);
            connect(pWnd, SIGNAL(accepted(const QString &)), this, SLOT(onValueChanged(const QString &)));
            connect(pWnd, SIGNAL(rejected()), this, SLOT(onRejected()));
        }
        break;

    case verticalSpeedWidget:
        {
            pWnd = new VerticalSpeedWidget("ACID", sVal);
            connect(pWnd, SIGNAL(accepted(const QString &)), this, SLOT(onValueChanged(const QString &)));
            connect(pWnd, SIGNAL(rejected()), this, SLOT(onRejected()));
        }
        break;
    //case ListWithEditType:
    case NSDailyPlan:
    {
        QString type = "DailyPlan";
        if (!m_sValues2.isEmpty())
            type = m_sValues2.first();

        pWnd = new DailyPlanWidget(m_ACID, m_code);

        connect(pWnd, SIGNAL(accepted()), this, SLOT(onDailyPlanChoosed()));
        connect(pWnd, SIGNAL(rejected()), this, SLOT(onRejected()));

    }
    break;
    default:
        pWnd = new QWidget(0, Qt::Popup);
    }

    pWnd->setAttribute(Qt::WA_DeleteOnClose);
    pWnd->setFocusPolicy(Qt::StrongFocus);

    if (m_type == TimeType)
    {
        QGridLayout* pGrid = new QGridLayout(pWnd);
        pGrid->setSpacing(1);
        pGrid->setContentsMargins(2, 2, 2, 2);
        m_timeEdit = new QTimeEdit(pWnd);
        m_timeEdit->setDisplayFormat("hh:mm:ss");
        quint32 secs = m_sValues1[0].toUInt();
        m_timeEdit->setTime(QTime(secs / 60 / 60, (secs / 60) % 60, secs % 60));
        connect(m_timeEdit, SIGNAL(editingFinished()), this, SLOT(SelectTimeEdit()));
        pGrid->addWidget(m_timeEdit, 0, 0);
        pWnd->setLayout(pGrid);
        pWnd->adjustSize();
    }
    if (m_type == NumEditType)
    {
        QGridLayout* pGrid = new QGridLayout(pWnd);
        pGrid->setSpacing(1);
        pGrid->setContentsMargins(2, 2, 2, 2);
        m_pEdit = new QLineEdit(pWnd);
        m_pEdit->setValidator(new QIntValidator(1, 15000, pWnd));
        m_pEdit->setText(sVal);
        m_pEdit->setFocus();
        m_pEdit->selectAll();
        pGrid->addWidget(m_pEdit, 0, 0);
        connect(m_pEdit, SIGNAL( editingFinished() ), this, SLOT( SelectEditItem() ));
    }

    if (m_type == NSAssume || m_type == NSROF || m_type == NSTransfer_out || m_type == NSCalculate_sector)
    {
        QGridLayout* pGrid = new QGridLayout(pWnd);
        pGrid->setSpacing(1);
        pGrid->setContentsMargins(2, 2, 2, 2);
        m_pButton = new QPushButton(pWnd);
        if (m_type == NSAssume)
            m_pButton->setText(tr("Assume"));
        else if (m_type == NSROF)
            m_pButton->setText(tr("ROF"));
        else if (m_type == NSTransfer_out)
            m_pButton->setText(tr("OUT"));
        else if (m_type == NSCalculate_sector) {
            QString sText =  m_sValues1[0];
            m_pButton->setText(tr("ns:") + sText);
        }
        pGrid->addWidget(m_pButton, 0, 0);
        if(m_type != NSCalculate_sector)
            connect(m_pButton, SIGNAL( clicked() ), this, SLOT( SelectToggBut() ));
        pWnd->setLayout(pGrid);
        pWnd->adjustSize();
    }

        // if (m_type == ListType || m_type == ListWithEditType || m_type == ListMachType || m_type == ListFootType
        //         || m_type == ListPointType || m_type == ListPointWithHoldType || m_type == NSTransfer)
        if (m_type == ListType || m_type == ListMachType || m_type == ListFootType
            || m_type == ListPointType || m_type == ListPointWithHoldType || m_type == NSTransfer)
    {
        QGridLayout* pGrid = new QGridLayout(pWnd);
        pGrid->setSpacing(1);
        pGrid->setContentsMargins(1, 1, 1, 1);
        if (m_type == ListWithEditType)
        {
            m_pEdit = new QLineEdit(pWnd);
            QRegExp regExp("[0-7]{1,4}");
            m_pEdit->setValidator(new QRegExpValidator(regExp, pWnd));
            m_pEdit->setText(sVal);
            m_pEdit->setFocus();
            m_pEdit->selectAll();
            pGrid->addWidget(m_pEdit, 0, 0);
        }
        if(m_type != ListWithEditType) /////
        {
            m_pListView = new QListView(pWnd);
//            QFont listFont = GetXFont(FONT_CTRL_DLG_LIST);
//            if (listFont != QFont())
//                m_pListView->setFont(listFont);
            m_pListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_sModel.setStringList(m_sValues1);
            int index = m_sValues1.indexOf(sVal);
            QModelIndex curIndex = m_sModel.index(index, 0);
            m_pListView->setModel(&m_sModel);
            m_pListView->setCurrentIndex(curIndex);

            QPalette pal(m_pListView->palette());
            pal.setBrush(QPalette::Base, Qt::NoBrush);
            m_pListView->setPalette(pal);
        } /////

        switch (m_type)
        {
        case ListPointWithHoldType:
            m_pTabBar = new QTabBar(pWnd);
            m_pTabBar->addTab(tr("Point"));
            m_pTabBar->addTab(tr("Holding"));
            m_pTabBar->setShape(QTabBar::TriangularSouth);
            connect(m_pTabBar, SIGNAL( currentChanged(int) ), this, SLOT( SelectTab(int) ));
            pGrid->addWidget(m_pTabBar, 1, 0);
            pGrid->addWidget(m_pListView, 0, 0);
            break;
        case ListWithEditType:
            /////pGrid->addWidget(m_pListView, 1, 0);
            break;
        default:
            pGrid->addWidget(m_pListView, 0, 0);
            break;
        }

        if (m_type != ListWithEditType)
            m_pListView->setFocus();

        if (m_type == ListMachType)
        {
            m_pButton = new QPushButton(pWnd);
            m_pButton->setText(tr("MACH"));
            pGrid->addWidget(m_pButton, 1, 0);
            connect(m_pButton, SIGNAL( clicked() ), this, SLOT( SelectToggBut() ));
        }
        if (m_type == ListFootType)
        {
            m_pButton = new QPushButton(pWnd);
            m_pButton->setText(tr("METERS"));
            pGrid->addWidget(m_pButton, 1, 0);
            connect(m_pButton, SIGNAL( clicked() ), this, SLOT( SelectToggBut() ));
        }
        if (m_type == ListPointType || m_type == ListPointWithHoldType)
        {
            /* temp disable
            m_pButton = new QPushButton(pWnd);
            m_pButton->setText(tr("direct to"));
            pGrid->addWidget(m_pButton, 2, 0);
            connect(m_pButton, SIGNAL( clicked() ), this, SLOT( SelectToggBut() ));
            */
        }
        if (m_type == NSAssume || m_type == NSROF || m_type == NSTransfer || m_type == NSTransfer_out)
        {
            m_pButton = new QPushButton(pWnd);
            if (m_type == NSAssume)
                m_pButton->setText(tr("Assume"));
            else if (m_type == NSROF)
                m_pButton->setText(tr("ROF"));
            else if (m_type == NSTransfer || m_type == NSTransfer_out)
                m_pButton->setText(tr("OUT"));
            pGrid->addWidget(m_pButton, 1, 0);
            connect(m_pButton, SIGNAL( clicked() ), this, SLOT( SelectToggBut() ));
            if(m_type == NSTransfer)
            {
                m_pButton->setDisabled(networkSettings()->value("oldi/disable_COF", "false").toString() == "true");
            }
        }

        pWnd->setLayout(pGrid);
        pWnd->adjustSize();

        if (m_type == ListWithEditType)
        {
            /////pWnd->setTabOrder(m_pEdit, m_pListView);
            connect(m_pEdit, SIGNAL( editingFinished() ), this, SLOT( SelectEditItem() ));
        }
        else /////
        {
            connect(m_pListView, SIGNAL( activated(const QModelIndex&) ), this, SLOT( SelectNewItem(const QModelIndex&) ));
            /*Pasha*/connect(m_pListView, SIGNAL( clicked(const QModelIndex&) ),
            /*OneClickative*/this, SLOT( SelectNewItem(const QModelIndex&) ));
        } /////
    }
    if(m_type == Spd1Type)
    {
        QVBoxLayout* pGrid = new QVBoxLayout(pWnd);
        pGrid->setSpacing(1);
        pGrid->setContentsMargins(1, 1, 1, 1);

        m_pEdit = new QLineEdit(pWnd);

        int max = m_sValues1.first().toInt();
        int min = m_sValues1.last().toInt();
        m_pEdit->setValidator(new QIntValidator(min, max, m_pEdit));
        m_pEdit->setText(sVal);
        m_pEdit->setFocus();
        m_pEdit->selectAll();
        pGrid->addWidget(m_pEdit, 0, 0);

        m_pListView = new QListView(pWnd);
//        QFont listFont = GetXFont(FONT_CTRL_DLG_LIST);
//        if (listFont != QFont())
//            m_pListView->setFont(listFont);
        m_pListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_sModel.setStringList(m_sValues1);
        int index = m_sValues1.indexOf(sVal);
        QModelIndex curIndex = m_sModel.index(index, 0);
        m_pListView->setModel(&m_sModel);
        m_pListView->setCurrentIndex(curIndex);

        QPalette pal(m_pListView->palette());
        pal.setBrush(QPalette::Base, Qt::NoBrush);
        m_pListView->setPalette(pal);
        pGrid->addWidget(m_pListView, 0, 0);

        m_pEdit->setFocus();

        pWnd->setLayout(pGrid);
        pWnd->adjustSize();

        connect(m_pEdit, SIGNAL( editingFinished() ), this, SLOT( SelectEditItem() ));
        connect(m_pListView, SIGNAL( activated(const QModelIndex&) ), this, SLOT( SelectNewItem(const QModelIndex&) ));
        /*Pasha*/connect(m_pListView, SIGNAL( clicked(const QModelIndex&) ),
        /*OneClickative*/this, SLOT( SelectNewItem(const QModelIndex&) ));
    }

    QPalette pal(pWnd->palette());
    pal.setBrush(QPalette::Background, QBrush(Qt::lightGray));
    pWnd->setPalette(pal);

    QRectF rect(0, 0, 10, 40);

    switch(m_type)
    {
    case RelativeType:
    case TimeType:
    case NumEditType:
    case NSAssume:
    case NSTransfer_out:
    case NSROF:
    case ListWithEditType: /////
        pWnd->resize(rect.width(), 1);
       // pWnd->resize(rect.width(), 10);
        break;
        //    case ListWithEditType:
        //        {
        //            int height = m_sModel.rowCount()
        //                         * m_pListView->visualRect(m_sModel.index(0)).height();
        //            height += m_pEdit->rect().height();
        //            height += 7;
        //            pWnd->resize(rect.width(), height);
        //        }
        //        break;
        //    case ListPointType:
        //        pWnd->resize(rect.width(), 147);
        //        break;
    case EchelonsWidgetType:
        //pWnd->resize(110, 180);
        pWnd->resize(110, 360);
        break;
//    case ListType:
//    {
//        pWnd->resize(pWnd->width() / 2, 50);
//        break;
//    }
    case ListType:
    case ListPointType:
        {
            int height = 50;
            int width = rect.width();
            if(m_sModel.rowCount())
            {
                height = (m_sModel.rowCount() > 5 ? 5 : m_sModel.rowCount())
                            * m_pListView->visualRect(m_sModel.index(0)).height();
                height += 7;
                width = m_pListView->visualRect(m_sModel.index(0)).width();
                width += 7;
            }
            if(m_type == ListType)
                pWnd->resize(qBound(20, qRound(rect.width()), 120), qBound(20, height, 120));
            else
                pWnd->resize(qBound(20, qRound(width/2.0), 120), qBound(20, height, 120));
        }
        break;
    case NSDailyPlan:
        //pWnd->resize(110, 180);
        //pWnd->resize(110, 360);
        pWnd->resize(220, 260);
        break;
    default: pWnd->resize(rect.width(), 50);
    }

    setWidget(pWnd);
}

void CGUIControlDlg::SelectNewItem(const QModelIndex& item)
{
    if (m_type == ListMachType)
    {
        QStringList sValues;
        if (m_butState)
            sValues << "MACH";
        else
            sValues << "CAS";
        QString sValue = item.data().toString();
        sValues << sValue;
        emit itemChanged(sValues);
        close();
        return;
    }
    if (m_type == ListPointType || m_type == ListPointWithHoldType)
    {
        QStringList sValues;
        if (m_type == ListPointWithHoldType)
        {
            if (m_pTabBar->currentIndex())
                sValues << "Zone";
            else
                sValues << "Point";
        }
        else
            sValues << "Point";

        sValues << QString::number(item.row());
        sValues << item.data().toString();
        emit itemChanged(sValues);
        close();
        return;
    }
    if (m_type == NSTransfer)
    {
        QStringList sValues;
        sValues << QString::number(item.row());
        sValues << item.data().toString();
        emit itemChanged(sValues);
        close();
        return;
    }

    QString sValue = item.data().toString();
    emit itemChanged(QStringList(sValue));
    close();
}

void CGUIControlDlg::SelectEditItem()
{
    QString sValue = m_pEdit->text();
    if (sValue.toInt() == 0)
        return;
    emit itemChanged(QStringList(sValue));
    close();
}

void CGUIControlDlg::OnAzimuth(int degrees)
{
    QStringList sValues;
    sValues << "";
    sValues << QString::number(degrees);
    emit itemChanged(sValues);
    close();
}

void CGUIControlDlg::SelectExecBut()
{
    if(m_type == FPLType)
    {
        QStringList sValues;
        sValues << "Activate";
        emit itemChanged(sValues);
        close();
    }
    if(m_type == RelativeType)
    {
        QStringList sValues;
        CRelativeDlg* pWnd = static_cast<CRelativeDlg*>(m_widget);
        if(pWnd->checkBox->checkState() == Qt::Checked)
        {
            if(pWnd->comboBox->currentIndex())
                sValues << "RIGHT";
            else
                sValues << "LEFT";
        }
        else
        {
            sValues << "";
        }
        sValues << QString::number(pWnd->spinBox->value());
        emit itemChanged(sValues);
        close();
    }
}

void CGUIControlDlg::SelectToggBut()
{
    m_butState = !m_butState;
    if (m_type == ListMachType)
    {
        if (m_butState)
        {
            m_pButton->setText(tr("CAS"));
            m_sModel.setStringList(m_sValues2);
            m_pListView->setModel(&m_sModel);
        }
        else
        {
            m_pButton->setText(tr("MACH"));
            m_sModel.setStringList(m_sValues1);
            m_pListView->setModel(&m_sModel);
        }
    }
    if (m_type == ListFootType)
    {
        if (m_butState)
        {
            m_pButton->setText(tr("FEET"));
            m_sModel.setStringList(m_sValues2);
            m_pListView->setModel(&m_sModel);
        }
        else
        {
            m_pButton->setText(tr("METERS"));
            m_sModel.setStringList(m_sValues1);
            m_pListView->setModel(&m_sModel);
        }
    }
    if (m_type == ListPointType || m_type == ListPointWithHoldType)
    {
        emit itemChanged(QStringList(""));
        close();
    }
    if (m_type == NSAssume || m_type == NSROF)
    {
        emit itemChanged(QStringList("ASSUME"));
        close();
    }
    if (m_type == NSTransfer || m_type == NSTransfer_out)
    {
//        QModelIndex index = m_pListView->currentIndex();
//        if(index.isValid())
//        {
//            QStringList sValues;
//            sValues << QString::number(index.row());
//            sValues << index.data().toString();
//            emit itemChanged(sValues);
//            close();
//        }
        emit itemChanged(QStringList("OUT"));
        close();
    }
}

void CGUIControlDlg::OnGetControlDlg(CGUIControlDlg*& pNewDlg, CGUIControlDlg::Type type)
{
    pNewDlg = new CGUIControlDlg(type);
}

void CGUIControlDlg::onValueChanged(const QString &new_value)
{
    QStringList values;
    values << new_value;
    emit itemChanged(values);
    close();
}

void CGUIControlDlg::onRejected()
{
    emit rejected();
    close();
}

void CGUIControlDlg::onEchelonValueChanged(double val_feet, double val_meter, int measure)
{
    QStringList values;
    values << QString::number(val_feet);
    values << QString::number(val_meter);
    values << QString::number(measure);
    emit itemChanged(values);
    close();
}

void CGUIControlDlg::onDailyPlanChoosed()
{
    QStringList sValues;
    sValues << "ASSUME";
    emit itemChanged(sValues);
    close();
}

void CGUIControlDlg::onEchelonValueLandChanged(QString sValue)
{
    QStringList sValues;
    sValues << QString::number(0);
    sValues << QString::number(0);
    sValues << QString::number(EchelonsWidget::Feet);
    emit itemChanged(sValues);
    close();
}

void CGUIControlDlg::SelectTab(int index)
{
    if (index == 0)
    {
        m_sModel.setStringList(m_sValues1);
        m_pListView->setModel(&m_sModel);
    }
    if (index == 1)
    {
        m_sModel.setStringList(m_sValues2);
        m_pListView->setModel(&m_sModel);
    }
}

void CGUIControlDlg::SelectTimeEdit()
{
    QTime time = m_timeEdit->time();
    quint32 secs = (time.hour() * 60 * 60) + (time.minute() * 60) + time.second();
    emit itemChanged(QStringList(QString::number(secs)));
    close();
}

void CGUIControlDlg::UpdateXFont()
{

}

void CGUIControlDlg::showEvent(QShowEvent *event)
{
    event->accept();
    QTimer::singleShot(0, this, SLOT(OnSingleUpdate()));
}

void CGUIControlDlg::hideEvent(QHideEvent *event)
{
    event->accept();
    QTimer::singleShot(0, this, SLOT(close()));
}

void CGUIControlDlg::OnSingleUpdate()
{
    update();
}

void CGUIControlDlg::Show(const QPoint &pos, const QRect &rect)
{
    move(pos);
    show();

    if (!rect.isNull())
    {
        QPoint diff;
        diff.setX(qMax(pos.x() + width() - rect.right(), 0));
        diff.setY(qMax(pos.y() + height() - rect.bottom(), 0));

        move(pos - diff);
    }
}

void CGUIControlDlg::setWidget(QWidget *widget)
{
    m_widget = widget;

    resize(m_widget->minimumSizeHint());

    QLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(m_widget);
    setLayout(layout);
}
