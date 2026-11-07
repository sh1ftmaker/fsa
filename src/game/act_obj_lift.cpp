#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameMath.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"
#include "game/act/act_obj_lift.h"

// The effect shown where the lift breaks or respawns: an emitter on the main layers, a KMRI
// actor from layer 8 up (fn_803513C0 inlines it behind its own layer test).
inline void OBJLift_804ACA00::showBreakEffect(s32 layer) {
    if (layer < 8) {
        spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 2.0f);
    } else {
        getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 0x83, -1, -1);
        field_0x28c = 1.0f;
    }
}

// Four strips along the lift's edges, then ten boxes by state (two per state from 16 on).
static JGeometry::TBox2f lbl_8050ADF8[4] = {
    JGeometry::TBox2f(-31.0f, -31.0f, 31.0f, -17.0f),
    JGeometry::TBox2f(-31.0f, 17.0f, 31.0f, 31.0f),
    JGeometry::TBox2f(-31.0f, -31.0f, -17.0f, 31.0f),
    JGeometry::TBox2f(17.0f, -31.0f, 31.0f, 31.0f),
};
static JGeometry::TBox2f lbl_8050AE38[10] = {
    JGeometry::TBox2f(-14.0f, -6.0f, 0.0f, 22.0f), JGeometry::TBox2f(0.0f, -22.0f, 14.0f, 6.0f),
    JGeometry::TBox2f(-14.0f, -22.0f, 0.0f, -8.0f), JGeometry::TBox2f(0.0f, -22.0f, 14.0f, 22.0f),
    JGeometry::TBox2f(-22.0f, -14.0f, 6.0f, 0.0f), JGeometry::TBox2f(-6.0f, 0.0f, 22.0f, 14.0f),
    JGeometry::TBox2f(-14.0f, 8.0f, 0.0f, 22.0f), JGeometry::TBox2f(0.0f, -22.0f, 14.0f, 22.0f),
    JGeometry::TBox2f(-14.0f, -22.0f, 0.0f, 22.0f), JGeometry::TBox2f(0.0f, -22.0f, 14.0f, -8.0f),
};

// 0x803513C0.
// FIXME(shotgun): 74.37 %, different + reloc-name
void OBJLift_804ACA00::fn_803513C0() {
    s32 layer = field_0x4;
    if (layer >= 8) {
        showBreakEffect(layer);
        startSound_801F0E34(0x30003, 0);
    } else {
        spawnEmitter_8013CC50(layer, &mPos, 0x459, 0, -3, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x45A, 0, -3, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x45B, 0, -3, NULL, NULL, true, 1.0f);
    }
    startSound_801F0E34(0x30065, 0);
    field_0x254 = 30;
    field_0x258 = 12;
    field_0x230 = 0;
    field_0x240 = 10;
}

// 0x803511F0.
// FIXME(shotgun): 53.54 %, missing/extra + reloc-name
void OBJLift_804ACA00::fn_803511F0() {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x97, 0, -3, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x98, 0, -3, NULL, NULL, true, 1.0f);
    field_0x258 = 11;
    field_0x282 = 0;
    field_0x24c = 0;
    field_0x230 = 7;
    field_0x240 = 11;
    field_0x248 = 10;
    field_0x274 = -1;
    field_0x268 = field_0x264;
    f32 dist = std::sqrtf(square(mPos.x - mSpawnPos.x) + square(mPos.y - mSpawnPos.y));
    if (dist > 48.0f) {
        mPos = mSpawnPos;
        s32 layer = field_0x4;
        if (layer >= 8) {
            getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 0x83, -1, -1);
            field_0x28c = 1.0f;
        } else {
            spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 2.0f);
        }
        startSound_801F0E34(0x30003, 0);
    } else {
        mPos = mSpawnPos;
    }
    if (field_0x4 < 8) {
        field_0x28c = 0.0f;
    }
}

