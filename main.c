/***********************************************
����STC15F2K60S2ϵ�е�Ƭ��C���Ա��ʵ��
ʹ������ͷ�ļ������������ٰ���"REG51.H"
#include <STC15F2K60S2.h>
***********************************************/
#include "sys_fun.h"
#include "songs.h"
#include "display.h"

uchar port_status;
uchar key;
unsigned char Count;

void Time0_Int() interrupt 1
{
	 TH0 = 0xDC;
	 TL0 = 0x00;
	 Count++;   
}

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
	Disp(4,1,12,"Һ����ʾ�ɹ�");//��ʾ���ݵ�LCD12864�ӳ���
	while(1)
	{
		
		if((ctrl_port_check()&0xF0)==0xF0)
		{  	
			s1_s2_check();
			P4M1=0x00;
			P4M0=0x00;
			Ini_Lcd();//Һ����ʼ���ӳ���
			Disp(1,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
			Disp(2,1,12,"������·���");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(4,1,12,"Һ����ʾ�ɹ�");//��ʾ���ݵ�LCD12864�ӳ���
			Delay_xMs(2500);		
		}
		
		if(!(ctrl_port_check()&0x80))
		{ 
			KeyIO=0xF0;
			Ini_Lcd();//Һ����ʼ���ӳ���
			Disp(1,0,16,"ϵͳ���а������");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(4,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
			while(!(ctrl_port_check()&0x80))
			{
				s1_s2_check();				
				if((P1&0xf0)!=0xf0) //����м�����
		        {
		            Delay_xMs(100);   //��ʱȥ����
		            if((KeyIO&0xf0)!=0xf0)   //���ж�
		            {
						key=scankey();
						switch(key)
						{
							case 11:Disp(2,1,12,"KEY_VALUE:01");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 12:Disp(2,1,12,"KEY_VALUE:02");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 13:Disp(2,1,12,"KEY_VALUE:03");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 14:Disp(2,1,12,"KEY_VALUE:04");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 21:Disp(2,1,12,"KEY_VALUE:05");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 22:Disp(2,1,12,"KEY_VALUE:06");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 23:Disp(2,1,12,"KEY_VALUE:07");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 24:Disp(2,1,12,"KEY_VALUE:08");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 31:Disp(2,1,12,"KEY_VALUE:09");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 32:Disp(2,1,12,"KEY_VALUE:10");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 33:Disp(2,1,12,"KEY_VALUE:11");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 34:Disp(2,1,12,"KEY_VALUE:12");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 41:Disp(2,1,12,"KEY_VALUE:13");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 42:Disp(2,1,12,"KEY_VALUE:14");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 43:Disp(2,1,12,"KEY_VALUE:15");	break;//��ʾ���ݵ�LCD12864�ӳ���
							case 44:Disp(2,1,12,"KEY_VALUE:16");	break;//��ʾ���ݵ�LCD12864�ӳ���
						}
					}   
				}
			}
		}
		else if(!(ctrl_port_check()&0x40))
		{ 
			s1_s2_check();
			Ini_Lcd();//Һ����ʼ���ӳ���
			Disp(1,0,16,"ϵͳ�������ֲ���");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(2,1,13,"Playing Music");//��ʾ���ݵ�LCD12864�ӳ���  
			Disp(3,2,8,"87951197");//��ʾ���ݵ�LCD12864�ӳ���
			Disp(4,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���		
			Time0_Init();
			Play_Song(0);			 
		} 
		else if(!(ctrl_port_check()&0x20))
		{ 
			 s1_s2_check();
			 Disp(1,0,16,"ϵͳ���й�����");//��ʾ���ݵ�LCD12864�ӳ���
			 OPT_CHECK = 0xFF;
			 //if((OPT_CHECK&0x7F) != 0x7F)
			 {
  				 if(OPT_CHECK&0x01)
				 {
				   ShowQQChar(0x90,"1:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x90,"1:��",2);
				 }
  				 if(OPT_CHECK&0x02)
				 {
				   ShowQQChar(0x92,"2:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x92,"2:��",2);
				 }
  				 if(OPT_CHECK&0x04)
				 {
				   ShowQQChar(0x94,"3:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x94,"3:��",2);
				 }
  				 if(OPT_CHECK&0x08)
				 {
				   ShowQQChar(0x96,"4:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x96,"4:��",2);
				 }
  				 if(OPT_CHECK&0x10)
				 {
				   ShowQQChar(0x88,"5:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x88,"5:��",2);
				 }
  				 if(OPT_CHECK&0x20)
				 {
				   ShowQQChar(0x8A,"6:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x8A,"6:��",2);
				 }
  				 if(OPT_CHECK&0x40)
				 {
				   ShowQQChar(0x8C,"7:��  ",3);
				 }
				 else
				 {
				   ShowQQChar(0x8C,"7:��  ",3);
				 }				 
				 
				 Delay_xMs(2500);
			}
		} 

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
	 while(!(ctrl_port_check()&0x40))
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
			  while(!(ctrl_port_check()&0x40))
			  {
			     bee_Speak = ~bee_Speak;
			     Delay_xMs(Temp1);
			     if ( Temp2 == Count )
			     {
			        Count = 0;
			        break;
			     }
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

