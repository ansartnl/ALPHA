#include "stdafx.h"
#include "math.h"
#include "DecodeAsterix.h"
#include "Communication_MOXA502.h"
#include "Communication_UKIS.h"
#include "NetDGSocket.h"
#include "CRC_16_ASTERIX.h"

#include "ComArray.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CDecodeAsterix::CDecodeAsterix()
{
	pMRT = NULL;
	pUTS = NULL;
	f = 0;
	RlkSt.Clear();
}

CDecodeAsterix::~CDecodeAsterix()
{
}

void CDecodeAsterix::get_packet(unsigned char N, int L, void* pVComSource, void* pVM_502, void *pVSocket, CMRTTrack *pTrackMRT, void* pvUKIS)
{
	CCommunicationSource* pComSource;
	pComSource = (CCommunicationSource*)pVComSource;

	CCommunication_MOXA502* pM_502;
	pM_502 = (CCommunication_MOXA502*)pVM_502;

	CNetDGSocket *pSocket;
	pSocket = (CNetDGSocket*)pVSocket;
	PlotMRT.Clear();
	TrackMRT.Zero();

	CComArray *pCA = NULL;
	if(pComSource != NULL) pCA = (CComArray*)(pComSource->pCA);	

	i_comport = 0;
	uc_LpdType = 0;

	unsigned char def_SAC = 0, def_SIC = 0;
	BOOL b_Add_SacSic = FALSE;

	if(pComSource != NULL)
	{
		i_comport = pComSource->SourcePort.CC.i_N_CP;
		uc_LpdType = 0;
		pComSource->N_Packets++;
		if(pComSource->SourcePort.CC.b_AddSacSic)
		{
			b_Add_SacSic = TRUE;
			def_SAC = pComSource->SourcePort.CC.add_SAC;
			def_SIC = pComSource->SourcePort.CC.add_SIC;
		}
	}
	CCommunication_UKIS* pUKIS = (CCommunication_UKIS*)pvUKIS;

	if (pUKIS != NULL)
	{
		if(pUKIS->UC.b_AddSacSic)
		{
			b_Add_SacSic = true;
			def_SAC = pUKIS->UC.add_SAC;
			def_SIC = pUKIS->UC.add_SIC;
		}

		i_comport = pUKIS->UC.i_N_Number;
		uc_LpdType = 1;

		pUKIS->N_Packets++;
		pUKIS->l_byte+=L;
	}
	
	if(pM_502 != NULL)
	{
		i_comport = pM_502->CC502.i_N_CP;
		uc_LpdType = 2;
		pM_502->N_Packets++;

		if(pM_502->CC502.b_AddSacSic)
		{
			b_Add_SacSic = TRUE;
			def_SAC = pM_502->CC502.add_SAC;
			def_SIC = pM_502->CC502.add_SIC;
		}
	}

	if(pSocket != NULL)
	{
		i_comport = pSocket->iSocketNumber;
		uc_LpdType = 3;
		pSocket->N_Packets++;

		if (pSocket->b_Add_SacSic)
		{
			b_Add_SacSic = TRUE;
			def_SAC = pSocket->add_SAC;
			def_SIC = pSocket->add_SIC;
		}
	}

	if(N == 0xFA)
	{	//Контрольный пакет
		int i_err=0;
		if(pComSource != 0)
		{
			pComSource->N_Packets_control++;
			if(L != 4) i_err = 1;	//Bad Packet Length
			else
			{					//Good Packet Length
				if(pbuf[1] != 0x55) i_err++;
				if(pbuf[2] != 0xAA) i_err++;
			}
			if(i_err == 0)
			{
				UCHAR uc_N;

				uc_N = pbuf[0];
				if(pComSource->SourcePort.i_LastNControlPacket < 0 || uc_N == 0xFF)
					pComSource->SourcePort.i_LastNControlPacket = uc_N;//Первый раз
				else
				{
					int iN, delta;
					iN = uc_N;
					if(iN > pComSource->SourcePort.i_LastNControlPacket)
						delta = iN-pComSource->SourcePort.i_LastNControlPacket;
					else
					{
						//Номера пакетов меняются от 0 до 254.
						//Значение 255 присваевается в момент открытия порта.
						delta = (254+1) - (pComSource->SourcePort.i_LastNControlPacket - iN);
					}
					i_err += UCHAR(delta - 1);
				}

				if(uc_N == 0xFF)
					pComSource->SourcePort.i_LastNControlPacket = -1;//Перезапуск порта Абонента
				else
					pComSource->SourcePort.i_LastNControlPacket = uc_N;

				pComSource->SourcePort.uc_ComStateOfAbonent = pbuf[3];
				pComSource->SourcePort.uc_TimeOutControlPacketOfAbonent = 0;
			}
			if(i_err != 0)
			{	
				pComSource->N_BadPackets_control++;
				pComSource->SourcePort.l_bad_byte += i_err;
			}
		}

		if(pSocket != 0)
		{
			i_err = 0;
			pSocket->N_Packets_control++;

			if(L != 4) i_err = 1;//Bad Packet Length
			else				//Good Packet Length
			{
				if(pbuf[1] != 0x55) i_err++;
				if(pbuf[2] != 0xAA) i_err++;
			}

			if (i_err != 0)
			{
				pSocket->N_BadPackets_control++;
				pSocket->l_bad_byte += i_err;
			}
		}
		return;//Выход, т.к. это контролльный пакет
	}

	//CRC Control
	CCRC_16_ASTERIX crc_16_a;

	if(L > 3)
	{
		crc_16_a.Clear();
		for(int i=0; i<L-2; i++) crc_16_a.AddByte(pbuf[i]);
		long l_crc, l_crc_get;
		l_crc_get = pbuf[L-2];
		l_crc_get = (l_crc_get<<8) + pbuf[L-1];
		l_crc = crc_16_a.GetCRC();
		//Циклический сдвиг
		/*BYTE b, b8;
		b=l_crc & 0xFF;
		b8=b & 0x80;
		b=b<<1;
		if(b8) b=b | 0x01;
		l_crc=(l_crc & 0xFF00) | b;*/

		if(l_crc != l_crc_get)
		{
			if(pComSource != NULL)
			{
				if (pComSource->SourcePort.CC.i_CRC != 0)//надо ли считать CRC в режиме CCITT_1(0x8408)
					pComSource->N_BadPackets++;
			}
			if(pM_502 != NULL)
				pM_502->N_BadPackets++;
			/* По сети CRC не считаем
			if(pSocket != NULL)
				pSocket->N_BadPackets++;*/
			
		}
	}
	else
	{
//		if(L==3 && pbuf[0]==0 && pbuf[1]==0) 
//			return;
		if(pComSource != NULL) pComSource->N_BadPackets++;
		if(pM_502 != NULL) pM_502->N_BadPackets++;
		if(pSocket != NULL) pSocket->N_BadPackets++;
		return;
	}

	CString str;
	int i;
	long len;
	bool fspec[36];
	
	for(i = 0; i < 25; i++) fspec[i] = false;

//	return;
	//Хутор-Гора
	if (/*N==0xC1 &&*/ pbuf[0]==0x01 && pbuf[1]==0x02)
	{
		if(L==8)
		{
			if(pbuf[4]==0x01){	//Азимутальные меики
				float f_as_met;
				int sector;
				sector=pbuf[3];
				f_as_met=float( 360.0*sector/32.0 );

				SAC=0x11;
				SIC=0x23;
				if(sector==0){
					if(pMRT!=NULL){			
						CMRTNorth N(SAC, SIC, 0, i_comport, uc_LpdType);
						pMRT->AddNorthMRT(N);
						N.track=1;
						pMRT->AddNorthMRT(N);
					}
				}

				if(pMRT!=NULL){
					CMRTAm Am(SAC, SIC, 0, i_comport, uc_LpdType, f_as_met);
					pMRT->AddAmMRT(Am);
					Am.track=1;
					pMRT->AddAmMRT(Am);
				}
				
				return;
			}
		}
		else
		{
			if(pComSource!=NULL) pComSource->N_BadPackets++;
			if(pM_502!=NULL) pM_502->N_BadPackets++;
		}
	}

	if (/*N==0xC1 &&*/ pbuf[0]==0x01 && pbuf[1]==0x0C)
	{
		//if(L==17){
			if(pbuf[4]==0x03){	//ВС УВД
				IKI=B=MK=0;
				X=Y=0;
				Az=D=0;
				EXNB=0;
				NB=0;
				EXH=ZH=0;
				H=0;
				OT=BRF=0;
				OriginCoord=0;

				SAC=0x11;
				SIC=0x23;
				TYP=0;//плоты
				IKI=0x02;//ВРЛ
				MK=0;//УВД

				long n1, n2, n3, n4, n5;
				n1=pbuf[11] & 0x0F;
				n2=pbuf[11]>>4;
				n3=pbuf[12] & 0x0F;
				n4=pbuf[12]>>4;
				n5=pbuf[ 9] & 0x0F;
				NB=n1 + n2*10 + n3*100 + n4*1000 + n5*10000;
				if(NB!=0) EXNB=1;

				n1=pbuf[13] & 0x0F;
				n2=pbuf[13]>>4;
				n3=pbuf[14] & 0x0F;
				n4=(pbuf[14] & 0x30) >> 4;
				H=n1*10 + n2*100 + n3*1000 + n4*10000;
				if(H!=0) EXH=1;
				if(pbuf[14] & 0x80) ZH=1;

				unsigned char sx, sy;
				long l_x, l_y;
				sx=sy=0;

				n1=pbuf[5];
				n2=pbuf[6];
				l_x= ((n2&0x7F)<<4) + (n1>>4);
				l_x=(l_x*200000*2)/2047;
				if(pbuf[6] & 0x80) X=-l_x;
				else X=l_x;

				n1=pbuf[7];
				n2=pbuf[8];
				l_y= ((n2&0x7F)<<4) + (n1>>4);
				l_y=(l_y*200000*2)/2047;
				if(pbuf[8] & 0x80) Y=-l_y;
				else Y=l_y;

				OT=pbuf[10] & 0x0F;

				PlotMRT.IKI = IKI;
				PlotMRT.MK  = MK;
				PlotMRT.x=X;
				PlotMRT.y=Y;
				PlotMRT.d_D=D;
				PlotMRT.d_Az=Az;
				PlotMRT.EXNB=EXNB;
				PlotMRT.number=NB;
				PlotMRT.EXH=EXH;
				PlotMRT.ZH=ZH;
				PlotMRT.heigh=H;
				PlotMRT.b=B;
				PlotMRT.ot=OT;
				PlotMRT.BRF=BRF;
				PlotMRT.SPI=SPI;
				PlotMRT.flag=FLAG;
				PlotMRT.SAC=SAC;
				PlotMRT.SIC=SIC;

				PlotMRT.track=TYP;
				PlotMRT.m_nLpd=i_comport;
				PlotMRT.m_ucLpdType=uc_LpdType;
				PlotMRT.OriginCoord=OriginCoord;

				if(pMRT!=NULL) pMRT->AddPlotMRT(PlotMRT);
				return;
			}
		//}
		//else ComArray[i_comport-1].N_BadPackets++;
	}
	if (/*N==0xC1 &&*/ pbuf[0]==0x01 && pbuf[1]==0x0A)
	{
		if(L == 16)
		{
			if(pbuf[4]==0x05) //ВС RBS
			{
				IKI=B=MK=0;
				X=Y=0;
				Az=D=0;
				EXNB=0;
				NB=0;
				EXH=ZH=0;
				H=0;
				OT=BRF=0;

				SAC=0x11;
				SIC=0x23;
				TYP=0;//плоты
				IKI=0x02;//ВРЛ
				MK=1;//RBS

				OriginCoord=0;

				long n1, n2, n3, n4;//, n5;
				n1=pbuf[ 9] & 0x0F;
				n2=pbuf[ 9]>>4;
				n3=pbuf[10] & 0x0F;
				n4=pbuf[10]>>4;
				NB=n1 + n2*10 + n3*100 + n4*1000;
				if(NB!=0) EXNB=1;

				n1=pbuf[11] & 0x0F;
				n2=pbuf[11]>>4;
				n3=pbuf[12] & 0x0F;
				n4=(pbuf[12] & 0x30) >> 4;
				H=n1*10 + n2*100 + n3*1000 + n4*10000;
				if(H!=0) EXH = 1;
				if(pbuf[12] & 0x80)
				{
					ZH = 0;
					H = -H;
				}
				else 
					ZH = 1;

				unsigned char sx, sy;
				long l_x, l_y;
				sx=sy=0;

				n1=pbuf[5];
				n2=pbuf[6];
				l_x= ((n2&0x7F)<<4) + (n1>>4);
				l_x=(l_x*200000*2)/2047;
				if(pbuf[6] & 0x80) X=-l_x;
				else X=l_x;

				n1=pbuf[7];
				n2=pbuf[8];
				l_y= ((n2&0x7F)<<4) + (n1>>4);
				l_y=(l_y*200000*2)/2047;
				if(pbuf[8] & 0x80) Y=-l_y;
				else Y=l_y;

				OT=0;

				PlotMRT.IKI = IKI;
				PlotMRT.MK  = MK;
				PlotMRT.x=X;
				PlotMRT.y=Y;
				PlotMRT.d_D=D;
				PlotMRT.d_Az=Az;
				PlotMRT.EXNB=EXNB;
				PlotMRT.number=NB;
				PlotMRT.EXH=EXH;
				PlotMRT.ZH=ZH;
				PlotMRT.heigh=H;
				PlotMRT.b=B;
				PlotMRT.ot=OT;
				PlotMRT.BRF=BRF;
				PlotMRT.SPI=SPI;
				PlotMRT.flag=FLAG;
				PlotMRT.SAC=SAC;
				PlotMRT.SIC=SIC;

				PlotMRT.track=TYP;
				PlotMRT.m_nLpd=i_comport;
				PlotMRT.m_ucLpdType=uc_LpdType;
				PlotMRT.OriginCoord=OriginCoord;

				if(pMRT!=NULL) pMRT->AddPlotMRT(PlotMRT);				
				return;
			}
		}
		else{
			if(pComSource!=NULL) pComSource->N_BadPackets++;
			if(pM_502!=NULL) pM_502->N_BadPackets++;
		}
	}

	if(N != 2)
	{
		if ((pbuf[0]!=0x01 || pbuf[1]!=0x03) && pbuf[0]!=0xFF)
		{
			if (pbuf[0]!=0x00 || pbuf[1]!=0x03) return;	
		}
	}
	
/* Rem for Alenia SIR-M
	Begin

	
	if (N==0xC1 || N==0xC3) {		//HDLC Asterix


		if (pbuf[L-1]!=0) {
			if(pbuf[2]==2){
				int r=0;
				r=r;
			}
			//pComSource->N_BadPackets++;
			ComArray[i_comport-1].N_BadPackets++;
		}

		if (pbuf[L-1]==0) {
			if(pbuf[2]==2 && pbuf[10]==224){
				int r=0;
				r=r;
			}
		}


		//if (pbuf[L-1]==0) {	//Good CRC
		if (1) {	//Good CRC

			
			category=pbuf[2];
			len=(((long)(pbuf[3]))<<8)+(long)(pbuf[4]);
			//if(len>=256) return;
			if((len+5)!=L) {
				//pComSource->N_BadPackets++;
				ComArray[i_comport-1].N_BadPackets++;
				if(pbuf[2]!=1){
					int y;
					y=0;
				}
				return;
			}
			index=5;//2
			
			if(category==0x01) {
			  do{
				for(i=0; i<25; i++) fspec[i]=false;
				
				

				IKI=B=MK=0;
				X=Y=0;
				EXNB=0;
				NB=0;
				EXH=ZH=0;
				H=0;
				OT=BRF=0;
				SPI=FLAG=0;

				//FSPEC
				byte=read_buffer();
				for(i=0; i<7; i++) {
					if(byte & (0x80>>i)) fspec[i+1]=true;
				}
				if(byte & 0x01) {	//FX
					byte=read_buffer();
					for(i=0; i<7; i++) {
						if(byte & (0x80>>i)) fspec[i+8]=true;
					}
				}
				if(byte & 0x01) {	//FX
					byte=read_buffer();
					for(i=0; i<7; i++) {
						if(byte & (0x80>>i)) fspec[i+15]=true;
					}
				}
				//End FSPEC

				if(fspec[1]){	//Data Source Identifier I001/010
					decode_I001_010();					
				}
				if(fspec[2]){	//Target Report Description I001/020
					decode_I001_020();
				}
				if(fspec[3]){	//Measured position (polar coordinates) I001/040
					decode_I001_040();
				}
				if(fspec[4]){	//Mode 3/A Octal I001_070
					decode_I001_070();
				}
				if(fspec[5]){	//Mode C code Binary I001/090
					decode_I001_090();					
				}
				if(fspec[14]){	//Warning/Error Condition I001/030
					decode_I001_030();					
				}
				if(fspec[20]){	//Reserved for SP Indicator
					decode_I001_SP();


				}

				

				pGDoc->add_plot_track(SAC, SIC, TYP, i_comport, IKI, MK, X, Y, EXNB, NB, EXH, ZH, H, B, OT, BRF);
				
			  //}while(index<(L-3));
			  }while(index<(len-2));
			}
			if(category==0x02) {
			  do{
				for(i=0; i<25; i++) fspec[i]=false;
				//MessageBeep((WORD)-1);
				//FSPEC
				byte=read_buffer();
				for(i=0; i<7; i++) {
					if(byte & (0x80>>i)) fspec[i+1]=true;
				}
				if(byte & 0x01) {	//FX
					byte=read_buffer();
					for(i=0; i<7; i++) {
						if(byte & (0x80>>i)) fspec[i+8]=true;
					}
				}
				//End FSPEC

				if(fspec[1]){	//Data Source Identifier I002/010
					decode_I002_010();					
				}
				if(fspec[2]){	//Message Type I002/000
					decode_I002_000();
				}
				if(fspec[3]){	//Sector Number I002/020
					decode_I002_020();
				}
				if(fspec[4]){	//Time of day I002/030
					//decode_I002_030();
				}
				if(fspec[5]){	//Antenna Rotation Period I002/041
					decode_I002_041();
				}
				if(fspec[6]){	//Station Configuration Status I002/050
					decode_I002_050();
				}
				if(fspec[7]){	//Station Processing Mode I002/060
					decode_I002_060();
				}

				if(fspec[13]){	//Reserved for SP Indicator I002/020
					decode_I002_SP();
				}

			  //}while(index<(L-3));	
			  }while(index<(len+2));
			}
		}
		return;
	}
 Rem for Alenia SIR-M
	End
*/
//	if (N==0x02 || N==0xC1) {		//HDLC Концентратор

	if (N==0x02 || N==0x04 || N==0xC1 || N==0xC3) //HDLC Концентратор + UKIS for Tercas
	{
		if(N == 0x04)
		{
			UCHAR uc_UKIS_Port;
			uc_UKIS_Port = pbuf[L-1]-1;

			if(uc_UKIS_Port < N_UKIS_Port)
			{
				if(pCA != NULL)
					pCA->UKISArray[uc_UKIS_Port].l_byte+=12;
			}			
		}
			
		index = 0;

		if((pbuf[0]==0x01  && pbuf[1]==0x03) ||
			(pbuf[0]==0x00 && pbuf[1]==0x03) ||
			 pbuf[0]==0xFF)
		{
			index = 2;
		}
								

		category = read_buffer();
		len = (((long)(read_buffer()))<<8)+(long)(read_buffer());

		if (len > sizeof(pbuf)-2) return; //error packet
		
		if(pComSource != NULL) 
		{
			if (pComSource->SourcePort.CC.i_CRC == 0)
			{
				if (len != L) 
				{
					pComSource->N_BadPackets++;
				}
			}
		}
		if(pM_502 != NULL) 
		{
			if (len != L-3) 
			{
				pM_502->N_BadPackets++;
				pM_502->l_all_bad_byte += L-3;
			}
		}
		if(pSocket != NULL)
		{
			if (len != L) 
			{
				pSocket->N_BadPackets++;
			}
		}

		if(category == 0x01)
		{
			do
			{
				for(i=0; i<29; i++) fspec[i]=false;
				
				IKI = B = MK = 0;
				X = Y = 0;
				D = Az = 0;
				EXNB = 0;
				NB = 0;
				EXH = ZH = 0;
				H = 0;
				OT = BRF = 0;
				SPI = FLAG = 0;
				time_stamp.ZeroTime();
				OriginCoord = 0;

				//FSPEC
/*				byte=read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1]=true;}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8]=true;}
				}
				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+15]=true;}
				}*/
				//FSPEC
				byte = read_buffer();
				for(i=0; i<7; i++) 
				{
					if(byte & (0x80>>i)) fspec[i+1]=true;
				}
				if(byte & 0x01)
				{
					byte = read_buffer();
					for(i=0; i<7; i++)
					{
						if(byte & (0x80>>i)) fspec[i+8]=true;
					}
					if(byte & 0x01)
					{
						byte = read_buffer();
						for(i=0; i<7; i++)
						{
							if(byte & (0x80>>i)) fspec[i+15]=true;
						}
						if(byte & 0x01)
						{
							byte = read_buffer();
							for(i=0; i<7; i++)
							{
								if(byte & (0x80>>i)) fspec[i+22]=true;
							}
						}
					}
				}

				//End FSPEC

				//Михаил Зубков
				if(fspec[20]){	//Reserved for SP Indicator
					int r;
					r=3;
				}

				if(fspec[1])	//Data Source Identifier I001/010
				{	
					decode_I001_010();
				}
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}

				if(fspec[2]){	//Target Report Description I001/020
					decode_I001_020();
				}
				if(TYP==0){	//PLOT
					if(fspec[3]){	//Measured position (polar coordinates) I001/040
						decode_I001_040();
						OriginCoord=1;
					}
					if(fspec[4]){	//Mode 3/A Octal
						decode_I001_070();
					}
					if(fspec[5]){	//Mode C code Binary
						decode_I001_090();						
					}
					if(fspec[6])
					{
						decode_I001_130();
					}
					if(fspec[7]){	//Truncated Time of Day
						decode_I001_141();						
					}
					if(fspec[8])
					{
						decode_I001_050();
					}
					if(fspec[9])
					{
						decode_I001_120();
					}
					if(fspec[10])
					{
						decode_I001_131();
					}
					if(fspec[11])
					{
						decode_I001_080();
					}
					if(fspec[12])
					{
						decode_I001_100();
					}
					if(fspec[13])
					{
						decode_I001_060();
					}
					if(fspec[14]){	//Warning/Error Condition I001/030
						decode_I001_030();
					}
					if(fspec[15])
					{
						decode_I001_150();
					}
					if(fspec[20]){	//Reserved for SP Indicator
						decode_I001_SP();
					}
					if(fspec[21])
					{
						byte = read_buffer();
						int len_re = byte;
						for (int k = 1; k < len_re; k++)
						{
							byte = read_buffer();
						}					
					}
				}// END PLOT

				if (TYP == 1) // TRACK
				{	
					if(fspec[3]){
						decode_I001_161();//Track Number;
					}
					if(fspec[4]){
						decode_I001_040();//Measured position (polar coordinates) I001/040
						OriginCoord=1; // add 2008 04 21
					}
					if(fspec[5]){
						decode_I001_042();//Calculated position (cartesian coordinates)
						OriginCoord=0; // add 2008 04 21
					}
					if(fspec[6]){
						decode_I001_200();//Track speed
					}
					if(fspec[7]){
						decode_I001_070();//Board number 3/A
					}
					if(fspec[8]){
						decode_I001_090();//Heigh C
					}
					if(fspec[9]){
						decode_I001_141();//Truncated time of day
					}
					if(fspec[10]){
						decode_I001_130();//Parameters of plots
					}
					if(fspec[11]){
						decode_I001_131();//Reciving power
					}
					if(fspec[12]){
						decode_I001_120();//Messured Radial speed
					}
					if(fspec[13]){
						decode_I001_170();//Track status
					}
					if(fspec[14]){
						decode_I001_210();//Track quality
					}
					if(fspec[15]){
						decode_I001_050();//Board number 2
					}
					if(fspec[16]){
						decode_I001_080();
					}
					if(fspec[17]){
						decode_I001_100();
					}
					if(fspec[18]){
						decode_I001_060();
					}
					if(fspec[19]){
						decode_I001_030();
					}
					if(fspec[20]){
						decode_I001_SP();//UVD
					}
					if(fspec[21]){
						byte = read_buffer();
						int len_re = byte;
						for (int k = 1; k < len_re; k++)
						{
							byte = read_buffer();
						}
					}
					if(fspec[22]){
						decode_I001_150();
					}
				}
				PlotMRT.IKI = IKI;
				PlotMRT.MK  = MK;
				PlotMRT.x = X;
				PlotMRT.y = Y;
				PlotMRT.d_Az = Az;
				PlotMRT.d_D = D;
				PlotMRT.EXNB = EXNB;
				PlotMRT.number = NB;
				PlotMRT.EXH = EXH;
				PlotMRT.ZH = ZH;
				PlotMRT.heigh = H;
				PlotMRT.b = B;
				PlotMRT.ot = OT;
				PlotMRT.BRF = BRF;
				PlotMRT.SPI = SPI;
				PlotMRT.flag = FLAG;
				PlotMRT.SAC = SAC;
				PlotMRT.SIC = SIC;
				
				PlotMRT.track = TYP;
				PlotMRT.m_nLpd = i_comport;
				PlotMRT.m_ucLpdType = uc_LpdType;

				PlotMRT.time_stamp = time_stamp;
				PlotMRT.OriginCoord = OriginCoord;

				//PlotMRT.ANT = ANT;
				PlotMRT.ANT = 0;//Глиссадная антенна только на РП-5Г				

				if(pMRT != NULL) pMRT->AddPlotMRT(PlotMRT);
		
			}while(index < (L - 3));
		}

		if(category == 0x02) 
		{
			do{
				UCHAR MessageType = 0;
				time_stamp.ZeroTime();
				for(i = 0; i < 25; i++) fspec[i] = false;
				
				//FSPEC
				byte=read_buffer();
				for(i = 0; i < 7; i++) {if(byte & (0x80 >> i)) fspec[i + 1]=true;}

				if(byte & 0x01) //FX
				{	
					byte=read_buffer();
					for(i = 0; i < 7; i++) {if(byte & (0x80 >> i)) fspec[i + 8] = true;}
				}
				//End FSPEC

				if(fspec[1]){	//Data Source Identifier I002/010
					decode_I002_010();
				}
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}

				if(fspec[2]){	//Message Type I002/000
					MessageType = decode_I002_000();
				}
				if(fspec[3]){	//Sector Number I002/020
					decode_I002_020();
				}
				if(fspec[4]){	//Time of day I002/030
					decode_I002_030();
				}
				if(fspec[5]){	//Antenna Rotation Period I002/041
					decode_I002_041();
				}
				if(fspec[6]){	//Station Configuration Status I002/050
					decode_I002_050();
				}
				if(fspec[7])
				{
					decode_I002_060();
				}
				if(fspec[8])
				{
					decode_I002_070();
				}
				if(fspec[9])
				{
					decode_I002_100();
				}
				if(fspec[10])
				{
					decode_I002_090();
				}
				if(fspec[11])
				{
					decode_I002_080();
				}
				if(fspec[12])
				{
				}
				if(fspec[13])
				{
					decode_I002_SP();
				}
				if(fspec[14])
				{
					byte = read_buffer();
					int num_oct = byte;
					for (int k = 1; k < num_oct; k++)
					{
						byte = read_buffer();
					}
				}

				if(MessageType == 0x01)//North
				{
					if(pMRT != NULL)
					{
						NorthMRT.time_stamp = time_stamp;

						NorthMRT.RLKState = RlkSt;
						pMRT->AddNorthMRT(NorthMRT);
						NorthMRT.track = 1;
						pMRT->AddNorthMRT(NorthMRT);
					}
				}
					
				if(MessageType == 0x02)//Am
				{
					if(pMRT != NULL)
					{
						AmMRT.time_stamp = time_stamp;

						pMRT->AddAmMRT(AmMRT);
						AmMRT.track = 1;
						pMRT->AddAmMRT(AmMRT);
					}
				}
			}while(index < (L - 3));	
		}

		if(category == 0x08)
		{
			do{
				//FSPEC
				for(i=0; i<16; i++) fspec[i] = false;
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
				}
				//End FSPEC

				UCHAR FST=0;
				if(fspec[1]) // Идентификатор источника данных
				{
					decode_I008_010();
				}
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}

				UCHAR type;
				if(fspec[2]) // Тип источника сообщений
					type=decode_I008_000();

				if(fspec[3]){  // Идентификатор вектора
					decode_I008_020();}

				int num = 0;
				if(fspec[4]){	// Последовательность векторов
					num=decode_I008_036();
				}


				if(fspec[6]) // Идентификатор контура
				{	
					int a,b;
					FST = decode_I008_040(a,b);

					if(FST == 3 || FST == 2) MeteoPolygon.Clear();

					MeteoPolygon.m_codeEvent=a;
					MeteoPolygon.m_ContNumber=b;
				}

				if(fspec[7]){	// Последовательность точек контура в SPF-формате
					decode_I008_050();
				}

				if(fspec[8]){	// Время
					decode_I008_090();
				}

				if(fspec[9]){	// Статус обработки
					f = decode_I008_100();
				}

				int num_all = 0;
				if(fspec[11]){	// Общее количество элементов
					num_all = decode_I008_120();
				}

				if(fspec[12]){	// Последовательность векторов
					num=decode_I008_038();
				}

				if(FST == 3 || FST == 1)
				{
					MeteoPolygon.m_MsgNumber = 0;
					MeteoPolygon.SAC = SAC;
					MeteoPolygon.SIC = SIC;
					MeteoPolygon.m_TimeCurr = TimeGet;
					MeteoPolygon.m_TimeGet = TimeGet;
					MeteoPolygon.m_TimeStop = MeteoPolygon.m_TimeGet;
					MeteoPolygon.m_TimeStop.AddMS(3600000);

					MeteoPolygon.m_nLpd=i_comport;
					MeteoPolygon.m_ucLpdType=uc_LpdType;

					//MeteoPolygon.MakeOrthodrom();

					if(pMRT!=NULL) pMRT->AddMeteo_PolygonLocal(MeteoPolygon, FALSE);
				}
				if(type==0x04)
				{
					CMRTTime time;
					time.GetSysTime();
 		
					MeteoVector.f			= f;
					MeteoVector.m_nLpd		= i_comport;
					MeteoVector.SAC			= SAC;
					MeteoVector.SIC			= SIC;
					MeteoVector.m_ucLpdType	= uc_LpdType;
					MeteoVector.time		= time;
					MeteoVector.time_stamp	= time;
			
					MeteoVector.m_cIntensity=Intensity;
					MeteoVector.North		= North;
					MeteoVector.NumVect		= num;

					if(pMRT!=NULL) 
					{
						pMRT->AddMeteoVectorLocal(MeteoVector);
					}
					
				}
				if(type == 0xFF){
					MeteoVector.NumVectAll = num_all;
				}
			}while(index < L-3);
		}
		if(category == 21) 
		{
			do
			{
				PlotMRT.Clear();
				PlotMRT.track = 0;
				PlotMRT.m_ucLpdType = uc_LpdType;
				PlotMRT.m_nLpd = i_comport;
				PlotMRT.MK = 1;

				int i;
				bool fspec[55];
				for(i=0; i<40; i++) fspec[i]=false;
					
				m_byte=get_byte();//FSPEC #1
				for(i=0; i<7; i++) {if(m_byte & (0x80>>i)) fspec[i+1]=true;}

				if(m_byte & 0x01)
				{
					m_byte=get_byte();//FSPEC #2
					for(i=0; i<7; i++) {if(m_byte & (0x80>>i)) fspec[i+8]=true;}

					if(m_byte & 0x01)
					{
						m_byte=get_byte();//FSPEC #3
						for(i=0; i<7; i++) {if(m_byte & (0x80>>i)) fspec[i+15]=true;}

						if(m_byte & 0x01)
						{
							m_byte=get_byte();//FSPEC #4
							for(i=0; i<7; i++) {if(m_byte & (0x80>>i)) fspec[i+22]=true;}

							if(m_byte & 0x01)
							{
								m_byte=get_byte();//FSPEC #5
								for(i=0; i<7; i++) {if(m_byte & (0x80>>i)) fspec[i+29]=true;}
							}
						}
					}
				}					
				//End FSPEC

				if(fspec[1]){	//Data Source Identifier I001/010
					decode_I021_010();					
				}
				if(b_Add_SacSic)
				{
					PlotMRT.SAC = def_SAC;
					PlotMRT.SIC = def_SIC;
				}
				if(fspec[2]){	//Target report descriptor
					decode_I021_040();
				}
				if(fspec[3]){	//Time of Day
					decode_I021_030();
				}
				if(fspec[4]){	//Position in WGS-84 coordinates
					decode_I021_130();
				}
				if(fspec[5]){	
					decode_I021_080();// Target Address
				}
				if(fspec[6]){	
					decode_I021_140();// Geometric altitude
				}
				if(fspec[7]){	
					decode_I021_090();// Figure of merit
				}
				if(fspec[8]){	
					decode_I021_210();// Link technology
				}
				if(fspec[9]){	
					decode_I021_230();// Link technology
				}
				if(fspec[10]){	
					decode_I021_145();// Flight level
				}
				if(fspec[11]){	
					decode_I021_150();// Link technology
				}
				if(fspec[12]){	
					decode_I021_151();// Link technology
				}
				if(fspec[13]){	
					decode_I021_152();// Link technology
				}
				if(fspec[14]){	
					decode_I021_155();// Link technology
				}
				if(fspec[15]){	
					decode_I021_157();// Geometric vertical rate
				}
				if(fspec[16]){	
					decode_I021_160();// Ground vector
				}
				if(fspec[17]){	
					decode_I021_165();// Rate of turn
				}
				if(fspec[18]){	
					decode_I021_170();// Target Identification
				}
				if(fspec[19]){	
					decode_I021_095();// Velocity accuracy
				}
				if(fspec[20]){	
					decode_I021_032();// Time of Day Accuracy
				}
				if(fspec[21]){	
					decode_I021_200();// Target Status
				}
				if(fspec[22]){	
					decode_I021_020();// Emitter category
				}
				if(fspec[23]){	
					decode_I021_220();// Met Information
				}
				if(fspec[24]){	
					decode_I021_146();// Intermediate State Selected Altitude
				}
				if(fspec[25]){	
					decode_I021_148();// Final State Selected Altitude
				}
				if(fspec[26]){	
					decode_I021_110();// Trajectory Intent
				}
				if(fspec[27]){	
					decode_I021_070();// Mode 3/A Code
				}
				if(fspec[28]){	
					decode_I021_131();// Signal Amplitude
				}
				if(fspec[29]){	
					decode_I021_910();// Категория навигационной целостности
				}
				if(fspec[30]){	
					decode_I021_920();// Характеристика наблюдения цели
				}
				if(fspec[31]){	
					decode_I021_930();// Бортовой номер цели в режиме УВД
				}

				if(pMRT != NULL) pMRT->AddPlotMRT(PlotMRT);
				//if(pMRTInf_Rec != NULL) pMRTInf_Rec->AddPlotMRTInf(m_PlotMRT);

			}while(index < (L-2));
		}//End Category 21
		
		if(category == 48)
		{
			do
			{
				for(i=0; i<33; i++) fspec[i]=false;

				IKI = B = SPI =0;
				MS_type = 0;
				MK = 1;
				X = Y = 0;
				EXNB = 0;
				NB = 0;
				Address_ICAO = 0;
				EXH = ZH = 0;
				H = 0;
				Height_3D = 0;
				FLAG = 0;
				TYP = 0;
				Ident.Empty();
				D = Az = 0;
				H = 0;
				OT = BRF = 0;
				time_stamp.ZeroTime();
				OriginCoord = 0;

				//FSPEC
				byte = read_buffer();
				for(i=0; i<7; i++) 
				{
					if(byte & (0x80>>i)) fspec[i+1]=true;
				}
				if(byte & 0x01)
				{
					byte = read_buffer();
					for(i=0; i<7; i++)
					{
						if(byte & (0x80>>i)) fspec[i+8]=true;
					}
					if(byte & 0x01)
					{
						byte = read_buffer();
						for(i=0; i<7; i++)
						{
							if(byte & (0x80>>i)) fspec[i+15]=true;
						}
						if(byte & 0x01)
						{
							byte = read_buffer();
							for(i=0; i<7; i++)
							{
								if(byte & (0x80>>i)) fspec[i+22]=true;
							}
						}
					}
				}
				//End FSPEC

				if(fspec[1]){	//Data Source Identifier I001/010
					decode_I048_010();
				}
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}

				if(fspec[2]){	//Time of Day
					decode_I048_140();
				}
				if(fspec[3]){	//Target report descriptor
					decode_I048_020();
				}
				if(fspec[4]){	//Measured position in slant polar coordinates
					decode_I048_040();
				}
				if(fspec[5]){	//Mode 3/A Octal I001_070
					decode_I048_070();					
				}
				if(fspec[6]){	//Mode C code Binary I001/090
					decode_I048_090();					
				}
				if(fspec[7]){
					decode_I048_130();// Radar Plot Characteristics
				}
				if(fspec[8]){	
					decode_I048_220();// Aircraft Address
				}
				if(fspec[9]){	
					decode_I048_240();//Aircraft Identification
				}
				if(fspec[10]){	
					decode_I048_250();//Mode S MB Data
				}
				if(fspec[11]){	//Номер траектории
					decode_I048_161();
				}
				if(fspec[12]){	//Вычисленное положение в декартовых координатах
					decode_I048_042();
				}
				if(fspec[13]){	//Вычисленная скорость сопровождения траектории в полярных координатах
					decode_I048_200();
				}
				if(fspec[14]){	//Статус траектории сопровождени
					decode_I048_170();
				}
				if(fspec[15]){	
					decode_I048_210();//Treck quality
				}
				if(fspec[16]){	
					decode_I048_030();
				}
				if(fspec[17]){	
					decode_I048_080();
				}
				if(fspec[18]){	
					decode_I048_100();
				}			
				if(fspec[19]){	
					decode_I048_110();//Hieght measured by 3D Radar
				}
				if(fspec[20]){	
					decode_I048_120();
				}
				if(fspec[21]){	
					decode_I048_230();//Mode 1
				}
				if(fspec[22]){	
					decode_I048_260();
				}
				if(fspec[23]){	
					decode_I048_055();//Mode 1
				}
				if(fspec[24]){	
					decode_I048_050();//Mode 2
				}
				if(fspec[25]){	
					decode_I048_065();
				}
				if(fspec[26]){	
					decode_I048_060();
				}
				if(fspec[27]){	
					byte = read_buffer();
					int len_re = byte;
					for (int k = 1; k < len_re; k++)
					{
						byte = read_buffer();
					}
				}
				if(fspec[28]){	
					byte = read_buffer();
					int len_re = byte;
					for (int k = 1; k < len_re; k++)
					{
						byte = read_buffer();
					}
				}

				/////////MODE S/////////////
				/*if(IKI==0x02){
					IKI=0x04;
					Address_ICAO=0x00FF0000 + NB;
				}*/

				PlotMRT.IKI = IKI;
				PlotMRT.MS_type = MS_type;
				PlotMRT.MK  = MK;
				PlotMRT.b = B;
				PlotMRT.SPI = SPI;
				PlotMRT.x = X;
				PlotMRT.y = Y;
				PlotMRT.d_Az = Az;
				PlotMRT.d_D = D;
				PlotMRT.ZH = ZH;//!
				PlotMRT.ot = OT;//!
				PlotMRT.BRF = BRF;//!

				PlotMRT.EXNB = EXNB;
				PlotMRT.number = NB;
				PlotMRT.Address_ICAO = Address_ICAO;
				CMyMath MM;
				PlotMRT.State = MM.ICAOCodeToState(PlotMRT.Address_ICAO);

				PlotMRT.EXH = EXH;
				PlotMRT.heigh = H;
				PlotMRT.Height_3D = Height_3D;
				PlotMRT.flag = FLAG;
				PlotMRT.SAC = SAC;
				PlotMRT.SIC = SIC;
				PlotMRT.Ident = Ident;
				PlotMRT.track = TYP;
				PlotMRT.m_nLpd = i_comport;
				PlotMRT.m_ucLpdType = uc_LpdType;
				PlotMRT.time_stamp = time_stamp;//!
				PlotMRT.OriginCoord = OriginCoord;//!



				if(pMRT != NULL) pMRT->AddPlotMRT(PlotMRT);
				if ((index<(L - 3)))
				{
					int gdfgfgd=0;
				}
			}while(index<(L - 3)); //(index < len)
		}

		if(category == 34)
		{
			UCHAR MessageType = 0;

			do
			{
				for(i=0; i<16; i++) fspec[i] = false;
				
				byte = read_buffer();
				for(i=0; i<7; i++)
				{
					if(byte & (0x80>>i)) fspec[i+1]=true;
				}
				if(byte & 0x01)
				{
					byte = read_buffer();
					for(i=0; i<7; i++)
					{
						if(byte & (0x80>>i)) fspec[i+8]=true;
					}
				}
				//End FSPEC

				if(fspec[1]){	
					decode_I034_010();	//Data sorce identifier
				}
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}

				if(fspec[2]){	
					MessageType = decode_I034_000();//Message type
				}
				if(fspec[3]){	
					decode_I034_030();//Time of day
				}
				if(fspec[4]){
					decode_I034_020();//Sector number
				}
				if(fspec[5]){
					decode_I034_041();
				}
				if(fspec[6]){	
					decode_I034_050();//System configuration and status
				}
				if(fspec[7]){	
					decode_I034_060();//System processing mode
				}
				if(fspec[8]){
					decode_I034_070();
				}
				if(fspec[9]){
					decode_I034_100();
				}
				if(fspec[10]){	
					decode_I034_110();
				}
				if(fspec[11]){	
					decode_I034_120();//3D position of Data sourse
				}
				if(fspec[12]){	
					decode_I034_090();//Collimation error
				}
				if(fspec[13]){	
					byte = read_buffer();
					int num_oct = byte;
					for (int k = 1; k < num_oct; k++)
					{
						byte = read_buffer();
					}
				}
				if(fspec[14]){	
					byte = read_buffer();
					int num_oct = byte;
					for (int k = 1; k < num_oct; k++)
					{
						byte = read_buffer();
					}
				}
				/* Симферополь - 25.01.2010
				NorthMRT.SAC=SAC;
				NorthMRT.SIC=SIC;
				NorthMRT.m_nLpd=i_comport;
				NorthMRT.m_ucLpdType=uc_LpdType;

				if(pMRT!=NULL) pMRT->AddNorthMRT(NorthMRT);*/

			/*	if(MessageType == 0x02)//Am
				{
					if(pMRT != NULL)
					{
						pMRT->AddAmMRT(AmMRT);
						AmMRT.track = 1;
						pMRT->AddAmMRT(AmMRT);
					}
				}*/
				if(MessageType == 0x01)//North
				{
					if(pMRT != NULL)
					{
						NorthMRT.time_stamp = time_stamp;

						NorthMRT.RLKState = RlkSt;
						pMRT->AddNorthMRT(NorthMRT);
						NorthMRT.track = 1;
						pMRT->AddNorthMRT(NorthMRT);
					}
				}
					
				if(MessageType == 0x02)//Am
				{
					if(pMRT != NULL)
					{
						AmMRT.time_stamp = time_stamp;

						pMRT->AddAmMRT(AmMRT);
						AmMRT.track = 1;
						pMRT->AddAmMRT(AmMRT);
					}
				}

			}while(index < (L - 3));
		}
		//if(category == 62)
		//{
		//	do
		//	{
		//		for(i=0; i<=35; i++) fspec[i]=false;

		//		TrackMRT.Zero();
		//		TrackMRT.estm_Plot.MK=1;


		//		//FSPEC
		//		byte = read_buffer();
		//		for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

		//		if(byte & 0x01) //FX
		//		{	
		//			byte = read_buffer();
		//			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
		//		}

		//		if(byte & 0x01) //FX
		//		{	
		//			byte = read_buffer();
		//			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+15] = true;}
		//		}

		//		if(byte & 0x01) //FX
		//		{	
		//			byte = read_buffer();
		//			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+22] = true;}
		//		}

		//		if(byte & 0x01) //FX
		//		{	
		//			byte = read_buffer();
		//			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+29] = true;}
		//		}
		//		//End FSPEC

		//		TrackMRT.deltaT_H = 0;
		//		TrackMRT.deltaT_NB = 0;

		//		if(fspec[1]){	//Data Source Identifier I062/010
		//			decode_I062_010();
		//			if(b_Add_SacSic)
		//			{
		//				SAC = def_SAC;
		//				TrackMRT.estm_Plot.SAC = def_SAC;

		//				SIC = def_SIC;
		//				TrackMRT.estm_Plot.SIC = def_SIC;
		//			}
		//		}
		//		if(fspec[2]){	//	SPARE
		//			decode_I062_000();
		//		}
		//		if(fspec[3]){	//	I062/015
		//			decode_I062_015();
		//		}
		//		if(fspec[4]){	//	I062/070
		//			decode_I062_070();//	
		//		}
		//		if(fspec[5]){	//	I062/105
		//			decode_I062_105();
		//		}
		//		if(fspec[6]){	//	I062/100 координаты
		//			decode_I062_100();
		//			PlotMRT.OriginCoord=0;
		//		}
		//		if(fspec[7]){	//  Скорость
		//			decode_I062_185();
		//		}
		//		if(fspec[8]){	//Ускорение
		//			decode_I062_210();
		//		}
		//		if(fspec[9]){
		//			decode_I062_060();//Track mode 3/A
		//		}
		//		if(fspec[11]){
		//			decode_I062_380();//Aircraft Derived Data
		//		}
		//		if(fspec[12]){
		//			decode_I062_040();//Index
		//		}
		//		if(fspec[13]){
		//			decode_I062_080();//Board number 3/A
		//		}
		//		if(fspec[15]){
		//			decode_I062_200();
		//		}
		//		if(fspec[17]){	
		//			decode_I062_136();
		//		}
		//		if(fspec[19]){
		//			decode_I062_135();
		//		}
		//		if(fspec[20]){
		//			decode_I062_220();
		//		}
		//		if(fspec[21]){
		//			decode_I062_390();
		//		}
		//		if(fspec[35]){
		//			decode_I062_SP();
		//		}

		//		//Эту ошибку обнаружил в ЛИИ Фальков А.И.
		//			//При воспроизведении в 3D самолеты шли на 0-высоте.
		//			//Эти строки исправляют обнаруженную ошибку.
		//		if(TrackMRT.estm_Plot.EXH)
		//		{
		//			double dB, dL, dH;
		//			TrackMRT.estm_Plot.GetBLH(dB, dL, dH);
		//			dH = TrackMRT.estm_Plot.heigh;
		//			TrackMRT.estm_Plot.SetBLH(dB, dL, dH);
		//		}

		//												
		//		PlotMRT.IKI = IKI;
		//		PlotMRT.MK  = MK;
		//		PlotMRT.x = X;
		//		PlotMRT.y = Y;
		//		PlotMRT.EXNB = EXNB;
		//		PlotMRT.number = NB;
		//		PlotMRT.EXH = EXH;
		//		PlotMRT.ZH = ZH;
		//		PlotMRT.heigh = H;
		//		PlotMRT.b = B;
		//		PlotMRT.ot = OT;
		//		PlotMRT.BRF = BRF;
		//		PlotMRT.SPI = SPI;
		//		PlotMRT.flag = FLAG;
		//		PlotMRT.SAC = SAC;
		//		PlotMRT.SIC = SIC;

		//		//if(pMRT!=NULL) pMRT->AddPlotMRT(SAC, SIC, TYP, i_comport, PlotMRT);
		//		if(pTrackMRT == NULL)
		//		{
		//			if(pMRT != NULL) pMRT->AddTrackMRT(i_comport, TrackMRT);
		//		}
		//		else
		//		{
		//			*pTrackMRT = TrackMRT;
		//			return;
		//		}
		//		
		//	
		//	}while(index<(L-3));
		//}
		if(category == 62)
		{
			//do
			{
				for(i=0; i<=35; i++) fspec[i]=false;

				TrackMRT.Zero();
				TrackMRT.estm_Plot.MK=1;

				//FSPEC
				byte = read_buffer();
				for(i = 0; i < 7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
				}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+15] = true;}
				}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+22] = true;}
				}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+29] = true;}
				}
				//End FSPEC

				TrackMRT.deltaT_H = 0;
				TrackMRT.deltaT_NB = 0;

				if(fspec[1])	//Data Source Identifier I062/010
				{
					decode_I062_010();
				}
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					TrackMRT.estm_Plot.SAC = def_SAC;
					SIC = def_SIC;
					TrackMRT.estm_Plot.SIC = def_SIC;
				}

				if(fspec[2]){	//	SPARE
					decode_I062_000();
				}
				if(fspec[3]){	//	I062/015 Service Identification
					decode_I062_015();
				}
				if(fspec[4]){	//	I062/070 Time Of Track Information
					decode_I062_070();
				}
				if(fspec[5]){	//	I062/105 Calculated Position In WGS-84 Co-ordinates
					decode_I062_105();
                    TrackMRT.estm_Plot.OriginCoord = 2;
				}
				if(fspec[6]){	//	I062/100 Calculated Track Position. (Cartesian)
					decode_I062_100();
					TrackMRT.estm_Plot.OriginCoord = 0;
					PlotMRT.OriginCoord = 0;
				}
				if(fspec[7]){	//  Calculated Track Velocity (Cartesian)
					decode_I062_185();
				}
				if(fspec[8]){	//	Calculated Acceleration (Cartesian)
					decode_I062_210();
				}
				if(fspec[9]){
					decode_I062_060();//Track Mode 3/A Code
				}
				if(fspec[10]){
					decode_I062_245();//Позывной (8 символов)
				}
				if(fspec[11]){
					decode_I062_380();//Aircraft Derived Data (типы данных, получаеиые от ВС)
				}
				if(fspec[12]){
					decode_I062_040();//Номер трека
				}
				if(fspec[13]){
					decode_I062_080();//Статус трека
				}
				if(fspec[14]){
					decode_I062_290();//TSystem Track Update Ages
				}
				if(fspec[15]){
					decode_I062_200();//Mode of Movement
				}
				if(fspec[16]){
					decode_I062_295();//Track Data Ages
				}
				if(fspec[17]){	
					decode_I062_136();//Measured Flight Level
				}
				if(fspec[18]){
					decode_I062_130();//Calculated Track Geometric Altitude
				}
				if(fspec[19]){
					decode_I062_135();//Calculated Track Barometric Altitude
				}
				if(fspec[20]){
					decode_I062_220();//Calculated Rate Of Climb/Descent
				}
				if(fspec[21]){
					decode_I062_390();//Flight Plan Related Data
				}
				if(fspec[28]){
					decode_I062_340();//Measured Information
				}
				if(fspec[35]){
					decode_I062_SP();
				}

				//Эту ошибку обнаружил в ЛИИ Фальков А.И.
				//При воспроизведении в 3D самолеты шли на 0-высоте.
				//Эти строки исправляют обнаруженную ошибку.
				if( TrackMRT.estm_Plot.EXH && TrackMRT.estm_Plot.OriginCoord == 2 )
				{
					double dB, dL, dH;

					TrackMRT.estm_Plot.GetBLH(dB, dL, dH);
					dH = TrackMRT.estm_Plot.heigh;
					TrackMRT.estm_Plot.SetBLH(dB, dL, dH);
				}

				PlotMRT.IKI = IKI;
				PlotMRT.MK  = MK;
				PlotMRT.x = X;
				PlotMRT.y = Y;
				PlotMRT.EXNB = EXNB;
				PlotMRT.number = NB;
				PlotMRT.EXH = EXH;
				PlotMRT.ZH = ZH;
				PlotMRT.heigh = H;
				PlotMRT.b = B;
				PlotMRT.ot = OT;
				PlotMRT.BRF = BRF;
				PlotMRT.SPI = SPI;
				PlotMRT.flag = FLAG;
				PlotMRT.SAC = SAC;
				PlotMRT.SIC = SIC;
                TrackMRT.estm_Plot.m_ucLpdType = uc_LpdType;
                TrackMRT.estm_Plot.m_nLpd = i_comport;
                TrackMRT.estm_Plot.track = 1;
			
				TrackMRT.estm_Plot.IKI = IKI;
				TrackMRT.estm_Plot.MK  = MK;
				TrackMRT.estm_Plot.x = X;
				TrackMRT.estm_Plot.y = Y;
				TrackMRT.estm_Plot.EXNB = EXNB;
				TrackMRT.estm_Plot.number = NB;
				TrackMRT.estm_Plot.EXH = EXH;
				TrackMRT.estm_Plot.ZH = ZH;
				TrackMRT.estm_Plot.heigh = H;
				TrackMRT.estm_Plot.b = B;
				TrackMRT.estm_Plot.ot = OT;
				TrackMRT.estm_Plot.BRF = BRF;
				TrackMRT.estm_Plot.SPI = SPI;
				TrackMRT.estm_Plot.flag = FLAG;
				TrackMRT.estm_Plot.SAC = SAC;
				TrackMRT.estm_Plot.SIC = SIC;



                PlotMRT.m_ucLpdType = uc_LpdType;
                PlotMRT.m_nLpd = i_comport;
                PlotMRT.track = 1;

				if(pTrackMRT == NULL)
				{
					if(pMRT != NULL)
						pMRT->AddTrackMRT(i_comport, TrackMRT);
                        //pMRT->AddPlotMRT(PlotMRT);
				}
				else
				{
					*pTrackMRT = TrackMRT;
					return;
				}
			}//while(index < (L - 3));
		}

		if( category == 63 )
		{
			do
			{
				for(i=0; i<=16; i++) fspec[i]=false;

                SensorStatus.Clear();

				//FSPEC
				byte = read_buffer();
				for(i = 0; i < 7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
				}
				//End FSPEC

				if(fspec[1])	//Data Source Identifier I063/010
				{
					decode_I063_010();
				}
				if(b_Add_SacSic)
				{
                    SensorStatus.m_SAC_SDPS = def_SAC;
					SensorStatus.m_SIC_SDPS = def_SIC;
				}
				if(fspec[2]){	//	I063/015
					decode_I063_015();
				}
				if(fspec[3]){	//	I063/030
					decode_I063_030();
				}
				if(fspec[4]){	//	I063/050
					decode_I063_050();	
				}
				if(fspec[5]){	//	I063/060
					decode_I063_060();
				}
				if(fspec[6]){	//	I063/070 
					decode_I063_070();
				}
				if(fspec[7]){	//  I063/080
					decode_I063_080();
				}
				if(fspec[8]){	//  I063/081
					decode_I063_081();
				}
				if(fspec[9]){   //  I063/090
					decode_I063_090();
				}
				if(fspec[10]){  //  I063/091
					decode_I063_091();
				}
				if(fspec[11]){  //  I063/092
					decode_I063_092();
				}

                SensorStatus.m_ucLpdType = uc_LpdType;
                SensorStatus.m_nLpd = i_comport;

                if(pMRT != NULL)
					pMRT->AddSensorStatus( SensorStatus );
			
			}while(index < (L - 3));
		}

		if(category == 201)
		{
			do
			{
				for(i=0; i<25; i++) fspec[i]=false;
				
				unsigned char SAC, SIC;
				unsigned char IKI, B, MK;
				long X, Y;
				unsigned char EXNB;
				long NB;
				unsigned char EXH, ZH;
				long H;
				unsigned char OT, BRF;

				IKI = B = MK = 0;
				X = Y = 0;
				Az = D = 0;
				EXNB = 0;
				NB = 0;
				EXH = ZH = 0;
				H = 0;
				OT = BRF = 0;
				SPI = FLAG = 0;
				OriginCoord = 0;

				//FSPEC
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
				}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+15] = true;}
				}
				//End FSPEC

				if(fspec[1]) //Data Source Identifier I001/010
				{	
					byte = read_buffer();
					SAC = byte;
					byte = read_buffer();
					SIC = byte;

					if(b_Add_SacSic)
					{
						SAC = def_SAC;
						SIC = def_SIC;
					}
				}

				if(fspec[2]) //Тип плота I201/002
				{					
					byte = read_buffer();
					IKI = byte >> 6;
					MK = (byte & 0x20) >> 5;
					B = (byte & 0x10) >> 4;
					OT = byte & 0x04;

				}

				if(fspec[3]) //Measured position I201/003
				{					
					long l1, l2, l3, l4;
					
					l1=read_buffer();
					l2=read_buffer();
					l3=read_buffer();
					l4=read_buffer();

					X= ((l1 & 0x7F)<<24) + (l2<<16) + (l3<<8) +l4;
					if(l1 & 0x80) X=-X;

					l1=read_buffer();
					l2=read_buffer();
					l3=read_buffer();
					l4=read_buffer();

					Y = ((l1 & 0x7F)<<24) + (l2<<16) + (l3<<8) +l4;
					if(l1 & 0x80) Y=-Y;

					
					
//					str.Format("X=%ld, Y=%ld", X, Y);
//					pGView->MessageBox(str);
					OriginCoord=0;
				}

				if(fspec[4]) //NB I201/004
				{										
					long l1, l2, l3;//, l4;

					EXNB=1;

					l1=read_buffer();
					l2=read_buffer();
					l3=read_buffer();

					NB = (l1<<16) + (l2<<8) +l3;
					

					//str.Format("NB=%ld", NB);
					//pGView->MessageBox(str);
				}

				if(fspec[5]) //H I201/005
				{					
					long l1, l2, l3;//, l4;

					EXH=1;

					l1=read_buffer();
					l2=read_buffer();
					l3=read_buffer();

					H = ((l1 & 0x7F)<<16) + (l2<<8) +l3;

					ZH=(unsigned char)(l1>>7);	//RBS - absolute heigh
					
					//str.Format("H=%ld", H);
					//pGView->MessageBox(str);
				}

				if(fspec[6]) //BRF I201/006
				{					
					BRF=read_buffer();
				}
				
				
				PlotMRT.IKI = IKI;
				PlotMRT.MK  = MK;
				PlotMRT.x = X;
				PlotMRT.y = Y;
				PlotMRT.d_Az = Az;
				PlotMRT.d_D = D;
				PlotMRT.EXNB = EXNB;
				PlotMRT.number = NB;
				PlotMRT.EXH = EXH;
				PlotMRT.ZH = ZH;
				PlotMRT.heigh = H;
				PlotMRT.b = B;
				PlotMRT.ot = OT;
				PlotMRT.BRF = BRF;
				PlotMRT.SPI = SPI;
				PlotMRT.flag = FLAG;
				PlotMRT.SAC = SAC;
				PlotMRT.SIC = SIC;

				PlotMRT.track = TYP;
				PlotMRT.m_nLpd = i_comport;
				PlotMRT.m_ucLpdType = uc_LpdType;
				PlotMRT.OriginCoord = OriginCoord;

				if(pMRT != NULL) pMRT->AddPlotMRT(PlotMRT);
				
			}while(index<(L-3));
		}

		if(category == 202)
		{
			unsigned char SAC, SIC;
			  
			for(i=0; i<25; i++) fspec[i]=false;
			
			byte = read_buffer();
			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

			if(byte & 0x01)	//FX
			{
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
			}
			//End FSPEC

			if(fspec[1])	//Data Source Identifier I202/001
			{					
				byte = read_buffer();
				SAC = byte;
				byte = read_buffer();
				SIC = byte;
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}
			}

			if(fspec[2])	//Север I202/002
			{
				byte = read_buffer();

				if(pMRT != NULL)
				{
					CMRTNorth N(SAC, SIC, 0, i_comport, uc_LpdType);
					pMRT->AddNorthMRT(N);
					N.track = 1;
					pMRT->AddNorthMRT(N);
				}
			}

			if(fspec[3])	//Sector Number I002/020
			{
				byte = read_buffer();

				am = ((long)(byte))*30;

				if(pMRT != NULL)
				{
					CMRTAm Am(SAC, SIC, 0, i_comport, uc_LpdType, float(am));
					pMRT->AddAmMRT(Am);
					Am.track = 1;
					pMRT->AddAmMRT(Am);
				}
			}
		}

		if(category == 241)
		{
			time_stamp.ZeroTime();
			for(i=0; i<24; i++) fspec[i] = false;
			
			//FSPEC
			byte = read_buffer();
			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

			if(byte & 0x01) //FX
			{	
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
			}

			if(byte & 0x01) //FX
			{	
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+15]=true;}
			}

			if(byte & 0x01) //FX
			{	
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+22]=true;}
			}
			//End FSPEC


			if(fspec[1]) // I241/010
			{	
				decode_I241_010();
			}
			if(b_Add_SacSic)
			{
				SAC=def_SAC;
				SIC=def_SIC;
			}

			UCHAR type;
			if(fspec[2]){	// I241/000
				type=decode_I241_000();
			}
			if(fspec[3]){	// I241/600
				decode_I241_600();
			}
			if(fspec[4]){	// I241/030
				decode_I241_030();
			}
			if(fspec[5]){	// I241/601
				decode_I241_601();
			}

			if(type == 0x01)
			{
				MeteoPolygon.Clear();
				if(fspec[6]){	// I241/602
					decode_I241_602();
				}

				if(fspec[7]) // I241/105
				{	
					byte = read_buffer();
					int n = 0;
					n = (int)byte;
					for(i=0; i<n; i++) decode_I241_105();						
				}
				MeteoPolygon.m_MsgNumber = MsgNumber;
				MeteoPolygon.SAC = SAC;
				MeteoPolygon.SIC = SIC;
				MeteoPolygon.m_TimeCurr = TimeGet;
				MeteoPolygon.m_TimeStop = TimeStop;
				MeteoPolygon.m_nLpd = i_comport;
				MeteoPolygon.m_ucLpdType = uc_LpdType;

				MeteoPolygon.MakeOrthodrom();
					
				if(pMRT != NULL) pMRT->AddMeteo_Polygon(MeteoPolygon, FALSE);
			}

			if(type==0x02 || type==0x03)
			{
				MeteoRW.Clear();
				if(type==0x02)
					MeteoRW.m_bStorm=FALSE;
				else
					MeteoRW.m_bStorm=TRUE;

				if(fspec[8]){	// I241/603
					decode_I241_603();
				}
				if(fspec[9]){	// I241/604
					decode_I241_604();
				}
				if(fspec[10]){	// I241/605
					decode_I241_605();
				}
				if(fspec[11]){	// I241/606
					decode_I241_606();
				}
				if(fspec[12]){	// I241/607
					decode_I241_607();
				}
				if(fspec[13]){	// I241/608
					decode_I241_608();
				}
				if(fspec[14]){	// I241/609
					decode_I241_609();
				}
				if(fspec[15]){	// I241/610
					decode_I241_610();
				}
				if(fspec[16]){	// I241/611
					decode_I241_611();
				}
				if(fspec[17]){	// I241/612
					decode_I241_612();
				}
				if(fspec[18]){	// I241/613
					decode_I241_613();
				}
				if(fspec[19]){	// I241/614
					decode_I241_614();
				}
				if(fspec[20]){	// I241/615
					decode_I241_615();
				}
				if(fspec[21]){	// I241/616
					decode_I241_616();
				}
				if(fspec[22]){	// I241/617
					decode_I241_617();
				}
				if(fspec[23]){	// I241/618
					decode_I241_618();
				}

				MeteoRW.m_MsgNumber = MsgNumber;
				MeteoRW.SAC = SAC;
				MeteoRW.SIC = SIC;
				MeteoRW.m_TimeGet = TimeGet;
				MeteoRW.m_TimeStop = TimeStop;
				MeteoRW.m_nLpd = i_comport;
				MeteoRW.m_ucLpdType = uc_LpdType;

				if(pMRT != NULL) pMRT->AddMeteoRW(MeteoRW, FALSE, TRUE);
			}
		}

		if(category == 242)
		{
//			do{
				for(i=0; i<25; i++) fspec[i]=false;
				//FSPEC
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

				if(byte & 0x01) //FX
				{	
					byte = read_buffer();
					for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
				}
				//End FSPEC

				if(fspec[1]) //Data Source Identifier I002/010
				{	
					decode_I242_010();
				}
				if(b_Add_SacSic)
				{
					SAC=def_SAC;
					SIC=def_SIC;
				}

				if(fspec[2]) //Message Type I002/000 FileTime
				{	
					CMRTTime Time;
					Time = decode_I242_020();
					//if(pMRT!=NULL) pMRT->AddTimeMRT(Time);
					if(pUTS != NULL) pUTS->TimePacket(0, 0, Time);
					
				}
//			}while(index<(L-3));	
		}

		if(category == 250)
		{
			SAC = SIC = 0;
			for(i=0; i<25; i++) fspec[i]=false;
		
			byte = read_buffer();
			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

			if(byte & 0x01) //FX
			{	
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
			}
			//End FSPEC

			if(fspec[1])	//Метка идентификации источника I250/010
			{
				SAC = read_buffer();
				SIC = read_buffer();

				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}
			}

			if(fspec[6])	//Пакет сектора
			{
				int Sector = read_buffer();
				if(pMRT != NULL) pMRT->DelA2D_RLI_OLD(SAC, SIC, i_comport, uc_LpdType, Sector);
				
			}
			if(fspec[7])	//Пакет видео
			{
				A2D_RLI res_A2D_RLI;
				unsigned int iAngle, iDist;
				UCHAR type;
				UCHAR byte1, byte2;
				
				int Rep = read_buffer();

				for (int i=0; i<Rep; i++)
				{
					//Азимут начала пакета
					byte1 = read_buffer();
					byte2 = read_buffer();
					iAngle = (unsigned int) byte2;
					iAngle = (iAngle<<8) + (unsigned int)(byte1);					
					type = byte2 >> 6;//0 - вторичка, 1 - первичка					
					iAngle &= 0xFFF;
					res_A2D_RLI.AngleStart = float (iAngle*360 / 4095);

					//Дальность начала пакета
					int koef = 100;

					byte1 = read_buffer();
					byte2 = read_buffer();
					if (type == 1) koef = byte2 >> 5;
					iDist = (unsigned int) byte2;					
					iDist = (iDist<<8) + (unsigned int) (byte1);
					iDist &= 0x1FFF;
					res_A2D_RLI.DistStart = iDist * koef;

					//Азимут конца пакета
					byte1 = read_buffer();
					byte2 = read_buffer();
					iAngle = (unsigned int) byte2;
					iAngle = (iAngle<<8) + (unsigned int)(byte1);					
					type = byte2 >> 6;//0 - вторичка, 1 - первичка					
					iAngle &= 0xFFF;
					res_A2D_RLI.AngleEnd = float (iAngle*360 / 4095);

					//Ширина пакета
					byte = read_buffer();
					res_A2D_RLI.Width = 100 * byte;					

					if(pMRT != NULL) pMRT->AddA2D_RLI(SAC, SIC, i_comport, uc_LpdType, res_A2D_RLI);					
				}

				//Второй пакет(сектор) здесь же. Чтобы не зацикливать делаем так-
				res_A2D_RLI.Sector = 0;
				if (index < (L-3))
				{
					byte = read_buffer();
					if (byte == 0x84)
					{
						byte = read_buffer();
						byte = read_buffer();
						int Sector = read_buffer();
						res_A2D_RLI.Sector = Sector;

						if(pMRT != NULL) pMRT->DelA2D_RLI_OLD(SAC, SIC, i_comport, uc_LpdType, Sector);
					}					
				}
			}
		}

		if(category == 253)
		{
			time_stamp.ZeroTime();
			float fPeleng;
			UCHAR ucChanel;
			fPeleng = 0;
			ucChanel = 0;
			SAC = SIC = 0;
			for(i=0; i<25; i++) fspec[i]=false;
		
			byte = read_buffer();
			for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+1] = true;}

			if(byte & 0x01) //FX
			{	
				byte = read_buffer();
				for(i=0; i<7; i++) {if(byte & (0x80>>i)) fspec[i+8] = true;}
			}
			//End FSPEC

			if(fspec[1])	//Метка идентификации источника пеленга I253/N01
			{
				byte = read_buffer();
				SAC = byte;
				byte = read_buffer();
				SIC = byte;
				if(b_Add_SacSic)
				{
					SAC = def_SAC;
					SIC = def_SIC;
				}
			}

			if(fspec[2])	//Направление на объект (Пеленг) I253/N02
			{
				ucChanel = read_buffer();

				unsigned int i16Peleng;
				byte = read_buffer();
				i16Peleng = (unsigned int)(byte);
				byte = read_buffer();
				i16Peleng = (i16Peleng<<8) + (unsigned int)(byte);
				fPeleng = float(i16Peleng*360/65536);
			}

			if(fspec[3])	// Каналы АРП включены I253/N03
			{
				byte = read_buffer();
				byte = read_buffer();
			}

			if(fspec[4])	// Каналы АРП выключены I253/N04
			{
				byte = read_buffer();
				byte = read_buffer();
			}

			PelengMRT.Clear();
			PelengMRT.m_bAnalogDigital = TRUE;
			PelengMRT.m_fPeleng = fPeleng;
			PelengMRT.m_iBRF = -1;
			PelengMRT.m_nLpd = i_comport;
			PelengMRT.m_ucLpdType = uc_LpdType;
			PelengMRT.m_SAC = SAC;
			PelengMRT.m_SIC = SIC;
			PelengMRT.m_ucChanel = ucChanel;

			if (PelengMRT.m_ucChanel > 0)
			{
				PelengMRT.m_ucChanel = PelengMRT.m_ucChanel - 1;
				if(pMRT != NULL) pMRT->AddPelengMRT(PelengMRT);
			}
		}
	}
}

