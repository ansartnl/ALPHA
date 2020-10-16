#ifndef CGUILISTCONSTRUCTOR_H
#define CGUILISTCONSTRUCTOR_H

#include <QDialog>
#include "EditHeaderDialog.h"
#include "ui_EditHeaderDialog.h"

namespace Ui {
class CGUIListConstructor;
}

class CGUIListConstructor
        : public QDialog
{
    Q_OBJECT
    
public:
    explicit CGUIListConstructor(QWidget *parent = 0);
    ~CGUIListConstructor();

    void setUsername(QString UserName)
    {
        sUserName = UserName;
        //for all users, nah
        sUserName = "ListConstructor";
        loadUserData();
    }

    QString sResult;
protected:
    void loadUserData();
    void loadListData();
    
private:
    void SetupNewDialogWidget();
    void translate();

    Ui::CGUIListConstructor *ui;
    EditHeaderDialog* pEditDlg;
    QColor  m_WarningColor;
    QColor  m_DangerColor;
    QString sUserName;

protected slots:
    void OnBtnWarning();
    void OnBtnDanger();
    void OnBtnAllDefault();
    void OnBtnCurrentDefault();
    void onAccepted();
    void onRejected();
    void OnIndexChanged(QString sCur);
};

#endif // CGUILISTCONSTRUCTOR_H
