#include "game/GameAudio.h"
#include "game/GameGlobals.h"

// Element constructor of the 4-element array at this+0x140 (fn_8016F0BC).
extern "C" void fn_8016FDE0() {}

// Returns lbl_8053AAF8->field_0x4->field_0x24 (the object act_environment_control_2
// writes flags into).
extern "C" GameRootSub24_8016A3AC* fn_8016A3AC() {
    return lbl_8053AAF8->field_0x4->field_0x24;
}

// Clears eight words of the sub-object at r3 (the caller passes this+0x90 of
// fn_803A5380's object, tu_803A3484; that class is not written yet).
extern "C" void fn_8016ED88(s32* p) {
    p[0x0 / 4] = 0;
    p[0x14 / 4] = 0;
    p[0x1C / 4] = 0;
    p[0x30 / 4] = 0;
    p[0x38 / 4] = 0;
    p[0x4C / 4] = 0;
    p[0x54 / 4] = 0;
    p[0x68 / 4] = 0;
}
