#include "asterix062.h"
#include "presentdata.h"
#include "shareutility.h"
#include "asterixdata.h"

#include <math.h>

#include <QtCore/QTime>


const int Asterix062Proto::mCategory = 62;
const QString Asterix062Proto::mEdition = "1.13";

const QString Asterix062Proto::DataSourceIdentifier_SAC = ASTERIX_SAC;
const QString Asterix062Proto::DataSourceIdentifier_SIC = ASTERIX_SIC;

const QString Asterix062Proto::ServiceIdentification = "a062.ServiceIdentification";

const QString Asterix062Proto::TimeOfTrackInformation = "a062.TimeOfTrackInformation";

const QString Asterix062Proto::CalculatedTrackPositionWGS_Latitude = "a062.CalculatedTrackPosition(WGS-84).Latitude";
const QString Asterix062Proto::CalculatedTrackPositionWGS_Longitude = "a062.CalculatedTrackPosition(WGS-84).Longitude";

const QString Asterix062Proto::CalculatedTrackPositionCartesian_x = "a062.CalculatedTrackPosition(Cartesian).x";
const QString Asterix062Proto::CalculatedTrackPositionCartesian_y = "a062.CalculatedTrackPosition(Cartesian).y";

const QString Asterix062Proto::CalculatedTrackVelocityCartesian_Vx = "a062.CalculatedTrackVelocity(Cartesian).Vx";
const QString Asterix062Proto::CalculatedTrackVelocityCartesian_Vy = "a062.CalculatedTrackVelocity(Cartesian).Vy";

const QString Asterix062Proto::CalculatedAccelerationCartesian_Ax = "a062.CalculatedAcceleration(Cartesian).Ax";
const QString Asterix062Proto::CalculatedAccelerationCartesian_Ay = "a062.CalculatedAcceleration(Cartesian).Ay";

const QString Asterix062Proto::TrackMode3ACode_hasChanged = "a062.TrackMode3/ACode.hasChanged";
const QString Asterix062Proto::TrackMode3ACode_reply = "a062.TrackMode3/ACode.reply";

const QString Asterix062Proto::TargetIdentification_STI = "a062.TargetIdentification.STI";
const QString Asterix062Proto::TargetIdentification_Character1 = "a062.TargetIdentification.Character1";
const QString Asterix062Proto::TargetIdentification_Character2 = "a062.TargetIdentification.Character2";
const QString Asterix062Proto::TargetIdentification_Character3 = "a062.TargetIdentification.Character3";
const QString Asterix062Proto::TargetIdentification_Character4 = "a062.TargetIdentification.Character4";
const QString Asterix062Proto::TargetIdentification_Character5 = "a062.TargetIdentification.Character5";
const QString Asterix062Proto::TargetIdentification_Character6 = "a062.TargetIdentification.Character6";
const QString Asterix062Proto::TargetIdentification_Character7 = "a062.TargetIdentification.Character7";
const QString Asterix062Proto::TargetIdentification_Character8 = "a062.TargetIdentification.Character8";

const QString Asterix062Proto::AircraftDerivedData_ADR = "a062.AircraftDerivedData.ADR";
const QString Asterix062Proto::AircraftDerivedData_ID = "a062.AircraftDerivedData.ID";
const QString Asterix062Proto::AircraftDerivedData_ID_Character1 = "a062.AircraftDerivedData.ID.Character1";
const QString Asterix062Proto::AircraftDerivedData_ID_Character2 = "a062.AircraftDerivedData.ID.Character2";
const QString Asterix062Proto::AircraftDerivedData_ID_Character3 = "a062.AircraftDerivedData.ID.Character3";
const QString Asterix062Proto::AircraftDerivedData_ID_Character4 = "a062.AircraftDerivedData.ID.Character4";
const QString Asterix062Proto::AircraftDerivedData_ID_Character5 = "a062.AircraftDerivedData.ID.Character5";
const QString Asterix062Proto::AircraftDerivedData_ID_Character6 = "a062.AircraftDerivedData.ID.Character6";
const QString Asterix062Proto::AircraftDerivedData_ID_Character7 = "a062.AircraftDerivedData.ID.Character7";
const QString Asterix062Proto::AircraftDerivedData_ID_Character8 = "a062.AircraftDerivedData.ID.Character8";
const QString Asterix062Proto::AircraftDerivedData_MHG = "a062.AircraftDerivedData.MHG";
const QString Asterix062Proto::AircraftDerivedData_IAS_IM = "a062.AircraftDerivedData.IAS.IM";
const QString Asterix062Proto::AircraftDerivedData_IAS = "a062.AircraftDerivedData.IAS";
const QString Asterix062Proto::AircraftDerivedData_TAS = "a062.AircraftDerivedData.TAS";
const QString Asterix062Proto::AircraftDerivedData_SAL_SAS = "a062.AircraftDerivedData.SAL.SAS";
const QString Asterix062Proto::AircraftDerivedData_SAL_Source = "a062.AircraftDerivedData.SAL.Source";
const QString Asterix062Proto::AircraftDerivedData_SAL_Altitude = "a062.AircraftDerivedData.SAL.Altitude";
const QString Asterix062Proto::AircraftDerivedData_FSS_MV = "a062.AircraftDerivedData.FSS.MV";
const QString Asterix062Proto::AircraftDerivedData_FSS_AH = "a062.AircraftDerivedData.FSS.AH";
const QString Asterix062Proto::AircraftDerivedData_FSS_AM = "a062.AircraftDerivedData.FSS.AM";
const QString Asterix062Proto::AircraftDerivedData_FSS_Altitude = "a062.AircraftDerivedData.FSS.Altitude";
const QString Asterix062Proto::AircraftDerivedData_TIS_NAV = "a062.AircraftDerivedData.TIS.NAV";
const QString Asterix062Proto::AircraftDerivedData_TIS_NVB = "a062.AircraftDerivedData.TIS.NVB";
const QString Asterix062Proto::AircraftDerivedData_TID_REP = "a062.AircraftDerivedData.TID.REP";
const QString Asterix062Proto::AircraftDerivedData_TID_TCA = "a062.AircraftDerivedData.TID.TCA";
const QString Asterix062Proto::AircraftDerivedData_TID_NC = "a062.AircraftDerivedData.TID.NC";
const QString Asterix062Proto::AircraftDerivedData_TID_TCP = "a062.AircraftDerivedData.TID.TCP";
const QString Asterix062Proto::AircraftDerivedData_TID_Altitude = "a062.AircraftDerivedData.TID.Altitude";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_Latitude = "a062.AircraftDerivedData.TID.REP.Latitude";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_Longitude = "a062.AircraftDerivedData.TID.REP.Longitude";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_PointType = "a062.AircraftDerivedData.TID.REP.PointType";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_TD = "a062.AircraftDerivedData.TID.REP.TD";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_TRA = "a062.AircraftDerivedData.TID.REP.TRA";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_TOA = "a062.AircraftDerivedData.TID.REP.TOA";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_TOV = "a062.AircraftDerivedData.TID.REP.TOV";
const QString Asterix062Proto::AircraftDerivedData_TID_REP_TTR = "a062.AircraftDerivedData.TID.REP.TTR";
const QString Asterix062Proto::AircraftDerivedData_COM = "a062.AircraftDerivedData.COM";
const QString Asterix062Proto::AircraftDerivedData_COM_STAT = "a062.AircraftDerivedData.COM.STAT";
const QString Asterix062Proto::AircraftDerivedData_COM_SSC = "a062.AircraftDerivedData.COM.SSC";
const QString Asterix062Proto::AircraftDerivedData_COM_ARC = "a062.AircraftDerivedData.COM.ARC";
const QString Asterix062Proto::AircraftDerivedData_COM_AIC = "a062.AircraftDerivedData.COM.AIC";
const QString Asterix062Proto::AircraftDerivedData_COM_B1A = "a062.AircraftDerivedData.COM.B1A";
const QString Asterix062Proto::AircraftDerivedData_COM_B1B = "a062.AircraftDerivedData.COM.B1B";
const QString Asterix062Proto::AircraftDerivedData_SAB_AC = "a062.AircraftDerivedData.SAB.AC";
const QString Asterix062Proto::AircraftDerivedData_SAB_MN = "a062.AircraftDerivedData.SAB.MN";
const QString Asterix062Proto::AircraftDerivedData_SAB_DC = "a062.AircraftDerivedData.SAB.DC";
const QString Asterix062Proto::AircraftDerivedData_SAB_GBS = "a062.AircraftDerivedData.SAB.GBS";
const QString Asterix062Proto::AircraftDerivedData_SAB_STAT = "a062.AircraftDerivedData.SAB.STAT";
const QString Asterix062Proto::AircraftDerivedData_ACS = "a062.AircraftDerivedData.ACS";
const QString Asterix062Proto::AircraftDerivedData_BVR = "a062.AircraftDerivedData.BVR";
const QString Asterix062Proto::AircraftDerivedData_GVR = "a062.AircraftDerivedData.GVR";
const QString Asterix062Proto::AircraftDerivedData_RAN = "a062.AircraftDerivedData.RAN";
const QString Asterix062Proto::AircraftDerivedData_TAR_TI = "a062.AircraftDerivedData.TAR.TI";
const QString Asterix062Proto::AircraftDerivedData_TAR_RateOfTurn = "a062.AircraftDerivedData.TAR.RateOfTurn";
const QString Asterix062Proto::AircraftDerivedData_TAN = "a062.AircraftDerivedData.TAN";
const QString Asterix062Proto::AircraftDerivedData_GSP = "a062.AircraftDerivedData.GSP";
const QString Asterix062Proto::AircraftDerivedData_VUN = "a062.AircraftDerivedData.VUN";
const QString Asterix062Proto::AircraftDerivedData_MET_WS = "a062.AircraftDerivedData.MET.WS";
const QString Asterix062Proto::AircraftDerivedData_MET_WD = "a062.AircraftDerivedData.MET.WD";
const QString Asterix062Proto::AircraftDerivedData_MET_TMP = "a062.AircraftDerivedData.MET.TMP";
const QString Asterix062Proto::AircraftDerivedData_MET_TRB = "a062.AircraftDerivedData.MET.TRB";
const QString Asterix062Proto::AircraftDerivedData_MET_WindSpeed = "a062.AircraftDerivedData.MET.WindSpeed";
const QString Asterix062Proto::AircraftDerivedData_MET_WindDirection = "a062.AircraftDerivedData.MET.WindDirection";
const QString Asterix062Proto::AircraftDerivedData_MET_Temperature = "a062.AircraftDerivedData.MET.Temperature";
const QString Asterix062Proto::AircraftDerivedData_MET_Turbulence = "a062.AircraftDerivedData.MET.Turbulence";
const QString Asterix062Proto::AircraftDerivedData_EMC = "a062.AircraftDerivedData.EMC";
const QString Asterix062Proto::AircraftDerivedData_POS_Latitude = "a062.AircraftDerivedData.POS.Latitude";
const QString Asterix062Proto::AircraftDerivedData_POS_Longitude = "a062.AircraftDerivedData.POS.Longitude";
const QString Asterix062Proto::AircraftDerivedData_GAL = "a062.AircraftDerivedData.GAL";
const QString Asterix062Proto::AircraftDerivedData_PUN = "a062.AircraftDerivedData.PUN";
const QString Asterix062Proto::AircraftDerivedData_MB_REP = "a062.AircraftDerivedData.MB.REP";
const QString Asterix062Proto::AircraftDerivedData_MB_data = "a062.AircraftDerivedData.MB.data";
const QString Asterix062Proto::AircraftDerivedData_MB_BDS1 = "a062.AircraftDerivedData.MB.BDS1";
const QString Asterix062Proto::AircraftDerivedData_MB_BDS2 = "a062.AircraftDerivedData.MB.BDS2";
const QString Asterix062Proto::AircraftDerivedData_IAR = "a062.AircraftDerivedData.IAR";
const QString Asterix062Proto::AircraftDerivedData_MAC = "a062.AircraftDerivedData.MAC";
const QString Asterix062Proto::AircraftDerivedData_BPS = "a062.AircraftDerivedData.BPS";

const QString Asterix062Proto::TrackNumber = "a062.TrackNumber";

const QString Asterix062Proto::TrackStatus_MON = "a062.TrackStatus.MON";
const QString Asterix062Proto::TrackStatus_SPI = "a062.TrackStatus.SPI";
const QString Asterix062Proto::TrackStatus_MRH = "a062.TrackStatus.MRH";
const QString Asterix062Proto::TrackStatus_SRC = "a062.TrackStatus.SRC";
const QString Asterix062Proto::TrackStatus_CNF = "a062.TrackStatus.CNF";

const QString Asterix062Proto::TrackStatus = "a062.TrackStatus";

const QString Asterix062Proto::SystemTrackUpdateAges_TRK = "a062.SystemTrackUpdateAges.TRK";
const QString Asterix062Proto::SystemTrackUpdateAges_PSR = "a062.SystemTrackUpdateAges.PSR";
const QString Asterix062Proto::SystemTrackUpdateAges_SSR = "a062.SystemTrackUpdateAges.SSR";
const QString Asterix062Proto::SystemTrackUpdateAges_MDS = "a062.SystemTrackUpdateAges.MDS";
const QString Asterix062Proto::SystemTrackUpdateAges_ADS = "a062.SystemTrackUpdateAges.ADS";
const QString Asterix062Proto::SystemTrackUpdateAges_ES = "a062.SystemTrackUpdateAges.ES";
const QString Asterix062Proto::SystemTrackUpdateAges_VDL = "a062.SystemTrackUpdateAges.VDL";
const QString Asterix062Proto::SystemTrackUpdateAges_UAT = "a062.SystemTrackUpdateAges.UAT";
const QString Asterix062Proto::SystemTrackUpdateAges_LOP = "a062.SystemTrackUpdateAges.LOP";
const QString Asterix062Proto::SystemTrackUpdateAges_MLT = "a062.SystemTrackUpdateAges.MLT";

const QString Asterix062Proto::ModeOfMovement_TRANS = "a062.ModeOfMovement.TRANS";
const QString Asterix062Proto::ModeOfMovement_LONG = "a062.ModeOfMovement.LONG";
const QString Asterix062Proto::ModeOfMovement_VERT = "a062.ModeOfMovement.VERT";
const QString Asterix062Proto::ModeOfMovement_ADF = "a062.ModeOfMovement.ADF";

const QString Asterix062Proto::TrackDataAges_FML = "a062.TrackDataAges.FML";
const QString Asterix062Proto::TrackDataAges_MD1 = "a062.TrackDataAges.MD1";
const QString Asterix062Proto::TrackDataAges_MD2 = "a062.TrackDataAges.MD2";
const QString Asterix062Proto::TrackDataAges_MDA = "a062.TrackDataAges.MDA";
const QString Asterix062Proto::TrackDataAges_MD4 = "a062.TrackDataAges.MD4";
const QString Asterix062Proto::TrackDataAges_MD5 = "a062.TrackDataAges.MD5";
const QString Asterix062Proto::TrackDataAges_MHG = "a062.TrackDataAges.MHG";
const QString Asterix062Proto::TrackDataAges_IAS = "a062.TrackDataAges.IAS";
const QString Asterix062Proto::TrackDataAges_TAS = "a062.TrackDataAges.TAS";
const QString Asterix062Proto::TrackDataAges_SAL = "a062.TrackDataAges.SAL";
const QString Asterix062Proto::TrackDataAges_FSS = "a062.TrackDataAges.FSS";
const QString Asterix062Proto::TrackDataAges_TID = "a062.TrackDataAges.TID";
const QString Asterix062Proto::TrackDataAges_COM = "a062.TrackDataAges.COM";
const QString Asterix062Proto::TrackDataAges_SAB = "a062.TrackDataAges.SAB";
const QString Asterix062Proto::TrackDataAges_ACS = "a062.TrackDataAges.ACS";
const QString Asterix062Proto::TrackDataAges_BVR = "a062.TrackDataAges.BVR";
const QString Asterix062Proto::TrackDataAges_GVR = "a062.TrackDataAges.GVR";
const QString Asterix062Proto::TrackDataAges_RAN = "a062.TrackDataAges.RAN";
const QString Asterix062Proto::TrackDataAges_TAR = "a062.TrackDataAges.TAR";
const QString Asterix062Proto::TrackDataAges_TAN = "a062.TrackDataAges.TAN";
const QString Asterix062Proto::TrackDataAges_GSP = "a062.TrackDataAges.GSP";
const QString Asterix062Proto::TrackDataAges_VUN = "a062.TrackDataAges.VUN";
const QString Asterix062Proto::TrackDataAges_MET = "a062.TrackDataAges.MET";
const QString Asterix062Proto::TrackDataAges_EMC = "a062.TrackDataAges.EMC";
const QString Asterix062Proto::TrackDataAges_POS = "a062.TrackDataAges.POS";
const QString Asterix062Proto::TrackDataAges_GAL = "a062.TrackDataAges.GAL";
const QString Asterix062Proto::TrackDataAges_PUN = "a062.TrackDataAges.PUN";
const QString Asterix062Proto::TrackDataAges_MB = "a062.TrackDataAges.MB";
const QString Asterix062Proto::TrackDataAges_IAR = "a062.TrackDataAges.IAR";
const QString Asterix062Proto::TrackDataAges_MAC = "a062.TrackDataAges.MAC";
const QString Asterix062Proto::TrackDataAges_BPS = "a062.TrackDataAges.BPS";

