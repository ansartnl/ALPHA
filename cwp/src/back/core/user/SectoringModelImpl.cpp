#include "SectoringModelImpl.h"

const char CSectoringModel::LISTACC_NAME[] = "ACC";
const char CSectoringModel::LISTACC_DESCR[] = "Area control center";
const char CSectoringModel::LISTACC_UNIQUE[] = "List ACC";

const char CSectoringModel::LISTSILACC_NAME[] = "SIL ACC";
const char CSectoringModel::LISTSILACC_DESCR[] = "Single inboundary list ACC";
const char CSectoringModel::LISTSILACC_UNIQUE[] = "List SIL ACC";

const char CSectoringModel::LISTAPP_NAME[] = "APP";
const char CSectoringModel::LISTAPP_DESCR[] = "Approach position";
const char CSectoringModel::LISTAPP_UNIQUE[] = "List APP";

const char CSectoringModel::LISTSILAPPARR_NAME[] = "SIL APP ARR";
const char CSectoringModel::LISTSILAPPARR_DESCR[] = "Single inboundary list APP arrival";
const char CSectoringModel::LISTSILAPPARR_UNIQUE[] = "List SIL APP ARR";

const char CSectoringModel::LISTSILAPPDEP_NAME[] = "SIL APP DEP";
const char CSectoringModel::LISTSILAPPDEP_DESCR[] = "Single inboundary list APP departure";
const char CSectoringModel::LISTSILAPPDEP_UNIQUE[] = "List SIL APP DEP";

const char CSectoringModel::LISTMETEO_NAME[] = "Runway";
const char CSectoringModel::LISTMETEO_DESCR[] = "Runway occupation";
const char CSectoringModel::LISTMETEO_UNIQUE[] = "Runway occupation";

const char CSectoringModel::LISTRESTRICTION_NAME[] = "Restriction";
const char CSectoringModel::LISTRESTRICTION_DESCR[] = "User restriction zone editor";
const char CSectoringModel::LISTRESTRICTION_UNIQUE[] = "List restrictions";

const char CSectoringModel::RUNWAY_NAME[] = "Meteo";
const char CSectoringModel::RUNWAY_DESCR[] = "Meteo information in area control center";
const char CSectoringModel::RUNWAY_UNIQUE[] = "List meteo";

const char CSectoringModel::LISTMETEOTAF_NAME[] = "Meteo Taf";
const char CSectoringModel::LISTMETEOTAF_DESCR[] = "Meteo Taf occupation";
const char CSectoringModel::LISTMETEOTAF_UNIQUE[] = "Meteo Taf occupation";

const char CSectoringModel::LISTMETEOMETAR_NAME[] = "Meteo Metar";
const char CSectoringModel::LISTMETEOMETAR_DESCR[] = "Meteo Metar occupation";
const char CSectoringModel::LISTMETEOMETAR_UNIQUE[] = "Meteo Metar occupation";

const char CSectoringModel::LISTMETEOSIGMET_NAME[] = "Meteo Sigmet";
const char CSectoringModel::LISTMETEOSIGMET_DESCR[] = "Meteo Sigmet occupation";
const char CSectoringModel::LISTMETEOSIGMET_UNIQUE[] = "Meteo Sigmet occupation";

const char CSectoringModel::LISTMETEOGAMET_NAME[] = "Meteo Gamet";
const char CSectoringModel::LISTMETEOGAMET_DESCR[] = "Meteo Gamet occupation";
const char CSectoringModel::LISTMETEOGAMET_UNIQUE[] = "Meteo Gamet occupation";

const char CSectoringModel::LISTMETEOMETREPORT_NAME[] = "Meteo Met Report";
const char CSectoringModel::LISTMETEOMETREPORT_DESCR[] = "Meteo Met Report occupation";
const char CSectoringModel::LISTMETEOMETREPORT_UNIQUE[] = "Meteo Met Report occupation";

const char CSectoringModel::LISTACTIN_NAME[] = "ACT IN";
const char CSectoringModel::LISTACTIN_DESCR[] = "List ACT IN";
const char CSectoringModel::LISTACTIN_UNIQUE[] = "List ACT_IN";

const char CSectoringModel::LISTACTOUT_NAME[] = "ACT OUT";
const char CSectoringModel::LISTACTOUT_DESCR[] = "List ACT OUT";
const char CSectoringModel::LISTACTOUT_UNIQUE[] = "List ACT_OUT";

