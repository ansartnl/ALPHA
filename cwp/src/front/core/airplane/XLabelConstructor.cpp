#include "XLabelConstructor.h"
#include <QGraphicsLineItem>
#include <QPushButton>
#include <QWheelEvent>
#include <math.h>
#include <qmath.h>
#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include "front/core/airplane/GUIAirplaneLabel.h"
#include "front/core/GUIAirplane.h"

CXLabelConstructor::CXLabelConstructor(QWidget *parent) :
	QDialog(parent), m_dScale(0), m_iDimension(0)
    , m_iColX(0), m_iColY(0), m_bPosChange(false), m_bModify(false), m_bPull(false)
{
	setupUi(this);

    comboBoxType->addItem(QString::null, XGridLabel::Controlled);
    comboBoxType->addItem(QString::null, XGridLabel::ControlledShort);
    comboBoxType->addItem(QString::null, XGridLabel::ControlledSelected);
    comboBoxType->addItem(QString::null, XGridLabel::In);
    comboBoxType->addItem(QString::null, XGridLabel::InShort);
    comboBoxType->addItem(QString::null, XGridLabel::InSelected);
    comboBoxType->addItem(QString::null, XGridLabel::Related);
    comboBoxType->addItem(QString::null, XGridLabel::RelatedShort);
    comboBoxType->addItem(QString::null, XGridLabel::RelatedSelected);
    comboBoxType->addItem(QString::null, XGridLabel::Unrelated);
    comboBoxType->addItem(QString::null, XGridLabel::UnrelatedShort);
    comboBoxType->addItem(QString::null, XGridLabel::UnrelatedSelected);

	comboBoxDimension->addItem("3x3", 3);
	comboBoxDimension->addItem("4x4", 4);
    comboBoxDimension->addItem("5x5", 5);
    comboBoxDimension->addItem("6x6", 6);

    graphicsView->setMinimumSize(QSize(get_VIEW_SIZE_X()+2, get_VIEW_SIZE_Y()+2));

    QGraphicsScene* pscene = new QGraphicsScene(0, 0, get_VIEW_SIZE_X(), get_VIEW_SIZE_Y());
	graphicsView->setScene(pscene);
	//graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	graphicsView->setRenderHints(QPainter::TextAntialiasing);
    graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	/*
	QLinearGradient backGradient(0, 0, 2000, 1000);
	backGradient.setColorAt(0.0, QColor(200, 250, 200));
	backGradient.setColorAt(1.0, QColor(100, 150, 100));
	backGradient.setSpread(QGradient::PadSpread);
	graphicsView->setBackgroundBrush(QBrush(backGradient));
	*/
	graphicsView->setBackgroundBrush(QBrush(QColor(0x60, 0x60, 0x60)));

    graphicsView->viewport()->installEventFilter(this);
    graphicsView->viewport()->setMouseTracking(true);

	m_bPosChange = true;

    //////////////////////////////////////////////////////////////////////////
	// column 1
	QPointF defaultPoint(5, 5);
	int maxColumnWidth(0);
	QSharedPointer<CXLabelItem> pItem(new CXLabelItem(this, XGridItem::ACID));
	pscene->addItem(pItem.data());
	///////pItem->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::ACID, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::CODE));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::CODE, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::LX));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::LX, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::NS));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::NS, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::AFL));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::AFL, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::CFL));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::CFL, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::TREND));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::TREND, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::VRC));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::VRC, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::HDG));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::HDG, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::BANK));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::BANK, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::NextPoint));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::NextPoint, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::SPD1));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::SPD1, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::SPD2));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::SPD2, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::V));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::V, pItem);

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::GOV));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	if(pItem->boundingRect().width() > maxColumnWidth) maxColumnWidth = pItem->boundingRect().width();
	m_listLabelItems.insert(XGridItem::GOV, pItem);
	/////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// column 2
	defaultPoint.rx() = maxColumnWidth + 5 + 5;
	defaultPoint.ry() = 5;

	pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::FUEL));
	pscene->addItem(pItem.data());
	pItem->SetDefaultPoint(defaultPoint);
	defaultPoint.ry() += pItem->boundingRect().height() + 5;
	m_listLabelItems.insert(XGridItem::FUEL, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::TFL));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::TFL, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::HDG2));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::HDG2, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::UVD));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::UVD, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::PEL));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::PEL, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::RFL));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::RFL, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::AZIMUTH));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::AZIMUTH, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::DISTANCE));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::DISTANCE, pItem);

    pItem = QSharedPointer<CXLabelItem>(new CXLabelItem(this, XGridItem::QNH));
    pscene->addItem(pItem.data());
    pItem->SetDefaultPoint(defaultPoint);
    defaultPoint.ry() += pItem->boundingRect().height() + 5;
    m_listLabelItems.insert(XGridItem::QNH, pItem);
    /////////////////////////////////////////////////////////////////////////

	m_bPosChange = false;

    QGraphicsLineItem* pLine = new QGraphicsLineItem(get_SEPARATOR_POS(), 1.5, get_SEPARATOR_POS(), get_VIEW_SIZE_Y()-1.5);
	QPen pen(QBrush(Qt::black), 3);
	pen.setCosmetic(true);
	pLine->setPen(pen);
	pscene->addItem(pLine);

	QGraphicsRectItem* psceneRect = new QGraphicsRectItem(pscene->sceneRect());
	pscene->addItem(psceneRect);

	Translate();

	OnComboDimension(comboBoxDimension->currentIndex());

	connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(OnRuleButtons(QAbstractButton*)));
	connect(comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboType(int)));
	connect(comboBoxDimension, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboDimension(int)));
	connect(butForeground, SIGNAL(clicked()), this, SLOT(OnButForeground()));
	connect(butBackground, SIGNAL(clicked()), this, SLOT(OnButBackground()));
	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(OnSpinTransparent(int)));

	XGridLabel::Load(m_mapGrig);
	OnComboType(comboBoxType->currentIndex());

    checkBoxFillEmpty->setCheckState(Qt::Checked);
	SetModify(false);

    resize(minimumWidth(), minimumHeight());
}

