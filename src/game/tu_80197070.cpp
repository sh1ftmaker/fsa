#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameState.h"
#include "game/GameGlobals.h"
#include "game/act/act_ganon.h"
#include "game/act/act_life_meter.h"
#include "game/act/tu_801F9484.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/tu_80379F84.h"
#include "game/tu_803B8C74.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/Message.h"
#include "game/act/act_arrow.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"


// .sbss flags of the ZLD2 actor (cls_804A2944).
u8 lbl_8053AB98;
s32 lbl_8053AB9C;
s32 lbl_8053ABA0;
s32 lbl_8053ABA4;
u8 lbl_8053ABA8;

// Tables of the ZLD2 actor, not written as source here (.rodata 0x80468428: two s32[6] sprite
// frame tables, then the sprite bank names; .data 0x804A2880 / 0x804A28C4 / 0x804A28D8 /
// 0x804A28FC: u16 angles, u8 colours and u16 emitter ids indexed by field_0x94).
extern const s32 lbl_80468428[12];
extern const s32 lbl_804A2700[];
extern const u16 lbl_804A2880[];
extern const s32 lbl_804A288C[];
extern const s32 lbl_804A289C[];
extern const s32 lbl_804A28B0[];
extern const u8 lbl_804A28C4[];
extern const u16 lbl_804A28D8[];
extern const u16 lbl_804A28FC[];
extern const s32 lbl_8053A0E8[2];
extern const s32 lbl_8053A0F0[2];

bool fn_8017EB1C(f32 value);
u8 fn_8017EB44(const JGeometry::TVec3f* pos);
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value);
void fn_8018E8DC(s32 idx, bool flag, u8 alpha);
void fn_80196D30(s32 a, s32 b);

void LifeMeter_804B174C::fn_8019CEF8(s32 serial, f32 yOffset, s32 index) {
    field_0x250 = serial;
    field_0x254 = yOffset;
    field_0x24c = index;
}

void fn_8019CEE8(u8 v) {
    lbl_80529DEC.field_0x24d = v;
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8019CECC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void Ganon_804A25F0::fn_8019CEC4(s32 value) {
    field_0x2f8 = value;
}

void Ganon_804A25F0::fn_8019CEBC(s32 value) {
    field_0x28c = value;
}

s32 ActorBase_804A5770::fn_8019CEB4() const {
    return field_0xb0;
}

void ActorBase_804A5770::fn_8019CEAC(s32 v) {
    field_0xb0 = v;
}

BOOL fn_8019CE40(s32 stage, s32 sub) {
    return isStageFlagSet(stage, sub);
}

// Same body as setStageFlag (StageFlag.h, inlined by tu_803A8DFC); not a wrapper call
// because that compiles the early returns differently (90.87 %).
void fn_8019CDE4(s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return;
    }
    if (sub < 0 || sub > 3) {
        return;
    }
    lbl_8050EC80.getCurrentFile().mFlags |= 1 << (stage * 4 + sub);
}

void fn_8019CDC4(s32 v) {
    GameRoot_8049EF20* root = gGameRoot_8053AAF8;
    root->field_0x85 = 1;
    root->field_0x74 = 0;
    root->field_0x78 = 0;
    root->field_0x54 = v;
}

void fn_8019CD9C(s32 v) {
    gGameRoot_8053AAF8->fn_8013670C(v);
}

// FIXME(shotgun): 96.15 %, missing/extra + reloc-name.
void cls_804A2944::init() {
    lbl_8053ABA8 = 0;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    for (int i = 0; i < 4; i++) {
        field_0x2a4[i].set(0.0f, 0.0f, 0.0f);
        field_0x2d4[i].set(0.0f, 0.0f, 0.0f);
    }
    field_0x25c = -1;
    field_0x264 = -1;
    field_0x27c = 0.0f;
    field_0x278 = 0.0f;
    field_0x274 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x94 = (field_0x90 >> 4) & 0xF;
    field_0x90 &= 3;
    field_0x26c = 0.0f;
    field_0x270 = 0.0f;
    field_0x268 = 0;
    for (int i = 0; i < 3; i++) {
        field_0x280[i].x = mPos.x;
        field_0x280[i].y = mPos.y;
        field_0x280[i].z = mPos.z;
    }
    for (int i = 0; i < 13; i++) {
        mEmitters[i] = NULL;
    }
    lbl_8053AB9C = 0;
    field_0xb0 = 0xA040004;
    field_0x254 = 0;
    field_0x258 = 5;
    switch (field_0x90) {
    case 0: {
        field_0x98 = 2;
        lbl_8053AB98 = 0;
        field_0x254 = 0x24;
        lbl_8053ABA0 = 0;
        lbl_8053ABA4 = 0;
        getGameView_802D6A9C()->fn_802D7440(false);
        gAudioMgr_8053AF80->fn_80458F9C(60);
        field_0x260 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', field_0x4, &mPos, 0, -1, -1);
        s32 serial = field_0x260;
        if (serial >= 0) {
            ActorBase_804A5770* limit = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (limit != NULL) {
                static_cast<LifeMeter_804B174C*>(limit)->fn_8019CEF8(mSerial, -28.0f, field_0x258);
            }
        }
        JGeometry::TVec3f origin(field_0x280[0].x, field_0x280[0].y, field_0x280[0].z);
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &origin, 0x1CA, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &origin, 0x1CB, 0, 0, NULL, NULL, true, 1.0f);
        }
        mPos.y += 148.0f;
        mPos.x = field_0x280[0].x;
        mState = 0;
        break;
    }
    case 1:
        field_0xb0 = 0xA000000;
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x533, 1, 5, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x534, 1, 5, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[2] == NULL) {
            mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x535, 1, 5, NULL, NULL, true, 1.0f);
        }
        mState = 0x46;
        break;
    case 2:
        field_0x270 = 255.0f;
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, 0, 4, NULL, NULL, true, 1.0f);
            const u8* color = &lbl_804A28C4[field_0x94 * 3];
            mEmitters[0]->setGlobalEnvColor(color[0], color[1], color[2]);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, 4, NULL, NULL, true, 1.0f);
            const u8* color = &lbl_804A28C4[field_0x94 * 3];
            mEmitters[1]->setGlobalEnvColor(color[0], color[1], color[2]);
        }
        mState = 0x4A;
        break;
    case 3:
        break;
    default:
        field_0x26c = mPos.z;
        field_0x234 = 9;
        mState = 0x4F;
        break;
    }
}

cls_804A2944::~cls_804A2944() {
    for (s32 i = 0; i < 13; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    // field_0x90 is u32, but the original tests it with a signed cmpwi.
    s32 param = field_0x90;
    if (param == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        lbl_8053AB98 = 0;
        lbl_8053ABA8 = 0;
    }
}

// FIXME(shotgun): 99.55 %, different.
void cls_804A2944::vfunc_0x44() {
    if (field_0x90 == 1 && mState != 0x49) {
        if (getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c) != NULL) {
            s32 serial = field_0xa0;
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == 'ARRW') {
                    mPos.set(actor->mPos);
                    if (static_cast<ArrowPlayer_804A05E0*>(actor)->field_0x26c >= 1) {
                        field_0x250 = -1;
                        return;
                    }
                    if (field_0x250 == 0) {
                        field_0x250 = lbl_8053A100;
                        field_0x250++;
                        for (int i = 0; i < 3; i++) {
                            deleteEmitter(&mEmitters[i]);
                        }
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB5, 1, 5, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB6, 1, 5, NULL, NULL, true, 1.0f);
                        if (mEmitters[0] == NULL) {
                            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x199, 1, 5, NULL, NULL, true, 1.0f);
                        }
                        if (mEmitters[1] == NULL) {
                            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x19A, 1, 5, NULL, NULL, true, 1.0f);
                        }
                    }
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0x274 = 0.0f;
                    field_0x278 = 0.0f;
                    field_0x27c = 0.0f;
                    for (int i = 0; i < 13; i++) {
                        setEmitterPos_8013CB44(mEmitters[i], &mPos, 1, 5, true);
                    }
                }
            }
        }
    }
}

