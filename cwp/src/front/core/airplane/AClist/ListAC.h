#ifndef LISTAC_H
#define LISTAC_H

#include <QTableView>
#include "Dao.h"
#include "ListACModel.h"
#include "ListACView.h"
#include <QDebug>

class QHBoxLayout;
class CAirplane;
class CMaster;
class OLDIMessage;
template <typename T> class QFutureWatcher;


//! Base class for airplane's lists
class CListAC: public QWidget
{
    Q_OBJECT
    public:
        typedef QList<CListAC*> TListView;

        typedef QList<int>  TIndexList;
        //! Constructor
        /*!
         \param pBase a pointer to Dao
         \param pMaster a pointer to CMaster
         \param pParent a parent widget
         */
        CListAC(CMaster* pMaster = 0, QWidget* pParent = 0);
        virtual ~CListAC();

        //! Insert new record for plane
        static void AddPlaneRecord(const QSharedPointer<CAirplane>& pAirplane);
        //! Modify record for plane
        static void ModPlaneRecord(const QSharedPointer<CAirplane>& pAirplane);
        //! Delete record for plane
        static void DelPlaneRecord(const QSharedPointer<CAirplane>& pAirplane);
        //! Select record for plane
        static void SelPlaneRecord(const QSharedPointer<CAirplane>& pAirplane);
        //! Recieve OLDI message
        static bool RecieveOLDIMessage(QSharedPointer<OLDIMessage>& pMes);
        //! Toggle List Scroll action trigerred
        static void toggleScroll();

        //! Refresh all data and select record
        static void RefreshAndSelect(quint32 idSFPL);
        static QDateTime lastTimeRefresh();
        static void StopFutureRefresh();
        static bool isFutureRefreshRunning();
        //! Size of table
        virtual const QSize GetVisibleSize() const;
        //! Resize list and send signal sizeChanged
        void SizeToContents(bool bSend = true);
        void SetMinimumWidthForTable(quint32 v_size)
        {
            m_qMinimumTableWidth = v_size;
            //qDebug()<<"SET MIN TABLE WIDTH="<<m_qMinimumTableWidth;
        }
        void SetMaxColumnsWidh(quint32 v_size)
        {
            m_qMaxCellWidth = v_size;
        }

protected:
        //! Update all data in model
        void Refresh(quint32 qSFPLID = 0, bool bProcessEvents = true);
        static TListView m_listView;
        static CListACModel m_model;
public:

        static CListACModel& GetSourceModel()
        { return m_model; }
        //! Find SFPL by SFPL key
        static CListACData& GetDataBySFPLkey(QString sACID, QString sADEP, QString sADES);

        static QSharedPointer<CSFPL> GetSFPLByCode(quint32 code);

        static QList<int> GetMode3AReplyList();

        void UpdateViewData();
        void fillSortModel();
        void setListType(QString sVal){
            sListType = sVal;
            m_list.setPlanType(sVal);
        }

        static QStringList getFirPoints();
        static QStringList getFir2Points();
        static CCoord getCoordsByPointName(const QString &name);
        static QString getPointNameByCoords(const CCoord &coord);

    signals:
        void sizeChanged();

    protected:
        //! Find row for plane
        /*! ( -1 not found ) */
        static int FindRow(const QSharedPointer<CAirplane>& pAirplane);
        //! Find row for SFPL
        /*! ( -1 not found ) */
        static int FindRow(const QSharedPointer<CSFPL>& pSFPL);
        //! Select row
        void SelectRow(const CListACData& pItemData);
        //! Write in log
        static void WriteLog(const QSharedPointer<CAirplane>& pAirplane, QString sType, QString sValue);
        //! Set REM/WRN RVSM text
        static void SetREM(CListACData& itemData);
        //! check REM double code
        static bool isDoubleCode(CListACData& itemData);
        //! Calculate InSector by COPin
        QString CalcInSector(QSharedPointer<CSFPL> pSFPL, bool bPodpora = true);
        //! Calculate InSector by airport
        QString GetAirportSector(const QString& sAirport, const QStringList& sSectors);
        //! Edit ACID
        void ItemEditACID(QSharedPointer<CSFPL> pSFPL);

        static int m_modelUsage;
        static bool mChanging;
        static QDateTime mLastTimeRefresh;

        CListACView m_list;
        CListACSortModel m_sortModel;

        QHBoxLayout *m_ControlsLayout;
        QHBoxLayout *m_ControlsLayout_add;

        CMaster* m_pMaster;

public slots:
        void OnShowRoute();
        void OnShowRouteCost(QSharedPointer<CSFPL> pSFPL);

    private slots:
        void OnSelectionChanged(const QItemSelection& current, const QItemSelection& previous);
        void OnEnteredItem(const QModelIndex& index);
        void OnTimerSlot();
        void OnContextMenu(const QPoint& pos);
        void OnTransfer();
        //! cancel activation
        void OnCancelAct();
        //void OnShowRoute();
        void OnEnterLeaveList(bool bEnter);
        void OnKeyPressed(const QModelIndex& index, QKeyEvent* event);
        void OnScrollItem(int index);

        void OnFutureRefresh(quint32 qSFPLID, bool bProcessEvents);
        void OnFutureRefreshFutwatchResult(int index);
        void OnFutureRefreshFutwatchFinish();

    private:
        virtual int InsertRow(const QSharedPointer<CAirplane>& pAirplane) = 0;
        virtual void ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane) = 0;
        virtual bool IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& pAirplane) = 0;
        virtual void TimerSlot() {}

        static int _InsertRow(const QSharedPointer<CAirplane>& pAirplane);
        static void _ModifyRow(int row, const QSharedPointer<CAirplane>& pAirplane);
        static bool _IsCanDeleteRow(int row, const QSharedPointer<CAirplane>& pAirplane);

        void SelectIndex(const QModelIndex& modelIndex);
        static void SelectCurrentIndex(const QModelIndex& modelIndex);
        void MigrationFrag(QSharedPointer<CFPLFrag>& pFragSFPL, const QSharedPointer<CFPLFrag>& pFragPlane);
        bool _RecieveOLDIMessage(QSharedPointer<OLDIMessage>& pMes);
        bool CalcTableColumnsWidth(QMap<quint32,quint32> &mas_colum); // return true if we calculate new width for colums and need resize, false if we didnt need resize

        int GetColumnIndex(const QString &column_name);
        void _toggleScroll();

        static CListACData CalcFutureRefresh(const CListACData& acData);
        void debugOnFutureRefreshFutwatchResult(int index, CListACData itemData);
        void debugOnFutureRefreshFutwatchFinish();

        QAction*            m_pActionShowRoute;
        QAction*            m_pActionTransfer;
        QAction*            m_pActionCancelACT;
        QWeakPointer<CSFPL> m_pActionSFPL;

        QString             sListType;
        TIndexList          m_ScrolledItems;

        static QFutureWatcher<CListACData>* m_FutureRefreshFutwatch;
        static volatile bool                m_FutureRefreshForceStop;
        static CListACModel::TListACData*   m_pNewListData;
        static quint32                      m_qSFPLID;
        static quint32                      m_qSFPLIDwhenRunning;
        static volatile bool                m_bFutureRefreshInProcess;

        quint32 m_qMinimumTableWidth;
        quint32 m_qMaxCellWidth;

        static QSet<CListAC*> liveObjects;
        QTimer spi_timer_;
};

#endif // LISTAC_H
