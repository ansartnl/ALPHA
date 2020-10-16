#include "CmdGUIRefresh.h"
#include "Common.h"
#include "Master.h"
#include "OLDIMessages.h"

#include <QMessageBox>

namespace obelix
{
    const char CCmdGUIRefresh::XML_CMDNAME[]    = "gui_refresh";
    const char CCmdGUIRefresh::XML_REFRESH[] = "guirefresh";
    const char CCmdGUIRefresh::XML_URZ_ID[]    = "urz_ID";
    const char CCmdGUIRefresh::XML_FIR_NAME[] = "sFirName";
    const char CCmdGUIRefresh::XML_TYPE[] = "MessageType";
    const char CCmdGUIRefresh::XML_URZTYPE[] = "urzDBType";

    CCmdGUIRefresh::CCmdGUIRefresh()
        : CXMLCmd(QL1S(XML_PROTO))
    {
        SetCmdName(QL1S(XML_CMDNAME));
        MarkACK(false);

        m_DERefresh = GetDoc().createElement(QL1S(XML_REFRESH));
        GetMainNode().appendChild(m_DERefresh);
    }

    QString CCmdGUIRefresh::GetValue(const QString &sAttrName)
    {
        return m_DERefresh.attribute(sAttrName);
    }

    bool CCmdGUIRefresh::IsValue(const QString &sAttrName)
    {
        return m_DERefresh.hasAttribute(sAttrName);
    }

    CCmdGUIRefresh::CCmdGUIRefresh(const CXMLData::TXMLData &pData)
        : CXMLCmd(pData)
    {
        m_DERefresh = GetMainNode().firstChildElement();
        TVA_CHECK(
            !m_DERefresh.isNull() && m_DERefresh.nodeName() == QL1S(XML_REFRESH),
            QString(QL1S(XML_REFRESH) + QObject::tr(" node in XML command is absent or has wrong name")),
            E_FAIL);
    }

    CCmdGUIRefreshExe::CCmdGUIRefreshExe(
        const CXMLData::TXMLData& pData,
        CMaster& refMaster)
        : CCmdGUIRefresh(pData), m_refMaster(refMaster)
    {

    }

