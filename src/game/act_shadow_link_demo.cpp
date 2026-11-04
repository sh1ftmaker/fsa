#include "game/GameAudio.h"
#include "game/act/act_shadow_link_demo.h"

s32 cls_8049DB90::vfunc_0x58() {
    return field_0x238;
}

bool cls_8049DB90::vfunc_0x5C() {
    switch (field_0x238) {
    case 12:
    case 21:
    case 24:
    case 33:
    case 34:
        return true;
    }
    return false;
}