CXLabelConstructor::~CXLabelConstructor()
{

}

void CXLabelConstructor::Translate()
{
	retranslateUi(this);

    QPushButton* pPreviewButton = buttonBox->button(QDialogButtonBox::Apply);
    if(pPreviewButton) pPreviewButton->setText(tr("Preview"));
    pPreviewButton = buttonBox->button(QDialogButtonBox::SaveAll);
    if(pPreviewButton) pPreviewButton->setText(tr("Set default"));

	for(int i = 0; i < comboBoxType->count(); ++i)
	{
		QString sText(comboBoxType->itemText(i));
		switch(comboBoxType->itemData(i).toInt())
		{
        case XGridLabel::Controlled:
            sText = tr("Controlled");
            break;
        case XGridLabel::ControlledShort:
            sText = tr("Controlled (Short)");
            break;
        case XGridLabel::ControlledSelected:
            sText = tr("Controlled Selected");
            break;
        case XGridLabel::In:
            sText = tr("In");
            break;
        case XGridLabel::InShort:
            sText = tr("In (Short)");
            break;
        case XGridLabel::InSelected:
            sText = tr("In Selected");
            break;
        case XGridLabel::Related:
            sText = tr("Concerned");
            break;
        case XGridLabel::RelatedShort:
            sText = tr("Concerned (Short)");
            break;
        case XGridLabel::RelatedSelected:
            sText = tr("Concerned Selected");
            break;
        case XGridLabel::Unrelated:
            sText = tr("Non concerned");
            break;
        case XGridLabel::UnrelatedShort:
            sText = tr("Non concerned (Short)");
            break;
        case XGridLabel::UnrelatedSelected:
            sText = tr("Non concerned Selected");
            break;
        }
		comboBoxType->setItemText(i, sText);
	}

	TLabelItems::const_iterator itItem = m_listLabelItems.begin();
	for(; itItem != m_listLabelItems.end(); ++itItem)
		(*itItem)->Translate();
}

