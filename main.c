/***********************************************
����STC15F2K60S2ϵ�е�Ƭ��C���Ա��ʵ��
ʹ������ͷ�ļ������������ٰ���"REG51.H"
#include <STC15F2K60S2.h>
***********************************************/
#include "sys_fun.h"
#include "songs.h"
#include "display.h"
#include "sing.h"

uchar port_status;
uchar key;
unsigned char Count;
double pitch = 7;
char pitchFlag = 1;

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
		MenuDisplay(1);
		KeyIO = 0xF0;
		while((ctrl_port_check()&0xF0)==0xF0){
			s1_s2_check();
			if ((P1&0xF0)!=0xF0){
				Delay_xMs(100);
				if((KeyIO&0xF0)!=0xF0){
					key = scankey();
					switch (key)
					{
					case 11:
						PlayMusic();
						MenuDisplay(1);
						break;
					case 12:
						ManualPlay();
						MenuDisplay(1);
						break;
					default:
						break;
					}
				}
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
	 unsigned int Addr;
	 Count = 0;      
	 Addr = i * 217;  
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
	default:
		break;
	}

}

void PlayMusic(){
	Ini_Lcd();
	Disp(1,2,8,"���ֲ���");
	Disp(4,1,12,"�����������");
	Time0_Init();
	Play_Song(0);
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
