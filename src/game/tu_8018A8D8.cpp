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
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"

// Animation lengths in frames (.data 0x804A1B70, not written as source here), indexed by
// field_0x270 + field_0x264.
extern const s32 lbl_804A1B70[];
// Sprite names of the animation frames (.data 0x804A1D24), indexed like lbl_804A1B70.
extern const char* const lbl_804A1D24[];
extern JGeometry::TVec3f lbl_804C18B8;
extern s32 lbl_80539FC8;
extern s32 lbl_80539FCC;
extern s32 lbl_80539FD0;

void fn_80154718(const JGeometry::TVec3f* pos);
void fn_801546BC(JGeometry::TVec3f* v);
s32 fn_8015465C(f32 y, f32 x);
void fn_801545F0(JGeometry::TVec3f* a, const JGeometry::TVec3f* b);
void fn_80154590(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1);
void fn_80154558(JGeometry::TVec2f* a, const JGeometry::TVec2f* b);
f32 fn_801544FC(s16 angle);
f32 fn_801544E4(s16 angle);
ActorBase_804A5770* fn_8015445C(s32 serial);
f32 fn_80154378(f32 x);
f32 fn_8015436C(f32 x);
s32 fn_80154310(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg, s32 a, s32 b);
s32 fn_801542AC(u32 id, s32 layer, ActorBase_804A5770** out, s32 excludeSerial);
void fn_8017ED08();
u8 fn_8017ECC0();
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value);
void fn_8017EC5C(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1);
void fn_8017EC38(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src);
bool fn_8017EB94(const JGeometry::TBox2f* box);
void fn_8017EAB4(JPABaseEmitter* emitter);
JGeometry::TBox2f* fn_801826A4(s32 layer);
AudioMgr_8045A080* fn_8018A6AC();
s32 fn_8018A660();
void fn_8018A63C();

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

