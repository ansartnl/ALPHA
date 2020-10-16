#include "AirplaneTypeEditor.h"

#include "TVAError.h"

#include "main.h"

#include "qtautotransaction.h"

#include <QtGui/QCloseEvent>

#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include <QMessageBox>
#include <QPushButton>

const char AirplaneTypeEditor::DB_AIR_TYPE_ID[] = "name";
const char AirplaneTypeEditor::DB_AIR_TYPE_LEVEL_MAX[] = "level_max";
const char AirplaneTypeEditor::DB_AIR_TYPE_BANK_MAX[] = "bank_max";
const char AirplaneTypeEditor::DB_AIR_TYPE_BANK_DEF[] = "bank_default";

const char AirplaneTypeEditor::DB_AIR_SPEED_ID[] = "id";
const char AirplaneTypeEditor::DB_AIR_SPEED_FLEVEL[] = "flevel";
const char AirplaneTypeEditor::DB_AIR_SPEED_V_CLIMB[] = "v_climb";
const char AirplaneTypeEditor::DB_AIR_SPEED_V_CLIMB_MAX[] = "v_climb_max";
const char AirplaneTypeEditor::DB_AIR_SPEED_DESCEND[] = "v_descend";
const char AirplaneTypeEditor::DB_AIR_SPEED_DESCEND_MAX[] = "v_descend_max";
const char AirplaneTypeEditor::DB_AIR_SPEED_IAS[] = "ias";
const char AirplaneTypeEditor::DB_AIR_SPEED_IAS_MIN[] = "ias_min";
const char AirplaneTypeEditor::DB_AIR_SPEED_IAS_MAX[] = "ias_max";
const char AirplaneTypeEditor::DB_AIR_SPEED_AIR_ID[] = "airplane_id";

