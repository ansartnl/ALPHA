#pragma once

#include "Afxmt.h"
#include "MRTAm.h"
#include "MRTNorth.h"
#include "MRTPlot.h"
#include "MRTTrack.h"
#include "MRTPeleng.h"
#include "Meteo_polygon.h"
#include "MeteoRW.h"

class CCodeAsterix
{
public:
	CCodeAsterix(void);
	~CCodeAsterix(void);

	unsigned char pbuf_out[4096];
	int index;

	UCHAR CAT;
	__int16 LEN;
	UCHAR FSPEC[15];

	CCriticalSection LockCode;
	
	int MakeAM(CMRTAm &Am);
	int MakeNorth(CMRTNorth &North);
	int MakePlot(CMRTPlot &Plot);
	int MakePlot21(CMRTPlot& Plot);
	int MakeTrack62(CMRTTrack &Track, BOOL bWriteFPL=FALSE, BOOL bUseExtrCoord=FALSE);
	int MakePeleng(CMRTPeleng & Peleng);

	// CAT 241
	int MakeMeteoPolygon(CMeteo_polygon & MeteoPolygon);
	int MakeMeteoRW1(CMeteoRW & MeteoRW);
	int MakeMeteoRW2(CMeteoRW & MeteoRW);

	// CAT 242
	int MakeTime(CMRTTime &Time);
	
	void WriteByte(unsigned char c);
	void WriteInt16(__int16 i);
	void WriteInt24(long l);
	void WriteInt32(long l);
	void WriteInt24_2(__int32 l);
	void WriteInt64(__int64 i64);

	void WriteString(CString& str, int n);
	void AddCRC(void);
};
