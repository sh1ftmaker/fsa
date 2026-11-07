#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"
#include "game/act/act_obj_lift.h"

// The effect shown where the lift breaks or respawns: an emitter on the main layers, a KMRI
// actor from layer 8 up (fn_803513C0 inlines it behind its own layer test).
inline void OBJLift_804ACA00::showBreakEffect(s32 layer) {
    if (layer >= 8) {
        getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 0x83, -1, -1);
        field_0x28c = 1.0f;
    } else {
        spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 2.0f);
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
// FIXME(shotgun): 89.0 %, missing/extra + reloc-name
void OBJLift_804ACA00::init() {
    field_0x258 = (field_0x90 & 0xF) + ((field_0x90 >> 12) & 0xF0);
    field_0x260 = (field_0x90 >> 4) & 0xF;
    field_0x264 = (field_0x90 >> 8) & 0xF;
    field_0x25c = (field_0x90 >> 12) & 0xF;
    field_0x268 = field_0x264;
    field_0x26c = 0;
    field_0x244 = 0;
    field_0x248 = 10;
    field_0x254 = 0;
    field_0x240 = field_0x258;
    field_0x287 = (field_0x90 >> 24) & 1;
    field_0x230 = 0;
    field_0x270 = 0;
    field_0x280 = 0;
    field_0x281 = 1;
    field_0x274 = -1;
    field_0x27c = 0;
    field_0x250 = 0;
    field_0x24c = 0;
    field_0x282 = 0;
    field_0x288 = 0;
    field_0x290 = 0.0f;
    field_0x29c = 0.0f;
    field_0x298 = 0.0f;
    field_0x294 = 0.0f;
    field_0x283[0] = 0;
    field_0x283[1] = 0;
    field_0x283[2] = 0;
    field_0x283[3] = 0;
    field_0x234 = mPos.x;
    field_0x238 = mPos.y;
    field_0x23c = mPos.z;
    switch (field_0x258) {
    case 0:
        field_0x80.i.x = -31.0f;
        field_0x80.i.y = -31.0f;
        field_0x80.f.x = 31.0f;
        field_0x80.f.y = 31.0f;
        break;
    case 1:
        field_0x80.i.x = -15.0f;
        field_0x80.i.y = -15.0f;
        field_0x80.f.x = 15.0f;
        field_0x80.f.y = 15.0f;
        break;
    case 2:
        field_0x80.i.x = -15.0f;
        field_0x80.i.y = -31.0f;
        field_0x80.f.x = 15.0f;
        field_0x80.f.y = 31.0f;
        break;
    case 3:
        field_0x80.i.x = -31.0f;
        field_0x80.i.y = -15.0f;
        field_0x80.f.x = 31.0f;
        field_0x80.f.y = 15.0f;
        break;
    case 4:
        field_0x80.i.x = -31.0f;
        field_0x80.i.y = -31.0f;
        field_0x80.f.x = 31.0f;
        field_0x80.f.y = 31.0f;
        break;
    case 5:
        field_0x80.i.x = -39.0f;
        field_0x80.i.y = -39.0f;
        field_0x80.f.x = 39.0f;
        field_0x80.f.y = 39.0f;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        field_0x80.i.x = -7.0f;
        field_0x80.i.y = -7.0f;
        field_0x80.f.x = 7.0f;
        field_0x80.f.y = 7.0f;
        break;
    case 10: {
        field_0x80.i.x = -7.0f;
        field_0x80.i.y = -7.0f;
        field_0x80.f.x = 7.0f;
        field_0x80.f.y = 7.0f;
        bool free[4];
        free[0] = true;
        free[1] = true;
        free[2] = true;
        free[3] = true;
        s32 taken = 0;
        s32 serial = -1;
        while (taken < 4) {
            serial = getActorManager_801F856C()->fn_801F78F4('OBLF', serial);
            if (serial < 0) {
                break;
            }
            if (serial == mSerial) {
                continue;
            }
            OBJLift_804ACA00* other = static_cast<OBJLift_804ACA00*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (other == NULL) {
                continue;
            }
            if (field_0x4 != other->field_0x4) {
                continue;
            }
            s32 state = other->field_0x258;
            if (state < 6 || state > 9) {
                continue;
            }
            if (free[state - 6] != true) {
                continue;
            }
            taken++;
            free[state - 6] = false;
        }
        if (taken >= 4 || taken == 0) {
            field_0x258 = (s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 6;
        } else {
            s32 list[4];
            s32 n = 0;
            if (free[0] == true) {
                list[n] = 0;
                n++;
            }
            if (free[1] == true) {
                list[n] = 1;
                n++;
            }
            if (free[2] == true) {
                list[n] = 2;
                n++;
            }
            if (free[3] == true) {
                list[n] = 3;
                n++;
            }
            field_0x258 = list[(s32)((f32)n * gGameRoot_8053AAF8->mRandom.get_ufloat_1())] + 6;
        }
        field_0x240 = field_0x258;
        break;
    }
    case 11:
        field_0x80.i.x = -23.0f;
        field_0x80.i.y = -23.0f;
        field_0x80.f.x = 23.0f;
        field_0x80.f.y = 23.0f;
        mBox.i.x = -23.0f;
        mBox.i.y = -23.0f;
        mBox.f.x = 23.0f;
        mBox.f.y = 23.0f;
        field_0x230 = 7;
        break;
    case 12: {
        field_0x80.i.x = -23.0f;
        field_0x80.i.y = -23.0f;
        field_0x80.f.x = 23.0f;
        field_0x80.f.y = 23.0f;
        mBox.i.x = -23.0f;
        mBox.i.y = -23.0f;
        mBox.f.x = 23.0f;
        mBox.f.y = 23.0f;
        field_0x240 = 10;
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
        break;
    }
    case 16:
    case 17:
        field_0x80.i.x = -15.0f;
        field_0x80.i.y = -23.0f;
        field_0x80.f.x = 15.0f;
        field_0x80.f.y = 23.0f;
        field_0x240 = field_0x258 - 3;
        break;
    case 18:
    case 19:
        field_0x80.i.x = -23.0f;
        field_0x80.i.y = -15.0f;
        field_0x80.f.x = 23.0f;
        field_0x80.f.y = 15.0f;
        field_0x240 = field_0x258 - 3;
        break;
    case 20:
        field_0x80.i.x = -15.0f;
        field_0x80.i.y = -23.0f;
        field_0x80.f.x = 15.0f;
        field_0x80.f.y = 23.0f;
        field_0x240 = 14;
        break;
    }
    field_0x28c = 1.0f;
    if (field_0x25c == 2) {
        field_0x230 = 4;
        field_0x28c = 0.0f;
    }
}

// FIXME(shotgun): 99.71 %, reloc-name
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
// FIXME(shotgun): 99.09 %, regalloc-only + reloc-name
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
    f32 dist = JGeometry::TUtil<f32>::sqrt(square(mPos.x - mSpawnPos.x) + square(mPos.y - mSpawnPos.y));
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
// Not auto-inlined: fn_80350974 calls fn_80351020 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 83.88 %, missing/extra + reloc-name
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
        f32 dist = JGeometry::TUtil<f32>::sqrt(v.x * v.x + v.y * v.y);
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
#pragma auto_inline reset

// FIXME(shotgun): 70.4 %, missing/extra + reloc-name
s32 OBJLift_804ACA00::fn_80350974() {
    field_0x283[0] = 0;
    field_0x283[1] = 0;
    field_0x283[2] = 0;
    field_0x283[3] = 0;
    if (field_0x230 == 7) {
        return 0;
    }
    u8 onLift = 0;
    s32 count = 0;
    JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
    for (s32 i = 0; i < 4; i++) {
        if (field_0x258 == 4) {
            JGeometry::TBox2f saved = field_0x80;
            for (s32 j = 0; j < 4; j++) {
                field_0x80 = lbl_8050ADF8[j];
                field_0x283[i] = fn_80351020(i);
                if (field_0x283[i] == 1 && getPlayerPos_8023E724(i)->z <= 0.0f) {
                    count++;
                    break;
                }
            }
            field_0x80 = saved;
        } else if (field_0x258 >= 0x10 && field_0x258 <= 0x14) {
            JGeometry::TBox2f saved = field_0x80;
            for (s32 j = 0; j < 2; j++) {
                field_0x80 = lbl_8050AE38[(field_0x258 - 0x10) * 2 + j];
                field_0x283[i] = fn_80351020(i);
                if (field_0x283[i] == 1 && getPlayerPos_8023E724(i)->z <= 0.0f) {
                    count++;
                    break;
                }
            }
            field_0x80 = saved;
        } else if (field_0x258 >= 6 && field_0x258 <= 9) {
            bool hit = fn_80351020(i);
            field_0x283[i] = hit;
            if (i == field_0x258 - 6) {
                field_0x280 = hit;
            }
        } else {
            field_0x283[i] = fn_80351020(i);
            if (field_0x283[i] == 1 && getPlayerPos_8023E724(i)->z <= 0.0f) {
                count++;
            }
        }
    }

    if (field_0x258 >= 6 && field_0x258 <= 9) {
        push = field_0x3c;
    } else if (field_0x230 == 1 || field_0x230 == 3 ||
               ((field_0x258 == 5 || field_0x258 == 0xC) && PSVECMag(&field_0x3c) > 0.0f)) {
        push = field_0x3c;
    } else if (field_0x230 == 5) {
        push.y = 0.1f;
    } else if (field_0x230 == 6) {
        push.y = -0.1f;
    }

    for (s32 i = 0; i < 4; i++) {
        if (field_0x283[i] == 1 && (fn_80230188(i) || fn_8022CCAC(i))) {
            onLift = 1;
        }
    }

    if (field_0x258 == 5 && count == 0) {
        f32 dx = mPos.x - mSpawnPos.x;
        f32 dy = mPos.y - mSpawnPos.y;
        if (dx * dx + dy * dy > 8.0f) {
            field_0x250++;
            if (field_0x250 > 0xF0) {
                s32 layer = field_0x4;
                if (layer >= 8) {
                    getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 0x83, -1, -1);
                    field_0x28c = 1.0f;
                } else {
                    spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 2.0f);
                }
                mPos.x = mSpawnPos.x;
                mPos.y = mSpawnPos.y;
                mPos.z = mSpawnPos.z;
                field_0x28c = 0.0f;
                layer = field_0x4;
                if (layer >= 8) {
                    getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 0x83, -1, -1);
                    field_0x28c = 1.0f;
                } else {
                    spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 2.0f);
                }
                startSound_801F0E34(0x30003, 0);
            }
        } else {
            field_0x250 = 0;
        }
    } else {
        field_0x250 = 0;
    }
    field_0x288 = onLift;

    if (field_0x289 == 1) {
        for (s32 i = 0; i < 4; i++) {
            if (field_0x283[i] == 0) {
                s32 layer = getPlayerLayer_8023DE58(i);
                if ((u16)fn_8022461C(getPlayerPos_8023E724(i), layer) == 0) {
                    field_0x289 = 0;
                    break;
                }
            }
        }
    }

    for (s32 i = 0; i < 4; i++) {
        if (field_0x283[i] == 1 && getPlayerPos_8023E724(i)->z <= 0.0f && field_0x288 == 0) {
            if (push.x * push.x + push.y * push.y + push.z * push.z > 32.0f * FLT_EPSILON) {
                bool apply = false;
                if (!fn_8022F478(i)) {
                    apply = true;
                } else {
                    if (field_0x283[fn_8022F3DC(i)] == 1 && (fn_8022CAF8(i) == 5 || fn_8022CAF8(i) == 4)) {
                        apply = true;
                    } else if (fn_80235580(i)) {
                        apply = true;
                    }
                }
                if (apply) {
                    if (field_0x289 == 1 && field_0x258 != 5 && field_0x258 != 0xC &&
                        !(field_0x258 >= 6 && field_0x258 <= 9)) {
                        fn_8023DEDC(i, &push);
                    } else {
                        fn_8023DEDC(i, &push);
                        for (s32 k = 0; k < 3; k++) {
                            fn_8022CB94(i);
                        }
                    }
                }
            }
        }
    }
    return count;
}

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
// FIXME(shotgun): 58.4 %, missing/extra + reloc-name
void OBJLift_804ACA00::update() {
    bool present = false;
    if (field_0x230 == 4) {
        if (vfunc_0x34() == 1) {
            showBreakEffect(field_0x4);
            startSound_801F0E34(0x30003, 0);
            fn_801F0D20(0x42);
            field_0x25c = 0;
            if (field_0x258 == 0xB) {
                field_0x230 = 7;
            } else {
                field_0x230 = 0;
            }
        }
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0x4 == getPlayerLayer_8023DE58(i) && !fn_8022F478(i)) {
            present = true;
            break;
        }
    }
    if (field_0x254 > 0) {
        field_0x254--;
        return;
    }
    if (field_0x282 == 1) {
        if (field_0x24c % 40 == 0x13) {
            startSound_801F0E34(0x30068, 0);
        }
        field_0x24c--;
        if (field_0x24c <= 0) {
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
            f32 dx = mPos.x - mSpawnPos.x;
            f32 dy = mPos.y - mSpawnPos.y;
            if (JGeometry::TVec2<f32>(dx, dy).length() > 48.0f) {
                mPos.x = mSpawnPos.x;
                mPos.y = mSpawnPos.y;
                mPos.z = mSpawnPos.z;
                showBreakEffect(field_0x4);
                startSound_801F0E34(0x30003, 0);
            } else {
                mPos.x = mSpawnPos.x;
                mPos.y = mSpawnPos.y;
                mPos.z = mSpawnPos.z;
            }
            if (field_0x4 < 8) {
                field_0x28c = 0.0f;
            }
            return;
        }
    }
    if (field_0x28c < 1.0f) {
        field_0x28c += 0.035f;
        if (field_0x28c >= 1.0f) {
            field_0x28c = 1.0f;
        }
        return;
    }
    field_0x234 = mPos.x;
    field_0x238 = mPos.y;
    field_0x23c = mPos.z;
    field_0x26c = 0;
    field_0x26c = fn_80350974();
    if (field_0x288 == 0 &&
        (field_0x230 == 1 || field_0x230 == 3 ||
         ((field_0x258 == 5 || field_0x258 == 0xC) && PSVECMag(&field_0x3c) > 0.0f))) {
        mPos += field_0x3c;
    }

    switch (field_0x230) {
    case 0: {
        s32 dir;
        if (field_0x260 == 0) {
            dir = -1;
        } else if (field_0x258 == 5) {
            dir = fn_803506E0();
        } else if (field_0x258 == 12) {
            dir = fn_80350530();
        } else if (field_0x258 <= 4 || (field_0x258 >= 16 && field_0x258 <= 20)) {
            if (field_0x281 == 1 && field_0x26c >= 4) {
                if (field_0x25c == 1 && !vfunc_0x34()) {
                    vfunc_0x30(0xFF, true);
                }
                startSound_801F0E34(0x30009, 0);
                field_0x244 = 30;
                field_0x289 = 1;
                field_0x230 = 5;
                dir = field_0x268;
            } else {
                dir = -1;
            }
        } else if (field_0x258 >= 6 && field_0x258 <= 9) {
            if (field_0x281 == 1 && field_0x280 == 1) {
                if (field_0x25c == 1 && !vfunc_0x34()) {
                    vfunc_0x30(0xFF, true);
                }
                startSound_801F0E34(0x30009, 0);
                field_0x230 = 1;
                dir = field_0x268;
            } else {
                dir = -1;
            }
        } else {
            dir = -1;
        }
        fn_8035001C(dir);
        if (field_0x268 != -1) {
            JGeometry::TVec3f saved = mPos;
            mPos += field_0x3c;
            if (fn_803502CC() == 1) {
                field_0x3c.x = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x230 = 0;
                if (field_0x258 == 0xC) {
                    field_0x244 = 0x78;
                    field_0x268 ^= 1;
                } else {
                    field_0x244 = 0;
                }
            }
            mPos = saved;
        } else if (field_0x258 >= 6 && field_0x258 <= 9) {
            if (field_0x280 == 0) {
                field_0x281 = 1;
                field_0x244 = 0;
            }
        } else if (field_0x26c <= 0) {
            field_0x281 = 1;
            field_0x244 = 0;
        }
        break;
    }
    case 1:
    case 3:
        if (fn_803502CC() == 1) {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            mPos.x = field_0x234;
            mPos.y = field_0x238;
            mPos.z = field_0x23c;
            if (field_0x258 <= 4 || (field_0x258 >= 0x10 && field_0x258 <= 0x14)) {
                field_0x230 = 6;
                field_0x281 = 0;
                field_0x244 = 30;
            } else if (field_0x258 == 5) {
                field_0x230 = 0;
                field_0x244 = 0;
                if (present == true && field_0x288 == 0) {
                    startSound_801F0E34(0x30067, 0);
                }
            } else {
                field_0x268 ^= 1;
                field_0x244 = 0x78;
                field_0x281 = 0;
                if (field_0x230 == 1) {
                    field_0x230 = 2;
                } else if (field_0x287 == 1) {
                    field_0x230 = 0;
                } else {
                    field_0x230 = 2;
                }
                if (present == true && field_0x288 == 0) {
                    startSound_801F0E34(0x30067, 0);
                }
            }
        }
        if (present == true && field_0x288 == 0) {
            startSound_801F0E34(0x30066, 0);
        }
        break;
    case 2:
        if (field_0x281 == 1 && field_0x244 == 0) {
            fn_8035001C(field_0x268);
            if (present == true && field_0x288 == 0) {
                startSound_801F0E34(0x30009, 0);
            }
            if (field_0x258 <= 4 || (field_0x258 >= 0x10 && field_0x258 <= 0x14)) {
                field_0x244 = 30;
                field_0x230 = 5;
            } else {
                field_0x230 = 3;
            }
        } else if (field_0x258 >= 6 && field_0x258 <= 9) {
            if (field_0x280 == 0) {
                field_0x281 = 1;
            }
            if (field_0x281 == 1 && field_0x280 == 1) {
                field_0x244 = 0;
            }
        } else {
            if (field_0x26c <= 0) {
                field_0x281 = 1;
            }
            if (field_0x281 == 1 && field_0x26c >= 4) {
                field_0x244 = 0;
            }
        }
        break;
    case 5:
        if (present == true && field_0x288 == 0) {
            startSound_801F0E34(0x30066, 0);
        }
        mPos.y += 0.1f;
        field_0x290 -= 0.1f;
        if (field_0x244 <= 0) {
            if (field_0x268 == field_0x264) {
                field_0x230 = 1;
            } else {
                field_0x230 = 3;
            }
            field_0x244 = 10;
        }
        break;
    case 6:
        mPos.y -= 0.1f;
        field_0x290 += 0.1f;
        if (field_0x244 <= 0) {
            if (field_0x268 == field_0x264) {
                field_0x230 = 2;
            } else if (field_0x287 == 1) {
                field_0x230 = 0;
            } else {
                field_0x230 = 2;
            }
            field_0x244 = 0x78;
            field_0x268 ^= 1;
            if (present == true && field_0x288 == 0) {
                startSound_801F0E34(0x30067, 0);
            }
        }
        if (field_0x26c <= 0) {
            field_0x281 = 1;
        }
        if (field_0x230 == 6 && present == true && field_0x288 == 0) {
            startSound_801F0E34(0x30066, 0);
        }
        break;
    case 7:
        field_0x248--;
        if (field_0x248 <= 0) {
            field_0x248 = 10;
            field_0x240++;
            if (field_0x240 > 12) {
                field_0x240 = 11;
            }
        }
        break;
    }

    if (field_0x244 > 0) {
        field_0x244--;
        if (field_0x244 <= 0 && (field_0x230 == 0 || field_0x230 == 2)) {
            field_0x281 = 1;
        }
    }
    if (present == true && field_0x288 == 0 && (field_0x258 == 5 || field_0x258 == 0xC)) {
        f32 limit = 32.0f * FLT_EPSILON;
        if (field_0x3c.x * field_0x3c.x + field_0x3c.y * field_0x3c.y + field_0x3c.z * field_0x3c.z > limit) {
            startSound_801F0E34(0x30066, 0);
        } else if (field_0x294 * field_0x294 + field_0x298 * field_0x298 + field_0x29c * field_0x29c > limit) {
            startSound_801F0E34(0x30067, 0);
        }
    }
    field_0x294 = field_0x3c.x;
    field_0x298 = field_0x3c.y;
    field_0x29c = field_0x3c.z;
}

