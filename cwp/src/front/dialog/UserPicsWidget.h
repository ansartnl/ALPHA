#ifndef USERPICS_WIDGET_H
#define USERPICS_WIDGET_H

#include <QWidget>
#include <QMap>
#include "front/common/XAppSettings.h"

#include <QtSql/QSqlRelationalTableModel>

class UserPicsModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit UserPicsModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    void setCheckedColumn(int column);
    int getCheckedColumn() const;

    //@{
    //! Inherited from QSqlRelationalTableModel
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &item, const QVariant &value, int role = Qt::EditRole);
    virtual void clear();
    virtual bool select();
    //@}

signals:
    void checkChanged(const QModelIndex& index, bool bCheck);

private:
    int mCheckedColumn;
    QMap<QPair<int, int>, bool> mCheckedData;
};


namespace Ui {
    class UserPicsWidget;
}

class UserPicsWidget: public QWidget, public AppSettings
{
    Q_OBJECT

public:
    UserPicsWidget(QWidget* pParent = 0);
    ~UserPicsWidget();

    //! Set data in model
    void SetModelData(const QString& curUser, int zoneID);

    const QSize GetVisibleSize() const;
    void SizeToContents(bool bSend = true);

    //@{
    //! User pics visibility for current ID FIR
    static bool IsUserPicstEnable(const QString& sUser, int idFir, int idUserPics);
    //@}
    //! Functions to load
    //@{
    static void Load(QDataStream& in);
    static void Save(QDataStream& out);
    //@}

public slots:
    void onPickCoord();

signals:
    void sizeChanged();
    void getPickCoord(bool bMulti);
    void userPicsSelectionDrop();
    void userPicsChanged();
    void userPicsSelectChanged(int, int, bool);
    void userPicsVisabilityChanged();

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont();
    virtual void SI() {}
    bool eventFilter ( QObject * watched, QEvent * event );
    //@}

private slots:
    void onUserPicsChanged(const QModelIndex& index);
    void onUserPicsCheckChanged(const QModelIndex& index, bool bCheck);
    void onUserPointPressRight();
    void onUserPointReleaseRight();
    void onButAdd();
    void onButEdit();
    void onButDelete();
    void onButAddPoint();
    void onButAddPointPick();
    void onButDeletePoint();
    void onButSave();
    void onButCancel();

    void onStretch();
    void onSizeToContents();
    void onDelayStretch();
    void onDelaySizeToContents();
    void onUserPointDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight );
    void onDelayCheckChanged();
    void onDelaySaveSlot();
    void onPasteCoords();
    void onTablePointsCurrentRowChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    bool selectAll(QSqlTableModel* model);
    int buttonsWidthPics() const;
    int buttonsWidthPoints() const;
    void setVisability();
    int GetCodeTypeByName(QString & name);
    void DifferentSN();
    void InitRelation();

private:
    Ui::UserPicsWidget* ui;
    QString mUser;
    int mZoneID;
    int mUserPicID;
    int mUserPicsLastRow;
    UserPicsModel* mUserPicsModel;
    QSqlRelationalTableModel* mUserPicsPointModel;
    QMap<QString, int> mUserPicsModelIndex;
    QMap<QString, int> mUserPicsPointModelIndex;
    QMap<int, int> m_selected_rows;
    bool mCheckSetting;
    bool mDelaySizeToContents;
    bool mDelayStretch;
    bool mHideArcColumns;
    bool mDelayCheck;
    bool m_was_selection;
    bool onDelaySave;
    int m_need_select_row_after_add;
    int m_last_left_column;
    int m_last_edit_row;
    typedef QPair<QString , int> TFIRKey;
    typedef QMap<int, bool> TUPVMap;
    typedef QMap<TFIRKey, TUPVMap> TUPVMapFIRs;

    static TUPVMapFIRs mMapUserPicsVisibility;
};

#endif // USERPICS_WIDGET_H