void CDecodeAsterix::decode_I001_010()
{
	byte = read_buffer();
	SAC = byte;
	byte = read_buffer();
	SIC = byte;
}

void CDecodeAsterix::decode_I001_020()
{
	unsigned char SIM, SSR_PSR, RAB;
	unsigned char TST, DS1_DS2, ME, MI; 
		
	MK = 1;

	byte = read_buffer();
	if(byte & 0x80) TYP = 1;
	else TYP = 0;
	if(byte & 0x40) SIM = 1;
	else SIM = 0;
	SSR_PSR = (byte & 0x30) >> 4;
	if(byte & 0x08) ANT = 1;
	else ANT = 0;
	if(byte & 0x04) SPI = 1;
	else SPI = 0;
	if(byte & 0x02) RAB = 1;
	else RAB = 0;

	TST = DS1_DS2 = ME = MI = 0; 

	while(byte & 0x01) 
	{
		byte = read_buffer();
		if(byte & 0x80) TST = 1;
		DS1_DS2 = (byte & 0x60) >> 5;
		if(byte & 0x10) ME = 1;
		if(byte & 0x08) MI = 1;
	}
	IKI = SSR_PSR;

	switch(DS1_DS2)
	{
		case 1: B = 3; break;
		case 2: B = 2; break;
		case 3: B = 1; break;
		default: B = 0; break;
	}

	FLAG = 0x00;
	if(TST) FLAG = 0x01;
	if(SIM) FLAG = 0x02;		
}

