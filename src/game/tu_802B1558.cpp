#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"

cls_804A8578::cls_804A8578() {
    lbl_8053ACA8 = this;
    field_0x4 = 0;
    field_0x5 = 0;
    field_0x6 = 0;
    field_0x7 = 0xFF;
}

cls_804A8578::~cls_804A8578() {}

void cls_804A8578::fn_802B1EB0() {}

// Copies the save region to 0x80700000, the area main registers with
// OSSetSaveRegion (see GameState.h), so it survives the reset.
// FIXME: 87.54 %. Same unrolled loop (mtctr 2, eight byte copies twice per
// pass); the original re-adds the index to the GameState base for every
// source pass (add r4, r5, r12), ours steps a source pointer by 0x10, and the
// registers differ.
void cls_804A8578::fn_802B1DE8() {
    lbl_80529DEC.mSaveRegion.f.field_0x9 = 0;
    lbl_80529DEC.mSaveRegion.f.field_0xa = 0;
    field_0x4 = 0;
    field_0x5 = 0;
    for (u32 i = 0; i < sizeof(lbl_80529DEC.mSaveRegion); i++) {
        ((u8*)0x80700000)[i] = lbl_80529DEC.mSaveRegion.bytes[i];
    }
}

bool cls_804A8578::fn_802B1DCC() {
    return lbl_80529DEC.mSaveRegion.f.field_0x9 != 0;
}

bool cls_804A8578::fn_802B1DB0() {
    return lbl_80529DEC.mSaveRegion.f.field_0xa != 0;
}

void cls_804A8578::fn_802B1D98() {
    lbl_80529DEC.mSaveRegion.f.field_0xb = 0;
    field_0x6 = 0;
}

bool cls_804A8578::fn_802B1D7C() {
    return lbl_80529DEC.mSaveRegion.f.field_0xb != 0;
}

// Reads the save-region flags in lbl_80529DEC (SaveRegion at 0x216).
s32 cls_804A8578::fn_802B1D60() {
    u32 index = lbl_80529DEC.mSaveRegion.f.field_0xc;
    if (index > 3) {
        index = -1;
    }
    return index;
}
