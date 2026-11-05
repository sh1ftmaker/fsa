// The SLR1 actor (sword soldier, docs/names/actors.tsv; vtable 0x8049FCA0,
// constructor 0x80203CA4 in the spawner). Only the small helpers (and the
// actor-manager wrappers) are written so far.

#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "game/act/act_chief_soldier.h"
#include "game/act/act_sword_soldier.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

s32 fn_80154248(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg) {
    return fn_801F856C()->fn_801F8544(id, layer, pos, arg);
}

s32 fn_801542AC(u32 id, s32 layer, ActorBase_804A5770** out, s32 excludeSerial) {
    return fn_801F856C()->fn_801F7818(id, layer, out, excludeSerial);
}

s32 fn_80154310(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg, s32 a, s32 b) {
    return fn_801F856C()->fn_801F9484(id, layer, pos, arg, a, b);
}

// fabs narrowed to f32, out of line.
f32 fn_8015436C(f32 x) {
    return fabs(x);
}

// FIXME: the instructions match; its four constants (0.0f, .5, 3.0, 0.0)
// only pair with lbl_8053D258 / lbl_8053D2F8-lbl_8053D308 once the unit's
// .sdata2 is laid out in the original order. std::sqrtf out of line; 13
// calls from 7 units.
f32 fn_80154378(f32 x) {
    return std::sqrtf(x);
}

ActorBase_804A5770* fn_8015445C(s32 serial) {
    return fn_801F856C()->findActorBySerial_801F666C(serial);
}

// fn_80154490 to fn_801544C0: the sword soldier (SLR1) calls them on the
// SLR4 (Chief Soldier) actor that fn_801542AC finds, so they are that class's
// members; they sit here because this is the first unit that needs them.
void ChiefSoldier_804A0B90::fn_80154490() {
    field_0x2a5 = 1;
}

void ChiefSoldier_804A0B90::fn_8015449C() {
    field_0x290 = field_0x290 + 1;
}

s32 ChiefSoldier_804A0B90::fn_801544AC() const {
    return field_0x28c;
}

void ChiefSoldier_804A0B90::fn_801544B4() {
    field_0x290 = -1;
}

s32 ChiefSoldier_804A0B90::fn_801544C0() {
    return field_0x288;
}

u32 ActorBase_804A5770::fn_801544C8() const {
    return field_0x90;
}

void ActorBase_804A5770::fn_801544D0(s32 value) {
    field_0xd8 = value;
    field_0xdc = value;
}

// An actor's position (its mPos). Out of line in the original, called from
// 4 other units on actors they got from findActorBySerial_801F666C / fn_8015445C.
JGeometry::TVec3f* ActorBase_804A5770::fn_801544DC() {
    return &mPos;
}

f32 fn_801544E4(s16 angle) {
    return JMACosShort(angle);
}

f32 fn_801544FC(s16 angle) {
    return JMASinShort(angle);
}

// The EnemyParam "atcp" damage (ActorBase's 0x114), passed to fn_801F2B7C.
s32 ActorBase_804A5770::fn_80154510() const {
    return field_0x114;
}

void fn_80154518(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// FIXME: 99.58. The instructions match; only the 0.5f relocation differs
// (lbl_8053D2B0 in the original; the unit's .sdata2 isn't laid out yet).
void fn_80154528(const JGeometry::TBox2f* box, JGeometry::TVec2f* out) {
    out->x = box->f.x + (box->i.x - box->f.x) * 0.5f;
    out->y = box->f.y + (box->i.y - box->f.y) * 0.5f;
}

void fn_80154558(JGeometry::TVec2f* a, const JGeometry::TVec2f* b) {
    a->add(*b);
}

void fn_8015457C(JGeometry::TVec2f* dst, const JGeometry::TVec2f* src) {
    dst->set(*src);
}

void fn_80154590(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1) {
    box->set(x0, y0, x1, y1);
}

void fn_801545A4(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

bool ActorBase_804A5770::fn_801545C0(JGeometry::TVec3f* out, bool flag) {
    return fn_801F2618(out, flag);
}

s32 ActorBase_804A5770::fn_801545E0() const {
    return field_0x9c;
}

s32 ActorBase_804A5770::fn_801545E8() const {
    return mSerial;
}

void fn_801545F0(JGeometry::TVec3f* a, const JGeometry::TVec3f* b) {
    a->add(*b);
}

void ActorBase_804A5770::fn_80154614(s32 value) {
    field_0xb8 = value;
}

s32 ActorBase_804A5770::fn_8015461C() const {
    return field_0x4;
}

u8 fn_80154624(s32 layer) {
    return viewLayerFlag(layer);
}

// FIXME: 99.67. The instructions match; only the 10430.378f relocation differs
// (lbl_8053D310 in the original; the unit's .sdata2 isn't laid out yet).
s32 fn_8015465C(f32 y, f32 x) {
    return JMath::atanTable_.atan2_(y, x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

u8 fn_80154698() {
    return getGameView_802D6A9C()->fn_802DCD0C();
}

void fn_801546BC(JGeometry::TVec3f* v) {
    v->zero();
}

void fn_801546D0(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    *dst = *src;
}

JGeometry::TVec2f* fn_801546E8(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
    return v;
}

void fn_801546F4(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src) {
    dst->set(*src);
}

// Keeps pos in the camera's view (3 units call it; they ignore r3).
void fn_80154718(const JGeometry::TVec3f* pos) {
    lbl_8053AB10->field_0x24->addFocusPoint_8030C210(pos);
}

// A random float in [0, 1) from the game's RNG. Called from 10 units.
// The instructions match; its 1.0f only pairs with lbl_8053D25C once the
// unit's earlier functions put 0.0f (lbl_8053D258) first in .sdata2.
f32 fn_80154744() {
    return lbl_8053AAF8->mRandom.get_ufloat_1();
}