void CDecodeAsterix::decode_I001_040()
{
	long Rho, Theta;
	double d_rho, d_theta; 

	byte = read_buffer();
	Rho = (long)(byte);
	byte = read_buffer();
	Rho = (Rho<<8) + byte;

	byte = read_buffer();
	Theta = (long)(byte);
	byte = read_buffer();
	Theta = (Theta<<8) + byte;

	d_rho = (double)(Rho);
	d_rho = d_rho*1852/128;

	d_theta = (double)(Theta);
	//d_theta=d_theta*0.0055;
	//d_theta=d_theta*0.0054931640625;
	d_theta = d_theta*(360.0/(4096*16));

	Az = d_theta;
	D = d_rho;

	X = (long)(d_rho * sin(d_theta*gr2rad));
	Y = (long)(d_rho * cos(d_theta*gr2rad));
}

void CDecodeAsterix::decode_I001_070()
{
	unsigned char a, b, c, d;
	a=b=c=d=0;
	byte = read_buffer();
					
	EXNB = 0;
	if(!(byte & 0x80)) EXNB = 1;
	if(byte & 0x08) a = a|0x04;
	if(byte & 0x04) a = a|0x02;
	if(byte & 0x02) a = a|0x01;
	if(byte & 0x01) b = b|0x04;
					
	byte=read_buffer();
	if(byte & 0x80) b = b|0x02;
	if(byte & 0x40) b = b|0x01;
	if(byte & 0x20) c = c|0x04;
	if(byte & 0x10) c = c|0x02;
	if(byte & 0x08) c = c|0x01;
	if(byte & 0x04) d = d|0x04;
	if(byte & 0x02) d = d|0x02;
	if(byte & 0x01) d = d|0x01;

	NB=0;
	if(EXNB) NB = ((long)(a))*1000 + ((long)(b))*100 + ((long)(c))*10 + ((long)(d));

}

