#ifndef SECTORINGMODELIMPL_H
#define SECTORINGMODELIMPL_H

#include "RoleModel.h"

//! Tree model
class CSectoringModel: public QStandardItemModel
{
    Q_OBJECT
public:
    // Lists
    static const char LISTACC_NAME[];
    static const char LISTACC_DESCR[];
    static const char LISTACC_UNIQUE[];

    static const char LISTSILACC_NAME[];
    static const char LISTSILACC_DESCR[];
    static const char LISTSILACC_UNIQUE[];

    static const char LISTAPP_NAME[];
    static const char LISTAPP_DESCR[];
    static const char LISTAPP_UNIQUE[];

    static const char LISTSILAPPARR_NAME[];
    static const char LISTSILAPPARR_DESCR[];
    static const char LISTSILAPPARR_UNIQUE[];

    static const char LISTSILAPPDEP_NAME[];
    static const char LISTSILAPPDEP_DESCR[];
    static const char LISTSILAPPDEP_UNIQUE[];

    static const char LISTMETEO_NAME[];
    static const char LISTMETEO_DESCR[];
    static const char LISTMETEO_UNIQUE[];

    static const char LISTMETEOTAF_NAME[];
    static const char LISTMETEOTAF_DESCR[];
    static const char LISTMETEOTAF_UNIQUE[];

    static const char LISTMETEOMETAR_NAME[];
    static const char LISTMETEOMETAR_DESCR[];
    static const char LISTMETEOMETAR_UNIQUE[];

    static const char LISTMETEOSIGMET_NAME[];
    static const char LISTMETEOSIGMET_DESCR[];
    static const char LISTMETEOSIGMET_UNIQUE[];

    static const char LISTMETEOGAMET_NAME[];
    static const char LISTMETEOGAMET_DESCR[];
    static const char LISTMETEOGAMET_UNIQUE[];

    static const char LISTMETEOMETREPORT_NAME[];
    static const char LISTMETEOMETREPORT_DESCR[];
    static const char LISTMETEOMETREPORT_UNIQUE[];

    static const char LISTRESTRICTION_NAME[];
    static const char LISTRESTRICTION_DESCR[];
    static const char LISTRESTRICTION_UNIQUE[];

    static const char RUNWAY_NAME[];
    static const char RUNWAY_DESCR[];
    static const char RUNWAY_UNIQUE[];

    static const char LISTACTIN_NAME[];
    static const char LISTACTIN_DESCR[];
    static const char LISTACTIN_UNIQUE[];

    static const char LISTACTOUT_NAME[];
    static const char LISTACTOUT_DESCR[];
    static const char LISTACTOUT_UNIQUE[];

    static const char LISTACTDEP_NAME[];
    static const char LISTACTDEP_DESCR[];
    static const char LISTACTDEP_UNIQUE[];

    static const char DAILYPLAN_NAME[];
    static const char DAILYPLAN_DESCR[];
    static const char DAILYPLAN_UNIQUE[];

    static const char LISTCOST_NAME[];
    static const char LISTCOST_DESCR[];
    static const char LISTCOST_UNIQUE[];

    // Toolbox
    static const char TOOLBTIME_NAME[];
    static const char TOOLBTIME_DESCR[];
    static const char TOOLBTIME_UNIQUE[];

    static const char TOOLBPRESSURE_NAME[];
    static const char TOOLBPRESSURE_DESCR[];
    static const char TOOLBPRESSURE_UNIQUE[];

    static const char TOOLBPAIRPLANELENGTH_NAME[];
    static const char TOOLBPAIRPLANELENGTH_DESCR[];
    static const char TOOLBPAIRPLANELENGTH_UNIQUE[];

    static const char TOOLBCUSTOMSECTOR_NAME[];
    static const char TOOLBCUSTOMSECTOR_DESCR[];
    static const char TOOLBCUSTOMSECTOR_UNIQUE[];

    static const char TOOLBALTITUDEFILTER_NAME[];
    static const char TOOLBALTITUDEFILTER_DESCR[];
    static const char TOOLBALTITUDEFILTER_UNIQUE[];

    static const char TOOLBRESTRICTION_NAME[];
    static const char TOOLBRESTRICTION_DESCR[];
    static const char TOOLBRESTRICTION_UNIQUE[];

    static const char TOOLBGROUND_NAME[];
    static const char TOOLBGROUND_DESCR[];
    static const char TOOLBGROUND_UNIQUE[];

    // Aerodrome (ground taxiing)
    static const char GROUNDTAXIING_NAME[];
    static const char GROUNDTAXIING_DESCR[];
    static const char GROUNDTAXIING_UNIQUE[];

    CSectoringModel();
    virtual ~CSectoringModel();

    void BuildTree();
    void retranslate();

private:
    void translateItem(QStandardItem* item);
    void _SetHeader();
};

#endif // SECTORINGMODELIMPL_H