void CXLabelConstructor::closeEvent(QCloseEvent *pEvent)
{
	if(m_bModify)
	{
		switch(QMessageBox::question(
				this,
				tr("Unsaved data"),
				tr("Save modified data?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
		case QMessageBox::Yes:
			{
				Save();
				pEvent->accept();
				break;
			}
		case QMessageBox::Cancel:
			{
				pEvent->ignore();
				return;
			}
		default:
			{
				pEvent->accept();
				break;
			}
		}
	}
	QDialog::closeEvent(pEvent);
}

void CXLabelConstructor::wheelEvent(QWheelEvent *pEvent)
{
	QDialog::wheelEvent(pEvent);

	if ((pEvent->modifiers() & Qt::ControlModifier) && !pEvent->buttons())
	{
		double numSteps = pEvent->delta() / (8.0 * 15.0);
		if ((m_dScale < MAX_SCALE && numSteps > 0) ||
			(m_dScale > 0 && numSteps < 0))
		{
			m_dScale += numSteps;

			if (m_dScale > MAX_SCALE)
				m_dScale = MAX_SCALE;
			else if (m_dScale + numSteps < 0)
				m_dScale = 0;
			double dScale = pow(1.125, m_dScale);
			graphicsView->setTransform(QTransform());
			graphicsView->scale(dScale, dScale);
		}
	}
}

void CXLabelConstructor::resizeEvent(QResizeEvent *pEvent)
{
	QDialog::resizeEvent(pEvent);

	double scaleX = graphicsView->maximumViewportSize().width();
	double scaleY = graphicsView->maximumViewportSize().height();
    double dScale = qMin(scaleX/get_VIEW_SIZE_X(), scaleY/get_VIEW_SIZE_Y());
    m_dScale = pow(1/1.125, dScale);
    graphicsView->setTransform(QTransform());
    graphicsView->scale(scaleX/get_VIEW_SIZE_X(), scaleY/get_VIEW_SIZE_Y());
}

void CXLabelConstructor::OnRuleButtons(QAbstractButton *pButton)
{
	if(pButton == buttonBox->button(QDialogButtonBox::Apply))
	{
		QDialog dlg(this);
		dlg.setWindowTitle(tr("Preview"));
		QGraphicsView view(&dlg);
		view.setMinimumSize(QSize(300+2, 300+2));
		view.setBackgroundBrush(QBrush(QColor(0x60, 0x60, 0x60)));
		QGraphicsScene scene(0, 0, 300, 300);
		view.setScene(&scene);

		CGUIAirplaneLabel label;
		XGridLabel labelGridType = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())];
        CalcSpan(labelGridType);
		labelGridType.SetType(XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt()));
		label.SetGridLabel(labelGridType);
		label.SetConstructorFlag(true);
		scene.addItem(&label);
		CGUIAirplane plane;
        plane.SetPlaneType(CAirplaneData::None);
		plane.SetMode3AReply(1);
		plane.SetFL(350);
		plane.SetVz(10);
		label.SetAirplane(&plane);
		label.setPos(20,20);
		label.UpdatePosition(QPointF(20,20));
		dlg.adjustSize();
		dlg.setFixedSize(dlg.size());
		dlg.exec();
	}
	if(pButton == buttonBox->button(QDialogButtonBox::Save))
	{
		Save();
	}
    if(pButton == buttonBox->button(QDialogButtonBox::Close))
    {
        close();
    }
    if(pButton == buttonBox->button(QDialogButtonBox::SaveAll))
    {
        SetDefaultLabel();
    }
}

