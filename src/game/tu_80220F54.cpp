#include "game/GameAudio.h"
#include "game/cls_804A6A00.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/ViewLayer.h"
#include "game/Camera.h"
#include "game/Effect.h"

static inline void setBox(JGeometry::TBox2f& box, f32 lo, f32 hi) {
    box.i.x = lo;
    box.i.y = lo;
    box.f.x = hi;
    box.f.y = hi;
}

static const s16 lbl_8053FE48[] = {60, 10, -1, -1};
static const s16 lbl_8053FE50[] = {61, 10, -1, -1};
static const s16 lbl_8053FE58[] = {62, 10, -1, -1};
static const s16 lbl_8053FE60[] = {63, 10, -1, -1};
static const s16 lbl_8046A7F0[] = {0, 3, 1, 3, 2, 3, 3, 3, 4, 3, 5, 3, 6, 3, 7, 3, -1, -1};
static const s16 lbl_8046A814[] = {8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14, 3, 15, 3, -1, -1};
static const s16 lbl_8046A838[] = {16, 3, 17, 3, 18, 3, 19, 3, 20, 3, 21, 3, 22, 3, 23, 3, -1, -1};
static const s16 lbl_8046A85C[] = {24, 3, 25, 3, 26, 3, 27, 3, 28, 3, 29, 3, 30, 3, 31, 3, -1, -1};
static const s16 lbl_8046A880[] = {241, 3, 242, 6, 243, 6, 244, 3, 245, 3, 246, 3, 247, 6, 248, 80, -1, -1};
static const s16 lbl_8046A8A4[] = {249, 3, 250, 6, 251, 6, 252, 3, 253, 3, 254, 3, 255, 6, 256, 80, -1, -1};
static const s16 lbl_8046A8C8[] = {257, 3, 258, 6, 259, 6, 260, 3, 261, 3, 262, 3, 263, 6, 264, 80, -1, -1};
static const s16 lbl_8046A8EC[] = {265, 3, 266, 6, 267, 6, 268, 3, 269, 3, 270, 3, 271, 6, 272, 80, -1, -1};
static const s16 lbl_8046A910[] = {167, 4, 168, 4, 169, 4, 170, 30, -1, -1};
static const s16 lbl_8046A924[] = {171, 4, 172, 4, 173, 4, 174, 30, -1, -1};
static const s16 lbl_8046A938[] = {175, 4, 176, 4, 177, 4, 178, 30, -1, -1};
static const s16 lbl_8046A94C[] = {179, 4, 180, 4, 181, 4, 182, 30, -1, -1};
static const s16 lbl_8046A960[] = {395, 2, 396, 1, 397, 1, 398, 6, 399, 1, 400, 2, -1, -1};
static const s16 lbl_8046A97C[] = {401, 2, 402, 1, 403, 1, 404, 6, 405, 1, 406, 2, -1, -1};
static const s16 lbl_8046A998[] = {407, 2, 408, 1, 409, 1, 410, 6, 411, 1, 412, 2, -1, -1};
static const s16 lbl_8046A9B4[] = {413, 2, 414, 1, 415, 1, 416, 6, 417, 1, 418, 2, -1, -1};
static const s16 lbl_8046A9D0[] = {197, 2, 198, 2, 199, 2, 200, 2, -1, -1};
static const s16 lbl_8046A9E4[] = {201, 2, 202, 2, 203, 2, 204, 2, -1, -1};
static const s16 lbl_8046A9F8[] = {205, 2, 206, 2, 207, 2, 208, 2, 205, 2, 209, 2, 210, 2, 211, 2, -1, -1};
static const s16 lbl_8046AA1C[] = {212, 2, 213, 2, 214, 2, 215, 2, 212, 2, 216, 2, 217, 2, 218, 2, -1, -1};
static const s16 lbl_8046AA40[] = {307, 1, 306, 1, 305, 30, -1, -1};
static const s16 lbl_8053FE68[] = {379, 30, -1, -1};
static const s16 lbl_8046AA50[] = {380, 1, 381, 1, 384, 1, 385, 1, 384, 1, 385, 1, 384, 1, 385, 1, 384, 2, 385, 2, 384, 2, 385, 5, 386, 10, -1, -1};
static const s16 lbl_8046AA88[] = {127, 25, 602, 1, 603, 4, 604, 1, 605, 2, -1, -1};