void CDecodeAsterix::decode_I001_090()
{
	ZH = 1;	//RBS - absolute heigh
	byte = read_buffer();
	if(!(byte & 0x80)) EXH = 1;
	
	H = (long)(byte & 0x3F);
	byte = read_buffer();
	H = (H<<8) + (long)(byte);
	if(H & 0x2000) H = H - (1<<14);
	//H=long((double(H))*1852.0*25/6080);
	H = long((double(H)) * 0.3048*25);
}

void CDecodeAsterix::decode_I001_030()
{
	int	iStByte = 0;

	while((byte = read_buffer()) & 0x01)
	{
		iStByte++;
		if(iStByte > 13)
			break;
	}
}

void CDecodeAsterix::decode_I001_SP()
{
	//RADAR PLOT SPECIAL DATA ITEM
	unsigned char s_len, s_fspec;

	byte = read_buffer();
	s_len = byte;
	if(s_len == 0x00 || s_len == 0x01) return;

	byte = read_buffer();
	s_fspec = byte;

	//EXNB=0;
	if(s_fspec & 0x80) //Board Number
	{
		MK = 0;
		EXNB = 1;
		byte = read_buffer();
		NB = (long)(byte);
		byte = read_buffer();
		NB = (NB<<8) + (long)(byte);
		byte = read_buffer();
		NB = (NB<<8) + (long)(byte);
	}

	//EXH=0;
	if(s_fspec & 0x40)//Altitude
	{
		MK = 0;
		byte = read_buffer();
		if(!(byte & 0x80)) EXH=1;
		if(byte & 0x40) ZH=0;
		else ZH=1;
		
		H = (long)(byte & 0x3F);
		byte = read_buffer();
		H = (H<<8) + (long)(byte);
		if(H & 0x2000) H = H - (1<<14);
		//H=long((double(H))*1852.0*25/6080);
		H = long((double(H)) * 0.3048*25.0);

	}

	//OT=0;
	if(s_fspec & 0x20)//Fuel reserv
	{
		MK = 0;
		byte = read_buffer();
		CMyMath MM;
		OT = MM.BinOT_to_4bits(byte);			
	}

	if(s_fspec & 0x10)//Emergency
	{		
		byte = read_buffer();
		if(byte & 0x80) B = 0x01;
		else B = 0x00;
		
	}

	if(s_fspec & 0x08)//Time in storage
	{		
		BRF = read_buffer();		
	}
}

void CDecodeAsterix::decode_I002_010()
{
	byte = read_buffer();
	SAC = byte;
	byte = read_buffer();
	SIC = byte;
}

UCHAR CDecodeAsterix::decode_I002_000()
{
	unsigned char Message_Type;
	byte = read_buffer();
	Message_Type = byte;//1-North, 2-AM, 3-South, 
	
	if(Message_Type == 0x01){
		NorthMRT=::CMRTNorth(SAC, SIC, 0, i_comport, uc_LpdType);
	}

	if(Message_Type == 0x02){
		AmMRT=::CMRTAm(SAC, SIC, 0, i_comport, uc_LpdType, 0.0f);
	}

	return(Message_Type);
}

void CDecodeAsterix::decode_I002_020()
{
	unsigned char Sector_Number;
	float f_am;
	byte = read_buffer();
	Sector_Number = byte;
	f_am = (float)( 1.40625*((float)(Sector_Number)) );
	am = (long)(f_am);

	AmMRT.f_AM = f_am;
}

void CDecodeAsterix::decode_I001_161()
{
	int nTrace;
	nTrace = 0x00;
	byte = read_buffer();
	nTrace = WORD(byte);
	byte = read_buffer();
	nTrace = (nTrace<<8) + byte;
}

void CDecodeAsterix::decode_I001_042()
{
//	unsigned char sign;
/*						
	X=0;
	Y=0;
	byte=read_buffer();	
	if(byte & 0x80) sign=1;//-
	else sign=0;//+
	X=long(byte & 0x7F);
	X=X<<8;
	byte=read_buffer();
	X=X+byte;
	if(sign) X=-X;
	X=(X*1852)/64;

	byte=read_buffer();	
	if(byte & 0x80) sign=1;//-
	else sign=0;//+
	Y=long(byte & 0x7F);
	Y=Y<<8;
	byte=read_buffer();
	Y=Y+byte;
	if(sign) Y=-Y;
	Y=(Y*1852)/64;
*/
	__int16 ix, iy;

	ix = 0;
	ix = ix | read_buffer();
	ix = (ix<<8) | read_buffer();

	iy = 0;
	iy = iy | read_buffer();
	iy = (iy<<8) | read_buffer();

	X = ix;
	X = (X*1852)/64;

	Y = iy;
	Y = (Y*1852)/64;
}

void CDecodeAsterix::decode_I001_200()
{
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
}

void CDecodeAsterix::decode_I001_170()
{
	while((byte = read_buffer()) & 0x01)
	{
	}
}

void CDecodeAsterix::decode_I001_050()
{
	unsigned char a, b, c, d;
	a=b=c=d=0;
	byte=read_buffer();
					
	EXNB=0;
	if(!(byte & 0x80)) EXNB=1;
	if(byte & 0x08) a=a|0x04;
	if(byte & 0x04) a=a|0x02;
	if(byte & 0x02) a=a|0x01;
	if(byte & 0x01) b=b|0x04;
					
	byte=read_buffer();
	if(byte & 0x80) b=b|0x02;
	if(byte & 0x40) b=b|0x01;
	if(byte & 0x20) c=c|0x04;
	if(byte & 0x10) c=c|0x02;
	if(byte & 0x08) c=c|0x01;
	if(byte & 0x04) d=d|0x04;
	if(byte & 0x02) d=d|0x02;
	if(byte & 0x01) d=d|0x01;

	NB = 0;
	if(EXNB) NB = ((long)(a))*1000 + ((long)(b))*100 + ((long)(c))*10 + ((long)(d));
}

void CDecodeAsterix::decode_I001_060()
{
	byte = read_buffer();
	byte = read_buffer();
}
void CDecodeAsterix::decode_I001_210()
{
	while((byte = read_buffer()) & 0x01)
	{
	}
}

void CDecodeAsterix::decode_I001_141()
{
//	byte=read_buffer();
	__int16 i16;
	byte=read_buffer();
	i16=byte;
	i16=i16<<8;
	byte=read_buffer();
	i16=i16 | byte;

	double dt;
	dt=((double)(i16))/128;
	
	CMRTTime cur_time;
	cur_time.GetSysTime();
	time_stamp=cur_time;
	int i_sec, i_512;
	i_sec=time_stamp.m_time.wHour*3600 + time_stamp.m_time.wMinute*60 + time_stamp.m_time.wSecond;
	i_512=i_sec/512;
	
	double d_sec;
	d_sec=i_512*512;
	d_sec=d_sec+dt;

	__int64 i64_ms;
	i64_ms=__int64(d_sec*1000);

	time_stamp.m_time.wHour = 0;
	time_stamp.m_time.wMinute = 0;
	time_stamp.m_time.wSecond = 0;
	time_stamp.m_time.wMilliseconds = 0;

	time_stamp.AddMS(i64_ms);

	long l_dt;
	l_dt = (long)(cur_time.GetMS() - time_stamp.GetMS());
	if(l_dt > (256*1000)) time_stamp.AddMS(512*1000);
	if(l_dt < (-256*1000)) time_stamp.SubMS(512*1000);
}

void CDecodeAsterix::decode_I001_130()
{
	while((byte = read_buffer()) & 0x01)
	{
	}
}

void CDecodeAsterix::decode_I001_131()
{
	byte = read_buffer();
}

void CDecodeAsterix::decode_I001_150()
{
	byte = read_buffer();
}

void CDecodeAsterix::decode_I001_120()
{
	byte = read_buffer();
}

void CDecodeAsterix::decode_I001_080()
{
	byte = read_buffer();
	byte = read_buffer();
}

void CDecodeAsterix::decode_I001_100()
{
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
}

