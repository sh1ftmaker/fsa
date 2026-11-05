#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/act/act_phantom_ganon_2.h"

// 0x80187144 (vtable slot 0x24): plays sound 0x40047 and re-issues the actor's
// vfunc_0x28 with the s8 at 0x2EE and (8.0, -16.0).
void cls_804A1A98::vfunc_0x24() {
    fn_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ee, 8.0f, -16.0f);
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
