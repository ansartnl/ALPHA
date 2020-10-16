#ifndef FPLEDITOR_H
#define FPLEDITOR_H

#include "ui_fpleditor.h"
#include "abstractdialog.h"
#include "AFTNDao.h"
#include "fpl.h"

#include <QtCore/QString>

#include <QDialog>

class QAbstractButton;
class QSqlTableModel;
class QSqlQueryModel;
class UppercaseValidator;

/// Dialog display FPL in the detailed style and allows to edit
class FplEditor: public AbstractDialog, public Ui::FplEditor
{
  Q_OBJECT
  public:
    FplEditor(QWidget* pParent = 0);
    virtual ~FplEditor();

    /*! Display dialog as modal */
    void setFpl(const FPL &Fpl);
    void setCanOtherInfoEmpty(bool bCan);
  protected:
    /** Update controls values from database */
    void init();
    /** Save changes (update old plan or save as new) */
    void doSavePlan(void);
    /** Save plan without closing form */
    void doApplyPlan(void);
    /** Copy information from FPL to dialog and over */
    bool fpl(FPL &Fpl);

    virtual void changeEvent(QEvent *event);
  private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
    void onTextChanged();
    void on_ACIDEdit_editingFinished();
    void on_EquipmentEdit_editingFinished();
    void on_AerodromeDepEdit_editingFinished();
    void on_SpeedEdit_editingFinished();
    void on_LevelEdit_editingFinished();
    void on_RouteEdit_editingFinished();
    void on_AerodromeDestEdit_editingFinished();
    void on_ALTNEdit_editingFinished();
    void on_ALTN2Edit_editingFinished();
    void OnFixedSize();
  private:
    FPL mFpl;
    QString validValue;
    QString invalidValue;
    UppercaseValidator *uppercaseValidator;
    QSqlQueryModel* firComboModel;
    QSqlTableModel *aircrafttypesModel;
    QSqlTableModel* flightTypesModel;
    QSqlTableModel* turbulenceModel;
    QSqlTableModel* flightRulesModel;
    QSqlTableModel* levelTypesModel;
    QSqlTableModel* speedTypesModel;
    QSqlTableModel* statusTypesModel;
    QPushButton *printButton;
    bool canOtherInfoEmpty;
};

#endif // FPLEDITOR_H