void CDecodeAsterix::decode_I002_030()
{
	long lt = 0;

	byte = read_buffer();
	lt |= byte;
	lt = lt<<8;

	byte = read_buffer();
	lt |= byte;
	lt = lt<<8;

	byte = read_buffer();
	lt |= byte;
	
	double dt;
	dt = (1.0/128)*lt;
	__int64 i_ms;
	i_ms = (__int64)(dt*1000);

	CMRTTime CurTime, TruncTime;
	CurTime.GetSysTime();
	TruncTime = CurTime;
	TruncTime.m_time.wHour = 0;
	TruncTime.m_time.wMinute = 0;
	TruncTime.m_time.wSecond = 0;
	TruncTime.m_time.wMilliseconds = 0;

	
	TruncTime.AddMS(i_ms);

	long l_dt;
	l_dt = (long)(CurTime.GetMS() - TruncTime.GetMS());
	if(l_dt > (12*3600*1000)) TruncTime.AddMS(24*3600*1000);
	if(l_dt < (-12*3600*1000)) TruncTime.SubMS(24*3600*1000);

	time_stamp = TruncTime;
}

void CDecodeAsterix::decode_I002_041()
{
	byte = read_buffer();
	byte = read_buffer();
}

void CDecodeAsterix::decode_I002_050()
{
	byte = read_buffer();

	//Это все в норме
	RlkSt.COMMON_On = 1;// Оборудование включено
	RlkSt.COMMON_St = 1;// РЛС в норме

	if (byte & 0x80) 
	{
		RlkSt.APOI1 = 0;
		RlkSt.APOI2 = 1;
	}
	else
	{
		RlkSt.APOI1 = 1;
		RlkSt.APOI2 = 0;
	}

	if (byte & 0x60) RlkSt.APOI1_St = 2;//не работает АПОИ1
	else RlkSt.APOI1_St = 1;//работает АПОИ1

	if (byte & 0x18) RlkSt.APOI2_St = 2;//не работает АПОИ2
	else RlkSt.APOI2_St = 1;//работает АПОИ2


//	if(byte & 0x01) byte=read_buffer();//FX
	while((byte = read_buffer()) & 0x01)//FX
	{
	}

}

void CDecodeAsterix::decode_I002_060()
{
	/*byte = read_buffer();
	if(byte & 0x01) byte=read_buffer();//FX*/
	while((byte = read_buffer()) & 0x01)//FX
	{
	}
}
void CDecodeAsterix::decode_I002_070()
{
	byte = read_buffer();
	int num_oct = byte;
	for (int k = 1; k < num_oct; k++)
	{
		byte = read_buffer();
		byte = read_buffer();
	}
}
void CDecodeAsterix::decode_I002_080()
{
	while((byte = read_buffer()) & 0x01)//FX
	{
	}
}
void CDecodeAsterix::decode_I002_090()
{
	byte = read_buffer();
	byte = read_buffer();
}
void CDecodeAsterix::decode_I002_100()
{
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
}
void CDecodeAsterix::decode_I002_SP()
{
	unsigned char s_len;
	s_len = read_buffer();
	if(s_len <= 1) return;
	for(int i = 2; i <= s_len; i++){
		byte=read_buffer();
	}
}

unsigned char CDecodeAsterix::read_buffer()
{
	unsigned char res;

	if((index < 0) || (index >= 4096))
		return 0;
	
	res = pbuf[index];
	index++;

	return res;
}

void CDecodeAsterix::decode_I062_010()
{
	byte = read_buffer();
	SAC = byte;
	TrackMRT.estm_Plot.SAC=byte;
		
	byte = read_buffer();
	SIC = byte;
	TrackMRT.estm_Plot.SIC = byte;
}

void CDecodeAsterix::decode_I062_015()	//Service Identification
{
	byte = read_buffer();		
}

void CDecodeAsterix::decode_I062_000()
{
	byte = read_buffer();
}

void CDecodeAsterix::decode_I062_040()
{
	int index = 0;

	byte = read_buffer();
	index = ((int)(byte)) << 8;
	byte = read_buffer();
	index = index | byte;

//	TrackMRT.estm_Plot.index = index;
	TrackMRT.track_num = index;
	TrackMRT.index = index;
}

void CDecodeAsterix::decode_I062_060()
{

	unsigned char a, b, c, d;

	a = b = c = d = 0;
	byte=read_buffer();			
		
	if(byte & 0x08) a=a|0x04;
	if(byte & 0x04) a=a|0x02;
	if(byte & 0x02) a=a|0x01;
	if(byte & 0x01) b=b|0x04;
					
	byte=read_buffer();
	if(byte & 0x80) b=b|0x02;
	if(byte & 0x40) b=b|0x01;
	if(byte & 0x20) c=c|0x04;
	if(byte & 0x10) c=c|0x02;
	if(byte & 0x08) c=c|0x01;
	if(byte & 0x04) d=d|0x04;
	if(byte & 0x02) d=d|0x02;
	if(byte & 0x01) d=d|0x01;

	EXNB = 1;
	MK = 1;
	NB = ((long)(a))*1000 + ((long)(b))*100 + ((long)(c))*10 + ((long)(d));

	TrackMRT.estm_Plot.MK = MK;
	TrackMRT.estm_Plot.EXNB = EXNB;
	TrackMRT.estm_Plot.number = NB;
    TrackMRT.estm_Plot.ValidNB = 1;

	switch(NB)
	{
		case 7700:
			TrackMRT.estm_Plot.b = 0x01;
			TrackMRT.m_ulFlagAlarm = 1;
			break;
		case 7600:
			TrackMRT.estm_Plot.b = 0x02;
			TrackMRT.m_ulFlagAlarm = 2;
			break;
		case 7500:
			TrackMRT.estm_Plot.b = 0x03;
			TrackMRT.m_ulFlagAlarm = 4;
			break;
	}
}

void CDecodeAsterix::decode_I062_070()
{
	long lt = 0;

	byte = read_buffer();
	lt |= byte;
	lt <<= 8;

	byte = read_buffer();
	lt |= byte;
	lt <<= 8;

	byte = read_buffer();
	lt |= byte;
	
	double dt;
	__int64 i_ms;

	dt = (1.0 / 128.0) * lt;
	i_ms = (__int64)(dt * 1000.0);

	CMRTTime CurTime, TruncTime;

	CurTime.GetSysTime();
	TruncTime = CurTime;
	TruncTime.m_time.wHour = 0;
	TruncTime.m_time.wMinute = 0;
	TruncTime.m_time.wSecond = 0;
	TruncTime.m_time.wMilliseconds = 0;

	
	TruncTime.AddMS(i_ms);

	long l_dt = (long)(CurTime.GetMS() - TruncTime.GetMS());

	if(l_dt > (12 * 3600 * 1000))
		TruncTime.AddMS(24 * 3600 * 1000);
	else if(l_dt < (-12 * 3600 * 1000))
		TruncTime.SubMS(24*3600*1000);

	TrackMRT.estm_Plot.time = TruncTime;
	TrackMRT.estm_Plot.time_stamp = TruncTime;
}

void CDecodeAsterix::decode_I062_080()
{
	IKI = 0x0F;
	TrackMRT.estm_Plot.IKI = IKI;

	BYTE sim = 0;

	//1
	byte = read_buffer();
	if(byte & 0x40) SPI = 1;
	else SPI = 0;
		
	TrackMRT.estm_Plot.SPI = SPI;
	if((byte & 0x01) == 0x00) return;

	//2
	byte = read_buffer();
	if(byte & 0x80) sim = 1;
	if(sim) FLAG = 0x02;
	else FLAG = 0x00;

	TrackMRT.estm_Plot.flag = FLAG;

	if(byte & 0x40) TrackMRT.b_TSE = TRUE;
	else TrackMRT.b_TSE = FALSE;

	if(byte & 0x20) TrackMRT.b_TSB = TRUE;
	else TrackMRT.b_TSB = FALSE;
		
	if((byte & 0x01) == 0x00) return;

	//3
	byte = read_buffer();
	if((byte & 0x01) == 0x00) return;

	//4
	byte=read_buffer();
	//IKI=0x00;
	//if(byte & 0x40) IKI=IKI | 0x01;
	//if(byte & 0x20) IKI=IKI | 0x02;
		
	if(byte & 0x40) IKI &= (~0x01);//Устарел ПРЛ
	if(byte & 0x20) IKI &= (~0x02);//Устарел ВРЛ
	if(byte & 0x10) IKI &= (~0x04);//Устарел ВРЛ Mode-S
	if(byte & 0x08) IKI &= (~0x08);//Устарел ADS-B

	TrackMRT.estm_Plot.IKI=IKI;		
}

void CDecodeAsterix::decode_I062_100()
{
	__int32 lx, ly;

	byte = read_buffer();
	lx = (((__int32)byte) << 24);

	byte = read_buffer();
	lx |= (((__int32)byte) << 16);

	byte = read_buffer();
	lx |= (((__int32)byte) << 8);

	lx /= 512;
	
	byte = read_buffer();
	ly = (((__int32)byte) << 24);

	byte = read_buffer();
	ly |= (((__int32)byte) << 16);

	byte = read_buffer();
	ly |= (((__int32)byte) << 8);

	ly /= 512;

	X = (long)lx;
	Y = (long)ly;

    TrackMRT.estm_Plot.x = X;
    TrackMRT.estm_Plot.y = Y;
}

void CDecodeAsterix::decode_I062_105()
{
	__int32 lB, lL;
	double dB, dL;

	lB=0;
	byte=read_buffer();
	lB=lB | ( ((__int32)byte)<<24 );

	byte=read_buffer();
	lB=lB | ( ((__int32)byte)<<16 );

	byte=read_buffer();
	lB=lB | ( ((__int32)byte)<<8 );

	byte=read_buffer();
	lB=lB | ( (__int32)byte );

	dB=lB;
	dB=dB*(180.0 / (1024.0 * 1024.0 * 32.0));

	lL=0;
	byte=read_buffer();
	lL=lL | ( ((__int32)byte)<<24 );

	byte=read_buffer();
	lL=lL | ( ((__int32)byte)<<16 );

	byte=read_buffer();
	lL=lL | ( ((__int32)byte)<<8 );

	byte=read_buffer();
	lL=lL | ( (__int32)byte );

	dL=lL;
	dL=dL*(180.0 / (1024.0 * 1024.0 * 32.0));

	TrackMRT.estm_Plot.SetBLH(dB, dL, 0.0);
}

void CDecodeAsterix::decode_I062_185()
{
	__int16 iVx, iVy;
	double dVx, dVy;
	CGeoid Gd;

	iVx = iVy = 0;

	byte=read_buffer();
	iVx=iVx | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	iVx=iVx | ( (__int16)byte );

	dVx=iVx;
	dVx=dVx*(0.25);

	byte=read_buffer();
	iVy=iVy | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	iVy=iVy | ( (__int16)byte );

	dVy=iVy;
	dVy=dVy*(0.25);

    if( TrackMRT.estm_Plot.OriginCoord == 2 )
    {
	    double dB, dL, dH;
	    TrackMRT.estm_Plot.GetBLH(dB, dL, dH);
	    Gd.SetBasePoint_Geoid(dB, dL, dH, 0.0);
    	
	    TrackMRT.estm_V=Gd.e_x0*dVx + Gd.e_y0*dVy;
	    TrackMRT.valid_V=TRUE;	
    }
    else
    {
        TrackMRT.Vx = dVx;
        TrackMRT.Vy = dVy;
    }
}

void CDecodeAsterix::decode_I062_210()
{
	__int16 iAx, iAy;
	double dAx, dAy;
	CGeoid Gd;

	iAx = iAy = 0;

	byte=read_buffer();
	iAx=byte;

	dAx=iAx;
	dAx=dAx*(0.25);

	byte=read_buffer();
	iAy=byte;

	dAy=iAy;
	dAy=dAy*(0.25);

    if( TrackMRT.estm_Plot.OriginCoord == 2 )
    {
	    double dB, dL, dH;
	    TrackMRT.estm_Plot.GetBLH(dB, dL, dH);
	    Gd.SetBasePoint_Geoid(dB, dL, dH, 0.0);
    	
	    TrackMRT.estm_A=Gd.e_x0*dAx + Gd.e_y0*dAy;
	    TrackMRT.valid_A=TRUE;	
    }
    else
    {
        TrackMRT.Ax = dAx;
        TrackMRT.Ay = dAy;
    }
}

void CDecodeAsterix::decode_I062_135()
{
	ZH = 1;	//RBS - absolute heigh
	EXH = 1;
	H = 0;
	byte = read_buffer();
	
	if(byte & 0x80)
	{
		TrackMRT.uc_H_corrected_Type = 2; // QNH correction applied
		ZH = 1;
	}
	else
	{
		TrackMRT.uc_H_corrected_Type = 0; // No QNH correction applied
		ZH = 0;
	}
					
	H = (long)(byte & 0x7F);
	byte = read_buffer();
	H = (H << 8) + byte;
	
	//H = (long)((H * 25.0 - 1500.0) * 0.3048);
	H = (long)(H * 25.0 * 0.3048);

	TrackMRT.estm_Plot.heigh = H;
	TrackMRT.estm_Plot.ZH = ZH;
	TrackMRT.estm_Plot.EXH = EXH;
    TrackMRT.estm_Plot.ValidH = 1;
}

void CDecodeAsterix::decode_I062_136()
{
	ZH = 1;	//RBS - absolute heigh
	EXH = 1;
	H = 0;
		
	__int16 iH;

	byte = read_buffer();
	iH = ((__int16)byte) << 8;
	byte = read_buffer();
	iH |= byte;
					
	//H = (long)((iH * 25.0 - 1500.0) * 0.3048);
	H = (long)(iH * 25.0 * 0.3048);

	TrackMRT.estm_Plot.ZH = ZH;
	TrackMRT.estm_Plot.EXH = EXH;
	TrackMRT.estm_Plot.heigh = H;
}

void CDecodeAsterix::decode_I062_200()
{
	byte = read_buffer();
}

void CDecodeAsterix::decode_I062_220()
{
	__int16 iV;

	byte = read_buffer();
	iV = ((__int16)(byte)) << 8;
	byte = read_buffer();
	iV = iV | byte;

	TrackMRT.v_vel = ((double)iV) * 0.3048 * 6.25;
}

void CDecodeAsterix::decode_I062_380()
{
	UCHAR SubField1, ADR, ID;
		
	//1
	SubField1=read_buffer();
	if(SubField1 & 0x80) ADR=1;
	else ADR=0;
	if(SubField1 & 0x40) ID=1;
	else ID=0;

	if(ADR)
	{
		long l1, l2, l3;
		l1=read_buffer();
		l2=read_buffer();
		l3=read_buffer();
		TrackMRT.estm_Plot.Address_ICAO=(l1<<16) + (l2<<8) + l3;
		CMyMath MM;
		TrackMRT.estm_Plot.State=MM.ICAOCodeToState(TrackMRT.estm_Plot.Address_ICAO);
	}
	if(ID)
	{
		char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8;
		unsigned char b;
		CString str;

		byte=read_buffer();
		ch1=((byte & 0xFC)>>2);
		b=(byte & 0x03);
			
		byte=read_buffer();
		ch2=(b<<4)+((byte & 0xF0)>>4);
		b=(byte & 0x0F);

		byte=read_buffer();
		ch3=(b<<2)+((byte & 0xC0)>>6);
		ch4=(byte & 0x3F);
			
		byte=read_buffer();
		ch5=((byte & 0xFC)>>2);
		b=(byte & 0x03);

		byte=read_buffer();
		ch6=(b<<4)+((byte & 0xF0)>>4);
		b=(byte & 0x0F);
			
		byte=read_buffer();
		ch7=(b<<2)+((byte & 0xC0)>>6);
		ch8=(byte & 0x3F);

		CMyMath MM;
		str.Empty();
		str += MM.ICAOChar_to_Char(ch1);
		str += MM.ICAOChar_to_Char(ch2);
		str += MM.ICAOChar_to_Char(ch3);
		str += MM.ICAOChar_to_Char(ch4);
		str += MM.ICAOChar_to_Char(ch5);
		str += MM.ICAOChar_to_Char(ch6);
		str += MM.ICAOChar_to_Char(ch7);
		str += MM.ICAOChar_to_Char(ch8);

		TrackMRT.estm_Plot.Ident = str;
	}		
}

void CDecodeAsterix::decode_I062_390()	//Flight Plan Related Data
{
	int i;

	TrackMRT.m_Fpl.Clear();

	//Primary Subfield
	BOOL TAG,  CSN,  IFI,  FCT,  TAC,  WTC,  DEP,  DST,  RDS,  CFL,  CTL,  TOD,  AST,  STS,  STD,  STA,  PEM,  PEC;
		 TAG = CSN = IFI = FCT = TAC = WTC = DEP = DST = RDS = CFL = CTL = TOD = AST = STS = STD = STA = PEM = PEC = FALSE;
	
	byte = read_buffer();
	if(byte & 0x80) TAG = TRUE;
	if(byte & 0x40) CSN = TRUE;
	if(byte & 0x20) IFI = TRUE;
	if(byte & 0x10) FCT = TRUE;
	if(byte & 0x08) TAC = TRUE;
	if(byte & 0x04) WTC = TRUE;
	if(byte & 0x02) DEP = TRUE;

	if(byte & 0x01)//FX
	{
		byte = read_buffer();
		if(byte & 0x80) DST = TRUE;
		if(byte & 0x40) RDS = TRUE;
		if(byte & 0x20) CFL = TRUE;
		if(byte & 0x10) CTL = TRUE;
		if(byte & 0x08) TOD = TRUE;
		if(byte & 0x04) AST = TRUE;
		if(byte & 0x02) STS = TRUE;

		if(byte & 0x01)//FX
		{
			byte = read_buffer();
			if(byte & 0x80) STD = TRUE;
			if(byte & 0x40) STA = TRUE;
			if(byte & 0x20) PEM = TRUE;
			if(byte & 0x10) PEC = TRUE;
		}
	}

	//# 1
	if(TAG)
	{
		byte = read_buffer();//SAC
		byte = read_buffer();//SIC
	}

	//#2 - Callsign
	if(CSN)
	{
		TrackMRT.m_Fpl.m_strFlight.Empty();
		for(i=0; i<7; i++)
		{
			byte = read_buffer();
			TrackMRT.m_Fpl.m_strFlight += byte;
		}
	}

	//#3 - IFPS_FLIGHT_ID
	if(IFI)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}

	//#4 - Flight Category
	if(FCT)
	{
		byte = read_buffer();
		UCHAR FR1_FR2;
		FR1_FR2 = (byte & 0x30)>>4;
		if(FR1_FR2 == 0) TrackMRT.m_Fpl.m_ucPppPvp=1;//PPP
		if(FR1_FR2 == 1) TrackMRT.m_Fpl.m_ucPppPvp=2;//PVP

		TrackMRT.m_Fpl.m_ucRVSM = (byte & 0x0C)>>2;//RVSM
	}

	//#5 - Type of Aircraft
	if(TAC)
	{
		TrackMRT.m_Fpl.m_strTypeFV.Empty();
		for(i=0; i<4; i++)
		{
			byte = read_buffer();
			TrackMRT.m_Fpl.m_strTypeFV += byte;
		}
	}

	//#6 - Wake Turbulence Category
	if(WTC)
	{
		byte = read_buffer();
		TrackMRT.m_Fpl.m_ucWeight = 0;
		if(byte == 'H') TrackMRT.m_Fpl.m_ucWeight = 1; 
		if(byte == 'M') TrackMRT.m_Fpl.m_ucWeight = 2;
		if(byte == 'L') TrackMRT.m_Fpl.m_ucWeight = 3;
	}

	//#7 - Departure Airport
	if(DEP)
	{
		TrackMRT.m_Fpl.m_strAPSrc.Empty();
		for(i=0; i<4; i++)
		{
			byte = read_buffer();
			TrackMRT.m_Fpl.m_strAPSrc += byte;
		}
	}

	//#8 - Destination Airport
	if(DST)
	{
		TrackMRT.m_Fpl.m_strAPDest.Empty();
		for(i=0; i<4; i++)
		{
			byte = read_buffer();
			TrackMRT.m_Fpl.m_strAPDest += byte;
		}
	}

	//#9 - Runway Designation
	if(RDS)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}

	//#10 - Current Cleared Flight Level
	if(CFL)
	{
		long l_H = 0;
		__int16 iH;

		byte = read_buffer();
		iH = ((__int16)(byte))<<8;
		byte = read_buffer();
		iH = iH | byte;

		l_H = (long)(((double)(iH))*0.3048*25);
		if(l_H < 0) l_H = -1;
	
		TrackMRT.m_Fpl.m_lGFL=l_H;
		TrackMRT.l_given_H=l_H;
	}

	//#11 - Current Control Position
	if(CTL)
	{
		byte = read_buffer();
		byte = read_buffer();
	}

	//#12 - Time of Departure / Arrival
	if(TOD)
	{
		UCHAR REP, TYP, DAY, HOR, MIN, SEC;
		BOOL AVS;

		REP = TYP = DAY = HOR = MIN = SEC = 0;
		AVS = FALSE;

		//1
		byte = read_buffer();
		REP = byte;

		do
		{
			//2
			byte = read_buffer();
			TYP = (byte & 0xF8)>>3;
			DAY = (byte & 0x06)>>1;

			//3
			byte = read_buffer();
			HOR = byte & 0x1F;

			//4
			byte = read_buffer();
			MIN = byte & 0x3F;

			//5
			byte = read_buffer();
			SEC = byte & 0x1F;
			AVS = (byte & 0x80)>>7;
			if(AVS == 0) SEC = 0;

			//Rep
			TrackMRT.m_Fpl.m_ucSNGMX = 0; 
			/*if(REP=='S') TrackMRT.m_Fpl.m_ucSNGMX=1; 
			if(REP=='N') TrackMRT.m_Fpl.m_ucSNGMX=2;
			if(REP=='G') TrackMRT.m_Fpl.m_ucSNGMX=3; 
			if(REP=='M') TrackMRT.m_Fpl.m_ucSNGMX=4; 
			if(REP=='X') TrackMRT.m_Fpl.m_ucSNGMX=5; 
			*/

			if(TYP == 8)
			{
				TrackMRT.m_Fpl.m_cgpPointIn.m_ctTime.ZeroTime();//=CTime(1971,1,1,HOR,MIN,SEC);
				TrackMRT.m_Fpl.m_cgpPointIn.m_ctTime.m_time.wHour=HOR;
				TrackMRT.m_Fpl.m_cgpPointIn.m_ctTime.m_time.wMinute=MIN;
				TrackMRT.m_Fpl.m_cgpPointIn.m_ctTime.m_time.wSecond=SEC;
			}

			if (REP > 0) REP--;

		}while (REP > 0);
	}

	//#13 - Aircraft Stand
	if (AST)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}

	//#14 - Stand Status
	if (STS)
	{
		byte = read_buffer();
	}

	//#15 - Standard Instrument Departure
	if (STD)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}

	//#16 - Standard Instrument Arrival
	if (STA)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}

	//#17 - Pre-Emergency Mode 3/A
	if (PEM)
	{
		unsigned char a, b, c, d;
		a=b=c=d=0;
		byte=read_buffer();
					
		UCHAR EXNB_fpl=0;
		long NB_fpl;
		if(!(byte & 0x10)) EXNB_fpl=1;
		if(byte & 0x08) a=a|0x04;
		if(byte & 0x04) a=a|0x02;
		if(byte & 0x02) a=a|0x01;
		if(byte & 0x01) b=b|0x04;
					
		byte=read_buffer();
		if(byte & 0x80) b=b|0x02;
		if(byte & 0x40) b=b|0x01;
		if(byte & 0x20) c=c|0x04;
		if(byte & 0x10) c=c|0x02;
		if(byte & 0x08) c=c|0x01;
		if(byte & 0x04) d=d|0x04;
		if(byte & 0x02) d=d|0x02;
		if(byte & 0x01) d=d|0x01;

		long NB = -1;
		if(EXNB_fpl) NB_fpl = ((long)(a))*1000 + ((long)(b))*100 + ((long)(c))*10 + ((long)(d));

		TrackMRT.m_Fpl.m_lTransponder = NB_fpl;
	}

	//#18 - Pre-Emergency Callsign
	if (PEC)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}
}