static const s16* lbl_804A6990[0x1C] = {
    lbl_8053FE48, lbl_8053FE50, lbl_8053FE58, lbl_8053FE60,
    lbl_8046A7F0, lbl_8046A814, lbl_8046A838, lbl_8046A85C,
    lbl_8046A880, lbl_8046A8A4, lbl_8046A8C8, lbl_8046A8EC,
    lbl_8046A910, lbl_8046A924, lbl_8046A938, lbl_8046A94C,
    lbl_8046A960, lbl_8046A97C, lbl_8046A998, lbl_8046A9B4,
    lbl_8046A9D0, lbl_8046A9E4, lbl_8046A9F8, lbl_8046AA1C,
    lbl_8046AA40, lbl_8053FE68, lbl_8046AA50, lbl_8046AA88,
};

const f32 lbl_8053FE70 = 2.5f;

ShadowLinkBase_804A6A00::ShadowLinkBase_804A6A00() {
    field_0x2a8 = 0;
    field_0x2ac = 0;
    field_0x2ad = 0;
    field_0x2b0.x = 0.0f;
    field_0x2b0.y = 0.0f;
}

void ShadowLinkBase_804A6A00::init() {
    EnemyBase_804A4808::init();
    field_0x2a8 = (field_0x90 >> 4) & 3;
    field_0x2ac = (field_0x90 >> 8) & 3;
    field_0x2ad = (field_0x90 >> 12) & 0xFF;
    setBox(field_0x80, -8.0f, 8.0f);
    setBox(mBox, 1.0f, -1.0f);
    mBox2.i = mBox.i;
    mBox2.f = mBox.f;
    setBox(mBox, -8.0f, 8.0f);
    field_0xb0 = 0;
    mFlags |= ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    field_0x2b0.x = mPos.x;
    field_0x2b0.y = mPos.y;
    mAnim.fn_801D2608(lbl_804A6990, 0x1C);
}

// FIXME: 96.07 %. missing/extra: ours has 2 fewer instrs (54 vs 56); common-subexpression reuses the mFlags loaded in r5 in cases 0 and 3 instead of reloading it
void ShadowLinkBase_804A6A00::fn_802216E8(s32 limit) {
    if (mFlags & CLS_804A6A00_FLAG_0x2000) {
        return;
    }
    switch (field_0x2ac) {
    case 0:
        if (mStateTimer >= limit) {
            mFlags |= CLS_804A6A00_FLAG_0x2000;
        }
        break;
    case 1:
        if (fn_801D05F8(&mPos, 96.0f) >= 0) {
            mFlags |= CLS_804A6A00_FLAG_0x2000;
        }
        break;
    case 2:
        if (fn_801D05F8(&mPos, 192.0f) >= 0) {
            mFlags |= CLS_804A6A00_FLAG_0x2000;
        }
        break;
    case 3:
        if (mStateTimer >= field_0x2ad) {
            mFlags |= CLS_804A6A00_FLAG_0x2000;
        }
        break;
    }
}

// FIXME: 96.83 %. Argument/register order only: fn_801CBB98 is now declared once (shared
// EnemyBase.h signature: angle before distance), which moves the fn_801F6874 call and swaps r30/r31.
void ShadowLinkBase_804A6A00::fn_80221670() {
    // pos reference: steers the register allocation (without it 81 %)
    const JGeometry::TVec3f& pos = mPos;
    JGeometry::TBox2f* box = layerBox(field_0x4);
    fn_801CBB98(box, &pos, fn_801F6874(field_0x2a8), 48.0f, &field_0x2b0);
}