void CXLabelConstructor::OnComboType(int index)
{
	if((index != -1) && (index < comboBoxType->count()))
	{
		const int iMAX_DIMENSION(comboBoxDimension->itemData(comboBoxDimension->count()-1).toInt());
		int iDimension(0);

		const XGridLabel::TMapItem& mapItem = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(index).toInt())].GetMapItem();
		XGridLabel::TMapItem::const_iterator itItem = mapItem.begin();
		for(; itItem != mapItem.end(); ++itItem)
		{
			if(itItem->GetRow() > iDimension) iDimension = itItem->GetRow();
			if(itItem->GetColumn() > iDimension) iDimension = itItem->GetColumn();
		}
		if(iDimension > iMAX_DIMENSION) iDimension = iMAX_DIMENSION;
        if(iDimension > m_iDimension)
		{
			for(int i = 0; i < comboBoxDimension->count(); ++i)
			{
				if(iDimension <= comboBoxDimension->itemData(i).toInt())
				{
					comboBoxDimension->setCurrentIndex(i);
					break;
				}
			}
		}
		SetItems(m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(index).toInt())]);
	}
	else
		SetItems(XGridLabel());
}

void CXLabelConstructor::OnComboDimension(int index)
{
	if((index != -1) && (index < comboBoxDimension->count()))
	{
		SetDimension(comboBoxDimension->itemData(index).toInt());

		XGridLabel::TMapItem& mapItems = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem();
		XGridLabel::TMapItem::iterator itItem = mapItems.begin();
		while(itItem != mapItems.end())
		{
			if((itItem->GetRow() > m_iDimension) || (itItem->GetColumn() > m_iDimension))
			{
				itItem = mapItems.erase(itItem);
				SetModify(true);
			}
			else
				++itItem;
		}
		SetItems(m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())]);
	}
	else
	{
		SetDimension(0);
		SetItems(XGridLabel());
	}
}

void CXLabelConstructor::SetDimension(int iDimension)
{
	TGridItems::const_iterator itItem = m_listGridItems.begin();
	for(; itItem != m_listGridItems.end(); ++itItem)
		graphicsView->scene()->removeItem((*itItem).data());
	m_listGridItems.clear();

	m_iColX = m_iColY = 0;
	m_iDimension = iDimension;
	if(m_iDimension == 0) return;

    m_iColX = ((get_VIEW_SIZE_X()-get_SEPARATOR_POS()) - ((get_VIEW_SIZE_X()-get_SEPARATOR_POS())%(m_iDimension+2))) / (m_iDimension+2);
    m_iColY = (get_VIEW_SIZE_Y() - get_VIEW_SIZE_Y()%(m_iDimension+2)) / (m_iDimension+2);

	QSharedPointer<QGraphicsLineItem> pLine;
	QSharedPointer<QGraphicsSimpleTextItem> pText;
	for(int i = 1; i < m_iDimension+2; ++i)
	{
        pLine = QSharedPointer<QGraphicsLineItem>(new QGraphicsLineItem(i*m_iColX + get_SEPARATOR_POS(), m_iColY/2, i*m_iColX + get_SEPARATOR_POS(), m_iColY*(m_iDimension+1)));
		QPen pen(QBrush(Qt::black), 2);
		pen.setCosmetic(true);
		pLine->setPen(pen);
		graphicsView->scene()->addItem(pLine.data());
		m_listGridItems.append(pLine);

        pLine = QSharedPointer<QGraphicsLineItem>(new QGraphicsLineItem(m_iColX/2 + get_SEPARATOR_POS(), i*m_iColY, m_iColX*(m_iDimension+1)+get_SEPARATOR_POS(), i*m_iColY));
		pLine->setPen(pen);
		graphicsView->scene()->addItem(pLine.data());
		m_listGridItems.append(pLine);

		if(i < m_iDimension+1)
		{
			pText = QSharedPointer<QGraphicsSimpleTextItem>(new QGraphicsSimpleTextItem(QString::number(i)));
			graphicsView->scene()->addItem(pText.data());
			QRectF textRect = pText->boundingRect();
            pText->setPos(i*m_iColX+get_SEPARATOR_POS() + (m_iColX/2 - textRect.width()/2), m_iColY-textRect.height()-10);
			m_listGridItems.append(pText);

			pText = QSharedPointer<QGraphicsSimpleTextItem>(new QGraphicsSimpleTextItem(QString::number(i)));
			graphicsView->scene()->addItem(pText.data());
            pText->setPos(m_iColX+get_SEPARATOR_POS()-textRect.width()-10, i*m_iColY + (m_iColY/2 - textRect.height()/2));
			m_listGridItems.append(pText);
		}
	}
}

