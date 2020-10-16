#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include "Master.h"
#include "GUIView.h"
#include "XAppSettings.h"
#include "XDocWidget.h"
#include "User.h"
#include "RoleModelImpl.h"
#include "RunwayWidgetData.h"

#include <QMainWindow>
#include <QDockWidget>

#include "front/ground/GUIGroundScene.h"
#include "dbconnectionsmanager.h"

class QSlider;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QLabel;
class QSqlTableModel;
class QActionGroup;
class QToolButton;
class QSessionManager;
class DocWidgetMoveHandler;
class QxtSpanSlider;
class ControlWindow;
class PdfWindow;
class RestrictionWidget;
class ActInListTableModel;
class ActInListWidget;
class LayoutDlg;

class PulsReceiver;
class AntioverlapThread;

extern QString currentUserName;


//! Main window of application
class XMasterWindow: public QMainWindow,
    public CMaster,
    public AppSettings,
    public XDocWidget::IDocHandler,
    public CGUIView::IParent,
    public GUIGroundScene::IParent
{
    Q_OBJECT
    public:
        static const int WINDOW_SIZE = 20;
        typedef QHash<QString, QString> TShortcutMap;
        //! Constructor
        /*! \param refTrans a pointer to CXTranslator */
        XMasterWindow(bool registered);
        //! Destructor
        ~XMasterWindow();

        //@{
        //! Interface CGUIView::IParent && GUIGroundScene::IParent
        void ZoomEvent(int iCurScale);
        void CoordinateEvent(const CCoord& Coordinate, const QString& sRoute);
        //@}
        //! Interface CXDocWidget::IDocHandler
        virtual void CloseDoc(XDocWidget::DocType type);

        virtual void addFormular(quint16 id);
        virtual void removeFormular(quint16 id);
        virtual void setFixedFormular(quint16 id, bool fixed);
        virtual qreal formularLinkAngle() const;
        virtual int formularLinkLength() const;
        virtual int formularLinkMinLength() const;
        virtual int formularLinkMaxLength() const;
        virtual bool formularLinkLengthLimited() const;

        //! Initialization function
        bool InitWin(bool lockMouse);

        //! Is shutdown in progress
        bool IsShutdown()
        { return m_bShutdown; }

        virtual void StopExercise(bool bSameDB);
        //! Meteo from simulator
        //@{
        virtual void EndSimMeteo(QString sIdFIR, bool bSameDB);
        //@}

        virtual void OnBearingData(CBearingData& Data);

        void SetCurSectorAirports();
        void _FindOrSetCustomSector();
        QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >& GetDocMap()
        {
            return m_mapDoc;
        }

        QString GetCurrentSector();

        //! Sets FDP status bar text
        void SetTempStatusText(QString text);

        //! Undo sectoring
        void _UndoSectoring();

        //! Forces sectoring
        void ForceSectoring();

        void RequestAllFPL();

        virtual void StandChangedGUI(const GroundStand& stand);

        void SetSIcombo();
        void EditMapNote(quint32 id);
        QStringList GetOrcamRange(){return lst_orcamrange;}

        bool getConnect();

Q_SIGNALS:
        void UpdateMeteo();
        void redisConnected();
        void closeDialog();

    public slots:
        void LoadEnvironment(const QString &sIndex);
        void InitRedis();
        void _LoadSlot();
        void _AddSlot();
        void _OnLogout();
        void _SectoringSlot();
        void _OnViewCWP();
        void _OnCustomSector();
        void _OnUserAction();
        void _OnRefreshSFPL();
        void _OnFontAAsmall();
        void _OnFontAAmedium();
        void _OnFontAAlarge();
        void _OnSaveFocus();
        void _OnRestoreFocus();
        void _OnRestoreFocusCentralPoint();
        void _OnRestoreFormular(bool check);

        void _BackgroundColorSlot();
        void _FormularActiveColorSlot();
        void _FormularPassiveColorSlot();
        void _ControlledColorSlot();
        void _ControlledBackColorSlot();
        void _InColorSlot();
        void _InBackColorSlot();
        void _RelatedColorSlot();
        void _RelatedBackColorSlot();
        void _UnrelatedColorSlot();
        void _UnrelatedBackColorSlot();
        void _ExtendedColorSlot();
        void _MapColorSlot();
        void _FIRColorSlot();
        void _BorderColorSlot();
        void _SectorColorSlot();
        void _LegendColorSlot();
        void _ApproachColorSlot();
        void _BlastoffColorSlot();
        void _RouteColorSlot();
        void _HoldingColorSlot();
        void _CorridorColorSlot();
        void _GridColorSlot();
        void _RestrictionColorSlot();
        void _MapNoteColorSlot();
        void _MeteoColorSlot();
        void _RestrictionUserColorSlot();
        void _RestrictionUserObstacleColorSlot();
        void _RestrictionUserNFZColorSlot();
        void _RestrictionUserMapColorSlot();
        void _ListActColorSlot();
        void _ListAssumeColorSlot();
        void _ListTransferedColorSlot();
        void _ListInColorSlot();
        void _ListSelectedTextColorSlot();
        void _BearingAirportFrequencyColorSlot();
        void _GaugeColorSlot();

        void _DarkCSSlot();
        void _KhakiCSSlot();
        void _EuroControlCSSlot();
        void _LightCSSlot();

        void _OnListACC();
        void _OnListSIL_ACC();
        void _OnListAPP();
        void _OnListSIL_APP_ARR();
        void _OnListSIL_APP_DEP();

        void _OnRestrictionAdmin();

        void _OnActiveSector();
        void _Font();
        void _FMvalues();

        void _FormularEditor();
        void _BearingEditor();
        void _FPLsEditor();
        void _StandEditor();
        void _UserPicsEditor();
        void _GroundRadioRestriction(bool show);
        void _PickCoordinates();
        void _OnPlanListsEditor();

        void _ZoomSlider(int iValue);
        void _FLSlider(int iLowValue, int iHighValue);
        void _OnBtnLayouts();
        void _OnBtnLayoutsMainFIR();
        void _OnBtnAddMapNote();
        void onRemindersClicked(bool checked);
        void onMeteoClicked(bool checked);
        void _SetupMapNote();
        void _OnChangeSI_M(int index);
        void _OnChangeSI_K(int index);
        void _OnCheckGauge(bool checked);
        void _OnCheckAutoCollapse(bool checked);
        void _OnCheckCorridorRoute(bool checked);
        void _OnCheckFlightLevelMonitoring(bool checked);
        void onPointZoomClicked(bool checked);
        void onRoutesClicked(bool checked);
        void onHidePlainsClicked(bool checked);
        void _OnCheckShowSectorBorder(bool checked);
        void _OnCheckShowRouteOnAssume(bool checked);
        void _OnChangeEchelon(int index, bool bFromRedis = false);
        void _OnChangePress(int iValue, bool bFromRedis = false);
        void _OnRcGridCoord();
        void _OnDF();

        void _OnAbout();
        void _OnSectoringCombo(int iIndex);
        void _OnTimeTimer();

        void InitAFTNMessageProcessor();
        void _OnDBConnected(const DbParams& database);
        void _OnDBDisconnected();
        void _OnDBConnectAttemptFailed(const DbParams& params);
        void _OnSetConnectPostgresTrue();
        void _OnSetConnectRedisTrue();

        void _OnHistoryPointChange(bool checked);
        //void onChangedOrcam(QString code, QString assigned_time);
    protected:
        //@{
        //! Interface CXAppSettings
        virtual void changeEvent(QEvent* event);
        virtual void Translate();
        virtual void UpdateXFont()
        {
        }
        virtual void SI()
        {
        }
        //@}
        virtual bool eventFilter(QObject *obj, QEvent *event);
        virtual void closeEvent(QCloseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    protected slots:
        virtual void airplaneLengthChanged(int val);
        virtual void airplaneLengthTypeChanged();
        void speedVectorClicked(int value);
        void distanceVectorClicked(int value);

        void OLDIMesAnswer(const OLDIMessage* pMes, const QStringList& sListError);
        void OLDIMesTimeout(const OLDIMessage* pMes);
        void RecieveOLDIMessage(QSharedPointer<OLDIMessage> pMes);

        QString FLSliderPositionChanged(int value);
        void OnShowToolButMenu();
        void OnToolTipCustomSector();
        void OnDataSourceMenu();
        void OnDataSourceMenuFDP();
        void OnDataSourceMenuRDP();

        void OnPulsarChanged(const QHostAddress& newpulsar, const QString& status, const QHostAddress& oldpulsar, const QVariantHash& info);
        void OnShutdown(QSessionManager& manager);
        void OnSuperStop(const QString& s);

        void _LoadGroundView(const QString& name);
        void _ToggleScrollTrigerred();
        void _onUndoSectoring();

        void onChangedSFPL(QSharedPointer<CSFPL> sfpl, QString operType);
        void onChangedRWF(QString idFIR, RunwayWidgetDataVPP rwf, QString operType);
        void onChangedSectors(SectorRedis sr, QString operType);
        void onChangedRestrictionRedis(const QUuid &uid, const QString &operation);

        void onORCAMDialog();

    private slots:
        void onToolboxHorizontalOrientation();
        void onToolboxVerticalOrientation();

        void onToolboxOldSpeedDistanceMeasure();
        void onToolboxNewSpeedDistanceMeasure();

        void onCheckPoints(bool checked);
        void onCheckBPoints(bool checked);
        void onCheckMeteo(bool checked);

        void onCreateRestriction(const QSharedPointer<CRestrictionUser> &restriction);
        void onUpdateRestriction(const QSharedPointer<CRestrictionUser> &restriction);
        void onDeleteRestriction(const QSharedPointer<CRestrictionUser> &restriction);

        void onLowerPositionChanged(int value);
        void onUpperPositionChanged(int value);

        void onRedisConnected(const QString &ip, bool connected);
        void onChangedConnectionRedis(const QString &ip, bool connected);
        void onChangedConnectionStateRedis(bool connected);

        void onAngleClicked(int value);
        void onAutoAntiOverlapClicked(bool checked);
        void onLabelRepositionRequired(quint16 id, qreal angle, int length);

        void onTriggeredHelpAction();
        void onTriggeredReferenceAction();

        void onApplyLayouts();
        void onUpdateMeteoToMaster();

        void onBearingIdChanged();
        void onChangedOrcam(QString code, QString assigned_time, QString otType);

    private:
        bool RestoreSector();
        bool SetSector(const QString &name);
        QStringList LoadZoneNames(int uiSectoring);
        void _CreateMenu();
        void _CreateToolBar();
        void _CreateStatusBar();
        void _UpdateColorScheme();
        void SceneClean();
        void OpenDoc(XDocWidget::DocType type);
        void _InitSectoringCombo();
        void _InitMainCombo();
        void _InitPressureCombo();
        void _InitPlaneCombo();
        void _InitCustomSector();
        void _InitAltitudeFilter();
        void _InitRestriction();
        void _InitGroundBar();

        void _LoadFIR(uint uiIdFIR);
        void _AddFIR(uint uiIdFIR);

        void _LoadEnvironment(int uiSectoring);
        void _LoadEnvironment(QString privileges, QList<uint> ListIdFIR, const QList<uint> &ListIdSector);
        void _LoadUserEnvironment();

        void loadCurrentUserSettings();
        void saveCurrentUserSettings();

        void _SendSectorsList(const QList<uint> &listIdFIR);
        void OccupySectors();
        QList<uint> _SendPrevSectorList();

        void _SetCustomSector();
        bool _FindSectorID(QList<uint> listUserSectors, QList<uint> listSectors, QList<uint>& listFind);
        void _SelectSectorModel();

        void CleanActiveSector();

        void LogUserLogon(bool bLogin, const QString &sUser);

        int GetEchelonValue(bool bFeet) const;

        void updatePlanList(QString sType);
        void SetEchelonString();

        void loadListPrivileges(const QString& privileges);
        void loadToolbarPrivileges(const QString& privileges);

        int GetEchelonComboIndex(int FL);

        void loadShortcuts();

        void _SetChangedFont(const QFont& font);

        void createRestriction(const QUuid &uid, bool send_to_restriction_widget = false);
        void updateRestriction(const QUuid &uid);
        void deleteRestriction(const QUuid &uid, bool send_to_restriction_widget = false);
        void deleteRestrictions();
        void reloadRestrictions();

        void initAntiOverlapThread();
        void setTransferSectors(QStringList &sectors, SectorRedis &sr);

        QMenu* createHistoryPointMenu();

        void saveApp(int ind);

        void setRegistered(bool registered);

    private:

        /** Menu */
        QMenu* m_AdminMenu;
        QAction* m_UserAction;
        QAction* m_ImportAction;
        QAction* m_SectoringAction;
        QAction* m_BearingAction;
        QAction* m_MapNoteMenuAction;
        /** Files */
        QMenuBar* m_MainMenu;
        //QMenu* m_FileMenu;
        QAction* m_LoadAction;
        QAction* m_AddAction;
        QAction* m_ChUserAction;
        QAction* m_ActRefreshSFPL;
        QAction* m_ExitAction;

        /** Runway widgets */
        QShortcut* m_pRunwayWidgetToggle;
        /** Lists */
        QShortcut* m_ScrollLists;

        /** Option */
        QMenu* mOptionMenu;

        QMenu* mColorMenu;
        QAction* m_BackgroundColorAction;
        
        QMenu* mFormularMenu;
        QAction* m_FormularActiveColorAction;
        QAction* m_FormularPassiveColorAction;
        QAction* m_ControlledAction;
        QAction* m_ControlledBackAction;
        QAction* m_InAction;
        QAction* m_InBackAction;
        QAction* m_RelatedAction;
        QAction* m_RelatedBackAction;
        QAction* m_UnrelatedAction;
        QAction* m_UnrelatedBackAction;
        QAction* m_ExtendedAction;
        
        QAction* m_MapColorAction;
        QAction* m_FIRColorAction;
        QAction* m_ActiveSectorColorAction;
        QAction* m_BorderColorAction;
        QAction* m_LegendColorAction;
        QAction* m_ApproachColorAction;
        QAction* m_BlastoffColorAction;
        QAction* m_RouteColorAction;
        QAction* m_HoldingColorAction;
        QAction* m_CorridorAction;
        QAction* m_GridColorAction;
        QMenu* m_RestrictionColorMenu;
        QAction* m_RestrictionColorAction;
        QAction* m_RestrictionUserColorAction;
        QAction* m_RestrictionUserObstacleColorAction;
        QAction* m_RestrictionUserNFZColorAction;
        QAction* m_RestrictionMapColorAction;
        QAction* m_GaugeColorAction;
        QAction* m_MapNoteColorAction;
        QAction* m_MeteoColorAction;
        QMenu* m_ListColorMenu;
        QAction* m_ListActColorAction;
        QAction* m_ListAssumeColorAction;
        QAction* m_ListInColorAction;
        QAction* m_ListSelectedTextColorAction;
        QAction* m_ListTransferedColorAction;
        QMenu* m_BearingColorMenu;
        //QAction* m_BearingAirportColorAction;
        QMenu* m_ColorSchemeMenu;
        QAction* m_DarkCSAction;
        QAction* m_KhakiCSAction;
        QAction* m_EuroControlCSAction;
        QAction* m_LightCSAction;

        QAction* m_Font;
        /** Tools */
        //QMenu* m_ToolsMenu;
        //QAction* m_FormularEditor;
        QAction* m_RestrictionsEditor;
        QAction* m_BearingEditor;
        QAction* m_FPLsEditor;
        QAction* m_StandEditor;
        QAction* m_UserPicsEditor;
        QAction* m_PickCoords;
        QAction* m_PlanListsEditor;
        QAction* m_FMvalues;
        /** Help */
        QMenu* m_HelpMenu;
        QAction* m_HelpAbout;

        /** Sectoring toolbar */
        QToolBar* m_SectoringBar;
        QComboBox* m_pSectoringCombo;
        QSqlQueryModel* m_SectorTypeModel;
        QSqlTableModel* m_pSectoringModel;
        QLabel* m_pTimeLabel;
        QTimer* m_pTimeTimer;
        QAction* m_pLogOutAction;
        QLabel* m_pUserFullNameLabel;
        QAction* m_pViewCWP;
        QToolButton* m_pFocus;
        QAction* m_pFocusSave;
        QAction* m_pFocusRestore;
        //QAction* m_pFormularRestore;

        QToolBar* m_MainBar;
        /** Zoom */
        QSlider* m_ZoomSlider;
        int m_iSlider;
        /** SI */
        QComboBox* m_SICombo_M;
        QComboBox* m_SICombo_K;
        QCheckBox* m_CheckGauge;
        QToolButton*    m_SettingMenu;
        //QAction* m_AutoCollapse;
        QAction* m_CorridorRoute;
        QAction* m_PointZoom;
        QAction* m_LimitLength;
        QAction* m_FlightLevelMonitoring;
        //QAction* m_ShowSectorBorder;
        //QAction* m_ShowRouteOnAssume;
        QMenu*   m_pFontAAMenu;
        QAction* m_pFontAAsmall;
        QAction* m_pFontAAmedium;
        QAction* m_pFontAAlarge;
        //QAction* m_LayoutsAction;
        QAction* m_LayoutsMainFIRAction;
        QAction* m_MapNoteAction;
        //QAction* m_RcGridCoord;
        //QAction* m_DF;
        QMenu* m_pHistoryPointMenu;
        /** Setting toolbar */
        QToolBar* m_pPressureBar;
        QLabel*     m_pEchelonLabel;
        QToolButton*    m_pEchelonBut;
        QComboBox* m_EchelonCombo;
        QLabel*     m_pPressLabel;
        QToolButton*    m_pPressBut;
        QSpinBox* m_PressSpin;
        /** Prediction toolbar */
        QToolBar *airplaneLengthBar;
        QSpinBox *airplaneLength;
        QActionGroup *airplaneLengthGroup;
        QAction *airplaneLengthSec;
        QAction *airplaneLengthKilometers;
        /** Custom sector toolbar */
        QToolBar* m_pCustomSectorBar;
        QAction* m_pCustomSector;
        /** Altitude filter toolbar */
        QToolBar* m_pAltitudeFilterBar;
        QLabel*     m_pSliderLabel;
        QToolButton*    m_pSliderBut;
        QxtSpanSlider* m_FLSlider;
        /** Restriction toolbar */
        QToolBar* m_pRestrictionBar;
        QAction*  m_pRestrictionAction;

        /** Ground toolbar */
        QToolBar* m_pGroundBar;
        QAction*  m_pGroundStandAction;
        QAction*  m_pGroundRestrictionAction;

        /** Status bar */
        QLabel* m_pStatusLabel;
        QLabel* m_pStatusTempText;
        QToolButton*    m_pDataSource;

        /** Doc widgets */
        QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> > m_mapDoc;
        DocWidgetMoveHandler *docHandler;
        friend class DocWidgetMoveHandler;
        /** Reminders */
        QDockWidget *m_pReminderDoc;

        QSharedPointer<CAirport> mMainAirport;
        QSharedPointer<CUser> mUser;

        QList<const OLDIMessage*>   m_listMes;
        QList<QList<uint> >         m_listPrevSect;

        int     m_SavedZoom;
        QPointF m_SavedPoint;
        bool    m_bFullScreenMode;
        bool    m_bShutdown;
        QString sCurrentSector;
        QString sSectOLDIMes;
        /** Pressure values */
        QMap<QString, int>          m_mapPressure;
        /** Echelon values */
        QMap<QString, int>          m_mapEchelon;

        /** Stands */
        bool    m_bStandTape;
        QMap<quint32, QSharedPointer<GroundStand> > m_mapGUIStand;
        TShortcutMap    m_Shortcuts;

        ControlWindow *control_window_;
        RestrictionWidget *restrictions_widget_;
        QMenu *pref_set_menu_;
        ActInListTableModel *act_in_list_model_;
        ActInListWidget *act_in_list_widget_;
        QMenu *toolbox_orientation_menu_;
        QMenu *toolbox_speed_distance_variants_menu;
        QAction *toolbox_vertical_;
        QAction *toolbox_horizontal_;
        QAction *toolbox_Old_SpeedDistance_measure;
        QAction *toolbox_New_SpeedDistance_measure;

        PulsReceiver *puls_receiver_;

        QLabel *m_pLicenseLabel;
        QLabel *active_db_;
        QLabel *active_redis_;

        QScopedPointer<AntioverlapThread> labels_anti_overlap_thread_;
        int mFormularLinkMinLength;
        int mFormularLinkMaxLength;
        int mFormularLinkPreferredLength;
        int mFormularLinkStep;

        QAction *help_action_;
        bool connected_to_db_;

        QScopedPointer<PdfWindow> help_window_;
        QScopedPointer<PdfWindow> reference_window_;
        QGraphicsScene * scene_fir;
        LayoutDlg* dlgLayout;

        bool mRegistered;
        QString privileges;
        QStringList lst_orcamrange;
        QStringList lst_bearing;
        QStringList lst_frequency;

        bool connectPostgresTrue;
        bool connectRedisTrue;
};

#endif // MASTERWINDOW_H
