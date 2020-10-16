#pragma once

#include "Uval.h"
#include "U_GlobalConst.h"
#include "MRT.h"
#include "MRTPlot.h"
#include "MRTTrackPure.h"
#include "UTS.h"
#include "DataSensorStatusM.h"

class CDecodeAsterix  
{
public:
	CDecodeAsterix();
	virtual ~CDecodeAsterix();

	CMRTPlot PlotMRT;
	CMRT *pMRT;
	CUTS *pUTS;
	CMRTTrackPure TrackMRT;
	CMRTAm AmMRT;
	CMRTNorth NorthMRT;
	CMRTPeleng PelengMRT;
	CMeteoRW MeteoRW;
	CMeteo_polygon MeteoPolygon;
    CDataSensorStatusM SensorStatus;
	CMeteoVectorSeq MeteoVector;

	CMRTTime time_stamp;
	CRLKState RlkSt;

	int f;

public:
	unsigned char read_buffer(void);
	void decode_I001_SP(void);
	void decode_I001_010(void);
	void decode_I001_020(void);
	void decode_I001_030(void);
	void decode_I001_040(void);
	void decode_I001_042(void);
	void decode_I001_050(void);
	void decode_I001_060(void);
	void decode_I001_070(void);
	void decode_I001_080(void);
	void decode_I001_090(void);	
	void decode_I001_100(void);	
	void decode_I001_120(void);
	void decode_I001_130(void);
	void decode_I001_131(void);	
	void decode_I001_141(void);
	void decode_I001_150(void);
	void decode_I001_161(void);
	void decode_I001_170(void);
	void decode_I001_200(void);
	void decode_I001_210(void);	
	UCHAR decode_I002_000(void);
	void decode_I002_010(void);
	void decode_I002_020(void);
	void decode_I002_030(void);
	void decode_I002_041(void);
	void decode_I002_050(void);
	void decode_I002_060(void);
	void decode_I002_070(void);
	void decode_I002_080(void);
	void decode_I002_090(void);
	void decode_I002_100(void);
	void decode_I002_SP(void);	
	
	void decode_I062_000(void);
	void decode_I062_010(void);	
	void decode_I062_015(void);
	void decode_I062_040(void);
	void decode_I062_060(void);
	void decode_I062_070(void);
	void decode_I062_080(void);
	void decode_I062_100(void);
	void decode_I062_105(void);
	void decode_I062_135(void);
	void decode_I062_136(void);
	void decode_I062_185(void);
	void decode_I062_200(void);
	void decode_I062_210(void);
	void decode_I062_220(void);
	void decode_I062_380(void);	//Aircraft Derived Data
	void decode_I062_390(void);	//Flight Plan Related Data
	void decode_I062_SP(void);
	void decode_I062_245(void);
	void decode_I062_290(void);
	void decode_I062_295(void);
	void decode_I062_130(void);
	void decode_I062_340(void);

	void get_packet(unsigned char N, int L, void* pVComSource, void* pVM_502=NULL, void *pVSocket=NULL, CMRTTrack *pTrackMRT=NULL, void* pvUKIS=NULL);
	unsigned char pbuf[4096];
	CUval uval;

	int i_comport;
	UCHAR uc_LpdType;

	unsigned char TYP, MS_type;
	unsigned char IKI, B, MK;
	long X, Y;
	double Az, D;
	unsigned char EXNB;
	long NB, Address_ICAO;
	unsigned char EXH, ZH;
	long H, Height_3D;
	unsigned char OT, BRF;
	int OriginCoord;
	unsigned char ANT;
	CString Ident;
//
	double Vx, Vy;
	unsigned char MI;
	CMyMath grrad;


//
	int index;

	unsigned char SAC, SIC;

	byte SPI, FLAG;

	long am;
	unsigned char category, byte, m_byte;

	// Meteo
	CMRTTime		TimeGet;				
	CMRTTime		TimeStop;				
	__int16			MsgNumber;
	UCHAR			Intensity;
	bool			org1;
	float			North;