void CXLabelConstructor::SetItems(const XGridLabel& GridLabel)
{
	{
		TLabelItems::const_iterator itItem = m_listLabelItems.begin();
		for(; itItem != m_listLabelItems.end(); ++itItem)
		{
			(*itItem)->setPos((*itItem)->GetDefaultPoint());
			(*itItem)->SetForeground(GridLabel.GetForeground());
			(*itItem)->SetBackground(GridLabel.GetBackground());
			(*itItem)->SetTransparent(GridLabel.GetTransparent());
		}
	}

	XGridLabel::TMapItem::const_iterator itItem = GridLabel.GetMapItem().begin();
	for(; itItem != GridLabel.GetMapItem().end(); ++itItem)
	{
		TLabelItems::const_iterator itFind = m_listLabelItems.find(itItem->GetType());
		if(itFind != m_listLabelItems.end())
		{
			// Set item position in cell
			SetItemPos((*itFind).data(), *itItem);
		}
	}
	m_bPosChange = true;
	spinBox->setValue(GridLabel.GetTransparent());
	m_bPosChange = false;
}

void CXLabelConstructor::SetPos(CXLabelItem* pItem, QPointF scenePos)
{
	if(m_bPosChange || !pItem) return;

	const qreal indent(10);
	bool bOk(false);
	if(m_iDimension)
	{
        QPair<int,int> iCell((scenePos.x()-get_SEPARATOR_POS())/m_iColX, scenePos.y()/m_iColY);
		if((iCell.first > 0) && (iCell.first <= m_iDimension)
			&& (iCell.second > 0) && (iCell.second <= m_iDimension))
		{
			bOk = true;
			XGridLabel::TMapItem::const_iterator itItem = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem().begin();
			XGridLabel::TMapItem::const_iterator itEnd = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem().end();
			for(; itItem != itEnd; ++itItem)
			{
				if((itItem->GetColumn() == iCell.first) && (itItem->GetRow() == iCell.second))
				{
					if((itItem->GetType() != XGridItem::None) && (itItem->GetType() != pItem->GetType()))
					{
						bOk = false;
					}
					break;
				}
			}
			if(bOk)
			{
				XGridItem labelItem;
				labelItem.SetType(pItem->GetType());
				labelItem.SetColumn(iCell.first);
				labelItem.SetRow(iCell.second);
				// Calculate alignment
				Qt::Alignment itemAlignment;
                if(scenePos.x() - m_iColX*iCell.first-get_SEPARATOR_POS() < indent)
					itemAlignment = Qt::AlignLeft;
				else
                    if(m_iColX*(iCell.first+1)+get_SEPARATOR_POS() - scenePos.x() < indent)
						itemAlignment = Qt::AlignRight;
					else
						itemAlignment = Qt::AlignHCenter;

				if(scenePos.y() - m_iColY*iCell.second < indent)
					itemAlignment |= Qt::AlignTop;
				else
					if(m_iColY*(iCell.second+1) - scenePos.y() < indent)
						itemAlignment |= Qt::AlignBottom;
					else
						itemAlignment |= Qt::AlignVCenter;

				labelItem.SetAlignment(itemAlignment);

				SetItemPos(pItem, labelItem);
				m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem().insert(pItem->GetType(), labelItem);
				SetModify(true);
			}
		}
	}

	if(!bOk)
	{
		if(m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem().remove(pItem->GetType()))
			SetModify(true);
		pItem->setPos(pItem->GetDefaultPoint());
	}
}