const QString Asterix062Proto::MeasuredFlightLevel = "a062.MeasuredFlightLevel";

const QString Asterix062Proto::CalculatedTrackGeometricAltitude = "a062.CalculatedTrackGeometricAltitude";

const QString Asterix062Proto::CalculatedTrackBarometricAltitude_QNH = "a062.CalculatedTrackBarometricAltitude.QNH";
const QString Asterix062Proto::CalculatedTrackBarometricAltitude = "a062.CalculatedTrackBarometricAltitude";

const QString Asterix062Proto::CalculatedRateOfClimbDescent = "a062.CalculatedRateOfClimb/Descent";

const QString Asterix062Proto::FlightPlanRelatedData_TAG_SAC = "a062.FlightPlanRelatedData.TAG.SAC";
const QString Asterix062Proto::FlightPlanRelatedData_TAG_SIC = "a062.FlightPlanRelatedData.TAG.SIC";
const QString Asterix062Proto::FlightPlanRelatedData_CSN = "a062.FlightPlanRelatedData.CSN";
const QString Asterix062Proto::FlightPlanRelatedData_IFI_TYP = "a062.FlightPlanRelatedData.IFI.TYP";
const QString Asterix062Proto::FlightPlanRelatedData_IFI_NBR = "a062.FlightPlanRelatedData.IFI.NBR";
const QString Asterix062Proto::FlightPlanRelatedData_FCT_GAT_OAT = "a062.FlightPlanRelatedData.FCT.GAT/OAT";
const QString Asterix062Proto::FlightPlanRelatedData_FCT_FR1_FR2 = "a062.FlightPlanRelatedData.FCT.FR1/FR2";
const QString Asterix062Proto::FlightPlanRelatedData_FCT_RVSM = "a062.FlightPlanRelatedData.FCT.RVSM";
const QString Asterix062Proto::FlightPlanRelatedData_FCT_HPR = "a062.FlightPlanRelatedData.FCT.HPR";
const QString Asterix062Proto::FlightPlanRelatedData_TAC = "a062.FlightPlanRelatedData.TAC";
const QString Asterix062Proto::FlightPlanRelatedData_WTC = "a062.FlightPlanRelatedData.WTC";
const QString Asterix062Proto::FlightPlanRelatedData_DEP = "a062.FlightPlanRelatedData.DEP";
const QString Asterix062Proto::FlightPlanRelatedData_DST = "a062.FlightPlanRelatedData.DST";
const QString Asterix062Proto::FlightPlanRelatedData_RDS_NU1 = "a062.FlightPlanRelatedData.RDS.NU1";
const QString Asterix062Proto::FlightPlanRelatedData_RDS_NU2 = "a062.FlightPlanRelatedData.RDS.NU2";
const QString Asterix062Proto::FlightPlanRelatedData_RDS_LTR = "a062.FlightPlanRelatedData.RDS.LTR";
const QString Asterix062Proto::FlightPlanRelatedData_CFL = "a062.FlightPlanRelatedData.CFL";
const QString Asterix062Proto::FlightPlanRelatedData_CTL_Centre = "a062.FlightPlanRelatedData.CTL.Centre";
const QString Asterix062Proto::FlightPlanRelatedData_CTL_Position = "a062.FlightPlanRelatedData.CTL.Position";
const QString Asterix062Proto::FlightPlanRelatedData_TOD_REP = "a062.FlightPlanRelatedData.TOD.REP";
const QString Asterix062Proto::FlightPlanRelatedData_TOD_TYP = "a062.FlightPlanRelatedData.TOD.TYP";
const QString Asterix062Proto::FlightPlanRelatedData_TOD_DAY = "a062.FlightPlanRelatedData.TOD.DAY";
const QString Asterix062Proto::FlightPlanRelatedData_TOD_HOR = "a062.FlightPlanRelatedData.TOD.HOR";
const QString Asterix062Proto::FlightPlanRelatedData_TOD_MIN = "a062.FlightPlanRelatedData.TOD.MIN";
const QString Asterix062Proto::FlightPlanRelatedData_TOD_SEC = "a062.FlightPlanRelatedData.TOD.SEC";
const QString Asterix062Proto::FlightPlanRelatedData_AST = "a062.FlightPlanRelatedData.AST";
const QString Asterix062Proto::FlightPlanRelatedData_STS_EMP = "a062.FlightPlanRelatedData.STS.EMP";
const QString Asterix062Proto::FlightPlanRelatedData_STS_AVL = "a062.FlightPlanRelatedData.STS.AVL";
const QString Asterix062Proto::FlightPlanRelatedData_STD = "a062.FlightPlanRelatedData.STD";
const QString Asterix062Proto::FlightPlanRelatedData_STA = "a062.FlightPlanRelatedData.STA";
const QString Asterix062Proto::FlightPlanRelatedData_PEM_VA = "a062.FlightPlanRelatedData.PEM.VA";
const QString Asterix062Proto::FlightPlanRelatedData_PEM_reply = "a062.FlightPlanRelatedData.PEM.reply";
const QString Asterix062Proto::FlightPlanRelatedData_PEC = "a062.FlightPlanRelatedData.PEC";

const QString Asterix062Proto::TargetSizeAndOrientation_Length = "a062.TargetSizeAndOrientation.Length";
const QString Asterix062Proto::TargetSizeAndOrientation_Orientation = "a062.TargetSizeAndOrientation.Orientation";
const QString Asterix062Proto::TargetSizeAndOrientation_Width = "a062.TargetSizeAndOrientation.Width";

const QString Asterix062Proto::VehicleFleetIdentification = "a062.VehicleFleetIdentification";

const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_M5 = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.M5";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_ID = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.ID";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_DA = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.DA";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_M1 = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.M1";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_M2 = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.M2";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_M3 = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.M3";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_MC = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.MC";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_SUM_X = "a062.Mode5DataReportsAndExtendedMode1Code.SUM.X";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_PMN_PIN = "a062.Mode5DataReportsAndExtendedMode1Code.PMN.PIN";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_PMN_NAT = "a062.Mode5DataReportsAndExtendedMode1Code.PMN.NAT";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_PMN_MIS = "a062.Mode5DataReportsAndExtendedMode1Code.PMN.MIS";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_POS_Latitude = "a062.Mode5DataReportsAndExtendedMode1Code.POS.Latitude";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_POS_Longitude = "a062.Mode5DataReportsAndExtendedMode1Code.POS.Longitude";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_GA_RES = "a062.Mode5DataReportsAndExtendedMode1Code.GA.RES";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_GA = "a062.Mode5DataReportsAndExtendedMode1Code.GA";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_EM1 = "a062.Mode5DataReportsAndExtendedMode1Code.EM1";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_TOS = "a062.Mode5DataReportsAndExtendedMode1Code.TOS";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_XP_X5 = "a062.Mode5DataReportsAndExtendedMode1Code.XP.X5";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_XP_XC = "a062.Mode5DataReportsAndExtendedMode1Code.XP.XC";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_XP_X3 = "a062.Mode5DataReportsAndExtendedMode1Code.XP.X3";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_XP_X2 = "a062.Mode5DataReportsAndExtendedMode1Code.XP.X2";
const QString Asterix062Proto::Mode5DataReportsAndExtendedMode1Code_XP_X1 = "a062.Mode5DataReportsAndExtendedMode1Code.XP.X1";

const QString Asterix062Proto::TrackMode2Code = "a062.TrackMode2Code";

const QString Asterix062Proto::ComposedTrackNumber_SystemUnitIdentification = "a062.ComposedTrackNumber.%1.SYSTEM_UNIT_IDENTIFICATION";
const QString Asterix062Proto::ComposedTrackNumber_SystemTrackNumber = "a062.ComposedTrackNumber.%1.SYSTEM_TRACK_NUMBER";

const QString Asterix062Proto::EstimatedAccuracies_APC_X = "a062.EstimatedAccuracies.APC.X";
const QString Asterix062Proto::EstimatedAccuracies_APC_Y = "a062.EstimatedAccuracies.APC.Y";
const QString Asterix062Proto::EstimatedAccuracies_COV = "a062.EstimatedAccuracies.COV";
const QString Asterix062Proto::EstimatedAccuracies_APW_Latitude = "a062.EstimatedAccuracies.APW.Latitude";
const QString Asterix062Proto::EstimatedAccuracies_APW_Longitude = "a062.EstimatedAccuracies.APW.Longitude";
const QString Asterix062Proto::EstimatedAccuracies_AGA = "a062.EstimatedAccuracies.AGA";
const QString Asterix062Proto::EstimatedAccuracies_ABA = "a062.EstimatedAccuracies.ABA";
const QString Asterix062Proto::EstimatedAccuracies_ATV_X = "a062.EstimatedAccuracies.ATV.X";
const QString Asterix062Proto::EstimatedAccuracies_ATV_Y = "a062.EstimatedAccuracies.ATV.Y";
const QString Asterix062Proto::EstimatedAccuracies_AA_X = "a062.EstimatedAccuracies.AA.X";
const QString Asterix062Proto::EstimatedAccuracies_AA_Y = "a062.EstimatedAccuracies.AA.Y";
const QString Asterix062Proto::EstimatedAccuracies_ARC = "a062.EstimatedAccuracies.ARC";

const QString Asterix062Proto::MeasuredInformation_SID_SAC = "a062.MeasuredInformation.SID.SAC";
const QString Asterix062Proto::MeasuredInformation_SID_SIC = "a062.MeasuredInformation.SID.SIC";
const QString Asterix062Proto::MeasuredInformation_POS_RHO = "a062.MeasuredInformation.POS.RHO";
const QString Asterix062Proto::MeasuredInformation_POS_THETA = "a062.MeasuredInformation.POS.THETA";
const QString Asterix062Proto::MeasuredInformation_HEI = "a062.MeasuredInformation.HEI";
const QString Asterix062Proto::MeasuredInformation_MDC_V = "a062.MeasuredInformation.MDC.V";
const QString Asterix062Proto::MeasuredInformation_MDC_G = "a062.MeasuredInformation.MDC.G";
const QString Asterix062Proto::MeasuredInformation_MDC = "a062.MeasuredInformation.MDC";
const QString Asterix062Proto::MeasuredInformation_MDA_V = "a062.MeasuredInformation.MDA.V";
const QString Asterix062Proto::MeasuredInformation_MDA_G = "a062.MeasuredInformation.MDA.G";
const QString Asterix062Proto::MeasuredInformation_MDA_L = "a062.MeasuredInformation.MDA.L";
const QString Asterix062Proto::MeasuredInformation_MDA = "a062.MeasuredInformation.MDA";
const QString Asterix062Proto::MeasuredInformation_TYP = "a062.MeasuredInformation.TYP";
const QString Asterix062Proto::MeasuredInformation_TYP_SIM = "a062.MeasuredInformation.TYP.SIM";
const QString Asterix062Proto::MeasuredInformation_TYP_RAB = "a062.MeasuredInformation.TYP.RAB";
const QString Asterix062Proto::MeasuredInformation_TYP_TST = "a062.MeasuredInformation.TYP.TST";

Asterix062Proto::Asterix062Proto() {}

int Asterix062Proto::category() const
{
    return mCategory;
}

QString Asterix062Proto::edition() const
{
    return mEdition;
}

