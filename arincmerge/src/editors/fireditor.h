#ifndef FIREDITOR_H
#define FIREDITOR_H

#include "ui_fireditor.h"
#include "viewdelegate.h"
#include "baseeditor.h"
#include <QWidget>

class QSqlRecord;
class QSqlTableModel;
class QSqlRelationalTableModel;
class QSqlDatabase;

//! Class for editing zones
class FirEditor : public BaseEditor, public Ui::FirEditor
{
    Q_OBJECT

public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param pWidget a parent widget
     */
    FirEditor(QWidget *parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~FirEditor();

    //! Interface ILanguage
    virtual void translate();

    virtual void refresh();

protected:
    //! Inherited from QWidget
    virtual void closeEvent(QCloseEvent* pEvent);
    virtual void changeEvent(QEvent* event);

private slots:
    void beforeFIRInsert(QSqlRecord& qRecord);
    void beforeEdgeInsert(QSqlRecord& qRecord);
    void onFirChangedOld(const QModelIndex& ModInd);

    virtual void addFir();
    virtual void deleteFir();
    virtual void addEdgePoint();
    virtual void deleteEdgePoint();
    virtual void onOperButtons(QAbstractButton*);

    void onFirDataChanged();
    void onEdgeDataChanged();

private:
    QSqlDatabase mDatabase;
    QSqlTableModel* firTableModel;
    QSqlRelationalTableModel* edgeTableModel;
    RelModifyDelegate* edgeDelegate;
    bool isFirModifyed;
};

#endif // FIREDITOR_H
