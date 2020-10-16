#include "MeteoEditor.h"
#include "Scene.h"
#include "TVAError.h"

#include "main.h"

#include <QtGui/QIntValidator>
#include <QtGui/QDoubleValidator>
#include <QtGui/QFocusEvent>

#include <QMessageBox>
#include <QAction>
#include <QTableWidget>
#include <QComboBox>
#include <QHeaderView>

namespace gui
{

	CXMeteoNameLineEdit::CXMeteoNameLineEdit(unsigned int uiMeteoID, const QString &sName, QWidget *pParent)
		: QLineEdit(pParent), m_uiMeteoID(uiMeteoID), m_sName(sName)
	{
		setMaxLength(20);
		setText(m_sName);

		connect(this, SIGNAL(editingFinished()), this, SLOT(OnNameChanged()));
	}

	void CXMeteoNameLineEdit::OnNameChanged()
	{
		QString sName = text();
		if(m_sName != sName)
		{
			m_sName = sName;
			emit nameChanged(m_uiMeteoID, m_sName);
		}
	}

	void CXMeteoNameLineEdit::focusInEvent(QFocusEvent *pEvent)
	{
		if(pEvent->reason() == Qt::MouseFocusReason)
			emit nameActivated(m_uiMeteoID);
		QLineEdit::focusInEvent(pEvent);
	}