// FIXME(shotgun): 84.1 %, missing/extra + reloc-name (state 0x13 rotation is a stand-in)
void OBJLift_804ACA00::draw() {
    s32 alpha = 0xFF;
    if (field_0x230 == 4) {
        return;
    }
    if (field_0x282 == 1 && field_0x24c % 40 < 20) {
        return;
    }
    if (field_0x254 > 0) {
        return;
    }
    if (field_0x28c <= 0.0f) {
        return;
    }
    if (field_0x250 > 0x78) {
        s32 phase = field_0x250 % 40;
        if (field_0x4 < 8) {
            if (phase > 20) {
                alpha = (s32)(255.0f * (((f32)phase - 20.0f) / 20.0f));
            } else {
                alpha = (s32)(255.0f * ((20.0f - (f32)phase) / 20.0f));
            }
        } else if (phase < 20) {
            return;
        }
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    params.field_0x10.a = alpha;
    params.field_0x20 = 0;
    f32 scale = field_0x28c;
    if (field_0x258 == 0xB) {
        f32 big = 1.5f * scale;
        mtx[0][0] = big;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = big;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = big;
    } else if (field_0x258 == 0x11) {
        mtx[0][0] = -scale;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = scale;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = scale;
    } else if (field_0x258 == 0x13) {
        // FIXME: the original builds this rotation from sincosTable_ entries directly.
        JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
        setRotScaleMtx(mtx, rot, scale);
    } else if (field_0x258 == 5) {
        f32 angle = 2.0f * (3.14f * (1.0f - scale));
        JGeometry::TVec3f rot(angle, angle, 0.0f);
        setRotScaleMtx(mtx, rot, scale);
    } else {
        mtx[0][0] = scale;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = scale;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = scale;
    }
    params.mMtx = mtx;
    params.field_0x21 = 1;
    if ((field_0x258 <= 4 && field_0x268 == 2 && (field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 6)) ||
        (field_0x268 == 3 && (((field_0x230 == 1 || field_0x230 == 3) && field_0x244 > 0) || field_0x230 == 5))) {
        params.field_0x1c = -1;
    } else {
        params.field_0x1c = 0;
    }
    params.field_0x1d = -3;
    if (field_0x258 == 0xB) {
        params.field_0x1d = -4;
    }
    drawSprite_801F06F0("enemy_both_sob.bin", field_0x240 + 0x14, &params, -1, 1.0f, 1.0f);
    fn_80307AC8(mSerial, field_0x4, &mPos, field_0x240 + 0x14, 0, 0, -1);
}

OBJLift_804ACA00::~OBJLift_804ACA00() {}
