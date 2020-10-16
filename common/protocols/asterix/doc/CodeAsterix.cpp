#include "StdAfx.h"
#include ".\codeasterix.h"
#include "Geoid.h"
#include "CRC_16_ASTERIX.h"

#include "math.h"

#include "Otladka.h"

CCodeAsterix::CCodeAsterix(void)
{
}

CCodeAsterix::~CCodeAsterix(void)
{
}

void CCodeAsterix::WriteByte(unsigned char c)
{
	if(index < 0 || index >= 4096) return;
	pbuf_out[index++] = c;
}

void CCodeAsterix::WriteInt16(__int16 i)
{
	UCHAR c;

	c=(UCHAR)(i>>8);
	WriteByte(c);

	c=(UCHAR)(i & 0x00FF);
	WriteByte(c);
}

void CCodeAsterix::WriteInt24(long l)
{
	UCHAR c;

	c=(UCHAR)(l>>16);
	WriteByte(c);

	c=(UCHAR)( (l & 0x00FF00)>>8);
	WriteByte(c);

	c=(UCHAR)( l & 0x0000FF );
	WriteByte(c);
}
void CCodeAsterix::WriteInt24_2(__int32 l)
{
	UCHAR c;

	c=(UCHAR)(l  >> 24);
	WriteByte(c);

	c=(UCHAR)( (l & 0xFF0000 ) >> 16);
	WriteByte(c);

	c=(UCHAR)( (l & 0x00FF00) >> 8 );
	WriteByte(c);
}

void CCodeAsterix::WriteInt32(long l)
{
	UCHAR c;

	c=(UCHAR)(l>>24);
	WriteByte(c);

	c=(UCHAR)( (l & 0x00FF0000)>>16);
	WriteByte(c);

	c=(UCHAR)( (l & 0x0000FF00)>>8 );
	WriteByte(c);

	c=(UCHAR)( l & 0x000000FF );
	WriteByte(c);
}

void CCodeAsterix::WriteInt64(__int64 i64)
{
	UCHAR c;

	c=(UCHAR)(  i64>>56                      );WriteByte(c);
	c=(UCHAR)( (i64 & 0x00FF000000000000)>>48);WriteByte(c);
	c=(UCHAR)( (i64 & 0x0000FF0000000000)>>40);WriteByte(c);
	c=(UCHAR)( (i64 & 0x000000FF00000000)>>32);WriteByte(c);
	c=(UCHAR)( (i64 & 0x00000000FF000000)>>24);WriteByte(c);
	c=(UCHAR)( (i64 & 0x0000000000FF0000)>>16);WriteByte(c);
	c=(UCHAR)( (i64 & 0x000000000000FF00)>>8 );WriteByte(c);
	c=(UCHAR)( (i64 & 0x00000000000000FF)    );WriteByte(c);
}

void CCodeAsterix::WriteString(CString& str, int n)
{
	int i, length;

	length=str.GetLength();

	if(length>=n)
	{
		for(i=0; i<n; i++) WriteByte(str[i]);
	}
	else
	{
		for(i=0; i<length; i++) WriteByte(str[i]);
		for(i=length; i<n; i++) WriteByte(' ');
	}
}

int CCodeAsterix::MakeAM(CMRTAm &Am)
{
	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(0x02);
	
	//LEN
	WriteInt16(0x00);

	//FSPEC
	WriteByte(0xF0);	//Data Source    I002/010
						//Message Type	 I002/000
						//Sector Number  I002/020
						//Time of Day    I002/030

	// I002/010
	WriteByte(Am.SAC_out);
	WriteByte(Am.SIC_out);

	// I002/000
	WriteByte(0x02);

	// I002/020
	UCHAR SN;//Sector Number
	SN=(UCHAR)floor((Am.f_AM+0.5)*256.0/360.0);	// Lsb=360/256
	WriteByte(SN);

	// I002/030;
	double dt;
	dt=Am.time.m_time.wHour*3600 + Am.time.m_time.wMinute*60 + Am.time.m_time.wSecond + Am.time.m_time.wMilliseconds*0.001;
	long lt;
	lt=(long)(dt*128);
	WriteInt24(lt);

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();

	return 0;
}