void CDecodeAsterix::decode_I062_SP()
{
	//RADAR PLOT SPECIAL DATA ITEM
	unsigned char s_len;

	byte = read_buffer();
	s_len = byte;
	if(s_len == 0x00) return;
#ifdef glb_ASC
    for( int i = 0; i < s_len; i++ )
        byte = read_buffer();
#else
	unsigned char s_fspec, s_fspec_1;
	byte = read_buffer();

	s_fspec = byte;
	if(s_fspec & 0x01)
		s_fspec_1 = read_buffer();
	else
		s_fspec_1 = 0;

	//EXNB=0;
	if(s_fspec & 0x80)	//Board Number
	{
		EXNB = 1;
		byte = read_buffer();
		NB = (long)(byte);
		byte = read_buffer();
		NB = (NB << 8) + (long)byte;
		byte = read_buffer();
		NB = (NB << 8)+(long)byte;

		TrackMRT.estm_Plot.EXNB = EXNB;
		TrackMRT.estm_Plot.number = NB;
		TrackMRT.estm_Plot.MK = 0;
	}

	//EXH=0;
	if(s_fspec & 0x40)//Altitude
	{		
		byte = read_buffer();
		if(!(byte & 0x80)) EXH = 1;

		if(byte & 0x40) ZH = 0;
		else ZH = 1;
		
		H = (long)(byte & 0x3F);
		byte = read_buffer();
		H = (H << 8) + (long)byte;
		
		//H = long((H * 25.0 - 1500.0) * 0.3048);
		H = long(H * 25.0 * 0.3048);

		TrackMRT.estm_Plot.ZH = ZH;
		TrackMRT.estm_Plot.EXH = EXH;
		TrackMRT.estm_Plot.heigh = H;
		TrackMRT.estm_Plot.MK = 0;
	}

	//OT=0;
	if(s_fspec & 0x20)	//Fuel reserv
	{
		byte = read_buffer();
		OT = OT | byte;

		TrackMRT.estm_Plot.ot = OT;
		TrackMRT.estm_Plot.MK = 0;
	}

	if(s_fspec & 0x10)	//Emergency
	{
		byte = read_buffer();
		if(byte & 0x80) B = 0x01;
		else B = 0x00;

		TrackMRT.estm_Plot.b = B;
		TrackMRT.m_ulFlagAlarm = 1;
	}

	if(s_fspec & 0x08)	//Time in storage
	{
		BRF = byte = read_buffer();
		TrackMRT.estm_Plot.BRF = BRF;		
	}

	if(s_fspec & 0x04)	//FPL status
	{
		UCHAR status;
		status = read_buffer();

		TrackMRT.m_Fpl.m_ucPlanState = status;
		TrackMRT.m_nFplState = 1;
	}

	if(s_fspec & 0x02)	//Not Valid NB or H
	{
		UCHAR status;
		status = read_buffer();

		if(status & 0x01) TrackMRT.estm_Plot.ValidNB = 0;
		if(status & 0x02) TrackMRT.estm_Plot.ValidH = 0;
	}

	if(s_fspec_1 & 0x80)	//QNH corrected height
	{
		byte = read_buffer();
		TrackMRT.uc_H_corrected_Type = byte;

		byte = read_buffer();
		
		__int16 iH = ((__int16)(byte)) << 8;
		
		byte = read_buffer();
		iH = iH | byte;
					
		long lH = (long)(H * 0.3048 * 25.0);
		//long lH = long((H * 25.0 - 1500.0) * 0.3048);

		TrackMRT.l_H_corrected = lH;
		if(lH > -10000)
			TrackMRT.estm_Plot.heigh = lH;
	}
#endif
}
void CDecodeAsterix::decode_I062_245(void)
{
    byte = read_buffer();
    byte = read_buffer();
    byte = read_buffer();
    byte = read_buffer();
    byte = read_buffer();
    byte = read_buffer();
    byte = read_buffer();
}
void CDecodeAsterix::decode_I062_290(void)
{
    bool TRK, PSR, SSR, MDS, ADS, ES, VDL, UAT, LOP, MLT;
    TRK = PSR = SSR = MDS = ADS = ES = VDL = UAT = LOP = MLT = false;
    byte = read_buffer();
    if( byte & 0x80 ) TRK = true;
    if( byte & 0x40 ) PSR = true;
	if( byte & 0x20 ) SSR = true;
	if( byte & 0x10 ) MDS = true;
	if( byte & 0x08 ) ADS = true;
	if( byte & 0x04 ) ES  = true;
	if( byte & 0x02 ) VDL = true;

    if( byte & 0x01 ) 
	{
		byte = read_buffer();
		if( byte & 0x80 ) UAT = true;
		if( byte & 0x40 ) LOP = true;
		if( byte & 0x20 ) MLT = true;
	}
    
    if( TRK )  read_buffer();
    if( PSR )  read_buffer();
	if( SSR )  read_buffer();
	if( MDS )  read_buffer();
    if( ADS )  
    {
        read_buffer();
        read_buffer();
    }
	if( ES  )  read_buffer();
	if( VDL )  read_buffer();
    if( UAT )  read_buffer();
	if( LOP )  read_buffer();
	if( MLT )  read_buffer();
}
void CDecodeAsterix::decode_I062_295(void)
{
    bool MFL, MD1, MD2, MDA, MD4, MD5, MHG, IAS, TAS, SAL, FSS, TID, COM, SAB, ACS, BVR, GVR, RAN;
    bool TAR, TAN, GSP, VUN, MET, EMC, POS, GAL, PUN, MB1, IAR, MAC, MCP, FMS, BPS, TAL, MFM;

    MFL = MD1 = MD2 = MDA = MD4 = MD5 = MHG = IAS = TAS = SAL = FSS = TID = COM = SAB = 
        ACS = BVR = GVR = RAN = TAR = TAN = GSP = VUN = MET = EMC = POS = GAL = PUN =
        MB1 = IAR = MAC = MCP = FMS = BPS = TAL = MFM = false;

    byte = read_buffer();
    if( byte & 0x80 ) MFL = true;
    if( byte & 0x40 ) MD1 = true;
	if( byte & 0x20 ) MD2 = true;
	if( byte & 0x10 ) MDA = true;
	if( byte & 0x08 ) MD4 = true;
	if( byte & 0x04 ) MD5 = true;
	if( byte & 0x02 ) MHG = true;

    if( byte & 0x01 ) 
	{
		byte = read_buffer();
		if( byte & 0x80 ) IAS = true;
		if( byte & 0x40 ) TAS = true;
		if( byte & 0x20 ) SAL = true;
        if( byte & 0x10 ) FSS = true;
	    if( byte & 0x08 ) TID = true;
	    if( byte & 0x04 ) COM = true;
	    if( byte & 0x02 ) SAB = true;
        
        if( byte & 0x01 ) 
	    {
		    byte = read_buffer();
		    if( byte & 0x80 ) ACS = true;
		    if( byte & 0x40 ) BVR = true;
		    if( byte & 0x20 ) GVR = true;
            if( byte & 0x10 ) RAN = true;
	        if( byte & 0x08 ) TAR = true;
	        if( byte & 0x04 ) TAN = true;
	        if( byte & 0x02 ) GSP = true;
            
            if( byte & 0x01 ) 
	        {
		        byte = read_buffer();
		        if( byte & 0x80 ) VUN = true;
		        if( byte & 0x40 ) MET = true;
		        if( byte & 0x20 ) EMC = true;
                if( byte & 0x10 ) POS = true;
	            if( byte & 0x08 ) GAL = true;
	            if( byte & 0x04 ) PUN = true;
	            if( byte & 0x02 ) MB1  = true;
            
                if( byte & 0x01 ) 
	            {
		            byte = read_buffer();
		            if( byte & 0x80 ) IAR = true;
		            if( byte & 0x40 ) MAC = true;
		            if( byte & 0x20 ) MCP = true;
                    if( byte & 0x10 ) FMS = true;
	                if( byte & 0x08 ) BPS = true;
	                if( byte & 0x04 ) TAL = true;
	                if( byte & 0x02 ) MFM = true;
	            }
	        }
	    }
	}
    
    if( MFL )  read_buffer();
    if( MD1 )  read_buffer();
	if( MD2 )  read_buffer();
	if( MDA )  read_buffer();
	if( MD4 )  read_buffer();
	if( MD5 )  read_buffer();
    if( MHG )  read_buffer();
	if( IAS )  read_buffer();
	if( TAS )  read_buffer();
    if( SAL )  read_buffer();
    if( FSS )  read_buffer();
	if( TID )  read_buffer();
	if( COM )  read_buffer();
	if( SAB )  read_buffer();
	if( ACS )  read_buffer();
    if( BVR )  read_buffer();
	if( GVR )  read_buffer();
	if( RAN )  read_buffer();
    if( TAR )  read_buffer();
    if( TAN )  read_buffer();
	if( GSP )  read_buffer();
	if( VUN )  read_buffer();
	if( MET )  read_buffer();
	if( EMC )  read_buffer();
    if( POS )  read_buffer();
	if( GAL )  read_buffer();
	if( PUN )  read_buffer();
	if( MB1 )  read_buffer();
	if( IAR )  read_buffer();
	if( MAC )  read_buffer();
    if( MCP )  read_buffer();
	if( FMS )  read_buffer();
	if( BPS )  read_buffer();
	if( TAL )  read_buffer();
	if( MFM )  read_buffer();
}
void CDecodeAsterix::decode_I062_130(void)
{
    byte = read_buffer();
    byte = read_buffer();
}
void CDecodeAsterix::decode_I062_340(void)
{
    bool SID, POS, HEI, MDC, MDA, TYP;
    SID = POS = HEI = MDC = MDA = TYP = false;
    
    byte = read_buffer();
    if( byte & 0x80 ) SID = true;
    if( byte & 0x40 ) POS = true;
	if( byte & 0x20 ) HEI = true;
	if( byte & 0x10 ) MDC = true;
	if( byte & 0x08 ) MDA = true;
	if( byte & 0x04 ) TYP = true;

    if( SID )
    {
        read_buffer();
        read_buffer();
    }
    if( POS )
    {
        read_buffer();
        read_buffer();
        read_buffer();
        read_buffer();
    }
	if( HEI )    
    {
        read_buffer();
        read_buffer();
    }

	if( MDC )  
    {
        read_buffer();
        read_buffer();
    }
	if( MDA )
    {
        read_buffer();
        read_buffer();
    }
	if( TYP )  read_buffer();
}

void CDecodeAsterix::decode_I241_010(void)
{
	byte = read_buffer();
	SAC = byte;
	byte = read_buffer();
	SIC = byte;
}

UCHAR CDecodeAsterix::decode_I241_000(void)
{
	unsigned char Message_Type;
	byte = read_buffer();
	Message_Type = byte; 
	return(Message_Type);
}
void CDecodeAsterix::decode_I241_600(void)
{
	MsgNumber = 0;
	byte = read_buffer();
	MsgNumber = MsgNumber | ( ((__int16)byte)<<8 );

	byte = read_buffer();
	MsgNumber = MsgNumber | ( (__int16)byte );
}

void CDecodeAsterix::decode_I241_030(void)
{
	long lt;
	lt=0;
	byte=read_buffer();
	lt|=byte;
	lt=lt<<8;
	byte=read_buffer();
	lt|=byte;
	lt=lt<<8;
	byte=read_buffer();
	lt|=byte;

	double dt;
	dt=(1.0/128)*lt;
	__int64 i_ms;
	i_ms=(__int64)(dt*1000);

	CMRTTime CurTime, TruncTime;
	CurTime.GetSysTime();
	TruncTime=CurTime;
	TruncTime.m_time.wHour=0;
	TruncTime.m_time.wMinute=0;
	TruncTime.m_time.wSecond=0;
	TruncTime.m_time.wMilliseconds=0;

	TruncTime.AddMS(i_ms);

	long l_dt;
	l_dt=(long)(CurTime.GetMS()-TruncTime.GetMS());
	if(l_dt>(12*3600*1000)) TruncTime.AddMS(24*3600*1000);
	if(l_dt<(-12*3600*1000)) TruncTime.SubMS(24*3600*1000);

	TimeGet=TruncTime;
}

void CDecodeAsterix::decode_I241_601(void)
{
	byte=read_buffer();
	int hour;
	hour = (int)byte;
	TimeStop=TimeGet;
	hour=hour*3600*1000;
	TimeStop.AddMS(hour);
}

void CDecodeAsterix::decode_I241_602(void)
{
	byte=read_buffer();
	MeteoPolygon.m_ContNumber = (int)byte;
	byte=read_buffer();
	MeteoPolygon.m_codeEvent = (int)byte;
	__int32 i32=0;
	__int16 i16=0;
	byte=read_buffer();
	i32=i32 | ( ((__int32)byte)<<24 );
	byte=read_buffer();
	i32=i32 | ( ((__int32)byte)<<16 );
	byte=read_buffer();
	i32=i32 | ( ((__int32)byte)<<8 );
	byte=read_buffer();
	i32=i32 | ( (__int32)byte );
	i16=(__int16)(i32>>22) & 0x3FF;
	MeteoPolygon.m_HRadioecho = (long)i16;
	i16=(__int16)(i32>>12) & 0x3FF;
	MeteoPolygon.m_VelRadioecho = (int)i16;
	i16=(__int16)(i32>>3) & 0x1FF;
	//MeteoPolygon.m_VelRadioecho = (int)i16;
	MeteoPolygon.m_CourseRadioecho = (int)i16;
}

void CDecodeAsterix::decode_I241_105(void)
{
	__int32 lB, lL;
	double dB, dL;

	lB=0;
	byte=read_buffer();
	lB=lB | ( ((__int32)byte)<<24 );

	byte=read_buffer();
	lB=lB | ( ((__int32)byte)<<16 );

	byte=read_buffer();
	lB=lB | ( ((__int32)byte)<<8 );

	byte=read_buffer();
	lB=lB | ( (__int32)byte );

	dB=lB;
	dB=dB*(180.0/(1024*1024*32));

	lL=0;
	byte=read_buffer();
	lL=lL | ( ((__int32)byte)<<24 );

	byte=read_buffer();
	lL=lL | ( ((__int32)byte)<<16 );

	byte=read_buffer();
	lL=lL | ( ((__int32)byte)<<8 );

	byte=read_buffer();
	lL=lL | ( (__int32)byte );

	dL=lL;
	dL=dL*(180.0/(1024*1024*32));

	CGeo_point gp;
	gp.B=dB;
	gp.L=dL;
	gp.H=MeteoPolygon.m_HRadioecho;
	gp.MakeXYZ_from_BLH();
	MeteoPolygon.AddPointToEnd(gp);
}

void CDecodeAsterix::decode_I241_603(void)
{
	byte=read_buffer();
	MeteoRW.m_cICAO_Identifier[0]=byte;
	byte=read_buffer();
	MeteoRW.m_cICAO_Identifier[1]=byte;
	byte=read_buffer();
	MeteoRW.m_cICAO_Identifier[2]=byte;
	byte=read_buffer();
	MeteoRW.m_cICAO_Identifier[3]=byte;
}

void CDecodeAsterix::decode_I241_604(void)
{
	byte=read_buffer();
	MeteoRW.m_strRWName="RW";
	UCHAR b;
	int a1, a2;
	b=(byte>>2)&0x3F;
	a1=(int)b;
	a2=a1/10;
	a1=a1-a2*10;
	b=UCHAR(a2) + 0x30;
	MeteoRW.m_strRWName.AppendChar(b);
	b=UCHAR(a1) + 0x30;
	MeteoRW.m_strRWName.AppendChar(b);
	if((byte & 0x03)==0x03)
		MeteoRW.m_strRWName.AppendChar('M');
	if((byte & 0x03)==0x02)
		MeteoRW.m_strRWName.AppendChar('R');
	if((byte & 0x03)==0x01)
		MeteoRW.m_strRWName.AppendChar('L');
}

void CDecodeAsterix::decode_I241_605(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_WindCourse=(double)i16;
}

void CDecodeAsterix::decode_I241_606(void)
{
	byte=read_buffer();

	MeteoRW.m_WindEstm=(double)byte;
}

void CDecodeAsterix::decode_I241_607(void)
{
	byte=read_buffer();

	MeteoRW.m_WindMax=(double)byte;
}

void CDecodeAsterix::decode_I241_608(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_WindSide=((double)i16)/10.0;
}

void CDecodeAsterix::decode_I241_609(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_Temp=((double)i16)/10.0;
}

void CDecodeAsterix::decode_I241_610(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_DewPoint=((double)i16)/10.0;
}

void CDecodeAsterix::decode_I241_611(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_Vis=(long)i16;
}

void CDecodeAsterix::decode_I241_612(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_DistanceVisBeg=(long)i16;
}

void CDecodeAsterix::decode_I241_613(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_DistanceVisMdl=(long)i16;
}

