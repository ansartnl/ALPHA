#ifndef SECTORING_H
#define SECTORING_H

#include <QtCore/qglobal.h>
#include <QtCore/QString>
#include <QTableWidget>
#include <QtSql/QSqlQueryModel>
#include <QItemDelegate>
#include <QSqlQuery>

#include "Dao.h"
#include "ui_Sectoring.h"
#include "core/user/SectoringModelImpl.h"

class QSqlTableModel;
class QSqlRelationalTableModel;
class QSqlQueryModel;
class CheckableSQLQueryModel;



class Sectoring: public QDialog, public Ui::Sectoring
{
    Q_OBJECT
    public:
        Sectoring(QWidget* pPrnt = 0);
        ~Sectoring();

        void setBearingPoints(const QList<CBearingPoint> &points);

    protected:
        void changeEvent(QEvent* event);

    private:
        void loadSettings();
        void saveSettings();
        uint getSectorId();

    private slots:
        void OnAddSGroup();
        void OnRemoveSGroup();
        void OnOperButton(QAbstractButton* pButton);
        void OnSGroupChanged(const QModelIndex& qIndex);
        void OnFirChanged(const QModelIndex& qIndex);//
        void OnZoneChanged(const QModelIndex& qIndex);
        void OnAirportChanged(const QModelIndex& index);
        void OnAirportAllowed(bool allowed);
        void OnAddSector();
        void OnAddFir();
        void OnRemoveSector();
        void OnRemoveFir();
        void OnTypeSelect();
        void OnAddType();
        void OnRemoveType();
        void OnAllow(int iState);
        //void OnMainFIR(int iState);

        void onTabChanged(int index);

        void onFirViewClicked(const QModelIndex& qIndex);
        void onSectorViewClicked(const QModelIndex& qIndex);
        void onBearingChanged(int index);
        //void onBearingChecked(int index);
        void onCheckedItemsChanged(QStringList list);
private:
        void _UpdateSectors(uint uiSectID);
        void _UpdateFirs(uint uiSectID);//
        void _SetMainFIR(uint uiSectID, uint uiZoneID);
        void _SetMainSector(uint uiSectID, uint uiZoneID, uint uiParentZoneID);
        Dao::TSectorZoneList m_SectZoneList;

        QSqlRelationalTableModel* sectorGroupModel;
        QSqlQueryModel* sectorsModel;
        //CheckableSQLQueryModel* FirsModel;
        QSqlQueryModel* FirsModel;
        QSqlTableModel* sectorTypeModel;
        QSqlTableModel* sectorBearingModel;

        QSharedPointer<CSectoringModel> m_pModel;
        int m_iSGTypeIDIndex;
        QHash<quint32, QStringList> mSectorAirports;
        QTableWidget* FirView1;
        typedef QSet<quint32> TBearingIdSet;
        TBearingIdSet result;
};

//! Class based on sectoring
class CSectoring
{
    public:
        CSectoring();
        virtual ~CSectoring();

        //! Set id
        void SetID(quint32 uiID);
        //! Get id
        uint GetID() const;
        //! Set name
        void SetName(const QString& sName);
        //! Get name
        const QString& GetName() const;

    private:
        quint32 m_uiID;
        QString m_sName;
};

//! Class based on sectoring_zone table
class CSectorZone
{
    public:
        //! Constructor
        CSectorZone();
        //! Destructor
        virtual ~CSectorZone();

        //! Set id
        void SetID(uint uiID);
        //! Get id
        uint GetID() const;
        //! Set sectoring id
        void SetSectoringID(uint uiID);
        //! Get sectoring id
        uint GetSectoringID() const;
        //! Set zone id
        void SetZoneID(uint uiID);
        //! Get zone id
        uint GetZoneID() const;
        //! Set main FIR
        void SetMainFIR(bool bMain);
        //! Get main FIR
        bool GetMainFIR() const;

    private:
        uint m_uiID;
        uint m_uiSectoringID;
        uint m_uiZoneID;
        bool m_bMainFIR;

};



#endif // SECTORING_H