void Asterix062Proto::parse(QDataStream &stream, AsterixData &map)
{
    try
    {
        PresentData mainMask(stream);

        // 062/010 Data Source Identifier [M]
        if (mainMask.isSet(1))
        {
            map.insert(DataSourceIdentifier_SAC, read<quint8>(stream));
            map.insert(DataSourceIdentifier_SIC, read<quint8>(stream));
        } else
            map.appendError(mandatory("Data Source Identifier"));

        // I062/015 Service Identification [O]
        if (mainMask.isSet(3))
            map.insert(ServiceIdentification, read<quint8>(stream));

        // I062/070 Time Of Track Information [M]
        if (mainMask.isSet(4))
            map.insert(TimeOfTrackInformation, QTime().addMSecs(ceil(double(read24(stream)) * 1000 / 128)));
        else
            map.appendError(mandatory("Time Of Track Information"));

        // I062/105 Calculated Track Position (WGS-84) [O]
        if (mainMask.isSet(5))
        {
            map.insert(CalculatedTrackPositionWGS_Latitude, double(read<qint32>(stream)) * 180 / 33554432); // -90/90
            map.insert(CalculatedTrackPositionWGS_Longitude, double(read<qint32>(stream)) * 180 / 33554432); // -180/180
        }

        // I062/100 Calculated Track Position (Cartesian) [O]
        if (mainMask.isSet(6))
        {
            map.insert(CalculatedTrackPositionCartesian_x, double(read24(stream)) / 2); // i hope it's unsignet
            map.insert(CalculatedTrackPositionCartesian_y, double(read24(stream)) / 2); // i hope it's unsignet
        }

        // I062/185 Calculated Track Velocity (Cartesian) [O]
        if (mainMask.isSet(7))
        {
            map.insert(CalculatedTrackVelocityCartesian_Vx, double(read<qint16>(stream)) / 4); // -8192m/s <= Vx <= 8191.75m/s
            map.insert(CalculatedTrackVelocityCartesian_Vy, double(read<qint16>(stream)) / 4); // -8192m/s <= Vy <= 8191.75m/s
        }

        // I062/210 Calculated Acceleration (Cartesian) [O]
        if (mainMask.isSet(8))
        {
            map.insert(CalculatedAccelerationCartesian_Ax, double(read<qint8>(stream)) / 4);
            map.insert(CalculatedAccelerationCartesian_Ay, double(read<qint8>(stream)) / 4);
        }

        // I062/060 Track Mode 3/A Code [O]
        if (mainMask.isSet(9))
        {
            quint16 data = read<quint16>(stream);
            map.insert(TrackMode3ACode_hasChanged, data >> 13 & M1);
            map.insert(TrackMode3ACode_reply, data & M12);
        }

        // I062/245 Target Identification [O]
        if (mainMask.isSet(10))
        {
            map.insert(TargetIdentification_STI, read<quint8>(stream) >> 6 & M2);
            quint32 data = read24(stream);
            map.insert(TargetIdentification_Character1, data >> 18 & M6);
            map.insert(TargetIdentification_Character2, data >> 12 & M6);
            map.insert(TargetIdentification_Character3, data >> 6 & M6);
            map.insert(TargetIdentification_Character4, data & M6);
            data = read24(stream);
            map.insert(TargetIdentification_Character5, data >> 18 & M6);
            map.insert(TargetIdentification_Character6, data >> 12 & M6);
            map.insert(TargetIdentification_Character7, data >> 6 & M6);
            map.insert(TargetIdentification_Character8, data & M6);
        }

        // I062/380 Aircraft Derived Data [O]
        if (mainMask.isSet(11))
        {
            PresentData mask(stream);
            if (mask.isSet(1)) // Target Address
                map.insert(AircraftDerivedData_ADR, read24(stream));

            if (mask.isSet(2)) // Target Identification
            {
                QString id;
                quint8 ch;

                quint32 data = read24(stream);
                ch = data >> 18 & M6;
                map.insert(AircraftDerivedData_ID_Character1, ch);
                id.append(decode(ch));

                ch = data >> 12 & M6;
                map.insert(AircraftDerivedData_ID_Character2, ch);
                id.append(decode(ch));

                ch = data >> 6 & M6;
                map.insert(AircraftDerivedData_ID_Character3, ch);
                id.append(decode(ch));

                ch = data & M6;
                map.insert(AircraftDerivedData_ID_Character4, ch);
                id.append(decode(ch));

                data = read24(stream);
                ch = data >> 18 & M6;
                map.insert(AircraftDerivedData_ID_Character5, ch);
                id.append(decode(ch));

                ch = data >> 12 & M6;
                map.insert(AircraftDerivedData_ID_Character6, ch);
                id.append(decode(ch));

                ch = data >> 6 & M6;
                map.insert(AircraftDerivedData_ID_Character7, ch);
                id.append(decode(ch));

                ch = data & M6;
                map.insert(AircraftDerivedData_ID_Character8, ch);
                id.append(decode(ch));

                map.insert(AircraftDerivedData_ID, id);
            }

            if (mask.isSet(3)) // Magnetic Heading
                map.insert(AircraftDerivedData_MHG, double(read<quint16>(stream)) * 360 / 65536);

            if (mask.isSet(4)) // Indicated Airspeed/Mach Number
            {
                qint16 data = read<qint16>(stream);
                bool bIM = data >> 15 & M1;
                map.insert(AircraftDerivedData_IAS_IM, bIM);
                map.insert(AircraftDerivedData_IAS, double(data & M15) / (bIM ? 1000 : 16384) );
            }

            if (mask.isSet(5)) // True Airspeed
                map.insert(AircraftDerivedData_TAS, read<quint16>(stream)); // 0 <= True Air Speed <= 2046 knots

            if (mask.isSet(6)) // Selected Altitude
            {
                qint16 data = read<qint16>(stream);
                map.insert(AircraftDerivedData_SAL_SAS, data >> 15 & M1);
                map.insert(AircraftDerivedData_SAL_Source, data >> 13 & M2);
                map.insert(AircraftDerivedData_SAL_Altitude, double(restoreSign(data, 13)) * 25); // -1300ft <= Altitude <= 100000ft
            }

            if (mask.isSet(7)) // Final State SelectedAltitude
            {
                qint16 data = read<qint16>(stream);
                map.insert(AircraftDerivedData_FSS_MV, data >> 15 & M1);
                map.insert(AircraftDerivedData_FSS_AH, data >> 14 & M1);
                map.insert(AircraftDerivedData_FSS_AM, data >> 13 & M1);
                map.insert(AircraftDerivedData_FSS_Altitude, double(restoreSign(data, 13)) * 25); // -1300ft <= Altitude <= 100000ft
            }

            if (mask.isSet(8)) // Trajectory Intent Status
            {
                quint8 data = read<quint8>(stream);
                map.insert(AircraftDerivedData_TIS_NAV, data >> 7 & M1);
                map.insert(AircraftDerivedData_TIS_NVB, data >> 6 & M1);
                while (data & M1) // WHAT DOES EXTENSION MEAN???
                    data = read<quint8>(stream);
            }

            if (mask.isSet(9)) // Trajectory Intent Data
            {
                map.insert(AircraftDerivedData_TID_REP, read<qint8>(stream)); // is it unsigned?
                quint8 data = read<quint8>(stream);
                map.insert(AircraftDerivedData_TID_TCA, data >> 7 & M1);
                map.insert(AircraftDerivedData_TID_NC, data >> 6 & M1);
                map.insert(AircraftDerivedData_TID_TCP, data & M6);
                map.insert(AircraftDerivedData_TID_Altitude, double(read<qint16>(stream)) * 10); // -1500 ft <= altitude <= 150000 ft
                map.insert(AircraftDerivedData_TID_REP_Latitude, double(read24s(stream)) * 180 / 8388608); // -90 <= latitude <= 90 deg.
                map.insert(AircraftDerivedData_TID_REP_Longitude, double(read24s(stream)) * 180 / 8388608); // -180 <= longitude < 180
                data = read<quint8>(stream);
                map.insert(AircraftDerivedData_TID_REP_PointType, data >> 4 & M4);
                map.insert(AircraftDerivedData_TID_REP_TD, data >> 2 & M2);
                map.insert(AircraftDerivedData_TID_REP_TRA, data >> 1 & M1);
                map.insert(AircraftDerivedData_TID_REP_TOA, data & M1);
                map.insert(AircraftDerivedData_TID_REP_TOV, read24(stream));
                map.insert(AircraftDerivedData_TID_REP_TTR, double(read<quint16>(stream)) / 100); // 0 <= TTR <= 655.36 Nm
            }

            if (mask.isSet(10)) // Communications / ACAS; Capability and Flight Status
            {
                quint16 data = read<quint16>(stream);
                map.insert(AircraftDerivedData_COM, data >> 13 & M3);
                map.insert(AircraftDerivedData_COM_STAT, data >> 10 & M3);
                map.insert(AircraftDerivedData_COM_SSC, data >> 7 & M1);
                map.insert(AircraftDerivedData_COM_ARC, data >> 6 & M1);
                map.insert(AircraftDerivedData_COM_AIC, data >> 5 & M1);
                map.insert(AircraftDerivedData_COM_B1A, data >> 4 & M1);
                map.insert(AircraftDerivedData_COM_B1B, data & M4);
            }

            if (mask.isSet(11)) // Status reported by ADS-B
            {
                quint16 data = read<quint16>(stream);
                map.insert(AircraftDerivedData_SAB_AC, data >> 14 & M2);
                map.insert(AircraftDerivedData_SAB_MN, data >> 12 & M2);
                map.insert(AircraftDerivedData_SAB_DC, data >> 10 & M2);
                map.insert(AircraftDerivedData_SAB_GBS, data >> 9 & M1);
                map.insert(AircraftDerivedData_SAB_STAT, data & M3);
            }

            if (mask.isSet(12)) // ACAS Resolution Advisory Report
                map.insert(AircraftDerivedData_ACS, readArray(stream, 7));

            if (mask.isSet(13)) // Barometric Vertical Rate
                map.insert(AircraftDerivedData_BVR, double(read<qint16>(stream)) * 6.25);

            if (mask.isSet(14)) // Geometric Vertical Rate
                map.insert(AircraftDerivedData_GVR, double(read<qint16>(stream)) * 6.25);

            if (mask.isSet(15)) // Roll Angle
                map.insert(AircraftDerivedData_RAN, double(read<qint16>(stream)) / 100); // -180 <= Roll Angle <= 180

            if (mask.isSet(16)) // Track Angle Rate
            {
                qint16 data = read<qint16>(stream);
                map.insert(AircraftDerivedData_TAR_TI, data >> 14 & M2);
                map.insert(AircraftDerivedData_TAR_RateOfTurn, double(restoreSign(data >> 1, 7)) / 4); // -15 °/s <= Rate of Turn <= 15 °/s
            }

            if (mask.isSet(17)) // Track Angle
                map.insert(AircraftDerivedData_TAN, double(read<quint16>(stream)) * 360 / 65536);

            if (mask.isSet(18)) // Ground Speed
                map.insert(AircraftDerivedData_GSP, double(read<qint16>(stream)) / 16384); // -2 NM/s <= Ground Speed < 2 NM/s

            if (mask.isSet(19)) // Velocity Uncertainty
                map.insert(AircraftDerivedData_VUN, read<qint8>(stream)); // is it unsigned<=

            if (mask.isSet(20)) // Meteorological Data
            {
                quint8 data = read<quint8>(stream);
                map.insert(AircraftDerivedData_MET_WS, data >> 7 & M1);
                map.insert(AircraftDerivedData_MET_WD, data >> 6 & M1);
                map.insert(AircraftDerivedData_MET_TMP, data >> 5 & M1);
                map.insert(AircraftDerivedData_MET_TRB, data >> 4 & M1);
                map.insert(AircraftDerivedData_MET_WindSpeed, read<quint16>(stream)); // 0 <= Wind Speed <= 300
                map.insert(AircraftDerivedData_MET_WindDirection, read<quint16>(stream)); // 1 <= Wind Direction <= 360
                map.insert(AircraftDerivedData_MET_Temperature, double(read<qint16>(stream)) / 4); // -100 °C <= Temperature <= 100 °C
                map.insert(AircraftDerivedData_MET_Turbulence, read<quint8>(stream)); // Integer between 0 and 15 inclusive
            }

            if (mask.isSet(21)) // Emitter Category
                map.insert(AircraftDerivedData_EMC, read<quint8>(stream));

            if (mask.isSet(22)) // Position Data
            {
                map.insert(AircraftDerivedData_POS_Latitude, double(read24s(stream)) * 180 / 8388608); // Range -90 <= latitude <= 90 deg.
                map.insert(AircraftDerivedData_POS_Longitude, double(read24s(stream)) * 180 / 8388608); // Range -180 <= longitude < 180 deg.
            }

            if (mask.isSet(23)) // Geometric Altitude Data
                map.insert(AircraftDerivedData_GAL, double(read<qint16>(stream)) * 6.25); // -1500 ft <= Altitude <= 150000 ft

            if (mask.isSet(24)) // Position Uncertainty Data
                map.insert(AircraftDerivedData_PUN, read<quint8>(stream) & M4);

            if (mask.isSet(25)) // Mode S MB Data
            {
                int count = read<qint8>(stream);
                map.insert(AircraftDerivedData_MB_REP, count);

                QVariantList mbdata, mbbds1, mbbds2;
                for (int i = 0; i < count; ++i)
                {
                    mbdata.append(readArray(stream, 7));

                    quint8 data = read<quint8>(stream);
                    mbbds1.append(data >> 4 & M4);
                    mbbds2.append(data & M4);
                }

                map.insert(AircraftDerivedData_MB_data, mbdata);
                map.insert(AircraftDerivedData_MB_BDS1, mbbds1);
                map.insert(AircraftDerivedData_MB_BDS2, mbbds2);
            }

            if (mask.isSet(26)) // Indicated Airspeed
                map.insert(AircraftDerivedData_IAR, read<quint16>(stream)); // 0 Kt <= Indicated Airspeed <= 1100 Kt

            if (mask.isSet(27)) // Mach Number
                map.insert(AircraftDerivedData_MAC, double(read<quint16>(stream)) * 0.008); // 0 <= Mach Number <= 4.096

            if (mask.isSet(28)) // Barometric Pressure Setting
                map.insert(AircraftDerivedData_BPS, double(read<quint16>(stream) & M12) / 10); // -0ft <= BPS <= 409.5 mb
        }

        // I062/040 Track Number [M]
        if (mainMask.isSet(12))
            map.insert(TrackNumber, read<quint16>(stream));
        else
            map.appendError(mandatory("Track Number"));

        // I062/080 Track Status [M]
        if (mainMask.isSet(13))
        {
            quint8 data = read<quint8>(stream);

            // Leave this code for compatibility - obligatory.
            map.insert(TrackStatus_MON, data >> 7 & M1);
            map.insert(TrackStatus_SPI, data >> 6 & M1);
            map.insert(TrackStatus_MRH, data >> 5 & M1);
            map.insert(TrackStatus_SRC, data >> 2 & M3);
            map.insert(TrackStatus_CNF, data >> 1 & M1);

            TTrackStatusMap trackStatus;    // Use this map better
            trackStatus.insert(ETrackStatus_MON, data >> 7 & M1);
            trackStatus.insert(ETrackStatus_SPI, data >> 6 & M1);
            trackStatus.insert(ETrackStatus_MRH, data >> 5 & M1);
            trackStatus.insert(ETrackStatus_SRC, data >> 2 & M3);
            trackStatus.insert(ETrackStatus_CNF, data >> 1 & M1);

            int extentNumber = 1;
            while (data & M1)
            {
                data = read<quint8>(stream);
                if ( 1 == extentNumber )
                {
                    trackStatus.insert(ETrackStatus_SIM, data >> 7 & M1);
                    trackStatus.insert(ETrackStatus_TSE, data >> 6 & M1);
                    trackStatus.insert(ETrackStatus_TSB, data >> 5 & M1);
                    trackStatus.insert(ETrackStatus_FPC, data >> 4 & M1);
                    trackStatus.insert(ETrackStatus_AFF, data >> 3 & M1);
                    trackStatus.insert(ETrackStatus_STP, data >> 2 & M1);
                    trackStatus.insert(ETrackStatus_KOS, data >> 1 & M1);
                }
                else if ( 2 == extentNumber )
                {
                    trackStatus.insert(ETrackStatus_AMA, data >> 7 & M1);
                    trackStatus.insert(ETrackStatus_MD4, data >> 5 & M2);
                    trackStatus.insert(ETrackStatus_ME, data >> 4 & M1);
                    trackStatus.insert(ETrackStatus_MI, data >> 3 & M1);
                    trackStatus.insert(ETrackStatus_MD5, data >> 1 & M2);
                }
                else if ( 3 == extentNumber )
                {
                    trackStatus.insert(ETrackStatus_CST, data >> 7 & M1);
                    trackStatus.insert(ETrackStatus_PSR, data >> 6 & M1);
                    trackStatus.insert(ETrackStatus_SSR, data >> 5 & M1);
                    trackStatus.insert(ETrackStatus_MDS, data >> 4 & M1);
                    trackStatus.insert(ETrackStatus_ADS, data >> 3 & M1);
                    trackStatus.insert(ETrackStatus_SUC, data >> 2 & M1);
                    trackStatus.insert(ETrackStatus_AAC, data >> 1 & M1);
                }
                extentNumber++;
            }

            map.insert(TrackStatus, QVariant::fromValue(trackStatus));
        } else
            map.appendError(mandatory("Track Status"));

        // I062/290 System Track Update Ages [O]
        if (mainMask.isSet(14))
        {
            PresentData mask(stream);
            if (mask.isSet(1))
                map.insert(SystemTrackUpdateAges_TRK, double(read<quint8>(stream)) / 4); // Maximum value =63.75s
            if (mask.isSet(2))
                map.insert(SystemTrackUpdateAges_PSR, double(read<quint8>(stream)) / 4); // Maximum value =63.75s
            if (mask.isSet(3))
                map.insert(SystemTrackUpdateAges_SSR, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (mask.isSet(4))
                map.insert(SystemTrackUpdateAges_MDS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (mask.isSet(5))
                map.insert(SystemTrackUpdateAges_ADS, double(read<quint16>(stream)) / 4); // Max. value = 16383.75s
            if (mask.isSet(6))
                map.insert(SystemTrackUpdateAges_ES, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (mask.isSet(7))
                map.insert(SystemTrackUpdateAges_VDL, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (mask.isSet(8))
                map.insert(SystemTrackUpdateAges_UAT, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (mask.isSet(9))
                map.insert(SystemTrackUpdateAges_LOP, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (mask.isSet(10))
                map.insert(SystemTrackUpdateAges_MLT, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
        }

        // I062/200 Mode of Movement [O]
        if (mainMask.isSet(15))
        {
            quint8 data = read<quint8>(stream);
            map.insert(ModeOfMovement_TRANS, data >> 6 & M2);
            map.insert(ModeOfMovement_LONG, data >> 4 & M2);
            map.insert(ModeOfMovement_VERT, data >> 2 & M2);
            map.insert(ModeOfMovement_ADF, data >> 1 & M1);
        }

        // I062/295 Track Data Ages [O]
        if (mainMask.isSet(16))
        {
            PresentData trackMask(stream);
            if (trackMask.isSet(1))
                map.insert(TrackDataAges_FML, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(2))
                map.insert(TrackDataAges_MD1, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(3))
                map.insert(TrackDataAges_MD2, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(4))
                map.insert(TrackDataAges_MDA, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(5))
                map.insert(TrackDataAges_MD4, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(6))
                map.insert(TrackDataAges_MD5, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(7))
                map.insert(TrackDataAges_MHG, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(8))
                map.insert(TrackDataAges_IAS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(9))
                map.insert(TrackDataAges_TAS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(10))
                map.insert(TrackDataAges_SAL, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(11))
                map.insert(TrackDataAges_FSS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(12))
                map.insert(TrackDataAges_TID, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(13))
                map.insert(TrackDataAges_COM, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(14))
                map.insert(TrackDataAges_SAB, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(15))
                map.insert(TrackDataAges_ACS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(16))
                map.insert(TrackDataAges_BVR, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(17))
                map.insert(TrackDataAges_GVR, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(18))
                map.insert(TrackDataAges_RAN, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(19))
                map.insert(TrackDataAges_TAR, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(20))
                map.insert(TrackDataAges_TAN, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(21))
                map.insert(TrackDataAges_GSP, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(22))
                map.insert(TrackDataAges_VUN, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(23))
                map.insert(TrackDataAges_MET, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(24))
                map.insert(TrackDataAges_EMC, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(25))
                map.insert(TrackDataAges_POS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(26))
                map.insert(TrackDataAges_GAL, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(27))
                map.insert(TrackDataAges_PUN, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(28))
                map.insert(TrackDataAges_MB, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(29))
                map.insert(TrackDataAges_IAR, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(30))
                map.insert(TrackDataAges_MAC, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
            if (trackMask.isSet(31))
                map.insert(TrackDataAges_BPS, double(read<quint8>(stream)) / 4); // Maximum value = 63.75s
        }

        // I062/136 Measured Flight Level [O]
        if (mainMask.isSet(17))
            map.insert(MeasuredFlightLevel, double(read<qint16>(stream)) * 25.); // Vmin = -15 F; Vmax = 1500 FL

        // I062/130 Calculated Track Geometric Altitude [O]
        if (mainMask.isSet(18))
            map.insert(CalculatedTrackGeometricAltitude, double(read<qint16>(stream)) * 6.25); // Vmin = -1500 ft; Vmax = 150000 ft

        // I062/135 Calculated Track Barometric Altitude [O]
        if (mainMask.isSet(19))
        {
            qint16 data = read<qint16>(stream);
            map.insert(CalculatedTrackBarometricAltitude_QNH, data >> 15 & M1);
            map.insert(CalculatedTrackBarometricAltitude, double(restoreSign(data, 15)) / 4); // Vmin = -15 FL; Vmax = 1500 FL
        }

        // I062/220 Calculated Rate Of Climb/Descent [O]
        if (mainMask.isSet(20))
            map.insert(CalculatedRateOfClimbDescent, double(read<qint16>(stream)) * 6.25);

        // I062/390 Flight Plan Related Data [O]
        if (mainMask.isSet(21))
        {
            PresentData flightMask(stream);

            if (flightMask.isSet(1))
            {
                map.insert(FlightPlanRelatedData_TAG_SAC, read<quint8>(stream));
                map.insert(FlightPlanRelatedData_TAG_SIC, read<quint8>(stream));
            }

            if (flightMask.isSet(2))
                map.insert(FlightPlanRelatedData_CSN, readArray(stream, 7));

            if (flightMask.isSet(3))
            {
                quint32 data = read<quint32>(stream);
                map.insert(FlightPlanRelatedData_IFI_TYP, data >> 30 & M2);
                map.insert(FlightPlanRelatedData_IFI_NBR, data & M27); // Number from 0 to 99 999 999
            }

            if (flightMask.isSet(4))
            {
                quint8 data = read<quint8>(stream);
                map.insert(FlightPlanRelatedData_FCT_GAT_OAT, data >> 6 & M2);
                map.insert(FlightPlanRelatedData_FCT_FR1_FR2, data >> 4 & M2);
                map.insert(FlightPlanRelatedData_FCT_RVSM, data >> 2 & M2);
                map.insert(FlightPlanRelatedData_FCT_HPR, data >> 1 & M1);
            }

            if (flightMask.isSet(5))
                map.insert(FlightPlanRelatedData_TAC, readArray(stream, 4));

            if (flightMask.isSet(6))
                map.insert(FlightPlanRelatedData_WTC, read<quint8>(stream));

            if (flightMask.isSet(7))
                map.insert(FlightPlanRelatedData_DEP, readArray(stream, 4));

            if (flightMask.isSet(8))
                map.insert(FlightPlanRelatedData_DST, readArray(stream, 4));

            if (flightMask.isSet(9))
            {
                map.insert(FlightPlanRelatedData_RDS_NU1, read<quint8>(stream));
                map.insert(FlightPlanRelatedData_RDS_NU2, read<quint8>(stream));
                map.insert(FlightPlanRelatedData_RDS_LTR, read<quint8>(stream));
            }

            if (flightMask.isSet(10))
                map.insert(FlightPlanRelatedData_CFL, double(read<qint16>(stream)) / 4);

            if (flightMask.isSet(11))
            {
                map.insert(FlightPlanRelatedData_CTL_Centre, read<quint8>(stream));
                map.insert(FlightPlanRelatedData_CTL_Position, read<quint8>(stream));
            }

            if (flightMask.isSet(12))
            {
                map.insert(FlightPlanRelatedData_TOD_REP, read<qint8>(stream));
                quint8 data = read<quint8>(stream);
                map.insert(FlightPlanRelatedData_TOD_TYP, data >> 3 & M5); // 0 - 13
                map.insert(FlightPlanRelatedData_TOD_DAY, data >> 1 & M2);
                map.insert(FlightPlanRelatedData_TOD_HOR, read<quint8>(stream) & M5); // Hours, from 0 to 23
                map.insert(FlightPlanRelatedData_TOD_MIN, read<quint8>(stream) & M6); // Minutes, from 0 to 59
                data = read<quint8>(stream);
                if (data >> 7 & M1)
                    map.insert(FlightPlanRelatedData_TOD_SEC, data & M6); // Seconds, from 0 to 59
            }

            if (flightMask.isSet(13))
                map.insert(FlightPlanRelatedData_AST, readArray(stream, 6));

            if (flightMask.isSet(14))
            {
                quint8 data = read<quint8>(stream);
                map.insert(FlightPlanRelatedData_STS_EMP, data >> 6 & M2);
                map.insert(FlightPlanRelatedData_STS_AVL, data >> 4 & M2);
            }

            if (flightMask.isSet(15))
                map.insert(FlightPlanRelatedData_STD, readArray(stream, 7));

            if (flightMask.isSet(16))
                map.insert(FlightPlanRelatedData_STA, readArray(stream, 7));

            if (flightMask.isSet(17)) //SHOULD IT BE 12 variables or 1?
            {
                quint16 data = read<quint16>(stream);
                bool va = (data >> 12) & M1;
                map.insert(FlightPlanRelatedData_PEM_VA, va);
                if (va)
                    map.insert(FlightPlanRelatedData_PEM_reply, data & M12);
            }

            if (flightMask.isSet(18))
                map.insert(FlightPlanRelatedData_PEC, readArray(stream, 7));
        }

        // I062/270 Target Size & Orientation [O]
        if (mainMask.isSet(22))
        {
            qint8 data = read<qint8>(stream);
            map.insert(TargetSizeAndOrientation_Length, data >> 1 & M7);
            if (data & M1)
            {
                data = read<qint8>(stream);
                map.insert(TargetSizeAndOrientation_Orientation, double(data >> 1 & M7) * 360 / 128);
                if (data & M1)
                {
                    data = read<qint8>(stream);
                    map.insert(TargetSizeAndOrientation_Width, data >> 1 & M7);
                    while (data & M1)
                        data = read<qint8>(stream);
                }
            }
        }

        // I062/300 Vehicle Fleet Identification [O]
        if (mainMask.isSet(23))
            map.insert(VehicleFleetIdentification, read<quint8>(stream)); // 0 - 16

        // I062/110 Mode 5 Data reports & Extended Mode 1 Code [O]
        if (mainMask.isSet(24))
        {
            PresentData mask(stream);

            if (mask.isSet(1))
            {
                quint8 data = read<quint8>(stream);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_M5, data >> 7 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_ID, data >> 6 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_DA, data >> 5 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_M1, data >> 4 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_M2, data >> 3 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_M3, data >> 2 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_MC, data >> 1 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_SUM_X, data & M1);
            }

            if (mask.isSet(2))
            {
                map.insert(Mode5DataReportsAndExtendedMode1Code_PMN_PIN, read<quint16>(stream) & M14);
                map.insert(Mode5DataReportsAndExtendedMode1Code_PMN_NAT, read<quint8>(stream) & M6);
                map.insert(Mode5DataReportsAndExtendedMode1Code_PMN_MIS, read<quint8>(stream) & M6);
            }

            if (mask.isSet(3))
            {
                map.insert(Mode5DataReportsAndExtendedMode1Code_POS_Latitude, double(read24s(stream)) * 180 / 8388608); // Range -90 <= latitude <= 90 deg.
                map.insert(Mode5DataReportsAndExtendedMode1Code_POS_Longitude, double(read24s(stream)) * 180 / 8388608); // Range -180 <= longitude < 180 deg.
            }

            if (mask.isSet(4))
            {
                qint16 data = read<qint16>(stream);
                map.insert(Mode5DataReportsAndExtendedMode1Code_GA_RES, data >> 14 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_GA, double(restoreSign(data, 14)) * 25); // The minimum value of GA that can be reported is -1000 ft.
            }

            if (mask.isSet(5))
                map.insert(Mode5DataReportsAndExtendedMode1Code_EM1, read<quint16>(stream) & M12);

            if (mask.isSet(6))
                map.insert(Mode5DataReportsAndExtendedMode1Code_TOS, double(read<quint8>(stream)) / 128);

            if (mask.isSet(7))
            {
                quint8 data = read<quint8>(stream);
                map.insert(Mode5DataReportsAndExtendedMode1Code_XP_X5, data >> 4 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_XP_XC, data >> 3 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_XP_X3, data >> 2 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_XP_X2, data >> 1 & M1);
                map.insert(Mode5DataReportsAndExtendedMode1Code_XP_X1, data & M1);
            }
        }

        // I062/120 Track Mode 2 Code
        if (mainMask.isSet(25))
            map.insert(TrackMode2Code, read<quint16>(stream) & M12);

        // I062/510 Composed Track Number
        if (mainMask.isSet(26))
        {
            int i = 0;
            quint16 data;
            do
            {
                map.insert(QString(ComposedTrackNumber_SystemUnitIdentification).arg(i), read<quint8>(stream));
                data = read<quint16>(stream);
                map.insert(QString(ComposedTrackNumber_SystemTrackNumber).arg(i++), data >> 1 & M15);
            } while(data & M1);
        }

        // I062/500 Estimated Accuracies
        if (mainMask.isSet(27))
        {
            PresentData mask(stream);
            if (mask.isSet(1))
            {
                map.insert(EstimatedAccuracies_APC_X, double(read<qint16>(stream)) / 2);
                map.insert(EstimatedAccuracies_APC_Y, double(read<qint16>(stream)) / 2);
            }

            if (mask.isSet(2))
                map.insert(EstimatedAccuracies_COV, double(read<qint16>(stream)) / 2);

            if (mask.isSet(3))
            {
                map.insert(EstimatedAccuracies_APW_Latitude, double(read<qint16>(stream)) * 180 / 33554432);
                map.insert(EstimatedAccuracies_APW_Longitude, double(read<qint16>(stream)) * 180 / 33554432);
            }

            if (mask.isSet(4))
                map.insert(EstimatedAccuracies_AGA, double(read<qint8>(stream)) * 6.25);

            if (mask.isSet(5))
                map.insert(EstimatedAccuracies_ABA, double(read<qint8>(stream)) / 4);

            if (mask.isSet(6))
            {
                map.insert(EstimatedAccuracies_ATV_X, double(read<qint8>(stream)) / 4);
                map.insert(EstimatedAccuracies_ATV_Y, double(read<qint8>(stream)) / 4);
            }

            if (mask.isSet(7))
            {
                map.insert(EstimatedAccuracies_AA_X, double(read<qint8>(stream)) / 4);
                map.insert(EstimatedAccuracies_AA_Y, double(read<qint8>(stream)) / 4);
            }

            if (mask.isSet(8))
                map.insert(EstimatedAccuracies_ARC, double(read<qint8>(stream)) * 6.25);
        }

        // I062/340 Measured Information
        if (mainMask.isSet(28))
        {
            PresentData mask(stream);
            if (mask.isSet(1))
            {
                map.insert(MeasuredInformation_SID_SAC, read<quint8>(stream));
                map.insert(MeasuredInformation_SID_SIC, read<quint8>(stream));
            }

            if (mask.isSet(2))
            {
                map.insert(MeasuredInformation_POS_RHO, double(read<qint16>(stream)) / 256); // sign ??
                map.insert(MeasuredInformation_POS_THETA, double(read<qint16>(stream)) * 360 / 65536); // sign ??
            }

            if (mask.isSet(3))
                map.insert(MeasuredInformation_HEI, double(read<qint16>(stream)) * 25);

            if (mask.isSet(4))
            {
                qint16 data = read<qint16>(stream);
                map.insert(MeasuredInformation_MDC_V, data >> 15 & M1);
                map.insert(MeasuredInformation_MDC_G, data >> 14 & M1);
                map.insert(MeasuredInformation_MDC, double(restoreSign(data, 14)) / 4); // Vmin = -12 FL; Vmax = 1270 FL
            }

            if (mask.isSet(5))
            {
                quint16 data = read<quint16>(stream);
                map.insert(MeasuredInformation_MDA_V, data >> 15 & M1);
                map.insert(MeasuredInformation_MDA_G, data >> 14 & M1);
                map.insert(MeasuredInformation_MDA_L, data >> 13 & M1);
                map.insert(MeasuredInformation_MDA, data & M12);
            }

            if (mask.isSet(6))
            {
                quint8 data = read<quint8>(stream);
                map.insert(MeasuredInformation_TYP, data >> 5 & M3);
                map.insert(MeasuredInformation_TYP_SIM, data >> 4 & M1);
                map.insert(MeasuredInformation_TYP_RAB, data >> 3 & M1);
                map.insert(MeasuredInformation_TYP_TST, data >> 2 & M1);
            }
        }

        if (mainMask.isSet(34))
            parse_re(stream, map);

        if (mainMask.isSet(35))
            parse_sp(stream, map);
    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
    catch (...)
    {
        qCritical() << "Unknown exception:" << Q_FUNC_INFO;
    }
}

QByteArray Asterix062Proto::generate(AsterixData &map)
{
    QByteArray result;
    PresentData mainMask;
    try
    {
        QDataStream stream(&result, QIODevice::WriteOnly);

        // 062/010 Data Source Identifier [M]
        if (map.contains(DataSourceIdentifier_SAC)
            && map.contains(DataSourceIdentifier_SIC))
            {
            mainMask.set(1);
            write8(stream, map.value(DataSourceIdentifier_SAC).toUInt());
            write8(stream, map.value(DataSourceIdentifier_SIC).toUInt());
        } else
            map.appendError(mandatory("Data Source Identifier"));

        // I062/015 Service Identification [O]
        if (map.contains(ServiceIdentification))
        {
            mainMask.set(3);
            write8(stream, map.value(ServiceIdentification).toUInt());
        }

        // I062/070 Time Of Track Information [M]
        if (map.contains(TimeOfTrackInformation))
        {
            mainMask.set(4);
            write24(stream, double(map.value(TimeOfTrackInformation).toTime().msecsTo(QTime(0,0,0,0))) * -128 / 1000);
        }
        else
            map.appendError(mandatory("Time Of Track Information"));

        // I062/105 Calculated Track Position (WGS-84) [O]
        if (map.contains(CalculatedTrackPositionWGS_Latitude)
            && map.contains(CalculatedTrackPositionWGS_Longitude))
            {
            mainMask.set(5);
            write32(stream, map.value(CalculatedTrackPositionWGS_Latitude).toDouble() * 33554432 / 180);
            write32(stream, map.value(CalculatedTrackPositionWGS_Longitude).toDouble() * 33554432 / 180);
        }

        // I062/100 Calculated Track Position (Cartesian) [O]
        if (map.contains(CalculatedTrackPositionCartesian_x)
            && map.contains(CalculatedTrackPositionCartesian_y))
            {
            mainMask.set(6);
            write24(stream, map.value(CalculatedTrackPositionCartesian_x).toDouble() * 2);
            write24(stream, map.value(CalculatedTrackPositionCartesian_y).toDouble() * 2);
        }

        // I062/185 Calculated Track Velocity (Cartesian) [O]
        if (map.contains(CalculatedTrackVelocityCartesian_Vx)
            && map.contains(CalculatedTrackVelocityCartesian_Vy))
            {
            mainMask.set(7);
            write16(stream, map.value(CalculatedTrackVelocityCartesian_Vx).toDouble() * 4);
            write16(stream, map.value(CalculatedTrackVelocityCartesian_Vy).toDouble() * 4);
        }

        // I062/210 Calculated Acceleration (Cartesian) [O]
        if (map.contains(CalculatedAccelerationCartesian_Ax)
            && map.contains(CalculatedAccelerationCartesian_Ay))
            {
            mainMask.set(8);
            write8(stream, map.value(CalculatedAccelerationCartesian_Ax).toDouble() * 4);
            write8(stream, map.value(CalculatedAccelerationCartesian_Ay).toDouble() * 4);
        }

        // I062/060 Track Mode 3/A Code [O]
        if (map.contains(TrackMode3ACode_hasChanged)
            && map.contains(TrackMode3ACode_reply))
            {
            mainMask.set(9);
            quint16 data = (map.value(TrackMode3ACode_hasChanged).toUInt() & M1) << 13;
            data |= (map.value(TrackMode3ACode_reply).toUInt() & M12);
            write16(stream, data);
        }

        // I062/245 Target Identification [O]
        if (map.contains(TargetIdentification_STI) &&
            map.contains(TargetIdentification_Character1) &&
            map.contains(TargetIdentification_Character2) &&
            map.contains(TargetIdentification_Character3) &&
            map.contains(TargetIdentification_Character4) &&
            map.contains(TargetIdentification_Character5) &&
            map.contains(TargetIdentification_Character6) &&
            map.contains(TargetIdentification_Character7) &&
            map.contains(TargetIdentification_Character8))
        {
            mainMask.set(10);
            write8(stream, (map.value(TargetIdentification_STI).toUInt() & M2) << 6);
            quint32 data = (map.value(TargetIdentification_Character1).toUInt() & M6) << 18;
            data |= (map.value(TargetIdentification_Character2).toUInt() & M6) << 12;
            data |= (map.value(TargetIdentification_Character3).toUInt() & M6) << 6;
            data |= map.value(TargetIdentification_Character4).toUInt() & M6;
            write24(stream, data);
            data = (map.value(TargetIdentification_Character5).toUInt() & M6) << 18;
            data |= (map.value(TargetIdentification_Character6).toUInt() & M6) << 12;
            data |= (map.value(TargetIdentification_Character7).toUInt() & M6) << 6;
            data |= map.value(TargetIdentification_Character8).toUInt() & M6;
            write24(stream, data);
        }

        // I062/380 Aircraft Derived Data [O]
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(AircraftDerivedData_ADR)) // Target Address
            {
                subMask.set(1);
                write24(subStream, map.value(AircraftDerivedData_ADR).toUInt());
            }

            if (map.contains(AircraftDerivedData_ID_Character1)
                && map.contains(AircraftDerivedData_ID_Character2)
                && map.contains(AircraftDerivedData_ID_Character3)
                && map.contains(AircraftDerivedData_ID_Character4)
                && map.contains(AircraftDerivedData_ID_Character5)
                && map.contains(AircraftDerivedData_ID_Character6)
                && map.contains(AircraftDerivedData_ID_Character7)
                && map.contains(AircraftDerivedData_ID_Character8)) // Target Identification
                {
                subMask.set(2);
                quint32 data = (map.value(AircraftDerivedData_ID_Character1).toUInt() & M6) << 18;
                data |= (map.value(AircraftDerivedData_ID_Character2).toUInt() & M6) << 12;
                data |= (map.value(AircraftDerivedData_ID_Character3).toUInt() & M6) << 6;
                data |= map.value(AircraftDerivedData_ID_Character4).toUInt() & M6;
                write24(subStream, data);
                data = (map.value(AircraftDerivedData_ID_Character5).toUInt() & M6) << 18;
                data |= (map.value(AircraftDerivedData_ID_Character6).toUInt() & M6) << 12;
                data |= (map.value(AircraftDerivedData_ID_Character7).toUInt() & M6) << 6;
                data |= map.value(AircraftDerivedData_ID_Character8).toUInt() & M6;
                write24(subStream, data);
            }

            if (map.contains(AircraftDerivedData_MHG)) // Magnetic Heading
            {
                subMask.set(3);
                write16(subStream, map.value(AircraftDerivedData_MHG).toDouble() * 65536 / 360);
            }

            if (map.contains(AircraftDerivedData_IAS_IM)
                && map.contains(AircraftDerivedData_IAS)) // Indicated Airspeed/Mach Number
                {
                subMask.set(4);
                qint16 data = (map.value(AircraftDerivedData_IAS_IM).toUInt() & M1) << 15;
                data |= qRound(map.value(AircraftDerivedData_IAS).toDouble() * (data ? 1000 : 16384));
                write16(subStream, data);
            }

            if (map.contains(AircraftDerivedData_TAS)) // True Airspeed
            {
                subMask.set(5);
                write16(subStream, map.value(AircraftDerivedData_TAS).toUInt());
            }

            if (map.contains(AircraftDerivedData_SAL_SAS)
                && map.contains(AircraftDerivedData_SAL_Source)
                && map.contains(AircraftDerivedData_SAL_Altitude)) // Selected Altitude
                {
                subMask.set(6);
                qint16 data = (map.value(AircraftDerivedData_SAL_SAS).toUInt() & M1) << 15;
                data |= (map.value(AircraftDerivedData_SAL_Source).toUInt() & M2) << 13;
                data |= qint16(map.value(AircraftDerivedData_SAL_Altitude).toDouble() / 25) & M13;
                write16(subStream, data);
            }

            if (map.contains(AircraftDerivedData_FSS_MV)
                && map.contains(AircraftDerivedData_FSS_AH)
                && map.contains(AircraftDerivedData_FSS_AM)
                && map.contains(AircraftDerivedData_FSS_Altitude)) // Final State SelectedAltitude
                {
                subMask.set(7);
                quint16 data = (map.value(AircraftDerivedData_FSS_MV).toUInt() & M1) << 15;
                data |= (map.value(AircraftDerivedData_FSS_AH).toUInt() & M1) << 14;
                data |= (map.value(AircraftDerivedData_FSS_AM).toUInt() & M1) << 13;
                data |= quint16(map.value(AircraftDerivedData_FSS_Altitude).toDouble() / 25) & M13;
                write16(subStream, data);
            }

            if (map.contains(AircraftDerivedData_TIS_NAV)
                && map.contains(AircraftDerivedData_TIS_NVB)) // Trajectory Intent Status
                {
                subMask.set(8);
                quint8 data = (map.value(AircraftDerivedData_TIS_NAV).toUInt() & M1) << 7;
                data |= (map.value(AircraftDerivedData_TIS_NVB).toUInt() & M1) << 6;
                write8(subStream, data);
            }

            if (map.contains(AircraftDerivedData_TID_REP) && map.contains(AircraftDerivedData_TID_TCA)
                && map.contains(AircraftDerivedData_TID_NC) && map.contains(AircraftDerivedData_TID_TCP)
                && map.contains(AircraftDerivedData_TID_Altitude) && map.contains(AircraftDerivedData_TID_REP_Latitude)
                && map.contains(AircraftDerivedData_TID_REP_Longitude) && map.contains(AircraftDerivedData_TID_REP_PointType)
                && map.contains(AircraftDerivedData_TID_REP_TD) && map.contains(AircraftDerivedData_TID_REP_TRA)
                && map.contains(AircraftDerivedData_TID_REP_TOA) && map.contains(AircraftDerivedData_TID_REP_TOV) && map.contains(AircraftDerivedData_TID_REP_TTR)) // Trajectory Intent Data
                {
                subMask.set(9);
                write8(subStream, map.value(AircraftDerivedData_TID_REP).toInt());
                quint8 data = (map.value(AircraftDerivedData_TID_TCA).toUInt() & M1) << 7;
                data |= (map.value(AircraftDerivedData_TID_NC).toUInt() & M1) << 6;
                data |= (map.value(AircraftDerivedData_TID_TCP).toUInt() & M6);
                write8(subStream, data);
                write16(subStream, map.value(AircraftDerivedData_TID_Altitude).toDouble() / 10);
                write24(subStream, map.value(AircraftDerivedData_TID_REP_Latitude).toDouble() * 8388608 / 180);
                write24(subStream, map.value(AircraftDerivedData_TID_REP_Longitude).toDouble() * 8388608 / 180);
                data = (map.value(AircraftDerivedData_TID_REP_PointType).toUInt() & M4) << 4;
                data |= (map.value(AircraftDerivedData_TID_REP_TD).toUInt() & M2) << 2;
                data |= (map.value(AircraftDerivedData_TID_REP_TRA).toUInt() & M1) << 1;
                data |= map.value(AircraftDerivedData_TID_REP_TOA).toUInt() & M1;
                write8(subStream, data);
                write24(subStream, map.value(AircraftDerivedData_TID_REP_TOV).toUInt());
                write16(subStream, map.value(AircraftDerivedData_TID_REP_TTR).toDouble() * 100);
            }

            if (map.contains(AircraftDerivedData_COM)
                && map.contains(AircraftDerivedData_COM_STAT)
                && map.contains(AircraftDerivedData_COM_SSC)
                && map.contains(AircraftDerivedData_COM_ARC)
                && map.contains(AircraftDerivedData_COM_AIC)
                && map.contains(AircraftDerivedData_COM_B1A)
                && map.contains(AircraftDerivedData_COM_B1B)) // Communications / ACAS; Capability and Flight Status
                {
                subMask.set(10);
                quint16 data = (map.value(AircraftDerivedData_COM).toUInt() & M3) << 13;
                data |= (map.value(AircraftDerivedData_COM_STAT).toUInt() & M3) << 10;
                data |= (map.value(AircraftDerivedData_COM_SSC).toUInt() & M1) << 7;
                data |= (map.value(AircraftDerivedData_COM_ARC).toUInt() & M1) << 6;
                data |= (map.value(AircraftDerivedData_COM_AIC).toUInt() & M1) << 5;
                data |= (map.value(AircraftDerivedData_COM_B1A).toUInt() & M1) << 4;
                data |= map.value(AircraftDerivedData_COM_B1B).toUInt() & M4;
                write16(subStream, data);
            }

            if (map.contains(AircraftDerivedData_SAB_AC)
                && map.contains(AircraftDerivedData_SAB_MN)
                && map.contains(AircraftDerivedData_SAB_DC)
                && map.contains(AircraftDerivedData_SAB_GBS)
                && map.contains(AircraftDerivedData_SAB_STAT)) // Status reported by ADS-B
                {
                subMask.set(11);
                quint16 data = (map.value(AircraftDerivedData_SAB_AC).toUInt() & M2) << 14;
                data |= (map.value(AircraftDerivedData_SAB_MN).toUInt() & M2) << 12;
                data |= (map.value(AircraftDerivedData_SAB_DC).toUInt() & M2) << 10;
                data |= (map.value(AircraftDerivedData_SAB_GBS).toUInt() & M1) << 9;
                data |= map.value(AircraftDerivedData_SAB_STAT).toUInt() & M3;
                write16(subStream, data);
            }

            if (map.contains(AircraftDerivedData_ACS)) // ACAS Resolution Advisory Report
            {
                QByteArray data = map.value(AircraftDerivedData_ACS).toByteArray();
                if (7 == data.size())
                {
                    subMask.set(12);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(AircraftDerivedData_BVR)) // Barometric Vertical Rate
            {
                subMask.set(13);
                write16(subStream, map.value(AircraftDerivedData_BVR).toDouble() / 6.25);
            }

            if (map.contains(AircraftDerivedData_GVR)) // Geometric Vertical Rate
            {
                subMask.set(14);
                write16(subStream, map.value(AircraftDerivedData_GVR).toDouble() / 6.25);
            }

            if (map.contains(AircraftDerivedData_RAN)) // Roll Angle
            {
                subMask.set(15);
                write16(subStream, map.value(AircraftDerivedData_RAN).toDouble() * 100);
            }

            if (map.contains(AircraftDerivedData_TAR_TI)
                && map.contains(AircraftDerivedData_TAR_RateOfTurn)) // Track Angle Rate
                {
                subMask.set(16);
                qint16 data = (map.value(AircraftDerivedData_TAR_TI).toInt() & M2) << 14;
                data |= (qint16(map.value(AircraftDerivedData_TAR_RateOfTurn).toDouble() * 4) & M7) << 1;
                write16(subStream, data);
            }

            if (map.contains(AircraftDerivedData_TAN)) // Track Angle
            {
                subMask.set(17);
                write16(subStream, map.value(AircraftDerivedData_TAN).toDouble() * 65536 / 360);
            }

            if (map.contains(AircraftDerivedData_GSP)) // Ground Speed
            {
                subMask.set(18);
                write16(subStream, map.value(AircraftDerivedData_GSP).toDouble() * 16384);
            }

            if (map.contains(AircraftDerivedData_VUN)) // Velocity Uncertainty
            {
                subMask.set(19);
                write8(subStream, map.value(AircraftDerivedData_VUN).toInt());
            }

            if (map.contains(AircraftDerivedData_MET_WS)
                && map.contains(AircraftDerivedData_MET_WD)
                && map.contains(AircraftDerivedData_MET_TMP)
                && map.contains(AircraftDerivedData_MET_TRB)
                && map.contains(AircraftDerivedData_MET_WindSpeed)
                && map.contains(AircraftDerivedData_MET_WindDirection)
                && map.contains(AircraftDerivedData_MET_Temperature)
                && map.contains(AircraftDerivedData_MET_Turbulence)) // Meteorological Data
                {
                subMask.set(20);
                quint8 data = (map.value(AircraftDerivedData_MET_WS).toUInt() & M1) << 7;
                data |= (map.value(AircraftDerivedData_MET_WD).toUInt() & M1) << 6;
                data |= (map.value(AircraftDerivedData_MET_TMP).toUInt() & M1) << 5;
                data |= (map.value(AircraftDerivedData_MET_TRB).toUInt() & M1) << 4;
                write8(subStream, data);
                write16(subStream, map.value(AircraftDerivedData_MET_WindSpeed).toUInt());
                write16(subStream, map.value(AircraftDerivedData_MET_WindDirection).toUInt());
                write16(subStream, map.value(AircraftDerivedData_MET_Temperature).toDouble() / 0.25);
                write8(subStream, map.value(AircraftDerivedData_MET_Turbulence).toUInt());
            }

            if (map.contains(AircraftDerivedData_EMC)) // Emitter Category
            {
                subMask.set(21);
                write8(subStream, map.value(AircraftDerivedData_EMC).toUInt());
            }

            if (map.contains(AircraftDerivedData_POS_Latitude)
                && map.contains(AircraftDerivedData_POS_Longitude)) // Position Data
                {
                subMask.set(22);
                write24(subStream, map.value(AircraftDerivedData_POS_Latitude).toDouble() * 8388608 / 180);
                write24(subStream, map.value(AircraftDerivedData_POS_Longitude).toDouble() * 8388608 / 180);
            }

            if (map.contains(AircraftDerivedData_GAL)) // Geometric Altitude Data
            {
                subMask.set(23);
                write16(subStream, map.value(AircraftDerivedData_GAL).toDouble() / 6.25);
            }

            if (map.contains(AircraftDerivedData_PUN)) // Position Uncertainty Data
            {
                subMask.set(24);
                write8(subStream, map.value(AircraftDerivedData_PUN).toUInt() & M4);
            }

            if (map.contains(AircraftDerivedData_MB_REP)
                && map.contains(AircraftDerivedData_MB_data)
                && map.contains(AircraftDerivedData_MB_BDS1)
                && map.contains(AircraftDerivedData_MB_BDS2)) // Mode S MB Data
                {
                QByteArray dataArray = map.value(AircraftDerivedData_MB_data).toByteArray();
                if (7 == dataArray.size())
                {
                    subMask.set(25);
                    write8(subStream, map.value(AircraftDerivedData_MB_REP).toInt());
                    writeArray(subStream, dataArray);
                    quint8 data = (map.value(AircraftDerivedData_MB_BDS1).toUInt() & M4) << 4;
                    data |= map.value(AircraftDerivedData_MB_BDS2).toUInt() & M4;
                    write8(subStream, data);
                }
            }

            if (map.contains(AircraftDerivedData_IAR)) // Indicated Airspeed
            {
                subMask.set(26);
                write16(subStream, map.value(AircraftDerivedData_IAR).toUInt());
            }

            if (map.contains(AircraftDerivedData_MAC)) // Mach Number
            {
                subMask.set(27);
                write16(subStream, map.value(AircraftDerivedData_MAC).toDouble() / 0.008);
            }

            if (map.contains(AircraftDerivedData_BPS)) // Barometric Pressure Setting
            {
                subMask.set(28);
                write16(subStream, quint16(map.value(AircraftDerivedData_BPS).toDouble() * 10) & M12);
            }

            if (!subMask.isEmpty()) {
                mainMask.set(11);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }
        }

        // I062/040 Track Number [M]
        if (map.contains(TrackNumber)) {
            mainMask.set(12);
            write16(stream, map.value(TrackNumber).toUInt());
        }
        else
            map.appendError(mandatory("Track Number"));

        // I062/080 Track Status [M]
        if (map.contains(TrackStatus)
                || (map.contains(TrackStatus_MON) && map.contains(TrackStatus_SPI) && map.contains(TrackStatus_MRH)
                    && map.contains(TrackStatus_SRC) && map.contains(TrackStatus_CNF))) {
            mainMask.set(13);
            // Leave for compatibility with old code - obligatory.
            quint8 data = (map.value(TrackStatus_MON).toUInt() & M1) << 7;
            data |= (map.value(TrackStatus_SPI).toUInt() & M1) << 6;
            data |= (map.value(TrackStatus_MRH).toUInt() & M1) << 5;
            data |= (map.value(TrackStatus_SRC).toUInt() & M3) << 2;
            data |= (map.value(TrackStatus_CNF).toUInt() & M1) << 1;

            quint8 firstData, secondData, thirdData;
            bool isFirstExtent = false, isSecondExtent = false, isThirdExtent = false;
            // New implementation.
            if ( map.contains(TrackStatus) )
            {
                TTrackStatusMap trackStatus = map.value(TrackStatus).value<TTrackStatusMap>();
                data = (trackStatus.value(ETrackStatus_MON) & M1) << 7;
                data |= (trackStatus.value(ETrackStatus_SPI) & M1) << 6;
                data |= (trackStatus.value(ETrackStatus_MRH) & M1) << 5;
                data |= (trackStatus.value(ETrackStatus_SRC) & M3) << 2;
                data |= (trackStatus.value(ETrackStatus_CNF) & M1) << 1;

                isFirstExtent = trackStatus.contains(ETrackStatus_SIM) || trackStatus.contains(ETrackStatus_TSE)
                        || trackStatus.contains(ETrackStatus_TSB) || trackStatus.contains(ETrackStatus_FPC)
                        || trackStatus.contains(ETrackStatus_AFF) || trackStatus.contains(ETrackStatus_STP)
                        || trackStatus.contains(ETrackStatus_KOS);
                isSecondExtent = trackStatus.contains(ETrackStatus_AMA) || trackStatus.contains(ETrackStatus_MD4)
                        || trackStatus.contains(ETrackStatus_ME) || trackStatus.contains(ETrackStatus_MI)
                        || trackStatus.contains(ETrackStatus_MD5);
                isThirdExtent = trackStatus.contains(ETrackStatus_CST) || trackStatus.contains(ETrackStatus_PSR)
                        || trackStatus.contains(ETrackStatus_SSR) || trackStatus.contains(ETrackStatus_MDS)
                        || trackStatus.contains(ETrackStatus_ADS) || trackStatus.contains(ETrackStatus_SUC)
                        || trackStatus.contains(ETrackStatus_AAC);
                if ( isFirstExtent )
                {
                    data |= M1;

                    firstData = (trackStatus.value(ETrackStatus_SIM) & M1) << 7;
                    firstData |= (trackStatus.value(ETrackStatus_TSE) & M1) << 6;
                    firstData |= (trackStatus.value(ETrackStatus_TSB) & M1) << 5;
                    firstData |= (trackStatus.value(ETrackStatus_FPC) & M1) << 4;
                    firstData |= (trackStatus.value(ETrackStatus_AFF) & M1) << 3;
                    firstData |= (trackStatus.value(ETrackStatus_STP) & M1) << 2;
                    firstData |= (trackStatus.value(ETrackStatus_KOS) & M1) << 1;
                }
                if ( isFirstExtent && isSecondExtent )
                {
                    firstData |= M1;

                    secondData = (trackStatus.value(ETrackStatus_AMA) & M1) << 7;
                    secondData |= (trackStatus.value(ETrackStatus_MD4) & M2) << 5;
                    secondData |= (trackStatus.value(ETrackStatus_ME) & M1) << 4;
                    secondData |= (trackStatus.value(ETrackStatus_MI) & M1) << 3;
                    secondData |= (trackStatus.value(ETrackStatus_MD5) & M2) << 1;
                }
                if ( isFirstExtent && isSecondExtent && isThirdExtent )
                {
                    secondData |= M1;

                    thirdData = (trackStatus.value(ETrackStatus_CST) & M1) << 7;
                    thirdData |= (trackStatus.value(ETrackStatus_PSR) & M1) << 6;
                    thirdData |= (trackStatus.value(ETrackStatus_SSR) & M1) << 5;
                    thirdData |= (trackStatus.value(ETrackStatus_MDS) & M1) << 4;
                    thirdData |= (trackStatus.value(ETrackStatus_ADS) & M1) << 3;
                    thirdData |= (trackStatus.value(ETrackStatus_SUC) & M1) << 2;
                    thirdData |= (trackStatus.value(ETrackStatus_AAC) & M1) << 1;
                }
            }

            write8(stream, data);
            if ( isFirstExtent )
                write8(stream, firstData);
            if ( isFirstExtent && isSecondExtent )
                write8(stream, secondData);
            if ( isFirstExtent && isSecondExtent && isThirdExtent )
                write8(stream, thirdData);
        } else
            map.appendError(mandatory("Track Status"));

        // I062/290 System Track Update Ages [O]
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(SystemTrackUpdateAges_TRK)) {
                subMask.set(1);
                write8(subStream, map.value(SystemTrackUpdateAges_TRK).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_PSR)) {
                subMask.set(2);
                write8(subStream, map.value(SystemTrackUpdateAges_PSR).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_SSR)) {
                subMask.set(3);
                write8(subStream, map.value(SystemTrackUpdateAges_SSR).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_MDS)) {
                subMask.set(4);
                write8(subStream, map.value(SystemTrackUpdateAges_MDS).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_ADS)) {
                subMask.set(5);
                write16(subStream, map.value(SystemTrackUpdateAges_ADS).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_ES)) {
                subMask.set(6);
                write8(subStream, map.value(SystemTrackUpdateAges_ES).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_VDL)) {
                subMask.set(7);
                write8(subStream, map.value(SystemTrackUpdateAges_VDL).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_UAT)) {
                subMask.set(8);
                write8(subStream, map.value(SystemTrackUpdateAges_UAT).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_LOP)) {
                subMask.set(9);
                write8(subStream, map.value(SystemTrackUpdateAges_LOP).toDouble() * 4);
            }
            if (map.contains(SystemTrackUpdateAges_MLT)) {
                subMask.set(10);
                write8(subStream, map.value(SystemTrackUpdateAges_MLT).toDouble() * 4);
            }

            if (!subMask.isEmpty()) {
                mainMask.set(14);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }
        }

        // I062/200 Mode of Movement [O]
        if (map.contains(ModeOfMovement_TRANS) && map.contains(ModeOfMovement_LONG) && map.contains(ModeOfMovement_VERT) && map.contains(ModeOfMovement_ADF)) {
            mainMask.set(15);
            quint8 data = (map.value(ModeOfMovement_TRANS).toUInt() & M2) << 6;
            data |= (map.value(ModeOfMovement_LONG).toUInt() & M2) << 4;
            data |= (map.value(ModeOfMovement_VERT).toUInt() & M2) << 2;
            data |= (map.value(ModeOfMovement_ADF).toUInt() & M1) << 1;
            write8(stream, data);
        }

        // I062/295 Track Data Ages [O]
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(TrackDataAges_FML)) {
                subMask.set(1);
                write8(subStream, map.value(TrackDataAges_FML).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MD1)) {
                subMask.set(2);
                write8(subStream, map.value(TrackDataAges_MD1).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MD2)) {
                subMask.set(3);
                write8(subStream, map.value(TrackDataAges_MD2).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MDA)) {
                subMask.set(4);
                write8(subStream, map.value(TrackDataAges_MDA).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MD4)) {
                subMask.set(5);
                write8(subStream, map.value(TrackDataAges_MD4).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MD5)) {
                subMask.set(6);
                write8(subStream, map.value(TrackDataAges_MD5).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MHG)) {
                subMask.set(7);
                write8(subStream, map.value(TrackDataAges_MHG).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_IAS)) {
                subMask.set(8);
                write8(subStream, map.value(TrackDataAges_IAS).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_TAS)) {
                subMask.set(9);
                write8(subStream, map.value(TrackDataAges_TAS).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_SAL)) {
                subMask.set(10);
                write8(subStream, map.value(TrackDataAges_SAL).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_FSS)) {
                subMask.set(11);
                write8(subStream, map.value(TrackDataAges_FSS).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_TID)) {
                subMask.set(12);
                write8(subStream, map.value(TrackDataAges_TID).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_COM)) {
                subMask.set(13);
                write8(subStream, map.value(TrackDataAges_COM).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_SAB)) {
                subMask.set(14);
                write8(subStream, map.value(TrackDataAges_SAB).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_ACS)) {
                subMask.set(15);
                write8(subStream, map.value(TrackDataAges_ACS).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_BVR)) {
                subMask.set(16);
                write8(subStream, map.value(TrackDataAges_BVR).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_GVR)) {
                subMask.set(17);
                write8(subStream, map.value(TrackDataAges_GVR).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_RAN)) {
                subMask.set(18);
                write8(subStream, map.value(TrackDataAges_RAN).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_TAR)) {
                subMask.set(19);
                write8(subStream, map.value(TrackDataAges_TAR).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_TAN)) {
                subMask.set(20);
                write8(subStream, map.value(TrackDataAges_TAN).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_GSP)) {
                subMask.set(21);
                write8(subStream, map.value(TrackDataAges_GSP).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_VUN)) {
                subMask.set(22);
                write8(subStream, map.value(TrackDataAges_VUN).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MET)) {
                subMask.set(23);
                write8(subStream, map.value(TrackDataAges_MET).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_EMC)) {
                subMask.set(24);
                write8(subStream, map.value(TrackDataAges_EMC).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_POS)) {
                subMask.set(25);
                write8(subStream, map.value(TrackDataAges_POS).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_GAL)) {
                subMask.set(26);
                write8(subStream, map.value(TrackDataAges_GAL).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_PUN)) {
                subMask.set(27);
                write8(subStream, map.value(TrackDataAges_PUN).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MB)) {
                subMask.set(28);
                write8(subStream, map.value(TrackDataAges_MB).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_IAR)) {
                subMask.set(29);
                write8(subStream, map.value(TrackDataAges_IAR).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_MAC)) {
                subMask.set(30);
                write8(subStream, map.value(TrackDataAges_MAC).toDouble() * 4);
            }
            if (map.contains(TrackDataAges_BPS)) {
                subMask.set(31);
                write8(subStream, map.value(TrackDataAges_BPS).toDouble() * 4);
            }

            if (!subMask.isEmpty()) {
                mainMask.set(16);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }
        }

        // I062/136 Measured Flight Level [O]
        if (map.contains(MeasuredFlightLevel)) {
            mainMask.set(17);
            write16(stream, map.value(MeasuredFlightLevel).toDouble() / 25.);
        }

        // I062/130 Calculated Track Geometric Altitude [O]
        if (map.contains(CalculatedTrackGeometricAltitude)) {
            mainMask.set(18);
            write16(stream, map.value(CalculatedTrackGeometricAltitude).toDouble() / 6.25);
        }

        // I062/135 Calculated Track Barometric Altitude [O]
        if (map.contains(CalculatedTrackBarometricAltitude_QNH) && map.contains(CalculatedTrackBarometricAltitude)) {
            mainMask.set(19);
            quint16 data = (map.value(CalculatedTrackBarometricAltitude_QNH).toUInt() & M1 ) << 15;
            data |= quint16(map.value(CalculatedTrackBarometricAltitude).toDouble() * 4) & M15;
            write16(stream, data);
        }

        // I062/220 Calculated Rate Of Climb/Descent [O]
        if (map.contains(CalculatedRateOfClimbDescent)) {
            mainMask.set(20);
            write16(stream, map.value(CalculatedRateOfClimbDescent).toDouble() / 6.25);
        }

        // I062/390 Flight Plan Related Data [O]
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(FlightPlanRelatedData_TAG_SAC) && map.contains(FlightPlanRelatedData_TAG_SIC)) {
                subMask.set(1);
                write8(subStream, map.value(FlightPlanRelatedData_TAG_SAC).toUInt());
                write8(subStream, map.value(FlightPlanRelatedData_TAG_SIC).toUInt());
            }

            if (map.contains(FlightPlanRelatedData_CSN)) {
                QByteArray data = map.value(FlightPlanRelatedData_CSN).toByteArray();
                if (7 == data.size()) {
                    subMask.set(2);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_IFI_TYP) && map.contains(FlightPlanRelatedData_IFI_NBR)) {
                subMask.set(3);
                quint32 data = (map.value(FlightPlanRelatedData_IFI_TYP).toUInt() & M2 ) << 30;
                data |= map.value(FlightPlanRelatedData_IFI_NBR).toUInt() & M27;
                write32(subStream, data);
            }

            if (map.contains(FlightPlanRelatedData_FCT_GAT_OAT) && map.contains(FlightPlanRelatedData_FCT_FR1_FR2)
                && map.contains(FlightPlanRelatedData_FCT_RVSM) && map.contains(FlightPlanRelatedData_FCT_HPR)) {
                subMask.set(4);
                quint8 data = (map.value(FlightPlanRelatedData_FCT_GAT_OAT).toUInt() & M2) << 6;
                data |= (map.value(FlightPlanRelatedData_FCT_FR1_FR2).toUInt() & M2) << 4;
                data |= (map.value(FlightPlanRelatedData_FCT_RVSM).toUInt() & M2) << 2;
                data |= (map.value(FlightPlanRelatedData_FCT_HPR).toUInt() & M1) << 1;
                write8(subStream, data);
            }

            if (map.contains(FlightPlanRelatedData_TAC)) {
                QByteArray data = map.value(FlightPlanRelatedData_TAC).toByteArray();
                if (4 == data.size()) {
                    subMask.set(5);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_WTC)) {
                subMask.set(6);
                write8(subStream, map.value(FlightPlanRelatedData_WTC).toUInt());
            }

            if (map.contains(FlightPlanRelatedData_DEP)) {
                QByteArray data = map.value(FlightPlanRelatedData_DEP).toByteArray();
                if (4 == data.size()) {
                    subMask.set(7);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_DST)) {
                QByteArray data = map.value(FlightPlanRelatedData_DST).toByteArray();
                if (4 == data.size()) {
                    subMask.set(8);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_RDS_NU1) && map.contains(FlightPlanRelatedData_RDS_NU2) && map.contains(FlightPlanRelatedData_RDS_LTR)) {
                subMask.set(9);
                write8(subStream, map.value(FlightPlanRelatedData_RDS_NU1).toUInt());
                write8(subStream, map.value(FlightPlanRelatedData_RDS_NU2).toUInt());
                write8(subStream, map.value(FlightPlanRelatedData_RDS_LTR).toUInt());
            }

            if (map.contains(FlightPlanRelatedData_CFL)) {
                subMask.set(10);
                write16(subStream, map.value(FlightPlanRelatedData_CFL).toDouble() * 4);
            }

            if (map.contains(FlightPlanRelatedData_CTL_Centre) && map.contains(FlightPlanRelatedData_CTL_Position)) {
                subMask.set(11);
                write8(subStream, map.value(FlightPlanRelatedData_CTL_Centre).toUInt());
                write8(subStream, map.value(FlightPlanRelatedData_CTL_Position).toUInt());
            }

            if (map.contains(FlightPlanRelatedData_TOD_REP) && map.contains(FlightPlanRelatedData_TOD_TYP) && map.contains(FlightPlanRelatedData_TOD_DAY) && map.contains(FlightPlanRelatedData_TOD_HOR) && map.contains(FlightPlanRelatedData_TOD_MIN)) {
                subMask.set(12);
                write8(subStream, map.value(FlightPlanRelatedData_TOD_REP).toInt());

                quint8 data = (map.value(FlightPlanRelatedData_TOD_TYP).toUInt() & M5) << 3;
                data |= (map.value(FlightPlanRelatedData_TOD_DAY).toUInt() & M2) << 1;
                write8(subStream, data);
                write8(subStream, map.value(FlightPlanRelatedData_TOD_HOR).toUInt() & M5);
                write8(subStream, map.value(FlightPlanRelatedData_TOD_MIN).toUInt() & M6);
                data = 0;
                if (map.contains(FlightPlanRelatedData_TOD_SEC)) {
                    data |= M1 << 7;
                    data |= map.value(FlightPlanRelatedData_TOD_SEC).toUInt() & M6;
                }
                write8(subStream, data);
            }

            if (map.contains(FlightPlanRelatedData_AST)) {
                QByteArray data = map.value(FlightPlanRelatedData_AST).toByteArray();
                if (6 == data.size()) {
                    subMask.set(13);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_STS_EMP) && map.contains(FlightPlanRelatedData_STS_AVL)) {
                subMask.set(14);
                quint8 data = (map.value(FlightPlanRelatedData_STS_EMP).toUInt() & M2) << 6;
                data |= (map.value(FlightPlanRelatedData_STS_AVL).toUInt() & M2) << 4;
                write8(subStream, data);
            }

            if (map.contains(FlightPlanRelatedData_STD)) {
                QByteArray data = map.value(FlightPlanRelatedData_STD).toByteArray();
                if (7 == data.size()) {
                    subMask.set(15);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_STA)) {
                QByteArray data = map.value(FlightPlanRelatedData_STA).toByteArray();
                if (7 == data.size()) {
                    subMask.set(16);
                    writeArray(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_PEM_VA)) {
                quint16 data = (map.value(FlightPlanRelatedData_PEM_VA).toUInt() & M1) << 12;
                if (data) {
                    if (map.contains(FlightPlanRelatedData_PEM_reply)) {
                        data |= map.value(FlightPlanRelatedData_PEM_reply).toUInt() & M12;
                        subMask.set(17);
                        write16(subStream, data);
                    }
                } else {
                    subMask.set(17);
                    write16(subStream, data);
                }
            }

            if (map.contains(FlightPlanRelatedData_PEC)) {
                QByteArray data = map.value(FlightPlanRelatedData_PEC).toByteArray();
                if (7 == data.size()) {
                    subMask.set(18);
                    writeArray(subStream, data);
                }
            }

            if (!subMask.isEmpty()) {
                mainMask.set(21);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }
        }

        // I062/270 Target Size & Orientation [O]
        if (map.contains(TargetSizeAndOrientation_Length)) {
            mainMask.set(22);
            qint8 data = (map.value(TargetSizeAndOrientation_Length).toInt() & M7 ) << 1;
            if (map.contains(TargetSizeAndOrientation_Orientation))
            {
                data |= M1;
                write8(stream, data);
                data = (qint8(map.value(TargetSizeAndOrientation_Orientation).toDouble() * 128 / 360) & M7 ) << 1;
                if (map.contains(TargetSizeAndOrientation_Width))
                {
                    data |= M1;
                    write8(stream, data);
                    data = (map.value(TargetSizeAndOrientation_Width).toInt() & M7 ) << 1;
                    write8(stream, data);
                } else
                    write8(stream, data);
            } else
                write8(stream, data);
        }

        // I062/300 Vehicle Fleet Identification [O]
        if (map.contains(VehicleFleetIdentification)) {
            mainMask.set(23);
            write8(stream, map.value(VehicleFleetIdentification).toUInt());
        }

        // I062/110 Mode 5 Data reports & Extended Mode 1 Code [O]
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_M5) && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_ID)
                && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_DA) && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_M1)
                && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_M2) && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_M3)
                && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_MC) && map.contains(Mode5DataReportsAndExtendedMode1Code_SUM_X)) {
                subMask.set(1);
                quint8 data = (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_M5).toUInt() & M1 ) << 7;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_ID).toUInt() & M1 ) << 6;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_DA).toUInt() & M1 ) << 5;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_M1).toUInt() & M1 ) << 4;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_M2).toUInt() & M1 ) << 3;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_M3).toUInt() & M1 ) << 2;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_SUM_MC).toUInt() & M1 ) << 1;
                data |= map.value(Mode5DataReportsAndExtendedMode1Code_SUM_X).toUInt() & M1;
                write8(subStream, data);
            }

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_PMN_PIN) && map.contains(Mode5DataReportsAndExtendedMode1Code_PMN_NAT) && map.contains(Mode5DataReportsAndExtendedMode1Code_PMN_MIS)) {
                subMask.set(2);
                write16(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_PMN_PIN).toUInt() & M30);
                write8(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_PMN_NAT).toUInt() & M6);
                write8(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_PMN_MIS).toUInt() & M6);
            }

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_POS_Latitude) && map.contains(Mode5DataReportsAndExtendedMode1Code_POS_Longitude)) {
                subMask.set(3);
                write24(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_POS_Latitude).toDouble() * 8388608 / 180);
                write24(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_POS_Longitude).toDouble() * 8388608 / 180);
            }

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_GA_RES) && map.contains(Mode5DataReportsAndExtendedMode1Code_GA)) {
                subMask.set(4);
                qint16 data = (map.value(Mode5DataReportsAndExtendedMode1Code_GA_RES).toInt() & M1 ) << 14;
                data |= qint16(map.value(Mode5DataReportsAndExtendedMode1Code_GA).toDouble() / 25) & M14;
                write16(subStream, data);
            }

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_EM1)) {
                subMask.set(5);
                write16(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_EM1).toUInt() & M12);
            }

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_TOS)) {
                subMask.set(6);
                write8(subStream, map.value(Mode5DataReportsAndExtendedMode1Code_TOS).toDouble() * 128);
            }

            if (map.contains(Mode5DataReportsAndExtendedMode1Code_XP_X5)
                && map.contains(Mode5DataReportsAndExtendedMode1Code_XP_XC) && map.contains(Mode5DataReportsAndExtendedMode1Code_XP_X3)
                && map.contains(Mode5DataReportsAndExtendedMode1Code_XP_X2) && map.contains(Mode5DataReportsAndExtendedMode1Code_XP_X1)) {
                subMask.set(7);
                quint8 data = (map.value(Mode5DataReportsAndExtendedMode1Code_XP_X5).toUInt() & M1) << 4;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_XP_XC).toUInt() & M1) << 3;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_XP_X3).toUInt() & M1) << 2;
                data |= (map.value(Mode5DataReportsAndExtendedMode1Code_XP_X2).toUInt() & M1) << 1;
                data |= map.value(Mode5DataReportsAndExtendedMode1Code_XP_X1).toUInt() & M1;
                write8(subStream, data);
            }

            if (!subMask.isEmpty()) {
                mainMask.set(24);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }
        }

        // I062/120 Track Mode 2 Code
        if (map.contains(TrackMode2Code)) {
            mainMask.set(25);
            write16(stream, map.value(TrackMode2Code).toUInt() & M12);
        }

        // I062/510 Composed Track Number
        if (map.contains(QString(ComposedTrackNumber_SystemUnitIdentification).arg(0)) && map.contains(QString(ComposedTrackNumber_SystemTrackNumber).arg(0))) {
            mainMask.set(26);
            write8(stream, map.value(QString(ComposedTrackNumber_SystemUnitIdentification).arg(0)).toUInt());
            quint16 data = (map.value(QString(ComposedTrackNumber_SystemTrackNumber).arg(0)).toUInt() & M15) << 1;
            int i = 1;
            QString v1 = QString(ComposedTrackNumber_SystemUnitIdentification).arg(i);
            QString v2 = QString(ComposedTrackNumber_SystemTrackNumber).arg(i++);
            while (map.contains(v1) && map.contains(v2)) {
                data |= M1;
                write16(stream, data);
                write8(stream, map.value(v1).toUInt());
                data = (map.value(v2).toUInt() & M15) << 1;
                v1 = QString(ComposedTrackNumber_SystemUnitIdentification).arg(i);
                v2 = QString(ComposedTrackNumber_SystemTrackNumber).arg(i++);
            }
            write16(stream, data);
        }

        // I062/500 Estimated Accuracies
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(EstimatedAccuracies_APC_X) && map.contains(EstimatedAccuracies_APC_Y)) {
                subMask.set(1);
                write16(subStream, map.value(EstimatedAccuracies_APC_X).toDouble() * 2);
                write16(subStream, map.value(EstimatedAccuracies_APC_Y).toDouble() * 2);
            }

            if (map.contains(EstimatedAccuracies_COV)) {
                subMask.set(2);
                write16(subStream, map.value(EstimatedAccuracies_COV).toDouble() * 2);
            }

            if (map.contains(EstimatedAccuracies_APW_Latitude)&& map.contains(EstimatedAccuracies_APW_Longitude)) {
                subMask.set(3);
                write16(subStream, map.value(EstimatedAccuracies_APW_Latitude).toDouble() * 33554432 / 180);
                write16(subStream, map.value(EstimatedAccuracies_APW_Longitude).toDouble() * 33554432 / 180);
            }

            if (map.contains(EstimatedAccuracies_AGA)) {
                subMask.set(4);
                write8(subStream, map.value(EstimatedAccuracies_AGA).toDouble() / 6.25);
            }

            if (map.contains(EstimatedAccuracies_ABA)) {
                subMask.set(5);
                write8(subStream, map.value(EstimatedAccuracies_ABA).toDouble() * 4);
            }

            if (map.contains(EstimatedAccuracies_ATV_X) && map.contains(EstimatedAccuracies_ATV_Y)) {
                subMask.set(6);
                write8(subStream, map.value(EstimatedAccuracies_ATV_X).toDouble() * 4);
                write8(subStream, map.value(EstimatedAccuracies_ATV_Y).toDouble() * 4);
            }

            if (map.contains(EstimatedAccuracies_AA_X) && map.contains(EstimatedAccuracies_AA_Y)) {
                subMask.set(7);
                write8(subStream, map.value(EstimatedAccuracies_AA_X).toDouble() * 4);
                write8(subStream, map.value(EstimatedAccuracies_AA_Y).toDouble() * 4);
            }

            if (map.contains(EstimatedAccuracies_ARC)) {
                subMask.set(8);
                write8(subStream, map.value(EstimatedAccuracies_ARC).toDouble() / 6.25);
            }

            if (!subMask.isEmpty()) {
                mainMask.set(27);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }
        }

        // I062/340 Measured Information
        {
            PresentData subMask;
            QByteArray subArray;
            QDataStream subStream(&subArray, QIODevice::WriteOnly);

            if (map.contains(MeasuredInformation_SID_SAC) && map.contains(MeasuredInformation_SID_SIC)) {
                subMask.set(1);
                write8(subStream, map.value(MeasuredInformation_SID_SAC).toUInt());
                write8(subStream, map.value(MeasuredInformation_SID_SIC).toUInt());
            }

            if (map.contains(MeasuredInformation_POS_RHO) && map.contains(MeasuredInformation_POS_THETA)) {
                subMask.set(2);
                write16(subStream, map.value(MeasuredInformation_POS_RHO).toDouble() * 256);
                write16(subStream, map.value(MeasuredInformation_POS_THETA).toDouble() * 65536 / 360);
            }

            if (map.contains(MeasuredInformation_HEI)) {
                subMask.set(3);
                write16(subStream, map.value(MeasuredInformation_HEI).toDouble() / 25);
            }

            if (map.contains(MeasuredInformation_MDC_V) && map.contains(MeasuredInformation_MDC_G) && map.contains(MeasuredInformation_MDC)) {
                subMask.set(4);
                qint16 data = (map.value(MeasuredInformation_MDC_V).toInt() & M1) << 15;
                data |= (map.value(MeasuredInformation_MDC_G).toInt() & M1) << 14;
                data |= qint16(map.value(MeasuredInformation_MDC).toDouble() * 4) & M14;
                write16(subStream, data);
            }

            if (map.contains(MeasuredInformation_MDA_V) && map.contains(MeasuredInformation_MDA_G)
                && map.contains(MeasuredInformation_MDA_L) && map.contains(MeasuredInformation_MDA)) {
                subMask.set(5);
                quint16 data = (map.value(MeasuredInformation_MDA_V).toUInt() & M1) << 15;
                data |= (map.value(MeasuredInformation_MDA_G).toUInt() & M1) << 14;
                data |= (map.value(MeasuredInformation_MDA_L).toUInt() & M1) << 13;
                data |= map.value(MeasuredInformation_MDA).toUInt() & M12;
                write16(subStream, data);
            }

            if (map.contains(MeasuredInformation_TYP) && map.contains(MeasuredInformation_TYP_SIM)
                && map.contains(MeasuredInformation_TYP_RAB) && map.contains(MeasuredInformation_TYP_TST)) {
                subMask.set(6);
                quint8 data = (map.value(MeasuredInformation_TYP).toUInt() & M3) << 5;
                data |= (map.value(MeasuredInformation_TYP_SIM).toUInt() & M1) << 4;
                data |= (map.value(MeasuredInformation_TYP_RAB).toUInt() & M1) << 3;
                data |= (map.value(MeasuredInformation_TYP_TST).toUInt() & M1) << 2;
                write8(subStream, data);
            }

            if (!subMask.isEmpty()) {
                mainMask.set(28);
                writeArray(stream, subMask.data());
                writeArray(stream, subArray);
            }

            if (generate_re(map, stream))
                mainMask.set(34);

            if (generate_sp(map, stream))
                mainMask.set(35);
        }
    }
    catch (AsterixErrors::ErrorCode error)
    {
        map.appendError(error);
    }
    catch (AsterixErrors *error)
    {
        map.appendError(error);
    }
    catch (ShareError error)
    {
        if (error == Error_UnexpectedEndOfData)
            map.appendError(AsterixErrors::Error_UnexpectedEndOfData);
    }
    return result.prepend(mainMask.data());
}

