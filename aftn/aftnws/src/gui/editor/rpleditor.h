#ifndef RPLEDITOR_H
#define RPLEDITOR_H

#include "ui_rpleditor.h"
#include "AFTNDao.h"
#include "rpl.h"

#include "abstractdialog.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QDialog>

class QWidget;
class QAbstractButton;
class QSqlTableModel;
class QSqlQueryModel;
class UppercaseValidator;
/// Display RPL in the extended form into dialog. Allows to modify it.
class RplEditor: public AbstractDialog, public Ui::RplEditor
{
  Q_OBJECT
  public:
    RplEditor(QWidget* pParent = 0);
    virtual ~RplEditor();

    void setRpl(const RPL& Rpl); /*!< Load RPL into dialog fields */
  protected:

    /** Update controls values from database */
    void init();

    /** Save changes (update old plan or save as new) */
    void doSavePlan(void);

    /** Save plan without closing form */
    void doApplyPlan(void);

    bool rpl(RPL& Rpl);

  private slots:

    void onTextChanged();

    void on_buttonBox_clicked(QAbstractButton* pButton);
    void on_ACIDEdit_editingFinished();
    void on_EquipmentEdit_editingFinished();
    void on_AerodromeDepEdit_editingFinished();
    void on_SpeedEdit_editingFinished();
    void on_LevelEdit_editingFinished();
    void on_RouteEdit_editingFinished();
    void on_AerodromeDestEdit_editingFinished();
    void on_ALTNEdit_editingFinished();
    void on_ALTN2Edit_editingFinished();

    void on_ValidFromEdit_dateChanged(const QDate& date);
    void OnFixedSize();

  private:

    RPL mRpl;
    QString validValue;
    QString invalidValue;

    QSqlQueryModel* firComboModel;
    QSqlTableModel* aircrafttypesModel;
    QSqlTableModel* flightTypesModel;
    QSqlTableModel* turbulenceModel;
    QSqlTableModel* flightRulesModel;
    QSqlTableModel* levelTypesModel;
    QSqlTableModel* speedTypesModel;

    UppercaseValidator *uppercaseValidator;

    QPushButton* printButton;

  protected:
    virtual void changeEvent(QEvent* event);
};

#endif // RPLEDITOR_H