    CXMLResp::TXMLResp CCmdGUIRefreshExe::Execute()
    {
        QString sMessageType = GetValue(QL1S(XML_TYPE));
        CGUIView* pView = dynamic_cast<CGUIView*> (m_refMaster.GetView().data());
        XMasterWindow* pMain = dynamic_cast<XMasterWindow*>(getMainWindow());

        if(!pView)
            return CXMLResp::TXMLResp();

        TVA_CATCHALL_TRY
        {
            if(sMessageType == "STS")
            {
                OLDIClients::TClientMap mapCWP;
                TVA_CATCHALL_TRY
                {
                    dao()->LoadCWPSectors
                            (mapCWP, "client = '" +
                            networkSettings()->value("oldi/name", "Unknown").toString() + "'");
                } TVA_CATCHALL(err)
                {
                    QMessageBox::critical(NULL, getMainWindow()->windowTitle(), err.reason());
                }
                CView::TSceneList::iterator itFIR = pView->GetScenes().begin();
                for(; itFIR != pView->GetScenes().end(); ++itFIR)
                {//looking for active(selected) sector
                    Scene::TSceneList& pSectorList = (*itFIR)->GetSectorList();
                    Scene::TSceneList::iterator itSector = pSectorList.begin();
                    for(; itSector != pSectorList.end(); ++itSector)
                    {
                        bool bActive = false;
                        QList<OLDIClients> listSector = 1 == mapCWP.size() ? mapCWP.begin().value() : QList<OLDIClients>();
                        for(int i = 0; i < listSector.size(); ++i)
                        {
                            if(listSector.at(i).GetSector() == (*itSector)->GetShortName())
                            {
                                bActive = true;
                                break;
                            }
                        }
                        (*itSector)->MarkItemActive(bActive);
                    }
                }

                //store current sector in config
                QString sSectors;
                networkSettings()->setValue("users/current_sector", "");
                QList<OLDIClients> listSector = 1 == mapCWP.size() ? mapCWP.begin().value() : QList<OLDIClients>();
                for(int i = 0; i < listSector.size(); ++i)
                {
                    if(1 == listSector.at(i).GetStatus())
                        networkSettings()->setValue("users/current_sector", listSector.at(i).GetSector());
                    if(sSectors.size()) sSectors += "-";
                    sSectors += listSector.at(i).GetSector();
                }
                networkSettings()->setValue("users/current_sectors_short_name", sSectors);

                pMain->_FindOrSetCustomSector();

                if(!mapCWP.size())
                {// if no active sector - make all inactive
                    if(pView && pView->GetMainFIR())
                    {
                        CView::TSceneList::iterator itFIR = pView->GetScenes().begin();
                        for(; itFIR != pView->GetScenes().end(); ++itFIR)
                        {
                            Scene::TSceneList::iterator itSector = (*itFIR)->GetSectorList().begin();
                            for(; itSector != (*itFIR)->GetSectorList().end(); ++itSector)
                                (*itSector)->MarkItemActive(false);
                        }
                    }
                }
                pMain->SetCurSectorAirports(); //stores sector airports in XML
                pView->Draw();
                pView->Redraw();
            }
            else if(sMessageType == "URZ")
            {
                OLDIMesURZ::Type urzDBType(OLDIMesURZ::Type(GetValue(QL1S(XML_URZTYPE)).data()->toLatin1()));
                //update on view
                if(OLDIMesURZ::enInsert == urzDBType || OLDIMesURZ::enUpdate == urzDBType || OLDIMesURZ::enDelete == urzDBType)
                {
                    if(pView)
                    {   //this piece can go to client side
                        quint32 urzID = GetValue(QL1S(XML_URZ_ID)).toUInt();
                        CView::TSceneList::iterator itFIR = pView->GetScenes().find(GetValue(QL1S(XML_FIR_NAME)));
                        if(itFIR != pView->GetScenes().end())
                        {
                            // First - remove restriction zone from scene and cache (if it is there).
                            QSharedPointer<CRestrictionUser> restrictionZone = (*itFIR)->DelUserRestriction(urzID);
                            QGraphicsItem *guiItem = dynamic_cast<QGraphicsItem *>( restrictionZone.data() );
                            if ( guiItem )
                                pView->scene()->removeItem(guiItem);

                            // Add modified(created) restriction zone to scene.
                            if(OLDIMesURZ::enInsert == urzDBType || OLDIMesURZ::enUpdate == urzDBType)
                            {
                                dao()->LoadUserRestriction(*itFIR->data(), urzID);

                                for ( Scene::TRestrictionUserList::iterator itr = (*itFIR)->GetRestrictionUser().begin();
                                      itr != (*itFIR)->GetRestrictionUser().end(); ++itr )
                                {
                                    QSharedPointer<CRestrictionUser> &r = *itr;
                                    if ( r->GetID() == urzID )
                                    {
                                        guiItem = dynamic_cast<QGraphicsItem *>( r.data() );
                                        if ( guiItem )
                                        {
                                            pView->scene()->addItem( guiItem );
                                            IGUIItemHandler* guiHandler = dynamic_cast<IGUIItemHandler *>( r.data() );
                                            if(guiHandler)
                                            {
                                                qint64 qllXMin, qllYMax;
                                                double dScale;
                                                CCoord centralCoord;
                                                pView->GetCoordSystem(qllXMin, qllYMax, dScale, centralCoord);
                                                guiHandler->SetPosition(qllXMin, qllYMax, dScale, centralCoord);
                                            }
                                        }
                                        break;
                                    }
                                }
                            }

                            QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = pMain->GetDocMap().find(XDocWidget::RestrictionsUserEditor);
                            if (itDoc != pMain->GetDocMap().end()) {
                                if (QSharedPointer<XDocWidget> widget = qSharedPointerDynamicCast<XDocWidget>(*itDoc))
                                    widget->Refresh();
                            }
                            itDoc = pMain->GetDocMap().find(XDocWidget::RestrictionsAdminEditor);
                            if (itDoc != pMain->GetDocMap().end()) {
                                if (QSharedPointer<XDocWidget> widget = qSharedPointerDynamicCast<XDocWidget>(*itDoc))
                                    widget->Refresh();
                            }
                        }
                    }
                }
                else if(OLDIMesURZ::enMeteo == urzDBType)
                {
                    QMap<XDocWidget::DocType, QSharedPointer<QDockWidget> >::iterator itDoc = pMain->GetDocMap().find(XDocWidget::MeteoRunwayInfo);
                    if (itDoc != pMain->GetDocMap().end()) {
                        if (QSharedPointer<XDocWidget> widget = qSharedPointerDynamicCast<XDocWidget>(*itDoc))
                            widget->Refresh();
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
