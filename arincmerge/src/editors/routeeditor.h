#ifndef ROUTEEDITOR_H
#define ROUTEEDITOR_H

#include "ui_routeeditor.h"
#include "viewdelegate.h"
#include "baseeditor.h"
#include <QWidget>

class QSqlRelationalTableModel;
class QSqlTableModel;
class QSqlQueryModel;
class QSqlDatabase;
class QSqlRecord;

//! Class for editing routes of zone
class RouteEditor : public BaseEditor, public Ui::RouteEditor
{
Q_OBJECT
public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param pWidget a parent widget
     */
    RouteEditor(QWidget *parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~RouteEditor();
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
    void addRoute();
    void deleteRoute();
    void BeforeInsertRoute(QSqlRecord& qRecord);
    void OnRouteSelect(const QModelIndex& qIndex);
    void addRoutePoints();
    void deleteRoutePoints();

    void onOperButtons(QAbstractButton*);

    void routeDataChanged();
    void routePointsDataChanged();

private:
    void updatePointsTableFilter();
    void InitRelation();

    QSqlDatabase mDatabase;
    QSqlRelationalTableModel *pointsTableModel;
    QSqlTableModel *routeTableModel;
    QSqlRelationalTableModel *routePointTableModel;
    RelModifyDelegate *pointDelegate;
    RelModifyDelegate *routePointDelegate;
    QMap<QString, int> pointIndexes;
    QMap<QString, int> routeIndexes;
    QMap<QString, int> routePointIndexes;
    uint routeID;
    bool idRouteChanged;
    bool isRoutePointChanged;
};

#endif // ROUTEEDITOR_H