// FIXME: 89.43. Scheduling and registers: the original keeps the layer in r31,
// the player index in r29 and the position pointer in r30; ours has the layer
// in r29, the index in r30 and the pointer in r31. The .sdata2 literal pairs
// once the unit's literals are laid out.
void cls_804A2944::fn_8019C500(s32 keepY) {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            s32 layer = field_0x4;
            if (layer == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f pos(field_0x2a4[i].x, field_0x2a4[i].y, field_0x2a4[i].z);
                if (!keepY) {
                    pos.y = 32.0f + mPos.y;
                }
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            }
        }
    }
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8019C4F4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// FIXME(shotgun): 80.13 %, missing/extra + reloc-name.
void cls_804A2944::fn_80198828() {
    JGeometry::TVec3f savedPos;
    fn_801F3D94(1);
    if (field_0x23c == 0 && (mState == 0x1F || mState == 0x21)) {
        if (fn_801F2DB4('NULL', 'NULL', NULL) && mLife != 10000) {
            field_0x274 = 0.0f;
            field_0x278 = 0.0f;
            field_0x27c = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            mState = 0x25;
        }
    }
    JGeometry::TVec3f origin0(field_0x280[0].x, field_0x280[0].y, field_0x280[0].z);
    if (field_0x260 >= 0) {
        field_0x258 += lbl_8053AB9C / 4;
        lbl_8053AB9C = 0;
        if (field_0x258 > 5) {
            field_0x258 = 5;
        }
        ActorBase_804A5770* limit = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x260);
        if (limit != NULL) {
            static_cast<LifeMeter_804B174C*>(limit)->fn_8019CEF8(mSerial, -28.0f, field_0x258);
        }
        if (mEmitters[0] != NULL) {
            JGeometry::TVec3f savedPos;
            fn_8019CECC(&savedPos, &mPos);
            fn_8019CECC(&mPos, &origin0);
            startSound_801F0E34(0x30058, 0);
            fn_8019CECC(&mPos, &savedPos);
        }
    }
    if (mState == 3 || (mState >= 6 && mState <= 0x12) || mState == 0x13) {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &origin0);
        focusPos.y += 32.0f;
        fn_8017EB44(&focusPos);
        fn_8017EB1C(0.05f);
    }
    if (mState == 0x2E || mState == 0x2F || (mState >= 0x33 && mState <= 0x43) || mState == 0x44) {
        if (fn_8019CE40(7, 3)) {
            fn_8019CDC4(12);
            fn_8019CEE8(1);
        }
    }
    if (mState == 0x2E || mState == 0x2F || (mState >= 0x33 && mState <= 0x3F) || mState == 0x40) {
        JGeometry::TVec3f savedPos;
        fn_8019CECC(&savedPos, &mPos);
        fn_8019CECC(&mPos, &origin0);
        startSound_801F0E34(0x5000A, 0);
        fn_8019CECC(&mPos, &savedPos);
    }

    switch (mState) {
    case 0:
        field_0x234 = 3;
        if (!getActorManager_801F856C()->fn_801F68E0()) {
            break;
        }
        spawnEmitter_8013CC50(field_0x4, &origin0, 0x4A8, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &origin0, 0x4A9, 0, 0, NULL, NULL, true, 1.0f);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                if (i == 0) {
                    fn_8019CECC(&field_0x2a4[i], getPlayerPos_8023E724(i));
                    field_0x2a4[i].x = mPos.x - 24.0f;
                } else {
                    fn_8019CECC(&field_0x2a4[i], &field_0x2a4[0]);
                    field_0x2a4[i].x += 16.0f * (f32)i;
                }
                fn_8019CECC(&field_0x2d4[i], &field_0x2a4[i]);
                fn_80230498(i, &field_0x2a4[i], 0xC3, 0xFF, -0x80, -0x80, true);
            }
        }
        field_0x248 = 0;
        field_0x244 = 4;
        field_0x234 = 0xB4;
        mState = 1;
        // fall through
    case 1:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x244--;
            if (field_0x244 <= 0) {
                field_0x244 = 0;
                field_0x234 = 50;
                field_0x254 = lbl_804A289C[field_0x244];
                mState = 2;
            } else {
                field_0x234 = lbl_804A28B0[field_0x244];
                field_0x254 = lbl_804A289C[field_0x244];
            }
        }
        break;
    case 2:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x98 = 0;
            field_0x234 = 3;
            field_0x254 = (field_0x98 << 3) + field_0x244;
            mState = 3;
        }
        break;
    case 3:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        if (field_0x98 == 3) {
            field_0x278 = -1.0f;
        }
        if (field_0x234 == 0) {
            field_0x98 = 3;
            field_0x254 = (field_0x98 << 3) + field_0x244;
            field_0x234 = 3;
            field_0x244 = (field_0x244 + 1) & 7;
        }
        if (mPos.y <= field_0x2a4[0].y - 48.0f) {
            field_0x274 = 0.0f;
            field_0x278 = 0.0f;
            field_0x27c = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x244 = 0;
            field_0x254 = (field_0x98 << 3) + field_0x244;
            field_0x248++;
            if (field_0x248 >= 50) {
                field_0x248 = 0;
                JGeometry::TVec3f pos;
                fn_8019CECC(&pos, &mPos);
                pos.y -= 16.0f;
                startSound_801F0E34(0x400B5, 0);
                spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
                mState = 4;
            }
        }
        break;
    case 4:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        field_0x248++;
        if (field_0x248 >= 50) {
            field_0x248 = 0;
            mState = 5;
        } else {
            break;
        }
        // fall through
    case 5: {
        field_0x248++;
        if (field_0x248 < 200) {
            JGeometry::TVec3f pos;
            fn_8019CECC(&pos, &origin0);
            fn_8017EB44(&pos);
            fn_8017EB1C(0.05f);
            break;
        }
        field_0x248 = 200;
        JGeometry::TVec3f pos;
        fn_8019CECC(&pos, &origin0);
        pos.y += 32.0f;
        fn_8017EB44(&pos);
        fn_8017EB1C(0.05f);
        field_0x278 = -1.0f;
        if (field_0x234 == 0) {
            field_0x254 = (field_0x98 << 3) + field_0x244;
            field_0x234 = 3;
            field_0x244 = (field_0x244 + 1) & 7;
        }
        if (mPos.y <= field_0x2a4[0].y - 112.0f) {
            field_0x274 = 0.0f;
            field_0x278 = 0.0f;
            field_0x27c = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x234 = 8;
            field_0x98 = 3;
            field_0x244 = 0;
            field_0x254 = (field_0x98 << 3) + field_0x244;
            JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
            fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
            if (fn_8038AD24(fn_8022EA0C(0), 0x2D8, mSerial, field_0x4, msgPos, 0x4102)) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8023F264(i)) {
                        fn_80230498(i, getPlayerPos_8023E724(i), 0x3F, 0xFF, -0x80, -0x80, true);
                    }
                }
                mState = 6;
            }
        }
        break;
    }
    case 6:
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            field_0x98 = 0;
            field_0x244 = 0;
            field_0x254 = (field_0x98 << 3) + field_0x244;
            field_0x234 = 8;
            field_0x248 = 0;
            mState = 7;
        } else {
            break;
        }
        // fall through
    case 7:
        if (field_0x234 == 0) {
            field_0x268 = 1;
            field_0x254 = lbl_804A2700[field_0x244];
            field_0x234 = lbl_804A2700[7 + field_0x244];
            field_0x244++;
            if (field_0x244 > 2) {
                mState = 8;
            }
        }
        break;
    case 8:
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[field_0x244];
            field_0x234 = lbl_804A2700[7 + field_0x244];
            if (field_0x244 == 3) {
                JGeometry::TVec3f pos;
                fn_8019CECC(&pos, &mPos);
                pos.y -= 48.0f;
                field_0x264 = getActorManager_801F856C()->spawnActor_801F9484('ZLD2', field_0x4, &pos, 1, -1, -1);
            }
            field_0x98 = 0;
            field_0x244++;
            if (field_0x244 > 6) {
                field_0x244 = 0;
                field_0x98 = 3;
                mState = 9;
            }
        }
        break;
    case 9:
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[14 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        field_0x248++;
        if (field_0x248 >= 50) {
            field_0x248 = 0;
            mState = 10;
        } else {
            break;
        }
        // fall through
    case 10: {
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[14 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_8038AD24(fn_8022EA0C(0), 0x2D9, mSerial, field_0x4, msgPos, 0x4102)) {
            mState = 11;
        }
        break;
    }
    case 11:
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[14 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            field_0x248 = 0;
            field_0x24c = 0;
            mState = 12;
        }
        break;
    case 12:
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[14 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        field_0x248++;
        if (field_0x248 >= 180) {
            JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
            fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
            if (fn_8038AD24(fn_8022EA0C(0), 0x2DA, mSerial, field_0x4, msgPos, 0x4102)) {
                field_0x248 = 0;
                mState = 13;
            }
        }
        break;
    case 13:
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[14 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            field_0x244 = 0;
            if (field_0x264 >= 0) {
                cls_804A2944* actor = static_cast<cls_804A2944*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264));
                if (actor != NULL) {
                    actor->field_0x258 = 0;
                }
                field_0x264 = -1;
            }
            mState = 14;
        }
        break;
    case 14:
        if (field_0x234 == 0) {
            field_0x254 = lbl_8053A0E8[field_0x244];
            field_0x234 = lbl_8053A0F0[field_0x244];
            field_0x244++;
            if (field_0x244 > 1) {
                field_0x244 = 1;
            }
        }
        field_0x248++;
        if (field_0x248 >= 100) {
            field_0x248 = 0;
            JGeometry::TVec3f pos;
            fn_8019CECC(&pos, &mPos);
            pos.y -= 16.0f;
            spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x29A, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x29B, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x29C, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x29D, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[3] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x292, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[4] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x293, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[5] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x294, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[6] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x295, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[7] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x296, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[8] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x297, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[9] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x298, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[10] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x299, 0, 0, NULL, NULL, true, 1.0f);
            field_0x244 = 0;
            field_0x24c = 0x140;
            field_0x98 = 3;
            mState = 15;
        }
        break;
    case 15:
        startSound_801F0E34(0x30057, 0);
        field_0x24c--;
        field_0x248++;
        if (field_0x248 >= 20) {
            field_0x244 = 0;
            field_0x268 = 0;
            field_0x254 = (field_0x98 << 3) + field_0x244;
            field_0x234 = 20;
            mState = 16;
        }
        break;
    case 16:
        startSound_801F0E34(0x30057, 0);
        field_0x24c--;
        if (field_0x24c < 40) {
            field_0x98 = 2;
            field_0x244 = 0;
            startSound_801F0E34(0x400B6, 0);
            mState = 17;
            break;
        }
        if (field_0x244 <= 2 && field_0x234 == 0) {
            if (field_0x98 == 3 || field_0x98 == 1) {
                field_0x98 = 0;
                field_0x244++;
            } else if (field_0x98 == 0) {
                field_0x98 = 1;
            }
            field_0x254 = field_0x98 << 3;
            field_0x234 = 20;
        }
        break;
    case 17:
        startSound_801F0E34(0x30057, 0);
        field_0x24c--;
        if (field_0x234 == 0) {
            field_0x268 = 1;
            field_0x254 = lbl_804A2700[52 + field_0x244];
            field_0x234 = lbl_804A2700[58 + field_0x244];
            field_0x244++;
            if (field_0x244 >= 2) {
                field_0x244 = 0;
                mState = 0x12;
            }
        }
        break;
    case 0x12:
        startSound_801F0E34(0x30057, 0);
        field_0x24c--;
        if (field_0x24c <= 0) {
            field_0x24c = 0;
            field_0x26c = 400.0f;
            if (mPos.z > 200.0f) {
                mPos.z = field_0x26c;
                field_0x248 = 0;
                mState = 0x13;
            }
        }
        for (s32 i = 3; i < 13; i++) {
            setEmitterPos_8013CB44(mEmitters[i], &mPos, 0, 0, true);
        }
        break;
    case 0x13:
        field_0x248++;
        if (field_0x248 >= 10) {
            field_0x248 = 0;
            field_0x24c = 600;
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x1C1, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[11] = spawnEmitter_8013CC50(field_0x4, &origin0, 0x1C2, 0, 0, NULL, NULL, true, 1.0f);
            mEmitters[12] = spawnEmitter_8013CC50(field_0x4, &origin0, 0x1C3, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x1C4, 0, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f savedPos;
            fn_8019CECC(&savedPos, &mPos);
            fn_8019CECC(&mPos, &origin0);
            startSound_801F0E34(0x400B8, 0);
            fn_8019CECC(&mPos, &savedPos);
            mState = 0x14;
        }
        break;
    case 0x14: {
        JGeometry::TVec3f savedPos;
        fn_8019CECC(&savedPos, &mPos);
        fn_8019CECC(&mPos, &origin0);
        startSound_801F0E34(0x30059, 0);
        fn_8019CECC(&mPos, &savedPos);
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &origin0);
        focusPos.y -= 40.0f;
        fn_8017EB44(&focusPos);
        fn_8017EB1C(0.05f);
        field_0x24c--;
        if (field_0x24c < 0) {
            field_0x248 = 0;
            field_0x24c = 0;
            fn_80119DD0()->fn_80119998(100, 0, 0, 0, 0, 0, 0);
            if (mEmitters[11] != NULL) {
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[11]);
                mEmitters[11] = NULL;
            }
            if (mEmitters[12] != NULL) {
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[12]);
                mEmitters[12] = NULL;
            }
            field_0x25c = getActorManager_801F856C()->spawnActor_801F9484('GNON', field_0x4, &origin0, 0, -1, -1);
            if (field_0x25c >= 0) {
                ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
                if (ganon != NULL) {
                    static_cast<Ganon_804A25F0*>(ganon)->fn_8019CEC4(mSerial);
                }
                if (mEmitters[0] != NULL) {
                    getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[0]);
                    mEmitters[0] = NULL;
                }
                if (mEmitters[1] != NULL) {
                    getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[1]);
                    mEmitters[1] = NULL;
                }
                mState = 0x15;
            }
        }
        break;
    }
    case 0x15: {
        if (field_0x25c >= 0) {
            ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
            if (ganon != NULL) {
                if (ganon->fn_8018A590() == 1) {
                    s32 dir;
                    fn_801EEC98(mSerial, NULL, &dir, NULL, NULL, 0, 0);
                    if (dir >= 0) {
                        field_0x98 = dir;
                    }
                } else {
                    field_0x234 = 1;
                }
            }
        }
        ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
        if (ganon != NULL && ganon->fn_8018A590() == 2) {
            field_0x26c = 0.0f;
            fn_8019C500(0);
            mState = 0x16;
        }
        break;
    }
    case 0x16:
        field_0x244 = 0;
        field_0x248 = 0;
        field_0x234 = 0;
        field_0x98 = 2;
        if (mEmitters[9] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[9]);
            mEmitters[9] = NULL;
        }
        if (mEmitters[10] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[10]);
            mEmitters[10] = NULL;
        }
        mEmitters[9] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A4, 0, 0, NULL, NULL, true, 1.0f);
        mEmitters[10] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A5, 0, 0, NULL, NULL, true, 1.0f);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                fn_8019CECC(&field_0x2d4[i], getPlayerPos_8023E724(i));
                if (fn_80236B50(i)) {
                    fn_8023764C(i, 1);
                }
            }
        }
        field_0x268 = 1;
        mState = 0x17;
        // fall through
    case 0x17: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        focusPos.y -= focusPos.z;
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        for (s32 i = 3; i < 13; i++) {
            setEmitterPos_8013CB44(mEmitters[i], &mPos, 0, 0, true);
        }
        if (mPos.z < 1.0f) {
            field_0x248 = 0xFF;
            mPos.z = 0.0f;
            mState = 0x30;
        } else {
            startSound_801F0E34(0x50003, 0);
        }
        break;
    }
    case 0x30: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        focusPos.y -= focusPos.z;
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        for (s32 i = 3; i < 13; i++) {
            setEmitterPos_8013CB44(mEmitters[i], &mPos, 0, 0, true);
        }
        field_0x248 -= 4;
        if (field_0x248 <= 0) {
            field_0x248 = 0;
            mState = 0x31;
        } else {
            if (mEmitters[9] != NULL) {
                mEmitters[9]->setGlobalAlpha((u8)field_0x248);
            }
            if (mEmitters[10] != NULL) {
                mEmitters[10]->setGlobalAlpha((u8)field_0x248);
            }
        }
        break;
    }
    case 0x31: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        focusPos.y -= focusPos.z;
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[17 + field_0x244];
            field_0x234 = lbl_804A2700[24 + field_0x244];
            if (field_0x244 == 1) {
                for (s32 i = 0; i < 13; i++) {
                    if (mEmitters[i] != NULL) {
                        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[i]);
                        mEmitters[i] = NULL;
                    }
                }
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x29E, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x29F, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A0, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A1, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A2, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A3, 0, 0, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x50068, 0);
            }
            field_0x244++;
            if (field_0x244 > 6) {
                field_0x244 = 0;
                mState = 0x32;
            }
        }
        break;
    }
    case 0x32: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        focusPos.y -= focusPos.z;
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
            fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
            if (fn_8038AD24(fn_8022EA0C(0), 0x2DB, mSerial, field_0x4, msgPos, 0x4102)) {
                field_0x248 = 0;
                mState = 0x18;
            }
        }
        break;
    }
    case 0x18: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            field_0x244 = 0;
            field_0x248 = 0;
            field_0x234 = 0;
            mState = 0x19;
        }
        break;
    }
    case 0x19: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[31 + field_0x244];
            field_0x234 = lbl_804A2700[40 + field_0x244];
            field_0x244++;
            if (field_0x244 > 8) {
                field_0x244 = 0;
                field_0x248 = 1;
            }
        }
        if (field_0x244 == 3 && field_0x234 == 40 && field_0x264 < 0) {
            JGeometry::TVec3f pos;
            fn_8019CECC(&pos, &mPos);
            pos.y -= 48.0f;
            field_0x264 = getActorManager_801F856C()->spawnActor_801F9484('ZLD2', field_0x4, &pos, 1, -1, -1);
        }
        if (field_0x248 != 0) {
            if (field_0x234 == 0) {
                field_0x254 = lbl_804A2700[49 + field_0x244];
                field_0x234 = 5;
                field_0x244++;
                if (field_0x244 > 2) {
                    field_0x244 = 0;
                }
            }
            JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
            fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
            if (fn_8038AD24(fn_8022EA0C(0), 0x2DC, mSerial, field_0x4, msgPos, 0x4102)) {
                field_0x248 = 0;
                mState = 0x1A;
            }
        }
        break;
    }
    case 0x1A: {
        JGeometry::TVec3f focusPos;
        fn_8019CECC(&focusPos, &mPos);
        fn_8017EC70(&focusPos, 1.0f);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[49 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
            if (ganon != NULL) {
                static_cast<Ganon_804A25F0*>(ganon)->fn_8019CEBC(0xB);
            }
            mState = 0x1B;
        }
        break;
    }
    case 0x1B: {
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[49 + field_0x244];
            field_0x234 = 5;
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 0;
            }
        }
        ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
        if (ganon != NULL) {
            if (ganon->fn_8018A590() == 4) {
                JGeometry::TVec3f focusPos;
                fn_8019CECC(&focusPos, &mPos);
                fn_8017EC70(&focusPos, 1.0f);
                fn_8017EB1C(0.05f);
            }
            ganon->getPos();
            ganon->getPos();
            if (ganon->fn_8018A590() == 3) {
                field_0x280[1].x = mPos.x;
                field_0x280[1].y = mPos.y;
                field_0x280[1].z = mPos.z;
                field_0x80.set(-16.0f, -16.0f, 16.0f, 16.0f);
                mState = 0x1C;
            }
        }
        break;
    }
    case 0x1C:
        field_0x244 = 0;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x23c = 0;
        field_0x240 = 0;
        field_0x98 = 2;
        field_0x278 = 1.0f;
        mState = 0x1D;
        // fall through
    case 0x1D:
        if (field_0x264 >= 0) {
            cls_804A2944* actor = static_cast<cls_804A2944*>(
                getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264));
            if (actor != NULL) {
                JGeometry::TVec3f pos;
                fn_8019CECC(&pos, &mPos);
                pos.y -= 40.0f;
                actor->field_0x25c = field_0x25c;
                actor->fn_80182660(pos);
            }
        }
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[64 + (field_0x98 << 3) + field_0x244];
            field_0x234 = 3;
            field_0x244 = (field_0x244 + 1) & 7;
        }
        if (fn_801545C0(NULL, false)) {
            field_0x274 = 0.0f;
            field_0x278 = 0.0f;
            field_0x27c = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            mState = 0x1E;
        }
        break;
    case 0x1E: {
        field_0x280[2].x = field_0x280[1].x + (128.0f - 256.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x280[2].y = field_0x280[1].y + (96.0f + 16.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x280[2].z = field_0x280[1].z;
        f32 dx = field_0x280[2].x - mPos.x;
        f32 dy = field_0x280[2].y - mPos.y;
        field_0x274 = 0.7f * JMath::sincosTable_.sinShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        field_0x278 = 0.7f * JMath::sincosTable_.cosShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        field_0x98 = fn_801F681C((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x238 = 0;
        if (field_0x264 < 0) {
            JGeometry::TVec3f pos;
            fn_8019CECC(&pos, &mPos);
            pos.y -= 48.0f;
            field_0x264 = getActorManager_801F856C()->spawnActor_801F9484('ZLD2', field_0x4, &pos, 1, -1, -1);
        }
        field_0x268 = 1;
        mState = 0x1F;
        // fall through
    }
    case 0x1F: {
        if (field_0x234 == 0) {
            field_0x254 = lbl_804A2700[64 + (field_0x98 << 3) + field_0x244];
            field_0x234 = 3;
            field_0x244 = (field_0x244 + 1) & 7;
        }
        if (field_0x264 >= 0) {
            cls_804A2944* actor = static_cast<cls_804A2944*>(
                getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264));
            if (actor != NULL) {
                JGeometry::TVec3f pos;
                fn_8019CECC(&pos, &mPos);
                pos.y -= 40.0f;
                actor->field_0x25c = field_0x25c;
                actor->fn_80182660(pos);
            }
        }
        ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
        if (ganon != NULL && ganon->fn_8019CEB4() == 0xE && field_0x264 >= 0) {
            mState = 0x22;
            break;
        }
        if (fn_801545C0(NULL, false)) {
            field_0x274 = 0.0f;
            field_0x278 = 0.0f;
            field_0x27c = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            mState = 0x20;
            break;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
                f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
                if (std::sqrtf(dx * dx + dy * dy) < 24.0f) {
                    u16 ang = (s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f) + 0x8000;
                    s32 dir = fn_801F681C((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
                    field_0x274 = 1.2f * JMath::sincosTable_.sinShort(ang);
                    field_0x278 = 1.2f * JMath::sincosTable_.cosShort(ang);
                    field_0x238 = 20;
                    field_0x98 = dir ^ 1;
                    return;
                }
            }
        }
        if (mPos.y < field_0x280[1].y + 96.0f || field_0x238 == 1) {
            s16 ang = (s16)(8192.0f - 16384.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            field_0x238 = 10;
            field_0x274 = 0.7f * JMath::sincosTable_.sinShort(ang);
            field_0x278 = 0.7f * JMath::sincosTable_.cosShort(ang);
            field_0x98 = fn_801F681C(ang);
        }
        break;
    }
    case 0x20:
        field_0x98 = 3;
        field_0x254 = lbl_804A2700[64 + (field_0x98 << 3)];
        mState = 0x21;
        // fall through
    case 0x21: {
        ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
        if (ganon != NULL && ganon->fn_8019CEB4() == 0xE && field_0x264 >= 0) {
            mState = 0x22;
            break;
        }
        u16 ang;
        s32 dir;
        f32 dist;
        fn_801EEC98(mSerial, &ang, &dir, &dist, NULL, 0, 0);
        if (dir >= 0 && dist > 32.0f) {
            field_0x98 = dir;
            field_0x254 = lbl_804A2700[64 + (field_0x98 << 3)];
            mState = 0x1E;
        }
        break;
    }
    case 0x22: {
        JGeometry::TVec3f pos;
        fn_8019CECC(&pos, &mPos);
        pos.y -= 16.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
        field_0x274 = 0.0f;
        field_0x278 = 0.0f;
        field_0x27c = 0.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0x268 = 0;
        mState = 0x23;
        // fall through
    }
    case 0x23: {
        ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
        if (ganon == NULL) {
            break;
        }
        field_0x98 = 3;
        field_0x254 = field_0x98 << 3;
        if (field_0x264 >= 0) {
            cls_804A2944* zld = static_cast<cls_804A2944*>(
                getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264));
            if (zld != NULL) {
                JGeometry::TVec3f vel(0.0f, 0.0f, 0.0f);
                f32 dx = ganon->mPos.x - zld->mPos.x;
                f32 dy = ganon->mPos.y - (zld->mPos.y - zld->mPos.z);
                vel.x = 2.0f * JMath::sincosTable_.sinShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
                vel.y = 2.0f * JMath::sincosTable_.cosShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
                zld->fn_8017EAF8(vel);
                zld->field_0x274 = vel.x;
                zld->field_0x278 = vel.y;
                zld->field_0x27c = vel.z;
                zld->fn_8019CEAC(0x11);
            }
        }
        field_0x234 = 20;
        mState = 0x24;
        break;
    }
    case 0x24: {
        if (field_0x234 == 0) {
            field_0x98 = 3;
            field_0x254 = field_0x98 << 3;
        }
        ActorBase_804A5770* ganon = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c);
        if (ganon != NULL) {
            if (ganon->fn_8018A590() == 6) {
                mState = 0x2E;
                break;
            }
            if (ganon->fn_8018A590() == 5) {
                break;
            }
        } else {
            mState = 0x2E;
            break;
        }
        if (field_0x264 >= 0) {
            if (getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264) == NULL) {
                field_0x264 = -1;
                mState = 0x1E;
            }
        }
        break;
    }
    case 0x25:
        field_0x268 = 0;
        field_0x274 = 0.0f;
        field_0x278 = 0.0f;
        field_0x27c = 0.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        startSound_801F0E34(0x400B6, 0);
        field_0x3c.z = 10.0f;
        mState = 0x26;
        break;
    case 0x26:
        if (field_0x264 >= 0) {
            cls_804A2944* zld = static_cast<cls_804A2944*>(
                getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264));
            if (zld != NULL) {
                zld->field_0x258 = 0;
            }
            field_0x264 = -1;
        }
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        } else {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x258--;
            if (field_0x258 > 0) {
                field_0x23c = 300;
                mState = 0x1E;
            } else {
                mState = 0x27;
            }
        }
        break;
    case 0x27:
        field_0x244 = 0;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x23c = 0;
        field_0x240 = 0;
        startSound_801F0E34(0x400B7, 0);
        for (s32 i = 0; i < 4; i++) {
            if (field_0x98 == lbl_804A288C[i]) {
                field_0x244 = i;
                field_0x234 = 8;
                mState = 0x28;
                break;
            }
        }
        break;
    case 0x28:
        if (field_0x234 == 0) {
            field_0x244 = (field_0x244 + 1) & 3;
            field_0x234 = 8;
            field_0x98 = lbl_804A288C[field_0x244];
            field_0x248++;
            if (field_0x248 >= 12 && field_0x98 == 0) {
                mState = 0x29;
                break;
            }
        }
        field_0x254 = field_0x98 << 3;
        break;
    case 0x29:
        field_0x244 = 0;
        field_0x234 = lbl_804A28B0[field_0x244];
        field_0x254 = lbl_804A289C[field_0x244];
        mState = 0x2A;
        // fall through
    case 0x2A:
        if (field_0x234 == 0) {
            field_0x244++;
            if (field_0x244 > 4) {
                field_0x244 = 0;
                lbl_8053ABA8 = 1;
                mState = 0x2B;
            } else {
                field_0x234 = lbl_804A28B0[field_0x244];
                field_0x254 = lbl_804A289C[field_0x244];
            }
        }
        break;
    case 0x2B:
        if (lbl_8053AB98 == 1) {
            lbl_8053ABA8 = 0;
            lbl_8053AB98 = 0;
            mState = 0x2C;
        }
        break;
    case 0x2C:
        field_0x244 = 4;
        field_0x234 = lbl_804A28B0[field_0x244];
        field_0x254 = lbl_804A289C[field_0x244];
        mState = 0x2D;
        // fall through
    case 0x2D:
        if (field_0x234 == 0) {
            field_0x258++;
            if (field_0x258 > 5) {
                field_0x258 = 5;
            }
            field_0x244--;
            if (field_0x244 < 0) {
                field_0x258 = 5;
                mState = 0x1E;
            } else {
                field_0x234 = lbl_804A28B0[field_0x244];
                field_0x254 = lbl_804A289C[field_0x244];
            }
        }
        break;
    case 0x2E:
        field_0x258 = 5;
        field_0x268 = 0;
        fn_8019CECC(&mPos, &origin0);
        mPos.y += 130.0f;
        field_0x98 = 2;
        field_0x280[0].y -= 32.0f;
        origin0.y = field_0x280[0].y;
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &origin0, 0x40D, 0, 6, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &origin0, 0x40E, 0, 6, NULL, NULL, true, 1.0f);
        }
        field_0x254 = (field_0x98 << 3) + field_0x244;
        fn_8019C500(1);
        field_0x244 = 0;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x23c = 0;
        field_0x240 = 0;
        field_0x234 = 70;
        field_0x238 = 70;
        mState = 0x2F;
        // fall through
    case 0x2F:
        if (field_0x268 == 0) {
            fn_8017EC70(&mPos, 1.0f);
            fn_8017EB1C(0.05f);
        }
        if (field_0x234 == 0) {
            field_0x268 = 1;
            field_0x254 = lbl_804A2700[52 + field_0x244];
            field_0x234 = lbl_804A2700[58 + field_0x244];
            field_0x244++;
            if (field_0x244 > 2) {
                field_0x244 = 2;
            }
        }
        if (field_0x268 == 1 && field_0x238 == 0) {
            JGeometry::TVec3f pos;
            fn_8019CECC(&pos, &field_0x2a4[0]);
            pos.y -= 16.0f;
            s32 idx = field_0x248 & 3;
            if (field_0x2a4[idx].x != 0.0f || field_0x2a4[idx].y != 0.0f) {
                fn_8019CECC(&pos, &field_0x2a4[idx]);
            }
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('ZLD2', field_0x4, &pos,
                                                                         (field_0x248 << 4) | 2, -1, -1);
            if (serial >= 0) {
                cls_804A2944* zld = static_cast<cls_804A2944*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (zld != NULL) {
                    zld->field_0x280[0].x = field_0x280[0].x;
                    zld->field_0x280[0].y = field_0x280[0].y;
                    zld->field_0x280[0].z = field_0x280[0].z;
                }
                field_0x248++;
                field_0x238 = 50;
                if (field_0x248 > 5) {
                    mState = 0x33;
                }
            }
        }
        break;
    case 0x33:
        if (lbl_8053ABA0 >= 6) {
            field_0x248 = 0;
            field_0x238 = 0;
            field_0x234 = 50;
            mState = 0x34;
        }
        break;
    case 0x34:
        if (field_0x234 == 0) {
            field_0x268 = 0;
            field_0x234 = 15;
            if (field_0x98 == 2) {
                field_0x98 = 0;
            } else if (field_0x98 == 0) {
                field_0x98 = 3;
                field_0x234 = 50;
                mState = 0x35;
            }
            field_0x254 = field_0x98 << 3;
        }
        break;
    case 0x35:
        if (field_0x234 == 0) {
            field_0x268 = 1;
            field_0x254 = lbl_804A2700[64 + (field_0x98 << 3)];
            lbl_8053ABA0 = 0;
            lbl_8053ABA4 = 1;
            field_0x248 = 0;
            mState = 0x36;
        }
        break;
    case 0x36:
        if (lbl_8053ABA0 >= 6) {
            field_0x248++;
            if (field_0x248 > 60) {
                lbl_8053ABA0 = 0;
                field_0x248 = 0;
                lbl_8053ABA4 = 2;
                JGeometry::TVec3f savedPos;
                fn_8019CECC(&savedPos, &mPos);
                fn_8019CECC(&mPos, &origin0);
                startSound_801F0E34(0x5000C, 0);
                fn_8019CECC(&mPos, &savedPos);
                field_0x234 = 0;
                mState = 0x37;
            }
        }
        break;
    case 0x37:
        if (lbl_8053ABA4 == 3 && field_0x234 == 0) {
            field_0x268 = 0;
            if (field_0x98 == 3) {
                field_0x98 = 0;
            } else if (field_0x98 == 0) {
                field_0x98 = 2;
            } else if (field_0x98 == 2) {
                mState = 0x38;
                break;
            }
            field_0x234 = 8;
            field_0x254 = field_0x98 << 3;
        }
        break;
    case 0x38: {
        fn_8017EC70(&mPos, 1.0f);
        fn_8017EB1C(0.05f);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
        if (fn_8038AD24(fn_8022EA0C(0), 0x2DE, mSerial, field_0x4, msgPos, 0x4102)) {
            mState = 0x39;
        } else {
            break;
        }
    }
        // fall through
    case 0x39:
        fn_8017EC70(&mPos, 1.0f);
        fn_8017EB1C(0.05f);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            field_0x244 = 0;
            field_0x234 = 20;
            mState = 0x3A;
        } else {
            break;
        }
        // fall through
    case 0x3A:
        if (field_0x234 == 0) {
            field_0x268 = 0;
            if (field_0x98 == 2) {
                field_0x98 = 0;
                field_0x234 = 8;
            } else if (field_0x98 == 0) {
                field_0x234 = 3;
                field_0x244 = (field_0x244 + 1) & 7;
                field_0x274 = 0.7f;
                if (mPos.x > field_0x280[0].x + 32.0f) {
                    field_0x274 = 0.0f;
                    field_0x278 = 0.0f;
                    field_0x27c = 0.0f;
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0x244 = 0;
                    field_0x98 = 2;
                    field_0x238 = 30;
                    for (s32 i = 0; i < 4; i++) {
                        fn_8019CECC(&field_0x2d4[i], &field_0x2a4[i]);
                    }
                    mState = 0x3B;
                }
            }
            field_0x254 = (field_0x98 << 3) + field_0x244;
        }
        break;
    case 0x3B: {
        JGeometry::TVec3f targetPos;
        fn_8019CECC(&targetPos, &field_0x2a4[0]);
        targetPos.x = field_0x280[0].x - 5.0f;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                addCalc2_801F71A4(&field_0x2d4[i].x, targetPos.x, 0.3f, 1.0f);
                addCalc2_801F71A4(&field_0x2d4[i].y, targetPos.y, 0.3f, 1.0f);
                fn_80230498(i, &field_0x2d4[i], 0x3F, 0xFF, -0x80, -0x80, true);
            }
        }
        if (field_0x238 == 0) {
            for (s32 i = 1; i < 4; i++) {
                if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    fn_80230498(i, getPlayerPos_8023E724(i), 0x122, 0xFF, -0x80, -0x80, true);
                }
            }
            fn_8019CECC(&field_0x2d4[0], getPlayerPos_8023E724(0));
            field_0x280[2].y = field_0x2d4[0].y - 32.0f;
            mState = 0x3C;
        }
        break;
    }
    case 0x3C: {
        fn_8019CECC(&savedPos, &mPos);
        addCalc2_801F71A4(&field_0x2d4[0].y, field_0x280[2].y, 0.3f, 1.0f);
        fn_80230BF4(0, 3);
        fn_802303FC(0, &field_0x2d4[0], true);
        if (std::fabsf(field_0x280[2].y - field_0x2d4[0].y) <= 1.0f) {
            field_0x238 = 8;
            field_0x248 = 0;
            mState = 0x3D;
        }
        break;
    }
    case 0x3D:
        if (field_0x248 == 0) {
            fn_80230498(0, getPlayerPos_8023E724(0), 0x3F, 0xFF, -0x80, -0x80, true);
        } else if (field_0x248 == 1) {
            fn_80230498(0, getPlayerPos_8023E724(0), 0x3C, 0xFF, -0x80, -0x80, true);
        } else {
            fn_80230498(0, getPlayerPos_8023E724(0), 0x3E, 0xFF, -0x80, -0x80, true);
        }
        if (field_0x238 == 0) {
            field_0x238 = 8;
            field_0x248++;
            if (field_0x248 == 2) {
                field_0x238 = 20;
            }
            if (field_0x248 >= 3) {
                field_0x238 = 0;
                field_0x248 = 0;
                fn_80230FD0(0, 0x2000E);
                fn_8019CECC(&mPos, &savedPos);
                mState = 0x3E;
            }
        }
        break;
    case 0x3E:
        if (field_0x238 == 0) {
            fn_80230498(0, getPlayerPos_8023E724(0), field_0x248 + 0x31A, 0xFF, -0x80, -0x80, true);
            if (field_0x248 == 0) {
                field_0x238 = 8;
            } else if (field_0x248 == 1) {
                field_0x238 = 3;
            } else if (field_0x248 == 2) {
                field_0x238 = 30;
            }
            field_0x248++;
            if (field_0x248 >= 3) {
                mState = 0x3F;
            }
        }
        break;
    case 0x3F:
        if (field_0x238 == 0) {
            fn_8017EC70(&origin0, 1.0f);
            fn_8017EB1C(0.05f);
            fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
            JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
            if (fn_8038AD24(fn_8022EA0C(0), 0x2DF, mSerial, field_0x4, msgPos, 0x4102)) {
                mState = 0x40;
            }
        }
        break;
    case 0x40:
        fn_8017EC70(&origin0, 1.0f);
        fn_8017EB1C(0.05f);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
                mEmitters[1] = NULL;
            }
            JGeometry::TVec3f savedPos;
            fn_8019CECC(&savedPos, &mPos);
            fn_8019CECC(&mPos, &origin0);
            startSound_801F0E34(0x50061, 0);
            fn_8019CECC(&mPos, &savedPos);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3F4, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3F5, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3F6, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3F7, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3F8, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3F9, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3FA, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x3FB, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, getPlayerPos_8023E724(0), 0, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, getPlayerPos_8023E724(0), 1, 0, 6, NULL, NULL, true, 1.0f);
            mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &origin0, 0x12, 0, 6, NULL, NULL, true, 1.0f);
            mEmitters[3] = spawnEmitter_8013CC50(field_0x4, &origin0, 0x13, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x40F, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x410, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x411, 0, 6, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &origin0, 0x412, 0, 6, NULL, NULL, true, 1.0f);
            field_0x248 = 0;
            mState = 0x41;
        }
        break;
    case 0x41:
        field_0x248++;
        if (field_0x248 < 600) {
            JGeometry::TVec3f savedPos;
            fn_8019CECC(&savedPos, &mPos);
            fn_8019CECC(&mPos, &origin0);
            startSound_801F0E34(0x5000A, 0);
            fn_8019CECC(&mPos, &savedPos);
        }
        if (field_0x248 == 600) {
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
                mEmitters[0] = NULL;
            }
            JGeometry::TVec3f savedPos;
            fn_8019CECC(&savedPos, &mPos);
            fn_8019CECC(&mPos, &origin0);
            startSound_801F0E34(0x5000E, 0);
            fn_8019CECC(&mPos, &savedPos);
        }
        if (field_0x248 == 670) {
            fn_80196D30(0x14A, 0);
        }
        if (field_0x248 == 990) {
            lbl_8053ABA4 = 4;
            startSound_801F0E34(0x5000F, 0);
            fn_8018E8DC(1, true, 0xFF);
        }
        if (field_0x248 == 1000) {
            JGeometry::TVec3f savedPos;
            fn_8019CECC(&savedPos, &mPos);
            fn_8019CECC(&mPos, &origin0);
            startSound_801F0E34(0x50060, 0);
            fn_8019CECC(&mPos, &savedPos);
        }
        if (field_0x248 >= 1440) {
            fn_80119DD0()->fn_80119998(100, 0, 2, 0, 0, 100, 0);
            fn_80230498(0, getPlayerPos_8023E724(0), 0x3F, 0xFF, -0x80, -0x80, true);
            field_0x238 = 200;
            field_0x248 = 0;
            mState = 0x42;
        }
        break;
    case 0x42:
        if (field_0x238 == 50) {
            lbl_8053ABA4 = 5;
            JGeometry::TVec3f pos;
            fn_8019CECC(&pos, &origin0);
            pos.y += 16.0f;
            fn_8019CECC(&mPos, &pos);
            fn_8019C500(0);
        }
        if (field_0x238 != 0) {
            if (field_0x238 < 50) {
                JGeometry::TVec3f pos;
                fn_8019CECC(&pos, &origin0);
                pos.y += 16.0f;
                fn_8019CECC(&mPos, &pos);
                fn_8017EC70(&pos, 1.0f);
                fn_8017EB1C(0.05f);
            }
            break;
        }
        mState = 0x43;
        // fall through
    case 0x43: {
        JGeometry::TVec3f pos;
        fn_8019CECC(&pos, &origin0);
        pos.y += 16.0f;
        fn_8019CECC(&mPos, &pos);
        fn_8017EC70(&pos, 1.0f);
        fn_8017EB1C(0.05f);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        JGeometry::TVec3f msgPos(0.0f, -10.0f, 0.0f);
        if (fn_8038AD24(fn_8022EA0C(0), 0x2E0, mSerial, field_0x4, msgPos, 0x4102)) {
            mState = 0x44;
        }
        break;
    }
    case 0x44: {
        JGeometry::TVec3f pos;
        fn_8019CECC(&pos, &origin0);
        pos.y += 16.0f;
        fn_8019CECC(&mPos, &pos);
        fn_8017EC70(&pos, 1.0f);
        fn_8017EB1C(0.05f);
        fn_8019C500(0);
        fn_8022FE80(fn_8022EA0C(0), field_0x4, mSerial, false, false);
        if (fn_80388CAC(fn_8022EA0C(0), mSerial)) {
            fn_8019CDE4(7, 3);
            fn_8019CD9C(12);
            fn_8019CEE8(1);
            mState = 0x45;
        }
        break;
    }
    }

    if (mState == 0x1F || mState == 0x21 || mState == 0x23 || mState == 0x24 || mState == 0x26 || mState == 0x2D) {
        JGeometry::TBox2f box;
        box = mBox;
        box.addPos(mPos.x, mPos.y);
        fn_80239914(&box, field_0x4, mSerial);
    }

    if (mState == 3) {
        field_0x268 = 0;
        if (field_0x234 == 0) {
            field_0x254 = (field_0x98 << 3) + field_0x244;
            field_0x234 = 3;
            field_0x244 = (field_0x244 + 1) & 7;
        }
    }
}

