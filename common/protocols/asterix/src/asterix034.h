#ifndef ASTERIX034_PROTO_H
#define ASTERIX034_PROTO_H

#include "asterix.h"

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QDataStream>
#include <QtCore/QStringList>

class AsterixData;

class ASTERIX_EXPORT Asterix034Proto : public Asterix
{
protected:
    void parse_000(QDataStream &stream, AsterixData &result);
    void parse_010(QDataStream &stream, AsterixData &result);
    void parse_020(QDataStream &stream, AsterixData &result);
    void parse_030(QDataStream &stream, AsterixData &result);
    void parse_041(QDataStream &stream, AsterixData &result);
    void parse_050(QDataStream &stream, AsterixData &result);
    void parse_060(QDataStream &stream, AsterixData &result);
    void parse_070(QDataStream &stream, AsterixData &result);
    void parse_090(QDataStream &stream, AsterixData &result);
    void parse_100(QDataStream &stream, AsterixData &result);
    void parse_110(QDataStream &stream, AsterixData &result);
    void parse_120(QDataStream &stream, AsterixData &result);

    bool generate_000(AsterixData &map, QDataStream &stream);
    bool generate_010(AsterixData &map, QDataStream &stream);
    bool generate_020(AsterixData &map, QDataStream &stream);
    bool generate_030(AsterixData &map, QDataStream &stream);
    bool generate_041(AsterixData &map, QDataStream &stream);
    bool generate_050(AsterixData &map, QDataStream &stream);
    bool generate_060(AsterixData &map, QDataStream &stream);
    bool generate_070(AsterixData &map, QDataStream &stream);
    bool generate_090(AsterixData &map, QDataStream &stream);
    bool generate_100(AsterixData &map, QDataStream &stream);
    bool generate_110(AsterixData &map, QDataStream &stream);
    bool generate_120(AsterixData &map, QDataStream &stream);

public:
    Asterix034Proto();

    virtual int category() const;
    virtual QString edition() const;

    virtual void parse(QDataStream &stream, AsterixData &asterix);
    virtual QByteArray generate(AsterixData &asterix);

    virtual QStringList fields() const;

    static const int mCategory;
    static const QString mEdition;

    enum EMessageType {
        NorthMarker                     = 1,
        SectorCrossing                  = 2,
        GeographicalFiltering           = 3,
        JammingStrobe                   = 4
    };

    enum EDataFilter
    {
        InvalidValue,
        WeatherData,
        JammingStrobeFilter,
        PSRdata,
        SSRmodeSdata,
        SSRModeS_PSRdata,
        EnhancedSurveillanceData,
        PSR_EnhancedSurveillanceData,
        PSR_EnhSurv_SSRmodeS,           //!< PSR+Enhanced Surveillance + SSR/Mode S data not in Area of Prime Interes
        PSR_EnhSurv_AllSSRmodeS         //!< PSR+Enhanced Surveillance + all SSR/Mode S data

    };

    enum SystemConfigStatusMask
    {
        MDS = 0x04,
        SSR = 0x08,
        PSR = 0x10,
        COM = 0x80
    };

    //! Structure of COM Subfield: System Configuration and Status
    enum COMSubFieldMaskSCS
    {
        COM_TSV     = 0x02,
        COM_MSC     = 0x04,
        COM_OVL_XMT = 0x08,
        COM_OVL_RDP = 0x10,
        COM_RDPR    = 0x20,
        COM_RDPC    = 0x40,
        COM_NOGO    = 0x80
    };

    //! Structure of PSR, SSR Subfield: Specific Status information for a PSR, SSR sensor
    enum SRSubFieldMaskSCS
    {
        SR_MSC     = 0x08,
        SR_OVL     = 0x10,
        SR_CH_A_B  = 0x60,
        SR_ANT     = 0x80
    };

    //! Structure of MDS Subfield: Specific Status information for a Mode S sensor
    enum MDSSubFieldMaskSCS
    {
        MDS_OVL_DLF = 0x0080,
        MDS_OVL_SCF = 0x0100,
        MDS_DLF     = 0x0200,
        MDS_SCF     = 0x0400,
        MDS_MSC     = 0x0800,
        MDS_OVL_SUR = 0x1000,
        MDS_CH_A_B  = 0x6000,
        MDS_ANT     = 0x8000
    };

    //! Structure of COM Subfield: System Processing Mode
    enum COMSubFieldMaskSPM
    {
        COM_RED_XMT = 0x0E,
        COM_RED_RDP = 0x70
    };

