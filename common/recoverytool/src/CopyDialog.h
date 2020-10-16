#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>
#include "FileCopy.h"

namespace Ui {
class CCopyDialog;
}

class CCopyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CCopyDialog(QWidget *parent = 0);
    ~CCopyDialog();
    
    void copy(const QString &src, QString dst);

private slots:
    void onCancel();
    void onTick();
    void onDone();

private:
    Ui::CCopyDialog *ui;

    CFileCopy mFileCopy;
};

#endif // COPYDIALOG_H