// ----------------------------------------------------------------------------------

static QMap<QString, QPair<QVariant, QVariant> > initValidationMap()
{
    QMap<QString, QPair<QVariant, QVariant> > result;
    result.insert(Asterix062Proto::CalculatedTrackPositionWGS_Latitude, QPair<QVariant, QVariant>(double(-90), double(90)));
    result.insert(Asterix062Proto::CalculatedTrackPositionWGS_Longitude, QPair<QVariant, QVariant>(double(-180), double(180)));
    result.insert(Asterix062Proto::CalculatedTrackVelocityCartesian_Vx, QPair<QVariant, QVariant>(double(-8192), double(8191.75))); // -8192m/s <= Vx <= 8191.75m/s
    result.insert(Asterix062Proto::CalculatedTrackVelocityCartesian_Vy, QPair<QVariant, QVariant>(double(-8192), double(8191.75))); // -8192m/s <= Vy <= 8191.75m/s
    result.insert(Asterix062Proto::AircraftDerivedData_TAS, QPair<QVariant, QVariant>(quint16(0), quint16(2046))); // 0 <= True Air Speed <= 2046 knots
    result.insert(Asterix062Proto::AircraftDerivedData_SAL_Altitude, QPair<QVariant, QVariant>(double(-1300), double(100000))); // -1300ft <= Altitude <= 100000ft
    result.insert(Asterix062Proto::AircraftDerivedData_FSS_Altitude, QPair<QVariant, QVariant>(double(-1300), double(100000))); // -1300ft <= Altitude <= 100000ft
    result.insert(Asterix062Proto::AircraftDerivedData_TID_Altitude, QPair<QVariant, QVariant>(double(-1500), double(150000))); // -1500 ft <= altitude <= 150000 ft
    result.insert(Asterix062Proto::AircraftDerivedData_TID_REP_Latitude, QPair<QVariant, QVariant>(double(-90), double(90))); // -90 <= latitude <= 90 deg.
    result.insert(Asterix062Proto::AircraftDerivedData_TID_REP_Longitude, QPair<QVariant, QVariant>(double(-180), double(180))); // -180 <= longitude < 180
    result.insert(Asterix062Proto::AircraftDerivedData_TID_REP_TTR, QPair<QVariant, QVariant>(double(0), double(655.55))); // 0 <= TTR <= 655.35 Nm
    result.insert(Asterix062Proto::AircraftDerivedData_RAN, QPair<QVariant, QVariant>(double(-180), double(180))); // -180 <= Roll Angle <= 180
    result.insert(Asterix062Proto::AircraftDerivedData_TAR_RateOfTurn, QPair<QVariant, QVariant>(double(-15), double(15))); // -15 °/s <= Rate of Turn <= 15 °/s
    result.insert(Asterix062Proto::AircraftDerivedData_GSP, QPair<QVariant, QVariant>(double(-2), double(2))); // -2 NM/s <= Ground Speed < 2 NM/s
    result.insert(Asterix062Proto::AircraftDerivedData_MET_WindSpeed, QPair<QVariant, QVariant>(quint16(0), quint16(300))); // 0 <= Wind Speed <= 300
    result.insert(Asterix062Proto::AircraftDerivedData_MET_WindDirection, QPair<QVariant, QVariant>(quint16(1), quint16(360))); // 1 <= Wind Direction <= 360
    result.insert(Asterix062Proto::AircraftDerivedData_MET_Temperature, QPair<QVariant, QVariant>(double(-100), double(100))); // -100 °C <= Temperature <= 100 °C
    result.insert(Asterix062Proto::AircraftDerivedData_MET_Turbulence, QPair<QVariant, QVariant>(quint8(0), quint8(15))); // Integer between 0 and 15 inclusive
    result.insert(Asterix062Proto::AircraftDerivedData_POS_Latitude, QPair<QVariant, QVariant>(double(-90), double(90))); // Range -90 <= latitude <= 90 deg.
    result.insert(Asterix062Proto::AircraftDerivedData_POS_Longitude, QPair<QVariant, QVariant>(double(-180), double(180))); // Range -180 <= longitude < 180 deg.
    result.insert(Asterix062Proto::AircraftDerivedData_GAL, QPair<QVariant, QVariant>(double(-1500), double(150000))); // -1500 ft <= Altitude <= 150000 ft
    result.insert(Asterix062Proto::AircraftDerivedData_IAR, QPair<QVariant, QVariant>(quint16(0), quint16(1100))); // 0 Kt <= Indicated Airspeed <= 1100 Kt
    result.insert(Asterix062Proto::AircraftDerivedData_MAC, QPair<QVariant, QVariant>(double(0), double(4.096))); // 0 <= Mach Number <= 4.096
    result.insert(Asterix062Proto::AircraftDerivedData_BPS, QPair<QVariant, QVariant>(double(0), double(409.5))); // -0ft <= BPS <= 409.5 mb
    result.insert(Asterix062Proto::SystemTrackUpdateAges_TRK, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value =63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_PSR, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value =63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_SSR, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_MDS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_ADS, QPair<QVariant, QVariant>(double(0), double(16383.75))); // Max. value = 16383.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_ES, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_VDL, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_UAT, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_LOP, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::SystemTrackUpdateAges_MLT, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_FML, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MD1, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MD2, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MDA, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MD4, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MD5, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MHG, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_IAS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_TAS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_SAL, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_FSS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_TID, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_COM, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_SAB, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_ACS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_BVR, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_GVR, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_RAN, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_TAR, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_TAN, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_GSP, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_VUN, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MET, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_EMC, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_POS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_GAL, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_PUN, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MB, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_IAR, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_MAC, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::TrackDataAges_BPS, QPair<QVariant, QVariant>(double(0), double(63.75))); // Maximum value = 63.75s
    result.insert(Asterix062Proto::MeasuredFlightLevel, QPair<QVariant, QVariant>(double(-15), double(1500))); // Vmin = -15 F; Vmax = 1500 FL
    result.insert(Asterix062Proto::CalculatedTrackGeometricAltitude, QPair<QVariant, QVariant>(double(-1500), double(150000))); // Vmin = -1500 ft; Vmax = 150000 ft
    result.insert(Asterix062Proto::CalculatedTrackBarometricAltitude, QPair<QVariant, QVariant>(double(-15), double(1500))); // Vmin = -15 FL; Vmax = 1500 FL
    result.insert(Asterix062Proto::FlightPlanRelatedData_IFI_NBR, QPair<QVariant, QVariant>(quint32(0), quint32(99999999))); // Number from 0 to 99 999 999
    result.insert(Asterix062Proto::FlightPlanRelatedData_TOD_TYP, QPair<QVariant, QVariant>(quint8(0), quint8(13))); // 0 - 13
    result.insert(Asterix062Proto::FlightPlanRelatedData_TOD_HOR, QPair<QVariant, QVariant>(quint16(0), quint16(23))); // Hours, from 0 to 23
    result.insert(Asterix062Proto::FlightPlanRelatedData_TOD_MIN, QPair<QVariant, QVariant>(quint16(0), quint16(59))); // Minutes, from 0 to 59
    result.insert(Asterix062Proto::FlightPlanRelatedData_TOD_SEC, QPair<QVariant, QVariant>(quint16(0), quint16(59))); // Seconds, from 0 to 59
    result.insert(Asterix062Proto::VehicleFleetIdentification, QPair<QVariant, QVariant>(quint8(0), quint8(16))); // 0 - 16
    result.insert(Asterix062Proto::MeasuredInformation_MDC, QPair<QVariant, QVariant>(double(-12), double(1270))); // Vmin = -12 FL; Vmax = 1270 FL
    return result;
}