// FIXME(shotgun): 92.72 %, different + reloc-name.
void cls_804A1E50::init() {
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
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
    lbl_804C18B8.z = 0.0f;
    lbl_804C18B8.y = 0.0f;
    lbl_804C18B8.x = 0.0f;
    lbl_80539FC8 = -1;
    field_0x270 = 0;
    field_0x264 = 0;
    field_0x280 = 0.0f;
    field_0x288 = 0.0f;
    field_0x268 = 0;
    field_0x27b = 0;
    field_0x29c = 0;
    field_0x2ac.z = 0.0f;
    field_0x2ac.y = 0.0f;
    field_0x2ac.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
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

// FIXME(shotgun): 97.85 %, missing/extra + reloc-name.
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

// FIXME(shotgun): 92.00 %, missing/extra + reloc-name.
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

// FIXME(shotgun): 96.41 %, missing/extra + reloc-name.
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

// FIXME(shotgun): 97.29 %, missing/extra + reloc-name
void cls_804A1E50::update() {
    JGeometry::TBox2f box50;
    fn_8017EC5C(&box50, -30.0f, -30.0f, 30.0f, 30.0f);
    JGeometry::TVec2f vec14;
    fn_8018E964(&vec14, mPos.x, mPos.y);
    JGeometry::TVec3f pos44;
    JGeometry::TVec3f pos38;
    u16 outAngle;
    s32 outDir = -1;
    f32 outDist;
    bool r29 = false;
    addCalc2_801F71A4(&field_0x294, field_0x298, 0.2f, 0.4f);
    mLife = 10000;
    if (field_0x26c == 0) {
        fn_8017ECC0();
    }
    if (vfunc_0x2C(0.0f, 16.0f, 50.0f)) {
        field_0x24c++;
        field_0x24c &= 0xF;
        if (field_0x24c == 0) {
            fn_8018E8C0(&pos44, &mPos);
            pos44.x += -(64.0f * fn_80154744() - 32.0f);
            pos44.y += -(64.0f * fn_80154744() - 32.0f);
            spawnEmitter_8013CC50(field_0x4, &pos44, 0x1FF, 1, 2, NULL, NULL, true, 1.0f);
        }
        fn_8018E8C0(&pos44, &mPos);
        pos44.y -= 40.0f;
        fn_8017EC70(&pos44, 1.0f);
    }
    if (fn_801F3D94(field_0x26c != 0)) {
        if (field_0x26c == 0) {
            field_0x2ac.z = 60.0f;
            mPos.z = 60.0f;
            field_0x3c.z = 0.0f;
        }
        if (field_0x26c == 1) {
            fn_801545C0(NULL, false);
            return;
        }
    }
    if (field_0x26c == 1 && lbl_80539FCC >= 0 && mState != 0x1F && mState != 0x20 && mState != 0x21 &&
        field_0x240 == 0 && lbl_80539FD0 > 0 && lbl_80539FCC != 10 && fn_801F2FAC()) {
        fn_801545C0(NULL, false);
        startSound_801F0E34(0x40014, 0);
        mState = 0x1F;
        return;
    }
    addCalc2_801F71A4(&field_0x2c4[1].x, field_0x2c4[2].x, 0.3f, 2.0f);
    addCalc2_801F71A4(&field_0x2c4[1].y, field_0x2c4[2].y, 0.3f, 2.0f);
    addCalc2_801F71A4(&field_0x3c.x, field_0x2ac.x, 0.1f, field_0x2b8);
    addCalc2_801F71A4(&field_0x3c.y, field_0x2ac.y, 0.1f, field_0x2bc);
    addCalc2_801F71A4(&field_0x2b8, 1.0f, 0.1f, 0.1f);
    addCalc2_801F71A4(&field_0x2bc, 1.0f, 0.1f, 0.1f);
    if (field_0x26c == 0) {
        field_0x2ac.z = 60.0f + 6.0f * fn_801544E4(field_0x278);
        field_0x278 += 0x300;
        field_0x28c = fn_801544FC((s16)field_0x250);
        field_0x250 += field_0x274;
        addCalc2_801F71A4(&field_0x280, field_0x284, 0.5f, 2.0f);
        addCalc2_801F71A4(&mPos.z, field_0x2ac.z, 0.3f, field_0x2c0);
    }
    fn_801545F0(&mPos, &field_0x3c);
    if (field_0x26c == 0) {
        JGeometry::TBox2f box28;
        fn_8017EC38(&box28, fn_801826A4(fn_8015461C()));
        if (field_0x330 != NULL) {
            fn_8018E8C0(&pos44, &mPos);
            pos44.x -= 46.0f;
            pos44.x += -16.0f * fn_801544FC((s16)(s32)(8192.0f * field_0x28c));
            pos44.y += 40.0f;
            setEmitterPos_8013CB44(field_0x330, &pos44, 1, 2, true);
        }
        if (mState != 0x14 && mState != 0x15 && mState != 0x13 && mState != 0x16 &&
            (mState != 0x17 || field_0x258 != 0)) {
            f32 marginMinY = 128.0f;
            if (mState == 0xB) {
                marginMinY = 64.0f;
            }
            if (mPos.y < box28.i.y + marginMinY) {
                if (field_0x3c.y < 0.0f) {
                    field_0x2ac.y = 0.0f;
                    field_0x3c.y = 0.0f;
                    mPos.y = box28.i.y + marginMinY;
                }
                r29 = true;
            }
            f32 marginMaxY = 64.0f;
            if (mState == 0xB) {
                marginMaxY = 100.0f;
            }
            if (mPos.y > box28.f.y - marginMaxY) {
                if (field_0x3c.y > 0.0f) {
                    field_0x2ac.y = 0.0f;
                    field_0x3c.y = 0.0f;
                    mPos.y = box28.f.y - marginMaxY;
                }
                r29 = true;
            }
            f32 marginX = 64.0f;
            if (mPos.x < marginX + box28.i.x) {
                if (field_0x3c.x < 0.0f) {
                    field_0x2ac.x = 0.0f;
                    field_0x3c.x = 0.0f;
                    mPos.x = marginX + box28.i.x;
                }
                r29 = true;
            }
            f32 marginMaxX = 64.0f;
            if (mPos.x > box28.f.x - marginMaxX) {
                if (field_0x3c.x > 0.0f) {
                    field_0x2ac.x = 0.0f;
                    field_0x3c.x = 0.0f;
                    mPos.x = box28.f.x - marginMaxX;
                }
                r29 = true;
            }
        }
        fn_8018E8C0(&pos44, &mPos);
        pos44.x -= 16.0f;
        pos44.y -= 64.0f;
        fn_80154718(&pos44);
        if (mState == 1 || mState == 2) {
            fn_8017EC70(&pos44, 1.0f);
            fn_8017ED08();
        }
    }
    if (field_0x26c == 1 && lbl_80539FCC >= 0 && mState != 0x1F && mState != 0x20 && mState != 0x22) {
        fn_8018E8C0(&pos44, &mPos);
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        fn_801F2B7C(fn_80154510(), -1, 6);
        fn_8018E8C0(&mPos, &pos44);
    }
    u8 r28 = 0;
    if (field_0x26c == 1) {
        r28 = fn_801545C0(NULL, false);
    }

    switch (mState) {
    case 0:
        if (fn_8018E930((field_0x90 >> 12) & 0x1F) && fn_8018E930((field_0x90 >> 17) & 0x1F) &&
            fn_8018E930((field_0x90 >> 22) & 0x1F) && fn_8018E930((u32)field_0x90 >> 27)) {
            fn_80154558(&box50.i, &vec14);
            fn_80154558(&box50.f, &vec14);
            if (fn_8017EB94(&box50) && fn_8018A660() == 1) {
                fn_8018E30C(0);
                fn_8022ADE4();
                field_0x234 = 0;
                fn_8017ED08();
                field_0x284 = 250.0f;
                startSound_801F0E34(0x4001D, 0);
                mState = 1;
            }
        }
        break;
    case 1:
        field_0x288 = field_0x280;
        if (field_0x330 == NULL) {
            fn_8018E8C0(&pos44, &mPos);
            pos44.x -= 46.0f;
            pos44.x += -16.0f * fn_801544FC((s16)(s32)(8192.0f * field_0x28c));
            pos44.y += 40.0f;
            field_0x330 = spawnEmitter_8013CC50(field_0x4, &pos44, 0x56, 1, 2, NULL, NULL, true, 1.0f);
        }
        if (field_0x280 < 249.0f) {
            break;
        }
        startSound_801F0E34(0x40024, 0);
        field_0x280 = 250.0f;
        field_0x288 = 250.0f;
        mState = 2;
        // fallthrough
    case 2: {
        f32 targetY = field_0x2a0.y - 96.0f;
        addCalc2_801F71A4(&mPos.y, targetY, 0.2f, 1.0f);
        if (fn_8015436C(mPos.y - targetY) > 1.0f) {
            field_0x238 = 40;
            break;
        }
        if (field_0x238 != 0) {
            break;
        }
        fn_8018E30C(10);
        if (field_0x27a == 0) {
            break;
        }
        fn_8018E30C(5);
        field_0x274 = 0x600;
        mState = 3;
    }
        // fallthrough
    case 3:
        if (field_0x27a == 0) {
            field_0x238 = 70;
            break;
        }
        if (field_0x238 == 1) {
            fn_8018E8DC((field_0x90 >> 12) & 0x1F, false, 0xFF);
            fn_8018E8DC((field_0x90 >> 17) & 0x1F, false, 0xFF);
            fn_8018E8DC((field_0x90 >> 22) & 0x1F, false, 0xFF);
            fn_8018E8DC((u32)field_0x90 >> 27, false, 0xFF);
            if (field_0x328 != NULL) {
                fn_8017EAB4(field_0x328);
                field_0x328 = NULL;
            }
            if (field_0x32c != NULL) {
                fn_8017EAB4(field_0x32c);
                field_0x32c = NULL;
            }
            field_0x284 = 100.0f;
        }
        if (field_0x280 < 101.0f) {
            ActorBase_804A5770* switches[ACTOR_MANAGER_MAX_ACTORS];
            fn_801542AC('LWSW', fn_8015461C(), switches, -1);
            for (s32 i = 0; i < 4; i++) {
                ActorBase_804A5770* sw = switches[i];
                if (sw != NULL && sw->fn_8018A5B0()) {
                    field_0x2e8[i] = sw->vfunc_0x3C();
                    fn_8018E8C0(&field_0x2f8[i], sw->fn_801544DC());
                }
            }
            fn_8018A6AC()->fn_80458FF0(0x1000004, false, 0);
            fn_8018A63C();
            field_0x280 = 100.0f;
            field_0x244 = 300;
            mState = 4;
        }
        break;
    case 4: {
        field_0x27b = 0;
        field_0x27c = -1;
        if (field_0x328 != NULL) {
            fn_8017EAB4(field_0x328);
            field_0x328 = NULL;
        }
        if (field_0x32c != NULL) {
            fn_8017EAB4(field_0x32c);
            field_0x32c = NULL;
        }
        field_0x284 = 100.0f;
        field_0x274 = 0x300;
        fn_8018E30C(0);
        f32 f30 = 500.0f * (f32)fn_8018E89C() + 500.0f;
        field_0x23c = (s32)(f30 * fn_80154744() + f30);
        field_0x258 = 0;
        field_0x25c = 0;
        mState = 5;
        break;
    }
    case 5: {
        if (field_0x330 == NULL) {
            fn_8018E8C0(&pos44, &mPos);
            pos44.x -= 46.0f;
            pos44.x += -16.0f * fn_801544FC((s16)(s32)(8192.0f * field_0x28c));
            pos44.y += 40.0f;
            field_0x330 = spawnEmitter_8013CC50(field_0x4, &pos44, 0x56, 1, 2, NULL, NULL, true, 1.0f);
        }
        if (field_0x284 != 255.0f && field_0x244 == 0) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && fn_8015461C() == getPlayerLayer_8023DE58(i) && !fn_8022F478(i) &&
                    !fn_80230188(i)) {
                    f32 dx = field_0x2c4[1].x - getPlayerPos_8023E724(i)->x;
                    f32 dy = field_0x2c4[1].y - getPlayerPos_8023E724(i)->y;
                    if (fn_80154378(dx * dx + dy * dy) < 48.0f) {
                        if (fn_802308E0(i) != 1) {
                            return;
                        }
                        field_0x29c = 1;
                        startSound_801F0E34(0x40024, 0);
                        field_0x27c = i;
                        fn_8018E8C0(&field_0x2c4[2], getPlayerPos_8023E724(i));
                        mState = 10;
                        return;
                    }
                }
            }
        }
        if (field_0x23c == 0) {
            mState = 12;
            break;
        }
        if (fn_8018DA6C()) {
            break;
        }
        f32 dx = field_0x2a0.x - mPos.x;
        f32 dy = field_0x2a0.y - mPos.y;
        if (r29 == true) {
            if (field_0x238 < 30) {
                field_0x238 = 30;
            }
            field_0x2ac.x = 1.2f * fn_801544FC(fn_8015465C(dx, dy));
            field_0x2ac.y = 1.2f * fn_801544E4(fn_8015465C(dx, dy));
        } else if (field_0x238 == 0) {
            if (fn_80154744() < 0.6f || (field_0x2ac.x == 0.0f && field_0x2ac.y == 0.0f)) {
                s32 ang = (s32)(65536.0f * fn_80154744());
                field_0x238 = (s32)(300.0f * fn_80154744() + 300.0f);
                field_0x2ac.x = 1.2f * fn_801544FC((s16)ang);
                field_0x2ac.y = 1.2f * fn_801544E4((s16)ang);
                field_0x2b8 = 0.0f;
                field_0x2bc = 0.0f;
            } else {
                field_0x238 = (s32)(50.0f * fn_80154744() + 50.0f);
                field_0x2ac.x = 0.0f;
                field_0x2ac.y = 0.0f;
                field_0x2b8 = 0.0f;
                field_0x2bc = 0.0f;
            }
        }
        break;
    }
    case 6:
        field_0x27b = 0;
        fn_8018E30C(12);
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x258 = 0;
        field_0x25c = (s16)fn_8015465C(field_0x2a0.x - mPos.x, field_0x2a0.y - mPos.y);
        field_0x2b8 = 0.0f;
        field_0x2bc = 0.0f;
        mState = 7;
        // fallthrough
    case 7:
        if (!fn_8018E930((field_0x90 >> 12) & 0x1F) || !fn_8018E930((field_0x90 >> 17) & 0x1F) ||
            !fn_8018E930((field_0x90 >> 22) & 0x1F) || !fn_8018E930((u32)field_0x90 >> 27)) {
            mState = 4;
            break;
        }
        startSound_801F0E34(0x40020, 0);
        fn_8018E8C0(&pos44, &mPos);
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        if (field_0x27a == 1 && field_0x2ac.x == 0.0f && field_0x2ac.y == 0.0f) {
            field_0x2ac.x = 1.2f * fn_801544FC((s16)field_0x25c);
            field_0x2ac.y = 1.2f * fn_801544E4((s16)field_0x25c);
        }
        if (r29 == true) {
            if (fn_80154744() < 0.5f) {
                field_0x25c += 0x2000;
            } else {
                field_0x25c -= 0x2000;
            }
            field_0x25c = (u16)field_0x25c;
            field_0x2ac.x = 1.2f * fn_801544FC((s16)field_0x25c);
            field_0x2ac.y = 1.2f * fn_801544E4((s16)field_0x25c);
            field_0x3c.x = field_0x2ac.x;
            field_0x3c.y = field_0x2ac.y;
        }
        fn_80154590(&mBox, -50.0f, -40.0f, 42.0f, 40.0f);
        if (fn_801F2FAC()) {
            for (s32 i = 0; i < 4; i++) {
                s32 hit = fn_801F285C(NULL, NULL, true, i);
                field_0x2ac.x = 0.0f;
                field_0x2ac.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x3c.y = 0.0f;
                if (hit >= 0) {
                    if (fn_8022F514(hit)) {
                        field_0x254 = fn_8022F3DC(hit);
                    } else {
                        field_0x254 = hit;
                    }
                    fn_8018E8C0(&mPos, &pos44);
                    field_0x280 = 255.0f;
                    mState = 8;
                    break;
                }
            }
        }
        fn_80154590(&mBox, -42.0f, -32.0f, 34.0f, 32.0f);
        fn_801F2B7C(fn_80154510(), -1, 6);
        fn_8018E8C0(&mPos, &pos44);
        if (lbl_80539FD0 < 9) {
            field_0x258++;
            if (field_0x258 > 600) {
                field_0x27b = 1;
                mState = 12;
            }
        }
        break;
    case 8:
        if (field_0x328 != NULL) {
            fn_8017EAB4(field_0x328);
            field_0x328 = NULL;
        }
        if (field_0x32c != NULL) {
            fn_8017EAB4(field_0x32c);
            field_0x32c = NULL;
        }
        startSound_801F0E34(0x40014, 0);
        fn_8018E30C(9);
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        field_0x274 = 0x600;
        mState = 9;
        // fallthrough
    case 9:
        if (field_0x27a != 0) {
            if (field_0x254 >= 0 && fn_8023F264(field_0x254) &&
                fn_8015461C() == getPlayerLayer_8023DE58(field_0x254)) {
                mState = 0x12;
                break;
            }
            mState = 10;
        } else {
            break;
        }
        // fallthrough
    case 10:
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        fn_8018E30C(3);
        field_0x258 = 0;
        field_0x25c = (s32)(65535.0f * fn_80154744());
        if (fn_80154744() < 0.5f) {
            field_0x260 = 0x2000;
        } else {
            field_0x260 = 0xE000;
        }
        field_0x274 = 0x600;
        mState = 11;
        // fallthrough
    case 11:
        if (field_0x27c >= 0) {
            if (fn_8023F264(field_0x27c) && fn_8015461C() == getPlayerLayer_8023DE58(field_0x27c)) {
                fn_8018E8C0(&field_0x2c4[2], getPlayerPos_8023E724(field_0x27c));
            } else {
                field_0x27c = -1;
            }
        }
        if (fn_8018DA6C()) {
            break;
        }
        fn_8018DC70(0);
        if (field_0x27a == 1) {
            if (field_0x27c >= 0) {
                fn_8023061C(field_0x27c, false);
                field_0x27c = -1;
                field_0x29c = 0;
            }
            field_0x258++;
            if (field_0x2ac.x == 0.0f && field_0x2ac.y == 0.0f) {
                field_0x2ac.x = 1.5f * fn_801544FC((s16)field_0x25c);
                field_0x2ac.y = 1.5f * fn_801544E4((s16)field_0x25c);
            }
            if (r29 == true) {
                field_0x25c += field_0x260;
                field_0x25c = (u16)field_0x25c;
                field_0x2ac.x = 1.5f * fn_801544FC((s16)field_0x25c);
                field_0x2ac.y = 1.5f * fn_801544E4((s16)field_0x25c);
                field_0x3c.x = field_0x2ac.x;
                field_0x3c.y = field_0x2ac.y;
            }
            for (s32 i = 0; i < 4; i++) {
                if ((field_0x258 & 0x1F) == 0 && fn_8018E930(field_0x2e8[i])) {
                    if (fn_8015436C(field_0x2f8[i].x - mPos.x) < 80.0f &&
                        mPos.y - 16.0f < field_0x2f8[i].y &&
                        fn_8015436C(field_0x2f8[i].y - mPos.y) < 120.0f) {
                        fn_8018E8DC(field_0x2e8[i], false, 0xFF);
                        ActorBase_804A5770* poohs[ACTOR_MANAGER_MAX_ACTORS];
                        if (fn_801542AC('POOH', fn_8015461C(), poohs, -1) < 8) {
                            fn_80154310('POOH', fn_8015461C(), &field_0x2f8[i], 9, -1, -1);
                        }
                    }
                }
            }
            if (field_0x258 > 600) {
                field_0x258 = 0;
                field_0x25c = 0;
                field_0x244 = 300;
                mState = 4;
            }
        }
        break;
    case 12:
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        startSound_801F0E34(0x40024, 0);
        field_0x274 = 0x600;
        mState = 13;
        // fallthrough
    case 13: {
        if (fn_8018DA6C()) {
            break;
        }
        f32 targetY = field_0x2a0.y - 96.0f;
        addCalc2_801F71A4(&mPos.y, targetY, 0.2f, 1.0f);
        addCalc2_801F71A4(&mPos.x, field_0x2a0.x, 0.2f, 1.0f);
        if (fn_8015436C(mPos.y - targetY) > 1.0f || fn_8015436C(mPos.x - field_0x2a0.x) > 1.0f) {
            field_0x238 = 40;
            break;
        }
        if (field_0x238 != 0) {
            break;
        }
        fn_8018E30C(10);
        if (field_0x27a == 0) {
            break;
        }
        fn_8018E30C(5);
        field_0x258 = 0;
        mState = 14;
    }
        // fallthrough
    case 14:
        if (fn_8018DA6C()) {
            break;
        }
        // fallthrough
    case 17:
        if (field_0x27a == 1) {
            fn_8018DC70(1);
            field_0x258++;
        }
        if (field_0x258 > 70) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8018E930(field_0x2e8[i])) {
                    ActorBase_804A5770* poohs[ACTOR_MANAGER_MAX_ACTORS];
                    if (fn_801542AC('POOH', fn_8015461C(), poohs, -1) < 8) {
                        fn_80154310('POOH', fn_8015461C(), &field_0x2f8[i], 9, -1, -1);
                    }
                    fn_8018E8DC(field_0x2e8[i], false, 0xFF);
                }
            }
            mState = 4;
        }
        break;
    case 15:
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        field_0x274 = 0x600;
        field_0x284 = 100.0f;
        if (field_0x254 >= 0 && fn_8022D75C(field_0x254) == 1) {
            break;
        }
        field_0x254 = -1;
        field_0x274 = 0x600;
        fn_8018E30C(10);
        startSound_801F0E34(0x40024, 0);
        mState = 16;
        // fallthrough
    case 16: {
        f32 targetY = field_0x2a0.y - 96.0f;
        addCalc2_801F71A4(&mPos.y, targetY, 0.5f, 5.0f);
        addCalc2_801F71A4(&mPos.x, field_0x2a0.x, 0.5f, 5.0f);
        if (fn_8015436C(mPos.y - targetY) <= 1.0f && fn_8015436C(mPos.x - field_0x2a0.x) <= 1.0f) {
            fn_8018E30C(5);
            field_0x258 = 0;
            mState = 17;
        }
        break;
    }
    case 18:
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        if (field_0x254 >= 0 && fn_8023F264(field_0x254) &&
            fn_8015461C() == getPlayerLayer_8023DE58(field_0x254)) {
            mState = 19;
        } else {
            mState = 10;
        }
        break;
    case 19:
        if (field_0x254 >= 0 && fn_8023F264(field_0x254) &&
            fn_8015461C() == getPlayerLayer_8023DE58(field_0x254)) {
            f32 dx = getPlayerPos_8023E724(field_0x254)->x - mPos.x;
            f32 dy = (getPlayerPos_8023E724(field_0x254)->y - 64.0f) - mPos.y;
            if (fn_80154378(dx * dx + dy * dy) > 16.0f) {
                field_0x2ac.x = 3.0f * fn_801544FC(fn_8015465C(dx, dy));
                field_0x2ac.y = 3.0f * fn_801544E4(fn_8015465C(dx, dy));
            } else {
                field_0x2ac.x = 0.0f;
                field_0x2ac.y = 0.0f;
                fn_8022CA6C(field_0x254, 5, false);
                if (fn_802308E0(field_0x254)) {
                    fn_8018E30C(1);
                    for (s32 i = 0; i < 4; i++) {
                        fn_80230C6C(i);
                    }
                    field_0x29c = 1;
                    mState = 20;
                }
            }
        } else {
            mState = 10;
        }
        break;
    case 20:
        if (field_0x254 >= 0 && fn_8023F264(field_0x254) &&
            fn_8015461C() == getPlayerLayer_8023DE58(field_0x254)) {
            fn_8018E8C0(&field_0x2c4[0], getPlayerPos_8023E724(field_0x254));
        }
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        field_0x258 = 1;
        field_0x25c = 0;
        field_0x238 = 400;
        field_0x23c = 4;
        field_0x290 = 1.0f;
        field_0x274 = 0x600;
        mState = 21;
        // fallthrough
    case 21:
        if (field_0x25c == 0) {
            if (field_0x27a == 0) {
                fn_802303FC(field_0x254, getPlayerPos_8023E724(field_0x254), true);
                break;
            }
            if (field_0x254 >= 0 && lbl_80539FC8 > -1 &&
                (lbl_804C18B8.x > 0.0f || lbl_804C18B8.y > 0.0f)) {
                fn_8018E8C0(&pos44, &mPos);
                pos44.x -= 4.0f;
                pos44.y -= 32.0f;
                addCalc2_801F71A4(&field_0x2c4[0].x, pos44.x, 0.1f, field_0x290);
                addCalc2_801F71A4(&field_0x2c4[0].y, pos44.y, 0.1f, field_0x290);
                addCalc2_801F71A4(&field_0x2c4[0].z, pos44.z, 0.1f, field_0x290);
                f32 dy = field_0x2c4[0].y - pos44.y;
                f32 dx = field_0x2c4[0].x - pos44.x;
                f32 dz = field_0x2c4[0].z - pos44.z;
                if (fn_80154378(dx * dx + dy * dy + dz * dz) < 80.0f) {
                    field_0x290 = 3.0f;
                }
                fn_80230498(field_0x254, &field_0x2c4[0], field_0x258 + 0xAF, 0xFF, -0x80, -0x80, true);
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8023F264(i) && fn_8015461C() == getPlayerLayer_8023DE58(i) &&
                        fn_8022F514(i) && field_0x254 == fn_8022F3DC(i)) {
                        fn_8018E8C0(&pos38, getPlayerPos_8023E724(i));
                        addCalc2_801F71A4(&pos38.x, pos44.x, 0.1f, 2.0f * field_0x290);
                        addCalc2_801F71A4(&pos38.y, pos44.y, 0.1f, 2.0f * field_0x290);
                        addCalc2_801F71A4(&pos38.z, pos44.z, 0.1f, 2.0f * field_0x290);
                        fn_80230498(i, &pos38, field_0x258 + 0xAF, 0xFF, -0x80, -0x80, true);
                    }
                }
                if (field_0x23c == 0) {
                    field_0x258++;
                    if (field_0x258 > 3) {
                        field_0x258 = 3;
                    }
                    field_0x23c = 4;
                }
                f32 pdx = getPlayerPos_8023E724(field_0x254)->x - pos44.x;
                f32 pdy = getPlayerPos_8023E724(field_0x254)->y - pos44.y;
                if (fn_80154378(pdx * pdx + pdy * pdy) > 8.0f) {
                    break;
                }
                startSound_801F0E34(0x4005C, 0);
                fn_8018E30C(7);
                field_0x25c = 1;
                fn_8023061C(field_0x254, false);
                field_0x29c = 0;
            } else {
                field_0x254 = -1;
                mState = 4;
            }
        }
        if (field_0x25c != 0) {
            fn_8018E8C0(&pos44, &lbl_804C18B8);
            pos44.x -= 64.0f;
            fn_8022CA6C(field_0x254, 5, false);
            if (fn_802317F0(field_0x254, lbl_80539FC8, getPlayerPos_8023E724(field_0x254), &pos44, 0x1D)) {
                fn_8018E8C0(&pos44, &mPos);
                pos44.y += 32.0f;
                lbl_80539FC8 = fn_8015461C();
                fn_8018E8C0(&lbl_804C18B8, &pos44);
                field_0x25c = 0;
                lbl_80539FCC = field_0x254;
                mState = 22;
            }
        }
        break;
    case 22:
        if (field_0x328 != NULL) {
            fn_8017EAB4(field_0x328);
            field_0x328 = NULL;
        }
        if (field_0x32c != NULL) {
            fn_8017EAB4(field_0x32c);
            field_0x32c = NULL;
        }
        for (s32 i = 0; i < 4; i++) {
            if (i != field_0x254 && fn_8023F264(i) && fn_8022F514(i) &&
                field_0x254 == fn_8022F3DC(i)) {
                fn_8018E8C0(&pos38, getPlayerPos_8023E724(i));
                addCalc2_801F71A4(&pos38.x, field_0x2c4[0].x, 0.1f, 2.0f * field_0x290);
                addCalc2_801F71A4(&pos38.y, field_0x2c4[0].y, 0.1f, 2.0f * field_0x290);
                addCalc2_801F71A4(&pos38.z, field_0x2c4[0].z, 0.1f, 2.0f * field_0x290);
                fn_80230498(i, &pos38, field_0x258 + 0xAF, 0xFF, -0x80, -0x80, true);
            }
        }
        if (fn_8022D75C(field_0x254) == 1) {
            break;
        }
        fn_8023061C(field_0x254, false);
        field_0x258 = 0;
        mState = 23;
        // fallthrough
    case 23:
        if (field_0x258 == 0) {
            addCalc2_801F71A4(&mPos.y, field_0x2a0.y, 0.3f, 3.0f);
            addCalc2_801F71A4(&mPos.x, field_0x2a0.x, 0.3f, 3.0f);
            if (fn_8015436C(mPos.y - field_0x2a0.y) > 1.0f || fn_8015436C(mPos.x - field_0x2a0.x) > 1.0f) {
                break;
            }
            field_0x258 = 1;
        }
        fn_8018E8C0(&pos44, &mPos);
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        fn_801F2B7C(fn_80154510(), -1, 6);
        fn_8018E8C0(&mPos, &pos44);
        if (field_0x254 >= 0 && fn_8023F264(field_0x254) && lbl_80539FCC == field_0x254) {
            if (!fn_80230188(field_0x254)) {
                field_0x2ac.x = 2.0f * fn_8022C82C(field_0x254);
                field_0x2ac.y = -2.0f * fn_8022C76C(field_0x254);
            } else {
                field_0x2ac.x = 0.0f;
                field_0x2ac.y = 0.0f;
            }
        } else {
            field_0x2ac.x = 0.0f;
            field_0x2ac.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            if (lbl_80539FD0 <= 0) {
                ActorBase_804A5770* poohs[ACTOR_MANAGER_MAX_ACTORS];
                s32 count = fn_801542AC('POOH', fn_8015461C(), poohs, -1);
                for (s32 i = 0; i < count; i++) {
                    ActorBase_804A5770* pooh = poohs[i];
                    if (pooh != NULL) {
                        static_cast<cls_804A1E50*>(pooh)->fn_8018E894(1);
                    }
                }
            }
            mState = 24;
        }
        break;
    case 24:
        addCalc2_801F71A4(&mPos.y, field_0x2a0.y - 16.0f, 0.3f, 3.0f);
        addCalc2_801F71A4(&mPos.x, field_0x2a0.x, 0.3f, 3.0f);
        if (fn_8015436C(mPos.y - (field_0x2a0.y - 16.0f)) > 1.0f ||
            fn_8015436C(mPos.x - field_0x2a0.x) > 1.0f) {
            break;
        }
        if (lbl_80539FCC == 10) {
            lbl_80539FCC = field_0x254;
            if (lbl_80539FD0 > 0) {
                fn_8018E30C(11);
            }
            fn_8018E8C0(&pos44, &mPos);
            pos44.y += 32.0f;
            lbl_80539FC8 = fn_8015461C();
            fn_8018E8C0(&lbl_804C18B8, &pos44);
        }
        if (lbl_80539FCC < 0) {
            if (lbl_80539FD0 > 0) {
                mState = 15;
            } else {
                vfunc_0x24();
                mState = 25;
            }
        }
        break;
    case 25:
        break;
    case 26:
        fn_8018E8C0(&mPos, &field_0x2a0);
        if (lbl_80539FCC < 0 || fn_8015461C() != getPlayerLayer_8023DE58(lbl_80539FCC) ||
            fn_8022D75C(lbl_80539FCC) == 1) {
            break;
        }
        field_0x244 = 900;
        field_0x258 = 3;
        field_0x238 = (s32)(100.0f * fn_80154744()) + 100;
        // fallthrough
    case 27:
        field_0x234 = 8;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x254 = 0;
        field_0x23c = 0;
        field_0x240 = 0;
        field_0x264 = lbl_804A1B70[0x12C / 4 + field_0x24c];
        if (field_0x238 == 0) {
            field_0x238 = (s32)(50.0f * fn_80154744()) + 50;
        }
        mState = 28;
        // fallthrough
    case 28:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
            startSound_801F0E34(0x50086, 0);
        }
        if (lbl_80539FCC >= 0 && fn_8023F264(lbl_80539FCC)) {
            fn_801EEC98(fn_801545E8(), &outAngle, &outDir, &outDist, NULL, lbl_80539FCC, 2);
            if (r28 == 1 || outDist > 80.0f) {
                field_0x2ac.x = 0.0f;
                field_0x2ac.y = 0.0f;
                if (r28 == 1) {
                    field_0x3c.x *= -1.0f;
                    field_0x3c.y *= -1.0f;
                }
                if (field_0x238 == 0) {
                    mState = 29;
                    break;
                }
                if (r28 == 1) {
                    if (fn_80154744() < 0.5f) {
                        field_0x250 = 0x4000;
                    } else {
                        field_0x250 -= 0x4000;
                    }
                    field_0x23c = 10;
                } else if (field_0x23c == 0) {
                    field_0x250 = 0;
                }
            } else if (field_0x23c == 0) {
                field_0x254 = outAngle + 0x8000;
            }
            field_0x2ac.x = 2.5f * fn_801544FC((s16)(field_0x254 + field_0x250));
            field_0x2ac.y = 2.5f * fn_801544E4((s16)(field_0x254 + field_0x250));
        }
        if (field_0x234 == 0) {
            field_0x24c++;
            field_0x24c &= 3;
            field_0x234 = 8;
            field_0x264 = lbl_804A1B70[0x12C / 4 + field_0x24c];
        }
        if (field_0x244 == 0) {
            field_0x254 = lbl_80539FCC;
            lbl_80539FCC = 10;
            mState = 33;
        }
        break;
    case 29:
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x234 = lbl_804A1B70[0x164 / 4 + field_0x24c];
        field_0x264 = lbl_804A1B70[0x13C / 4 + field_0x24c];
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        mState = 30;
        // fallthrough
    case 30:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
        }
        if (field_0x234 == 0) {
            field_0x24c++;
            if (field_0x24c == 8) {
                fn_8018E8C0(&pos44, &mPos);
                pos44.z = 0.0f;
                s32 ser = fn_80154310('OKTM', fn_8015461C(), &pos44, 3, -1, -1);
                if (ser >= 0) {
                    startSound_801F0E34(0x40013, 0);
                    ActorBase_804A5770* oktm = fn_8015445C(ser);
                    if (oktm != NULL) {
                        JGeometry::TVec3f vel;
                        fn_801546BC(&vel);
                        outAngle = 0;
                        fn_801EEC98(fn_801545E8(), &outAngle, NULL, NULL, NULL, 0, 0);
                        vel.x = 1.8f * fn_801544FC((s16)outAngle);
                        vel.y = 1.8f * fn_801544E4((s16)outAngle);
                        oktm->fn_8017EAF8(vel);
                    }
                }
            }
            if (field_0x24c > 9) {
                mState = 27;
            } else {
                field_0x234 = lbl_804A1B70[0x164 / 4 + field_0x24c];
                field_0x264 = lbl_804A1B70[0x13C / 4 + field_0x24c];
            }
        }
        break;
    case 31:
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x234 = lbl_804A1B70[0x1A0 / 4 + field_0x24c];
        field_0x264 = lbl_804A1B70[0x18C / 4 + field_0x24c];
        fn_801546BC(&field_0x3c);
        field_0x2ac.x = 0.0f;
        field_0x2ac.y = 0.0f;
        lbl_80539FD0--;
        field_0x258--;
        field_0x254 = lbl_80539FCC;
        if (field_0x244 == 0 || field_0x258 == 0 || lbl_80539FD0 == 0) {
            lbl_80539FCC = 10;
        }
        if (lbl_80539FD0 > 0) {
            mState = 32;
        } else {
            lbl_80539FCC = 10;
            field_0x240 = 200;
            field_0x23c = 150;
            mState = 34;
        }
        break;
    case 32:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
        }
        if (field_0x234 == 0) {
            field_0x24c++;
            if (field_0x24c > 4) {
                field_0x24c = 5;
                if (field_0x244 != 0 && field_0x258 > 0 && lbl_80539FD0 > 0) {
                    field_0x240 = 20;
                    mState = 29;
                } else {
                    mState = 33;
                }
            } else {
                field_0x234 = lbl_804A1B70[0x1A0 / 4 + field_0x24c];
                field_0x264 = lbl_804A1B70[0x18C / 4 + field_0x24c];
            }
        }
        break;
    case 34:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
        }
        if (field_0x23c != 0) {
            if ((field_0x23c & 7) == 0) {
                fn_8018E8C0(&pos44, &mPos);
                pos44.x += -(64.0f * fn_80154744() - 32.0f);
                pos44.y += -(64.0f * fn_80154744() - 32.0f);
                fn_80154310('HBMB', fn_8015461C(), &pos44, 7, -1, -1);
            }
            break;
        }
        // fallthrough
    case 33:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
        }
        if (lbl_80539FCC != 10) {
            fn_8022CA6C(field_0x254, 5, false);
            if (fn_802317F0(field_0x254, lbl_80539FC8, getPlayerPos_8023E724(field_0x254), &lbl_804C18B8, 0x1E)) {
                lbl_80539FC8 = fn_8015461C();
                fn_8018E8C0(&lbl_804C18B8, &field_0x2a0);
                lbl_80539FCC = -1;
                fn_801546BC(&field_0x2ac);
                fn_801546BC(&field_0x3c);
                mPos.z = 0.0f;
                if (lbl_80539FD0 > 0) {
                    mState = 26;
                } else {
                    fn_8018A598(false);
                }
            }
        }
        break;
    }

    fn_8018DFCC();
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
    if (field_0x248 > 0) {
        field_0x248--;
    }
}


