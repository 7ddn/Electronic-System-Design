#include "sys_cfg.h"
#include "display.h"
#include "sys_fun.h"

void playnote(int note, char range){
    bee_Speak = 0;
    TMOD = 0x10;
    ET1 = 1;
    EA = 1;
    TR1 = 1;
    

}