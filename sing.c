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