const QMap<QString, QPair<QVariant, QVariant> > Asterix062Proto::validationMap = initValidationMap();

QStringList Asterix062Proto::validate(const QMap<QString, QVariant> &map)
{
    QStringList errors;
    for (QMap<QString, QPair<QVariant, QVariant> >::const_iterator it = validationMap.begin(), end = validationMap.end(); it != end; ++it)
    {
        QMap<QString, QVariant>::const_iterator valueIt = map.find(it.key());
        if (map.end() != valueIt)
        {
            switch (it->first.type())
            {
            case QVariant::Double:
                {
                    double data = valueIt.value().toDouble();
                    double min = it->first.toDouble();
                    double max = it->second.toDouble();
                    if (data < min || data > max)
                        errors << QString("%1 should be between %2 and %3. Current value is %4").arg(valueIt.key()).arg(min).arg(max).arg(data);
                    break;
                }
            case QVariant::Int:
                {
                    int data = valueIt.value().toInt();
                    int min = it->first.toInt();
                    int max = it->second.toInt();
                    if (data < min || data > max)
                        errors << QString("%1 should be between %2 and %3. Current value is %4").arg(valueIt.key()).arg(min).arg(max).arg(data);
                    break;
                }
            case QVariant::UInt:
                {
                    uint data = valueIt.value().toUInt();
                    uint min = it->first.toUInt();
                    uint max = it->second.toUInt();
                    if (data < min || data > max)
                        errors << QString("%1 should be between %2 and %3. Current value is %4").arg(valueIt.key()).arg(min).arg(max).arg(data);
                    break;
                }
            default:
                // throw QString("INTERNAL ERROR; Asterix62::validate; UNKNOWN TYPE"); // this shouldn't happen
                break;
            }
        }
    }

    if (map.contains(TimeOfTrackInformation))
        if (!map.value(TimeOfTrackInformation).toTime().isValid())
            errors << QString("%1 contains invalid time").arg(TimeOfTrackInformation);

    return errors;
}