void CDecodeAsterix::decode_I241_614(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_DistanceVisEnd=(long)i16;
}

void CDecodeAsterix::decode_I241_615(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_HCloud=(long)i16;
}

void CDecodeAsterix::decode_I241_616(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_QFE=(double)i16;
}

void CDecodeAsterix::decode_I241_617(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_QFEh=(double)i16;
}

void CDecodeAsterix::decode_I241_618(void)
{
	__int16 i16;

	i16=0;
	byte=read_buffer();
	i16=i16 | ( ((__int16)byte)<<8 );

	byte=read_buffer();
	i16=i16 | ( (__int16)byte );

	MeteoRW.m_QNHh=(double)i16;
}

void CDecodeAsterix::decode_I242_010()
{
	byte=read_buffer();
	SAC=byte;
	byte=read_buffer();
	SIC=byte;
}

CMRTTime CDecodeAsterix::decode_I242_020()
{
	int i;
	__int64 i64;
	i64=0;
	for(i=0; i<8; i++){
		byte=read_buffer();
		i64=i64<<8;
		i64=i64 | byte;
	}
	CMRTTime Time;
	union unTIME
	{
		FILETIME m_ft;     //  Время в формате FILETIME
		__int64 i64;       //  Оно же в формате __int64
	}unT;

	unT.i64=i64;
	FileTimeToSystemTime(&unT.m_ft, (SYSTEMTIME*)&Time.m_time);

	return Time;
}

void CDecodeAsterix::decode_I008_010(void)
{
	byte = read_buffer();
	SAC = byte;
	byte = read_buffer();
	SIC = byte;
}

UCHAR CDecodeAsterix::decode_I008_000(void)
{
	unsigned char Message_Type;
	byte=read_buffer();
	Message_Type=byte; 
	return(Message_Type);
}
void CDecodeAsterix::decode_I008_020(void)
{
	byte=read_buffer();
	if(byte&0x80) org1=true;
	else org1=false;

	Intensity=byte;
	Intensity=((Intensity>>4)&0x07);
	
	byte=((byte>>1)&0x07);
	North=float(byte*22.5);
}
int CDecodeAsterix::decode_I008_036(void)
{
	//MeteoVector.Clear();
	double x50, y50, length;
	CMyMath MM;
	f=6;
	byte=read_buffer();
	int REP;
	REP=byte;  // количество векторов

	CGeo_point gp;
	double cmr=0;
	cmr=pow(2.0,f-6);//цена младшего разряда

	for(int i=0; i<REP; i++)
	{
		if (i>=N_METEO_VECTOR)  
		{
			byte=read_buffer();
			byte=read_buffer();
			byte=read_buffer();
			return (i);
		}

		byte=read_buffer();
		x50=(double)(char(byte));
 		MeteoVector.metV[i].x1=x50;

		byte=read_buffer();
		y50=(double)(char(byte));
 		MeteoVector.metV[i].y1=y50;
		
		byte=read_buffer();
		length=(double)(char(byte));


		gp.B=x50*cmr*1852;	// NM
		gp.L=y50*cmr*1852;	// NM
		gp.H=0;
		gp.MakeXYZ_from_BLH();
 		MeteoVector.metV[i].m_gp0=gp;

		x50=x50 + length*MM.sin_gr(North);
		y50=y50 + length*MM.cos_gr(North);
		
		MeteoVector.metV[i].x2=x50;
 		MeteoVector.metV[i].y2=y50;

		gp.B=x50*cmr*1852;	// NM
		gp.L=y50*cmr*1852;	// NM
		gp.H=0;
		gp.MakeXYZ_from_BLH();
 		MeteoVector.metV[i].m_gp1=gp;
	}
	return REP;
}

int CDecodeAsterix::decode_I008_038(void)
{
	double x50, y50;
	CMyMath MM;
	//f=7;
	byte=read_buffer();
	int REP;
	REP=byte;  // количество векторов

	CGeo_point gp;
	double cmr=0;
	cmr=pow(2.0,f-6);//цена младшего разряда

	for(int i=0; i<REP; i++)
	{
		if (i>=N_METEO_VECTOR)  
		{
			byte=read_buffer();
			byte=read_buffer();
			byte=read_buffer();
			byte=read_buffer();
			return (i);
		}

		byte=read_buffer();
		x50=(double)(char(byte));
		MeteoVector.metV[i].x1=x50;

		byte=read_buffer();
		y50=(double)(char(byte));
		MeteoVector.metV[i].y1=y50;
		
		gp.B=x50*cmr*1852;	// NM
		gp.L=y50*cmr*1852;	// NM
		gp.H=0;
		gp.MakeXYZ_from_BLH();
		MeteoVector.metV[i].m_gp0=gp;

		byte=read_buffer();
		x50=(double)(char(byte));
 		MeteoVector.metV[i].x2=x50;

		byte=read_buffer();
		y50=(double)(char(byte));
	 	MeteoVector.metV[i].y2=y50;

		gp.B=x50*cmr*1852;	// NM
		gp.L=y50*cmr*1852;	// NM
		gp.H=0;
		gp.MakeXYZ_from_BLH();
	 	MeteoVector.metV[i].m_gp1=gp;
	}
	return REP;
}

UCHAR CDecodeAsterix::decode_I008_040(int &I, int &num)
{
	unsigned char ORG, FST;

	byte=read_buffer();

	ORG=(byte & 0x80)>>7;

	if(ORG==0x00){ // Local coord.
	}
	else if(ORG==0x01){ // System coord.
	}

	//int Ii, numi;
	I =int((byte & 0x70)>>4); // 0-7 всякие явления

	FST=(byte & 0x03);	// 0-3 записа контура

	byte=read_buffer();

	num=int(byte); // порядковый номер записи контура

	//I=Ii;
	//num=numi;

	//if((FST==3)||(FST==2))return TRUE;
	//else return FALSE;
	return FST;
}

void CDecodeAsterix::decode_I008_050()
{
	double x50, y50;
	byte=read_buffer();
	int REP;
	REP=byte;  // количество точек контура

	CGeo_point gp;
	double cmr=0;
	cmr=pow(2.0,f-6);//цена младшего разряда

	for(int i=0; i<REP; i++)
	{
		byte=read_buffer();
		x50=(double)(char(byte));

		byte=read_buffer();
		y50=(double)(char(byte));

		gp.B=x50*cmr*1852;	// NM
		gp.L=y50*cmr*1852;	// NM
		gp.H=0;
		gp.MakeXYZ_from_BLH();
		MeteoPolygon.AddPointToEnd(gp);
	}
}

void CDecodeAsterix::decode_I008_090(void)
{
	long lt;
	lt=0;
	byte=read_buffer();
	lt|=byte;
	lt=lt<<8;
	byte=read_buffer();
	lt|=byte;
	lt=lt<<8;
	byte=read_buffer();
	lt|=byte;

	double dt;
	dt=(1.0/128)*lt;
	__int64 i_ms;
	i_ms=(__int64)(dt*1000);

	CMRTTime CurTime;
	CurTime.GetSysTime();
	CurTime.m_time.wHour=0;
	CurTime.m_time.wMinute=0;
	CurTime.m_time.wSecond=0;
	CurTime.m_time.wMilliseconds=0;

	CurTime.AddMS(i_ms);

	//long l_dt;
	//l_dt=(long)(CurTime.GetMS()-TruncTime.GetMS());
	//if(l_dt>(12*3600*1000)) TruncTime.AddMS(24*3600*1000);
	//if(l_dt<(-12*3600*1000)) TruncTime.SubMS(24*3600*1000);

	CurTime.GetSysTime();

	TimeGet=CurTime;
}

int CDecodeAsterix::decode_I008_100(void)
{
	unsigned char R;
	__int16 Q; 
	int f;


	byte = read_buffer();

	f=(char)(byte & 0xF8);
	f=f/8;

	//if(byte & 0x80)
	//	f=-int((byte & 0x78)>>3); // -
	//else
	//	f=int((byte & 0x78)>>3);  // +

	R=(byte & 0x07);

	byte=read_buffer();
	Q=__int16(byte);

	byte=read_buffer();
	Q=(Q<<7)+__int16((byte & 0xFE)>>1);

	//if(byte & 0x01) byte=read_buffer();
	return f;
}

int CDecodeAsterix::decode_I008_120(void)
{
	__int16 num=0;

	byte=read_buffer();
	num=__int16(byte);

	byte=read_buffer();
	num=(num<<8)+__int16(byte);
	if(num > 65534) num = 0;
	return num;
}

void CDecodeAsterix::decode_I063_010( void )
{
	byte = read_buffer();
    SensorStatus.m_SAC_SDPS = byte;

    byte = read_buffer();
	SensorStatus.m_SIC_SDPS = byte;
}

void CDecodeAsterix::decode_I063_015( void )
{
	byte = read_buffer();
}

void CDecodeAsterix::decode_I063_030( void )
{
	long lt;
	lt = 0;

	byte = read_buffer();
	lt |= byte;
	lt = lt << 8;
	
	byte = read_buffer();
	lt |= byte;
	lt = lt << 8;
	
	byte = read_buffer();
	lt |= byte;
	
	double dt;
	dt=(1.0/128)*lt;
	__int64 i_ms;
	i_ms=(__int64)(dt*1000);

	CMRTTime CurTime, TruncTime;
	CurTime.GetSysTime();
	TruncTime = CurTime;
	TruncTime.m_time.wHour = 0;
	TruncTime.m_time.wMinute = 0;
	TruncTime.m_time.wSecond = 0;
	TruncTime.m_time.wMilliseconds = 0;

	
	TruncTime.AddMS( i_ms );

	long l_dt;
	l_dt = ( long )( CurTime.GetMS() - TruncTime.GetMS() );
	if( l_dt > ( 12 * 3600 * 1000 ) ) TruncTime.AddMS( 24 * 3600 * 1000 );
	if( l_dt < (-12 * 3600 * 1000 ) ) TruncTime.SubMS( 24 * 3600 * 1000 );

	SensorStatus.m_time = TruncTime;
}

void CDecodeAsterix::decode_I063_050( void )
{
	byte = read_buffer();
    SensorStatus.m_SAC_RLP = byte;

	byte = read_buffer();
    SensorStatus.m_SIC_RLP = byte;
}

void CDecodeAsterix::decode_I063_060( void )
{
	byte = read_buffer();
	if( byte & 0xC0 ) SensorStatus.m_uc_CON = ( byte & 0xC0 ) >> 6;
	if( byte & 0x20 ) SensorStatus.m_uc_PSR = 1;
	if( byte & 0x10 ) SensorStatus.m_uc_SSR = 1;
	if( byte & 0x08 ) SensorStatus.m_uc_MDS = 1;
	if( byte & 0x04 ) SensorStatus.m_uc_ADS = 1;
	if( byte & 0x02 ) SensorStatus.m_uc_MLT = 1;

	if( byte & 0x01 ) 
	{
		byte = read_buffer();
		if( byte & 0x80 ) SensorStatus.m_uc_OPS = 1;
		if( byte & 0x40 ) SensorStatus.m_uc_ODP = 1;
		if( byte & 0x20 ) SensorStatus.m_uc_OXT = 1;
		if( byte & 0x10 ) SensorStatus.m_uc_MSC = 1;
		if( byte & 0x08 ) SensorStatus.m_uc_TSV = 1;
		if( byte & 0x04 ) SensorStatus.m_uc_NPW = 1;
	}
}

void CDecodeAsterix::decode_I063_070( void )
{
	__int16 i16;
	i16 = 0;

	byte = read_buffer();
	i16 |= byte;
	i16 = i16 << 8;
	
	byte = read_buffer();
	i16 |= byte;

    SensorStatus.m_i_timeBias = i16;
}

void CDecodeAsterix::decode_I063_080( void )
{
	__int16 i16;
	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_d_SRG = double( i16/100000 );

	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_f_SRB = float( i16 * 1852 /128 );
}

void CDecodeAsterix::decode_I063_081( void )
{
	__int16 i16;
	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_d_SAB = double( i16 * 360.0 / pow(2.0, 16.0) );
}

void CDecodeAsterix::decode_I063_090( void )
{
	__int16 i16;
	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_d_PRG = double( i16/100000 );

	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_f_PRB = float( i16 * 1852 /128 );
}

void CDecodeAsterix::decode_I063_091( void )
{
	__int16 i16;
	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_d_PAB = double( i16 * 360.0 / pow( 2.0, 16.0) );
}

void CDecodeAsterix::decode_I063_092( void )
{
	__int16 i16;
	byte = read_buffer();
	i16 = byte;
	byte = read_buffer();
	i16 = ( ( i16 << 8) & 0xFF00 ) + byte;

	SensorStatus.m_d_PEB = double( i16 * 360.0 / pow( 2.0, 16.0) );
}
//void CDecodeAsterix::decode_I063_RE( void )
//{}
//void CDecodeAsterix::decode_I063_SP( void )
//{}

UCHAR CDecodeAsterix::get_byte(void)
{	
	return(read_buffer());
}

__int16 CDecodeAsterix::get_int16(void)
{
	__int16 iRes = 0;

	iRes |= get_byte();
	
	iRes = iRes << 8;
	iRes |= get_byte();

	return(iRes);
}

long CDecodeAsterix::get_int24(void)
{
	long lRes = 0;

	lRes |= get_byte();
	
	lRes = lRes << 8;
	lRes |= get_byte();

	lRes = lRes << 8;
	lRes |= get_byte();

	return(lRes);
}

long CDecodeAsterix::get_long(void)
{
	long lRes = 0;

	lRes |= get_byte();
	
	lRes = lRes << 8;
	lRes |= get_byte();

	lRes = lRes << 8;
	lRes |= get_byte();

	lRes = lRes << 8;
	lRes |= get_byte();

	return(lRes);
}

void CDecodeAsterix::decode_I021_010()//Data Source Identification
{
	PlotMRT.SAC = get_byte();
	PlotMRT.SIC = get_byte();
}

void CDecodeAsterix::decode_I021_020()//Emmiter Category
{
	PlotMRT.ECAT = get_byte();
}

void CDecodeAsterix::decode_I021_030()//Time of Day
{
	double d;
	d = get_int24();
	d = ((d*1000.0)/128.0 + 0.5);
	PlotMRT.time_stamp.ZeroTime();
	PlotMRT.time_stamp.AddMS(__int64(d));
}

void CDecodeAsterix::decode_I021_032()//Time of Day Accuracy
{
	m_byte = get_byte();
}

void CDecodeAsterix::decode_I021_040()//Target Report Descriptor
{
	UCHAR SIM, TST, SPI; 
	
	SIM = TST = SPI = 0;

	PlotMRT.IKI |= 0x08;
	PlotMRT.MS_type = 0x00;

	m_byte = get_byte();

	if(m_byte & 0x20) SIM=1;
	if(m_byte & 0x10) TST=1;
	if(m_byte & 0x02) SPI=1;
	
	if(SIM) PlotMRT.flag |= 0x02;
	if(TST) PlotMRT.flag |= 0x01;
	if(SPI) PlotMRT.SPI = 1;
	
	m_byte = get_byte();
}

void CDecodeAsterix::decode_I021_070() //Mode 3/A Code
{
	unsigned char a, b, c, d;
	a=b=c=d=0;
	m_byte = get_byte();
					
	PlotMRT.EXNB = 0;
	if(!(m_byte & 0x80)) PlotMRT.EXNB = 1;
	if(!(m_byte & 0x40)) PlotMRT.ValidNB = 1;
	if(m_byte & 0x08) a = a|0x04;
	if(m_byte & 0x04) a = a|0x02;
	if(m_byte & 0x02) a = a|0x01;
	if(m_byte & 0x01) b = b|0x04;
					
	m_byte = get_byte();
	if(m_byte & 0x80) b = b|0x02;
	if(m_byte & 0x40) b = b|0x01;
	if(m_byte & 0x20) c = c|0x04;
	if(m_byte & 0x10) c = c|0x02;
	if(m_byte & 0x08) c = c|0x01;
	if(m_byte & 0x04) d = d|0x04;
	if(m_byte & 0x02) d = d|0x02;
	if(m_byte & 0x01) d = d|0x01;

	PlotMRT.number = ((long)(a))*1000+((long)(b))*100+((long)(c))*10+((long)(d));
}

void CDecodeAsterix::decode_I021_080()//Target Address
{
	CMyMath MM;
	PlotMRT.Address_ICAO = get_int24();
	PlotMRT.State = MM.ICAOCodeToState(PlotMRT.Address_ICAO);
}

void CDecodeAsterix::decode_I021_090()//Figure of Merit
{
	UCHAR acas, mna, dc, pa;
	m_byte = get_byte();
	acas = (m_byte&0xC0)>>6;// ACAS
	mna = (m_byte&0x30)>>4;	// Multiple navigational aids
	dc = (m_byte&0x0C)>>2;	// Differential correction
	m_byte = get_byte();
	pa = m_byte&0x0F;		// Position Accuracy
}

void CDecodeAsterix::decode_I021_095()//Velocity Accuracy
{
	m_byte = get_byte();
}

void CDecodeAsterix::decode_I021_110()//Trajectory Intent
{
	UCHAR TIS, TID;
	TIS = TID = 0;

	m_byte = get_byte();
	if(m_byte & 0x80) TIS=1;
	if(m_byte & 0x40) TID=1;

	if(m_byte & 0x01)//FX
	{
		m_byte = get_byte();
	}

	if(TIS)
	{
		m_byte = get_byte();
		if(m_byte & 0x01)//FX
		{
			m_byte = get_byte();
		}
	}

	if(TID)
	{
		UCHAR REP;
		REP = get_byte();
		for(int i=0; i<REP; i++)
		{
			get_byte();//TCP number
			get_int16();//Altitude
			get_int24();//Latitude WGS-84
			get_int24();//Longitude WGS-84
			get_byte();//Point Type
			get_int24();//TOV
			get_int16();//TTR
		}
	}
}

void CDecodeAsterix::decode_I021_130()//Position in WGS-84 Co-ordinates
{
	__int32 lB, lL;
	double dB, dL;

	lB = get_long();
	dB = lB;
	dB = dB*(180.0/(1024*1024*32));

	lL = get_long();
	dL = lL;
	dL = dL*(180.0/(1024*1024*32));

	PlotMRT.SetBLH(dB, dL, 0.0);
	PlotMRT.OriginCoord = 2;
}

void CDecodeAsterix::decode_I021_131() //Signal Amplitude
{
	UCHAR SAM;
	SAM = get_byte();
}

void CDecodeAsterix::decode_I021_140()//Geometric Altitude
{
	__int16 i16;
	i16 = get_int16();
	
	PlotMRT.Height_3D = (long)(((double)(i16)) * (6.25*0.3048)); // geometric atitude
}

void CDecodeAsterix::decode_I021_145()//Flight Level
{
	__int16 i16;
	i16 = get_int16();
	
	PlotMRT.heigh = long((double(i16))*0.3048*25);
	PlotMRT.EXH = 1;

	double dB, dL, dH;
	PlotMRT.GetBLH(dB, dL, dH);
	dH = PlotMRT.heigh;
	PlotMRT.SetBLH(dB, dL, dH);
}

void CDecodeAsterix::decode_I021_146()//Intermediate State Selected Altitude
{
	get_int16();
}

void CDecodeAsterix::decode_I021_148()//Final State Selected Altitude
{
	get_int16();
}

void CDecodeAsterix::decode_I021_150()//Air Speed
{
	__int16 i16;
	i16 = get_int16();
}

void CDecodeAsterix::decode_I021_151()//True Air Speed
{
	__int16 i16;
	i16 = get_int16();
}

void CDecodeAsterix::decode_I021_152()//Magnetic Heading
{
	double dMH;
	dMH = get_int16();
	dMH = dMH * 360.0/65536.0;
}

void CDecodeAsterix::decode_I021_155()//Barometric Vertical Rate
{
	double dBVR;
	dBVR = get_int16();
	dBVR = dBVR * 6.25 * 0.3048/60.0;// LSB = 6.25 feet/minute
}

void CDecodeAsterix::decode_I021_157()// Geometric vertical rate
{
	double dGVR;
	dGVR = get_int16();
	dGVR = dGVR * 6.25 * 0.3048/60.0;// LSB = 6.25 feet/minute
}

void CDecodeAsterix::decode_I021_160()//Ground Vector
{
	__int16 i_grSpeed, i_trAngle;
	long grSpeed, trAngle;

	i_grSpeed = get_int16();
	i_trAngle = get_int16();

	grSpeed = long(double(i_grSpeed*1852.0*pow(2.0,-14.0)));
	trAngle = long(double(i_trAngle*(360.0/(4096.0*16.0))));
}

