#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/DrawShadow.h"
#include "game/cls_804A8588.h"

cls_804A8588::~cls_804A8588() {}

u8 cls_804A8588::fn_802B21AC(s32 layer, const JGeometry::TVec3f* pos) {
    bool result = ActorBase_804A5770::vfunc_0x4C(layer, pos);
    if (!result) {
        field_0x27c = 0;
    }
    return result;
}

// FIXME: 40.17 %. The original builds a SpriteParams_801F5AE0 on the stack (default
// constructor stores at 0x18..0x50) and still passes NULL to fn_801F06F0; with no
// use of the struct MWCC drops those stores (try.py: 61 instructions missing). The
// two float literals of the shadow call and fn_801F06F0 are @NNN vs lbl_ (data split).
void cls_804A8588::fn_802B2728() {
    if (field_0x25c == 0) {
        return;
    }
    fn_801F06F0("enemy_both2_sob.bin", field_0x258 + 0x78, NULL, 1, 1.0f, 1.0f);
    drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 30.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x258 + 0x78, false, 2, -1, 0, 0, 0);
}

// Two entries; field_0x248 indexes it when field_0x240 is 1.
static const f32 lbl_805409C8[2] = {1.0f, -1.0f};

// FIXME: 82.09 %. Only the literal split differs (lbl_80540A0C/0A10/0A14 are @NNN here).
// The original's direction vectors go through stack TVec3f temporaries (stwu frame,
// psq copies); MWCC keeps them in registers when built from a named local, so the
// frame is missing.
void cls_804A8588::init() {
    mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
    field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
    mPos.y += 12.0f;
    field_0x234.set(mPos);
    field_0x250 = 4;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x248 = field_0x90 & 0x7F;
    field_0x24c = (field_0x90 >> 12) & 0x1F;
    field_0x240 = (field_0x90 >> 7) & 0x1F;
    if (field_0x240 == 0) {
        field_0x25c = 0;
    } else {
        field_0x25c = 1;
    }
    field_0x270.set(0.0f, 0.0f, 0.0f);
    field_0x27c = 1;
    if (field_0x240 == 1) {
        field_0x260 = lbl_805409C8[field_0x248];
        field_0x264 = 1.0f;
        field_0x26c = 0.75f;
    } else if (field_0x240 == 2) {
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x80.set(-6.0f, -6.0f, 6.0f, 6.0f);
        switch (field_0x248) {
        case 0:
            field_0x3c.set(1.0f, 0.0f, 0.0f);
            break;
        case 1:
            field_0x3c.set(-1.0f, 0.0f, 0.0f);
            break;
        case 2:
            field_0x3c.set(0.0f, 1.0f, 0.0f);
            break;
        case 3:
            field_0x3c.set(0.0f, -1.0f, 0.0f);
            break;
        }
    }
    field_0x230 = 0;
}