const char CSectoringModel::LISTACTDEP_NAME[] = "ACT DEP";
const char CSectoringModel::LISTACTDEP_DESCR[] = "List ACT DEPARTURE";
const char CSectoringModel::LISTACTDEP_UNIQUE[] = "List ACT_DEP";

const char CSectoringModel::DAILYPLAN_NAME[] = "DAILY PLAN";
const char CSectoringModel::DAILYPLAN_DESCR[] = "List DAILY PLAN";
const char CSectoringModel::DAILYPLAN_UNIQUE[] = "List DAILY_PLAN";

const char CSectoringModel::LISTCOST_NAME[] = "COST";
const char CSectoringModel::LISTCOST_DESCR[] = "List COST";
const char CSectoringModel::LISTCOST_UNIQUE[] = "List COST";

const char CSectoringModel::TOOLBTIME_NAME[] = "Time";
const char CSectoringModel::TOOLBTIME_DESCR[] = "Current time";
const char CSectoringModel::TOOLBTIME_UNIQUE[] = "Toolbox time";

const char CSectoringModel::TOOLBPRESSURE_NAME[] = "Pressure/Level";
const char CSectoringModel::TOOLBPRESSURE_DESCR[] = "Airdrome pressure and transition level";
const char CSectoringModel::TOOLBPRESSURE_UNIQUE[] = "Toolbox pressure/level";

const char CSectoringModel::TOOLBPAIRPLANELENGTH_NAME[] = "Aiplane/Length";
const char CSectoringModel::TOOLBPAIRPLANELENGTH_DESCR[] = "Aiplane line length";
const char CSectoringModel::TOOLBPAIRPLANELENGTH_UNIQUE[] = "Toolbox aiplane/Length";

const char CSectoringModel::TOOLBCUSTOMSECTOR_NAME[] = "Custom sector";
const char CSectoringModel::TOOLBCUSTOMSECTOR_DESCR[] = "Create custom sector";
const char CSectoringModel::TOOLBCUSTOMSECTOR_UNIQUE[] = "Toolbox custom sector";

const char CSectoringModel::TOOLBALTITUDEFILTER_NAME[] = "Altitude filter";
const char CSectoringModel::TOOLBALTITUDEFILTER_DESCR[] = "Aiplane altitude filter";
const char CSectoringModel::TOOLBALTITUDEFILTER_UNIQUE[] = "Toolbox altitude filter";

const char CSectoringModel::TOOLBRESTRICTION_NAME[] = "User restrictions";
const char CSectoringModel::TOOLBRESTRICTION_DESCR[] = "Create/modify user restriction zones";
const char CSectoringModel::TOOLBRESTRICTION_UNIQUE[] = "Toolbox user restrictions";

const char CSectoringModel::TOOLBGROUND_NAME[] = "Ground taxiing";
const char CSectoringModel::TOOLBGROUND_DESCR[] = "Ground taxiing toolbar";
const char CSectoringModel::TOOLBGROUND_UNIQUE[] = "Toolbox ground taxiing";


const char CSectoringModel::GROUNDTAXIING_NAME[] = "Aerodrome (ground taxiing)";
const char CSectoringModel::GROUNDTAXIING_DESCR[] = "Shows airport view on the ground";
const char CSectoringModel::GROUNDTAXIING_UNIQUE[] = "Aerodrome (ground taxiing)";


static void unusedTrWorkaround()
{
    CSectoringModel::tr("ACC");
    CSectoringModel::tr("Area control center");
    CSectoringModel::tr("List ACC");

    CSectoringModel::tr("SIL ACC");
    CSectoringModel::tr("Single inboundary list ACC");
    CSectoringModel::tr("List SIL ACC");

    CSectoringModel::tr("APP");
    CSectoringModel::tr("Approach position");
    CSectoringModel::tr("List APP");

    CSectoringModel::tr("SIL APP ARR");
    CSectoringModel::tr("Single inboundary list APP arrival");
    CSectoringModel::tr("List SIL APP ARR");

    CSectoringModel::tr("SIL APP DEP");
    CSectoringModel::tr("Single inboundary list APP departure");
    CSectoringModel::tr("List SIL APP DEP");

    CSectoringModel::tr("ACT IN");
    CSectoringModel::tr("ACT IN");
    CSectoringModel::tr("ACT IN");

    CSectoringModel::tr("Meteo");
    CSectoringModel::tr("Meteo information in area control center");
    CSectoringModel::tr("List meteo");

    CSectoringModel::tr("Restriction");
    CSectoringModel::tr("User restriction zone editor");
    CSectoringModel::tr("List restrictions");

    CSectoringModel::tr("Time");
    CSectoringModel::tr("Current time");
    CSectoringModel::tr("Toolbox time");

    CSectoringModel::tr("Pressure/Level");
    CSectoringModel::tr("Airdrome pressure and transition level");
    CSectoringModel::tr("Toolbox pressure/level");

    CSectoringModel::tr("Aiplane/Length");
    CSectoringModel::tr("Aiplane line length");
    CSectoringModel::tr("Toolbox aiplane/Length");

    CSectoringModel::tr("Custom sector");
    CSectoringModel::tr("Create custom sector");
    CSectoringModel::tr("Toolbox custom sector");

    CSectoringModel::tr("Altitude filter");
    CSectoringModel::tr("Aiplane altitude filter");
    CSectoringModel::tr("Toolbox altitude filter");

    CSectoringModel::tr("User restrictions");
    CSectoringModel::tr("Create/modify user restriction zones");
    CSectoringModel::tr("Toolbox user restrictions");

    CSectoringModel::tr("Ground taxiing");
    CSectoringModel::tr("Ground taxiing toolbar");
    CSectoringModel::tr("Toolbox ground taxiing");

    CSectoringModel::tr("Aerodrome (ground taxiing)");
    CSectoringModel::tr("Shows airport view on the ground");
    CSectoringModel::tr("Aerodrome (ground taxiing)");
}