QStringList Asterix062Proto::fields() const
{
    static QStringList result = QStringList()

    << DataSourceIdentifier_SAC
    << DataSourceIdentifier_SIC

    << ServiceIdentification

    << TimeOfTrackInformation

    << CalculatedTrackPositionWGS_Latitude
    << CalculatedTrackPositionWGS_Longitude

    << CalculatedTrackPositionCartesian_x
    << CalculatedTrackPositionCartesian_y

    << CalculatedTrackVelocityCartesian_Vx
    << CalculatedTrackVelocityCartesian_Vy

    << CalculatedAccelerationCartesian_Ax
    << CalculatedAccelerationCartesian_Ay

    << TrackMode3ACode_hasChanged
    << TrackMode3ACode_reply

    << TargetIdentification_STI
    << TargetIdentification_Character1
    << TargetIdentification_Character2
    << TargetIdentification_Character3
    << TargetIdentification_Character4
    << TargetIdentification_Character5
    << TargetIdentification_Character6
    << TargetIdentification_Character7
    << TargetIdentification_Character8

    << AircraftDerivedData_ADR
    << AircraftDerivedData_ID
    << AircraftDerivedData_ID_Character1
    << AircraftDerivedData_ID_Character2
    << AircraftDerivedData_ID_Character3
    << AircraftDerivedData_ID_Character4
    << AircraftDerivedData_ID_Character5
    << AircraftDerivedData_ID_Character6
    << AircraftDerivedData_ID_Character7
    << AircraftDerivedData_ID_Character8
    << AircraftDerivedData_MHG
    << AircraftDerivedData_IAS_IM
    << AircraftDerivedData_IAS
    << AircraftDerivedData_TAS
    << AircraftDerivedData_SAL_SAS
    << AircraftDerivedData_SAL_Source
    << AircraftDerivedData_SAL_Altitude
    << AircraftDerivedData_FSS_MV
    << AircraftDerivedData_FSS_AH
    << AircraftDerivedData_FSS_AM
    << AircraftDerivedData_FSS_Altitude
    << AircraftDerivedData_TIS_NAV
    << AircraftDerivedData_TIS_NVB
    << AircraftDerivedData_TID_REP
    << AircraftDerivedData_TID_TCA
    << AircraftDerivedData_TID_NC
    << AircraftDerivedData_TID_TCP
    << AircraftDerivedData_TID_Altitude
    << AircraftDerivedData_TID_REP_Latitude
    << AircraftDerivedData_TID_REP_Longitude
    << AircraftDerivedData_TID_REP_PointType
    << AircraftDerivedData_TID_REP_TD
    << AircraftDerivedData_TID_REP_TRA
    << AircraftDerivedData_TID_REP_TOA
    << AircraftDerivedData_TID_REP_TOV
    << AircraftDerivedData_TID_REP_TTR

    << AircraftDerivedData_COM
    << AircraftDerivedData_COM_STAT
    << AircraftDerivedData_COM_SSC
    << AircraftDerivedData_COM_ARC
    << AircraftDerivedData_COM_AIC
    << AircraftDerivedData_COM_B1A
    << AircraftDerivedData_COM_B1B

    << AircraftDerivedData_SAB_AC
    << AircraftDerivedData_SAB_MN
    << AircraftDerivedData_SAB_DC
    << AircraftDerivedData_SAB_GBS
    << AircraftDerivedData_SAB_STAT

    << AircraftDerivedData_ACS

    << AircraftDerivedData_BVR

    << AircraftDerivedData_GVR

    << AircraftDerivedData_RAN

    << AircraftDerivedData_TAR_TI
    << AircraftDerivedData_TAR_RateOfTurn

    << AircraftDerivedData_TAN

    << AircraftDerivedData_GSP

    << AircraftDerivedData_VUN

    << AircraftDerivedData_MET_WS
    << AircraftDerivedData_MET_WD
    << AircraftDerivedData_MET_TMP
    << AircraftDerivedData_MET_TRB
    << AircraftDerivedData_MET_WindSpeed
    << AircraftDerivedData_MET_WindDirection
    << AircraftDerivedData_MET_Temperature
    << AircraftDerivedData_MET_Turbulence

    << AircraftDerivedData_EMC

    << AircraftDerivedData_POS_Latitude
    << AircraftDerivedData_POS_Longitude

    << AircraftDerivedData_GAL

    << AircraftDerivedData_PUN

    << AircraftDerivedData_MB_REP
    << AircraftDerivedData_MB_data
    << AircraftDerivedData_MB_BDS1
    << AircraftDerivedData_MB_BDS2

    << AircraftDerivedData_IAR

    << AircraftDerivedData_MAC

    << AircraftDerivedData_BPS

    << TrackNumber

    << TrackStatus_MON
    << TrackStatus_SPI
    << TrackStatus_MRH
    << TrackStatus_SRC
    << TrackStatus_CNF

    << TrackStatus

    << SystemTrackUpdateAges_TRK
    << SystemTrackUpdateAges_PSR
    << SystemTrackUpdateAges_SSR
    << SystemTrackUpdateAges_MDS
    << SystemTrackUpdateAges_ADS
    << SystemTrackUpdateAges_ES
    << SystemTrackUpdateAges_VDL
    << SystemTrackUpdateAges_UAT
    << SystemTrackUpdateAges_LOP
    << SystemTrackUpdateAges_MLT

    << ModeOfMovement_TRANS
    << ModeOfMovement_LONG
    << ModeOfMovement_VERT
    << ModeOfMovement_ADF

    << TrackDataAges_FML
    << TrackDataAges_MD1
    << TrackDataAges_MD2
    << TrackDataAges_MDA
    << TrackDataAges_MD4
    << TrackDataAges_MD5
    << TrackDataAges_MHG
    << TrackDataAges_IAS
    << TrackDataAges_TAS
    << TrackDataAges_SAL
    << TrackDataAges_FSS
    << TrackDataAges_TID
    << TrackDataAges_COM
    << TrackDataAges_SAB
    << TrackDataAges_ACS
    << TrackDataAges_BVR
    << TrackDataAges_GVR
    << TrackDataAges_RAN
    << TrackDataAges_TAR
    << TrackDataAges_TAN
    << TrackDataAges_GSP
    << TrackDataAges_VUN
    << TrackDataAges_MET
    << TrackDataAges_EMC
    << TrackDataAges_POS
    << TrackDataAges_GAL
    << TrackDataAges_PUN
    << TrackDataAges_MB
    << TrackDataAges_IAR
    << TrackDataAges_MAC
    << TrackDataAges_BPS

    << MeasuredFlightLevel

    << CalculatedTrackGeometricAltitude

    << CalculatedTrackBarometricAltitude_QNH
    << CalculatedTrackBarometricAltitude

    << CalculatedRateOfClimbDescent

    << FlightPlanRelatedData_TAG_SAC
    << FlightPlanRelatedData_TAG_SIC

    << FlightPlanRelatedData_CSN

    << FlightPlanRelatedData_IFI_TYP
    << FlightPlanRelatedData_IFI_NBR

    << FlightPlanRelatedData_FCT_GAT_OAT
    << FlightPlanRelatedData_FCT_FR1_FR2
    << FlightPlanRelatedData_FCT_RVSM
    << FlightPlanRelatedData_FCT_HPR

    << FlightPlanRelatedData_TAC

    << FlightPlanRelatedData_WTC

    << FlightPlanRelatedData_DEP

    << FlightPlanRelatedData_DST

    << FlightPlanRelatedData_RDS_NU1
    << FlightPlanRelatedData_RDS_NU2
    << FlightPlanRelatedData_RDS_LTR

    << FlightPlanRelatedData_CFL

    << FlightPlanRelatedData_CTL_Centre
    << FlightPlanRelatedData_CTL_Position

    << FlightPlanRelatedData_TOD_REP
    << FlightPlanRelatedData_TOD_TYP
    << FlightPlanRelatedData_TOD_DAY
    << FlightPlanRelatedData_TOD_HOR
    << FlightPlanRelatedData_TOD_MIN
    << FlightPlanRelatedData_TOD_SEC

    << FlightPlanRelatedData_AST

    << FlightPlanRelatedData_STS_EMP
    << FlightPlanRelatedData_STS_AVL

    << FlightPlanRelatedData_STD

    << FlightPlanRelatedData_STA

    << FlightPlanRelatedData_PEM_VA
    << FlightPlanRelatedData_PEM_reply

    << FlightPlanRelatedData_PEC

    << TargetSizeAndOrientation_Length
    << TargetSizeAndOrientation_Orientation
    << TargetSizeAndOrientation_Width

    << VehicleFleetIdentification

    << Mode5DataReportsAndExtendedMode1Code_SUM_M5
    << Mode5DataReportsAndExtendedMode1Code_SUM_ID
    << Mode5DataReportsAndExtendedMode1Code_SUM_DA
    << Mode5DataReportsAndExtendedMode1Code_SUM_M1
    << Mode5DataReportsAndExtendedMode1Code_SUM_M2
    << Mode5DataReportsAndExtendedMode1Code_SUM_M3
    << Mode5DataReportsAndExtendedMode1Code_SUM_MC
    << Mode5DataReportsAndExtendedMode1Code_SUM_X

    << Mode5DataReportsAndExtendedMode1Code_PMN_PIN
    << Mode5DataReportsAndExtendedMode1Code_PMN_NAT
    << Mode5DataReportsAndExtendedMode1Code_PMN_MIS

    << Mode5DataReportsAndExtendedMode1Code_POS_Latitude
    << Mode5DataReportsAndExtendedMode1Code_POS_Longitude

    << Mode5DataReportsAndExtendedMode1Code_GA_RES
    << Mode5DataReportsAndExtendedMode1Code_GA

    << Mode5DataReportsAndExtendedMode1Code_EM1

    << Mode5DataReportsAndExtendedMode1Code_TOS

    << Mode5DataReportsAndExtendedMode1Code_XP_X5
    << Mode5DataReportsAndExtendedMode1Code_XP_XC
    << Mode5DataReportsAndExtendedMode1Code_XP_X3
    << Mode5DataReportsAndExtendedMode1Code_XP_X2
    << Mode5DataReportsAndExtendedMode1Code_XP_X1

    << TrackMode2Code

    << ComposedTrackNumber_SystemUnitIdentification
    << ComposedTrackNumber_SystemTrackNumber

    << EstimatedAccuracies_APC_X
    << EstimatedAccuracies_APC_Y

    << EstimatedAccuracies_COV

    << EstimatedAccuracies_APW_Latitude
    << EstimatedAccuracies_APW_Longitude

    << EstimatedAccuracies_AGA

    << EstimatedAccuracies_ABA

    << EstimatedAccuracies_ATV_X
    << EstimatedAccuracies_ATV_Y

    << EstimatedAccuracies_AA_X
    << EstimatedAccuracies_AA_Y

    << EstimatedAccuracies_ARC

    << MeasuredInformation_SID_SAC
    << MeasuredInformation_SID_SIC

    << MeasuredInformation_POS_RHO
    << MeasuredInformation_POS_THETA

    << MeasuredInformation_HEI

    << MeasuredInformation_MDC_V
    << MeasuredInformation_MDC_G
    << MeasuredInformation_MDC

    << MeasuredInformation_MDA_V
    << MeasuredInformation_MDA_G
    << MeasuredInformation_MDA_L
    << MeasuredInformation_MDA

    << MeasuredInformation_TYP
    << MeasuredInformation_TYP_SIM
    << MeasuredInformation_TYP_RAB
    << MeasuredInformation_TYP_TST;

    return result;
}

