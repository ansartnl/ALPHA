#ifndef SSAEDITOR_H
#define SSAEDITOR_H

#include "ui_ssaeditor.h"
#include "baseeditor.h"
#include "viewdelegate.h"
#include <QWidget>

class QSqlRelationalTableModel;
class QSqlTableModel;
class QSqlRecord;
class QSqlQueryModel;
class QSqlDatabase;

//! Class for editing SSA
class SsaEditor : public BaseEditor, public Ui::SsaEditor
{
Q_OBJECT
public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param pWidget a parent widget
     */
    SsaEditor(QWidget* parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~SsaEditor();

    //! Interface ILanguage
    virtual void translate();

    virtual void refresh();

protected:
    void updateFirName();
    void setFirId();

    void updateSsaFilter();

    //! Inherited from QWidget
    void closeEvent(QCloseEvent *pEvent);
    void changeEvent(QEvent *event);

private slots:
    void airportChanged(int iIndex);
    void schemeChanged(int iIndex);
    void addRunway();
    void deleteRunway();
    void OnRunBeforeInsert(QSqlRecord& qRecord);
    void OnRunSelect(const QModelIndex& qIndex);
    void addSsa();
    void deleteSsa();
    void OnSSABeforeInsert(QSqlRecord& qRecord);
    void OnSSASelect(const QModelIndex& qIndex);
    void addSsaPoint();
    void deleteSsaPoint();
    void OnSSAPointBeforeInsert(QSqlRecord& qRecord);
    void onOperButtons(QAbstractButton*);

    void OnRunDataChanged();
    void OnSSADataChanged();
    void OnSSAPointDataChanged();

    void OnChangedRunwaySSA(int newRunwayID, int oldRunwayID, QList<int> listSSA);

private:
    QSqlDatabase mDatabase;
    QSqlTableModel *runwayTableModel;
    QSqlRelationalTableModel *ssaTableModel;
    QSqlRelationalTableModel *ssaPointTableModel;
    QSqlQueryModel *airportComboModel;
    QSqlQueryModel *schemeComboModel;
    RelModifyDelegate *runwayDelegate;
    RelModifyDelegate *ssaDelegate;
    RelModifyDelegate *ssaPointDelegate;
    QMap<QString, int> runwayIndexes;
    QMap<QString, int> ssaIndexes;
    QMap<QString, int> ssaPointIndexes;
    uint runwayId;
    uint ssaId;
    bool isRunwayChanged;
    bool isSsaChanged;
    bool isSsaPointChanged;

    int scheme;
    int IDturn;
    int IDpoint;
    int IDtype;
};

Q_DECLARE_METATYPE(QList<int>)

#endif // SSAEDITOR_H
