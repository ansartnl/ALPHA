#ifndef ORCAMMODEL_H
#define ORCAMMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include "front/common/XAppSettings.h"
#include "back/common/AirplaneState.h"
#include "SFPL.h"

class ORCAMModel : public QAbstractTableModel
        , public AppSettings
{
    Q_OBJECT

    struct ModelItem
    {
        QString code;
        QDateTime lastUpdate;
        QDateTime assigned;
    };
    typedef QMap<QString, ModelItem> TItems;
public:
    explicit ORCAMModel(const QString &minCode, const QString &maxCode, QObject *parent = 0);
    explicit ORCAMModel(const QStringList& _lst, int minutes, QObject *parent = 0);
    ~ORCAMModel(){}
    
    void checkTimeout(int minutes);
    void checkTimeOccupied();
    void clear();
    int findCode(const QString& filter, int startWith = 0);
    //@{
    //! Inherited from QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //@}
    void ChangedOrcamRedis(QString _code, QString assigned_time, QString otType);
public slots:
    void airplaneData(const CAirplaneState& data);
    //void onChangedOrcam(QMultiMap<QString, QString> map, QString operType);
protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate();
    virtual void UpdateXFont() {}
    virtual void SI() {}
    //@}
private:
    QVariant GetItemText(const QModelIndex &index) const;
    
    void addToOrcamFile(const QString &code, const QDateTime &assigned_time);
    void loadOrcamFile();
    void saveOrcamFile();
    void codeChanged(const QString &code);
    QString codeFromInt(int value) const;
    void loadOrcamFileFromRedis();

private:
    uint mMinCode;
    uint mMaxCode;
    TItems mItems;
    QSharedPointer<CSFPL> sfpl_;
    QStringList lst;
    int minutes;

};

#endif // ORCAMMODEL_H
