/***********************************************
����STC15F2K60S2ϵ�е�Ƭ��C���Ա��ʵ��
ʹ������ͷ�ļ������������ٰ���"REG51.H"
#include <STC15F2K60S2.h>
***********************************************/
#include "sys_fun.h"
#include "songs.h"
#include "display.h"
#include "sing.h"
#include "string.h"

uchar port_status;
uchar key;
unsigned char Count;
double pitch = 7;
char pitchFlag = 1;
char currentPage = 1;
unsigned char RECORDED[250];
unsigned char SheetMid[250];
unsigned char SheetUp[250];
unsigned char SheetDown[250];
unsigned int Sheet2Note[250];



int lastStartCount = -1;
int lastEndCount = 0;
uchar lastNote = 0x00;

void Time0_Int() interrupt 1
{
	 TH0 = 0xDC;
	 TL0 = 0x00;
	 Count++;   
}

void Playnote(uchar flag, int i){
	OPT_CHECK = 0xFF;
	while (OPT_CHECK&flag){
		bee_Speak = ~bee_Speak;
		Delay_xMs(NOTE[i-1]);
		OPT_CHECK = 0xFF;
	}
	pitchFlag = 1;
}

void PlayMusic();
void ManualPlay();
void MenuDisplay(int);
void Record();
int Recordnote(uchar, int, int);
void PlayRecord();
void GetSheet(int);