CSectoringModel::CSectoringModel()
{
    unusedTrWorkaround();
    BuildTree();
}

CSectoringModel::~CSectoringModel()
{

}

void CSectoringModel::BuildTree()
{
    _SetHeader();
    QStandardItem *parentItem = invisibleRootItem();
    // 1 Level
    // List
    QStandardItem* pList = new QStandardItem(tr("List"));
    parentItem->setChild(0, 0, pList);
    parentItem->setChild(0, 1, new QStandardItem(tr("List for depiction")));
    // Toolbox
    QStandardItem* pToolbox = new QStandardItem(tr("Toolbox"));
    parentItem->setChild(1, 0, pToolbox);
    parentItem->setChild(1, 1, new QStandardItem(tr("Toolbox for operation")));
    // Aerodrome (ground taxiing)
    /*{
        QStandardItem* pDescr = new QStandardItem();
        CRoleItem* pListItem = new CRoleItem(pDescr, GROUNDTAXIING_UNIQUE,
                                             GROUNDTAXIING_NAME, GROUNDTAXIING_DESCR);
        parentItem->setChild(2, 0, pListItem);
        parentItem->setChild(2, 1, pDescr);
    }*/
    // 2 Level
    // List
    QStandardItem* pDescr = new QStandardItem();
    CRoleItem* pListItem = new CRoleItem(pDescr, LISTACC_UNIQUE,
                                         LISTACC_NAME, LISTACC_DESCR);
    pList->setChild(0, 0, pListItem);
    pList->setChild(0, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTSILACC_UNIQUE,
                              LISTSILACC_NAME, LISTSILACC_DESCR);
    pList->setChild(1, 0, pListItem);
    pList->setChild(1, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTAPP_UNIQUE, LISTAPP_NAME,
                              LISTAPP_DESCR);
    pList->setChild(2, 0, pListItem);
    pList->setChild(2, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTSILAPPARR_UNIQUE,
                              LISTSILAPPARR_NAME, LISTSILAPPARR_DESCR);
    pList->setChild(3, 0, pListItem);
    pList->setChild(3, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTSILAPPDEP_UNIQUE,
                              LISTSILAPPDEP_NAME, LISTSILAPPDEP_DESCR);
    pList->setChild(4, 0, pListItem);
    pList->setChild(4, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTACTIN_UNIQUE,
                              LISTACTIN_NAME, LISTACTIN_DESCR);
    pList->setChild(5, 0, pListItem);
    pList->setChild(5, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTACTOUT_UNIQUE,
                              LISTACTOUT_NAME, LISTACTOUT_DESCR);
    pList->setChild(6, 0, pListItem);
    pList->setChild(6, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTACTDEP_UNIQUE,
                              LISTACTDEP_NAME, LISTACTDEP_DESCR);
    pList->setChild(7, 0, pListItem);
    pList->setChild(7, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTMETEO_UNIQUE,
                              LISTMETEO_NAME, LISTMETEO_DESCR);
    pList->setChild(8, 0, pListItem);
    pList->setChild(8, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTRESTRICTION_UNIQUE,
                              LISTRESTRICTION_NAME, LISTRESTRICTION_DESCR);
    pList->setChild(9, 0, pListItem);
    pList->setChild(9, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, RUNWAY_UNIQUE,
                              RUNWAY_NAME, RUNWAY_DESCR);
    pList->setChild(10, 0, pListItem);
    pList->setChild(10, 1, pDescr);

    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTMETEOTAF_UNIQUE,
                              LISTMETEOTAF_NAME, LISTMETEOTAF_DESCR);
    pList->setChild(11, 0, pListItem);
    pList->setChild(11, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTMETEOMETAR_UNIQUE,
                              LISTMETEOMETAR_NAME, LISTMETEOMETAR_DESCR);
    pList->setChild(12, 0, pListItem);
    pList->setChild(12, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTMETEOSIGMET_UNIQUE,
                              LISTMETEOSIGMET_NAME, LISTMETEOSIGMET_DESCR);
    pList->setChild(13, 0, pListItem);
    pList->setChild(13, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTMETEOGAMET_UNIQUE,
                              LISTMETEOGAMET_NAME, LISTMETEOGAMET_DESCR);
    pList->setChild(14, 0, pListItem);
    pList->setChild(14, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTMETEOMETREPORT_UNIQUE,
                              LISTMETEOMETREPORT_NAME, LISTMETEOMETREPORT_DESCR);
    pList->setChild(15, 0, pListItem);
    pList->setChild(15, 1, pDescr);

    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, DAILYPLAN_UNIQUE,
                              DAILYPLAN_NAME, DAILYPLAN_DESCR);
    pList->setChild(16, 0, pListItem);
    pList->setChild(16, 1, pDescr);

    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, LISTCOST_UNIQUE,
                              LISTCOST_NAME, LISTCOST_DESCR);
    pList->setChild(17, 0, pListItem);
    pList->setChild(17, 1, pDescr);

    // Toolbox
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBTIME_UNIQUE,
                              TOOLBTIME_NAME, TOOLBTIME_DESCR);
    pToolbox->setChild(0, 0, pListItem);
    pToolbox->setChild(0, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBPRESSURE_UNIQUE,
                              TOOLBPRESSURE_NAME, TOOLBPRESSURE_DESCR);
    pToolbox->setChild(1, 0, pListItem);
    pToolbox->setChild(1, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBPAIRPLANELENGTH_UNIQUE,
                              TOOLBPAIRPLANELENGTH_NAME, TOOLBPAIRPLANELENGTH_DESCR);
    pToolbox->setChild(2, 0, pListItem);
    pToolbox->setChild(2, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBCUSTOMSECTOR_UNIQUE,
                              TOOLBCUSTOMSECTOR_NAME, TOOLBCUSTOMSECTOR_DESCR);
    pToolbox->setChild(3, 0, pListItem);
    pToolbox->setChild(3, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBALTITUDEFILTER_UNIQUE,
                              TOOLBALTITUDEFILTER_NAME, TOOLBALTITUDEFILTER_DESCR);
    pToolbox->setChild(4, 0, pListItem);
    pToolbox->setChild(4, 1, pDescr);
    //----
    pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBRESTRICTION_UNIQUE,
                              TOOLBRESTRICTION_NAME, TOOLBRESTRICTION_DESCR);
    pToolbox->setChild(5, 0, pListItem);
    pToolbox->setChild(5, 1, pDescr);
    //----
    /*pDescr = new QStandardItem();
    pListItem = new CRoleItem(pDescr, TOOLBGROUND_UNIQUE,
                              TOOLBGROUND_NAME, TOOLBGROUND_DESCR);
    pToolbox->setChild(6, 0, pListItem);
    pToolbox->setChild(6, 1, pDescr);*/
    //----
    retranslate();
}

void CSectoringModel::retranslate()
{
    _SetHeader();
    QStandardItem* item = invisibleRootItem();
    for (int i = 0; i < item->rowCount(); ++i)
        translateItem(item->child(i));
}

void CSectoringModel::translateItem(QStandardItem* item)
{
    CRoleItem* roleItem = dynamic_cast<CRoleItem *>(item);
    if (roleItem)
    {
        roleItem->setText(tr(roleItem->GetName().toLatin1()));
        if (roleItem->m_pDescr != NULL && !roleItem->GetDescription().isEmpty())
            roleItem->m_pDescr->setText(tr(roleItem->GetDescription().toLatin1()));
    }

    for (int i = 0; i < item->rowCount(); ++i)
        translateItem(item->child(i));
}

void CSectoringModel::_SetHeader()
{
    QStringList sHeader;
    sHeader << tr("Permission") << tr("Description");
    setHorizontalHeaderLabels(sHeader);
}
