#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"

// FIXME(shotgun): 99 %, reloc-name
void ForceLike_8049C578::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x235 = 0;
    field_0x240 = 0;
    field_0x230 = 0;
    field_0x244 = 0;
    field_0x234 = field_0x90 & 0xF;
    field_0x23c = 1;
    field_0x238 = 0;
    field_0x264 = 0.0f;
    field_0x260 = 0.0f;
    if (field_0x90 & 0x100) {
        field_0x230 = 1;
    }
    if (vfunc_0x3C()) {
        field_0x230 = 4;
    }
    if (field_0x90 & 0x400) {
        field_0x23c = 0;
        mPos.z = 400.0f;
    }
    field_0x24c = 0;
}

// FIXME(shotgun): 100 %, matches
ForceLike_8049C578::~ForceLike_8049C578() {
    if (fn_8022DCD8(field_0x238)) {
        fn_8022DD74(field_0x238);
    }
}
