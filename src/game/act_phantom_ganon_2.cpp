#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/act/act_phantom_ganon_2.h"
#include "game/SpriteParams.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Tables of the FGN2 actor (.data 0x804A1960, not written as source here): the start
// position and "FGAN" sprite names per player, emitter ids and sprite offsets.
extern const f32 lbl_804A1960[][3];
extern const char* const lbl_804A1A10[];
extern const f32 lbl_804A1A28[][2];
extern const char* const lbl_804A1A50[];
extern const char* const lbl_804A1A64[];
extern const char* const lbl_804A1A84[];
extern const u16 lbl_804A19F0[];
extern u32 lbl_804C17A8[4];
extern s32 lbl_80539FB8;
extern s32 lbl_8053AB7C;
extern s32 lbl_8053AB80;
extern u8 lbl_80539FBC;
extern u8 lbl_8053AB84;

// .sbss 0x8053AB78 (a mode word; the unwritten update and init also use it).
s32 lbl_8053AB78;

// FIXME: __sinit_act_phantom_ganon_2_cpp (compiler-generated, no source line of its own)
// is 99.93 %: the JAudio pool records are 0x10 higher in the original's .bss
// (lbl_804C17A8 + 0x10); the first 0x10 bytes belong to a static that the
// unwritten update (fn_80187530) defines.
//
// Not written yet: draw (0x801871A0), update (0x80187530), init (0x8018A070),
// the JPABaseEmitter helpers 0x8018A5C8-0x8018A5E0 (offsets 0xBB / 0xF4 do not
// fit include/JSystem/JParticle/JPAEmitter.h) and ActorBase::setLife
// (0x8018A588, an inline whose out-of-line copy is only emitted when a caller
// does not inline it).