// The actor `actor` has been reached: the ZLD2 actor gives up one of its field_0x250 tokens
// (inlined twice in fn_80197EB0).
static inline void takeToken(cls_804A2944* self, ActorBase_804A5770* actor) {
    actor->field_0x94 = 5;
    self->field_0x250 -= 1;
    if (self->field_0x250 < 0) {
        self->field_0x250 = 0;
    }
    if (fn_8022F514(self->field_0x250)) {
        lbl_80529DEC.field_0x1a4 = fn_8022F3DC(self->field_0x250);
    } else {
        lbl_80529DEC.field_0x1a4 = self->field_0x250;
    }
    self->field_0x238 = 50;
    self->mState = 0x49;
    deleteEmitter(&self->mEmitters[0]);
    deleteEmitter(&self->mEmitters[1]);
}

// FIXME(shotgun): 92.19 %, missing/extra + reloc-name.
void cls_804A2944::fn_80197EB0() {
    switch (mState) {
    case 0x46:
        mState = 0x47;
        // fall through
    case 0x47: {
        startSound_801F0E34(0x30056, 0);
        if (field_0x250 == 0 && field_0x3c.x == 0.0f && field_0x3c.y == 0.0f) {
            break;
        }
        if (mEmitters[2] != NULL) {
            mEmitters[2]->becomeInvalidEmitter();
            mEmitters[2] = NULL;
        }
        if (fn_801F6758(&mPos, field_0x4, 16.0f)) {
            field_0x258 = 0;
            break;
        }
        s32 serial = field_0x25c;
        if (serial < 0) {
            field_0x258 = 0;
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0x258 = 0;
            break;
        }
        f32 dx = actor->mPos.x - mPos.x;
        f32 dy = actor->mPos.y - mPos.y;
        f32 distSq = dx * dx + dy * dy;
        if (std::sqrtf(distSq) <= 90.0f) {
            field_0x27c = 0.0f;
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x248 = (s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f) + 0x8000;
            field_0x24c = (s32)std::sqrtf(distSq);
            for (int i = 0; i < 13; i++) {
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->mGlobalPScl.x = 1.5f;
                    mEmitters[i]->mGlobalPScl.y = 1.5f;
                }
            }
            mBox.i.x = -10.0f;
            mBox.i.y = -10.0f;
            mBox.f.x = 10.0f;
            mBox.f.y = 10.0f;
            mState = 0x48;
        }
        serial = field_0x25c;
        if (serial < 0) {
            break;
        }
        actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL || field_0x250 == 0) {
            break;
        }
        dx = actor->mPos.x - mPos.x;
        dy = actor->mPos.y - mPos.y;
        if (field_0x250 >= 1 && std::sqrtf(dx * dx + dy * dy) <= 80.0f) {
            takeToken(this, actor);
        } else {
            field_0x258 = 0;
        }
        break;
    }
    case 0x48: {
        startSound_801F0E34(0x30056, 0);
        s32 serial = field_0x25c;
        if (serial < 0) {
            field_0x258 = 0;
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0x258 = 0;
            break;
        }
        if ((u8)actor->field_0xb0 != 0xE) {
            field_0x258 = 0;
            break;
        }
        if (field_0x250 == 0) {
            addCalc2_801F71A4(&mPos.x,
                              actor->mPos.x + field_0x24c * JMath::sincosTable_.sinShort(field_0x248),
                              0.8f, 10.0f);
            addCalc2_801F71A4(&mPos.y,
                              actor->mPos.y + field_0x24c * JMath::sincosTable_.cosShort(field_0x248),
                              0.8f, 10.0f);
            field_0x248 += 0x150;
        } else {
            serial = field_0x25c;
            if (serial < 0) {
                break;
            }
            actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor == NULL || field_0x250 == 0) {
                break;
            }
            f32 dx = actor->mPos.x - mPos.x;
            f32 dy = actor->mPos.y - mPos.y;
            if (field_0x250 >= 1 && std::sqrtf(dx * dx + dy * dy) <= 80.0f) {
                takeToken(this, actor);
            } else {
                field_0x258 = 0;
            }
        }
        break;
    }
    case 0x49:
        if (field_0x238 == 0) {
            field_0x258 = 0;
        }
        break;
    }
    if (field_0x258 <= 0) {
        for (int i = 0; i < 13; i++) {
            if (mEmitters[i] != NULL) {
                mEmitters[i]->becomeInvalidEmitter();
                mEmitters[i] = NULL;
            }
        }
        mAlive = false;
    } else {
        for (int i = 0; i < 13; i++) {
            setEmitterPos_8013CB44(mEmitters[i], &mPos, 1, 5, true);
        }
    }
}

