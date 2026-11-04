#include "game/GameAudio.h"
#include "game/act/act_opening_goal.h"

#include "game/GameGlobals.h"
#include "JSystem/JParticle/JPAEmitter.h"

// A key table of mAnim: (cel, frames) s16 pairs, ended by a cel of -1.
static const s16 lbl_80464AE8[8] = {0, 2, 1, 2, 2, 2, -1, -1};

// The table list passed to mAnim: one table, then a null end.
static const s16* lbl_80539F18[2] = {lbl_80464AE8, NULL};

cls_8049DB30::cls_8049DB30()
    : field_0x230(1), field_0x234(0), field_0x238(0), field_0x23c(0), field_0x240(0),
      field_0x244(0x9F), field_0x245(0xAF), field_0x246(0xFF), field_0x247(0), field_0x248(0xFF),
      mEmitter24c(NULL), mEmitter250(NULL), mEmitter254(NULL), mEmitter258(NULL) {}

cls_8049DB30::~cls_8049DB30() {
    fn_801EE434(mEmitter24c);
    mEmitter24c = NULL;
    fn_801EE434(mEmitter250);
    mEmitter250 = NULL;
    fn_801EE434(mEmitter254);
    mEmitter254 = NULL;
    fn_801EE434(mEmitter258);
    mEmitter258 = NULL;
}

void cls_8049DB30::init() {
    cls_8049D140::init();
    field_0x238 = field_0x90 & 0xF;
    field_0x23c = (field_0x90 >> 4) & 0xF;
    mAnim.fn_801D2608(lbl_80539F18, 1);
    mAnim.fn_801D22DC(0, 2, false);
    mAnim.fn_801D1F14(0.0f);
    fn_802D6A9C()->fn_802D7440(false);
}

void cls_8049DB30::vfunc_0x54() {
    switch (field_0x234) {
    case 3:
        if (fn_8011921C()->field_0x8 < 0x78) {
            break;
        }
        fn_801184F0(fn_8011921C(), false);
        fn_802D6A9C()->fn_802D9FF4(field_0x238, field_0x23c);
        fn_802D6A9C()->fn_802DA8DC();
        field_0x234 = 4;
        break;
    case 0:
        if (fn_8011921C()->field_0x8 < 0x47E) {
            break;
        }
        fn_8013CC50(field_0x4, &mPos, 0x4D7, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &mPos, 0x4D8, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &mPos, 0x4D9, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &mPos, 0x4DA, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &mPos, 0x4DB, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &mPos, 0x4DC, 0, 0, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &mPos, 0x4DD, 0, 0, NULL, NULL, true, 1.0f);
        if (mEmitter24c == NULL) {
            mEmitter24c = fn_8013CC50(field_0x4, &mPos, 0x4DE, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitter250 == NULL) {
            mEmitter250 = fn_8013CC50(field_0x4, &mPos, 0x4DF, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x240 = 0xB4;
        field_0x234 = 1;
        break;
    case 1:
        if (fn_8011921C()->field_0x8 < 0x794) {
            break;
        }
        field_0x240 = 0x78;
        field_0x234 = 2;
        break;
    case 2:
        break;
    case 4:
        break;
    }
}

// FIXME: 93.5 %. Ours keeps the constant 1 stored at 0x7A8 in r30 (saved)
// instead of loading it into r0 at the store, lacks the bge/b pair after the
// switch's cmpwi 2, swaps f3/f4 in the box sums and loads the 1 of the bit mask
// later (no clrlwi); the float literals are not split into this unit yet.
void cls_8049DB30::update() {
    cls_8049D140::update();
    fn_801902C0()->field_0x7a8 = 1;
    switch (field_0x234) {
    case 2:
        if (vfunc_0x34()) {
            if (mEmitter254 == NULL) {
                mEmitter254 = fn_8013CC50(field_0x4, &mPos, 0x111, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (mEmitter258 == NULL) {
                mEmitter258 = fn_8013CC50(field_0x4, &mPos, 0x112, 0, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            if (mEmitter254 != NULL) {
                mEmitter254->becomeInvalidEmitter();
                mEmitter254 = NULL;
            }
            if (mEmitter258 != NULL) {
                mEmitter258->becomeInvalidEmitter();
                mEmitter258 = NULL;
            }
        }
        if (fn_8011921C()->field_0x0 == 0) {
            JGeometry::TBox2f box(mBox);
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            bool flag;
            s32 player = fn_8023AF14(&box, field_0x4, false, 0.0f);
            if (player < 0) {
                flag = false;
            } else {
                flag = true;
                if (!(lbl_8053AAF8->field_0x82 & (1 << player))) {
                    flag = false;
                }
            }
            if (flag) {
                fn_8011888C(fn_8011921C(), this);
                fn_801186B0(fn_8011921C(), 0xE);
                fn_80119DD0()->fn_80119998(0xB4, 0, 2, 0, 1, 0x3C, 0);
                field_0x234 = 3;
            }
        }
        break;
    }
    cls_804A03A8* field30 = lbl_8053AB10->field_0x30;
    if (field30->field_0x284 == 0) {
        field30->field_0x284 = 1;
        field30->field_0x28c = 0.05f;
    }
    mAnim.fn_801D1C18();
    fn_8011A4BC();
}

// Moves a colour byte 1/frames of the way towards target; it is set outright
// once frames is below FLT_EPSILON.
static inline void fadeColour(u8& colour, s32 target, f32 frames) {
    if (frames < FLT_EPSILON) {
        colour = target;
    } else {
        colour = (u8)((f32)(target - colour) * (1.0f / frames) + (f32)colour);
    }
}

void cls_8049DB30::fn_8011A4BC() {
    switch (field_0x234) {
    case 1:
        field_0x240--;
        if (field_0x240 <= 0) {
            field_0x240 = 0;
        }
        fadeColour(field_0x244, 0xFF, field_0x240);
        fadeColour(field_0x245, 0xFF, field_0x240);
        fadeColour(field_0x246, 0xFF, field_0x240);
        fadeColour(field_0x247, 0, field_0x240);
        fadeColour(field_0x248, 0xFF, field_0x240);
        break;
    case 2:
        field_0x240--;
        if (field_0x240 <= 0) {
            field_0x240 = 0;
        }
        fadeColour(field_0x244, 0x9F, field_0x240);
        fadeColour(field_0x245, 0xAF, field_0x240);
        fadeColour(field_0x246, 0xFF, field_0x240);
        fadeColour(field_0x247, 0, field_0x240);
        fadeColour(field_0x248, 0xFF, field_0x240);
        break;
    }
    // The shadow sprite's colour (lbl_8053AB10->0x2C) follows this actor's.
    cls_804A0160* shadow = lbl_8053AB10->field_0x2c;
    shadow->field_0x4.r = field_0x244;
    shadow->field_0x4.g = field_0x245;
    shadow->field_0x4.b = field_0x246;
    shadow->field_0x4.a = field_0x247;
    lbl_8053AB10->field_0x2c->field_0xc = field_0x248;
}

void cls_8049DB30::draw() {
    cls_8049D140::draw();
}

s32 cls_8049DB30::vfunc_0x58() {
    return field_0x234;
}

s32 cls_8049DB30::vfunc_0x5C() {
    return field_0x234 != 3;
}
