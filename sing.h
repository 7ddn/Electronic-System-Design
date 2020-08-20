#ifndef _SING_H_
#define _SING_H_

#include <STC15F2K60S2.h>
#include "sys_cfg.h"


//void playnote(int note, char range);
uchar GetCycle(char , char);
uchar NOTE[] ={
    0x30 ,//mid C
    0x2b ,//mid D
    0x26 ,//mid E
    0x24 ,//mid F
    0x20 ,//mid G
    0x1c ,//mid A
    0x10 ,//mid B
};


#endif