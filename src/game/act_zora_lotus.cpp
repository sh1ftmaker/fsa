#include "game/GameAudio.h"
#include "game/act/act_zora_lotus.h"

cls_804AC3E0::cls_804AC3E0() {
    field_0x2b8 = 0;
}

void cls_804AC3E0::vfunc_0xA4() {
    field_0x2ac = 0;
    field_0x2ad = 0;
    field_0x2ae = 0;
    field_0x2af = 0;
    field_0x2b2 = 0;
    field_0x2b0 = 0;
    mPos.y = mSpawnPos.y;
}

bool cls_804AC3E0::vfunc_0x5C() {
    return false;
}

bool cls_804AC3E0::vfunc_0x60() {
    return false;
}

u8 cls_804AC3E0::vfunc_0x68() {
    return 0;
}

cls_804AC3E0::~cls_804AC3E0() {
}
