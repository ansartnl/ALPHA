#include "XtProtocol.h"
#include "XtGlobal.h"

#include <QtCore/QDebug>


XT_BEGIN_NAMESPACE

//! Protocol identifier - same for each message
static const quint32 g_protoID = 0x01FB004C;

//! Protocol version
static const quint32 g_protoVersion = 0x00010005;


int CProtocol::Version()
{
    return g_protoVersion;
}

QByteArray CProtocol::Serialize(const TMessage &message)
{
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);

    out << g_protoID << g_protoVersion;
    // Reserve integer value for total size of the data block we are serializing
    // (for verification purpose).
    out << (quint32)0;
    message->Serialize(out);
    // Seek back to the beginning of the QByteArray, and overwrite the reserved
    // 32 bit integer value with the total size of the array.
    out.device()->seek(2*sizeof(quint32));
    out << (quint32)(buffer.size() - 2*sizeof(quint32));

    return buffer;
}

TMessage CProtocol::Deserialize(const QByteArray& buffer, quint32 *bytesRead)
{
    if ( buffer.isEmpty() )
        return TMessage(new CMessage);

    QDataStream in(buffer);

    quint32 protoID, protoVersion;
    in >> protoID >> protoVersion;

    // If procol identifier does not meet our fixed ID - quit with the null message.
    // Pprocol version of sender should correspond to current compiled protocol version
    // if not - quit with the null message. This can happen when old or new client library
    // sent message with changed structure.
    if ( protoID != g_protoID || g_protoVersion != protoVersion )
        return TMessage(new CMessage);

    quint32 bufferSize;
    in >> bufferSize;
    bufferSize += 2*sizeof(quint32);

    // Verify data block size that was serialized with the one that is deserializing.
//    qDebug() << "CProtocol::Deserialize Buffer size is " << buffer.size();
    if ( bufferSize > (quint32)buffer.size() )
        return TMessage(new CMessage);

    if ( bytesRead )
        *bytesRead = bufferSize;

    CMessage message;
    message.Deserialize(in);

    // Parse data messages
    if ( message.GetType() == CDataMessage::GetClassType() )
    {
        CDataMessage messageData;
        messageData.Deserialize(in, true);

        TObject obj;
        TMessage msgData(new CDataMessage);

        if ( messageData.GetDataType() == CAsterix62::GetClassType() )
            obj = TObject(new CAsterix62);
        else if ( messageData.GetDataType() == Plot::GetClassType() )
            obj = TObject(new Plot);
        else if ( messageData.GetDataType() == Track::GetClassType() )
            obj = TObject(new Track);
        else if ( messageData.GetDataType() == AirplaneInfo::GetClassType() )
            obj = TObject(new AirplaneInfo);
        else if ( messageData.GetDataType() == MeteoVector::GetClassType() )
            obj = TObject(new MeteoVector);
        else if ( messageData.GetDataType() == MeteoContour::GetClassType() )
            obj = TObject(new MeteoContour);
        else if ( messageData.GetDataType() == Radar::GetClassType() )
            obj = TObject(new Radar);
        else if ( messageData.GetDataType() == Peleng::GetClassType() )
            obj = TObject(new Peleng);
        else if ( messageData.GetDataType() == Stand::GetClassType() )
            obj = TObject(new Stand);
        else if ( messageData.GetDataType() == SimCmd::GetClassType() )
            obj = TObject(new SimCmd);
        else if ( messageData.GetDataType() == Vehicle::GetClassType() )
            obj = TObject(new Vehicle);

        // Unsupported data message.
        if ( !obj )
            return TMessage(new CMessage);

        obj->Deserialize(in);

        static_cast<CDataMessage *>(msgData.data())->SetObject(obj);

        return msgData;
    }
    // Parse subscribe messages
    else if ( message.GetType() == CSubscribeMessage::GetClassType() )
    {
        CSubscribeMessage messageSubs;
        messageSubs.Deserialize(in, true);

        if ( messageSubs.GetSubsType() == CMSubscribeType::GetClassType() )
        {
            TMessage msgSubs(new CMSubscribeType);
            msgSubs->Deserialize(in, true);
            return msgSubs;
        }
    }
    // Parse request messages
    else if ( message.GetType() == CRequestMessage::GetClassType() )
    {
        CRequestMessage messageRequ;
        messageRequ.Deserialize(in, true);

        if ( messageRequ.GetRequType() == CMRequestAll::GetClassType() )
            return TMessage(new CMRequestAll);

        else if ( messageRequ.GetRequType() == CMRequestType::GetClassType() )
        {
            TMessage msgRequ(new CMRequestType);
            msgRequ->Deserialize(in, true);
            return msgRequ;
        }

        else if ( messageRequ.GetRequType() == CMRequestMasterServer::GetClassType() )
            return TMessage(new CMRequestMasterServer);
    }

    // Message was not successfully parsed.
    return TMessage(new CMessage);
}

XT_END_NAMESPACE