// FIXME(shotgun): 87.03 %, missing/extra + reloc-name
void cls_804A1E50::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x26c == 0) {
        if (mState == 0) {
            return;
        }
        params.field_0x1f = 0;
        void* res = fn_804032E8('BIGP', "boss_p_rantan.bti");
        setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, field_0x28c, 0.0f), 1.0f);
        params.field_0x1d = 2;
        params.field_0x1c = 1;
        params.mMtx = mtx;
        params.field_0x10.a = (u8)(s32)field_0x288;
        fn_801EFD34(res, &params, -1, -46.0f, 33.0f, 0.0f);

        JGeometry::TVec3f top = mPos;
        top.x -= 46.0f;
        top.y += 24.0f;
        JGeometry::TVec3f shadow(top.x, top.y + 8.0f, top.z);
        JUtility::TColor shadowColor(0, 0, 0, (u8)(s32)(0.5f * field_0x288));
        gGameRoot2_8053AB10->field_0x2c->drawShadow_80158B5C(field_0x4, &shadow, shadowColor, 80.0f);

        top.x += -16.0f * JMath::sincosTable_.sinShort((s32)(8192.0f * field_0x28c));
        if (field_0x27c < 0) {
            field_0x2c4[2].x = top.x;
            field_0x2c4[2].y = top.y;
            field_0x2c4[2].z = top.z;
            field_0x2c4[2].x += -16.0f * JMath::sincosTable_.sinShort((s32)(8192.0f * field_0x28c));
            field_0x2c4[2].y += 16.0f;
            field_0x2c4[2].z = 0.0f;
        }
        if (field_0x284 != 255.0f) {
            JUtility::TColor color(0x3F3F1F7F);
            gGameRoot2_8053AB10->field_0x2c->fn_80158624(&top, &field_0x2c4[1], color, 48.0f);
        }

        params.field_0x1d = 2;
        params.field_0x1c = 1;
        u8 alpha = (u8)(s32)field_0x280;
        params.field_0x10.a = alpha;
        JGeometry::TVec3f base = mPos;
        base.x -= 8.0f;
        if (alpha == 255) {
            params.field_0x26 = 1;
            params.field_0x30 = 0x11;
        } else {
            params.field_0x26 = 0;
        }
        JUtility::TColor shadowColor2(0, 0, 0, (u8)(s32)(0.5f * field_0x280));
        gGameRoot2_8053AB10->field_0x2c->drawShadow_80158B5C(field_0x4, &base, shadowColor2, 120.0f);

        setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), field_0x294);
        params.mMtx = mtx;
        res = fn_804032E8('BIGP', lbl_804A1D24[field_0x270 + field_0x264]);
        fn_801EFD34(res, &params, -1, 0.0f, -8.0f, 0.0f);
        if (field_0x268 == 2) {
            res = fn_804032E8('BIGP', lbl_804A1D24[0xE8 / 4]);
            params.field_0x26 = 1;
            params.field_0x1d = 1;
            params.field_0x1c = 0;
            fn_801EFD34(res, &params, -1, 0.0f, -8.0f, 0.0f);
        }
    } else if (field_0x26c == 1) {
        if (lbl_80539FCC >= 0) {
            params.field_0x1d = 0;
            params.field_0x1c = 1;
            fn_801F02BC("enemy_both2_sob.bin", field_0x264 + 0x1B1, NULL, 0, 0, false, 1.0f, 1.0f);
        }
    }
}

void cls_804A1E50::vfunc_0x24() {
    startSound_801F0E34(0x40015, 0);
    vfunc_0x28(2, 0.0f, 16.0f);
}