AirplaneTypeEditor::AirplaneTypeEditor( 
  QWidget *parent) :
  QWidget(parent)
{
  setupUi(this);

  //Type
  m_pTMType = new QSqlRelationalTableModel(this);
  m_pTMType->setTable("AIRPLANE_TYPE");
  m_pTMType->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_mapModelIndexType.insert(DB_AIR_TYPE_ID, m_pTMType->fieldIndex(
    DB_AIR_TYPE_ID));
  m_mapModelIndexType.insert(DB_AIR_TYPE_LEVEL_MAX, m_pTMType->fieldIndex(
    DB_AIR_TYPE_LEVEL_MAX));
  m_mapModelIndexType.insert(DB_AIR_TYPE_BANK_MAX, m_pTMType->fieldIndex(
    DB_AIR_TYPE_BANK_MAX));
  m_mapModelIndexType.insert(DB_AIR_TYPE_BANK_DEF, m_pTMType->fieldIndex(
    DB_AIR_TYPE_BANK_DEF));
  m_pTMType->select();

  MainView->setModel(m_pTMType);
  MainView->horizontalHeader()->moveSection(
    MainView->horizontalHeader()->visualIndex(
      m_mapModelIndexType[DB_AIR_TYPE_ID]), 0);
  MainView->horizontalHeader()->moveSection(
    MainView->horizontalHeader()->visualIndex(
      m_mapModelIndexType[DB_AIR_TYPE_LEVEL_MAX]), 1);
  MainView->horizontalHeader()->moveSection(
    MainView->horizontalHeader()->visualIndex(
      m_mapModelIndexType[DB_AIR_TYPE_BANK_MAX]), 2);
  MainView->horizontalHeader()->moveSection(
    MainView->horizontalHeader()->visualIndex(
      m_mapModelIndexType[DB_AIR_TYPE_BANK_DEF]), 3);
  MainView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
  MainView->resizeColumnsToContents();

  m_pModDelegateType = QSharedPointer<CRelModifyDelegate> (
    new CRelModifyDelegate(MainView));
  connect(m_pTMType, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
    SLOT(OnViewActivate()));
  MainView->setItemDelegate(m_pModDelegateType.data());

  CRelModifyDelegate::TColMap colMap;
  colMap.insert(m_mapModelIndexType[DB_AIR_TYPE_ID], QVariant::String);
  colMap.insert(m_mapModelIndexType[DB_AIR_TYPE_LEVEL_MAX], QVariant::Double);
  colMap.insert(m_mapModelIndexType[DB_AIR_TYPE_BANK_MAX], QVariant::UInt);
  colMap.insert(m_mapModelIndexType[DB_AIR_TYPE_BANK_DEF], QVariant::UInt);
  m_pModDelegateType->SetColsType(colMap);

  //Speed
  m_pTMSpeed = new QSqlRelationalTableModel(this);
  m_pTMSpeed->setTable("AIRPLANE_SPEED");
  m_pTMSpeed->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_ID, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_ID));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_FLEVEL, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_FLEVEL));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_V_CLIMB, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_V_CLIMB));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_V_CLIMB_MAX, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_V_CLIMB_MAX));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_DESCEND, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_DESCEND));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_DESCEND_MAX, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_DESCEND_MAX));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_IAS, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_IAS));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_IAS_MIN, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_IAS_MIN));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_IAS_MAX, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_IAS_MAX));
  m_mapModelIndexSpeed.insert(DB_AIR_SPEED_AIR_ID, m_pTMSpeed->fieldIndex(
    DB_AIR_SPEED_AIR_ID));
  m_pTMSpeed->select();

  SpeedView->setModel(m_pTMSpeed);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_ID]), 0);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_FLEVEL]), 1);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_V_CLIMB]), 2);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_V_CLIMB_MAX]), 3);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_DESCEND]), 4);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_DESCEND_MAX]), 5);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_IAS]), 6);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_IAS_MIN]), 7);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_IAS_MAX]), 8);
  SpeedView->horizontalHeader()->moveSection(
    SpeedView->horizontalHeader()->visualIndex(
      m_mapModelIndexSpeed[DB_AIR_SPEED_AIR_ID]), 9);
  SpeedView->setColumnHidden(m_mapModelIndexSpeed[DB_AIR_SPEED_ID], true);
  SpeedView->setColumnHidden(m_mapModelIndexSpeed[DB_AIR_SPEED_AIR_ID], true);
  SpeedView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
  SpeedView->resizeColumnsToContents();

  m_pModDelegateSpeed = QSharedPointer<CRelModifyDelegate> (
    new CRelModifyDelegate(SpeedView));
  connect(m_pTMSpeed, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this,
    SLOT(OnViewActivate()));
  SpeedView->setItemDelegate(m_pModDelegateSpeed.data());

  colMap.clear();
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_ID], QVariant::UInt);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_FLEVEL], QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_V_CLIMB], QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_V_CLIMB_MAX],
    QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_DESCEND], QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_DESCEND_MAX],
    QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_IAS], QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_IAS_MIN], QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_IAS_MAX], QVariant::Double);
  colMap.insert(m_mapModelIndexSpeed[DB_AIR_SPEED_AIR_ID], QVariant::UInt);
  m_pModDelegateSpeed->SetColsType(colMap);

  Translate();

  ActivateButtons(false);

  connect(OperButtons, SIGNAL(clicked(QAbstractButton*)), this,
    SLOT(OnRuleButtons(QAbstractButton*)));
  connect(AddTypeButton, SIGNAL(clicked()), this, SLOT(OnAddTypeRecord()));
  connect(DeleteTypeButton, SIGNAL(clicked()), this, SLOT(OnDeleteTypeRecord()));
  connect(AddSpeedButton, SIGNAL(clicked()), this, SLOT(OnAddSpeedRecord()));
  connect(DeleteSpeedButtun, SIGNAL(clicked()), this,
    SLOT(OnDeleteSpeedRecord()));
  connect(MainView->selectionModel(),
    SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)), this,
    SLOT(OnTypeChanged(const QModelIndex&)));

  OnTypeChanged(QModelIndex());
}

AirplaneTypeEditor::~AirplaneTypeEditor()
{

}