// FIXME(shotgun): 94.26 %, missing/extra.
void cls_804A2944::fn_801979C4() {
    for (int i = 0; i < 13; i++) {
        setEmitterPos_8013CB44(mEmitters[i], &mPos, 0, 4, true);
    }
    field_0x254 += 1;
    if (field_0x254 > 5) {
        field_0x254 = 0;
    }
    switch (mState) {
    case 0x4A:
        field_0x26c = 16.0f;
        mState = 0x4A;
        // fall through
    case 0x4B:
        if (std::fabsf(field_0x26c - mPos.z) < 1.0f) {
            startSound_801F0E34(0x50009, 0);
            mState = 0x4C;
        }
        break;
    case 0x4C: {
        s16 angle = lbl_804A2880[field_0x94];
        f32 tx = field_0x280[0].x + 112.0f * JMath::sincosTable_.sinShort(angle);
        f32 ty = field_0x280[0].y + 112.0f * JMath::sincosTable_.cosShort(angle);
        f32 dx = tx - mPos.x;
        f32 dy = ty - (mPos.y - mPos.z);
        field_0x274 = 2.0f * JMath::sincosTable_.sinShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        field_0x278 = 2.0f * JMath::sincosTable_.cosShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        if (std::sqrtf(dx * dx + dy * dy) < 2.0f) {
            lbl_8053ABA0 += 1;
            mState = 0x4D;
            field_0x27c = 0.0f;
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
        }
        break;
    }
    case 0x4D:
        if (lbl_8053ABA4 == 1) {
            const u16* ids = &lbl_804A28D8[field_0x94 * 3];
            spawnEmitter_8013CC50(field_0x4, &mPos, ids[0], 0, 5, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, ids[1], 0, 5, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, ids[2], 0, 5, NULL, NULL, true, 1.0f);
            field_0x248 = 0;
            mState = 0x4E;
        }
        break;
    case 0x4E:
        field_0x248 += 1;
        startSound_801F0E34(0x5000B, 0);
        addCalc2_801F71A4(&field_0x270, 0.0f, 0.3f, 10.0f);
        if (field_0x248 == 60) {
            field_0x24c = 1;
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
                mEmitters[0] = NULL;
            }
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
                mEmitters[1] = NULL;
            }
        }
        if (field_0x248 == 0x14A) {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('ZLD2', field_0x4, &mPos,
                                                                         (field_0x94 << 4) | 3, -1, -1);
            if (serial >= 0) {
                cls_804A2944* actor = static_cast<cls_804A2944*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (actor != NULL) {
                    actor->field_0x280[0].x = field_0x280[0].x;
                    actor->field_0x280[0].y = field_0x280[0].y;
                    actor->field_0x280[0].z = field_0x280[0].z;
                }
            }
            mAlive = false;
        }
        break;
    }
}

