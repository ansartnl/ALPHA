#ifndef SFPLEDITOR_H
#define SFPLEDITOR_H

#include "ui_sfpleditor.h"
#include "abstractdialog.h"
#include "SFPL.h"

#include <QtCore/QString>

#include <QDialog>

class QAbstractButton;
class QSqlTableModel;
class QSqlQueryModel;
class UppercaseValidator;

/// Dialog display FPL in the detailed style and allows to edit
class SfplEditor: public AbstractDialog, public Ui::SfplEditor
{
  Q_OBJECT
  public:
    SfplEditor(QWidget* pParent = 0);
    virtual ~SfplEditor();

    /*! Display dialog as modal */
    void setSfpl(const CSFPL &Sfpl);
    void setCanOtherInfoEmpty(bool bCan);
  protected:
    /** Update controls values from database */
    void init();
    /** Save changes (update old plan or save as new) */
    void doSavePlan(void);
    /** Save plan without closing form */
    void doApplyPlan(void);
    /** Copy information from FPL to dialog and over */
    bool sfpl(CSFPL &Sfpl);

    virtual void changeEvent(QEvent *event);
  private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
    void onTextChanged();
//    void on_ACIDEdit_editingFinished();
//    void on_EquipmentEdit_editingFinished();
//    void on_AerodromeDepEdit_editingFinished();
//    void on_SpeedEdit_editingFinished();
//    void on_LevelEdit_editingFinished();
//    void on_RouteEdit_editingFinished();
//    void on_AerodromeDestEdit_editingFinished();
//    void on_ALTNEdit_editingFinished();
//    void on_ALTN2Edit_editingFinished();
    void OnFixedSize();
  private:
    CSFPL mSfpl;
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

#endif // SFPLEDITOR_H