AudioMgr_8045A080* fn_8018A6AC() {
    return gAudioMgr_8053AF80;
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8018A6A0(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8018A684(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

s32 fn_8018A660() {
    return getActorManager_801F856C()->fn_801F68E0();
}

void fn_8018A63C() {
    getActorManager_801F856C()->fn_801F6884();
}

// Whether bit `player` of the game root's mPlayerMask is set (game/PlayerBit.h,
// on gGameRoot_8053AAF8 itself).
bool fn_8018A614(s32 player) {
    return isPlayerBitSet(gGameRoot_8053AAF8, player);
}

// The effect system's particle emitter manager, fetched through a getter.
JPAEmitterManager* fn_8018A5F0() {
    return getEffectSystem_8013C824()->mEmitterManager;
}

// Out-of-line copy of JPABaseEmitter::setStatus(4).
void fn_8018A5E0(JPABaseEmitter* emitter) {
    emitter->setStatus(4);
}

// Out-of-line copy of JPABaseEmitter::clearStatus(4).
void fn_8018A5D0(JPABaseEmitter* emitter) {
    emitter->clearStatus(4);
}

// Out-of-line copy of JPABaseEmitter::setGlobalAlpha(alpha).
void fn_8018A5C8(JPABaseEmitter* emitter, u8 alpha) {
    emitter->setGlobalAlpha(alpha);
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_8018A5B8(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

bool ActorBase_804A5770::fn_8018A5B0() const {
    return mAlive;
}

void ActorBase_804A5770::fn_8018A5A8(u16 value) {
    field_0x94 = value;
}

void ActorBase_804A5770::fn_8018A5A0(s32 value) {
    field_0x98 = value;
}

void ActorBase_804A5770::fn_8018A598(bool alive) {
    mAlive = alive;
}

// fn_8018A588 (setLife) is ActorBase's inline setter.

u16 ActorBase_804A5770::fn_8018A590() const {
    return field_0x94;
}

// FIXME(shotgun): 56.67 %, missing/extra + reloc-name. Constants and the stmw prologue are scheduled differently; the tables are extern.
void PhantomGanon2_804A1A98::init() {
    field_0x94 = (field_0x90 >> 4) & 0xF;
    field_0x90 &= 0xF;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x196 = 1;
    field_0x26c = 0;
    field_0x2ac = 0.0f;
    field_0x2b8 = 0.0f;
    field_0x2b4 = 0.0f;
    field_0x2b0 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        mEmitters2f4[i] = NULL;
        mEmitters304[i] = NULL;
    }
    mEmitter314 = NULL;
    mPos.z = 1.0f;
    field_0x280 = 2.0f;
    field_0x28c = 0.0f;
    field_0x2ef = 0;
    field_0x274 = -1;
    field_0x278 = 2;
    field_0x2ec = 0;
    field_0x294 = 255.0f;
    field_0x2a0.x = lbl_804A1960[field_0x94][0];
    field_0x298 = 255.0f;
    field_0x2a0.y = lbl_804A1960[field_0x94][1];
    field_0x29c = 255.0f;
    field_0x2a0.z = lbl_804A1960[field_0x94][2];
    mLife = 10000;
    field_0x2f0 = 0;
    field_0x290 = 255.0f;
    field_0x284 = 0.0f;
    field_0x288 = 0.0f;
    field_0x2c8 = 1.0f;
    field_0x2cc = 1.0f;
    field_0x2d0 = 1.0f;
    field_0x2bc.set(mPos);
    field_0x2bc.y += 8.0f;
    field_0x2d4.set(mPos);
    switch (field_0x90) {
    case 1:
        mBox.i.x = -16.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        field_0x290 = 0.0f;
        field_0x2d4.x += 48.0f * (f32)field_0x94;
        field_0x114 = 2;
        field_0x288 = 40.0f;
        field_0x2ee = 1;
        field_0x230 = 5;
        break;
    case 10:
        for (int i = 0; i < 4; i++) {
            mEmitters2f4[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A19F0[i * 2], 1, 2, NULL, NULL, true, 1.0f);
            mEmitters304[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A19F0[i * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
            if (i != field_0x94) {
                if (mEmitters2f4[i] != NULL) {
                    mEmitters2f4[i]->setStatus(4);
                }
                if (mEmitters304[i] != NULL) {
                    mEmitters304[i]->setStatus(4);
                }
            }
        }
        field_0x2f0 = 11;
        if (lbl_8053AB78 == 1) {
            field_0x2f0 = 6;
        }
        // fall through
    case 11:
        field_0x2d0 = 0.0f;
        field_0x2cc = 0.0f;
        field_0x2c8 = 0.0f;
        field_0x2ee = 2;
        mBox.i.x = -7.0f;
        mBox.i.y = -7.0f;
        mBox.f.x = 7.0f;
        mBox.f.y = 7.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        if (field_0x90 == 11) {
            mEmitters2f4[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x37A, 1, 2, NULL, NULL, true, 1.0f);
            mEmitters304[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x37B, 1, 2, NULL, NULL, true, 1.0f);
        }
        field_0x118 = 4;
        field_0x230 = 29;
        break;
    case 12:
        field_0x234 = 30;
        field_0x2ee = 2;
        mBox.i.x = -2.0f;
        mBox.i.y = -2.0f;
        mBox.f.x = 2.0f;
        mBox.f.y = 2.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        field_0x118 = 0;
        field_0x230 = 30;
        break;
    default:
        mBox.i.x = -16.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        field_0x290 = 0.0f;
        mPos.x -= 80.0f;
        field_0x2e0.set(mPos);
        field_0x2d4.set(mPos);
        getGameView_802D6A9C()->fn_802D7440(false);
        lbl_80539FB8 = 100;
        lbl_8053AB78 = 0;
        lbl_8053AB7C = 0;
        lbl_8053AB80 = 0;
        lbl_80539FBC = 0;
        lbl_8053AB84 = 0;
        for (int i = 0; i < 4; i++) {
            lbl_804C17A8[i] = 0;
        }
        field_0x114 = 2;
        field_0x288 = 40.0f;
        field_0x2ee = 1;
        field_0x230 = 0;
        break;
    }
    field_0x27c = field_0x94;
    field_0x270 = lbl_80539FB8 << 2;
}

// FIXME: 99.7. Regalloc only: the target keeps the serial in r31 (the register of the
// emitter loop's pointer), we keep it in r30 (the loop counter's).
PhantomGanon2_804A1A98::~PhantomGanon2_804A1A98() {
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2f4[i]);
        releaseEmitter_801EE434(mEmitters304[i]);
    }
    releaseEmitter_801EE434(mEmitter314);
    if (field_0x90 == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    if (field_0x90 == 10) {
        s32 serial = field_0x274;
        if (serial >= 0) {
            PhantomGanon2_804A1A98* other = (PhantomGanon2_804A1A98*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (other != NULL) {
                other->field_0x28c = 3.0f;
                field_0x274 = -1;
            }
        }
    }
}

// FIXME: 98.9. Regalloc only: the target keeps this in r30 and the two layer reads in r29,
// we use r29 for this and r30/r28 for the counter and the layer reads.
u16 PhantomGanon2_804A1A98::fn_80189DEC() {
    s32 player = fn_8022EA0C(field_0x94);
    if (player < 0) {
        player = 0;
    }
    if (!(fn_8023F264(player) && getField_0x4() == getPlayerLayer_8023DE58(player) && !fn_80230188(player) &&
          !fn_80236B50(player) && lbl_8053AB78 != 1))
    {
        s32 pick = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        for (s32 i = 0; i < 4; i++) {
            if (pick != player && fn_8023F264(pick) && getField_0x4() == getPlayerLayer_8023DE58(pick) &&
                pick == fn_8022EA0C(pick) && !fn_80230188(pick) && !fn_80236B50(pick) &&
                !fn_8022DCD8(pick))
            {
                field_0x94 = pick;
                if (lbl_8053AB78 == 1) {
                    field_0x27c = field_0x94;
                }
                return field_0x94;
            }
            pick = (pick + 1) & 3;
        }
    }
    field_0x94 = player;
    return field_0x94;
}

// FIXME(shotgun): 77.36 %, missing/extra + reloc-name. The original addresses the tables off one base register (r31), here they are extern arrays.
void PhantomGanon2_804A1A98::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f pos = mPos;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1f = 0;
    if (field_0x90 == 0 || field_0x90 == 1) {
        mPos.z = 0.0f;
        f32 scaleX = 0.0f;
        f32 scaleY = 0.0f;
        void* res;
        switch (field_0x278) {
        case 0:
            res = fn_804032E8('FGAN', lbl_804A1A10[field_0x26c]);
            break;
        case 1:
            res = fn_804032E8('FGAN', lbl_804A1A50[field_0x26c]);
            scaleX = lbl_804A1A28[field_0x26c][0];
            scaleY = lbl_804A1A28[field_0x26c][1];
            break;
        case 2:
            res = fn_804032E8('FGAN', lbl_804A1A64[field_0x26c]);
            scaleY = -21.0f;
            break;
        case 3:
            res = fn_804032E8('FGAN', lbl_804A1A84[field_0x26c]);
            break;
        }
        params.field_0x1c = 0;
        params.field_0x1d = -2;
        params.field_0x10.set(0, 0, 0, (u8)(0.39f * field_0x290));
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 0.8f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 0.8f;
        params.mMtx = mtx;
        fn_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
        mPos.set(pos);
        params.field_0x1d = field_0x2ee;
        params.field_0x30 = 14;
        mtx[0][0] = field_0x2c8;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x2cc;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x2cc;
        params.mMtx = mtx;
        params.field_0x10.set(0xFF, 0xFF, 0xFF, (u8)field_0x290);
        fn_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
    }
}

// 0x80187144 (vtable slot 0x24): plays sound 0x40047 and re-issues the actor's
// vfunc_0x28 with the s8 at 0x2EE and (8.0, -16.0).
void PhantomGanon2_804A1A98::vfunc_0x24() {
    startSound_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ee, 8.0f, -16.0f);
}

// FIXME(shotgun): update (fn_80187530, 0x28BC bytes) and ActorBase::setLife's out-of-line copy
// (0x8018A588, an inline in ActorBase.h) are not written.
