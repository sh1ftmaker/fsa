#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_phantom_ganon_2.h"

s32 lbl_8053AB78;

// 0x80187144 (vtable slot 0x24): plays sound 0x40047 and re-issues the actor's
// vfunc_0x28 with the s8 at 0x2EE and (8.0, -16.0).
void cls_804A1A98::vfunc_0x24() {
    fn_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ee, 8.0f, -16.0f);
}

// The target reads field_0x4 into a register before the fn_8023DE58 call and keeps it
// there across the call; a helper taking the actor does that, a plain
// `fn_8023DE58(p) == field_0x4` loads it after the call.
static inline bool inLayer(cls_804A1A98* self, s32 player) {
    s32 layer = self->field_0x4;
    return layer == fn_8023DE58(player);
}

// FIXME: 98.9. Regalloc only: the target keeps this in r30 and the two layer reads in r29,
// we use r29 for this and r30/r28 for the counter and the layer reads.
u16 cls_804A1A98::fn_80189DEC() {
    s32 player = fn_8022EA0C(field_0x94);
    if (player < 0) {
        player = 0;
    }
    if (!(fn_8023F264(player) && inLayer(this, player) && !fn_80230188(player) &&
          !fn_80236B50(player) && lbl_8053AB78 != 1))
    {
        s32 pick = (s32)(3.99f * lbl_8053AAF8->mRandom.get_ufloat_1());
        for (s32 i = 0; i < 4; i++) {
            if (pick != player && fn_8023F264(pick) && inLayer(this, pick) &&
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
cls_804A1A98::~cls_804A1A98() {
    for (s32 i = 0; i < 4; i++) {
        fn_801EE434(mEmitters2f4[i]);
        fn_801EE434(mEmitters304[i]);
    }
    fn_801EE434(mEmitter314);
    if (field_0x90 == 0) {
        fn_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    if (field_0x90 == 10) {
        s32 serial = field_0x274;
        if (serial >= 0) {
            cls_804A1A98* other = (cls_804A1A98*)fn_801F856C()->fn_801F666C(serial);
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
    return fn_8013C824()->mEmitterManager;
}

// Whether bit `player` of the game root's field_0x82 is set (an inline copy of
// GameRoot_8049EF20::fn_8023F98C that loads lbl_8053AAF8 itself).
bool fn_8018A614(s32 player) {
    u8 bit = 1 << player;
    return (lbl_8053AAF8->field_0x82 & bit) != 0;
}

void fn_8018A63C() {
    fn_801F856C()->fn_801F6884();
}

s32 fn_8018A660() {
    return fn_801F856C()->fn_801F68E0();
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8018A684(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8018A6A0(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

Unk_8045A080* fn_8018A6AC() {
    return lbl_8053AF80;
}