void main()
{
	P5M1=0x00;
	P5M0=0x30;
	
	P4M1=0xC0;
	P4M0=0x00;
	
	P3M1=0x00;
	P3M0=0x00;
	
	Ini_Lcd();//Һ����ʼ���ӳ���
	Disp(1,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
	Disp(2,1,12,"������·���");//��ʾ���ݵ�LCD12864�ӳ���
	Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
	Disp(4,1,12,"Һ����ʾ�ɹ�");//��ʾ���ݵ�LCD12864�ӳ���*/
	while(1)
	{
		s1_s2_check();
		P4M1=0x00;
		P4M0=0x00;
		Ini_Lcd();//Һ����ʼ���ӳ���
		MenuDisplay(currentPage);
		KeyIO = 0xF0;
		while(1){
			s1_s2_check();
			if ((P1&0xF0)!=0xF0){
				Delay_xMs(100);
				if((KeyIO&0xF0)!=0xF0){
					key = scankey();
					switch (key)
					{
					case 11:
						PlayMusic();
						bee_Speak = 0;
						MenuDisplay(currentPage);
						break;
					case 12:
						ManualPlay();
						MenuDisplay(currentPage);
						break;
					case 13:
						currentPage ++;
						MenuDisplay(currentPage);
						break;
					case 14:
						Record();
						MenuDisplay(currentPage);
						break;
					case 21:
						PlayRecord();
						MenuDisplay(currentPage);
						break;
					case 22:
						currentPage --;
						MenuDisplay(currentPage);
						break;
					default:
						break;
					}
				}
				Delay_xMs(2500);
			}
		}
		Delay_xMs(2500);		
	}
}

/*******************��ʱ����ʼ��********************/
void Time0_Init()
{
 TMOD = 0x01;
 IE   = 0x82;
 TH0  = 0xDC;
 TL0  = 0x00;  //11.0592MZ
}
/****************��������*************************/
void s1_s2_check(void)
{
	if(P4&0x40) 
		DLED_1 =1;
	else	
		DLED_1 =0;
	if((P4&0x80)) 
		DLED_2 =1;
	else	
		DLED_2 =0;
}

/****************��������*************************/
void Play_Song(unsigned char i)
{
	unsigned char Temp1,Temp2;
	unsigned int Addr,start;
	unsigned int lineCount, loc;
	unsigned char lrc[20];

	lineCount = 1;
	loc = 0;
	start = INDEX[i];
	Addr = start;  
	GetSheet(Addr);
	
	strncpy(lrc,  SheetUp, 16);
	Disp(2,0,16,lrc);
	strncpy(lrc, SheetMid, 16);
	Disp(3,0,16,lrc);
	strncpy(lrc, SheetDown, 16);
	Disp(4,0,16,lrc);
	Count = 0;    

	while(1)
	{
		
	 	Temp1 = SONG[Addr++];
	    if ( Temp1 == 0xFF )
	    {
		    TR0 = 0;                 
		    Delay_xMs(100);
	    }
	    else if ( Temp1 == 0x00 )   
	    {
	     	return;
	    }
	    else
	    {
		    Temp2 = SONG[Addr++];
		    TR0 = 1;
			while(1)
			{
			    bee_Speak = ~bee_Speak;
			    Delay_xMs(Temp1);
			    if ( Temp2 == Count )
			    {
					if (Sheet2Note[Addr - start] > lineCount*16){
						loc = loc + 16;
						lineCount++;
						strncpy(lrc, SheetUp+loc, 16);
						Disp(2,0,16,lrc);
						strncpy(lrc, SheetMid+loc, 16);
						Disp(3,0,16,lrc);
						strncpy(lrc, SheetDown+loc, 16);
						Disp(4,0,16,lrc);
					}
			        Count = 0;
			        break;
			    }
				KeyIO=0xF0;
				if ((P1&0xf0)!=0xf0) return;
			}
	    }
	}
	bee_Speak = 0;	//�رշ�����
}


/*****************�����ƶ˿ڵ�״̬*****************/
uchar ctrl_port_check(void)
{
		SW_CTRL = 0xFF;
		return 	SW_CTRL&0xF0;
}

uchar scankey(void)//������̷�תɨ��
{
	uint keyvalue=0;
	uchar temp1,temp2,keycode;

	KeyIO=0xf0;		//����0����1
	temp1=KeyIO&0xf0;
	if((temp1&0xf0)==0xf0) return(0);  //���޼����·���0
	KeyIO=0x0f;
	temp2=KeyIO&0x0f;					   //���м����£�����1����0
	keycode=~(temp1|temp2);			   //��ü�����
	switch(keycode)					   //���ݼ����뷵�ؼ�ֵ
	{
		case 0x11:return(11);break;
		case 0x21:return(12);break;
		case 0x41:return(13);break;
		case 0x81:return(14);break;
		case 0x12:return(21);break;
		case 0x22:return(22);break;
		case 0x42:return(23);break;
		case 0x82:return(24);break;
		case 0x14:return(31);break;
		case 0x24:return(32);break;
		case 0x44:return(33);break;
		case 0x84:return(34);break;
		case 0x18:return(41);break;
		case 0x28:return(42);break;
		case 0x48:return(43);break;
		case 0x88:return(44);break;
		default:  return(0);break;
	}
	return(0);
}
	 
/****************�ӳٺ�����************************/
void Delay_xMs(unsigned int x)
{
    unsigned int i,j;
    for( i = 0;i < x;i++ )
    {
        for( j = 0;j<2;j++ );
    }
}

// �˵����ܺ���

void MenuDisplay(int page){
	Ini_Lcd();
	switch (page)
	{
	case 1:
		Disp(1,3,4,"�˵�");
		Disp(2,0,10,"1.��������");
		Disp(3,0,10,"2.����ģʽ");
		Disp(4,0,8,"3.��һҳ");
		break;
	case 2:
		Disp(1,3,4,"�˵�");
		Disp(2,0,10,"4.¼��ģʽ");
		Disp(3,0,10,"5.����¼��");
		Disp(4,0,8,"6.��һҳ");
	default:
		break;
	}

}

void PlayMusic(){
	Ini_Lcd();
	Disp(1,0,8,"1.�±���");
	Disp(2,0,6,"2.����");
	Disp(3,0,10,"3.ף��ƽ��");
	Disp(4,0,10,"4.���֮��");

	Time0_Init();
	KeyIO = 0xF0;
	while(1){
		s1_s2_check();
		if ((P1&0xF0)!=0xF0){
			Delay_xMs(100);
			if((KeyIO&0xF0)!=0xF0){
				key = scankey();
				switch (key)
				{
				case 11:
					Ini_Lcd();
					Disp(1,0,8,"1.�±���");
					Play_Song(0);
					return;
					break;
				case 12:
					Ini_Lcd();
					Disp(1,0,6,"2.����");
					Play_Song(1);
					return;
					break;
				case 13:
					Ini_Lcd();
					Disp(1,0,10,"3.ף��ƽ��");
					Play_Song(2);
					return;
					break;
				case 14:
					Ini_Lcd();
					Disp(1,0,10,"4.���֮��");
					Play_Song(3);
					return;
					break;
				default:
					return;
					break;
				}
			}
			Delay_xMs(2500);
		}
	}
}

void ManualPlay(){
	s1_s2_check();
	Ini_Lcd();
	Disp(1,2,8,"����ģʽ");
	if (pitch!=0) Disp(2,0,10,"2.��������");
	if (pitch!=14) Disp(3,0,10,"3.�������");
	Disp(4,1,12,"�����������");
	while(1) {
		KeyIO=0xF0;
		if ((P1&0xf0)!=0xf0) {
			Delay_xMs(100);
			if((KeyIO&0xF0)!=0xF0){
				key = scankey();
				switch (key){
					case 12:
						if (!pitchFlag) break;
						if (pitch>0) pitch-=7;
						if (pitch==0) Disp(2,0,16,"�����Ѿ��������");
						Disp(3,0,10,"3.�������");
						pitchFlag = 0;
						break;
					case 13:
						if (!pitchFlag) break;
						if (pitch<14) pitch+=7;
						if (pitch==14) Disp(3,0,16,"�����Ѿ��������");
						Disp(2,0,10,"2.��������");
						pitchFlag = 0;
						break;
					default:
						return;
						break;
				}
			}
			Delay_xMs(100);
		}
	 	OPT_CHECK = 0xFF;
	 	if (OPT_CHECK&0x01) Playnote(0x01, 7+pitch);
		 else if (OPT_CHECK&0x02) Playnote(0x02, 6+pitch);
		 else if (OPT_CHECK&0x04) Playnote(0x04, 5+pitch);
		 else if (OPT_CHECK&0x08) Playnote(0x08, 4+pitch);
		 else if (OPT_CHECK&0x10) Playnote(0x10, 3+pitch);
		 else if (OPT_CHECK&0x20) Playnote(0x20, 2+pitch);
		 else if (OPT_CHECK&0x40) Playnote(0x40, 1+pitch);	
	}
}

// ¼��

void Record(){
	char continueFlag = 0;
	int currentIndex = 0;
	lastEndCount = 0;
	lastStartCount = -1;
	lastNote = 0x00;

	s1_s2_check();
	Ini_Lcd();
	Disp(1,2,8,"¼��ģʽ");
	Disp(2,0,10,"1.��ʼ¼��");
	
	Disp(4,1,12,"������������");
	
	while(!continueFlag) {
		KeyIO=0xF0;
		if ((P1&0xf0)!=0xf0) {
			Delay_xMs(100);
			if((KeyIO&0xF0)!=0xF0){
				key = scankey();
				switch (key){
					case 11: continueFlag = 1;break;
					default: return;
				}
			}
			Delay_xMs(100);
		}
	}
	memset(RECORDED,0x00,sizeof(RECORDED));
	Disp(2,0,6,"¼����");
	Disp(3,0,10,"2.����¼��");
	Count = 0;
	Time0_Init();
	while(1){
		KeyIO=0xF0;
		if ((P1&0xf0)!=0xf0) {
			Delay_xMs(100);
			if((KeyIO&0xF0)!=0xF0){
				key = scankey();
				switch (key){
					case 12: 
						Disp(3,0,8,"¼�����");
						RECORDED[currentIndex] = lastNote;
						RECORDED[currentIndex+1] = lastEndCount - lastStartCount; 
						return;
					default: return;
				}
			}
			Delay_xMs(100);
		}
		OPT_CHECK = 0xFF;
	 	if (OPT_CHECK&0x01) currentIndex = Recordnote(0x01, 7+pitch, currentIndex);
		 else if (OPT_CHECK&0x02) currentIndex  = Recordnote(0x02, 6+pitch, currentIndex);
		 else if (OPT_CHECK&0x04) currentIndex  = Recordnote(0x04, 5+pitch, currentIndex);
		 else if (OPT_CHECK&0x08) currentIndex  = Recordnote(0x08, 4+pitch, currentIndex);
		 else if (OPT_CHECK&0x10) currentIndex  = Recordnote(0x10, 3+pitch, currentIndex);
		 else if (OPT_CHECK&0x20) currentIndex  = Recordnote(0x20, 2+pitch, currentIndex);
		 else if (OPT_CHECK&0x40) currentIndex  = Recordnote(0x40, 1+pitch, currentIndex);	
	}
	
}

//����¼��

int Recordnote(uchar flag, int i, int index){
	if (lastStartCount != -1){
		RECORDED[index] = lastNote;
		RECORDED[index+1] = Count - lastStartCount;
	}
	OPT_CHECK = 0xFF;
	lastStartCount = Count;
	while (OPT_CHECK&flag){
		bee_Speak = ~bee_Speak;
		Delay_xMs(NOTE[i-1]);
		OPT_CHECK = 0xFF;
	}
	lastEndCount = Count;
	lastNote = NOTE[i-1];
	return index+2;
}

// ¼������

void PlayRecord()
{
	unsigned char Temp1,Temp2;
	unsigned int Addr;

	s1_s2_check();
	Ini_Lcd();
	Disp(1,2,8,"����¼��");
	Disp(4,1,12,"�����������");

	     
	Time0_Init();
	Addr = 2;  
	Count = 0; 
	while(1)
	{
		Temp1 = RECORDED[Addr++];
	    if ( Temp1 == 0xFF )  
	    {
		    TR0 = 0;                 
		    Delay_xMs(100);
	    }
	    else if ( Temp1 == 0x00 )   
	    {
	     	return;
	    }
	    else
	    {
		    Temp2 = RECORDED[Addr++];
		    TR0 = 1;
			while(1)
			{
			    bee_Speak = ~bee_Speak;
			    Delay_xMs(Temp1);
			    if ( Temp2 == Count )
			    {
			       Count = 0;
			       break;
			    }
			 	KeyIO=0xF0;
			 	if ((P1&0xf0)!=0xf0) return;
			}
	    }
	}
	bee_Speak = 0;	//�رշ�����
}

// ��������

void GetSheet(int start){
	unsigned char Temp1,Temp2;
	unsigned int Addr, Index;

	Addr = start; Index = 0;
	Temp1 = SONG[Addr];
	while (Temp1 != 0x00){
		Sheet2Note[Addr - start] = Index;
		switch (Temp1){
			case 0x60:
			case 0x30:
			case 0x18:
				SheetMid[Index] = '1';
				break;
			case 0x56:
			case 0x2b:
			case 0x15:
				SheetMid[Index] = '2';
				break;
			case 0x4C:
			case 0x26:
			case 0x13:
				SheetMid[Index] = '3';
				break;
			case 0x48:
			case 0x24:
			case 0x12:
				SheetMid[Index] = '4';
				break;
			case 0x40:
			case 0x20:
			case 0x10:
				SheetMid[Index] = '5';
				break;
			case 0x39:
			case 0x1C:
			case 0x0E:
				SheetMid[Index] = '6';
				break;
			case 0x32:
			case 0x19:
			case 0x0D:
				SheetMid[Index] = '7';
				break;
			default:
				Addr++;
				Temp1 = SONG[Addr];
				continue;
				break;
		}
		if (Temp1 == 0xFF) continue;
		if (Temp1 >=0x32){
			SheetDown[Index] = '.';
			SheetUp[Index] = ' ';
		} else if (Temp1 <= 0x18){
			SheetUp[Index] = '.';
			SheetDown[Index] = ' ';
		} else {
			SheetUp[Index] = ' ';
			SheetDown[Index] = ' ';
		}
		Addr++;
		Temp2 = SONG[Addr];
		if (Temp2 == 0x20) {
			Addr++;
			Index++;
			Temp1 = SONG[Addr];
			continue;
		}
		else if (Temp2 == 0x10)
			SheetDown[Index] = '-';
		else if (Temp2 == 0x08)
			SheetDown[Index] = '=';
		else if (Temp2 == 0x30){
			Index++;
			SheetMid[Index] = '.';
			SheetDown[Index] = ' ';
			SheetUp[Index] = ' ';
		} else if (Temp2 >= 0x40){
			Temp2 = Temp2 - 0x20;
			while (Temp2 >= 0x20){
				Index++;
				SheetMid[Index] = '-';
				SheetDown[Index] = ' ';
				SheetUp[Index] = ' ';
				Temp2 = Temp2 - 0x20;
			}
		}
		Addr++;
		Index++;
		Temp1 = SONG[Addr];
	}
	return;
}