/***********************************************
基于STC15F2K60S2系列单片机C语言编程实现
使用如下头文件，不用另外再包含"REG51.H"
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
	
	Ini_Lcd();//液晶初始化子程序
	Disp(1,0,16,"浙江大学光电学院");//显示数据到LCD12864子程序			
	Disp(2,1,12,"电子线路设计");//显示数据到LCD12864子程序
	Disp(3,2,8,"87951197");//显示数据到LCD12864子程序
	Disp(4,1,12,"液晶显示成功");//显示数据到LCD12864子程序*/
	while(1)
	{
		s1_s2_check();
		P4M1=0x00;
		P4M0=0x00;
		Ini_Lcd();//液晶初始化子程序
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
    for( i = 0;i < x;i++ )
    {
        for( j = 0;j<2;j++ );
    }
}

// 菜单功能函数

void MenuDisplay(int page){
	Ini_Lcd();
	switch (page)
	{
	case 1:
		Disp(1,3,4,"菜单");
		Disp(2,0,10,"1.播放音乐");
		Disp(3,0,10,"2.弹奏模式");
		Disp(4,0,8,"3.下一页");
		break;
	case 2:
		Disp(1,3,4,"菜单");
		Disp(2,0,10,"4.录音模式");
		Disp(3,0,10,"5.播放录音");
		Disp(4,0,8,"6.上一页");
	default:
		break;
	}

}

void PlayMusic(){
	Ini_Lcd();
	Disp(1,0,8,"1.新宝岛");
	Disp(2,0,6,"2.爱河");
	Disp(3,0,10,"3.祝你平安");
	Disp(4,0,10,"4.天空之城");

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
					Disp(1,0,8,"1.新宝岛");
					Play_Song(0);
					return;
					break;
				case 12:
					Ini_Lcd();
					Disp(1,0,6,"2.爱河");
					Play_Song(1);
					return;
					break;
				case 13:
					Ini_Lcd();
					Disp(1,0,10,"3.祝你平安");
					Play_Song(2);
					return;
					break;
				case 14:
					Ini_Lcd();
					Disp(1,0,10,"4.天空之城");
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
	Disp(1,2,8,"演奏模式");
	if (pitch!=0) Disp(2,0,10,"2.降低音高");
	if (pitch!=14) Disp(3,0,10,"3.提高音高");
	Disp(4,1,12,"按任意键返回");
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
						if (pitch==0) Disp(2,0,16,"音高已经是最低了");
						Disp(3,0,10,"3.提高音高");
						pitchFlag = 0;
						break;
					case 13:
						if (!pitchFlag) break;
						if (pitch<14) pitch+=7;
						if (pitch==14) Disp(3,0,16,"音高已经是最高了");
						Disp(2,0,10,"2.降低音高");
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

// 录制

void Record(){
	char continueFlag = 0;
	int currentIndex = 0;
	lastEndCount = 0;
	lastStartCount = -1;
	lastNote = 0x00;

	s1_s2_check();
	Ini_Lcd();
	Disp(1,2,8,"录音模式");
	Disp(2,0,10,"1.开始录音");
	
	Disp(4,1,12,"按其他键返回");
	
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
	Disp(2,0,6,"录音中");
	Disp(3,0,10,"2.结束录音");
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
						Disp(3,0,8,"录音完成");
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

//音符录制

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

// 录音播放

void PlayRecord()
{
	unsigned char Temp1,Temp2;
	unsigned int Addr;

	s1_s2_check();
	Ini_Lcd();
	Disp(1,2,8,"播放录音");
	Disp(4,1,12,"按任意键返回");

	     
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
	bee_Speak = 0;	//关闭蜂鸣器
}

// 简谱生成

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