void CXLabelConstructor::SetItemPos(CXLabelItem* pItem, const XGridItem& pLabelItem)
{
	const qreal indent(2);
	QRectF itemRect(pItem->boundingRect());
	QPointF itemPos(pItem->GetDefaultPoint());

    if(pLabelItem.GetAlignment() & Qt::AlignHCenter) itemPos.setX(m_iColX*pLabelItem.GetColumn()+get_SEPARATOR_POS()+m_iColX/2.0 - itemRect.width()/2.0);
    if(pLabelItem.GetAlignment() & Qt::AlignLeft) itemPos.setX(m_iColX*pLabelItem.GetColumn()+get_SEPARATOR_POS()+indent);
    if(pLabelItem.GetAlignment() & Qt::AlignRight) itemPos.setX(m_iColX*(pLabelItem.GetColumn()+1)+get_SEPARATOR_POS()-itemRect.width()-indent);

	if(pLabelItem.GetAlignment() & Qt::AlignVCenter) itemPos.setY(m_iColY*pLabelItem.GetRow()+m_iColY/2.0-itemRect.height()/2.0);
	if(pLabelItem.GetAlignment() & Qt::AlignTop) itemPos.setY(m_iColY*pLabelItem.GetRow()+indent);
	if(pLabelItem.GetAlignment() & Qt::AlignBottom) itemPos.setY(m_iColY*(pLabelItem.GetRow()+1)-itemRect.height()-indent);

	pItem->setPos(itemPos);
}

void CXLabelConstructor::CalcRowSpan(const int row, const int column, const int dimension, const XGridLabel::TMapItem& map, qint32& span)
{
	if(row <= dimension)
	{
		bool bCan(true);
		XGridLabel::TMapItem::const_iterator itItem = map.begin();
		for(; itItem != map.end(); ++itItem)
		{
			if((itItem->GetRow() == row) && (itItem->GetColumn() == column))
			{
				bCan = false;
				break;
			}
		}
		if(bCan)
		{
			++span;
            CalcRowSpan(row+1, column, dimension, map, span);
        }
	}
}

void CXLabelConstructor::CalcColumnSpan(const int row, const int column, const int dimension, const XGridLabel::TMapItem& map, qint32& span)
{
	if(column <= dimension)
	{
        bool bEmpty(true);
		bool bCan(true);
		XGridLabel::TMapItem::const_iterator itItem = map.begin();
		for(; itItem != map.end(); ++itItem)
		{
            if(itItem->GetColumn() == column)
            {
                bEmpty = false;
                if(itItem->GetRow() == row)
                {
                    bCan = false;
                    break;
                }
            }
		}
		if(bCan)
		{
            if(!bEmpty) ++span;
			CalcColumnSpan(row, column+1, dimension, map, span);
		}
	}
}

void CXLabelConstructor::CalcSpan(XGridLabel& label)
{
    int iMAXDimRow(0);
    int iMAXDimCol(0);
    XGridLabel::TMapItem::iterator itItem = label.GetMapItem().begin();
    for(; itItem != label.GetMapItem().end(); ++itItem)
    {
        if(itItem->GetRow() > iMAXDimRow) iMAXDimRow = itItem->GetRow();
        if(itItem->GetColumn() > iMAXDimCol) iMAXDimCol = itItem->GetColumn();
    }
    itItem = label.GetMapItem().begin();
    bool bFillEmpty = checkBoxFillEmpty->checkState() == Qt::Checked;
    for(; itItem != label.GetMapItem().end(); ++itItem)
    {
        qint32 span(1);
        if(bFillEmpty)
            CalcRowSpan(itItem->GetRow()+1, itItem->GetColumn(), iMAXDimRow, label.GetMapItem(), span);
        itItem->SetRowSpan(span);
        itItem->SetRowSpan(1);//!!!
        span = 1;
        if(bFillEmpty)
            CalcColumnSpan(itItem->GetRow(), itItem->GetColumn()+1, iMAXDimCol, label.GetMapItem(), span);
        itItem->SetColumnSpan(span);
    }
}

void CXLabelConstructor::SetModify(bool bModify)
{
	m_bModify = bModify;
	QPushButton* pPreviewButton = buttonBox->button(QDialogButtonBox::Save);
	if(pPreviewButton) pPreviewButton->setEnabled(m_bModify);
}

