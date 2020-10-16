#include "Common.h"
#include "back/ground/GroundScene.h"
#include "Master.h"
#include "CmdVehicleState.h"

namespace obelix
{
    const char CCmdVehicleState::XML_CMDNAME[]    = "vehicle_state";
    const char CCmdVehicleState::XML_VEHICLE[]    = "vehicle";
    const char CCmdVehicleState::XML_ID[]        = "id";
    const char CCmdVehicleState::XML_NAME[]    = "name";
    const char CCmdVehicleState::XML_IAS[]    = "ias";
    const char CCmdVehicleState::XML_LONGITUDE[]  = "longitude";
    const char CCmdVehicleState::XML_LATITUDE[]   = "latitude";
    const char CCmdVehicleState::XML_DEL[]          = "delete";
    const char CCmdVehicleState::XML_HIDDEN[] = "hidden";
    const char CCmdVehicleState::XML_LIST[] = "list";

    CCmdVehicleState::CCmdVehicleState(
            )
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DEList = GetDoc().createElement(QL1S(XML_LIST));

        GetMainNode().appendChild(m_DEList);
    }

    bool CCmdVehicleState::IsValue(const QString &sAttrName)
    {
        return !m_DEList.attributeNode(sAttrName).isNull();
    }

    void CCmdVehicleState::SetDel(bool bDelete)
    {
        m_DEList.setAttribute(XML_DEL, (int)bDelete);
    }

    bool CCmdVehicleState::GetDel() const
    {
        bool bDel(false);
        bool bOk(false);
        bDel = m_DEList.attribute(XML_DEL).toInt(&bOk)?true:false;
        TVA_CHECK(
            bOk,
            QString(QL1S(XML_DEL) + QObject::tr(" attribute in XML command has wrong structure")),
            E_FAIL);
        return bDel;
    }

    CCmdVehicleState::CCmdVehicleState(const CXMLData::TXMLData& pData)
        : CXMLCmd(pData)
    {
        m_DEList = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DEList.isNull() && m_DEList.nodeName() == QL1S(XML_LIST),
            QString(QL1S(XML_LIST) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);
    }

    QDomNodeList CCmdVehicleState::GetVehicles()
    {
        return m_DEList.childNodes();
    }

    VehicleState CCmdVehicleState::FormVehicle(QDomNode rem)
    {
        QDomNamedNodeMap attributes = rem.attributes();

        quint32 id = attributes.namedItem(QL1S(XML_ID)).toAttr().value().toInt();
        QString name = attributes.namedItem(QL1S(XML_NAME)).toAttr().value();
        double ias = attributes.namedItem(QL1S(XML_IAS)).toAttr().value().toDouble();
        double latitude = attributes.namedItem(QL1S(XML_LATITUDE)).toAttr().value().toDouble();
        double longitude = attributes.namedItem(QL1S(XML_LONGITUDE)).toAttr().value().toDouble();
        bool hidden = attributes.namedItem(QL1S(XML_HIDDEN)).toAttr().value().toInt();

        VehicleState vehicle;
        vehicle.ID = id;
        vehicle.name = name;
        vehicle.IAS = ias;
        vehicle.coord = CCoord(longitude, latitude);
        vehicle.hidden = hidden;

        return vehicle;
    }

    void CCmdVehicleState::InsertVehicle(quint32 id, const QString &name, double ias, const CCoord& coord, bool isHidden)
    {
        QDomElement DEVehicle = GetDoc().createElement(QL1S(XML_VEHICLE));
        QDomAttr DAID = GetDoc().createAttribute(QL1S(XML_ID));
        DAID.setValue(QString::number(id));
        QDomAttr DAName = GetDoc().createAttribute(QL1S(XML_NAME));
        DAName.setValue(name);
        QDomAttr DAIAS = GetDoc().createAttribute(QL1S(XML_IAS));
        DAIAS.setValue(QString::number(ias));
        QDomAttr DALat = GetDoc().createAttribute(QL1S(XML_LATITUDE));
        DALat.setValue(QString::number(coord.GetLatitude().GetAngle(), 'g', 14 ));
        QDomAttr DALon = GetDoc().createAttribute(QL1S(XML_LONGITUDE));
        DALon.setValue(QString::number(coord.GetLongitude().GetAngle(), 'g', 14 ));
        QDomAttr DAHidden = GetDoc().createAttribute(QL1S(XML_HIDDEN));
        DAHidden.setValue(QString::number((int)isHidden));

        DEVehicle.setAttributeNode(DAID);
        DEVehicle.setAttributeNode(DAName);
        DEVehicle.setAttributeNode(DAIAS);
        DEVehicle.setAttributeNode(DALon);
        DEVehicle.setAttributeNode(DALat);
        DEVehicle.setAttributeNode(DAHidden);

        m_DEList.appendChild(DEVehicle);
    }

    CCmdVehicleStateExe::CCmdVehicleStateExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdVehicleState(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdVehicleStateExe::Execute()
    {
        TVA_CATCHALL_TRY
        {
            if(m_refMaster.GetView())
            {
                QDomNodeList nodes = GetVehicles();
                for(int i = 0; i < nodes.count(); i++)
                {
                    QSharedPointer<GroundScene> scene = m_refMaster.GetView().dynamicCast<GroundScene>();
                    if(scene)
                    {
                        VehicleState vehicle = FormVehicle(nodes.at(i));
                        scene->UpdateVehicle(vehicle);
                        if(IsValue(XML_DEL) && GetDel())
                        {
                            GroundScene::TVehicleMap::iterator itFindVehicle =
                                    scene->getVehicles().find(vehicle.ID);
                            if(itFindVehicle != scene->getVehicles().end())
                            {
                                scene->DelVehicle(*itFindVehicle);
                            }
                        }
                    }
                }
            }

        } TVA_CATCHALL(err)
        {
            LOG_TVAERROR(ERROR, err)
        }
        return CXMLResp::TXMLResp();
    }
}