// FIXME(shotgun): 91.86 %, missing/extra.
void cls_804A2944::fn_801976E8() {
    s32 base = field_0x94 * 6;
    addCalc2_801F71A4(&field_0x270, 255.0f, 0.3f, 10.0f);
    switch (mState) {
    case 0x4F:
        lbl_8053ABA0 += 1;
        mState = 0x4F;
        // fall through
    case 0x50:
        if (field_0x234 == 0) {
            field_0x234 = 9;
            field_0x254 += 1;
            if (field_0x254 > 5) {
                field_0x254 = 0;
            }
        }
        if (lbl_8053ABA4 == 2) {
            for (s32 i = 0; i < 6; i++) {
                if (i <= 1 || i == 5) {
                    if (mEmitters[i] == NULL) {
                        mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A28FC[base + i], 0, 5,
                                                             NULL, NULL, true, 1.0f);
                    }
                } else {
                    spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A28FC[base + i], 0, 5, NULL, NULL, true,
                                          1.0f);
                }
            }
            field_0x238 = 0x1E0;
            mState = 0x51;
        }
        break;
    case 0x51:
        if (field_0x234 == 0) {
            field_0x234 = 9;
            field_0x254 += 1;
            if (field_0x254 > 5) {
                field_0x254 = 0;
            }
        }
        if (field_0x238 == 0) {
            lbl_8053ABA4 = 3;
            mState = 0x52;
        }
        break;
    case 0x52:
        if (lbl_8053ABA4 == 4) {
            field_0x26c = 0.0f;
            mPos.z = 0.0f;
            for (int i = 0; i < 6; i++) {
                if (mEmitters[i] != NULL) {
                    getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[i]);
                    mEmitters[i] = NULL;
                }
            }
        }
        if (lbl_8053ABA4 == 5) {
            mPos.y = field_0x280[0].y - 16.0f;
            mPos.x = field_0x280[0].x;
            mPos.x += 28.0f * (2.5f - (f32)field_0x94);
            field_0x254 = 8;
        } else if (field_0x234 == 0) {
            field_0x234 = 9;
            field_0x254 += 1;
            if (field_0x254 > 5) {
                field_0x254 = 0;
            }
        }
        break;
    }
}