void CDecodeAsterix::decode_I021_165()//Rate of Turn
{
	m_byte = get_byte();
	if(m_byte & 0x01)
	{
		m_byte = get_byte();
		if(m_byte & 0x01)
		{
			m_byte = get_byte();
		}
	}
}

void CDecodeAsterix::decode_I021_170()//Target Identification
{
	char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8;
	unsigned char b;
	CString str;

	m_byte = get_byte();
	ch1 = ((m_byte & 0xFC)>>2);
	b = (m_byte & 0x03);
	
	m_byte = get_byte();
	ch2 = (b<<4) + ((m_byte & 0xF0)>>4);
	b = (m_byte & 0x0F);

	m_byte = get_byte();
	ch3 = (b<<2) + ((m_byte & 0xC0)>>6);
	ch4 = (m_byte & 0x3F);
	
	m_byte = get_byte();
	ch5 = ((m_byte & 0xFC)>>2);
	b = (m_byte & 0x03);

	m_byte = get_byte();
	ch6 = (b<<4) + ((m_byte & 0xF0)>>4);
	b = (m_byte & 0x0F);
	
	m_byte = get_byte();
	ch7 = (b<<2) + ((m_byte & 0xC0)>>6);
	ch8 = (m_byte & 0x3F);

	CMyMath MM;
	str.Empty();
	str += MM.ICAOChar_to_Char(ch1);
	str += MM.ICAOChar_to_Char(ch2);
	str += MM.ICAOChar_to_Char(ch3);
	str += MM.ICAOChar_to_Char(ch4);
	str += MM.ICAOChar_to_Char(ch5);
	str += MM.ICAOChar_to_Char(ch6);
	str += MM.ICAOChar_to_Char(ch7);
	str += MM.ICAOChar_to_Char(ch8);

	PlotMRT.Ident = str;
}

void CDecodeAsterix::decode_I021_200()//Target Status
{
	m_byte = get_byte();

	if(m_byte == 1 || m_byte == 2 || m_byte == 3) PlotMRT.b = 0x01;
	if(m_byte == 4) PlotMRT.b = 0x02;
	if(m_byte == 5) PlotMRT.b = 0x03;
}

void CDecodeAsterix::decode_I021_210()//Link Technology Indicator
{
	UCHAR dti, mds, uat, vdl, otr;
	dti=mds=uat=vdl=otr=0;

	m_byte = get_byte();
	if(m_byte & 0x10) dti=1;	// CDTI
	if(m_byte & 0x08) mds=1;	// Mode S Extended Squitter
	if(m_byte & 0x04) uat=1;	// UAT
	if(m_byte & 0x02) vdl=1;	// VDL Mode 4
	if(m_byte & 0x01) otr=1;	// Other
}

void CDecodeAsterix::decode_I021_220()//Met Information
{
	UCHAR WS, WD, TMP, TRB;
	WS=WD=TMP=TRB=0;

	m_byte = get_byte();
	if(m_byte & 0x80) WS=1;
	if(m_byte & 0x40) WD=1;
	if(m_byte & 0x20) TMP=1;
	if(m_byte & 0x10) TRB=1;

	if(m_byte & 0x01)//FX
	{
		m_byte = get_byte();
	}

	if(WS) get_int16();//Wind Speed (1knot)	

	if(WD) get_int16();//Wind Direction (1°)
	
	if(TMP) get_int16();//Temperature (0.25 °C)
	
	if(TRB)	get_byte();//Turbulence	
}

void CDecodeAsterix::decode_I021_230()//Roll Angle
{
	double dRollAngle;
	dRollAngle = get_int16();
	dRollAngle *= 0.01;
}

void CDecodeAsterix::decode_I021_910() //Категория навигационной целостности
{
	UCHAR NIC;
	NIC = get_byte();
}

void CDecodeAsterix::decode_I021_920() //Характеристика наблюдения цели
{
	UCHAR uc;
	uc = get_byte();
	if(uc & 0x04) PlotMRT.IKI |= 0x01;//ПРЛ
	if(uc & 0x02)//УВД
	{
		PlotMRT.IKI |= 0x02;
		PlotMRT.MK = 0;
	}
	if(uc & 0x01)//RBS
	{
		PlotMRT.IKI |= 0x02;
		PlotMRT.MK = 1;
	}
}

void CDecodeAsterix::decode_I021_930() //: Бортовой номер цели, наблюдаемой МВРЛ в режиме УВД
{
	long ID;
	ID = get_int24();
	long a, b, c, d, e;
	a=b=c=d=e=0;
				
	a = (ID & 0x0F0000) >> 16;
	b = (ID & 0x00F000) >> 12;
	c = (ID & 0x000F00) >> 8;
	d = (ID & 0x0000F0) >> 4;
	e = (ID & 0x00000F);
	
	PlotMRT.number = a*10000 + b*1000 + c*100 + d*10 + e;
}

void CDecodeAsterix::decode_I048_010()
{
	byte = read_buffer();
	SAC = byte;
	byte = read_buffer();
	SIC = byte;
}
void CDecodeAsterix::decode_I048_130()
{
	unsigned char b;
	do
	{
		byte = read_buffer();
		b = byte;
		for(int i = 0; i < 7; i++)
		{
			if(b & (0x80 >> i))
			{
				byte = read_buffer();
			}
		}
	}while(b & 0x01);
}
void CDecodeAsterix::decode_I048_140()
{
	__int32 i32;
	byte = read_buffer();
	i32 = byte;
	i32 = i32 << 8;
	byte = read_buffer();
	i32 = i32 | byte;
	i32 = i32 << 8;
	byte = read_buffer();
	i32 = i32 | byte;

	double d_sec;
	d_sec = ((double)(i32)) / 128;
	
	__int64 i64_ms;
	i64_ms = __int64(d_sec * 1000);

	time_stamp.m_time.wHour = 0;
	time_stamp.m_time.wMinute = 0;
	time_stamp.m_time.wSecond = 0;
	time_stamp.m_time.wMilliseconds = 0;

	time_stamp.AddMS(i64_ms);
}

void CDecodeAsterix::decode_I048_020()
{
	unsigned char SIM, typ;
	unsigned char TST, RAB, ME; 
	
	IKI = 0;
	MK = 1;
	MS_type = 0;
	RAB = 0;

	SIM = TST = MI = 0;

	byte = read_buffer();

	typ = (byte & 0xE0)>>5;
	if(typ == 0x01)
	{
		int r;
		r=3;
	}
	if(typ==0x01)
	{
		IKI=0x01;
		MS_type=0;
	}
	if(typ==0x02)
	{
		IKI=0x02;
		MK=1;//RBS
		MS_type=0;
	}
	if(typ==0x03)
	{
		IKI=0x03;
		MK=1;
		MS_type=0;
	}
	if(typ==0x04)
	{
		IKI=0x04;
		MS_type=0;//All Call
	}
	if(typ==0x05)
	{
		IKI=0x04;
		MS_type=1;//Roll Call
	}
	if(typ==0x06)
	{
		IKI=0x04 | 0x01;//Mode S + PRL
		MS_type=0;//All Call
	}
	if(typ==0x07)
	{
		IKI=0x04 | 0x01;//Mode S + PRL
		MS_type=1;//Roll Call
	}
	if(byte & 0x10) SIM=1;
	if(byte & 0x04) SPI=1;
	if(byte & 0x02) RAB=1;

	if(byte & 0x01)
	{
		byte=read_buffer();
		if(byte & 0x80) TST=1;
		if(byte & 0x10) ME=1;
		if(byte & 0x08) MI=1;
	}
	if(byte & 0x01)
	{
		while((byte = read_buffer()) & 0x01)
		{
		}
	}
	if(SIM) FLAG = 0x02;
	if(TST) FLAG = 0x01;
}
void CDecodeAsterix::decode_I048_030()
{
	while((byte = read_buffer()) & 0x01)
	{
	}
}
void CDecodeAsterix::decode_I048_040()
{
	long Rho, Theta;
	double d_rho, d_theta; 

	byte=read_buffer();
	Rho=(long)(byte);
	byte=read_buffer();
	Rho=(Rho<<8)+byte;

	byte=read_buffer();
	Theta=(long)(byte);
	byte=read_buffer();
	Theta=(Theta<<8)+byte;

	d_rho=(double)(Rho);
	d_rho=d_rho*1852.0/256.0;

	d_theta=(double)(Theta);
	d_theta=d_theta*(360.0/(4096.0*16.0));

//	X=(long)(d_rho*sin(grrad.gr_to_rad(d_theta)));
//	Y=(long)(d_rho*cos(grrad.gr_to_rad(d_theta)));

	Az = d_theta;
	D = d_rho;

	X = (long)(d_rho * sin(d_theta*gr2rad));
	Y = (long)(d_rho * cos(d_theta*gr2rad));
}

void CDecodeAsterix::decode_I048_070()
{
	unsigned char a, b, c, d;
	a=b=c=d=0;
	byte=read_buffer();
				
	EXNB=0;
	if(!(byte & 0x80)) EXNB=1;
	if(byte & 0x08) a=a|0x04;
	if(byte & 0x04) a=a|0x02;
	if(byte & 0x02) a=a|0x01;
	if(byte & 0x01) b=b|0x04;
				
	byte=read_buffer();
	if(byte & 0x80) b=b|0x02;
	if(byte & 0x40) b=b|0x01;
	if(byte & 0x20) c=c|0x04;
	if(byte & 0x10) c=c|0x02;
	if(byte & 0x08) c=c|0x01;
	if(byte & 0x04) d=d|0x04;
	if(byte & 0x02) d=d|0x02;
	if(byte & 0x01) d=d|0x01;

	NB=0;
	if(EXNB) NB=((long)(a))*1000+((long)(b))*100+((long)(c))*10+((long)(d));

	if(NB==7700) B=0x01;
	if(NB==7600) B=0x02;
	if(NB==7500) B=0x03;
}
void CDecodeAsterix::decode_I048_080()
{
	byte = read_buffer();
	byte = read_buffer();
}
void CDecodeAsterix::decode_I048_090()
{
	__int16 i_H;

	ZH = 1;	//RBS - absolute heigh
	byte = read_buffer();
	EXH = 0;
	i_H = 0;
	if((!(byte & 0x80)) && (!(byte & 0x40)))
		EXH = 1;
	
	i_H = (__int16)(byte & 0x3F);
	byte = read_buffer();
	i_H = (i_H << 8) + byte;
	i_H <<= 2;
	i_H /= 4;
	H = i_H;

	/*if(EXH) H=(long)(((double)(H))*(1852.0*25.0/6080.0));
	else H=0;*/

	//H=(long)(((double)(H))*(1852.0*25.0/6080.0));
	H = long((double(H)) * 0.3048 * 25.0);
}
void CDecodeAsterix::decode_I048_100()
{
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
}
void CDecodeAsterix::decode_I048_120()
{
	byte = read_buffer();
	if(byte & 0x80)
	{
		read_buffer();
		read_buffer();
	}
	if(byte & 0x40)
	{
		unsigned char num_oct = read_buffer();
		for(int i = 0; i < num_oct; i++)
		{
			read_buffer();
			read_buffer();
			read_buffer();
			read_buffer();
			read_buffer();
			read_buffer();
		}
	}
}
void CDecodeAsterix::decode_I048_220()
{
	unsigned char a, b, c;

	byte = read_buffer();
	a = byte;
	byte = read_buffer();
	b = byte;
	byte = read_buffer();
	c = byte;
	Address_ICAO=(a<<16)+(b<<8)+c;
}

void CDecodeAsterix::decode_I048_230()
{
	byte = read_buffer();
	byte = read_buffer();
}

void CDecodeAsterix::decode_I048_240()
{
	char ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8;
	unsigned char b;
	CString str;

	byte=read_buffer();
	ch1=((byte & 0xFC)>>2);
	b=(byte & 0x03);
	
	byte=read_buffer();
	ch2=(b<<4)+((byte & 0xF0)>>4);
	b=(byte & 0x0F);

	byte=read_buffer();
	ch3=(b<<2)+((byte & 0xC0)>>6);
	ch4=(byte & 0x3F);
	
	byte=read_buffer();
	ch5=((byte & 0xFC)>>2);
	b=(byte & 0x03);

	byte=read_buffer();
	ch6=(b<<4)+((byte & 0xF0)>>4);
	b=(byte & 0x0F);
	
	byte=read_buffer();
	ch7=(b<<2)+((byte & 0xC0)>>6);
	ch8=(byte & 0x3F);

	CMyMath MM;
	str.Empty();
	str+=MM.ICAOChar_to_Char(ch1);
	str+=MM.ICAOChar_to_Char(ch2);
	str+=MM.ICAOChar_to_Char(ch3);
	str+=MM.ICAOChar_to_Char(ch4);
	str+=MM.ICAOChar_to_Char(ch5);
	str+=MM.ICAOChar_to_Char(ch6);
	str+=MM.ICAOChar_to_Char(ch7);
	str+=MM.ICAOChar_to_Char(ch8);

	Ident = str;
}

void CDecodeAsterix::decode_I048_250()
{
	byte = read_buffer();
	int num_oct = byte;
	for (int k = 1; k < num_oct; k++)
	{
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
		byte = read_buffer();
	}
}
void CDecodeAsterix::decode_I048_260()
{
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
	byte = read_buffer();
}

void CDecodeAsterix::decode_I048_161()
{
	int nt;

	byte = read_buffer();
	nt = int(byte & 0x0F);
	byte = read_buffer();
	nt = (nt<<8) + byte;
}

void CDecodeAsterix::decode_I048_042()
{
	/*long x, y;

	byte=read_buffer();
	x=(long)(byte);
	byte=read_buffer();
	x=(x<<8)+byte;

	byte=read_buffer();
	y=(long)(byte);
	byte=read_buffer();
	y=(y<<8)+byte;
	if(X==0 && Y==0){
		X=x;
		X=(long)(X*1852.0/128.0);

		Y=y;
		Y=(long)(Y*1852.0/128.0);
	}*/

	__int16 ix, iy;

	ix=0;
	ix=ix | read_buffer();
	ix=(ix<<8) | read_buffer();

	iy=0;
	iy=iy | read_buffer();
	iy=(iy<<8) | read_buffer();

	X=ix;
	X=(long)(X*1852.0/128.0);

	Y=iy;
	Y=(long)(Y*1852.0/128.0);
}
void CDecodeAsterix::decode_I048_200()
{
	double dV, dPhi;
	__int16 iV, iPhi;

	byte=read_buffer();
	iV=__int16(byte<<8);
	iV=iV & 0xFF00;
	byte=read_buffer();
	iV = iV | byte;
	dV = double(iV * 1852.0/0.00006103515625);
	byte=read_buffer();
	iPhi=__int16(byte<<8);
	iPhi=iPhi & 0xFF00;
	byte=read_buffer();
	iPhi = iPhi | byte;
	dPhi = double(iPhi * 360.0/65536.0);
	Vx = dV * cos(dPhi);
	Vy = dV * sin(dPhi);
}
void CDecodeAsterix::decode_I048_170()
{
	unsigned char cnf, rad, tre, byte = read_buffer();

	if(byte & 0x80)
		cnf = 1;//экстраполированный трек (не подтвержденный)
	
	rad = ((byte & 0x60) >> 5); //вид информации для трека (ПРЛ, ВРЛ, объединенный)
	if(byte & 0x01)
	{
		byte = read_buffer();
		if(byte & 0x80)
			tre = 1;//end of track lifetime
	}
	while(byte & 0x01)
	{
		byte = read_buffer();
	}

}
void CDecodeAsterix::decode_I048_210()
{
	long Sigma_xy, Sigma_h;

	byte = read_buffer();			
	Sigma_xy = (long)(byte*1852.0/128.0);//СКО по x,y
	
	byte = read_buffer();	
	Sigma_h = (long)(byte*1852.0/128.0);//СКО по h

	byte = read_buffer();	
	byte = read_buffer();	
}
void CDecodeAsterix::decode_I048_110()
{
	__int16 i_H;

	byte = read_buffer();
	i_H = (__int16)(byte & 0x3F);
	
	byte = read_buffer();
	i_H <<= 8;
	i_H += byte;
	i_H <<= 2;
	i_H /= 4;

	Height_3D = i_H;
	Height_3D = (long)(((double)(Height_3D))*(1852.0*25.0/6080.0));
}
void CDecodeAsterix::decode_I048_055()//Mode 1
{
	byte = read_buffer();		
}
void CDecodeAsterix::decode_I048_050()//Mode 2
{
	byte = read_buffer();		
	byte = read_buffer();	
}
void CDecodeAsterix::decode_I048_065()
{
	byte = read_buffer();		
}
void CDecodeAsterix::decode_I048_060()
{
	byte = read_buffer();		
	byte = read_buffer();	
}
void CDecodeAsterix::decode_I034_010()
{
	byte=read_buffer();
	SAC=byte;
	byte=read_buffer();
	SIC=byte;
}

UCHAR CDecodeAsterix::decode_I034_000()
{
	unsigned char Message_Type;

	byte = read_buffer();
	Message_Type = byte;

	if(Message_Type == 0x01) //North
	{
		if(pMRT != NULL)
		{	
			CMRTNorth N(SAC, SIC, 0, i_comport, uc_LpdType);

			pMRT->AddNorthMRT(N);
			N.track = 1;
			pMRT->AddNorthMRT(N);
		}
	}

	if(Message_Type == 0x02) //АМ
	{	
		if(pMRT != NULL)
			AmMRT=::CMRTAm(SAC, SIC, 0, i_comport, uc_LpdType, 0.0f);
	}

	return (Message_Type);
}
void CDecodeAsterix::decode_I034_020(void)
{
	unsigned char Sector_Number;
	float f_am;
	byte = read_buffer();
	Sector_Number = byte;
	f_am = (float)( 1.40625*((float)(Sector_Number)) );

	AmMRT.f_AM = f_am;
}

void CDecodeAsterix::decode_I034_030()
{
	__int32 i32;
	byte = read_buffer();
	i32 = byte;
	i32 = i32 << 8;
	byte = read_buffer();
	i32 = i32 | byte;
	i32 = i32 << 8;
	byte = read_buffer();
	i32 = i32 | byte;

	double d_sec;
	d_sec = ((double)(i32)) / 128;
	
	__int64 i64_ms;
	i64_ms = __int64(d_sec * 1000);

	time_stamp.m_time.wHour = 0;
	time_stamp.m_time.wMinute = 0;
	time_stamp.m_time.wSecond = 0;
	time_stamp.m_time.wMilliseconds = 0;

	time_stamp.AddMS(i64_ms);
}
void CDecodeAsterix::decode_I034_041()
{
	byte=read_buffer();
	byte=read_buffer();
}
void CDecodeAsterix::decode_I034_050()
{
	unsigned char b;
	do
	{
		byte=read_buffer();//Structure of system config. status
		b=byte;
		if(b & 0x80){
			byte=read_buffer();//COM
		}
		if(b & 0x10){
			byte=read_buffer();//PSR
		}
		if(b & 0x08){
			byte=read_buffer();//SSR
		}
		if(b & 0x04){
			byte=read_buffer();//MDS
			byte=read_buffer();//MDS
		}
	} while (b & 0x01);
	
}

void CDecodeAsterix::decode_I034_060()
{
	unsigned char b;
	do
	{
		byte=read_buffer();//Structure system Processing Mode
		b=byte;
		if(b & 0x80){
			byte=read_buffer();//COM
		}
		if(b & 0x10){
			byte=read_buffer();//PSR
		}
		if(b & 0x08){
			byte=read_buffer();//SSR
		}
		if(b & 0x04){
			byte=read_buffer();//MDS
		}
	} while (b & 0x01);

}
void CDecodeAsterix::decode_I034_070()
{
	byte = read_buffer();
	int num_oct = byte;
	for (int k = 1; k < num_oct; k++)
	{
		byte = read_buffer();
		byte = read_buffer();
	}
}
void CDecodeAsterix::decode_I034_090()
{
	byte=read_buffer();
	byte=read_buffer();
}
void CDecodeAsterix::decode_I034_100()
{
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
}
void CDecodeAsterix::decode_I034_110()
{
	byte=read_buffer();
}
void CDecodeAsterix::decode_I034_120()
{
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
	byte=read_buffer();
}

