#ifndef PLANEDITOR_H
#define PLANEDITOR_H

#include "ui_planeditor.h"
#include "abstractdialog.h"
#include "SFPL.h"
#include "../aftn/aftnws/src/core/fpl.h"
#include <QtCore/QString>
#include <QtSql/QSqlQuery>
#include <QDialog>

class QAbstractButton;
class QSqlTableModel;
class QSqlQueryModel;
class UppercaseValidator;
class ActWidget;
class ActTableModel;
class LineEditDelegate;

class PlanEditor: public AbstractDialog, public Ui::PlanEditor
{
  Q_OBJECT
  public:
    PlanEditor(int cur_index_row, QUuid _uid,QWidget* pParent = 0);
    PlanEditor(int id, QStringList _lst, QWidget* pParent = 0);
    virtual ~PlanEditor();
    void setCanOtherInfoEmpty(bool bCan);
    void SetUid(QUuid & uid);

    void SetNewData();
    void SetNewData(int id);
  protected:
    void init();
    virtual void changeEvent(QEvent *event);
    virtual void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void SetId(int id);
    void InitFpl();
    void FillFields();
  private:
    //void process12StandartEquipment(QString & first, QString & second, QString & third);
    void setActInWidget();
    void setActInWidget2();
    void setInSector(QSharedPointer<CSFPL> &sfpl);
    QStringList getFirPoints();
    QStringList getFir2Points();
    void AdaptateRoute(QString route);
    void setVis(bool visible);
    void InitSize();
    QString FreeCodeOrcam();

  private slots:
    void onTextChanged();
    /*void on_ACIDEdit_editingFinished();
    void on_CODEEdit_editingFinished();
    void on_Equipment2Edit_editingFinished();
    void on_Equipment3Edit_editingFinished();
    void on_AerodromeDepEdit_editingFinished();
    void on_SpeedEdit_editingFinished();
    void on_RFLEdit_editingFinished();
    void on_RouteEdit_editingFinished();
    void on_AerodromeDestEdit_editingFinished();
    void on_ALTNEdit_editingFinished();
    void on_ALTN2Edit_editingFinished();
    void on_OtherEdit_editingFinished();*/
    void OnFixedSize();
    void doSavePlan(void);
    void onPushButtonActClicked();
    void onPushButtonDeleteClicked();
    void onAct();
    void onClose(void);
    void onComboBoxActivated(const QString & text);
    void onPushButtonFirstClicked();
    void onPushButtonPrevClicked();
    void onPushButtonNextClicked();
    void onPushButtonLastClicked();

  private:
    ActWidget * m_act_widget;
    ActTableModel *act_in_model_;
    LineEditDelegate *cop_delegate;
    int current_row;
    QString MesTypeCombo_text;
    //QString FlightTypeCombo_text;
    //QString RulesCombo_text;
    QString AirplaneCombo_text;
    //QString TurbulenceCombo_text;
    QString Equipment1Combo_text;
    QString validValue;
    QString invalidValue;
    UppercaseValidator *uppercaseValidator;
    //QSqlQueryModel* firComboModel;
    QSqlTableModel *aircrafttypesModel;
    QSqlTableModel* flightTypesModel;
    QSqlTableModel* turbulenceModel;
    QSqlTableModel* flightRulesModel;
    QSqlTableModel* levelTypesModel;
    QSqlTableModel* speedTypesModel;
    QSqlTableModel* statusTypesModel;
    ////QPushButton *printButton;*/
    bool canOtherInfoEmpty;
    QSharedPointer<CSFPL> sfpl_;
    QUuid uid;
    int id;
    QSqlQuery query;
    QSqlQueryModel *model_fpl;
    FPL mFpl;
    QString newRoute;
    QStringList lst_orcamrange;

signals:
    void onFirstFPLClicked();
    void onPrevFPLClicked();
    void onNextFPLClicked();
    void onLastFPLClicked();

};

#endif // PLANEDITOR_H
