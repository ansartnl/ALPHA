#ifndef LISTACMODEL_H
#define LISTACMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include "ListACData.h"
#include "Dao.h"
#include "front/common/XAppSettings.h"

class QTimer;
class CMaster;
struct GroundStand;

//! Model for AC lists
class CListACModel : public QAbstractTableModel
		, public AppSettings
{
Q_OBJECT
public:
    enum ColType { Eq = 0, ACID, CODE, COPin, SID, COPin_SID, ETOin = 6,
                   ETO_ATOin, ETD_ATD, ETO_ATO_ETD, PEL, PEL_CFL = 11,
                   COPout, STAR, COPout_STAR, ETOout, ETO_ATOout = 16,
                   ETO_ATO_ATA, RFL_TFL, Type, Wct, Stand = 21,
                   REM, RVSM, Type_AC, ADEP, DEST = 26,
                   ALTN, Azimuth, CurrentSpeed,
                   STATUS, LIT, FPLEqf1, FPLEqf2, FPLEqf3, ETD, ETA, FType, MesType, DATE,
                   Scroll = 40,
				   None /*  None always last, not included */ };

    static const int COL_COUNT = 41;

	typedef QVector<CListACData> TListACData;

	//! Constructor
	explicit CListACModel(QObject *parent = 0);
	~CListACModel();
	//! Clear model
	void Clear();
	//! Set data in model
	void SetModelData(const TListACData& pListData);
	//@{
	//! Get model data
	TListACData& GetModelData();
	CListACData& GetItemData(const QModelIndex& index);
	CListACData& GetItemData(int row);
	//@}
	//! Insert row
	void AddRecord(const CListACData& pData);
	//! Delete row
	void DelRecord(const CListACData& pData);
	//@{
	//! Inherited from QAbstractTableModel
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
	void sort(int column, Qt::SortOrder order);
	//@}

    //! Gets custom set colors for warnings and danger background
    void UpdateCustomColors();

    void DeleteRowPlane(int row);

    //! Stands
    void SetStand(const QMap<quint32, QSharedPointer<GroundStand> >& pListData);
    void SetStand(const QSharedPointer<GroundStand>& pData);

    //! Current model index
public:
    const QModelIndex &GetCurrentIndex() const { return m_CurrentIndex; }
    void SetCurrentIndex(const QModelIndex &V) { m_CurrentIndex = V; }    
private:
    QModelIndex m_CurrentIndex;

public:
    void SetMasterPtr(CMaster* pMaster)
    { m_pMaster = pMaster; }

    static void SetEchelon(qint16 echelon) { m_qEchelon = echelon; }
    static qint16 GetEchelon() { return m_qEchelon; }
    static void SetPress(qint16 press) { m_qPress = press; }
    static qint16 GetPress() { return m_qPress; }

    bool isDoubleCode(CListACData& itemData);

signals:
	void timerModel();

protected:
	//@{
	//! Interface CXAppSettings
	virtual void Translate();
	virtual void UpdateXFont() {}
	virtual void SI() {}
	//@}
private slots:
	void OnTimerSlot();
    void OnDelRecord(int index);

private:
    QVariant GetItemText(const QModelIndex &index) const;
    QVariant GetItemEdit(const QModelIndex &index) const;
    QVariant GetItemForeground(const QModelIndex &index) const;
	QVariant GetItemBackground(const QModelIndex &index) const;

    double TransformM_SEC() const;

    /*! Pressure on the airfield
        \param FL a Flight Level in feet
    */
    double PressOnAirdromeFromSee(double FL) const;
    FLVal PressOnAirdromeFromSee(const FLVal& val) const;
    double PressOnAirdromeToSee(double FL) const;
    FLVal PressOnAirdromeToSee(const FLVal& val) const;
    QString GetFLText(const FLVal& val, bool bHundredMeters = false) const;

	void WriteLog(const QSharedPointer<CAirplane>& pPlane, QString sType, QString sValue) const;    

private:
	TListACData				m_listData;
	bool					m_bScroll;
	CListACData				m_emptyTempData;
	
	QTimer*					m_pTimer;
    CMaster*                m_pMaster;

    static qint16           m_qEchelon;
    static qint16           m_qPress;

    static QColor  m_WarningColor;
    static QColor  m_DangerColor;

    static QColor  m_ActColor;
    static QColor  m_AssumeColor;
    static QColor  m_InColor;
    static QColor  m_TransferedColor;

    QMap<QString, QString>  m_mapACID_Stand;
};

//! Sort model for AC lists
class CListACSortModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    enum RowFilterType { TYPE_None = 0, TYPE_SIL, TYPE_SIL_DEP, TYPE_SIL_ARR, TYPE_ACT_IN, TYPE_ACT_OUT, TYPE_ACT_DEP, TYPE_DAILY_PLAN, TYPE_COST };

    //! Column type
    class CTypeCol
    {
    public:
        explicit CTypeCol() : m_ColType(CListACModel::None), m_IsEdit(false) {}
        CTypeCol(CListACModel::ColType type, bool isEdit) : m_ColType(type), m_IsEdit(isEdit) {}

    public:
        const CListACModel::ColType &GetColType() const { return m_ColType; }
        void SetColType(const CListACModel::ColType &V) { m_ColType = V; }
    private:
        CListACModel::ColType m_ColType;

    public:
        const bool &GetIsEdit() const { return m_IsEdit; }
        void SetIsEdit(const bool &V) { m_IsEdit = V; }
    private:
        bool m_IsEdit;
    };

    typedef QVector<CTypeCol>		TListColType;

	//! Constructor
	explicit CListACSortModel(QObject *parent = 0);
	//! Set columns
	void SetColumns(const TListColType& pListCol);
    void SetColEdit(int index, bool bEdit);
    //@{
	//! Inherited from QSortFilterProxyModel
	Qt::ItemFlags flags(const QModelIndex &index) const;
	//@}

	virtual QVariant data(const QModelIndex &index, int role) const;

public slots:
	void setShowAll(bool show_all);
	
public:
    const RowFilterType &GetRowFilterType() const { return m_RowFilterType; }
	void SetRowFilterType(const RowFilterType filter_type);
private:
    RowFilterType m_RowFilterType;

protected:
	//@{
	//! Inherited from QSortFilterProxyModel
	virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
	virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
	//virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
	//@}

private:
	TListColType	m_listCol;
	bool show_all_;

};

#endif // LISTACMODEL_H
