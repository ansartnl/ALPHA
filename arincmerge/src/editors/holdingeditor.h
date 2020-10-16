#ifndef HOLDINGEDITOR_H
#define HOLDINGEDITOR_H

#include "ui_holdingeditor.h"
#include "viewdelegate.h"
#include "baseeditor.h"
#include <QWidget>

class QSqlRelationalTableModel;
class QSqlQueryModel;
class QSqlDatabase;
class GeoPoint;
struct Holding;

//! Class for editing holding zones
class HoldingEditor : public BaseEditor, public Ui::HoldingEditor
{
Q_OBJECT
public:
    //! Constructor
    /*!
     \param pDB a pointer to CDB
     \param parent a parent widget
     */
    explicit HoldingEditor(QWidget* parent = 0, const QSqlDatabase &db = QSqlDatabase::database());
    //! Destructor
    virtual ~HoldingEditor();

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
    virtual void addHolding();
    virtual void deleteHolding();
    virtual void onOperButtons(QAbstractButton* pButton);

    void enableOperButtons();

private:
    void verifyHoldingPointsForOldMaster();
    void updateHoldingPointsForOldMaster(const QMap<int, int> &changesIDmap, int idFir);
    void updateHoldingPointsForOldMaster(const QMap<int, int> &changesIDmap, const GeoPoint &centerPoint);
    bool deleteHoldingPointsForOldMaster(int idHolding);
    bool loadHolding(int idHolding, Holding &holding);
    bool getFirCenterPoint(int idFir, GeoPoint &centerPoint);
    bool getPointCoord(int idPoint, GeoPoint &point);

private:
    QSqlDatabase mDatabase;
    QSqlRelationalTableModel *holdingTableModel;
    RelModifyDelegate* holdingDelegate;
    quint32 zoneId;
    QMap<QString, int> holdingIndex;
};

#endif // HOLDINGEDITOR_H
