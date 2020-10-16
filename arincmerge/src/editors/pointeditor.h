#ifndef POINTEDITOR_H
#define POINTEDITOR_H

#include "ui_pointeditor.h"
#include "viewdelegate.h"
#include "baseeditor.h"
#include <QWidget>

class QSqlRelationalTableModel;
class QSqlTableModel;
class QSqlQueryModel;
class QSqlDatabase;

//! Class for editing points of zone
class PointEditor : public BaseEditor, public Ui::PointEditor
{
Q_OBJECT

public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param pWidget a parent widget
     */
    PointEditor(QWidget *parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~PointEditor();
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
    void pointsDataChanged();
    void firPointsDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void onOperButtons(QAbstractButton *pButton);
    void addPoint();
    void deletePoint();
    void pointSelected(const QModelIndex &qIndex);
    void addFirPoint();
    void deleteFirPoint();

private:
    void saveFirPoint();
    void updatePointsTableFilter();

    QSqlDatabase mDatabase;
    QMap<QString, int> pointIndexes;
    QMap<QString, int> firPointIndexes;
    QSqlRelationalTableModel *pointsTableModel;
    QSqlRelationalTableModel *firPointsTableModel;
    RelModifyDelegate *pointsDelegate;
    RelModifyDelegate *firPointsDelegate;
    bool isPointsChanged;
    bool isFirPointsChanged;
    QMap < int, int > mFirPointsToDelete;
};

#endif // POINTEDITOR_H
