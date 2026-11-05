#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/tu_8034DFF0.h"

cls_804AC9A8::~cls_804AC9A8() {}

// Frame offsets by mode (field_0x238), .rodata at 0x804721C8.
static const f32 lbl_804721C8[3] = {-5.0f, -2.5f, 0.0f};

// FIXME: 73.26 %. After the probe loop the original computes pos - players[nearest]
// (TVec3 operator-: copy of pos, sub, return copy, three stack copies) and never
// reads it; that dead result is missing here (15 instructions), so the stack
// slots after it move. The .sdata2 literal order also differs (-60 and 120 are
// first used in the other order in the original).
void cls_804AC9A8::fn_8034E050() {
    JGeometry::TVec3f players[4];
    u8 taken[4];
    for (s32 i = 0; i < 4; i++) {
        taken[i] = 0;
        players[i] = *getPlayerPos_8023E724(i);
    }
    for (s32 j = 0; j < 2; j++) {
        f32 offX = 120.0f * (f32)j + -60.0f;
        f32 storeX = 16.0f * (f32)j + -8.0f;
        for (s32 k = 0; k < 2; k++) {
            JGeometry::TVec3f probe = mPos;
            probe.x += offX;
            probe.y += 120.0f * (f32)k + -60.0f;
            f32 best = 255.0f;
            s32 nearest = -1;
            for (s32 l = 0; l < 4; l++) {
                if (taken[l] == 0) {
                    f32 dist = PSVECDistance(&probe, &players[l]);
                    if (best > dist) {
                        best = dist;
                        nearest = l;
                    }
                }
            }
            JGeometry::TVec3f pos = mPos;
            pos.x += storeX;
            pos.y += (16.0f * (f32)k + -8.0f) + lbl_804721C8[field_0x238];
            field_0x254[nearest] = pos;
            field_0x254[nearest].z = mPos.z;
            taken[nearest] = 1;
        }
    }
}

// FIXME: 99.91 %. Only the string's .rodata offset differs: the original has
// update()'s table (0x804721D4, update isn't written yet) between lbl_804721C8
// and "enemy_both_sob.bin".
void cls_804AC9A8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -3;
    params.field_0x1c = 0;
    s32 frame = field_0x238 + 0xD;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    fn_80307AC8(mSerial, field_0x4, &mPos, field_0x238 + 0xD, 0, 0, -1);
}

// FIXME: 99.72 %. The zeroing loop's store order differs (the original stores
// each field_0x254 vector as z, y, x, this build x, y, z), and -15/15 sit at other
// .sdata2 offsets: update()'s literals, not written yet, come before them.
void cls_804AC9A8::init() {
    field_0x80.set(-15.0f, -15.0f, 15.0f, 15.0f);
    mBox.set(-15.0f, -15.0f, 15.0f, 15.0f);
    field_0x238 = 0;
    field_0x23c = field_0x238;
    field_0x244 = 0;
    field_0x240 = 0;
    field_0x234 = field_0x90 & 0xF;
    for (s32 i = 0; i < 4; i++) {
        field_0x24c[i] = 0;
        field_0x250[i] = 0;
        field_0x254[i].set(0.0f, 0.0f, 0.0f);
    }
    field_0x230 = 0;
    if (field_0x234 == 1 && vfunc_0x34() == true) {
        field_0x238 = 2;
        field_0x230 = 4;
    }
}
