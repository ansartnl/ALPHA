#include "notamviewer.h"

#include <QtGui/QPushButton>

NotamViewer::NotamViewer(QWidget* pParent) :
  AbstractDialog(pParent), mNotam()
{
  setupUi(this);

  printButton = new QPushButton(tr("Print..."), this);
  buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);
  restoreState(staticMetaObject.className());
}

NotamViewer::~NotamViewer()
{
  saveState(staticMetaObject.className());
}

/** Copy information from NOTAM to dialog and over */
void NotamViewer::setNotam(const Notam& Notam)
{
  mNotam = Notam;
  setWindowTitle(tr("Displaying NOTAM #%1").arg(mNotam.id()));
  SeriesEdit->setText(mNotam.series());
  SnumberSpin ->setValue(mNotam.serialNumber());
  YearSpin->setValue(mNotam.serialYear());
  TypeEdit->setText(mNotam.nature());

  if (mNotam.nature() != QString("N")) {
    ReplSnumberSpin->show();
    ReplYearSpin->show();
    ReplSnumberLabel->show();
    ReplYearLabel->show();
    ReplSnumberSpin->setValue(mNotam.replaceSerialNumber());
    ReplYearSpin->setValue(mNotam.replaceSerialYear());
  } else {
    ReplSnumberSpin->hide();
    ReplYearSpin->hide();
    ReplSnumberLabel->hide();
    ReplYearLabel->hide();
  }

  FirEdit->setText(mNotam.FIR());
  SubjectEdit->setText(mNotam.subject());
  ConditionEdit->setText(mNotam.condition());
  TrafficEdit->setText(mNotam.traffic());
  PurposeEdit->setText(mNotam.purpose());
  ScopeEdit->setText(mNotam.scope());

  LowerLimitSpin->setValue(mNotam.qLowerLimit());
  UpperLimitSpin->setValue(mNotam.qUpperLimit());
  LocationEdit->setText(mNotam.location());
  RadiusSpin->setValue(mNotam.radius());
  AerodromEdit->setText(mNotam.aerodrome());
  ValidFromEdit->setText(
    mNotam.validFrom().toString(QLocale().dateTimeFormat()));

  if (mNotam.validUntilT() == "PERM") {
    EstimateCheck->setChecked(false);
    ValidUntilEdit->setText(tr("permanent"));
  } else {
    EstimateCheck->setChecked(mNotam.validUntilT() == "EST");
    ValidUntilEdit->setText(mNotam.validUntil().toString(
      QLocale().dateTimeFormat()));
  }

  ScheduleEdit->setPlainText(mNotam.schedule());
  DescriptionEdit->setPlainText(mNotam.description());
  LowerLimitEdit->setText(mNotam.lowerLimit());
  UpperLimitEdit->setText(mNotam.upperLimit());
}

void NotamViewer::changeEvent(QEvent* event)
{
  QDialog::changeEvent(event);

  if (event->type() == QEvent::LanguageChange) {
    retranslateUi(this);
    printButton->setText(tr("Print..."));
  }
}

void NotamViewer::on_buttonBox_clicked(QAbstractButton *button)
{
  if (button == printButton)
    printWidget(frame);
}
