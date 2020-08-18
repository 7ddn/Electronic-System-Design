/***********************************************
基于STC15F2K60S2系列单片机C语言编程实现
使用如下头文件，不用另外再包含"REG51.H"
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
	
	Ini_Lcd();//液晶初始化子程序
	Disp(1,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
	Disp(2,1,12,"电子线路设计");//显示数据到LCD12864子程序
	Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
	Disp(4,1,12,"液晶显示成功");//显示数据到LCD12864子程序
	while(1)
	{
		

		
		if((ctrl_port_check()&0xF0)==0xF0)
		{  	
			s1_s2_check();
			P4M1=0x00;
			P4M0=0x00;
			Ini_Lcd();//液晶初始化子程序
			Disp(1,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
			Disp(2,1,12,"电子线路设计");//显示数据到LCD12864子程序
			Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
			Disp(4,1,12,"液晶显示成功");//显示数据到LCD12864子程序
			Delay_xMs(2500);		
		}
		
		if(!(ctrl_port_check()&0x80))
		{ 
			KeyIO=0xF0;
			Ini_Lcd();//液晶初始化子程序
			Disp(1,0,16,"系统进行按键检测");//显示数据到LCD12864子程序
			Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
			Disp(4,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
			while(!(ctrl_port_check()&0x80))
			{
				s1_s2_check();				
				if((P1&0xf0)!=0xf0) //如果有键按下
		        {
		            Delay_xMs(100);   //延时去抖动
		            if((KeyIO&0xf0)!=0xf0)   //再判断
		            {
						key=scankey();
						switch(key)
						{
							case 11:Disp(2,1,12,"KEY_VALUE:01");	break;//显示数据到LCD12864子程序
							case 12:Disp(2,1,12,"KEY_VALUE:02");	break;//显示数据到LCD12864子程序
							case 13:Disp(2,1,12,"KEY_VALUE:03");	break;//显示数据到LCD12864子程序
							case 14:Disp(2,1,12,"KEY_VALUE:04");	break;//显示数据到LCD12864子程序
							case 21:Disp(2,1,12,"KEY_VALUE:05");	break;//显示数据到LCD12864子程序
							case 22:Disp(2,1,12,"KEY_VALUE:06");	break;//显示数据到LCD12864子程序
							case 23:Disp(2,1,12,"KEY_VALUE:07");	break;//显示数据到LCD12864子程序
							case 24:Disp(2,1,12,"KEY_VALUE:08");	break;//显示数据到LCD12864子程序
							case 31:Disp(2,1,12,"KEY_VALUE:09");	break;//显示数据到LCD12864子程序
							case 32:Disp(2,1,12,"KEY_VALUE:10");	break;//显示数据到LCD12864子程序
							case 33:Disp(2,1,12,"KEY_VALUE:11");	break;//显示数据到LCD12864子程序
							case 34:Disp(2,1,12,"KEY_VALUE:12");	break;//显示数据到LCD12864子程序
							case 41:Disp(2,1,12,"KEY_VALUE:13");	break;//显示数据到LCD12864子程序
							case 42:Disp(2,1,12,"KEY_VALUE:14");	break;//显示数据到LCD12864子程序
							case 43:Disp(2,1,12,"KEY_VALUE:15");	break;//显示数据到LCD12864子程序
							case 44:Disp(2,1,12,"KEY_VALUE:16");	break;//显示数据到LCD12864子程序
						}
					}   
				}
			}
		}
		else if(!(ctrl_port_check()&0x40))
		{ 
			s1_s2_check();
			Ini_Lcd();//液晶初始化子程序
			Disp(1,0,16,"系统进行音乐播放");//显示数据到LCD12864子程序
			Disp(2,1,13,"Playing Music");//显示数据到LCD12864子程序  
			Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
			Disp(4,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序		
			Time0_Init();
			Play_Song(0);			 
		} 
		else if(!(ctrl_port_check()&0x20))
		{ 
			 s1_s2_check();
			 Disp(1,0,16,"系统进行光耦检测");//显示数据到LCD12864子程序
			 OPT_CHECK = 0xFF;
			 //if((OPT_CHECK&0x7F) != 0x7F)
			 {
  				 if(OPT_CHECK&0x01)
				 {
				   ShowQQChar(0x90,"1:高",2);
				 }
				 else
				 {
				   ShowQQChar(0x90,"1:低",2);
				 }
  				 if(OPT_CHECK&0x02)
				 {
				   ShowQQChar(0x92,"2:高",2);
				 }
				 else
				 {
				   ShowQQChar(0x92,"2:低",2);
				 }
  				 if(OPT_CHECK&0x04)
				 {
				   ShowQQChar(0x94,"3:高",2);
				 }
				 else
				 {
				   ShowQQChar(0x94,"3:低",2);
				 }
  				 if(OPT_CHECK&0x08)
				 {
				   ShowQQChar(0x96,"4:高",2);
				 }
				 else
				 {
				   ShowQQChar(0x96,"4:低",2);
				 }
  				 if(OPT_CHECK&0x10)
				 {
				   ShowQQChar(0x88,"5:高",2);
				 }
				 else
				 {
				   ShowQQChar(0x88,"5:低",2);
				 }
  				 if(OPT_CHECK&0x20)
				 {
				   ShowQQChar(0x8A,"6:高",2);
				 }
				 else
				 {
				   ShowQQChar(0x8A,"6:低",2);
				 }
  				 if(OPT_CHECK&0x40)
				 {
				   ShowQQChar(0x8C,"7:高  ",3);
				 }
				 else
				 {
				   ShowQQChar(0x8C,"7:低  ",3);
				 }				 
				 
				 Delay_xMs(2500);
				 
				 
/*
				 if(!(OPT_CHECK&0x01))
				 {
				   ShowQQChar(0x88,"01号光耦为低电平",8);
				 }
				 if(!(OPT_CHECK&0x02))
				 {
				   ShowQQChar(0x88,"02号光耦为低电平",8);
				 }
				 if(!(OPT_CHECK&0x04))
				 {
				   ShowQQChar(0x88,"03号光耦为低电平",8);
				 }
				 if(!(OPT_CHECK&0x08))
				 {
				   ShowQQChar(0x88,"04号光耦为低电平",8);
				 }
				 if(!(OPT_CHECK&0x10))
				 {
				   ShowQQChar(0x88,"05号光耦为低电平",8);
				 }
				 if(!(OPT_CHECK&0x20))
				 {
				   ShowQQChar(0x88,"06号光耦为低电平",8);
				 }
				 if(!(OPT_CHECK&0x40))
				 {
				   ShowQQChar(0x88,"07号光耦为低电平",8);
				 }
*/
				 Delay_xMs(250);
			}
		} 

	}
}

