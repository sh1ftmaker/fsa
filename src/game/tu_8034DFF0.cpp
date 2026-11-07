#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/tu_8034DFF0.h"

// Frame offsets by mode (field_0x238), .rodata at 0x804721C8.
static const f32 lbl_804721C8[3] = {-5.0f, -2.5f, 0.0f};
// Frame counts of the pose steps (field_0x248), .rodata at 0x804721D4.
static const s32 lbl_804721D4[15] = {30, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 10, 1};


// FIXME: 99.72 %. The zeroing loop's store order differs (the original stores
// each field_0x254 vector as z, y, x, this build x, y, z).
// 0x8034EAB4.
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

// 0x8034E49C.
// FIXME(shotgun): 90.40 %, missing/extra
void cls_804AC9A8::update() {
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x, mPos.y);
    box.i.y += -2.5f;
    box.f.y += -2.5f;
    s32 count = 0;
    switch (field_0x230) {
    case 0: {
        for (s32 i = 0; i < 4; i++) {
            field_0x250[i] = field_0x24c[i];
            if (fn_8023F264(i) && !fn_8023EA28(i)) {
                field_0x24c[i] = fn_8023B0F4(i, &box, field_0x4, false, 4.0f);
                if (field_0x24c[i] == 1) {
                    count++;
                }
            }
        }
        bool leader = false;
        if (gGameRoot_8053AAF8->fn_80138A30() != 1) {
            leader = true;
        } else if (fn_8022CAF8(0) == 2) {
            leader = true;
        }
        switch (count) {
        case 0:
            field_0x238 = 0;
            field_0x244 = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (field_0x23c == 0) {
                startSound_801F0E34(0x5006E, 0);
            }
            field_0x238 = 1;
            field_0x244 = 0;
            break;
        case 4: {
            bool wait = true;
            if (field_0x240 != count) {
                if (field_0x23c == 0) {
                    startSound_801F0E34(0x5006E, 0);
                }
                field_0x238 = 1;
                field_0x244 = 5;
            } else if (field_0x244 <= 0 && !vfunc_0x34()) {
                wait = false;
                if (fn_8023077C()) {
                    JGeometry::TVec3f shift(0.0f, 0.0f, 0.0f);
                    shift.y = lbl_804721C8[field_0x238];
                    fn_8034E050();
                    for (s32 i = 0; i < 4; i++) {
                        fn_80230498(i, &field_0x254[i], 0xE8, 0xFF, -128, -128, true);
                        fn_8022FFC4(i, &shift);
                    }
                    field_0x230 = 2;
                    field_0x244 = 15;
                }
            }
            if (wait && leader == true && field_0x244 > 0) {
                field_0x244--;
            }
            break;
        }
        }
        if (field_0x234 == 0 && field_0x240 == 4 && count < 4) {
            vfunc_0x30(0xFF, false);
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && !fn_8023EA28(i)) {
                JGeometry::TVec3f v(0.0f, 0.0f, 0.0f);
                if (field_0x24c[i] == 1) {
                    v.y = lbl_804721C8[field_0x238];
                }
                if (0.0f != lbl_804721C8[field_0x238] || field_0x250[i] != field_0x24c[i]) {
                    fn_8022FFC4(i, &v);
                }
            }
        }
        field_0x240 = count;
        break;
    }
    case 2:
        if (field_0x244 <= 0) {
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                fn_80230498(i, &p, 0xE8, 0xFF, -128, -128, true);
            }
            startSound_801F0E34(0x30009, 0);
            field_0x238 = 2;
            field_0x248 = 0;
            field_0x244 = 30;
            field_0x230 = 1;
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f v(0.0f, 0.0f, 0.0f);
            v.y = lbl_804721C8[field_0x238];
            fn_8022FFC4(i, &v);
        }
        field_0x244--;
        break;
    case 1:
        field_0x244--;
        if (field_0x244 == 0) {
            if (field_0x248 >= 13) {
                fn_801F0D20(3);
                vfunc_0x30(0xFF, true);
                field_0x244 = 15;
                field_0x230 = 3;
            } else {
                field_0x248++;
                field_0x244 = lbl_804721D4[field_0x248];
                for (s32 i = 0; i < 4; i++) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    fn_80230498(i, &p, field_0x248 + 0xDB, 0xFF, -128, -128, true);
                    if (field_0x248 == 1) {
                        fn_80230FD0(i, 0x10001);
                    }
                }
            }
        }
        break;
    case 3:
        field_0x244--;
        if (field_0x244 < 0 && fn_80230534(false)) {
            field_0x244 = 0;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f v(0.0f, 0.0f, 0.0f);
                fn_8022FFC4(i, &v);
            }
            if (field_0x234 == 1) {
                field_0x230 = 4;
            } else {
                field_0x230 = 0;
            }
        }
        break;
    case 4:
        break;
    }
    field_0x23c = field_0x238;
}

// 0x8034E2F0.
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

// Destructor at 0x8034DFF0.
cls_804AC9A8::~cls_804AC9A8() {}
