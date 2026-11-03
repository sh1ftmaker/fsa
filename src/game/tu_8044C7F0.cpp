#include "game/GameAudio.h"
#include "game/cls_804B1D58.h"
#include "game/GameGlobals.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

Unk_80453830::Unk_80453830() {
    field_0x0 = -1;
    field_0x4 = -1;
    field_0x8 = -1;
    field_0xc = -1;
}

u8 cls_804B1D58::fn_8044CB00() {
    return field_0x140;
}

u8 cls_804B1D58::fn_8044D290() {
    return field_0x140;
}

// Stores x, y, z into v and returns it (the caller passes a stack TVec3f and
// uses the returned pointer as the copy source). Like fn_8017EB0C, the original
// keeps it as a plain global function, not a JGeometry member.
extern "C" JGeometry::TVec3f* fn_80453874(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

void cls_804B1D58::fn_804522C8() {
    if (field_0xfa == 7 || field_0xfa == 8 || field_0xfa == 9) {
        fn_804316EC(lbl_8053AEC0);
    }
}

// .sdata2 float, also read by other functions of this unit.
extern const f32 lbl_80543DFC;

void cls_804B1D58::fn_80452308() {
    lbl_8053AB10->fn_8013B490();
    mScreen->draw(lbl_80543DFC, lbl_80543DFC, lbl_8053AB10->field_0x10);
}

s32 fn_8044F664() {
    if (lbl_8053AF40 != nullptr) {
        return lbl_8053AF40->field_0x184;
    }
    return 9;
}
