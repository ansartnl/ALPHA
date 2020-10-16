#ifndef STAND_WIDGET_H
#define STAND_WIDGET_H

#include <QWidget>
#include "front/common/XAppSettings.h"

class QTableView;
class StandModel;
struct GroundStand;

class StandWidget: public QWidget, public AppSettings
{
    Q_OBJECT
public:
    StandWidget(QWidget* pParent = 0);
    ~StandWidget();

    //! Set data in model
    void SetModelData(const QMap<quint32, QSharedPointer<GroundStand> >& pListData);
    void SetModelData(const QSharedPointer<GroundStand>& pData);

    const QSize GetVisibleSize() const;
    void SizeToContents(bool bSend = true);

signals:
    void sizeChanged();

protected:
    //@{
    //! Interface CXAppSettings
    virtual void Translate() {}
    virtual void UpdateXFont();
    virtual void SI() {}
    //@}

private:
    QTableView* mView;
    StandModel* mModel;
    bool        mSortingEnable;
};

#endif // STAND_WIDGET_H
