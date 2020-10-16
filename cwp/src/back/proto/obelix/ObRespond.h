#ifndef OBRESPOND_H
#define OBRESPOND_H

#include <QObject>
#include <QSharedPointer>
#include <Obelix.h>
#include <obelix/CmdReminderDel.h>
#include <obelix/CmdReminderAdd.h>
#include <obelix/CmdTransfer.h>
#include "AirplaneData.h"
#include "OLDIMessage.h"
#include "OLDIMesHOP.h"

class CMaster;
class ObRespond : public QObject
{
    Q_OBJECT
public:
    explicit ObRespond(QObject *parent = 0);
    virtual ~ObRespond()
    {}

    void SetMaster(CMaster* pMaster){m_pMaster = pMaster;}

    //! Sets essential information for functioning
    void setParameters(CAirplaneData::PlaneType type, OLDIMesData lastMes, QString sACID, quint16 id);

    //! Sets operator name (for transport reminders value)
    void setOperator(QString val){sOperator = val;}
signals:
    
public slots:
    void MesAnswer(const OLDIMessage* pMes,const QStringList& err);
    void MesTimeout(const OLDIMessage*);

private:
    obelix::CConnect* m_pConnect;
    CAirplaneData::PlaneType planeType;
    OLDIMesData lastMesData;
    QString ACID;
    quint16 m_ID;
    QString sOperator;
    CMaster* m_pMaster;
};

#endif // OBRESPOND_H