    //! Structure of PSR Subfield: Specific Processing Mode information for a PSR sensor
    enum PSRSubFieldMaskSPM
    {
        PSR_STC     = 0x0C,
        PSR_RED_RAD = 0x70,
        PSR_POL     = 0x80
    };

    //! Structure of SSR Subfield: Specific Processing Mode information for a SSR sensor
    enum SSRSubFieldMaskSPM
    {
        SSR_RED_RAD = 0xE0
    };

    //! Structure of MDS Subfield: Specific Processing Mode information for a Mode S Sensor
    enum MDSSubFieldMaskSPM
    {
        MDS_CLU     = 0x10,
        MDS_RED_RAD = 0xE0
    };

    // Data Item I034/000, Message Type
    static const QString MessageType;                   // EMessageType

    // Data Item I034/010, Data Source Identifier
    static const QString DataSourceIdentifier_SAC; // System Area Code
    static const QString DataSourceIdentifier_SIC; // System Identification Code

    // Data Item I034/020, Sector Number
    static const QString SectorAzimuth;                 // degree

    // Data Item I034/030, Time of Day
    static const QString TimeOfDay;                     // QTime

    // Data Item I034/041, Antenna Rotation Speed
    static const QString AntennaRotationPeriod;         // double

    // Data Item I034/050, System Configuration and Status
    // COM Subfield:
    static const QString StationConfigurationStatus_COM_NOGO;
    static const QString StationConfigurationStatus_COM_RDPC;
    static const QString StationConfigurationStatus_COM_RDPR;
    static const QString StationConfigurationStatus_COM_OVLRDP;
    static const QString StationConfigurationStatus_COM_OVLXMT;
    static const QString StationConfigurationStatus_COM_MSC;
    static const QString StationConfigurationStatus_COM_TSV;
    // PSR Subfield:
    static const QString StationConfigurationStatus_PSR_ANT;
    static const QString StationConfigurationStatus_PSR_CHAB;
    static const QString StationConfigurationStatus_PSR_OVL;
    static const QString StationConfigurationStatus_PSR_MSC;
    // SSR Subfield:
    static const QString StationConfigurationStatus_SSR_ANT;
    static const QString StationConfigurationStatus_SSR_CHAB;
    static const QString StationConfigurationStatus_SSR_OVL;
    static const QString StationConfigurationStatus_SSR_MSC;
    // MDS Subfield:
    static const QString StationConfigurationStatus_MDS_ANT;
    static const QString StationConfigurationStatus_MDS_CHAB;
    static const QString StationConfigurationStatus_MDS_OVLSUR;
    static const QString StationConfigurationStatus_MDS_MSC;
    static const QString StationConfigurationStatus_MDS_SCF;
    static const QString StationConfigurationStatus_MDS_DLF;
    static const QString StationConfigurationStatus_MDS_OVLSCF;
    static const QString StationConfigurationStatus_MDS_OVLDLF;

    // Data Item I034/060, System Processing Mode
    // COM Subfield:
    static const QString StationProcessingMode_COM_REDRDP;
    static const QString StationProcessingMode_COM_REDXMT;
    // PSR Subfield:
    static const QString StationProcessingMode_PSR_POL;
    static const QString StationProcessingMode_PSR_REDRAD;
    static const QString StationProcessingMode_PSR_STC;
    // SSR Subfield:
    static const QString StationProcessingMode_SSR_REDRAD;
    // MDS Subfield:
    static const QString StationProcessingMode_MDS_REDRAD;
    static const QString StationProcessingMode_MDS_CLU;

    //Data Item I034/070, Message Count Values
    static const QString MessageCountValues;

    struct I034_070
    {
        quint8 TYP : 5;
        quint16 COUNTER : 11;
    };

    // Data Item I034/090, Collimation Error
    static const QString CollimationError_RangeError;        // Nautical Miles
    static const QString CollimationError_AzimuthError;      // degree

    // Data Item I034/100, Generic Polar Window
    static const QString GenericPolarWindow_RhoStart;
    static const QString GenericPolarWindow_RhoEnd;
    static const QString GenericPolarWindow_ThetaStart;
    static const QString GenericPolarWindow_ThetaEnd;

    // Data Item I034/110 Data Filter
    static const QString DataFilter;                        // EDataFilter

    // Data Item I034/120 3D-Position Of Data Source.
    static const QString PositionOfSource3D_Height;         // meters - uint
    static const QString PositionOfSource3D_Latitude;       // degree - double
    static const QString PositionOfSource3D_Longitude;      // degree - double
};

Q_DECLARE_METATYPE(Asterix034Proto::I034_070)

#endif // ASTERIX034_PROTO_H