void ShadowLinkBase_804A6A00::fn_8022149C(s32 state) {
    switch (state) {
    case 0:
    case 1:
        fn_801F3604();
        setBox(mBox, 1.0f, -1.0f);
        mBox2.i = mBox.i;
        mBox2.f = mBox.f;
        setBox(field_0x80, 1.0f, -1.0f);
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        mAnim.startAnim_801D22DC(field_0x98, 0, false);
        if (state == 0) {
            startSound_801F0E34(0x40037, 0);
        } else if (state == 1) {
            startSound_801F0E34(0x4004A, 0);
        }
        break;
    case 2:
        fn_801F3604();
        setBox(mBox, 1.0f, -1.0f);
        mBox2.i = mBox.i;
        mBox2.f = mBox.f;
        setBox(field_0x80, 1.0f, -1.0f);
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        fn_80221670();
        field_0x98 = field_0x2a8;
        mAnim.fn_801D22A0(field_0x98, 0);
        break;
    }
    mStateTimer = 0;
    mState = state;
}

void ShadowLinkBase_804A6A00::fn_80221408() {
    if (mStateTimer >= 0xA0) {
        fn_801CB434(field_0x4, &mPos, false);
        vfunc_0x24();
    } else if (mStateTimer % 8 == 0) {
        field_0x98 = fn_801CD30C(field_0x98);
        mAnim.startAnim_801D22DC(field_0x98, 0, false);
    }
}

void ShadowLinkBase_804A6A00::fn_80221334() {
    if (mStateTimer >= 0x73) {
        fn_8022149C(2);
    } else {
        if (mStateTimer == 0x46) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x416, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x417, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mStateTimer % 4 == 0) {
            field_0x98 = fn_801CD30C(field_0x98);
            mAnim.startAnim_801D22DC(field_0x98, 0, false);
        }
    }
}

void ShadowLinkBase_804A6A00::fn_80221250() {
    JGeometry::TVec2f d;
    fn_801D01A0((const JGeometry::TVec3f*)&field_0x2b0, &d, mSpeed);
    field_0x3c.x = d.x;
    field_0x3c.y = d.y;
    vfunc_0x84();
}

void ShadowLinkBase_804A6A00::fn_802212AC() {
    if (mStateTimer == 0x2D) {
        mTargetSpeed = 1.0f;
        mAnim.fn_801D22A0(field_0x98 + 4, 2);
    } else if (mStateTimer > 0x2D) {
        if (__fabsf(field_0x2b0.x - mPos.x) < 0.1f && __fabsf(field_0x2b0.y - mPos.y) < 0.1f) {
            fn_801F3668();
        }
    }
}

void ShadowLinkBase_804A6A00::vfunc_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        fn_801CE2BC(mapHit);
    } else {
        vfunc_0x94();
    }
}

void ShadowLinkBase_804A6A00::vfunc_0x6C() {
    s32 angle = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) *
                (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
    field_0x98 = fn_801F681C(angle - 0x8000);
    mAnim.startAnim_801D22DC(field_0x98 + 0xC, 0, false);
    startSound_801F0E34(0x400C2, 0);
}

void ShadowLinkBase_804A6A00::vfunc_0x74() {
    fn_8022149C(0);
}

u8 ShadowLinkBase_804A6A00::vfunc_0x60() {
    return 0;
}

void ShadowLinkBase_804A6A00::vfunc_0xA0() {
    JGeometry::TVec3f pos(mPos.x, mPos.y, 0.0f);
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
}

void ShadowLinkBase_804A6A00::draw() {
    SpriteParams_801F5AE0 params;
    // params.field_0x30 before field_0x2e for constant load order
    params.field_0x30 = 0xE;
    params.field_0x2e = 0x40;
    if (viewLayerFlag(field_0x4)) {
        params.field_0x1f = false;
    }
    if (mState == 2) {
        fn_801EF574("player_sob.bin", mAnim.mCel, &mPos, &params);
    } else {
        drawSprite_801F06F0("player_sob.bin", mAnim.mCel, &params, 7, 1.0f, 1.0f);
        fn_80307830(mSerial, field_0x4, &mPos, mAnim.mCel, 0, true, 0, 4);
    }
}
