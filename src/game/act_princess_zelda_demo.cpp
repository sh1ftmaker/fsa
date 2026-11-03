#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_demo.h"

s32 PrincessZeldaDemo_8049D208::vfunc_0x58() {
    return field_0x238;
}

void PrincessZeldaDemo_8049D208::fn_8010F9F0() {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(0x31);
        break;
    case 2:
        fn_8010E2D4(0x31);
        break;
    case 3: // nothing sets this state
        break;
    }
    field_0x238 = 0x31;
}
