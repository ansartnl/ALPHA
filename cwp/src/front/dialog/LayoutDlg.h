#ifndef XLAYOUTDLG_H
#define XLAYOUTDLG_H

#include <QDialog>
#include "ui_LayoutDlg.h"

class IScene;
class CView;
class Scene;

//! Class for working with layouts for FIR
class LayoutDlg: public QDialog, public Ui::LayoutDlg
{
    Q_OBJECT

    public:
        enum LayoutType
        {
            Holding = 1, Restriction = 2, Meteo = 4, Point = 8, Airport = 16, Route = 32,
            Airplane = 64, SSA = 128, Runway = 256, RcGrid = 512, Map = 1024, MapNote = 2048
        };
        enum NodeType
        {
            FIR, FIR_My, Others, All, Layout, LayoutSSA, LayoutSvg, LayoutHolding, LayoutRestriction, LayoutMap,
            LayoutSSA_SID, LayoutSSA_STAR, LayoutSSA_OTHER
        };

        //! Constructor
        /*!
         \param pscene a pointer to CView
         \param pParent a parent widget
         \param f a widget's flags
         */
        LayoutDlg(bool bOnlyMainFIR, const QSharedPointer<IScene>& pScene, QWidget* pParent = 0, Qt::WindowFlags f = 0);
        //! Destructor
        virtual ~LayoutDlg();

        //@{
        //! Layouts for current ID FIR
        static const quint32 GetLayouts(const QString sFir);
        static void SetLayouts(const QString sFir, const quint32 layouts);
        static const bool IsLayoutEnable(const QString sFir, const LayoutDlg::LayoutType layout);
        static const bool IsSSAEnable(const QString sFir, const quint32 AirportID, const quint32 RunwayID,
            const quint32 SSAID);
        static const bool IsSvgEnable(const QString sFir, const quint32 AirportID, const quint32 Svg_id);
        static const bool IsHoldingEnable(const QString sFir, const quint32 HoldingID);
        static const bool IsRestrictionEnable(const QString sKey, const qint32 RestrictionTypeID);
        static const bool IsAreaEnable(const QString& sFir, const QString& areaName);
        //@}
        //! Functions to load
        //@{
        static void Load(QDataStream& in);
        static void Save(QDataStream& out);
        //@}

    signals:
        void applyLayouts();

    protected:
        void changeEvent(QEvent* event);

        void saveSettings();
        void restoreSettings();

        void saveTreeState();
        void restoreTreeState();

        void applyExpandedState(const QSet<QString> &expandedItems,
                                QTreeView* treeView,
                                QAbstractItemModel* model,
                                QModelIndex startIndex);

    private slots:
        void accept();
        void OnItemChanged(QTreeWidgetItem* pItem, int column);

        void onItemExpanded(QTreeWidgetItem* pItem);
        void onItemCollapsed(QTreeWidgetItem* pItem);

        void onItemPressed(QTreeWidgetItem * item);
        void onItemClicked(QTreeWidgetItem * item);
    private:
        void AdvancedSelection(QTreeWidgetItem* pItem, const Qt::CheckState qState);
        void UpdateParent(QTreeWidgetItem* pItem);
        void OthersCheck(const quint32 layoutType, const Qt::CheckState qState);

        QTreeWidgetItem* InsertMyFIRItem(const QSharedPointer<Scene>& pFIR);
        QTreeWidgetItem* InsertFIRItem(const QSharedPointer<Scene>& pFIR, QTreeWidgetItem* parent);

        void InsertFIRSSA(const QSharedPointer<Scene>& pFIR, QTreeWidgetItem* parent);
        void SetFIRSSA(const QSharedPointer<Scene>& pFIR);

        void InsertFIRHolding(const QSharedPointer<Scene>& pFIR, QTreeWidgetItem* parent);
        void SetFIRHolding(const QSharedPointer<Scene>& pFIR);

        void InsertFIRRestriction(const QSharedPointer<Scene>& pFIR, QTreeWidgetItem* parent);
        void SetFIRRestriction(const QSharedPointer<Scene>& pFIR);

        void InsertFIRArea(const QSharedPointer<Scene>& pFIR, QTreeWidgetItem* parent);
        void SetFIRArea(const QSharedPointer<Scene>& pFIR);

        void SaveFIRLayouts(const QTreeWidgetItem* pFIRItem);

        typedef QPair<QPair<quint32, quint32> , quint32> TSSAKey;
        typedef QMap<TSSAKey, bool> TSSAMap;
        //typedef QPair<quint32, quint32> TSvgKey;
        //typedef QMap<TSvgKey, bool> TSvgMap;
        typedef QMap<QString, TSSAMap> TFIRMap;
        typedef QMap<quint32, bool> THoldingMap;
        typedef QMap<QString, THoldingMap> TFIRHoldMap;
        typedef QMap<qint32, bool> TRestrictionMap;
        typedef QMap<QString, TRestrictionMap> TFIRRestrictionMap;
        typedef QMap<QString, quint32> TFIRLayout;
        typedef QMap<QString, bool> TMapArea;
        typedef QMap<QString, TMapArea> TFIRAreas;

        static TFIRLayout   m_mapLayouts;
        static TFIRMap      m_mapSSA;
        static TFIRHoldMap  m_mapHolding;
        static TFIRRestrictionMap m_mapRestriction;
        static TFIRAreas     m_mapAreas;

        bool m_bChecking;
        QSharedPointer<CView> m_pView;

        QSet<QString> mExpandedItems;
        int mItemState;
};

#endif // XLAYOUTDLG_H