void AirplaneTypeEditor::changeEvent(QEvent* event)
{
        //LanguageChange events should not be confused with LocaleChange events.
    if (event->type() == QEvent::LocaleChange)
    {
        // HERE we can un|install qtranslator
        //    Locale-Change events are generated by the system and tell the application,
        //    "Maybe you should load a new translation."
    }
    else if (event->type() == QEvent::LanguageChange)
    {
        //    LanguageChange events are generated by Qt and tell the application's widgets,
        //    "Maybe you should retranslate all your strings."
        retranslateUi(this);
        Translate();
    }

    QWidget::changeEvent(event);
}


void AirplaneTypeEditor::Translate()
{
  m_pTMType->setHeaderData(m_mapModelIndexType[DB_AIR_TYPE_ID], Qt::Horizontal,
    tr("Name"));
  m_pTMType->setHeaderData(m_mapModelIndexType[DB_AIR_TYPE_LEVEL_MAX],
    Qt::Horizontal, tr("Level MAX"));
  m_pTMType->setHeaderData(m_mapModelIndexType[DB_AIR_TYPE_BANK_MAX],
    Qt::Horizontal, tr("Bank MAX"));
  m_pTMType->setHeaderData(m_mapModelIndexType[DB_AIR_TYPE_BANK_DEF],
    Qt::Horizontal, tr("Bank default"));

  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_FLEVEL],
    Qt::Horizontal, tr("FLevel"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_V_CLIMB],
    Qt::Horizontal, tr("V Climb"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_V_CLIMB_MAX],
    Qt::Horizontal, tr("V Climb MAX"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_DESCEND],
    Qt::Horizontal, tr("V Descend"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_DESCEND_MAX],
    Qt::Horizontal, tr("V Descend MAX"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_IAS],
    Qt::Horizontal, tr("IAS"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_IAS_MIN],
    Qt::Horizontal, tr("IAS MIN"));
  m_pTMSpeed->setHeaderData(m_mapModelIndexSpeed[DB_AIR_SPEED_IAS_MAX],
    Qt::Horizontal, tr("IAS MAX"));
}

void AirplaneTypeEditor::closeEvent(QCloseEvent* pEvent)
{
  TVA_CATCHALL_TRY
    {
      if (OperButtons->button(QDialogButtonBox::Save)->isEnabled()) {
        switch (QMessageBox::question(this, tr("Unsaved data"), tr(
          "Save modified data?"), QMessageBox::Yes | QMessageBox::No
            | QMessageBox::Cancel)) {
        case QMessageBox::Yes: {
          QtAutoTransaction transaction(QSqlDatabase::database());
          if (m_pTMSpeed->submitAll()) {
            if (m_pTMType->submitAll()) {
              transaction.commit();
              pEvent->accept();
            } else {
              QMessageBox::critical(this, tr("Error"),
                m_pTMType->lastError().text());
              pEvent->ignore();
            }
          } else {
            QMessageBox::critical(this, tr("Error"),
              m_pTMSpeed->lastError().text());
            pEvent->ignore();
          }
        }
          break;
        case QMessageBox::No:
          pEvent->accept();
          break;
        case QMessageBox::Cancel:
          pEvent->ignore();
          break;
        default:
          break;
        }

      } else
        pEvent->accept();
    }TVA_CATCHALL(err){
  LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
  QMessageBox::critical(this, tr("Error"), err.reason());
}
}

