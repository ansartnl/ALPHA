#ifndef AIRPORTEDITOR_H
#define AIRPORTEDITOR_H

#include "ui_airporteditor.h"
#include "baseeditor.h"
#include "viewdelegate.h"
#include <QWidget>

class QSqlRelationalTableModel;
class QSqlTableModel;
class QSqlDatabase;
class QSqlRecord;
class QSqlQueryModel;

//! Class for editing airports
class AirportEditor : public BaseEditor, public Ui::AirportEditor
{
    Q_OBJECT
public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param pWidget a parent widget
     */
    AirportEditor(QWidget *parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    virtual ~AirportEditor();

    //! Hot change language
    virtual void translate();

    virtual void refresh();

protected:
    void updateFirName();
    void setFirId();

    //! Inherited from QWidget
    void closeEvent(QCloseEvent *pEvent);
    void changeEvent(QEvent *event);

private slots:
    void addAirport();
    void deleteAirport();
    void addFirAirport();
    void deleteFirAirport();
    void onSelectAirport(const QModelIndex& qIndex);

    void onOperButtons(QAbstractButton*);

    void airportDataChanged();
    void firAirportDataChanged();

private:
    void saveFirAirport();
    void saveFir();

    void updateAirportsTableFilter();

    QSqlDatabase mDatabase;
    QSqlTableModel *airportsTableModel;
    QSqlTableModel *airportsByFirTableModel;
    RelModifyDelegate *airportsDelegate;
    RelModifyDelegate *airportsByFirDelegate;
    QMap<QString, int> airportIndexes;
    bool isAirportsChanged;
    bool isAirportsByFirChanged;

    QMap < int, int > mFirAirportsToDelete;
};

#endif // AIRPORTEDITOR_H