void CXLabelConstructor::Save()
{
	XGridLabel::TMapGrid::iterator itType = m_mapGrig.begin();
	for(; itType != m_mapGrig.end(); ++itType)
	{
        CalcSpan(*itType);
        itType->SetType(itType.key());
    }
	XGridLabel::Save(m_mapGrig);
	SetModify(false);
}

void CXLabelConstructor::OnButForeground()
{
    QColor previousColor = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetForeground();
    QColor color = QColorDialog::getColor(previousColor, this);
	if(color.isValid())
	{
		const XGridLabel::TMapItem& mapItem = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem();
		if(mapItem.begin() != mapItem.end()) SetModify(true);

		m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].SetForeground(color);

		TLabelItems::const_iterator itItem = m_listLabelItems.begin();
		for(; itItem != m_listLabelItems.end(); ++itItem)
		{
			(*itItem)->SetForeground(color);
		}

        if(previousColor != color)
            SetModify(true);
	}
}

void CXLabelConstructor::OnButBackground()
{
    QColor previousColor = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetBackground();
    QColor color = QColorDialog::getColor(previousColor, this);
	if(color.isValid())
	{
		const XGridLabel::TMapItem& mapItem = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem();
		if(mapItem.begin() != mapItem.end()) SetModify(true);

		m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].SetBackground(color);

		TLabelItems::const_iterator itItem = m_listLabelItems.begin();
		for(; itItem != m_listLabelItems.end(); ++itItem)
		{
			(*itItem)->SetBackground(color);
		}

        if(previousColor != color)
            SetModify(true);
	}
}

void CXLabelConstructor::OnSpinTransparent(int iValue)
{
	if(!m_bPosChange)
	{
		const XGridLabel::TMapItem& mapItem = m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].GetMapItem();
		if(mapItem.begin() != mapItem.end()) SetModify(true);

		m_mapGrig[XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt())].SetTransparent(iValue);

		TLabelItems::const_iterator itItem = m_listLabelItems.begin();
		for(; itItem != m_listLabelItems.end(); ++itItem)
		{
			(*itItem)->SetTransparent(iValue);
		}

        SetModify(true);
	}
}

void CXLabelConstructor::SetDefaultLabel()
{
    XGridLabel defaultLabel;
    XGridLabel::TMapItem& mapItems = defaultLabel.GetMapItem();
    XGridItem labelItem;

    labelItem.SetType(XGridItem::ACID);
    labelItem.SetRow(1);
    labelItem.SetColumn(1);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::ACID, labelItem);

    labelItem.SetType(XGridItem::CODE);
    labelItem.SetRow(2);
    labelItem.SetColumn(1);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::CODE, labelItem);

    labelItem.SetType(XGridItem::LX);
    labelItem.SetRow(2);
    labelItem.SetColumn(3);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::LX, labelItem);

    labelItem.SetType(XGridItem::NS);
    labelItem.SetRow(2);
    labelItem.SetColumn(4);
    labelItem.SetAlignment(Qt::AlignRight|Qt::AlignVCenter);
    mapItems.insert(XGridItem::NS, labelItem);

    labelItem.SetType(XGridItem::AFL);
    labelItem.SetRow(3);
    labelItem.SetColumn(1);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::AFL, labelItem);

    labelItem.SetType(XGridItem::TREND);
    labelItem.SetRow(3);
    labelItem.SetColumn(2);
    labelItem.SetAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mapItems.insert(XGridItem::TREND, labelItem);

    labelItem.SetType(XGridItem::CFL);
    labelItem.SetRow(3);
    labelItem.SetColumn(3);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::CFL, labelItem);

    labelItem.SetType(XGridItem::VRC);
    labelItem.SetRow(3);
    labelItem.SetColumn(4);
    labelItem.SetAlignment(Qt::AlignRight|Qt::AlignVCenter);
    mapItems.insert(XGridItem::VRC, labelItem);

    labelItem.SetType(XGridItem::HDG);
    labelItem.SetRow(4);
    labelItem.SetColumn(1);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::HDG, labelItem);

    labelItem.SetType(XGridItem::BANK);
    labelItem.SetRow(4);
    labelItem.SetColumn(2);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::BANK, labelItem);

    labelItem.SetType(XGridItem::NextPoint);
    labelItem.SetRow(4);
    labelItem.SetColumn(3);
    labelItem.SetAlignment(Qt::AlignRight|Qt::AlignVCenter);
    mapItems.insert(XGridItem::NextPoint, labelItem);

    labelItem.SetType(XGridItem::SPD1);
    labelItem.SetRow(5);
    labelItem.SetColumn(1);
    labelItem.SetAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    mapItems.insert(XGridItem::SPD1, labelItem);

    labelItem.SetType(XGridItem::SPD2);
    labelItem.SetRow(5);
    labelItem.SetColumn(2);
    labelItem.SetAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mapItems.insert(XGridItem::SPD2, labelItem);

    labelItem.SetType(XGridItem::V);
    labelItem.SetRow(5);
    labelItem.SetColumn(4);
    labelItem.SetAlignment(Qt::AlignRight|Qt::AlignVCenter);
    mapItems.insert(XGridItem::V, labelItem);

    XGridLabel::LabelType labelType = XGridLabel::LabelType(comboBoxType->itemData(comboBoxType->currentIndex()).toInt());
    defaultLabel.SetType(labelType);
    m_mapGrig.insert(labelType, defaultLabel);
    OnComboType(comboBoxType->currentIndex());
    SetModify(true);
}

