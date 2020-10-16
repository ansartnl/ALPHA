#ifndef ASTERIX062_PROTO_H
#define ASTERIX062_PROTO_H

#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix062Proto : public Asterix
{
    static const QMap<QString, QPair<QVariant, QVariant> > validationMap;

public:
    Asterix062Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    static QStringList validate(const QMap<QString, QVariant> &map);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    // Field names
    static const QString DataSourceIdentifier_SAC; // u8 mandatory
    static const QString DataSourceIdentifier_SIC; // u8 mandatory

    static const QString ServiceIdentification; // u8

    static const QString TimeOfTrackInformation; // QTime mandatory

    static const QString CalculatedTrackPositionWGS_Latitude; // double(-90~90)
    static const QString CalculatedTrackPositionWGS_Longitude; // double(-180~180)

    static const QString CalculatedTrackPositionCartesian_x; // double(24bit)/2
    static const QString CalculatedTrackPositionCartesian_y; // double(24bit)/2

    static const QString CalculatedTrackVelocityCartesian_Vx; // double(16bit)/4
    static const QString CalculatedTrackVelocityCartesian_Vy; // double(16bit)/4

    static const QString CalculatedAccelerationCartesian_Ax; // double(8bit)/4
    static const QString CalculatedAccelerationCartesian_Ay; // double(8bit)/4

    static const QString TrackMode3ACode_hasChanged; // bool
    static const QString TrackMode3ACode_reply; // 12bit

    static const QString TargetIdentification_STI; // 2bit
    static const QString TargetIdentification_Character1; // 6bit
    static const QString TargetIdentification_Character2; // 6bit
    static const QString TargetIdentification_Character3; // 6bit
    static const QString TargetIdentification_Character4; // 6bit
    static const QString TargetIdentification_Character5; // 6bit
    static const QString TargetIdentification_Character6; // 6bit
    static const QString TargetIdentification_Character7; // 6bit
    static const QString TargetIdentification_Character8; // 6bit

    static const QString AircraftDerivedData_ADR; // 24bit
    static const QString AircraftDerivedData_ID;
    static const QString AircraftDerivedData_ID_Character1; // 6bit
    static const QString AircraftDerivedData_ID_Character2; // 6bit
    static const QString AircraftDerivedData_ID_Character3; // 6bit
    static const QString AircraftDerivedData_ID_Character4; // 6bit
    static const QString AircraftDerivedData_ID_Character5; // 6bit
    static const QString AircraftDerivedData_ID_Character6; // 6bit
    static const QString AircraftDerivedData_ID_Character7; // 6bit
    static const QString AircraftDerivedData_ID_Character8; // 6bit
    static const QString AircraftDerivedData_MHG; // double(16bit)*360/65536
    static const QString AircraftDerivedData_IAS_IM; // bool
    static const QString AircraftDerivedData_IAS; // double(15bit) / (bIM ? 1000 : 16384)
    static const QString AircraftDerivedData_TAS; // 16bit
    static const QString AircraftDerivedData_SAL_SAS; // bool
    static const QString AircraftDerivedData_SAL_Source; // 2bit
    static const QString AircraftDerivedData_SAL_Altitude; // double(13bit)*25
    static const QString AircraftDerivedData_FSS_MV; // bool
    static const QString AircraftDerivedData_FSS_AH; // bool
    static const QString AircraftDerivedData_FSS_AM; // bool
    static const QString AircraftDerivedData_FSS_Altitude; // double(13bit)*25
    static const QString AircraftDerivedData_TIS_NAV; // bool
    static const QString AircraftDerivedData_TIS_NVB; // bool
    static const QString AircraftDerivedData_TID_REP; // 8bit
    static const QString AircraftDerivedData_TID_TCA; // bool
    static const QString AircraftDerivedData_TID_NC; // bool
    static const QString AircraftDerivedData_TID_TCP; // 6bit
    static const QString AircraftDerivedData_TID_Altitude; // double(16bit) * 10
    static const QString AircraftDerivedData_TID_REP_Latitude; // double(24bit) * 180 / 8388608
    static const QString AircraftDerivedData_TID_REP_Longitude; // double(24bit) * 180 / 8388608
    static const QString AircraftDerivedData_TID_REP_PointType; // 4bit
    static const QString AircraftDerivedData_TID_REP_TD; // 2bit
    static const QString AircraftDerivedData_TID_REP_TRA; // bool
    static const QString AircraftDerivedData_TID_REP_TOA; // bool
    static const QString AircraftDerivedData_TID_REP_TOV; // 24bit
    static const QString AircraftDerivedData_TID_REP_TTR; // double(16bit) / 100

    static const QString AircraftDerivedData_COM; // 3bit
    static const QString AircraftDerivedData_COM_STAT; // 3bit
    static const QString AircraftDerivedData_COM_SSC; // bool
    static const QString AircraftDerivedData_COM_ARC; // bool
    static const QString AircraftDerivedData_COM_AIC; // bool
    static const QString AircraftDerivedData_COM_B1A; // bool
    static const QString AircraftDerivedData_COM_B1B; // 4bit

    static const QString AircraftDerivedData_SAB_AC; // 2bit
    static const QString AircraftDerivedData_SAB_MN; // 2bit
    static const QString AircraftDerivedData_SAB_DC; // 2bit
    static const QString AircraftDerivedData_SAB_GBS; // bool
    static const QString AircraftDerivedData_SAB_STAT; // 3bit

    static const QString AircraftDerivedData_ACS; // Array 7byte

    static const QString AircraftDerivedData_BVR; // double(16bit) * 6.25

    static const QString AircraftDerivedData_GVR; // double(16bit) * 6.25

    static const QString AircraftDerivedData_RAN; // double(16bit) / 100

    static const QString AircraftDerivedData_TAR_TI; // 2bit
    static const QString AircraftDerivedData_TAR_RateOfTurn; // double(7bit) / 4

    static const QString AircraftDerivedData_TAN; // double(16bit) * 360 / 65536

    static const QString AircraftDerivedData_GSP; // double(16bit) / 16384

    static const QString AircraftDerivedData_VUN; // 8bit

    static const QString AircraftDerivedData_MET_WS; // bool
    static const QString AircraftDerivedData_MET_WD; // bool
    static const QString AircraftDerivedData_MET_TMP; // bool
    static const QString AircraftDerivedData_MET_TRB; // bool
    static const QString AircraftDerivedData_MET_WindSpeed; // 16bit
    static const QString AircraftDerivedData_MET_WindDirection; // 16bit
    static const QString AircraftDerivedData_MET_Temperature; // double(16bit) / 4
    static const QString AircraftDerivedData_MET_Turbulence;  // 8bit

    static const QString AircraftDerivedData_EMC; // 8bit

    static const QString AircraftDerivedData_POS_Latitude; // double(24bit) * 180 / 8388608
    static const QString AircraftDerivedData_POS_Longitude; // double(24bit) * 180 / 8388608

    static const QString AircraftDerivedData_GAL; // double(16bit) * 6.25

    static const QString AircraftDerivedData_PUN; // 4bit

    static const QString AircraftDerivedData_MB_REP; // 8 bit
    static const QString AircraftDerivedData_MB_data; // QVariantList of 7bytes
    static const QString AircraftDerivedData_MB_BDS1; // QVariantList of 4 bit
    static const QString AircraftDerivedData_MB_BDS2; // QVariantList of 4 bit

    static const QString AircraftDerivedData_IAR; // 16bit

    static const QString AircraftDerivedData_MAC; // double(16bit) * 0.008

    static const QString AircraftDerivedData_BPS; //double(12bit) / 10

    static const QString TrackNumber; // 16bit mandatory

    static const QString TrackStatus_MON; // bool mandatory
    static const QString TrackStatus_SPI; // bool mandatory
    static const QString TrackStatus_MRH; // bool mandatory
    static const QString TrackStatus_SRC; // 3bit mandatory
    static const QString TrackStatus_CNF; // bool mandatory

    enum TrackStatusIdentifier
    {
        // Status of a track - main field (mandatory)
        ETrackStatus_MON = 1,
        ETrackStatus_SPI,
        ETrackStatus_MRH,
        ETrackStatus_SRC,
        ETrackStatus_CNF,
        // Structure of First Extent (optional)
        ETrackStatus_SIM,
        ETrackStatus_TSE,
        ETrackStatus_TSB,
        ETrackStatus_FPC,
        ETrackStatus_AFF,
        ETrackStatus_STP,
        ETrackStatus_KOS,
        // Structure of Second Extent (optional)
        ETrackStatus_AMA,
        ETrackStatus_MD4,
        ETrackStatus_ME,
        ETrackStatus_MI,
        ETrackStatus_MD5,
        // Structure of Third Extent (optional)
        ETrackStatus_CST,
        ETrackStatus_PSR,
        ETrackStatus_SSR,
        ETrackStatus_MDS,
        ETrackStatus_ADS,
        ETrackStatus_SUC,
        ETrackStatus_AAC
    };
    typedef QMap<TrackStatusIdentifier, quint8> TTrackStatusMap;

    static const QString TrackStatus;     // TTrackStatusMap mandatory

    static const QString SystemTrackUpdateAges_TRK; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_PSR; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_SSR; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_MDS; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_ADS; // double(16bit) / 4
    static const QString SystemTrackUpdateAges_ES; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_VDL; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_UAT; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_LOP; // double(8bit) / 4
    static const QString SystemTrackUpdateAges_MLT; // double(8bit) / 4

    static const QString ModeOfMovement_TRANS; // 2bit
    static const QString ModeOfMovement_LONG; // 2bit
    static const QString ModeOfMovement_VERT; // 2bit
    static const QString ModeOfMovement_ADF; // bool

    static const QString TrackDataAges_FML; // double(8bit) / 4
    static const QString TrackDataAges_MD1; // double(8bit) / 4
    static const QString TrackDataAges_MD2; // double(8bit) / 4
    static const QString TrackDataAges_MDA; // double(8bit) / 4
    static const QString TrackDataAges_MD4; // double(8bit) / 4
    static const QString TrackDataAges_MD5; // double(8bit) / 4
    static const QString TrackDataAges_MHG; // double(8bit) / 4
    static const QString TrackDataAges_IAS; // double(8bit) / 4
    static const QString TrackDataAges_TAS; // double(8bit) / 4
    static const QString TrackDataAges_SAL; // double(8bit) / 4
    static const QString TrackDataAges_FSS; // double(8bit) / 4
    static const QString TrackDataAges_TID; // double(8bit) / 4
    static const QString TrackDataAges_COM; // double(8bit) / 4
    static const QString TrackDataAges_SAB; // double(8bit) / 4
    static const QString TrackDataAges_ACS; // double(8bit) / 4
    static const QString TrackDataAges_BVR; // double(8bit) / 4
    static const QString TrackDataAges_GVR; // double(8bit) / 4
    static const QString TrackDataAges_RAN; // double(8bit) / 4
    static const QString TrackDataAges_TAR; // double(8bit) / 4
    static const QString TrackDataAges_TAN; // double(8bit) / 4
    static const QString TrackDataAges_GSP; // double(8bit) / 4
    static const QString TrackDataAges_VUN; // double(8bit) / 4
    static const QString TrackDataAges_MET; // double(8bit) / 4
    static const QString TrackDataAges_EMC; // double(8bit) / 4
    static const QString TrackDataAges_POS; // double(8bit) / 4
    static const QString TrackDataAges_GAL; // double(8bit) / 4
    static const QString TrackDataAges_PUN; // double(8bit) / 4
    static const QString TrackDataAges_MB; // double(8bit) / 4
    static const QString TrackDataAges_IAR; // double(8bit) / 4
    static const QString TrackDataAges_MAC; // double(8bit) / 4
    static const QString TrackDataAges_BPS; // double(8bit) / 4

    static const QString MeasuredFlightLevel; // double(16bit) / 4

    static const QString CalculatedTrackGeometricAltitude; // double(16bit) * 6.25

    static const QString CalculatedTrackBarometricAltitude_QNH; // bool
    static const QString CalculatedTrackBarometricAltitude; // double(15bit) / 4

    static const QString CalculatedRateOfClimbDescent; // double(16bit) * 6.25

    static const QString FlightPlanRelatedData_TAG_SAC; // 8bit
    static const QString FlightPlanRelatedData_TAG_SIC; // 8bit

    static const QString FlightPlanRelatedData_CSN; // array 7byte

    static const QString FlightPlanRelatedData_IFI_TYP; // 2bit
    static const QString FlightPlanRelatedData_IFI_NBR; // 27bit

    static const QString FlightPlanRelatedData_FCT_GAT_OAT; // 2bit
    static const QString FlightPlanRelatedData_FCT_FR1_FR2; // 2bit
    static const QString FlightPlanRelatedData_FCT_RVSM; // 2bit
    static const QString FlightPlanRelatedData_FCT_HPR; // bool

    static const QString FlightPlanRelatedData_TAC; // array 4byte

    static const QString FlightPlanRelatedData_WTC; // 8bit

    static const QString FlightPlanRelatedData_DEP; // array 4byte

    static const QString FlightPlanRelatedData_DST; // array 4byte

    static const QString FlightPlanRelatedData_RDS_NU1; // 8bit
    static const QString FlightPlanRelatedData_RDS_NU2; // 8bit
    static const QString FlightPlanRelatedData_RDS_LTR; // 8bit

    static const QString FlightPlanRelatedData_CFL; // double(16bit) / 4

    static const QString FlightPlanRelatedData_CTL_Centre; // 8bit
    static const QString FlightPlanRelatedData_CTL_Position; // 8bit

    static const QString FlightPlanRelatedData_TOD_REP; // 8bit
    static const QString FlightPlanRelatedData_TOD_TYP; // 5bit
    static const QString FlightPlanRelatedData_TOD_DAY; // 2bit
    static const QString FlightPlanRelatedData_TOD_HOR; // 5bit
    static const QString FlightPlanRelatedData_TOD_MIN; // 6bit
    static const QString FlightPlanRelatedData_TOD_SEC; // 6bit

    static const QString FlightPlanRelatedData_AST; // array 6byte

    static const QString FlightPlanRelatedData_STS_EMP; // 2bit
    static const QString FlightPlanRelatedData_STS_AVL; // 2bit

    static const QString FlightPlanRelatedData_STD; // array 7byte

    static const QString FlightPlanRelatedData_STA; // array 7byte

    static const QString FlightPlanRelatedData_PEM_VA; // bool
    static const QString FlightPlanRelatedData_PEM_reply; // 12bit

    static const QString FlightPlanRelatedData_PEC; // array 7byte

    static const QString TargetSizeAndOrientation_Length; // 7bit
    static const QString TargetSizeAndOrientation_Orientation; // double(7bit) * 360 / 128
    static const QString TargetSizeAndOrientation_Width; // 7bit

    static const QString VehicleFleetIdentification; //8bit

    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_M5; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_ID; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_DA; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_M1; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_M2; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_M3; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_MC; //bool
    static const QString Mode5DataReportsAndExtendedMode1Code_SUM_X; //bool

    static const QString Mode5DataReportsAndExtendedMode1Code_PMN_PIN; // 14bit
    static const QString Mode5DataReportsAndExtendedMode1Code_PMN_NAT; // 6bit
    static const QString Mode5DataReportsAndExtendedMode1Code_PMN_MIS; // 6bit

    static const QString Mode5DataReportsAndExtendedMode1Code_POS_Latitude; // double(24bit) * 180 / 8388608
    static const QString Mode5DataReportsAndExtendedMode1Code_POS_Longitude; // double(24bit) * 180 / 8388608

    static const QString Mode5DataReportsAndExtendedMode1Code_GA_RES; // bool
    static const QString Mode5DataReportsAndExtendedMode1Code_GA; // 14bit * 25

    static const QString Mode5DataReportsAndExtendedMode1Code_EM1; // 12bit

    static const QString Mode5DataReportsAndExtendedMode1Code_TOS; // double(read<quint8>(stream)) / 128

    static const QString Mode5DataReportsAndExtendedMode1Code_XP_X5; // bool
    static const QString Mode5DataReportsAndExtendedMode1Code_XP_XC; // bool
    static const QString Mode5DataReportsAndExtendedMode1Code_XP_X3; // bool
    static const QString Mode5DataReportsAndExtendedMode1Code_XP_X2; // bool
    static const QString Mode5DataReportsAndExtendedMode1Code_XP_X1; // bool

    static const QString TrackMode2Code; // 12bit

    static const QString ComposedTrackNumber_SystemUnitIdentification; // 8bit EXTENDABLE
    static const QString ComposedTrackNumber_SystemTrackNumber; // 15bit

    static const QString EstimatedAccuracies_APC_X; // double(16bit) / 2
    static const QString EstimatedAccuracies_APC_Y; // double(16bit) / 2

    static const QString EstimatedAccuracies_COV; // double(16bit) / 2

    static const QString EstimatedAccuracies_APW_Latitude; // double(16bit) * 180 / 33554432
    static const QString EstimatedAccuracies_APW_Longitude; // double(16bit) * 180 / 33554432

    static const QString EstimatedAccuracies_AGA; // double(8bit) * 6.25

    static const QString EstimatedAccuracies_ABA; // double(8bit) / 4

    static const QString EstimatedAccuracies_ATV_X; // double(8bit) / 4
    static const QString EstimatedAccuracies_ATV_Y; // double(8bit) / 4

    static const QString EstimatedAccuracies_AA_X; // double(8bit) / 4
    static const QString EstimatedAccuracies_AA_Y; // double(8bit) / 4

    static const QString EstimatedAccuracies_ARC; // double(8bit) * 6.25

    static const QString MeasuredInformation_SID_SAC; // 8bit
    static const QString MeasuredInformation_SID_SIC; // 8bit

    static const QString MeasuredInformation_POS_RHO; // double(16bit) / 256
    static const QString MeasuredInformation_POS_THETA; // double(16bit) * 360 / 65536

    static const QString MeasuredInformation_HEI; // double(16bit) * 25

    static const QString MeasuredInformation_MDC_V; // bool
    static const QString MeasuredInformation_MDC_G; // bool
    static const QString MeasuredInformation_MDC; // double(14bit) / 4

    static const QString MeasuredInformation_MDA_V; // bool
    static const QString MeasuredInformation_MDA_G; // bool
    static const QString MeasuredInformation_MDA_L; // bool
    static const QString MeasuredInformation_MDA; // 12bit

    static const QString MeasuredInformation_TYP; // 3bit
    static const QString MeasuredInformation_TYP_SIM; // bool
    static const QString MeasuredInformation_TYP_RAB; // bool
    static const QString MeasuredInformation_TYP_TST; // bool

private:
    QChar decode(quint8 code);
};

Q_DECLARE_METATYPE(Asterix062Proto::TTrackStatusMap)

#endif //ASTERIX062_PROTO_H
