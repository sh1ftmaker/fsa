#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/J2D/J2DPicture.h"

// The texture name fn_80119D48 loads (.sdata 0x80539F10).
const char* lbl_80539F10 = "kk_black.bti";

inline cls_80119E5C::cls_80119E5C() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x10 = 0;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x2c = NULL;
}

// The accessor of the fade object; fn_80119DD0 is its out-of-line copy (callers elsewhere).
static inline cls_80119E5C* fadeObject() {
    static cls_80119E5C instance;
    return &instance;
}

void cls_801192AC::fn_80118384() {
    getActorManager_801F856C()->fn_801F6884();
    field_0x0 = 0;
}

void cls_801192AC::fn_801183BC() {
    field_0x1 = 0;
    fadeObject()->field_0x1 = 0;
}

void cls_801192AC::fn_80118454() {
    field_0x1 = 1;
    fadeObject()->field_0x1 = 1;
}

bool cls_801192AC::fn_801186B0(u32 value) {
    if (getActorManager_801F856C()->fn_801F68E0()) {
        field_0x0 = 1;
        field_0x1 = 0;
        field_0x2 = 0;
        field_0x4 = value;
        field_0x8 = 0;
        return true;
    }
    return false;
}

// The fade length for a mode: 0x23 for modes 0 and 1, 0x46 otherwise.
static inline s32 fadeFrames(s32 mode) {
    switch (mode) {
    case 0:
    case 1:
        return 0x23;
    case 2:
    default:
        return 0x46;
    }
}

void cls_80119E5C::fn_80119468(s32 g, s32 mode) {
    fn_80119998(fadeFrames(mode), 0, mode, 0, 2, 0, g);
}

void cls_80119E5C::fn_80119958() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x10 = 0;
}

void cls_80119E5C::fn_80119998(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    field_0x0 = 1;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0;
    field_0x8 = a;
    field_0xc = b;
    field_0x1c = c;
    field_0x20 = d;
    field_0x24 = e;
    field_0x28 = g;
    field_0x10 = f;
    if (field_0x10 > 0) {
        field_0x2 = 1;
    } else {
        field_0x2 = 0;
    }
    if (field_0x24 == 2) {
        field_0x8 = fadeFrames(c);
    }
    switch (field_0x1c) {
    case 0:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EC, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3ED, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EE, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EF, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 255.0f;
        field_0x18 = 255.0f / (f32)(s32)(field_0x8 - 1);
        break;
    case 1:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F1, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F2, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F3, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 0.0f;
        field_0x18 = 255.0f / (f32)(s32)(field_0x8 - 1);
        break;
    case 2:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F1, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F2, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F3, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 0.0f;
        field_0x18 = 510.0f / (f32)(s32)(field_0x8 - 1);
        break;
    }
}

// FIXME: 96.9. Regalloc: the original keeps `this` in r30 and the picture in r29, the texture
// in r31; here the constructor result takes an extra move through r0.
void cls_80119E5C::fn_80119D48() {
    if (field_0x2c == NULL) {
        void* texture = gGameRoot_8053AAF8->field_0xa4->getResource('????', lbl_80539F10);
        field_0x2c = new J2DPicture((ResTIMG*)texture);
    }
}

cls_80119E5C* fn_80119DD0() {
    return fadeObject();
}

cls_80119E5C::~cls_80119E5C() {
    if (field_0x2c != NULL) {
        delete field_0x2c;
    }
    field_0x2c = NULL;
}
