#ifndef _SING_H_
#define _SING_H_

#include <STC15F2K60S2.h>
#include "sys_cfg.h"


//void playnote(int note, char range);
uchar GetCycle(char , char);
uchar NOTE[] ={
    0x60 ,//C3
    0x56 ,//D3
    0x4C ,//E3
    0x48 ,//F3
    0X40 ,//G3
    0X38 ,//A3
    0x32 ,//B3
    0x30 ,//C4
    0x2b ,//D4
    0x26 ,//E4
    0x24 ,//F4
    0x20 ,//G4
    0x1C ,//A4
    0x19 ,//B4
    0x18 ,//C5
    0x15 ,//D5
    0x13 ,//E5
    0x12 ,//F5
    0x10 ,//G5
    0x0E ,//A5
    0x0D ,//B5
};


#endif