#ifndef RELATIVEDLG_H
#define RELATIVEDLG_H

#include <QDialog>
#include "ui_Azimuth.h"

//! Class for editing azimuth
class CRelativeDlg : public QDialog
		, public Ui::RelativeDlg
{
Q_OBJECT
public:
	explicit CRelativeDlg(QWidget *parent = 0, Qt::WindowFlags f = 0);

public slots:
	void setValidator(bool check);
};

#endif // RELATIVEDLG_H