	CXMeteoTypeComboBox::CXMeteoTypeComboBox(unsigned int uiMeteoID, core::CMeteo::Type Type, QWidget *pParent)
		: QComboBox(pParent), m_uiMeteoID(uiMeteoID), m_Type(Type)
	{
		typedef QMap<core::CMeteo::Type, QString> CTypeMap;
		CTypeMap Types = core::CMeteo::Types();

		clear();
		for(CTypeMap::const_iterator Iter = Types.begin(); Iter != Types.end(); ++ Iter)
		{
			addItem(Iter.value(), (int) Iter.key());
			if(Iter.key() == Type) setCurrentIndex(count() - 1);
		}

		connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTypeChanged(int)));
	}

	void CXMeteoTypeComboBox::OnTypeChanged(int iTypeIndex)
	{
		core::CMeteo::Type Type = (core::CMeteo::Type) itemData(iTypeIndex).toInt();
		if(m_Type != Type)
		{
			m_Type = Type;
			emit typeChanged(m_uiMeteoID, m_Type);
		}
	}

	void CXMeteoTypeComboBox::focusInEvent(QFocusEvent *pEvent)
	{
		if(pEvent->reason() == Qt::MouseFocusReason)
			emit typeActivated(m_uiMeteoID);
		QComboBox::focusInEvent(pEvent);
	}

	CXMeteoPointSNLineEdit::CXMeteoPointSNLineEdit(unsigned int uiMeteoPointID, int iSN, QWidget *pParent)
			: QLineEdit(pParent), m_pValidator(NULL), m_uiMeteoPointID(uiMeteoPointID), m_iSN(iSN)
	{
		setMaxLength(4);
		setText(QString("%1").arg(m_iSN));

		m_pValidator = new QIntValidator(this);
		m_pValidator->setRange(0, 9999);
		setValidator(m_pValidator);

		connect(this, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
	}

	void CXMeteoPointSNLineEdit::OnEditFinished()
	{
		int iSN = text().toInt();
		if(m_iSN != iSN)
		{
			m_iSN = iSN;
			emit snChanged(m_uiMeteoPointID, m_iSN);
		}
	}

	void CXMeteoPointSNLineEdit::focusInEvent(QFocusEvent *pEvent)
	{
		if(pEvent->reason() == Qt::MouseFocusReason)
			emit snActivated(m_uiMeteoPointID);
		QLineEdit::focusInEvent(pEvent);
	}

	CXMeteoPointLatLongLineEdit::CXMeteoPointLatLongLineEdit(unsigned int uiMeteoPointID, double dValue,
															 double dMinValue, double dMaxValue, QWidget *pParent)
			: QLineEdit(pParent), m_pValidator(NULL), m_uiMeteoPointID(uiMeteoPointID), m_dValue(dValue)
	{
		setMaxLength(10);
		setText(QString("%1").arg(m_dValue));

		m_pValidator = new QDoubleValidator(this);
		m_pValidator->setNotation(QDoubleValidator::StandardNotation);
		m_pValidator->setRange(dMinValue, dMaxValue, 10);
		setValidator(m_pValidator);

		connect(this, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
	}

	void CXMeteoPointLatLongLineEdit::OnEditFinished()
	{
		double dValue = text().toDouble();
		if(m_dValue != dValue)
		{
			m_dValue = dValue;
			emit valueChanged(m_uiMeteoPointID, m_dValue);
		}
	}

	void CXMeteoPointLatLongLineEdit::focusInEvent(QFocusEvent *pEvent)
	{
		if(pEvent->reason() == Qt::MouseFocusReason)
			emit valueActivated(m_uiMeteoPointID);
		QLineEdit::focusInEvent(pEvent);
	}

	CXMeteoEditor::CXMeteoEditor( QWidget *pParent)
		: QSplitter(pParent)

	{
		{
			m_pMeteosTable = new QTableWidget(this);
			m_pMeteosTable->horizontalHeader()->setMinimumHeight(30);
			m_pMeteosTable->showMaximized();
			m_pMeteosTable->setAutoFillBackground(false);
			m_pMeteosTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pMeteosTable->setAlternatingRowColors(true);
			m_pMeteosTable->setSelectionMode(QAbstractItemView::SingleSelection);
			m_pMeteosTable->setSelectionBehavior(QAbstractItemView::SelectItems);
			m_pMeteosTable->setSortingEnabled(true);
			m_pMeteosTable->horizontalHeader()->setCascadingSectionResizes(false);
			m_pMeteosTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
			m_pMeteosTable->verticalHeader()->setDefaultSectionSize(20);
			m_pMeteosTable->setColumnCount(2);

			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			sizePolicy.setHorizontalStretch(0);
			sizePolicy.setVerticalStretch(0);
			sizePolicy.setHeightForWidth(m_pMeteosTable->sizePolicy().hasHeightForWidth());
			m_pMeteosTable->setSizePolicy(sizePolicy);

			connect(m_pMeteosTable, SIGNAL(itemSelectionChanged()),
					this, SLOT(OnMeteoSelected()));
		}

		{
			m_pPointsTable = new QTableWidget(this);
			m_pPointsTable->horizontalHeader()->setMinimumHeight(30);
			m_pPointsTable->showMaximized();
			m_pPointsTable->setAutoFillBackground(false);
			m_pPointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pPointsTable->setAlternatingRowColors(true);
			m_pPointsTable->setSelectionMode(QAbstractItemView::SingleSelection);
			m_pPointsTable->setSelectionBehavior(QAbstractItemView::SelectItems);
			m_pPointsTable->setSortingEnabled(true);
			m_pPointsTable->horizontalHeader()->setCascadingSectionResizes(false);
			m_pPointsTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
			m_pPointsTable->verticalHeader()->setDefaultSectionSize(20);
			m_pPointsTable->setColumnCount(3);

			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			sizePolicy.setHorizontalStretch(0);
			sizePolicy.setVerticalStretch(0);
			sizePolicy.setHeightForWidth(m_pPointsTable->sizePolicy().hasHeightForWidth());
			m_pPointsTable->setSizePolicy(sizePolicy);

			connect(m_pPointsTable, SIGNAL(itemSelectionChanged()),
					this, SLOT(OnMeteoPointSelected()));
		}

		Translate();
		addWidget(m_pMeteosTable);
		addWidget(m_pPointsTable);
		setOrientation(Qt::Vertical);
	}

	CXMeteoEditor::~CXMeteoEditor()
	{}

	void CXMeteoEditor::OnMeteoSelected()
	{
		int iRow = m_pMeteosTable->currentRow();
		if(0 <= iRow && iRow < m_MeteoIndexes.count())
		{
			OnMeteoActivated(m_MeteoIndexes[iRow]);
		}
		else
		{
			OnMeteoActivated(0);
		}
	}

	void CXMeteoEditor::OnMeteoPointSelected()
	{
		int iRow = m_pPointsTable->currentRow();
		if(0 <= iRow && iRow < m_PointIndexes.count())
		{
			OnMeteoPointActivated(m_PointIndexes[iRow]);
		}
		else
		{
			OnMeteoPointActivated(0);
		}
	}


	void CXMeteoEditor::Translate()
	{
		setWindowTitle(tr("Meteo"));
		m_pMeteosTable->setHorizontalHeaderLabels(QStringList()
												  << tr("Name")
												  << tr("Type"));
		m_pPointsTable->setHorizontalHeaderLabels(QStringList()
												  << tr("Number")
												  << tr("Longitude")
												  << tr("Latitude"));
	}

	void CXMeteoEditor::UpdateActions(QComboBox *pZonesCombo, QAction *pNewMeteoAction, QAction *pDeleteMeteoAction,
									  QAction *pNewMeteoPointAction, QAction *pDeleteMeteoPointAction)
	{
		m_pNewMeteoAction         = pNewMeteoAction;
		m_pDeleteMeteoAction      = pDeleteMeteoAction;
		m_pNewMeteoPointAction    = pNewMeteoPointAction;
		m_pDeleteMeteoPointAction = pDeleteMeteoPointAction;

		m_pZonesCombo = pZonesCombo;
		disconnect(m_pZonesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnZoneChanged(int)));
		connect(m_pZonesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(OnZoneChanged(int)));

		m_pNewMeteoAction->setText(tr("New Meteo"));
		m_pNewMeteoAction->setStatusTip(tr("Create new Meteo"));
		m_pNewMeteoAction->setToolTip(tr("Create new Meteo"));
		disconnect(m_pNewMeteoAction, SIGNAL(triggered()), this, SLOT(OnNewMeteo()));
		connect(m_pNewMeteoAction, SIGNAL(triggered()), this, SLOT(OnNewMeteo()));

		m_pDeleteMeteoAction->setText(tr("Delete Meteo"));
		m_pDeleteMeteoAction->setStatusTip(tr("Delete selected Meteo"));
		m_pDeleteMeteoAction->setToolTip(tr("Delete selected Meteo"));
		disconnect(m_pDeleteMeteoAction, SIGNAL(triggered()), this, SLOT(OnDeleteMeteo()));
		connect(m_pDeleteMeteoAction, SIGNAL(triggered()), this, SLOT(OnDeleteMeteo()));

		m_pNewMeteoPointAction->setText(tr("New Meteo Point"));
		m_pNewMeteoPointAction->setStatusTip(tr("Create new Meteo Point"));
		m_pNewMeteoPointAction->setToolTip(tr("Create new Meteo Point"));
		disconnect(m_pNewMeteoPointAction, SIGNAL(triggered()), this, SLOT(OnNewMeteoPoint()));
		connect(m_pNewMeteoPointAction, SIGNAL(triggered()), this, SLOT(OnNewMeteoPoint()));

		m_pDeleteMeteoPointAction->setText(tr("Delete Meteo Point"));
		m_pDeleteMeteoPointAction->setStatusTip(tr("Delete selected Meteo Point"));
		m_pDeleteMeteoPointAction->setToolTip(tr("Delete selected Meteo Point"));
		disconnect(m_pDeleteMeteoPointAction, SIGNAL(triggered()), this, SLOT(OnDeleteMeteoPoint()));
		connect(m_pDeleteMeteoPointAction, SIGNAL(triggered()), this, SLOT(OnDeleteMeteoPoint()));

		TVA_CATCHALL_TRY
		{
			// load zones to combo
			Dao::TSceneMap FIRs;
			dao()->LoadFIRs(FIRs);

			m_pZonesCombo->clear();
			for(Dao::TSceneMap::const_iterator FIR = FIRs.begin(); FIR != FIRs.end(); ++ FIR)
			{
				QSharedPointer<Scene> pscene = *FIR;
				m_pZonesCombo->addItem(pscene->GetIDName(), pscene->GetID());
			}
			m_pZonesCombo->setCurrentIndex(0);

			// update Meteos
			if(false == FIRs.empty())
				OnZoneChanged(m_pZonesCombo->currentIndex());
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}

	}

	void CXMeteoEditor::OnZoneChanged(int iZoneIndex)
	{
		unsigned int uiZoneID = m_pZonesCombo->itemData(iZoneIndex).toUInt();

		ReloadMeteos(uiZoneID);

		if(!m_MeteoIndexes.empty())
			SelectMeteo(m_MeteoIndexes[0]);
	}

	void CXMeteoEditor::OnMeteoNameActivated(unsigned int uiMeteoID)
	{
		m_pMeteosTable->setCurrentCell(MeteoIDToRow(uiMeteoID), 0);
		OnMeteoActivated(uiMeteoID);
	}
	void CXMeteoEditor::OnMeteoTypeActivated(unsigned int uiMeteoID)
	{
		m_pMeteosTable->setCurrentCell(MeteoIDToRow(uiMeteoID), 1);
		OnMeteoActivated(uiMeteoID);
	}
	void CXMeteoEditor::OnMeteoActivated(unsigned int uiMeteoID)
	{
		if(0 != uiMeteoID)
		{
			m_pDeleteMeteoAction->setEnabled(true);
			m_pPointsTable->setEnabled(true);
			m_pNewMeteoPointAction->setEnabled(true);
		}
		else
		{
			m_pDeleteMeteoAction->setDisabled(true);
			m_pPointsTable->setDisabled(true);
			m_pNewMeteoPointAction->setDisabled(true);
			m_pDeleteMeteoPointAction->setDisabled(true);
		}

		ReloadMeteoPoints(uiMeteoID);
	}

	void CXMeteoEditor::OnMeteoPointSNActivated(unsigned int uiMeteoPointID)
	{
		m_pPointsTable->setCurrentCell(MeteoPointIDToRow(uiMeteoPointID), 0);
		OnMeteoPointActivated(uiMeteoPointID);
	}
	void CXMeteoEditor::OnMeteoPointLongitudeActivated(unsigned int uiMeteoPointID)
	{
		m_pPointsTable->setCurrentCell(MeteoPointIDToRow(uiMeteoPointID), 1);
		OnMeteoPointActivated(uiMeteoPointID);
	}
	void CXMeteoEditor::OnMeteoPointLatitudeActivated(unsigned int uiMeteoPointID)
	{
		m_pPointsTable->setCurrentCell(MeteoPointIDToRow(uiMeteoPointID), 2);
		OnMeteoPointActivated(uiMeteoPointID);
	}
	void CXMeteoEditor::OnMeteoPointActivated(unsigned int uiMeteoPointID)
	{
		if(0 != uiMeteoPointID)
		{
			m_pDeleteMeteoPointAction->setEnabled(true);
		}
		else
		{
			m_pDeleteMeteoPointAction->setDisabled(true);
		}
	}

	void CXMeteoEditor::OnNewMeteo()
	{
		TVA_CATCHALL_TRY
		{
			core::CMeteo Meteo;
			Meteo.SetZoneID(m_pZonesCombo->itemData(m_pZonesCombo->currentIndex()).toUInt());
			Meteo.SetName("<unassigned>");
			Meteo.SetType(core::CMeteo::Cloudburst);

			dao()->SaveMeteo(Meteo);

			AddMeteo(Meteo);
			SelectMeteo(Meteo.GetID());
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnDeleteMeteo()
	{
		int iRow = m_pMeteosTable->currentRow();
		if(0 > iRow || iRow >= m_MeteoIndexes.count())
			return;

		unsigned int uiMeteoID = m_MeteoIndexes[iRow];

		QString sText = QString(tr("Delete meteo '%1'?").arg(m_MeteoIDs[uiMeteoID].GetName()));
		if(QMessageBox::warning(this, tr("Meteo"), sText, QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
		TVA_CATCHALL_TRY
		{
			core::CMeteo Meteo = m_MeteoIDs[uiMeteoID];
			dao()->DeleteMeteo(Meteo);

			m_MeteoIDs.remove(uiMeteoID);
			m_MeteoIndexes.removeAt(iRow);
			m_pMeteosTable->removeRow(iRow);

			if(iRow < m_MeteoIndexes.count())
				SelectMeteo(m_MeteoIndexes[iRow]);
			else if(!m_MeteoIndexes.empty())
				SelectMeteo(m_MeteoIndexes[m_MeteoIndexes.count() - 1]);
			else
				SelectMeteo(0);
		} TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnNewMeteoPoint()
	{
		TVA_CATCHALL_TRY
		{
			int iRow = m_pMeteosTable->currentRow();
			if(0 > iRow || iRow >= m_MeteoIndexes.count())
				return;

			unsigned int uiMeteoID = m_MeteoIndexes[iRow];

			core::CMeteoPoint MeteoPoint;
			MeteoPoint.SetMeteoID(uiMeteoID);
			MeteoPoint.SetSN(0);
			MeteoPoint.SetPosition(CCoord());

			dao()->SaveMeteoPoint(MeteoPoint);

			AddMeteoPoint(MeteoPoint);
			SelectMeteoPoint(MeteoPoint.GetID());
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnDeleteMeteoPoint()
	{
		int iRow = m_pPointsTable->currentRow();
		if(0 > iRow || iRow >= m_PointIndexes.count())
			return;

		unsigned int uiMeteoPointID = m_PointIndexes[iRow];

		QString sText = QString(tr("Delete meteo point '%1'?").arg(m_PointIDs[uiMeteoPointID].GetSN()));
		if(QMessageBox::warning(this, tr("Meteo point"), sText, QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
		TVA_CATCHALL_TRY
		{
			core::CMeteoPoint MeteoPoint = m_PointIDs[uiMeteoPointID];
			dao()->DeleteMeteoPoint(MeteoPoint);

			m_PointIDs.remove(uiMeteoPointID);
			m_PointIndexes.removeAt(iRow);
			m_pPointsTable->removeRow(iRow);

			if(iRow < m_PointIndexes.count())
				SelectMeteoPoint(m_PointIndexes[iRow]);
			else if(!m_PointIndexes.empty())
				SelectMeteoPoint(m_PointIndexes[m_PointIndexes.count() - 1]);
			else
				SelectMeteoPoint(0);
		} TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnMeteoNameChanged(unsigned int uiMeteoID, const QString &sName)
	{
		TVA_CATCHALL_TRY
		{
			core::CMeteo &Meteo = m_MeteoIDs[uiMeteoID];
			Meteo.SetName(sName);
			dao()->SaveMeteo(Meteo);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnMeteoTypeChanged(unsigned int uiMeteoID, core::CMeteo::Type Type)
	{
		TVA_CATCHALL_TRY
		{
			core::CMeteo &Meteo = m_MeteoIDs[uiMeteoID];
			Meteo.SetType(Type);
			dao()->SaveMeteo(Meteo);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnMeteoPointSNChanged(unsigned int uiMeteoPointID, int iSN)
	{
		TVA_CATCHALL_TRY
		{
			core::CMeteoPoint &MeteoPoint = m_PointIDs[uiMeteoPointID];
			MeteoPoint.SetSN(iSN);
			dao()->SaveMeteoPoint(MeteoPoint);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnMeteoPointLongitudeChanged(unsigned int uiMeteoPointID, double dLongitude)
	{
		TVA_CATCHALL_TRY
		{
			core::CMeteoPoint &MeteoPoint = m_PointIDs[uiMeteoPointID];
			MeteoPoint.GetPosition().SetLongitude(CAngle(dLongitude));
			dao()->SaveMeteoPoint(MeteoPoint);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::OnMeteoPointLatitudeChanged(unsigned int uiMeteoPointID, double dLatitude)
	{
		TVA_CATCHALL_TRY
		{
			core::CMeteoPoint &MeteoPoint = m_PointIDs[uiMeteoPointID];
			MeteoPoint.GetPosition().SetLatitude(CAngle(dLatitude));
			dao()->SaveMeteoPoint(MeteoPoint);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::ReloadMeteos(unsigned int uiZoneID)
	{
		TVA_CATCHALL_TRY
		{
			unsigned int uiID = uiZoneID;

			// load Meteos from DB
			typedef QList<core::CMeteo> CMeteosList;
			CMeteosList Meteos;
			dao()->LoadMeteos(Meteos, uiID);

			// prepare table
			m_MeteoIDs.clear();
			m_MeteoIndexes.clear();
			m_pMeteosTable->clearContents();
			m_pMeteosTable->setRowCount(0);

			// show all Meteos in table
			for(CMeteosList::const_iterator Iter = Meteos.begin(); Iter != Meteos.end(); ++ Iter)
				AddMeteo(*Iter);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::ReloadMeteoPoints(unsigned int uiMeteoID)
	{
		TVA_CATCHALL_TRY
		{
			// load MeteoPointss from DB
			typedef QList<core::CMeteoPoint> CMeteoPointsList;
			CMeteoPointsList MeteoPoints;
			dao()->LoadMeteoPoints(MeteoPoints, uiMeteoID);

			// prepare table
			m_PointIDs.clear();
			m_PointIndexes.clear();
			m_pPointsTable->clearContents();
			m_pPointsTable->setRowCount(0);

			// show all Meteos in table
			for(CMeteoPointsList::const_iterator Iter = MeteoPoints.begin(); Iter != MeteoPoints.end(); ++ Iter)
				AddMeteoPoint(*Iter);
		}
		TVA_CATCHALL(err)
		{
			QMessageBox::critical(this, tr("Error"), err.reason());
		}
	}

	void CXMeteoEditor::AddMeteo(const core::CMeteo &Meteo)
	{
		int Row = m_pMeteosTable->rowCount();
		m_pMeteosTable->insertRow(Row);
		m_MeteoIDs[Meteo.GetID()] = Meteo;
		m_MeteoIndexes.append(Meteo.GetID());

		CXMeteoNameLineEdit *qName = new CXMeteoNameLineEdit(
				Meteo.GetID(), Meteo.GetName());
		connect(qName, SIGNAL(nameChanged(uint,QString)), this, SLOT(OnMeteoNameChanged(uint,QString)));
		connect(qName, SIGNAL(nameActivated(uint)), this, SLOT(OnMeteoNameActivated(uint)));

		CXMeteoTypeComboBox *qType = new CXMeteoTypeComboBox(
				Meteo.GetID(), Meteo.GetType());
		connect(qType, SIGNAL(typeChanged(uint,core::CMeteo::Type)), this, SLOT(OnMeteoTypeChanged(uint,core::CMeteo::Type)));
		connect(qType, SIGNAL(typeActivated(uint)), this, SLOT(OnMeteoTypeActivated(uint)));

		m_pMeteosTable->setCellWidget(Row, 0, qName);
		m_pMeteosTable->setCellWidget(Row, 1, qType);
	}

	void CXMeteoEditor::AddMeteoPoint(const core::CMeteoPoint &MeteoPoint)
	{
		int Row = m_pPointsTable->rowCount();
		m_pPointsTable->insertRow(Row);
		m_PointIDs[MeteoPoint.GetID()] = MeteoPoint;
		m_PointIndexes.append(MeteoPoint.GetID());

		CXMeteoPointSNLineEdit *qSN = new CXMeteoPointSNLineEdit(
				MeteoPoint.GetID(), MeteoPoint.GetSN());
		connect(qSN, SIGNAL(snChanged(uint,int)), this, SLOT(OnMeteoPointSNChanged(uint,int)));
		connect(qSN, SIGNAL(snActivated(uint)), this, SLOT(OnMeteoPointSNActivated(uint)));

		CXMeteoPointLatLongLineEdit *qLongitude = new CXMeteoPointLatLongLineEdit(
				MeteoPoint.GetID(), MeteoPoint.GetPosition().GetLongitude().GetAngle(), -180, +180);
		connect(qLongitude, SIGNAL(valueChanged(uint,double)), this, SLOT(OnMeteoPointLongitudeChanged(uint,double)));
		connect(qLongitude, SIGNAL(valueActivated(uint)), this, SLOT(OnMeteoPointLongitudeActivated(uint)));

		CXMeteoPointLatLongLineEdit *qLatitude = new CXMeteoPointLatLongLineEdit(
				MeteoPoint.GetID(), MeteoPoint.GetPosition().GetLatitude().GetAngle(), -90, +90);
		connect(qLatitude, SIGNAL(valueChanged(uint,double)), this, SLOT(OnMeteoPointLatitudeChanged(uint,double)));
		connect(qLatitude, SIGNAL(valueActivated(uint)), this, SLOT(OnMeteoPointLatitudeActivated(uint)));

		m_pPointsTable->setCellWidget(Row, 0, qSN);
		m_pPointsTable->setCellWidget(Row, 1, qLongitude);
		m_pPointsTable->setCellWidget(Row, 2, qLatitude);
	}

	void CXMeteoEditor::SelectMeteo(unsigned int uiMeteoID)
	{
		int iRow = MeteoIDToRow(uiMeteoID);
		if(-1 != iRow)
		{
			m_pMeteosTable->cellWidget(iRow, 0)->setFocus();
			OnMeteoNameActivated(uiMeteoID);
		}
		else
		{
			OnMeteoActivated(0);
		}
	}

	int CXMeteoEditor::MeteoIDToRow(unsigned int uiMeteoID)
	{
		for(int iRow = 0; iRow < m_MeteoIndexes.count(); ++ iRow)
			if(uiMeteoID == m_MeteoIndexes[iRow])
				return iRow;
		return -1;
	}

	void CXMeteoEditor::SelectMeteoPoint(unsigned int uiMeteoPointID)
	{
		int iRow = MeteoPointIDToRow(uiMeteoPointID);
		if(-1 != iRow)
		{
			m_pPointsTable->cellWidget(iRow, 0)->setFocus();
			OnMeteoPointSNActivated(uiMeteoPointID);
		}
		else
		{
			OnMeteoPointActivated(0);
		}
	}

	int CXMeteoEditor::MeteoPointIDToRow(unsigned int uiMeteoPointID)
	{
		for(int iRow = 0; iRow < m_PointIndexes.count(); ++ iRow)
			if(uiMeteoPointID == m_PointIndexes[iRow])
				return iRow;
		return -1;
	}

}
