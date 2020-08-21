#include "sys_cfg.h"
#include "display.h"
#include "sys_fun.h"
#include "math.h"

uchar GetCycle(char note, char range){
    switch (note)
    {
    case 0x40: // C
        return floor(956/range);

    case 0x20: // D
        return floor(851/range);
    
    case 0x10: // E
        return floor(758/range);

    case 0x06: // F
        return floor(716/range);
    
    case 0x04: // G
        return floor(637/range);

    case 0x02: // A
        return floor(568/range);

    case 0x01: // B
        return floor(506/range);
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
	Disp(4,1,12,"按任意键返回");
	while(1) {
		KeyIO=0xF0;
		if ((P1&0xf0)!=0xf0) {
			Delay_xMs(100);
			if((KeyIO&0xF0)!=0xF0){
				key = scankey();
				return;
				break;
			}
			Delay_xMs(100);
		}
	 	OPT_CHECK = 0xFF;
		s1_s2_check();
		pitch = 7 + DLED_2*7 - DLED_1*7;
		if (pitch == 14){
			Disp(2,3,4,"高音");
		} else if (pitch == 7){
			Disp(2,3,4,"中音");
		} else if (pitch == 0){
			Disp(2,3,4,"低音");
		}
	 	if (OPT_CHECK&0x01) Playnote(0x01, 7+pitch, 1);
		 else if (OPT_CHECK&0x02) Playnote(0x02, 6+pitch, 1);
		 else if (OPT_CHECK&0x04) Playnote(0x04, 5+pitch, 1);
		 else if (OPT_CHECK&0x08) Playnote(0x08, 4+pitch, 1);
		 else if (OPT_CHECK&0x10) Playnote(0x10, 3+pitch, 1);
		 else if (OPT_CHECK&0x20) Playnote(0x20, 2+pitch, 1);
		 else if (OPT_CHECK&0x40) Playnote(0x40, 1+pitch, 1);	
	}
}