// 0x80351020.
// FIXME(shotgun): 41.23 %, missing/extra + reloc-name
bool OBJLift_804ACA00::fn_80351020(s32 player) {
    s32 layer = field_0x4;
    if (getPlayerLayer_8023DE58(player) != layer) {
        return false;
    }
    for (s32 i = 0; i < 8; i++) {
        JGeometry::TVec3f v;
        fn_8022E088(player, i, &v);
        v -= mPos;
        s32 angle = (s32)(JMath::atanTable_.atan2_(v.y, v.x) * 10430.378f);
        f32 dist = std::sqrtf(v.x * v.x + v.y * v.y);
        v.z = 0.0f;
        v.x = dist * JMath::sincosTable_.cosShort(angle);
        v.y = dist * JMath::sincosTable_.sinShort(angle);
        if (v.x >= field_0x80.i.x - 2.0f && v.y >= field_0x80.i.y - 2.0f && v.x < field_0x80.f.x + 2.0f &&
            v.y < field_0x80.f.y + 2.0f) {
            fn_8022DF48(player);
            return true;
        }
    }
    return false;
}

// 0x803506E0.
// FIXME(shotgun): 92.65 %, missing/extra + reloc-name
s32 OBJLift_804ACA00::fn_803506E0() {
    static JGeometry::TVec2f lbl_8050AED8[4] = {
        JGeometry::TVec2f(32.0f, 0.0f),
        JGeometry::TVec2f(-32.0f, 0.0f),
        JGeometry::TVec2f(0.0f, 32.0f),
        JGeometry::TVec2f(0.0f, -32.0f),
    };
    s32 prev = field_0x270;
    field_0x270 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022F514(i) == true && fn_8022CAF8(i) == 5) {
            continue;
        }
        for (s32 d = 0; d < 4; d++) {
            f32 px = lbl_8050AED8[d].x + mPos.x;
            f32 py = lbl_8050AED8[d].y + mPos.y;
            s32 bit = 1 << d;
            s32 held = prev & bit;
            f32 limit = 64.0f;
            if (held) {
                limit = square(8.0f + field_0x260);
            }
            if (square(px - getPlayerPos_8023E724(i)->x) + square(py - getPlayerPos_8023E724(i)->y) < limit &&
                getPlayerPos_8023E724(i)->z == 0.0f) {
                if (held == 0) {
                    startSound_801F0E34(0x30009, 0);
                }
                field_0x270 |= bit;
            }
        }
    }
    s32 mask = field_0x270;
    if ((mask & 1) && (mask & 2)) {
        mask &= 0xC;
    }
    if ((mask & 4) && (mask & 8)) {
        mask &= 3;
    }
    if (mask == 1) {
        field_0x268 = 0;
    } else if (mask == 2) {
        field_0x268 = 1;
    } else if (mask == 4) {
        field_0x268 = 2;
    } else if (mask == 8) {
        field_0x268 = 3;
    } else if (mask == 5) {
        field_0x268 = 4;
    } else if (mask == 6) {
        field_0x268 = 5;
    } else if (mask == 9) {
        field_0x268 = 6;
    } else if (mask == 10) {
        field_0x268 = 7;
    } else {
        field_0x268 = -1;
    }
    return field_0x268;
}

// 0x80350530.
// FIXME(shotgun): 89.98 %, missing/extra + reloc-name
s32 OBJLift_804ACA00::fn_80350530() {
    s32 prev = field_0x270;
    field_0x270 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022F514(i) == true && fn_8022CAF8(i) == 5) {
            continue;
        }
        f32 cx = mPos.x;
        f32 cy = mPos.y;
        f32 limit = 64.0f;
        if (prev != 0) {
            limit = square(8.0f + field_0x260);
        }
        if (square(cx - getPlayerPos_8023E724(i)->x) + square(cy - getPlayerPos_8023E724(i)->y) < limit &&
            getPlayerPos_8023E724(i)->z == 0.0f) {
            if (prev == 0) {
                startSound_801F0E34(0x30009, 0);
                field_0x27c = 0;
                field_0x274 = i;
                if (fn_8022F514(i) == true) {
                    field_0x274 = fn_8022F3DC(i);
                }
            }
            field_0x270 = 1;
            if (field_0x244 > 0) {
                return -1;
            }
            return field_0x268;
        }
    }
    field_0x27c = 0;
    field_0x274 = -1;
    field_0x244 = 0;
    return -1;
}

