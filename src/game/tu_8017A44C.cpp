#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/tu_8017A44C.h"

// .rodata: the actor ids fn_8020D6AC skips, ended by 'NULL'.
static const u32 lbl_80466880[5] = {'TOMI', 'BMST', 'IPOT', 'IBBM', 'NULL'};

cls_804A1160::~cls_804A1160() {}

void cls_804A1160::draw() {
    SpriteParams_801F5AE0 params;
    if (field_0x230 == 1 || field_0x230 == 2) {
        if (field_0x240 == 0) {
            return;
        }
        params.field_0x1d = 1;
        params.field_0x30 = 0x11;
    }
    s32 frame = field_0x254 + 0x12d;
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f,
                                              1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x254 + 0x12d, false, 2, -1, 0, 0, 0);
}

// FIXME: 65 %. The original keeps the two tile codes as signed s32s (cmpwi
// against each listed code, the listed codes not merged into ranges, and
// the 0x10..0x17 test as subi + cmplwi 7); ours takes them as u16, merges
// the runs 2-7, 0x27-0x2F and 0x39-0x3B into range tests and emits the
// 0x10..0x17 test as two compares. The mSerial argument is also copied to
// r6 before pos goes to r5.
bool cls_804A1160::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, lbl_80466880) == 1) {
        return true;
    }

    u16 code = fn_8022461C(pos, layer);
    u16 code2 = fn_80226850(pos, layer);
    if ((code >= 0x10 && code <= 0x17) || (code2 >= 0x140 && code2 <= 0x15F)) {
        field_0x238 = 0;
        field_0x234 = 1;
        return false;
    }
    if (code2 == 0x230 || code2 == 0x231 || code2 == 0x234) {
        field_0x234 = 0;
        return false;
    }

    // Tile codes that never block (and go straight to the counter below).
    if (!(code == 0 || code == 4 || code == 5 || code == 6 || code == 7 || code == 2 ||
          code == 3 || code == 0x50 || code == 0xE || code == 0xC || code == 0x27 ||
          code == 0x28 || code == 0x29 || code == 0x2A || code == 0x2B || code == 0x2C ||
          code == 0x2D || code == 0x2E || code == 0x2F || code == 0x20 || code == 0xD ||
          code == 0x39 || code == 0x3A || code == 0x3B || code == 0x4C || code == 0x4D ||
          code == 0x4E || code == 0x4F || code == 0x24)) {
        if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
            field_0x234 = 0;
            return false;
        }
        if (code2 == 0x14) {
            field_0x234 = 0;
            return false;
        }
        if (code == 0x25) {
            if (field_0x234 == 0 && fn_802D6A9C()->field_0x171 == 0) {
                return true;
            }
            field_0x234 = 0;
            return false;
        }
        if (code == 0x26) {
            if (field_0x234 == 0 && fn_802D6A9C()->field_0x171 == 1) {
                return true;
            }
            field_0x234 = 0;
            return false;
        }
        if (field_0x234 == 0) {
            if (code != 0x22 || fn_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
        if (field_0x234 == 1) {
            return false;
        }
    }

    if (field_0x234 == 1) {
        field_0x238 = field_0x238 + 1;
        if (field_0x238 > 0xB) {
            field_0x238 = 0;
            field_0x234 = 0;
        }
    }
    return false;
}

void cls_804A1160::init() {
    field_0x80.i.x = -2.0f;
    field_0x80.i.y = -2.0f;
    field_0x80.f.x = 2.0f;
    field_0x80.f.y = 2.0f;
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;

    // The 0x90 word of the spawner's parameters: bits 4-5 go to 0x98, bits 0-3
    // to the player (0x258) and bits 8-11 to 0x25C.
    field_0x98 = (field_0x90 >> 4) & 3;
    field_0x258 = field_0x90 & 0xF;
    field_0x25c = (field_0x90 >> 8) & 0xF;

    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;

    if (fn_80237060(field_0x258) == 1) {
        mAlive = false;
        return;
    }

    field_0x234 = 0;
    field_0x238 = 0;
    field_0x254 = 0;
    field_0x118 = fn_8023B630(field_0x258);
    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
    field_0x23c = 2;
    field_0x230 = 0;
    fn_801F0E34(0x30026, 0);
}
