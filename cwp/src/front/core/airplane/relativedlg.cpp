#include "relativedlg.h"

CRelativeDlg::CRelativeDlg(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f)
{
	setupUi(this);
	connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(setValidator(bool)));
}

void CRelativeDlg::setValidator(bool check)
{
	if(check)
		spinBox->setMaximum(180);
	else
		spinBox->setMaximum(360);
}
