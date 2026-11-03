#include "game/GameAudio.h"
#include "game/GameGlobals.h"

// Element constructor of the 4-element array at this+0x140 (fn_8016F0BC).
extern "C" void fn_8016FDE0() {}

// Returns lbl_8053AAF8->field_0x4->field_0x24 (the object act_environment_control_2
// writes flags into).
extern "C" GameRootSub24_8016A3AC* fn_8016A3AC() {
    return lbl_8053AAF8->field_0x4->field_0x24;
}
