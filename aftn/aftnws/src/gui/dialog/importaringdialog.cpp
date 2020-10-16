#include "importaringdialog.h"

ImportAringDialog::ImportAringDialog(QWidget* parent) :
  QDialog(parent)
{
  setupUi(this);
}

void ImportAringDialog::changeEvent(QEvent* event)
{
  QDialog::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
  }
}