bool CXLabelConstructor::eventFilter(QObject* watched, QEvent* event)
{
    if(watched == graphicsView->viewport())
    {
        if(QEvent::MouseButtonPress == event->type())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::RightButton)
            {
                if(!m_bPull)
                {
                    //graphicsView->viewport()->setCursor(Qt::ClosedHandCursor);
                    graphicsView->setCursor(Qt::ClosedHandCursor);
                }
                m_bPull = true;
                m_PullPoint = graphicsView->mapToScene(mouseEvent->pos());
                event->accept();
                return true;
            }
        }
        else if(QEvent::MouseButtonRelease == event->type())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::RightButton)
            {
                if(m_bPull)
                {
                    //graphicsView->viewport()->unsetCursor();
                    graphicsView->unsetCursor();
                }
                m_bPull = false;
                event->accept();
                return true;
            }
        }
        else if(QEvent::MouseMove == event->type())
        {
            if(m_bPull)
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                QPointF NewPoint(graphicsView->mapToScene(mouseEvent->pos()));
                QPointF DeltaPoint = NewPoint - m_PullPoint;

                QPointF CenterPoint = graphicsView->mapToScene(graphicsView->viewport()->rect().center());
                CenterPoint -= DeltaPoint;
                //graphicsView->centerOn(CenterPoint);
                QMetaObject::invokeMethod(this, "OnPullScene", Qt::QueuedConnection, Q_ARG(QPointF, CenterPoint));
                m_PullPoint = NewPoint - DeltaPoint;
                event->accept();
                return true;
            }
        }
    }
    return QDialog::eventFilter(watched, event);
}

void CXLabelConstructor::OnPullScene(QPointF CenterPoint)
{
    graphicsView->centerOn(CenterPoint);
}

int CXLabelConstructor::get_VIEW_SIZE_X() const
{
    return qCeil(VIEW_SIZE_X * getSizeFactor());
}

int CXLabelConstructor::get_VIEW_SIZE_Y() const
{
    return qCeil(VIEW_SIZE_Y * getSizeFactor());
}

int CXLabelConstructor::get_SEPARATOR_POS() const
{
    return qCeil(SEPARATOR_POS * getSizeFactor());
}

double CXLabelConstructor::getSizeFactor() const
{
    double factor = GetTextSize().GetCurrentTextSize();
    factor /= 10;
    if(factor < 1)
        factor += 0.1;
    if(factor > 0.1 && factor < 5)
        return factor;
    return 1;
}
