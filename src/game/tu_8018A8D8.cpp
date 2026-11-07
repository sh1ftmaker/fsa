// The actor at vtable 0x804A1E50 (include/game/cls_804A1E50.h) and the
// helpers its unit shares.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/cls_804A1E50.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// Animation lengths in frames (.data 0x804A1B70, not written as source here), indexed by
// field_0x270 + field_0x264.
extern const s32 lbl_804A1B70[];
extern JGeometry::TVec3f lbl_804C18B8;
extern s32 lbl_80539FC8;
extern s32 lbl_80539FCC;
extern s32 lbl_80539FD0;

static inline void stopEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        emitter->becomeInvalidEmitter();
        emitter = NULL;
    }
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8018E964(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

bool fn_8018E930(s32 bit) {
    return getGameView_802D6A9C()->fn_802D800C(bit);
}

void fn_8018E8DC(s32 bit, bool set, u8 value) {
    getGameView_802D6A9C()->fn_802D8050(bit, set, value);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8018E8C0(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

s32 fn_8018E89C() {
    return gGameRoot_8053AAF8->fn_80138A30();
}

// Sets field_0x234.
void cls_804A1E50::fn_8018E894(s32 value) {
    field_0x234 = value;
}

// FIXME(shotgun): 92.51 %, different + reloc-name.
void cls_804A1E50::init() {
    mBox2.i.x = -8.0f;
    mBox2.i.y = -8.0f;
    mBox2.f.x = 8.0f;
    mBox2.f.y = 8.0f;
    field_0x26c = field_0x90 & 1;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x2e8[i] = 0;
        field_0x2f8[i].set(mPos);
    }
    lbl_804C18B8.set(0.0f, 0.0f, 0.0f);
    lbl_80539FC8 = -1;
    field_0x270 = 0;
    field_0x264 = 0;
    field_0x280 = 0.0f;
    field_0x288 = 0.0f;
    field_0x268 = 0;
    field_0x27b = 0;
    field_0x29c = 0;
    field_0x2b4 = 0.0f;
    field_0x2b0 = 0.0f;
    field_0x2ac = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x2a0.set(mPos);
    field_0x2c4[0].set(mPos);
    field_0x2c4[1].set(mPos);
    field_0x2c4[2].set(mPos);
    field_0x254 = -1;
    field_0x196 = 1;
    field_0x294 = 1.0f;
    field_0x298 = 1.0f;
    field_0x2b8 = 1.0f;
    field_0x2bc = 1.0f;
    field_0x2c0 = 3.0f;
    field_0x27a = 0;
    field_0x28c = 0.0f;
    field_0x284 = 100.0f;
    field_0x290 = 0.0f;
    field_0x274 = 0x300;
    field_0x328 = NULL;
    field_0x32c = NULL;
    field_0x330 = NULL;
    field_0x27c = -1;
    field_0x278 = 0;
    if (field_0x26c == 0) {
        lbl_80539FCC = -1;
        lbl_80539FD0 = 9;
        field_0x2a0.y += 24.0f;
        getGameView_802D6A9C()->fn_802D7440(false);
        mBox.i.x = -50.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 42.0f;
        mBox.f.y = 40.0f;
        gAudioMgr_8053AF80->fn_80458F9C(60);
        mState = 0;
    } else {
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0xb0 = 0x5040004;
        lbl_80539FC8 = field_0x4;
        lbl_804C18B8.set(field_0x2a0.x, field_0x2a0.y, field_0x2a0.z);
        mState = 0x1A;
    }
}

// FIXME(shotgun): 97.65 %, missing/extra + reloc-name.
void cls_804A1E50::fn_8018E30C(s32 anim) {
    JGeometry::TVec3f pos(mPos.x - 3.0f, mPos.y, mPos.z);
    if (field_0x26c == 1) {
        return;
    }
    if (anim < 0) {
        return;
    }
    if (field_0x268 == anim) {
        return;
    }
    field_0x268 = anim;
    field_0x27a = 0;
    field_0x234 = 0;
    field_0x264 = 0;
    switch (field_0x268) {
    case 0:
        field_0x270 = 0;
        break;
    case 1:
    case 10:
        startSound_801F0E34(0x4004C, 0);
        field_0x270 = 6;
        break;
    case 2:
        if (field_0x328 != NULL) {
            field_0x328->becomeInvalidEmitter();
        }
        if (field_0x32c != NULL) {
            field_0x32c->becomeInvalidEmitter();
        }
        pos.y += 44.0f;
        field_0x328 = spawnEmitter_8013CC50(field_0x4, &pos, 0x54, 1, 2, NULL, NULL, true, 1.0f);
        field_0x32c = spawnEmitter_8013CC50(field_0x4, &pos, 0x55, 1, 2, NULL, NULL, true, 1.0f);
        field_0x270 = 0xE;
        break;
    case 3:
    case 5:
    case 11:
        field_0x270 = 0x12;
        break;
    case 4:
    case 6:
        if (field_0x328 != NULL) {
            field_0x328->becomeInvalidEmitter();
        }
        if (field_0x32c != NULL) {
            field_0x32c->becomeInvalidEmitter();
        }
        pos.y += 60.0f;
        if (field_0x268 == 4) {
            field_0x328 = spawnEmitter_8013CC50(field_0x4, &pos, 0x52, 1, 2, NULL, NULL, true, 1.0f);
            field_0x32c = NULL;
        } else {
            field_0x328 = spawnEmitter_8013CC50(field_0x4, &pos, 0x50, 1, 2, NULL, NULL, true, 1.0f);
            field_0x32c = spawnEmitter_8013CC50(field_0x4, &pos, 0x51, 1, 2, NULL, NULL, true, 1.0f);
        }
        field_0x270 = 0x1A;
        break;
    case 7:
    case 9:
        field_0x270 = 0x1E;
        break;
    case 8:
        field_0x270 = 0x34;
        break;
    case 12:
        field_0x270 = 0x3B;
        break;
    case 13:
        field_0x270 = 0x47;
        break;
    }
    field_0x234 = lbl_804A1B70[field_0x270 + field_0x264];
}

// FIXME(shotgun): 94.02 %, scheduling+regalloc + reloc-name.
void cls_804A1E50::fn_8018DFCC() {
    f32 x = mPos.x - 3.0f;
    f32 y = mPos.y;
    f32 z = mPos.z;
    if (field_0x26c == 1) {
        return;
    }
    if (field_0x268 == 4 || field_0x268 == 6 || field_0x268 == 11) {
        startSound_801F0E34(0x40049, 0);
    }
    if (field_0x268 == 2) {
        startSound_801F0E34(0x4004B, 0);
    }
    if (field_0x234 != 0) {
        return;
    }
    switch (field_0x268) {
    case 0:
        field_0x264++;
        if (field_0x264 > 5) {
            field_0x264 = 0;
        }
        break;
    case 1:
    case 10:
        field_0x264++;
        if (field_0x264 > 7) {
            if (field_0x268 == 1) {
                fn_8018E30C(2);
            } else {
                field_0x264 = 7;
            }
            field_0x27a = 1;
        }
        break;
    case 2:
        y += 44.0f;
        if (field_0x328 != NULL) {
            field_0x328->mGlobalTrs.set(x, y, z);
        }
        if (field_0x32c != NULL) {
            field_0x32c->mGlobalTrs.set(x, y, z);
        }
        field_0x264++;
        field_0x264 &= 3;
        break;
    case 3:
    case 5:
    case 11:
        field_0x264++;
        if (field_0x264 > 7) {
            if (field_0x268 == 3) {
                fn_8018E30C(4);
            } else if (field_0x268 == 5) {
                fn_8018E30C(6);
            } else {
                field_0x264 = 7;
            }
            field_0x27a = 1;
        }
        break;
    case 4:
    case 6:
        y += 60.0f;
        if (field_0x328 != NULL) {
            field_0x328->mGlobalTrs.set(x, y, z);
        }
        if (field_0x32c != NULL) {
            field_0x32c->mGlobalTrs.set(x, y, z);
        }
        field_0x264++;
        field_0x264 &= 3;
        break;
    case 7:
    case 9:
        field_0x264++;
        if (field_0x264 > 21) {
            if (field_0x268 == 7) {
                fn_8018E30C(8);
            } else {
                field_0x264 = 21;
            }
            field_0x27a = 1;
        }
        break;
    case 8:
        field_0x264++;
        if (field_0x264 > 5) {
            field_0x264 = 0;
        }
        break;
    case 12:
        field_0x264++;
        if (field_0x264 > 11) {
            fn_8018E30C(13);
            field_0x27a = 1;
        }
        break;
    case 13:
        field_0x264++;
        field_0x264 &= 3;
        break;
    }
    field_0x234 = lbl_804A1B70[field_0x270 + field_0x264];
}

// FIXME(shotgun): 91.80 %, missing/extra + reloc-name.
void cls_804A1E50::fn_8018DC70(s32 mode) {
    if (field_0x26c == 1 || field_0x27a != 1) {
        return;
    }
    JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
    if (field_0x254 >= 0) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (!fn_8023F264(i)) {
            continue;
        }
        if (field_0x4 != getPlayerLayer_8023DE58(i)) {
            continue;
        }
        if (fn_80230188(i)) {
            continue;
        }
        if (mode != 3 && !(getPlayerPos_8023E724(i)->y > mPos.y)) {
            continue;
        }
        f32 py = mPos.y - 32.0f;
        f32 px = mPos.x;
        f32 dx = getPlayerPos_8023E724(i)->x - px;
        f32 dy = getPlayerPos_8023E724(i)->y - py;
        s32 angle = (u16)(s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        if (mode != 1 && angle >= 0x2000 && angle < 0xE000) {
            continue;
        }
        if (mode == 3) {
            angle += 0x8000;
            push.x = 3.5f;
            push.y = 3.5f;
        } else if (mode == 1) {
            push.x = 2.0f;
            push.y = 2.0f;
        } else {
            push.x = 3.0f;
            push.y = 3.0f;
        }
        if (mode == 0 || mode == 1) {
            ActorBase_804A5770* fires[ACTOR_MANAGER_MAX_ACTORS];
            s32 count = getActorManager_801F856C()->fn_801F7818('FIRE', field_0x4, fires, -1);
            for (s32 j = 0; j < count; j++) {
                ActorBase_804A5770* fire = fires[j];
                if (fire != NULL && ((fire->field_0x90 >> 6) & 3) == 0) {
                    if (std::fabsf(fire->mPos.x - mPos.x) < 80.0f && mPos.y - 16.0f < fire->mPos.y &&
                        std::fabsf(fire->mPos.y - mPos.y) < 120.0f) {
                        fire->vfunc_0x24();
                    }
                }
            }
        }
        if (mode != 1) {
            push.x = push.x - 0.03f * std::fabsf(getPlayerPos_8023E724(i)->x - px);
            if (push.x < 0.2f) {
                push.x = 0.2f;
            }
            push.y = push.y - 0.005f * std::fabsf(getPlayerPos_8023E724(i)->y - py);
            if (push.y < 0.2f) {
                push.y = 0.2f;
            }
        }
        push.x = push.x * JMath::sincosTable_.sinShort(angle);
        push.y = push.y * JMath::sincosTable_.cosShort(angle);
        if (!fn_80236BEC(i)) {
            fn_8023DFF4(i, &push);
        }
    }
}

// FIXME(shotgun): 96.29 %, missing/extra + reloc-name.
bool cls_804A1E50::fn_8018DA6C() {
    if (field_0x27b == 1) {
        return false;
    }
    if (getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 12) & 0x1F) &&
        getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 17) & 0x1F) &&
        getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 22) & 0x1F) &&
        getGameView_802D6A9C()->fn_802D800C(field_0x90 >> 27)) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i) && !fn_8022F478(i) &&
                fn_80230188(i) && field_0x29c == 1) {
                fn_8023061C(i, false);
                field_0x29c = 0;
            }
        }
        if (field_0x284 != 255.0f) {
            field_0x284 = 255.0f;
            field_0x244 = 300;
            startSound_801F0E34(0x4001F, 0);
        }
        if (field_0x280 > 200.0f) {
            field_0x27c = -1;
            stopEmitter(field_0x328);
            stopEmitter(field_0x32c);
            stopEmitter(field_0x330);
            field_0x294 = 2.0f;
            mState = 6;
            return true;
        }
    } else {
        field_0x284 = 100.0f;
    }
    return false;
}

cls_804A1E50::~cls_804A1E50() {
    releaseEmitter_801EE434(field_0x328);
    releaseEmitter_801EE434(field_0x32c);
    releaseEmitter_801EE434(field_0x330);
    if (field_0x26c == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
}

void cls_804A1E50::vfunc_0x24() {
    startSound_801F0E34(0x40015, 0);
    vfunc_0x28(2, 0.0f, 16.0f);
}

// FIXME(shotgun): draw (fn_8018A930, 0x6CC bytes), update (fn_8018AFFC, 0x29D8 bytes) and the
// out-of-line TBox2<f32> constructor (fn_8018E890) are not written.