void AirplaneTypeEditor::OnRuleButtons(QAbstractButton* pButton)
{
  TVA_CATCHALL_TRY
    {
      QDialogButtonBox::ButtonRole BtnRole = OperButtons->buttonRole(pButton);
      switch (BtnRole) {
      case QDialogButtonBox::AcceptRole: {
        QtAutoTransaction transaction(QSqlDatabase::database());
        TVA_CHECK(
            m_pTMSpeed->submitAll(),
            m_pTMSpeed->lastError().text(),
            E_FAIL);
        TVA_CHECK(
            m_pTMType->submitAll(),
            m_pTMType->lastError().text(),
            E_FAIL);
        transaction.commit();
        ActivateButtons(false);
      }
        break;
      case QDialogButtonBox::RejectRole:
      case QDialogButtonBox::DestructiveRole:
        m_pTMSpeed->revertAll();
        m_pTMSpeed->submitAll();
        m_pTMType->revertAll();
        m_pTMType->submitAll();
        ActivateButtons(false);
        break;
      default:
        break;
      }
    }TVA_CATCHALL(err){
  LOG_TVAERROR(Log4Qt::Level::ERROR_INT, err);
  QMessageBox::critical(this, tr("Error"), err.reason());
}
}

void AirplaneTypeEditor::OnViewActivate()
{
  ActivateButtons(true);
}

void AirplaneTypeEditor::ActivateButtons(bool bActive)
{
  OperButtons->button(QDialogButtonBox::Save)->setEnabled(bActive);
  if(OperButtons->button(QDialogButtonBox::Discard))
    OperButtons->button(QDialogButtonBox::Discard)->setEnabled(bActive);
  if(OperButtons->button(QDialogButtonBox::Cancel))
    OperButtons->button(QDialogButtonBox::Cancel)->setEnabled(bActive);
}

void AirplaneTypeEditor::OnAddTypeRecord()
{
  int iRow = m_pTMType->rowCount();
  m_pTMType->insertRow(iRow);
  QModelIndex qmIndex = m_pTMType->index(iRow,
    m_mapModelIndexType[DB_AIR_TYPE_ID]);
  MainView->setCurrentIndex(qmIndex);
  MainView->edit(qmIndex);
  ActivateButtons(true);
}

void AirplaneTypeEditor::OnDeleteTypeRecord()
{
  QModelIndexList SelectList =
      MainView->selectionModel()->selection().indexes();
  QModelIndexList::iterator itIndScan = SelectList.begin();
  for (; itIndScan != SelectList.end(); ++itIndScan) {
    if (itIndScan->isValid()) {
      m_pTMType->removeRow(itIndScan->row());
      ActivateButtons(true);
    }
  }
}

void AirplaneTypeEditor::OnTypeChanged(const QModelIndex& ModInd)
{
  if (ModInd.isValid()) {
    QSqlRecord qRec = m_pTMType->record(ModInd.row());
    m_sCurAirType = qRec.value(m_mapModelIndexType[DB_AIR_TYPE_ID]).toString();
    m_pTMSpeed->setFilter("airplane_id = '" + m_sCurAirType + "'");
  } else {
    m_pTMSpeed->setFilter("0 = 1");
    m_sCurAirType.clear();
  }
  m_pTMSpeed->select();

}

void AirplaneTypeEditor::OnAddSpeedRecord()
{
  if (!m_sCurAirType.isEmpty()) {
    int iRow = m_pTMSpeed->rowCount();
    m_pTMSpeed->insertRow(iRow);
    QSqlRecord qRec = m_pTMSpeed->record(iRow);
    qRec.setValue(m_mapModelIndexSpeed[DB_AIR_SPEED_AIR_ID], m_sCurAirType);
    m_pTMSpeed->setRecord(iRow, qRec);
    QModelIndex qmIndex = m_pTMSpeed->index(iRow,
      m_mapModelIndexSpeed[DB_AIR_SPEED_FLEVEL]);
    SpeedView->setCurrentIndex(qmIndex);
    SpeedView->edit(qmIndex);
    ActivateButtons(true);
  }
}

void AirplaneTypeEditor::OnDeleteSpeedRecord()
{
  QModelIndexList SelectList =
      SpeedView->selectionModel()->selection().indexes();
  QModelIndexList::iterator itIndScan = SelectList.begin();
  for (; itIndScan != SelectList.end(); ++itIndScan) {
    if (itIndScan->isValid()) {
      m_pTMSpeed->removeRow(itIndScan->row());
      ActivateButtons(true);
    }
  }
}