int CCodeAsterix::MakeNorth(CMRTNorth &North)
{
	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(0x02);
	
	//LEN
	WriteInt16(0x00);

	//FSPEC
	WriteByte(0xD0);	//Data Source    I002/010
						//Message Type	 I002/000
						//				 I002/020
						//Time of Day    I002/030

	// I002/010
	WriteByte(North.SAC_out);
	WriteByte(North.SIC_out);

	// I002/000
	WriteByte(0x01);

	// I002/030;
	double dt;
	dt=North.time.m_time.wHour*3600 + North.time.m_time.wMinute*60 + North.time.m_time.wSecond + North.time.m_time.wMilliseconds*0.001;
	long lt;
	lt=(long)(dt*128);
	WriteInt24(lt);

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

int CCodeAsterix::MakePlot(CMRTPlot &Plot)
{	
	if(Plot.IKI==0x08)
	{
		return MakePlot21(Plot);
	}
	
	
	index=0;
	int i;
	UCHAR ucFSPEC[3];
	for(i=0; i<3; i++) ucFSPEC[i]=0x00;

	CSingleLock Lock(&LockCode, true);

	double x, y, rho, theta;

	if(Plot.OriginCoord == 0)
	{
		x=Plot.x;
		y=Plot.y;

		rho = sqrt(x*x + y*y);
		theta = atan2(x,y);
		
		if(theta<0)
		{
			theta=theta+2*c_PI;
		}
		theta = theta * 180.0/c_PI;
	}

	if(Plot.OriginCoord == 1)
	{
		rho=Plot.d_D;
		theta=Plot.d_Az;
	}

	if((rho*128.0/1852)>65535) return -1;

//	if((Plot.d_D+1852.0/(128*2))>=(1852.0*65535/128)) return -1;//Too FAR for ASTERIX

	
	//Category
	WriteByte(0x01);
	
	//LEN
	WriteInt16(0x00);

	
	//FSPEC
	ucFSPEC[0]=0xE0;	//Data Source    I001/010
						//Target Report  I001/020
						//Position in Polar Coord I001/040
	if(Plot.IKI & 0x02)//ВРЛ
	{	
		if(Plot.MK)//RBS
		{		
			if(Plot.EXNB)
				ucFSPEC[0]=ucFSPEC[0] | 0x10;	//Mode3/A I001/070
			
			if(Plot.EXH)
				ucFSPEC[0]=ucFSPEC[0] | 0x08;	//Mode C I001/090			
		}
		else{				//УВД
			ucFSPEC[0] = ucFSPEC[0] | 0x01;//FX
			ucFSPEC[1] = ucFSPEC[1] | 0x01;//FX
			ucFSPEC[2] = ucFSPEC[2] | 0x04;//SP I001/SP
		}
	}
	if(Plot.time.GetMS()>0) ucFSPEC[0]=ucFSPEC[0] | 0x02;	//Truncated Time of Day I001/141


	WriteByte(ucFSPEC[0]);
	if(ucFSPEC[0] & 0x01) WriteByte(ucFSPEC[1]);
	if(ucFSPEC[1] & 0x01) WriteByte(ucFSPEC[2]);
						

	// I001/010
	if(ucFSPEC[0] & 0x80)
	{
		WriteByte(Plot.SAC_out);
		WriteByte(Plot.SIC_out);
	}

	// I001/020
	if(ucFSPEC[0] & 0x40)
	{
		UCHAR b1, b2;
		b1=b2=0;

		//PLOT
		//
		//SIM
		if(Plot.flag & 0x02) b1=b1 | 0x40;
		//SSR/PSR
		b1=b1 | ((Plot.IKI & 0x03)<<4);
		//ANT
		if(Plot.ANT) b1 = b1 | 0x08;//Глиссада
		else b1 = b1 & (~0x08);//Курс
		//b1=b1&(~0x08);
		
		//SPI
		if(Plot.SPI) b1=b1 | 0x04;
		//RAB
		//
		if( (Plot.flag & 0x01) || (Plot.b))
		{
			//FX
			b1=b1 | 0x01;
			//TST
			if(Plot.flag & 0x01) b2=b2 | 0x80;
			if(Plot.b)
			{
				UCHAR	B;

				switch(Plot.b)
				{
					case 1:	B = 3; break;
					case 2: B = 2; break;
					case 3: B = 1; break;
				}
				b2 = b2 | (B << 5);
			}
		}

		WriteByte(b1);
		if(b1 & 0x01) WriteByte(b2);
	}

	// I001/040
	if(ucFSPEC[0] & 0x20)
	{
		long Rho, Theta;

		Rho=long( (rho * 128.0 / 1852.0)/* + 0.5 */);			
		Theta=long( (theta * 65536.0 / 360.0)/* + 0.5 */);

//		Rho=long( (Plot.d_D * 128.0 / 1852.0) + 0.5 );
//		Theta=long( (Plot.d_Az * 65536.0 / 360.0) + 0.5 );

		WriteInt16(__int16(Rho));
		WriteInt16(__int16(Theta));
	}

	//I001/070
	if(ucFSPEC[0] & 0x10)
	{
		UCHAR a, b, c, d;
		__int16 i16;
		long NB;
		NB=Plot.number;
		d=(UCHAR)(NB%10);
		NB=NB/10;
		c=(UCHAR)(NB%10);
		NB=NB/10;
		b=(UCHAR)(NB%10);
		NB=NB/10;
		a=(UCHAR)(NB%10);

		i16=0;
		i16=((a&0x07)<<9) + ((b&0x07)<<6) + ((c&0x07)<<3) + (d&0x07);

		WriteInt16(i16);
	}

	//I001/090
	if(ucFSPEC[0] & 0x08)
	{
		__int16 i16;
		//25футов != 7,62м
		// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
		double dH;
		dH = Plot.heigh;
		//dH = dH*6080.0/(1852.0*25.0);
		dH = dH/(0.3048*25.0);
		double znH = 0.5;
		if(dH<0) znH = -0.5;
		i16=__int16(dH + znH);
			
		i16=(i16 & 0x3FFF);
		WriteInt16(i16);
	}

	// I001/141;
	if(ucFSPEC[0] & 0x02)
	{
		double dt;
		dt = Plot.time.m_time.wHour*3600 + 
			 Plot.time.m_time.wMinute*60 + 
			 Plot.time.m_time.wSecond + 
			 Plot.time.m_time.wMilliseconds*0.001;

		__int16 i16t;
		i16t=(__int16)(dt*128);
		WriteInt16(i16t);
	}

	if(ucFSPEC[2] & 0x04)
	{
		int index_l;
		UCHAR FSPEC_SP;
		index_l=index;
		WriteByte(0x00);//Length of SP
		//FSPEC_SP
		FSPEC_SP=0;
		if(Plot.EXNB)   FSPEC_SP=FSPEC_SP | 0x80;
		if(Plot.EXH)    FSPEC_SP=FSPEC_SP | 0x40;
		//if(Plot.ot!=0)  
		FSPEC_SP=FSPEC_SP | 0x20;//FUEL
		if(Plot.b)      FSPEC_SP=FSPEC_SP | 0x10;//Emergency
		if(Plot.BRF>=0) FSPEC_SP=FSPEC_SP | 0x08;//BRF

		WriteByte(FSPEC_SP);
		
		//I001/N01
		if(FSPEC_SP & 0x80)
		{
			long NB;
			NB = Plot.number;
			WriteInt24(NB);
		}

		//I001/N02
		if(FSPEC_SP & 0x40)
		{
			__int16 i16;
			//25футов != 7,62м
			// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
			double dH;
			dH = Plot.heigh;
			//dH = dH*6080.0/(1852.0*25.0);
			dH = dH/(0.3048*25.0);
			double znH = 0.5;
			if(dH<0) znH = -0.5;
			i16=__int16(dH + znH);
			
			i16=(i16 & 0x3FFF);
			if(Plot.ZH == 0) i16 = i16 | 0x4000;//Относительная высота
//			if(dH<0) i16 = 0x8000;
			WriteInt16(i16);
		}

		//I001/N03
		if(FSPEC_SP & 0x20)
		{
			UCHAR b;
			CMyMath MM;
			b = MM.FourBits_to_BinOT(Plot.ot);
			WriteByte(b);
		}

		//I001/N04
		if(FSPEC_SP & 0x10)
		{
			UCHAR b;
			//Астрахань
			if(Plot.b) b=0x80;
			else b=0x00;
			WriteByte(b);
		}

		//I001/N05 BRF
		if(FSPEC_SP & 0x08)
		{
			UCHAR brf;
			if(Plot.BRF < 0) brf = 0xFF;
			else brf = UCHAR(Plot.BRF);
			WriteByte(brf);
		}
		
		//Length SP
		UCHAR l;
		l = index-index_l;
		pbuf_out[index_l] = l;

	}

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

int CCodeAsterix::MakeTrack62(CMRTTrack &Track, BOOL bWriteFPL, BOOL bUseExtrCoord)
{
	int i;
	index=0;
	UCHAR ucFSPEC[5];
	for(i = 0; i < 5; i++) ucFSPEC[i] = 0x00;

	CSingleLock Lock(&LockCode, true);

	byte ValidNB, ValidH;
	ValidNB = Track.estm_Plot[0].ValidNB;
	ValidH = Track.estm_Plot[0].ValidH;

	//Category
	WriteByte(62);
	
	//LEN
	WriteInt16(0x00);

	//FSPEC
	//0
    ucFSPEC[0] |= 0x80;  //Data Source I062/010
	//			  0x40 - Spare
	ucFSPEC[0] |= 0x20;  //Service Identification I062/015
	ucFSPEC[0] |= 0x10;  //Time Of Track Information I062/070
	//ucFSPEC[0] |= 0x08;  //Calculated Track Position (WGS-84) I062/105
	//ucFSPEC[0] |= 0x04;  //Calculated Track Position (WGS-84) I062/105
	ucFSPEC[0] |= 0x02;  //Calculated Track Velocity (Cartesian) I062/185
	if (Track.estm_Plot[0].OriginCoord == 2)
	{
		ucFSPEC[0] |= 0x08;  //Calculated Track Position (WGS-84) I062/105
	}
	else
	{
		ucFSPEC[0] |= 0x04;  //Calculated Track Position (WGS-84) I062/105
	}
	//1
	ucFSPEC[1] |= 0x80;  //Calculated Track Acceleration (Cartesian) I062/210
	if(Track.estm_Plot[0].MK && Track.estm_Plot[0].EXNB)
		ucFSPEC[1] |= 0x40;  //Mode 3/A I062/060	

	if((Track.estm_Plot[0].Address_ICAO > 0) || (Track.estm_Plot[0].Ident.GetLength() > 0))
		ucFSPEC[1] |= 0x10;  //AirCraft Derived Data I062/380	

	ucFSPEC[1] |= 0x08;  //Track Number I062/040
	ucFSPEC[1] |= 0x04;  //Track Status I062/080

	//2
	if(Track.estm_Plot[0].MK && Track.estm_Plot[0].EXH)
		ucFSPEC[2] |= 0x20;  //Measured FL I062/136
	
	ucFSPEC[2] |= 0x04;  //Calculated Rate Of Climb I062/220

	if(bWriteFPL)
		ucFSPEC[2] |= 0x02;  //Flight Plan Related Data I062/390
	
	//3
	ucFSPEC[3]=0x00;//Not used

	//4
	if(!Track.estm_Plot[0].MK || bWriteFPL || (!ValidNB) || (!ValidH) || (Track.l_H_corrected>-10000))
		ucFSPEC[4] = 0x02;	//SP I062/SP

	//Field Extention 
	if(ucFSPEC[4]) ucFSPEC[3]|=0x01;
	if(ucFSPEC[3]) ucFSPEC[2]|=0x01;
	if(ucFSPEC[2]) ucFSPEC[1]|=0x01;
	if(ucFSPEC[1]) ucFSPEC[0]|=0x01;

	WriteByte(ucFSPEC[0]);
	if(ucFSPEC[0] & 0x01) WriteByte(ucFSPEC[1]);
	if(ucFSPEC[1] & 0x01) WriteByte(ucFSPEC[2]);
	if(ucFSPEC[2] & 0x01) WriteByte(ucFSPEC[3]);
	if(ucFSPEC[3] & 0x01) WriteByte(ucFSPEC[4]);

	
	if(ucFSPEC[0] & 0x80)//Data Source I062/010
	{
		WriteByte(Track.estm_Plot[0].SAC);
		WriteByte(Track.estm_Plot[0].SIC);
	}

	if(ucFSPEC[0] & 0x20) //Service Identification I062/015
		WriteByte(0x00);
	
	if(ucFSPEC[0] & 0x10)//Time Of Track Information I062/070
	{
		double dt;
		dt=Track.estm_Plot[0].time.m_time.wHour*3600 + 
			Track.estm_Plot[0].time.m_time.wMinute*60 + 
			Track.estm_Plot[0].time.m_time.wSecond + 
			Track.estm_Plot[0].time.m_time.wMilliseconds*0.001;
		long lt;
		lt=(long)(dt*128);
		WriteInt24(lt);
	}

	CGeoid Gd;

	if(ucFSPEC[0] & 0x08)//Calculated Track Position (WGS-84) I062/105
	{
		double dB, dL, dH;

		if(bUseExtrCoord == FALSE)
			Track.estm_Plot[0].GetBLH(dB, dL, dH);
		else
			Track.extr_Plot[0].GetBLH(dB, dL, dH);
		Track.estm_Plot[0].GetBLH(dB, dL, dH);
		
		Gd.SetBasePoint_Geoid(dB, dL, dH, 0.0);

		long lB, lL;

		lB=(long)((dB*(1024*1024*32))/180.0);
		lL=(long)((dL*(1024*1024*32))/180.0);

		WriteInt32(lB);
		WriteInt32(lL);		
	}

	if(ucFSPEC[0] & 0x04)//Calculated Track Position (WGS-84) I062/105
	{
		__int32 l_X, l_Y;
		l_X = Track.estm_Plot[0].x;
		l_Y = Track.estm_Plot[0].y;
		//l_X = -123456789;
		//l_Y = 123456789;
		l_X *= 512;
		l_Y *= 512;
		WriteInt24_2(l_X);
		WriteInt24_2(l_Y);
	}
	if(ucFSPEC[0] & 0x02)//Calculated Track Velocity (Cartesian) I062/185
	{
		double dVx, dVy;

		dVx = Track.Vx;//=Gd.e_x0 * Track.estm_V[0];
		dVy = Track.Vy;//=Gd.e_y0 * Track.estm_V[0];

		__int16 iVx, iVy;

		iVx=(__int16)(dVx/0.25);
		iVy=(__int16)(dVy/0.25);

		WriteInt16(iVx);
		WriteInt16(iVy);		
	}

	if(ucFSPEC[1] & 0x80)//Calculated Acceleration (Cartesian) I062/210
	{
		double dAx, dAy;

		dAx=Gd.e_x0 * Track.estm_A[0];
		dAy=Gd.e_y0 * Track.estm_A[0];

		BYTE ucAx, ucAy;

		ucAx=(byte)(dAx/0.25);
		ucAy=(byte)(dAy/0.25);

		WriteByte(ucAx);
		WriteByte(ucAy);
		
	}

	if(ucFSPEC[1] & 0x40)//Mode 3/A I062/060
	{
		UCHAR a, b, c, d;
		__int16 i16;
		long NB;
		NB=Track.estm_Plot[0].number;

		d=(UCHAR)(NB%10);
		NB=NB/10;
		c=(UCHAR)(NB%10);
		NB=NB/10;
		b=(UCHAR)(NB%10);
		NB=NB/10;
		a=(UCHAR)(NB%10);

		i16=0;
		i16=((a&0x07)<<9) + ((b&0x07)<<6) + ((c&0x07)<<3) + (d&0x07);

		WriteInt16(i16);
	}

	if(ucFSPEC[1] & 0x10)//Aircraft Derived Data I062/380
	{
		UCHAR SubField1, ADR, ID;
		SubField1=0x00;
		if(Track.estm_Plot[0].Address_ICAO>0) ADR=1;
		else ADR=0;
		if(Track.estm_Plot[0].Ident.GetLength()>0) ID=1;
		else ID=0;
		
		if(ADR) SubField1 = SubField1 | 0x80;
		if(ID) SubField1 = SubField1 | 0x40;

		WriteByte(SubField1);
		if(ADR) WriteInt24(Track.estm_Plot[0].Address_ICAO);
		
		if(ID)
		{
			CString ident;
			char ch[8];
			int lenght, i;
			ident=Track.estm_Plot[0].Ident;
			lenght=ident.GetLength();

			for(i=0; i<lenght; i++)
			{
				CMyMath MM;
				ch[i]=MM.Char_to_ICAOChar(ident[i]);
			}

			if(lenght<8)
			{
				for(i=lenght; i<8; i++) ch[i]=' ';
			}

			UCHAR b1=0, b2=0, b3=0, b4=0, b5=0, b6=0;
			
			b1=(((ch[0]&0x3F)<<2)&0xFC);
			b2=(((ch[1]&0x30)>>4)&0x03);
			b1 = b1 | b2;
			WriteByte(b1);

			b1=0;
			b2=0;
			b1=(((ch[1]&0x0F)<<4)&0xF0);
			b2=(((ch[2]&0x3C)>>2)&0x0F);
			b1 = b1 | b2;
			WriteByte(b1);

			b1=0;
			b2=0;
			b1=(((ch[2]&0x03)<<6)&0xC0);
			b2=(ch[3]&0x3F);
			b1 = b1 | b2;
			WriteByte(b1);

			b1=0;
			b2=0;
			b1=(((ch[4]&0x3F)<<2)&0xFC);
			b2=(((ch[5]&0x30)>>4)&0x03);
			b1 = b1 | b2;
			WriteByte(b1);

			b1=0;
			b2=0;
			b1=(((ch[5]&0x0F)<<4)&0xF0);
			b2=(((ch[6]&0x3C)>>2)&0x0F);
			b1 = b1 | b2;
			WriteByte(b1);

			b1=0;
			b2=0;
			b1=(((ch[6]&0x03)<<6)&0xC0);
			b2=(ch[7]&0x3F);
			b1 = b1 | b2;
			WriteByte(b1);
		}
	}

	if(ucFSPEC[1] & 0x08)//Track Number I062/040
	{
		__int16 i16;
		i16=Track.index;
		WriteInt16(i16);
	}

	if(ucFSPEC[1] & 0x04)//Track Status I062/080
	{
		UCHAR MON, SPI, MRH, SRC, CNF, SIM, TSE, TSB, FPC, AFF, STP, AMA, MD4, ME, MI, MD5, CST, PSR, SSR, MDS, ADS, SUC, AAC;
		MON=0x00;
		SPI=Track.estm_Plot[0].SPI;
		MRH=0x00;
		SRC=0x00;
		CNF=0x00;
		if(Track.estm_Plot[0].flag & 0x02) SIM=1;
		else SIM=0x00;
		TSE=Track.b_TSE;
		TSB=Track.b_TSB;
		FPC=0x00;
		AFF=0x00;
		STP=0x00;
		AMA=0x00;
		MD4=0x00;
		ME=0x00;
		MI=0x00;
		MD5=0x00;
		CST=0x00;
		if(Track.estm_Plot[0].IKI & 0x01) PSR=0;
		else PSR=1;
		if(Track.estm_Plot[0].IKI & 0x02) SSR=0;
		else SSR=1;
		if(Track.estm_Plot[0].IKI & 0x04) MDS=0;
		else MDS=1;
		if(Track.estm_Plot[0].IKI & 0x08) ADS=0;
		else ADS=1;
		SUC=0x00;
		AAC=0x00;

		UCHAR b0, b1, b2, b3;
		b0=b1=b2=b3=0;
		//0
		if(MON) b0|=0x80;
		if(SPI) b0|=0x40;
		if(MRH) b0|=0x20;
		b0|=(SRC&0x07)<<2;
		if(CNF) b0|=0x02;
		b0|=0x01;

		//1
		if(SIM) b1|=0x80;
		if(TSE) b1|=0x40;
		if(TSB) b1|=0x20;
		if(FPC) b1|=0x10;
		if(AFF) b1|=0x08;
		if(STP) b1|=0x04;
		b1|=0x01;

		//2
		if(AMA) b2|=0x80;
		b2|=(MD4&0x03)<<6;
		if(ME) b2|=0x10;
		if(MI) b2|=0x08;
		b2|=(MD5&0x03)<<1;
		b2|=0x01;

		//3
		if(CST) b3|=0x80;
		if(PSR) b3|=0x40;
		if(SSR) b3|=0x20;
		if(MDS) b3|=0x10;
		if(ADS) b3|=0x08;
		if(SUC) b3|=0x04;
		if(AAC) b3|=0x02;

		WriteByte(b0);
		if(b0 & 0x01) WriteByte(b1);
		if(b1 & 0x01) WriteByte(b2);
		if(b2 & 0x01) WriteByte(b3);
	}

	if(ucFSPEC[2] & 0x20)//Measured Flight Level I062/136
	{
		// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров

		__int16 i16;
		double dH = Track.estm_Plot[0].heigh;

		//dH *= 3.281;
		//dH += 1500.0;
		//dH /= 25;
		//i16 = __int16(dH + 0.5);
	

		dH = dH/(0.3048*25.0);
		double znH = 0.5;
		if(dH<0) znH = -0.5;
		i16=__int16(dH + znH);

		WriteInt16(i16);
	}

	if(ucFSPEC[2] & 0x04)//Calculated Rate Of Climb I062/220
	{
		__int16 i16;
		// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
		double dV;
		dV=Track.v_vel;
		//dV=dV*6080.0/(1852.0*6.25);
		dV = dV/(0.3048*6.25);
		i16=__int16(dV+0.5);
		
		WriteInt16(i16);
	}

	if(ucFSPEC[2] & 0x02)//Flight Plan Related Data I062/390
	{
		//Primary Subfield
		UCHAR uc;

		CString strFlight;	strFlight.Empty();
		UCHAR ucPppPvp;		ucPppPvp=' ';
		UCHAR ucRVSM;
		CString strTypeFV;	strTypeFV.Empty();
		UCHAR ucWeight;		ucWeight=' ';
		CString strAPSrc;	strAPSrc.Empty();
		CString strAPDest;	strAPDest.Empty();
		long lGFL;			lGFL=-1;
		UCHAR ucSNGMX;		ucSNGMX=' ';
		CMRTTime	ctTime;//(1971,1,1,0,0,0);

		if(Track.m_pFpl != NULL)
		{
			strFlight	= Track.m_pFpl->m_strFlight;
			ucPppPvp	= Track.m_pFpl->m_ucPppPvp;
			ucRVSM		= Track.m_pFpl->m_ucRVSM;
			strTypeFV	= Track.m_pFpl->m_strTypeFV;
			ucWeight	= Track.m_pFpl->m_ucWeight;
			strAPSrc	= Track.m_pFpl->m_strAPSrc;
			strAPDest	= Track.m_pFpl->m_strAPDest;
			//lGFL		= Track.m_pFpl->m_lGFL;
			ucSNGMX		= Track.m_pFpl->m_ucSNGMX;
			ctTime		= Track.m_pFpl->m_cgpPointIn.m_ctTime;
		}
		lGFL = Track.l_given_H;
		
		uc=0x5F;
		WriteByte(uc);
		uc=0xA8;
		WriteByte(uc);

		// #2 Callsign
		WriteString(strFlight, 7);

		// #4 flight Category
		uc=0x20;
		if(ucPppPvp==1) uc=0x00;//PPP
		if(ucPppPvp==2) uc=0x10;//PVP

		uc = uc | ((ucRVSM & 0x03) << 2);
		WriteByte(uc);

		// #5 Type of Aircraft
		WriteString(strTypeFV, 4);

		// #6 Wake Turbulence Category
		// H = 1, M = 2, L = 3
		uc=' ';
		if(ucWeight==1) uc='H'; 
		if(ucWeight==2) uc='M';
		if(ucWeight==3) uc='L';
		WriteByte(uc);

		// #7 Departure Airport
		WriteString(strAPSrc, 4);

		// #8 Destination Airport
		WriteString(strAPDest, 4);

		// #10 Current Cleared Flight Level
		__int16 i16;
		double dH;
		dH=lGFL;
		//dH=dH*6080.0/(1852*25);	// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
		dH = dH/(0.3048*25.0);
		i16=__int16(dH+0.5);
		if(lGFL<0) i16=-1;
		WriteInt16(i16);

		// #12 Time of Departure/Arrival
		//Rep
		// S = 1, N = 2, G = 3, M = 4, X = 5
		/*uc=' ';
		if(ucSNGMX==1) uc='S'; 
		if(ucSNGMX==2) uc='N'; 
		if(ucSNGMX==3) uc='G'; 
		if(ucSNGMX==4) uc='M'; 
		if(ucSNGMX==5) uc='X'; 
		WriteByte(uc);
		*/
		WriteByte(0);//выше было изложени неверное использование REP

		//TYP/DAY
		SYSTEMTIME st;
		st=ctTime.m_time;//.GetAsSystemTime(st);
		int nHourIn = st.wHour;
		int nMinuteIn = st.wMinute;
		int nYear = st.wYear;
		UCHAR TYP, DAY;
		TYP=8;
		DAY=0x03;
		uc=0;
		uc=((TYP & 0x1F)<<3) | ((DAY & 0x03)<<1);
		WriteByte(uc);

		//HOR
		uc=0;
		uc=nHourIn & 0x1F;
		WriteByte(uc);

		//MIN
		uc=0;
		uc=nMinuteIn & 0x3F;
		WriteByte(uc);

		//SEC
		uc=0x80;	//Seconds not available
		WriteByte(uc);		
	}

	if(ucFSPEC[4] & 0x02)//SP Special Purpose Indicator I062/SP
	{
		int index_l;
		UCHAR FSPEC_SP, FSPEC_SP_1;

		index_l = index;
		WriteByte(0x00);//Length of SP
		//FSPEC_SP
		FSPEC_SP = 0;
		FSPEC_SP_1 = 0;

		if(!Track.estm_Plot[0].MK)
		{
			if(Track.estm_Plot[0].EXNB)   FSPEC_SP=FSPEC_SP | 0x80;
			if(Track.estm_Plot[0].EXH)    FSPEC_SP=FSPEC_SP | 0x40;
			//if(Track.estm_Plot[0].ot!=0)  FSPEC_SP=FSPEC_SP | 0x20;//FUEL
			FSPEC_SP=FSPEC_SP | 0x20;//FUEL будем писать всегда, чтобы отличить УВД от RBS даже если нет ни номера ни высоты
		}
		if(Track.estm_Plot[0].b)      FSPEC_SP=FSPEC_SP | 0x10;//Emergency
		if(Track.estm_Plot[0].BRF >= 0) FSPEC_SP=FSPEC_SP | 0x08;//BRF
		if(ucFSPEC[2] & 0x02)	//Flight Plan Related Data I062/390
			FSPEC_SP=FSPEC_SP | 0x04;//FPL status
		if(!ValidNB || !ValidH) FSPEC_SP |= 0x02;//Not Valid H or NB

		if(Track.l_H_corrected > -10000)
		{
			FSPEC_SP |= 0x01;
			FSPEC_SP_1 |= 0x80;
		}

		WriteByte(FSPEC_SP);
		if(FSPEC_SP & 0x01) WriteByte(FSPEC_SP_1);
		
		//I001/N01
		if(FSPEC_SP & 0x80)
			WriteInt24(Track.estm_Plot[0].number);

		//I001/N02
		if(FSPEC_SP & 0x40)
		{
			// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
			__int16 i16;
			double dH = Track.estm_Plot[0].heigh;

			//dH = dH * 6080.0 / (1852.0 * 25.0);
			//i16 = __int16(dH + 0.5);

			/*dH *= 3.281;
			dH += 1500.0;
			dH /= 25;
			i16 = __int16(dH + 0.5);
			*/

			dH = dH/(0.3048*25.0);
			double znH = 0.5;
			if(dH<0) znH = -0.5;
			i16=__int16(dH + znH);
			
			i16 &= 0x3FFF;
			if(Track.estm_Plot[0].ZH == 0)	//Относительная высота
				i16 |= 0x4000;
			WriteInt16(i16);
		}

		//I001/N03
		if(FSPEC_SP & 0x20)
		{
			UCHAR f;
			f=Track.estm_Plot[0].ot;
			WriteByte(f);
		}

		//I001/N04
		if(FSPEC_SP & 0x10)
		{
			UCHAR b;
			if(Track.estm_Plot[0].b) b=0x80;
			else b=0x00;
			WriteByte(b);
		}

		//I001/N05 BRF
		if(FSPEC_SP & 0x08)
		{
			UCHAR brf;
			if(Track.estm_Plot[0].BRF<0) brf=0xFF;
			else brf=UCHAR(Track.estm_Plot[0].BRF);
			WriteByte(brf);
		}

		//I001/N06 FPL status
		if(FSPEC_SP & 0x04)
		{
			UCHAR status=0;
			if(Track.m_pFpl!=NULL)
			{
				status=Track.m_pFpl->m_ucPlanState;
			}
			WriteByte(status);
		}

		if(FSPEC_SP & 0x02)//Not Valid H or NB
		{ 
			UCHAR status=0;
			if(!ValidNB) status=status | 0x01;
			if(!ValidH) status=status | 0x02;

			WriteByte(status);
		}

		if(FSPEC_SP_1 & 0x80) //QNH corrected height
		{			
			UCHAR uc = Track.uc_H_corrected_Type;
			WriteByte(uc);
			
			// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
			__int16 i16;
			double dH = Track.l_H_corrected;

			//dH = dH * 6080.0 / (1852 * 25);
			//i16 = __int16(dH + 0.5);

			/*dH *= 3.281;
			dH += 1500.0;
			dH /= 25;
			i16 = __int16(dH + 0.5);
			*/

			dH = dH/(0.3048*25.0);
			double znH = 0.5;
			if(dH<0) znH = -0.5;
			i16=__int16(dH + znH);
			
			i16 = (i16 & 0x3FFF);
			if(Track.estm_Plot[0].ZH == 0)
				i16 |= 0x4000;//Относительная высота
			WriteInt16(i16);
		}
		
		//Length SP
		pbuf_out[index_l] = index - index_l;
	}

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

	return 0;
}

int CCodeAsterix::MakePeleng(CMRTPeleng & Peleng)
{
	UCHAR ucFSPEC;

	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(253);

	//LEN
	WriteInt16(0x00);

	//FSPEC
	ucFSPEC=0xC0;	

	WriteByte(ucFSPEC);

	// I253/N01		// Метка идентификации источника пеленга
	if(ucFSPEC & 0x80)
	{
		WriteByte(Peleng.m_SAC);
		WriteByte(Peleng.m_SIC);
	}

	// I253/N02		// Направление на объект (Пеленг)
	if(ucFSPEC & 0x40)
	{
		WriteByte(Peleng.m_ucChanel + 1); // канал выдачи АРП (1-12)
		unsigned int i16Peleng;
		i16Peleng = (unsigned int)(Peleng.m_fPeleng*65536/360 + 0.5); // ЦМД =~0,0055
		WriteInt16(i16Peleng);
	}

	// I253/N03		// Каналы АРП включены
	if(ucFSPEC & 0x20)
	{
		// 2-а байта
	}

	// I253/N04		// Каналы АРП выключены
	if(ucFSPEC & 0x10)
	{
		// 2-а байта
	}
	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

int CCodeAsterix::MakeMeteoPolygon(CMeteo_polygon & MeteoPolygon)
{
	UCHAR ucFSPEC;

	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(241);

	//LEN
	WriteInt16(0x00);


	//FSPEC
	ucFSPEC=0xFE;	// Идентификатор источника данных	I241/010
	// Тип сообщения					I241/000
	// Номер сообщения					I241/600
	// Время суток						I241/030
	// Срок действия					I241/601
	// Тип контура						I241/602
	// Координаты (WGS-84)				I241/105

	WriteByte(ucFSPEC);
	// I241/010
	if(ucFSPEC & 0x80)
	{
		WriteByte(MeteoPolygon.SAC);
		WriteByte(MeteoPolygon.SIC);
	}

	// I241/000
	if(ucFSPEC & 0x40) WriteByte(1);

	// I241/600
	if(ucFSPEC & 0x20)
		WriteInt16(MeteoPolygon.m_MsgNumber);	

	// I241/030
	if(ucFSPEC & 0x10)
	{
		double dt;
		dt=MeteoPolygon.m_TimeCurr.m_time.wHour*3600 + MeteoPolygon.m_TimeCurr.m_time.wMinute*60 + MeteoPolygon.m_TimeCurr.m_time.wSecond + MeteoPolygon.m_TimeCurr.m_time.wMilliseconds*0.001;
		long lt;
		lt=(long)(dt*128);
		WriteInt24(lt);
	}

	// I241/601
	if(ucFSPEC & 0x08)
	{
		int dtGet, dtStop, dt;
		dtGet=int(MeteoPolygon.m_TimeCurr.m_time.wHour);
		dtStop=int(MeteoPolygon.m_TimeStop.m_time.wHour);
		if(dtGet>dtStop)
			dtStop=dtStop+24;
		dt=dtStop-dtGet;
		WriteByte(dt);
	}

	// I241/602
	if(ucFSPEC & 0x04)
	{
		WriteByte(UCHAR(MeteoPolygon.m_ContNumber));
		WriteByte(UCHAR(MeteoPolygon.m_codeEvent));
		__int16 h, v, a;
		long type=0;
		h=__int16(MeteoPolygon.m_HRadioecho);
		h=h & 0x03FF;
		v=__int16(MeteoPolygon.m_VelRadioecho);
		v=v & 0x03FF;
		a=__int16(MeteoPolygon.m_CourseRadioecho);
		a=a & 0x01FF;
		type = (h<<22) + (v<<12) + (a<<3);
		WriteInt32(type);
	}

	// I241/105
	if(ucFSPEC & 0x04)
	{
		int n;
		CGeo_point pgp;
		n=MeteoPolygon.GetNumberOfPoint();
		WriteByte(UCHAR(n));
		for(int i=0; i<n; i++)
		{
			pgp = MeteoPolygon.GetPoint(i);

			long lB, lL;
			lB=(long)((pgp.B*(1024*1024*32))/180.0);
			lL=(long)((pgp.L*(1024*1024*32))/180.0);

			WriteInt32(lB);
			WriteInt32(lL);
		}
	}

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

int CCodeAsterix::MakeMeteoRW1(CMeteoRW & MeteoRW)
{
	int i;
	UCHAR ucFSPEC[4];
	for(i=0; i<4; i++) ucFSPEC[i]=0x00;

	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(241);

	//LEN
	WriteInt16(0x00);

	//FSPEC
	ucFSPEC[0]=0xF9;	// Идентификатор источника данных	I241/010
	// Тип сообщения					I241/000
	// Номер сообщения					I241/600
	// Время суток						I241/030
	// Срок действия					I241/601
	// FX
	ucFSPEC[1] = 0xC0;
	if(MeteoRW.m_WindCourse>-1000)	ucFSPEC[1] = ucFSPEC[1] | 0x20;
	if(MeteoRW.m_WindEstm>-1000)	ucFSPEC[1] = ucFSPEC[1] | 0x10;
	if(MeteoRW.m_WindMax>-1000)		ucFSPEC[1] = ucFSPEC[1] | 0x08;
	if(MeteoRW.m_WindSide>-1000)	ucFSPEC[1] = ucFSPEC[1] | 0x04;
	if(MeteoRW.m_Temp>-1000)		ucFSPEC[1] = ucFSPEC[1] | 0x02;

	if(MeteoRW.m_DewPoint>-1000 || MeteoRW.m_Vis>-1000 || MeteoRW.m_DistanceVisBeg>-1000 || MeteoRW.m_DistanceVisMdl>-1000.0 || MeteoRW.m_DistanceVisEnd>-1000.0 || MeteoRW.m_HCloud>-1000 || MeteoRW.m_QFE>-1000 || MeteoRW.m_QFEh>-1000 || MeteoRW.m_QNHh>-1000)
	{
		ucFSPEC[1]=ucFSPEC[1] | 0x01;

		if(MeteoRW.m_DewPoint>-1000)		ucFSPEC[2] = ucFSPEC[2] | 0x80;
		if(MeteoRW.m_Vis>-1000)				ucFSPEC[2] = ucFSPEC[2] | 0x40;
		if(MeteoRW.m_DistanceVisBeg>-1000)	ucFSPEC[2] = ucFSPEC[2] | 0x20;
		if(MeteoRW.m_DistanceVisMdl>-1000)	ucFSPEC[2] = ucFSPEC[2] | 0x10;
		if(MeteoRW.m_DistanceVisEnd>-1000)	ucFSPEC[2] = ucFSPEC[2] | 0x08;
		if(MeteoRW.m_HCloud>-1000)			ucFSPEC[2] = ucFSPEC[2] | 0x04;
		if(MeteoRW.m_QFE>-1000)				ucFSPEC[2] = ucFSPEC[2] | 0x02;

		if(MeteoRW.m_QFEh>-1000 || MeteoRW.m_QNHh>-1000)
		{
			ucFSPEC[2]=ucFSPEC[2] | 0x01;

			if(MeteoRW.m_QFEh>-1000)	ucFSPEC[3] = ucFSPEC[3] | 0x80;
			if(MeteoRW.m_QNHh>-1000)	ucFSPEC[3] = ucFSPEC[3] | 0x40;
		}
	}

	WriteByte(ucFSPEC[0]);
	if(ucFSPEC[0] & 0x01) WriteByte(ucFSPEC[1]);
	if(ucFSPEC[1] & 0x01) WriteByte(ucFSPEC[2]);
	if(ucFSPEC[2] & 0x01) WriteByte(ucFSPEC[3]);


	// I241/010
	if(ucFSPEC[0] & 0x80)
	{
		WriteByte(MeteoRW.SAC);
		WriteByte(MeteoRW.SIC);
	}

	// I241/000
	if(ucFSPEC[0] & 0x40) WriteByte(2);

	// I241/600
	if(ucFSPEC[0] & 0x20)	
		WriteInt16(MeteoRW.m_MsgNumber);	

	// I241/030
	if(ucFSPEC[0] & 0x10)
	{
		double dt;
		dt=MeteoRW.m_TimeGet.m_time.wHour*3600 + MeteoRW.m_TimeGet.m_time.wMinute*60 + MeteoRW.m_TimeGet.m_time.wSecond + MeteoRW.m_TimeGet.m_time.wMilliseconds*0.001;
		long lt;
		lt=(long)(dt*128);
		WriteInt24(lt);
	}

	// I241/601
	if(ucFSPEC[0] & 0x08)
	{
		int dtGet, dtStop, dt;
		dtGet=int(MeteoRW.m_TimeGet.m_time.wHour);
		dtStop=int(MeteoRW.m_TimeStop.m_time.wHour);
		if(dtGet>dtStop)
			dtStop=dtStop+24;
		dt=dtStop-dtGet;
		WriteByte(dt);
	}

	// I241/603
	if(ucFSPEC[1] & 0x80)
	{
		WriteByte(MeteoRW.m_cICAO_Identifier[0]);
		WriteByte(MeteoRW.m_cICAO_Identifier[1]);
		WriteByte(MeteoRW.m_cICAO_Identifier[2]);
		WriteByte(MeteoRW.m_cICAO_Identifier[3]);
	}

	// I241/604
	if(ucFSPEC[1] & 0x40)
	{
		CString str="00";
		int cours;
		if(MeteoRW.m_strRWName.GetLength()>=4) str = MeteoRW.m_strRWName.Mid(2,3);
		cours = (str[0]-0x30)*10 + (str[1]-0x30);
		UCHAR b;
		b = (UCHAR(cours)<<2) & 0xFC;
		if(str[2]=='L') b = b | 0x01;
		if(str[2]=='R') b = b | 0x02;
		if(str[2]=='M') b = b | 0x03;
		WriteByte(b);
	}

	// I241/605
	if(ucFSPEC[1] & 0x20)
		WriteInt16(__int16(MeteoRW.m_WindCourse));

	// I241/606
	if(ucFSPEC[1] & 0x10)
		WriteByte(UCHAR(int(MeteoRW.m_WindEstm)));

	// I241/607
	if(ucFSPEC[1] & 0x08)
		WriteByte(UCHAR(int(MeteoRW.m_WindMax)));

	// I241/608
	if(ucFSPEC[1] & 0x04)
		WriteInt16(__int16(MeteoRW.m_WindSide*10));

	// I241/609
	if(ucFSPEC[1] & 0x02)
		WriteInt16(__int16(MeteoRW.m_Temp*10));

	// I241/610
	if(ucFSPEC[2] & 0x80)
		WriteInt16(__int16(MeteoRW.m_DewPoint*10));

	// I241/611
	if(ucFSPEC[2] & 0x40)
		WriteInt16(__int16(MeteoRW.m_Vis));

	// I241/612
	if(ucFSPEC[2] & 0x20)
		WriteInt16(__int16(MeteoRW.m_DistanceVisBeg));

	// I241/613
	if(ucFSPEC[2] & 0x10)
		WriteInt16(__int16(MeteoRW.m_DistanceVisMdl));

	// I241/614
	if(ucFSPEC[2] & 0x08)
		WriteInt16(__int16(MeteoRW.m_DistanceVisEnd));

	// I241/615
	if(ucFSPEC[2] & 0x04)
		WriteInt16(__int16(MeteoRW.m_HCloud));

	// I241/616
	if(ucFSPEC[2] & 0x02)
		WriteInt16(__int16(MeteoRW.m_QFE));

	// I241/617
	if(ucFSPEC[3] & 0x80)
		WriteInt16(__int16(MeteoRW.m_QFEh));

	// I241/618
	if(ucFSPEC[3] & 0x40)
		WriteInt16(__int16(MeteoRW.m_QNHh));

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

int CCodeAsterix::MakeMeteoRW2(CMeteoRW & MeteoRW)
{
	int i;
	UCHAR ucFSPEC[4];
	for(i=0; i<4; i++) ucFSPEC[i]=0x00;

	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(241);

	//LEN
	WriteInt16(0x00);


	//FSPEC
	ucFSPEC[0]=0xF9;	// Идентификатор источника данных	I241/010
	// Тип сообщения					I241/000
	// Номер сообщения					I241/600
	// Время суток						I241/030
	// Срок действия					I241/601
	// FX
	ucFSPEC[1] = 0xC0;
	if(MeteoRW.m_WindCourse>-1000)	ucFSPEC[1] = ucFSPEC[1] | 0x20;
	if(MeteoRW.m_WindEstm>-1000)	ucFSPEC[1] = ucFSPEC[1] | 0x10;
	if(MeteoRW.m_WindMax>-1000)		ucFSPEC[1] = ucFSPEC[1] | 0x08;
	if(MeteoRW.m_WindSide>-1000)	ucFSPEC[1] = ucFSPEC[1] | 0x04;
	if(MeteoRW.m_Temp>-1000)		ucFSPEC[1] = ucFSPEC[1] | 0x02;

	if(MeteoRW.m_DewPoint>-1000 || MeteoRW.m_Vis>-1000 || MeteoRW.m_DistanceVisBeg>-1000 || MeteoRW.m_DistanceVisMdl>-1000.0 || MeteoRW.m_DistanceVisEnd>-1000.0 || MeteoRW.m_HCloud>-1000 || MeteoRW.m_QFE>-1000 || MeteoRW.m_QFEh>-1000 || MeteoRW.m_QNHh>-1000)
	{
		ucFSPEC[1]=ucFSPEC[1] | 0x01;

		if(MeteoRW.m_DewPoint>-1000)		ucFSPEC[2] = ucFSPEC[2] | 0x80;
		if(MeteoRW.m_Vis>-1000)				ucFSPEC[2] = ucFSPEC[2] | 0x40;
		if(MeteoRW.m_DistanceVisBeg>-1000)	ucFSPEC[2] = ucFSPEC[2] | 0x20;
		if(MeteoRW.m_DistanceVisMdl>-1000)	ucFSPEC[2] = ucFSPEC[2] | 0x10;
		if(MeteoRW.m_DistanceVisEnd>-1000)	ucFSPEC[2] = ucFSPEC[2] | 0x08;
		if(MeteoRW.m_HCloud>-1000)			ucFSPEC[2] = ucFSPEC[2] | 0x04;
		if(MeteoRW.m_QFE>-1000)				ucFSPEC[2] = ucFSPEC[2] | 0x02;

		if(MeteoRW.m_QFEh>-1000 || MeteoRW.m_QNHh>-1000)
		{
			ucFSPEC[2]=ucFSPEC[2] | 0x01;

			if(MeteoRW.m_QFEh>-1000)	ucFSPEC[3] = ucFSPEC[3] | 0x80;
			if(MeteoRW.m_QNHh>-1000)	ucFSPEC[3] = ucFSPEC[3] | 0x40;
		}
	}

	WriteByte(ucFSPEC[0]);
	if(ucFSPEC[0] & 0x01) WriteByte(ucFSPEC[1]);
	if(ucFSPEC[1] & 0x01) WriteByte(ucFSPEC[2]);
	if(ucFSPEC[2] & 0x01) WriteByte(ucFSPEC[3]);


	// I241/010
	if(ucFSPEC[0] & 0x80)
	{
		WriteByte(MeteoRW.SAC);
		WriteByte(MeteoRW.SIC);
	}

	// I241/000
	if(ucFSPEC[0] & 0x40) WriteByte(3);

	// I241/600
	if(ucFSPEC[0] & 0x20)
		WriteInt16(MeteoRW.m_MsgNumber);

	// I241/030
	if(ucFSPEC[0] & 0x10)
	{
		double dt;
		dt=MeteoRW.m_TimeGet.m_time.wHour*3600 + MeteoRW.m_TimeGet.m_time.wMinute*60 + MeteoRW.m_TimeGet.m_time.wSecond + MeteoRW.m_TimeGet.m_time.wMilliseconds*0.001;
		long lt;
		lt=(long)(dt*128);
		WriteInt24(lt);
	}

	// I241/601
	if(ucFSPEC[0] & 0x08)
	{
		int dtGet, dtStop, dt;
		dtGet=int(MeteoRW.m_TimeGet.m_time.wHour);
		dtStop=int(MeteoRW.m_TimeStop.m_time.wHour);
		if(dtGet>dtStop)
			dtStop=dtStop+24;
		dt=dtStop-dtGet;
		WriteByte(dt);
	}

	// I241/603
	if(ucFSPEC[1] & 0x80)
	{
		WriteByte(MeteoRW.m_cICAO_Identifier[0]);
		WriteByte(MeteoRW.m_cICAO_Identifier[1]);
		WriteByte(MeteoRW.m_cICAO_Identifier[2]);
		WriteByte(MeteoRW.m_cICAO_Identifier[3]);
	}

	// I241/604
	if(ucFSPEC[1] & 0x40)
	{
		CString str="00";
		int cours;
		if(MeteoRW.m_strRWName.GetLength()>=4) str = MeteoRW.m_strRWName.Mid(2,3);
		cours = (str[0]-0x30)*10 + (str[1]-0x30);
		UCHAR b;
		b = (UCHAR(cours)<<2) & 0xFC;
		if(str[2]=='L') b = b | 0x01;
		if(str[2]=='R') b = b | 0x02;
		if(str[2]=='M') b = b | 0x03;
		WriteByte(b);
	}

	// I241/605
	if(ucFSPEC[1] & 0x20)
		WriteInt16(__int16(MeteoRW.m_WindCourse));

	// I241/606
	if(ucFSPEC[1] & 0x10)
		WriteByte(UCHAR(int(MeteoRW.m_WindEstm)));

	// I241/607
	if(ucFSPEC[1] & 0x08)
		WriteByte(UCHAR(int(MeteoRW.m_WindMax)));

	// I241/608
	if(ucFSPEC[1] & 0x04)
		WriteInt16(__int16(MeteoRW.m_WindSide*10));

	// I241/609
	if(ucFSPEC[1] & 0x02)
		WriteInt16(__int16(MeteoRW.m_Temp*10));

	// I241/610
	if(ucFSPEC[2] & 0x80)
		WriteInt16(__int16(MeteoRW.m_DewPoint*10));

	// I241/611
	if(ucFSPEC[2] & 0x40)
		WriteInt16(__int16(MeteoRW.m_Vis));

	// I241/612
	if(ucFSPEC[2] & 0x20)
		WriteInt16(__int16(MeteoRW.m_DistanceVisBeg));

	// I241/613
	if(ucFSPEC[2] & 0x10)
		WriteInt16(__int16(MeteoRW.m_DistanceVisMdl));

	// I241/614
	if(ucFSPEC[2] & 0x08)
		WriteInt16(__int16(MeteoRW.m_DistanceVisEnd));

	// I241/615
	if(ucFSPEC[2] & 0x04)
		WriteInt16(__int16(MeteoRW.m_HCloud));

	// I241/616
	if(ucFSPEC[2] & 0x02)
		WriteInt16(__int16(MeteoRW.m_QFE));

	// I241/617
	if(ucFSPEC[3] & 0x80)
		WriteInt16(__int16(MeteoRW.m_QFEh));

	// I241/618
	if(ucFSPEC[3] & 0x40)
		WriteInt16(__int16(MeteoRW.m_QNHh));

	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

int CCodeAsterix::MakeTime(CMRTTime &Time)
{
	CSingleLock Lock(&LockCode, true);

	index=0;
	//Category
	WriteByte(242);

	//LEN
	WriteInt16(0x00);

	//FSPEC
	WriteByte(0xC0);	
	//Data Source    I242/010
	//FileTime	     I242/020
	//				 I002/030
	//				 I002/040

	// I242/010
	WriteByte(0);
	WriteByte(0);

	// I242/020 пишем FileTime
	union unTIME
	{
		FILETIME m_ft;     //  Время в формате FILETIME
		__int64 i64;       //  Оно же в формате __int64
	}unT;
	SystemTimeToFileTime((SYSTEMTIME*)&Time.m_time, &unT.m_ft);
	WriteInt64(unT.i64);
	
	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);

//	AddCRC();
	return 0;
}

void CCodeAsterix::AddCRC(void)
{
	//CRC Control
	CCRC_16_ASTERIX crc_16_a;
	long l_crc;
	
	crc_16_a.Clear();
	for(int i=0; i<index; i++) crc_16_a.AddByte(pbuf_out[i]);
	l_crc=crc_16_a.GetCRC();
	WriteInt16(__int16(l_crc));	

	//WriteByte(0);//Это старый протокол КС-2
}

int CCodeAsterix::MakePlot21(CMRTPlot& Plot)
{
	int i;
	UCHAR ucFSPEC[4];
	for(i=0; i<4; i++) ucFSPEC[i]=0x00;

	CSingleLock Lock(&LockCode, true);
	
	
	double dB, dL, dH;
	CGeoid Gd;
	Plot.GetBLH(dB, dL, dH);
	Gd.SetBasePoint_Geoid(dB, dL, dH, 0.0);

	long lB, lL;
	lB=(long)((dB*(1024*1024*8))/180.0);
	lL=(long)((dL*(1024*1024*8))/180.0);
	
	if(lB>16777215) return -1;
	if(lL>16777215) return -1;
	if(!(Plot.IKI & 0x08)) return -1;

	index=0;
	//Category
	WriteByte(21);
	
	//LEN
	WriteInt16(0x00);

	
	//FSPEC
	ucFSPEC[0]=0xF9;	// +Data Source				I021/010
						// +Target Report			I021/040
						// +Time of day				I021/030
						// +Position in WGS-84 Coord	I021/130		
						// +Traget address			I021/080
						// -Geometric altitude		I021/140
						// -Figure of merit			I021/090
						
	
	ucFSPEC[1]=0x21;	// -Link technology			I021/210
						// +Flight level				I021/145
						

	ucFSPEC[2]=0x11;	// -Geometric verticale rate	I021/157
						// -Ground vector			I021/160
						// -Rate of turn				I021/165
						// +Traget identification	I021/170
						// -Velocity accuracy		I021/095


	ucFSPEC[3]=0x80;	// Emitter category			I021/220
						

	WriteByte(ucFSPEC[0]);
	if(ucFSPEC[0] & 0x01)
	{
		WriteByte(ucFSPEC[1]);
		if(ucFSPEC[1] & 0x01)
		{
			WriteByte(ucFSPEC[2]);
			if(ucFSPEC[2] & 0x01)
			{
				WriteByte(ucFSPEC[3]);
			}
		}
	}

	// I021/010
	if(ucFSPEC[0] & 0x80){
		WriteByte(Plot.SAC);
		WriteByte(Plot.SIC);
	}
	// I021/040
	if(ucFSPEC[0] & 0x40){
		UCHAR b=0;

		//TST
		if(Plot.flag==0x01)
			b=b | 0x10;
		//SIM
		if(Plot.flag==0x02)
			b=b | 0x20;
		// SPI
		if(Plot.SPI)
			b=b | 0x02;
		WriteByte(b);
		WriteByte(0x20);
	}
	// I021/030
	if(ucFSPEC[0] & 0x20){
		double dt;
		long i24t;
		if(Plot.time.GetMS()>0)
		{
			dt=Plot.time.m_time.wHour*3600 + Plot.time.m_time.wMinute*60 + Plot.time.m_time.wSecond + Plot.time.m_time.wMilliseconds*0.001;
			i24t=(long)(dt*128);
		}
		else
			i24t=-1;
		WriteInt24(i24t);
	}
	// I021/130
	if(ucFSPEC[0] & 0x10){
		double dB, dL, dH;
		Plot.GetBLH(dB, dL, dH);
		Gd.SetBasePoint_Geoid(dB, dL, dH, 0.0);

		long lB, lL;
		lB=(long)((dB*(1024*1024*32))/180.0);
		lL=(long)((dL*(1024*1024*32))/180.0);

		//WriteInt24(lB);
		//WriteInt24(lL);
		WriteInt32(lB);
		WriteInt32(lL);
	}
	// I021/080
	if(ucFSPEC[0] & 0x08){
		WriteInt24(Plot.Address_ICAO);
	}
	// -I021/140
	if(ucFSPEC[0] & 0x04){
		__int16 i16;
		
		long h;
		//25футов != 7,62м
		// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
		double dH;
		dH=Plot.Height_3D;
		//dH=dH*6080.0/(1852*6.25);
		dH = dH/(0.3048*6.25);
		if(dH<0)
			h = long(dH - 0.5);
		else
			h = long(dH + 0.5);
		i16=__int16(h);
		WriteInt16(i16);
	}
	// -I021/090;
	if(ucFSPEC[0] & 0x02){
		WriteInt16(0);
	}
	
	// -I021/210;
	if(ucFSPEC[1] & 0x80){
		WriteByte(0x08);
	}
	// I021/145;
	if(ucFSPEC[1] & 0x20){
		__int16 i16=0;
		if(Plot.EXH)
		{
			long h;
			//25футов != 7,62м
			// 1 фут = 1852/6080 - это неверно (см.ICAO) 1 фут = 0.3048 метров
			double dH;
			dH=Plot.heigh;
			//dH=dH*6080.0/(1852*25);
			dH = dH/(0.3048*25.0);
			if(dH<0)
				h = long(dH - 0.5);
			else
				h = long(dH + 0.5);
			i16=__int16(h);
		}
		WriteInt16(i16);
	}
	
	// -I021/157;
	if(ucFSPEC[2] & 0x80){
		WriteInt16(0);
	}
	// -I021/160;
	if(ucFSPEC[2] & 0x40){
		WriteInt32(0);
	}
	// -I021/165;
	if(ucFSPEC[2] & 0x20){
		WriteByte(0);
	}
	// I021/170;
	if(ucFSPEC[2] & 0x10){
		CString ident;
		char ch[8];
		int lenght, i;
		ident=Plot.Ident;
		
		ident.MakeUpper();

		lenght=ident.GetLength();
		for(i=0; i<lenght; i++)
		{
			CMyMath MM;
			ch[i]=MM.Char_to_ICAOChar(ident[i]);
		}
		if(lenght<8)
		{
			for(i=lenght; i<8; i++)
			{
				ch[i]=' ';
			}
		}
		UCHAR b1=0, b2=0, b3=0, b4=0, b5=0, b6=0;
		
		b1=(((ch[0]&0x3F)<<2)&0xFC);
		b2=(((ch[1]&0x30)>>4)&0x03);
		b1 = b1 | b2;
		WriteByte(b1);

		b1=0;
		b2=0;
		b1=(((ch[1]&0x0F)<<4)&0xF0);
		b2=(((ch[2]&0x3C)>>2)&0x0F);
		b1 = b1 | b2;
		WriteByte(b1);

		b1=0;
		b2=0;
		b1=(((ch[2]&0x03)<<6)&0xC0);
		b2=(ch[3]&0x3F);
		b1 = b1 | b2;
		WriteByte(b1);

		b1=0;
		b2=0;
		b1=(((ch[4]&0x3F)<<2)&0xFC);
		b2=(((ch[5]&0x30)>>4)&0x03);
		b1 = b1 | b2;
		WriteByte(b1);

		b1=0;
		b2=0;
		b1=(((ch[5]&0x0F)<<4)&0xF0);
		b2=(((ch[6]&0x3C)>>2)&0x0F);
		b1 = b1 | b2;
		WriteByte(b1);

		b1=0;
		b2=0;
		b1=(((ch[6]&0x03)<<6)&0xC0);
		b2=(ch[7]&0x3F);
		b1 = b1 | b2;
		WriteByte(b1);
	}
	// -I021/095;
	if(ucFSPEC[2] & 0x08){ 
		WriteByte(0);
	}
	// I021/020;
	if(ucFSPEC[3] & 0x80){
		WriteByte(Plot.ECAT);
	}
			
	//LEN
	pbuf_out[1]=(UCHAR)(index>>8);
	pbuf_out[2]=(UCHAR)(index&0x00FF);
	return 0;
}
