#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/tu_80362634.h"

static const s32 lbl_80474B88[6] = {7, 10, 10, 7, 10, 10};
static const s32 lbl_80474BA0[6] = {0, 1, 2, 0, 3, 4};

void cls_804ADF28::fn_80362634() {
    deleteEmitter(&field_0x300);
}

// FIXME: 99.6. Instructions identical; only the relocation of the "fune.bti" string is @NNN here
// against lbl_80474C10 (the unit's rodata is not complete yet).
void cls_804ADF28::fn_80362B14() {
    void* res = fn_804032E8('TIMG', "fune.bti");
    if (field_0x4 < 8) {
        lbl_8053AB10->field_0x20->fn_802F745C(&mPos, res, NULL);
    }
}

void cls_804ADF28::fn_8036335C() {}

void cls_804ADF28::fn_80363F7C() {
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x268 >= 6) {
            field_0x268 = 0;
        }
        field_0x25c = lbl_80474B88[field_0x268];
        field_0x264 = lbl_80474BA0[field_0x268];
    }
    mPos.add(field_0x3c);
}

void cls_804ADF28::fn_80364F00() {
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x270[field_0x268] < 0) {
            field_0x268 = 0;
        }
        field_0x25c = field_0x270[field_0x268];
        field_0x264 = field_0x274[field_0x268];
    }
}

// FIXME: 99.5. Instructions identical; only the relocation of the 0.03f literal is @NNN here
// against lbl_80542BD4 (the unit's sdata2 is not complete yet).
void cls_804ADF28::fn_80365220() {
    if (field_0x248 == 0) {
        field_0x248 = 0x40;
        field_0x238++;
    }
    if ((field_0x238 & 1) == 0) {
        mPos.y += 0.03f;
    } else {
        mPos.y -= 0.03f;
    }
}