QChar Asterix062Proto::decode(quint8 code)
{
    char value;

    switch (code) {
    case 0x01: value = 'A'; break;
    case 0x02: value = 'B'; break;
    case 0x03: value = 'C'; break;
    case 0x04: value = 'D'; break;
    case 0x05: value = 'E'; break;
    case 0x06: value = 'F'; break;
    case 0x07: value = 'G'; break;
    case 0x08: value = 'H'; break;
    case 0x09: value = 'I'; break;
    case 0x0A: value = 'J'; break;
    case 0x0B: value = 'K'; break;
    case 0x0C: value = 'L'; break;
    case 0x0D: value = 'M'; break;
    case 0x0E: value = 'N'; break;
    case 0x0F: value = 'O'; break;

    case 0x10: value = 'P'; break;
    case 0x11: value = 'Q'; break;
    case 0x12: value = 'R'; break;
    case 0x13: value = 'S'; break;
    case 0x14: value = 'T'; break;
    case 0x15: value = 'U'; break;
    case 0x16: value = 'V'; break;
    case 0x17: value = 'W'; break;
    case 0x18: value = 'X'; break;
    case 0x19: value = 'Y'; break;
    case 0x1A: value = 'Z'; break;

    case 0x20: value = ' '; break;

    case 0x30: value = '0'; break;
    case 0x31: value = '1'; break;
    case 0x32: value = '2'; break;
    case 0x33: value = '3'; break;
    case 0x34: value = '4'; break;
    case 0x35: value = '5'; break;
    case 0x36: value = '6'; break;
    case 0x37: value = '7'; break;
    case 0x38: value = '8'; break;
    case 0x39: value = '9'; break;

    default:
        return QChar(0);
    }

    return QChar(value);
}