// 0x803502CC.
// FIXME(shotgun): 64.78 %, missing/extra + reloc-name
bool OBJLift_804ACA00::fn_803502CC() {
    f32 savedIX = field_0x80.i.x;
    f32 savedIY = field_0x80.i.y;
    f32 savedFX = field_0x80.f.x;
    f32 savedFY = field_0x80.f.y;
    if (field_0x258 == 4) {
        for (s32 i = 0; i < 4; i++) {
            field_0x80 = lbl_8050ADF8[i];
            if (field_0x268 == 0 || field_0x268 == 1) {
                field_0x80.f.y -= 3.0f;
            } else {
                field_0x80.i.y += field_0x290;
                field_0x80.f.y += field_0x290;
            }
            if (fn_801F2618(NULL, false) == 1) {
                field_0x80.i.x = savedIX;
                field_0x80.i.y = savedIY;
                field_0x80.f.x = savedFX;
                field_0x80.f.y = savedFY;
                return true;
            }
        }
        field_0x80.i.x = savedIX;
                field_0x80.i.y = savedIY;
                field_0x80.f.x = savedFX;
                field_0x80.f.y = savedFY;
        return false;
    } else if (field_0x258 >= 16 && field_0x258 <= 20) {
        for (s32 j = 0; j < 2; j++) {
            field_0x80 = lbl_8050AE38[(field_0x258 - 16) * 2 + j];
            if (field_0x268 == 0 || field_0x268 == 1) {
                field_0x80.f.y -= 3.0f;
            } else {
                field_0x80.i.y += field_0x290;
                field_0x80.f.y += field_0x290;
            }
            if (fn_801F2618(NULL, false) == 1) {
                field_0x80.i.x = savedIX;
                field_0x80.i.y = savedIY;
                field_0x80.f.x = savedFX;
                field_0x80.f.y = savedFY;
                return true;
            }
        }
        field_0x80.i.x = savedIX;
                field_0x80.i.y = savedIY;
                field_0x80.f.x = savedFX;
                field_0x80.f.y = savedFY;
        return false;
    }
    return fn_801F2618(NULL, false);
}

// 0x80350274.
bool OBJLift_804ACA00::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    if (code == 0x3A || code == 0x3 || code == 0xD || code == 0xE || code == 0x3B) {
        return false;
    }
    return true;
}

// 0x8035001C.
// FIXME(shotgun): 62.83 %, missing/extra + reloc-name
void OBJLift_804ACA00::fn_8035001C(u32 dir) {
    switch (dir) {
    case 0:
        field_0x3c.x = 0.5f * field_0x260;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 1:
        field_0x3c.x = 0.5f * -(f32)field_0x260;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 2:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.5f * field_0x260;
        field_0x3c.z = 0.0f;
        break;
    case 3:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.5f * -(f32)field_0x260;
        field_0x3c.z = 0.0f;
        break;
    case 4:
        field_0x3c.x = 0.5f * field_0x260;
        field_0x3c.y = 0.5f * field_0x260;
        field_0x3c.z = 0.0f;
        break;
    case 5:
        field_0x3c.x = 0.5f * -(f32)field_0x260;
        field_0x3c.y = 0.5f * field_0x260;
        field_0x3c.z = 0.0f;
        break;
    case 6:
        field_0x3c.x = 0.5f * field_0x260;
        field_0x3c.y = 0.5f * -(f32)field_0x260;
        field_0x3c.z = 0.0f;
        break;
    case 7:
        field_0x3c.x = 0.5f * -(f32)field_0x260;
        field_0x3c.y = 0.5f * -(f32)field_0x260;
        field_0x3c.z = 0.0f;
        break;
    default:
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        break;
    }
}

// Destructor at 0x8034EDDC.
OBJLift_804ACA00::~OBJLift_804ACA00() {}
