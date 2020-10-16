#ifndef IMPORTARINGDIALOG_H
#define IMPORTARINGDIALOG_H

#include "ui_importaringdialog.h"

/**
* @brief Dialog display FIR, that provides by arinc file
*
* @class ImportAringDialog importaringdialog.h
*/
class ImportAringDialog : public QDialog, public Ui::ImportAringDialog {
    Q_OBJECT
public:
    ImportAringDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // IMPORTARINGDIALOG_H
