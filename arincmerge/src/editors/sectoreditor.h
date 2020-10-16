#ifndef SECTOREDITOR_H
#define SECTOREDITOR_H

#include "ui_sectoreditor.h"
#include "viewdelegate.h"
#include "baseeditor.h"
#include <QWidget>

class QSqlRecord;
class QSqlTableModel;
class QSqlRelationalTableModel;
class QSqlDatabase;

//! Class for editing zones
class SectorEditor : public BaseEditor, public Ui::SectorEditor
{
    Q_OBJECT

public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param pWidget a parent widget
     */
    SectorEditor(QWidget *parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~SectorEditor();

    //! Interface ILanguage
    virtual void translate();

    virtual void refresh();

protected:
    void updateFirName();
    void setFirId();

    //! Inherited from QWidget
    void closeEvent(QCloseEvent *pEvent);
    void changeEvent(QEvent *event);

private slots:
    void beforeEdgeInsert(QSqlRecord &qRecord);
    void onSectorChanged(const QModelIndex &modInd);
    void onCurrentEdgeChanged(const QModelIndex &curInd);

    virtual void addSector();
    virtual void deleteSector();
    virtual void addEdgePoint();
    virtual void deleteEdgePoint();
    virtual void onOperButtons(QAbstractButton*);

    void onSectorDataChanged();
    void onEdgeDataChanged();

private:
    QSqlDatabase mDatabase;
    QSqlTableModel *sectorTableModel;
    QSqlRelationalTableModel *edgeTableModel;
    RelModifyDelegate *edgeDelegate;
    bool isSectorModifyed;
};

#endif // SECTOREDITOR_H