// FIXME(shotgun): 98.84 %, missing/extra. The last two timer decrements go through a shared base register (this+8) in the original.
void cls_804A2944::update() {
    JGeometry::TVec3f pos = mPos;
    mLife = 10000;
    addCalc2_801F71A4(&field_0x3c.x, field_0x274, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x278, 0.3f, 1.0f);
    addCalc2_801F71A4(&mPos.z, field_0x26c, 0.3f, 1.0f);
    mPos.add(field_0x3c);
    switch (field_0x90) {
    case 0:
        fn_80198828();
        break;
    case 1:
        fn_80197EB0();
        break;
    case 2:
        fn_801979C4();
        pos.x += 14.0f;
        pos.y -= 64.0f;
        break;
    case 3:
        fn_801976E8();
        pos.x += 14.0f;
        pos.y -= 64.0f;
        break;
    }
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
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
}

// FIXME(shotgun): 82.45 %, missing/extra. The original addresses the frame tables and sprite bank names off one .rodata base (r31); here the names are plain literals and the tables extern.
void cls_804A2944::draw() {
    switch (field_0x90) {
    case 0: {
        SpriteParams_801F5AE0 params;
        if (field_0x23c == 0 || (field_0x23c & 2)) {
            params.field_0x1f = 0;
            s32 frame = 0x9B;
            if (field_0x268 == 1) {
                frame = 0x274;
            }
            s32 flags = 7;
            if (mState == 0x15) {
                flags = -1;
            }
            drawSprite_801F06F0("enemy_both2_sob.bin", frame + field_0x254, &params, flags, 1.0f, 1.0f);
        }
        break;
    }
    case 2:
        if (field_0x24c == 0) {
            SpriteParams_801F5AE0 params;
            params.field_0x1d = 5;
            params.field_0x30 = 0x11;
            fn_801F02BC("enemy_both2_sob.bin", field_0x254 + lbl_80468428[field_0x94], &params, -1, 0, false,
                        1.0f, 1.0f);
            params.field_0x10.a = (u8)field_0x270;
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_both2_sob.bin",
                            field_0x254 + lbl_80468428[field_0x94], NULL);
            }
        }
        break;
    case 3: {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = 5;
        params.field_0x1f = 0;
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x254 + lbl_80468428[6 + field_0x94], &params, -1,
                            1.0f, 1.0f);
        params.field_0x10.a = (u8)field_0x270;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin",
                        field_0x254 + lbl_80468428[6 + field_0x94], NULL);
        }
        break;
    }
    }
}