/*******************定时器初始化********************/
void Time0_Init()
{
 TMOD = 0x01;
 IE   = 0x82;
 TH0  = 0xDC;
 TL0  = 0x00;  //11.0592MZ
}
/****************播放音乐*************************/
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

/****************播放音乐*************************/
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
	bee_Speak = 0;	//关闭蜂鸣器
}

/*****************检测控制端口的状态*****************/
uchar ctrl_port_check(void)
{
		SW_CTRL = 0xFF;
		return 	SW_CTRL&0xF0;
}

uchar scankey(void)//矩阵键盘翻转扫描
{
	uint keyvalue=0;
	uchar temp1,temp2,keycode;

	KeyIO=0xf0;		//行置0列置1
	temp1=KeyIO&0xf0;
	if((temp1&0xf0)==0xf0) return(0);  //若无键按下返回0
	KeyIO=0x0f;
	temp2=KeyIO&0x0f;					   //若有键按下，行置1列置0
	keycode=~(temp1|temp2);			   //获得键盘码
	switch(keycode)					   //根据键盘码返回键值
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
	 
/****************延迟毫秒数************************/
void Delay_xMs(unsigned int x)
{
    unsigned int i,j;
    for( i =0;i < x;i++ )
    {
        for( j =0;j<2;j++ );
    }
}