	UCHAR decode_I008_000(void);
	void  decode_I008_010(void);	
	void  decode_I008_020(void);	
	int	  decode_I008_036(void);
	int	  decode_I008_038(void);
	UCHAR decode_I008_040(int &I, int &num);
	void  decode_I008_050(void);
	void  decode_I008_090(void);
	int	  decode_I008_100(void);
	int  decode_I008_120(void);

	UCHAR decode_I241_000(void);
	void decode_I241_010(void);
	void decode_I241_030(void);
	void decode_I241_105(void);
	void decode_I241_600(void);	
	void decode_I241_601(void);
	void decode_I241_602(void);	
	void decode_I241_603(void);
	void decode_I241_604(void);
	void decode_I241_605(void);
	void decode_I241_606(void);
	void decode_I241_607(void);
	void decode_I241_608(void);
	void decode_I241_609(void);
	void decode_I241_610(void);
	void decode_I241_611(void);
	void decode_I241_612(void);
	void decode_I241_613(void);
	void decode_I241_614(void);
	void decode_I241_615(void);
	void decode_I241_616(void);
	void decode_I241_617(void);
	void decode_I241_618(void);
	
	void decode_I242_010();
	CMRTTime decode_I242_020();
	
	void decode_I063_010( void );	// Data Source Identifier
	void decode_I063_015( void );	// Service Identification
	void decode_I063_030( void );	// Time of Message
	void decode_I063_050( void );	// Sensor Identifier
	void decode_I063_060( void );	// Sensor Configuration and Status
	void decode_I063_070( void );	// Time Stamping Bias
	void decode_I063_080( void );	// SSR/Mode S Range Gain and Bias
	void decode_I063_081( void );	// SSR/Mode S Azimut Bias
	void decode_I063_090( void );	// PSR Range Gain and Bias
	void decode_I063_091( void );	// PSR Azimut Bias
	void decode_I063_092( void );	// PSR Elevation Bias
	//void decode_I063_RE( void );
	//void decode_I063_SP( void );


	UCHAR get_byte(void);
	__int16 get_int16(void);
	long get_int24(void);
	long get_long(void);

	void decode_I021_010();
	void decode_I021_020();
	void decode_I021_030();
	void decode_I021_032();
	void decode_I021_040();
	void decode_I021_070();
	void decode_I021_080();
	void decode_I021_090();
	void decode_I021_095();
	void decode_I021_110();
	void decode_I021_130();
	void decode_I021_131();
	void decode_I021_140();
	void decode_I021_145();
	void decode_I021_146();
	void decode_I021_148();
	void decode_I021_150();
	void decode_I021_151();
	void decode_I021_152();
	void decode_I021_155();
	void decode_I021_157();
	void decode_I021_160();
	void decode_I021_165();
	void decode_I021_170();

	void decode_I021_200();
	void decode_I021_210();
	void decode_I021_220();
	void decode_I021_230();	

	void decode_I021_910();
	void decode_I021_920();
	void decode_I021_930();

	void decode_I048_010();
	void decode_I048_020();
	void decode_I048_030();
	void decode_I048_040();
	void decode_I048_042();
	void decode_I048_050();
	void decode_I048_055();
	void decode_I048_060();
	void decode_I048_065();
	void decode_I048_070();
	void decode_I048_080();
	void decode_I048_090();
	void decode_I048_100();
	void decode_I048_110();
	void decode_I048_120();
	void decode_I048_130();
	void decode_I048_140();
	void decode_I048_170();
	void decode_I048_161();
	void decode_I048_200();
	void decode_I048_210();
	void decode_I048_220();
	void decode_I048_230();
	void decode_I048_240();
	void decode_I048_250();	
	void decode_I048_260();	
	
	UCHAR decode_I034_000();
	void decode_I034_010();
	void decode_I034_020();
	void decode_I034_030();
	void decode_I034_041();
	void decode_I034_050();
	void decode_I034_060();
	void decode_I034_070();
	void decode_I034_090();
	void decode_I034_100();
	void decode_I034_110();
	void decode_I034_120();	



};
