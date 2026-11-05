#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/act/act_phantom_ganon_2.h"

// .sbss 0x8053AB78 (a mode word; the unwritten update and init also use it).
s32 lbl_8053AB78;

// 0x80187144 (vtable slot 0x24): plays sound 0x40047 and re-issues the actor's
// vfunc_0x28 with the s8 at 0x2EE and (8.0, -16.0).
void PhantomGanon2_804A1A98::vfunc_0x24() {
    startSound_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ee, 8.0f, -16.0f);
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

// fn_8018A588 (setLife) is ActorBase's inline setter.

u16 ActorBase_804A5770::fn_8018A590() const {
    return field_0x94;
}

void ActorBase_804A5770::fn_8018A598(bool alive) {
    mAlive = alive;
}

void ActorBase_804A5770::fn_8018A5A0(s32 value) {
    field_0x98 = value;
}

void ActorBase_804A5770::fn_8018A5A8(u16 value) {
    field_0x94 = value;
}

bool ActorBase_804A5770::fn_8018A5B0() const {
    return mAlive;
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_8018A5B8(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// The effect system's particle emitter manager, fetched through a getter.
JPAEmitterManager* fn_8018A5F0() {
    return getEffectSystem_8013C824()->mEmitterManager;
}

// Whether bit `player` of the game root's field_0x82 is set (game/PlayerBit.h,
// on gGameRoot_8053AAF8 itself).
bool fn_8018A614(s32 player) {
    return isPlayerBitSet(gGameRoot_8053AAF8, player);
}

void fn_8018A63C() {
    getActorManager_801F856C()->fn_801F6884();
}

s32 fn_8018A660() {
    return getActorManager_801F856C()->fn_801F68E0();
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8018A684(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8018A6A0(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

AudioMgr_8045A080* fn_8018A6AC() {
    return gAudioMgr_8053AF80;
}

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
