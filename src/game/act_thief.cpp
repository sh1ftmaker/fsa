#include <stdlib.h>
#include "JSystem/JMath/JMath.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/EnemyBase.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_force_gem.h"
#include "game/act/act_thief.h"
#include "game/tu_801D2918.h"

struct GemDrop {
    s32 type;
    s32 amount;
};
extern const GemDrop lbl_8046A288[];

static JGeometry::TVec2f lbl_8053AC60; // the sort origin, set by fn_802063FC
static int fn_802068A0(const void* a, const void* b);
extern const s16* lbl_804A59B0[];

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

// qsort comparator (0x802068A0): orders actors by squared distance from lbl_8053AC60.
static int fn_802068A0(const void* a, const void* b) {
    const ActorBase_804A5770* actorA = *(ActorBase_804A5770* const*)a;
    const ActorBase_804A5770* actorB = *(ActorBase_804A5770* const*)b;
    f32 distA = square(actorA->mPos.x - lbl_8053AC60.x) + square(actorA->mPos.y - lbl_8053AC60.y);
    f32 distB = square(actorB->mPos.x - lbl_8053AC60.x) + square(actorB->mPos.y - lbl_8053AC60.y);
    int result = 0;
    if (distA < distB) {
        return -1;
    }
    if (distA > distB) {
        result = 1;
    }
    return result;
}

// FIXME: 99.86% (reloc-name). Differs only in literal symbol relocations.
Thief_804A5A30::Thief_804A5A30() {
    field_0x340 = -1;
    field_0x344 = 0;
    field_0x348 = 0;
    field_0x349 = 0;
    field_0x34a = 0;
    field_0x34b = 0;
    field_0x34c = 0;
    mType = 0;
    field_0x354 = 0;
    mEmitter358 = NULL;
    field_0x35c.frames = 0;
    field_0x35c.vel.x = 0.0f;
    field_0x35c.vel.y = 0.0f;
    field_0x368.fn_80217484();
    field_0x374.fn_8021BA30(0);
}

Thief_804A5A30::~Thief_804A5A30() {
    releaseEmitter_801EE434(mEmitter358);
    fn_80204638()->fn_802049E0(this);
}

// FIXME: 99.60% (regalloc-only + reloc-name). Register allocation and literal relocations.
void Thief_804A5A30::init() {
    EnemyBase_804A4808::init();

    u32 valA = field_0x90 & 0xF;
    field_0x34a = valA > 9 ? 9 : valA;

    u32 valB = (field_0x90 >> 8) & 0xFF;
    field_0x34b = valB <= 3 ? 3 : valB;

    u32 valC = (field_0x90 >> 16) & 0xFF;
    field_0x34c = valC <= 3 ? 3 : valC;

    if (field_0x34c <= field_0x34b) {
        field_0x34c = field_0x34b + 1;
    }

    s32 valD = (field_0x90 >> 24) & 3;
    mType = valD >= 3 ? 3 : valD;

    field_0x354 = field_0x90 >> 27;
    fn_802072E4();

    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0xb0 = 0x51303;
    mFlags |= ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x32c.x = mPos.x;
    field_0x32c.y = mPos.y;
    field_0x32c.z = mPos.z;
    field_0x33c = fn_801F6874(field_0x98);
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A59B0 + mType * 8, 8);
    mAnim.startAnim_801D22DC(field_0x98, 2, false);
    mAnim.fn_801D1D60();
    fn_80204638()->fn_80204A20(this);
    if (fn_802064E0()) {
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        field_0xb0 = 0;
        mAlive = false;
        fn_80204638()->fn_80204834();
    }
}

bool Thief_804A5A30::fn_802064E0() {
    bool result = false;
    s32 bit = field_0x354;
    if (bit != 0) {
        if (getGameView_802D6A9C()->fn_802D7F40(bit)) {
            result = true;
        }
    }
    return result;
}

s32 Thief_804A5A30::fn_802063FC() {
    ActorBase_804A5770* actors[0x400];
    JGeometry::TVec2f from;
    from.x = mPos.x;
    from.y = mPos.y;
    s32 count = getActorManager_801F856C()->fn_801F7274(field_0x4, actors, -1);
    if (count >= 2) {
        lbl_8053AC60 = from;
        qsort(actors, count, 4, fn_802068A0);
    }
    s32 result = -1;
    ActorBase_804A5770** p = actors;
    s32 end = count > 0x14 ? 0x14 : count;
    for (s32 i = 0; i < end; i++, p++) {
        if (!fn_801CF888(&from, &(*p)->mPos, field_0x4)) {
            result = (*p)->mSerial;
            break;
        }
    }
    return result;
}

// FIXME: 98.73% (regalloc-only + reloc-name). Register allocation swap between r29 and r30.
void Thief_804A5A30::fn_80206168() {
    if (field_0x344 == 0) {
        return;
    }
    bool inDark = fn_801CD2F8(field_0x4);
    u32 max = fn_801CD664(31) + 20;
    u32 count = field_0x344;
    if (count >= max) {
        count = max;
    }
    u32 toDrop = count;
    if (inDark && count >= 20) {
        toDrop = count - count % 20;
    }
    field_0x344 -= toDrop;
    while (toDrop > 0) {
        u32 gemType;
        if (toDrop >= 50) {
            gemType = 3;
            toDrop -= 50;
        } else if (toDrop >= 20 && (inDark || gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f)) {
            gemType = 2;
            toDrop -= 20;
        } else if (toDrop >= 5 && (inDark || gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f)) {
            gemType = 1;
            toDrop -= 5;
        } else {
            gemType = 0;
            toDrop -= 1;
        }
        s32 serial = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, gemType);
        if (serial >= 0) {
            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (gem != NULL) {
                gem->field_0x3c = JGeometry::TVec3f(
                    1.5f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                    1.5f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                    -0.5f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()
                );
            }
        }
    }
}

// True in state 1 when no fn_801CD2F8 hit is set for the layer and mPos lies
// outside the view box of that layer.
bool Thief_804A5A30::fn_8020609C() const {
    s32 layer;
    bool result = false;
    if (field_0x94 == 1) {
        if (!fn_801CD2F8(field_0x4)) {
            layer = field_0x4;
            GameView_804A9950* view = getGameView_802D6A9C();
            const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
            if (!contains(box, mPos)) {
                result = true;
            }
        }
    }
    return result;
}

// FIXME: 80.27% (scheduling + reloc-name). Instruction scheduling and literal relocations.
void Thief_804A5A30::fn_80205E0C(s32 newState) {
    if (newState == 7) {
        switch (mState) {
        case 3:
        case 4:
            return;
        }
    }
    switch (mState) {
    case 1:
    case 2:
    case 3:
        mFlags &= ~ENEMY_FLAG_NO_MAP_HIT;
        break;
    }
    switch (newState) {
    case 0:
        field_0x340 = -1;
        field_0x338 = -1;
        mTargetSpeed = 0.0f;
        mPathCount = 0;
        field_0x368.fn_80217484();
        break;
    case 1:
        field_0x349 = 30;
        mTargetSpeed = lbl_8046A238[mType].field_0x4;
        mFlags |= ENEMY_FLAG_NO_MAP_HIT;
        break;
    case 2:
        field_0x340 = -1;
        field_0x348 = 30;
        mTargetSpeed = lbl_8046A238[mType].field_0x0;
        mPathCount = 0;
        mFlags |= ENEMY_FLAG_NO_MAP_HIT;
        break;
    case 3:
        field_0x340 = -1;
        field_0x338 = -1;
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        mPathCount = 0;
        mFlags |= ENEMY_FLAG_NO_MAP_HIT;
        break;
    case 4:
        field_0x110 = -1;
        break;
    case 5:
        field_0x340 = -1;
        field_0x338 = -1;
        mPathCount = 0;
        break;
    case 6:
        field_0x340 = -1;
        field_0x348 = 30;
        mTargetSpeed = lbl_8046A238[mType].field_0x0;
        mPathCount = 0;
        field_0x33c = fn_801D0280(field_0x338) - 0x8000;
        break;
    case 7:
        field_0x340 = -1;
        field_0x338 = -1;
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mPathCount = 0;
        break;
    case 8:
        field_0x340 = -1;
        field_0x338 = -1;
        mTargetSpeed = lbl_8046A238[mType].field_0x0;
        mPathCount = 0;
        field_0x374.fn_8021BA30(field_0x33c);
        break;
    case 9:
        if (mState != 8) {
            field_0x340 = -1;
            field_0x338 = -1;
            mTargetSpeed = lbl_8046A238[mType].field_0x0;
            mPathCount = 0;
            field_0x374.fn_8021BA30(field_0x33c);
        }
        break;
    }
    mStateTimer = 0;
    mState = newState;
}

// FIXME: 94.19%. Ours has one more instruction (539 vs 538), the rest is register allocation (this in r29 vs
// r31, the layer argument of fn_801CF888 in states 0 and 6 is loaded into a saved register before
// getPlayerPos_8023E724 in the original) and literal relocations.
void Thief_804A5A30::vfunc_0x90(u8 mapHit) {
    // Declared at function scope (not in the state 4 loop) for the original's register assignment.
    u32 gemType = 0;

    if (mapHit) {
        fn_801F11B4();
    } else {
        switch (mState) {
        case 1:
        case 2:
            if (vfunc_0x4C(field_0x4, &mPos)) {
                mPos.x = mPrevPos.x;
                mPos.y = mPrevPos.y;
                fn_80205E0C(0);
            }
            break;
        }
    }

    if (field_0x348 != 0) {
        field_0x340 = -1;
    } else {
        switch (mState) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 9:
            field_0x340 = -1;
            break;
        default: {
            s32 serial = field_0x340;
            bool findNew = true;
            if (serial >= 0) {
                ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (target != NULL && fn_801CC9B0(target) &&
                    !fn_801CF888((const JGeometry::TVec2f*)&mPos, &target->mPos, field_0x4)) {
                    findNew = false;
                }
            }
            if (findNew) {
                field_0x340 = fn_802063FC();
            }
            break;
        }
        }
    }

    if (field_0x349 != 0) {
        field_0x338 = -1;
    } else {
        switch (mState) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 9:
            field_0x338 = -1;
            break;
        case 2:
        case 6:
            break;
        default:
            field_0x338 = fn_801D05F8(&mPos, (f32)field_0x34b * 16.0f);
            break;
        }
    }

    switch (mState) {
    case 0:
        if (field_0x340 >= 0) {
            fn_80205E0C(1);
        } else if (field_0x338 >= 0) {
            if (fn_801CCF34(field_0x338) <= 0 && !fn_801CF888((const JGeometry::TVec2f*)&mPos, getPlayerPos_8023E724(field_0x338), field_0x4)) {
                fn_80205E0C(6);
            } else if (fn_801D183C(0.0f)) {
                fn_80205E0C(2);
            }
        }
        if (mState == 0) {
            if (field_0x94 == 1) {
                fn_80205E0C(8);
            } else {
                field_0x33c = field_0x368.fn_80217374(field_0x33c);
            }
        } else if (mState == 8) {
            if (mapHit) {
                fn_801D11FC();
            }
            field_0x33c = field_0x374.fn_8021B904(field_0x33c, mapHit);
        }
        break;
    case 1:
        if (field_0x340 < 0) {
            fn_80205E0C(0);
        } else {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x340);
            field_0x32c.x = target->mPos.x;
            field_0x32c.y = target->mPos.y;
            field_0x32c.z = target->mPos.z;
            f32 dx = field_0x32c.x - mPos.x;
            f32 dy = field_0x32c.y - mPos.y;
            if (square(dx) + square(dy) <= 100.0f) {
                startSound_801F0E34(0x4009F, 0);
                s32 amount = target->fn_8032D564(target->field_0x90 & 0xFF);
                if (amount > 3000) {
                    amount = 3000;
                } else if (amount < 1) {
                    amount = 1;
                }
                field_0x344 += amount;
                ForceGem_804AB6D8* gem = static_cast<ForceGem_804AB6D8*>(target);
                gem->field_0x94 = 1;
                gem->field_0x264 = mSerial;
                gem->field_0x282 = 1;
                gem->field_0x230 = 3;
                field_0x340 = fn_802063FC();
            }
        }
        break;
    case 2:
        if (field_0x340 >= 0) {
            fn_80205E0C(1);
        } else if (field_0x338 < 0 || fn_801CCF34(field_0x338) <= 0 || !fn_801D0B44(field_0x338) ||
                   fn_801D08B4(field_0x338, &mPos, (f32)field_0x34c * 16.0f) == 0) {
            fn_80205E0C(0);
        } else if (!fn_801D12CC()) {
            fn_80205E0C(0);
        } else {
            field_0x33c = fn_801D02E0(&field_0x32c);
        }
        break;
    case 3:
        if (field_0xc4 == 0) {
            fn_80205E0C(4);
        }
        break;
    case 4:
        if (mPos.z <= 0.0f) {
            while (field_0x344 != 0) {
                gemType = 0;
                for (s32 i = 11; i >= 0; i--) {
                    if (field_0x344 >= lbl_8046A288[i].amount) {
                        gemType = lbl_8046A288[i].type;
                        field_0x344 -= lbl_8046A288[i].amount;
                        break;
                    }
                }
                JGeometry::TVec3f spawnPos(mPos.x, mPos.y, mPos.z + 24.0f);
                s32 serial = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &spawnPos, gemType);
                if (serial >= 0) {
                    ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (gem != NULL) {
                        gem->field_0x3c = JGeometry::TVec3f(
                            1.2f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                            1.2f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                            6.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()
                        );
                    }
                }
            }
            fn_80205E0C(0);
        }
        break;
    case 5:
        if (fn_801D2918(&field_0x35c)) {
            fn_80205E0C(7);
        }
        break;
    case 6:
        if (field_0x340 >= 0) {
            fn_80205E0C(1);
        } else if (field_0x338 < 0 || fn_801CCF34(field_0x338) > 0 || !fn_801D0B44(field_0x338) ||
                   fn_801D08B4(field_0x338, &mPos, (f32)field_0x34c * 16.0f) != 0 ||
                   fn_801CF888((const JGeometry::TVec2f*)&mPos, getPlayerPos_8023E724(field_0x338), field_0x4)) {
            fn_80205E0C(0);
        } else if (mapHit) {
            fn_801D11FC();
        } else {
            fn_801CD930(&field_0x33c, (s16)(fn_801D0280(field_0x338) - 0x8000), 0x200);
        }
        break;
    case 7:
        if (mStateTimer >= 60) {
            fn_80205E0C(0);
        }
        break;
    case 8:
        if (mapHit) {
            fn_801D11FC();
        }
        field_0x33c = field_0x374.fn_8021B904(field_0x33c, mapHit);
        break;
    case 9:
        break;
    }

    switch (mState) {
    case 0:
    case 6:
    case 8:
    case 9:
        field_0x98 = fn_801F681C((u16)field_0x33c);
        break;
    case 3:
    case 4:
    case 5:
    case 7:
        break;
    default:
        field_0x33c = fn_801D02E0(&field_0x32c);
        field_0x98 = fn_801F681C((u16)field_0x33c);
        break;
    }
}

void Thief_804A5A30::vfunc_0x94() {
    s32 state = mState;
    switch (state) {
    case 5:
    case 7:
        return;
    }
    if (mType == 2) {
        mAnim.fn_801D22A0(field_0x98, 2);
    } else if (state == 0) {
        switch (mAnim.mAnim) {
        case 0:
        case 1:
        case 2:
        case 3:
            mAnim.fn_801D2264(field_0x98, 2);
            break;
        default:
            mAnim.startAnim_801D22DC(field_0x98, 2, false);
            break;
        }
    } else {
        switch (mAnim.mAnim) {
        case 0:
        case 1:
        case 2:
        case 3:
            mAnim.startAnim_801D22DC(field_0x98 + 4, 2, false);
            break;
        default:
            if (mType == 0) {
                mAnim.fn_801D22A0(field_0x98 + 4, 2);
            } else {
                mAnim.fn_801D2264(field_0x98 + 4, 2);
            }
            break;
        }
    }
    EnemyBase_804A4808::vfunc_0x94();
}

// FIXME: 99.39% (regalloc-only). Register allocation order in loop.
void Thief_804A5A30::vfunc_0x9C(u8 mapHit) {
    if (field_0xc4 != 0 && field_0xc8 >= 0 && field_0xc8 < 4) {
        if (fn_80204638()->field_0x0 != 0 || fn_802064E0()) {
            fn_80230C6C(field_0xc8);
        }
    }
    if (fn_802064E0()) {
        switch (mState) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 9:
            break;
        default:
            fn_80205E0C(9);
            break;
        }
    }
    if (field_0x348 != 0) {
        field_0x348--;
    }
    if (field_0x349 != 0) {
        field_0x349--;
    }
    if (fn_8020609C()) {
        return;
    }
    switch (mState) {
    case 3:
    case 4:
    case 9:
        break;
    default:
        if (fn_80204638()->field_0x0 == 0 && !fn_802064E0()) {
            JGeometry::TBox2f box;
            fn_801F31C4(&box);
            for (s32 i = 0; i < 4; i++) {
                if (fn_801CCFE4(i)) {
                    // Local for codegen: the original reads our layer before calling getPlayerLayer_8023DE58.
                    s32 layer = field_0x4;
                    if (getPlayerLayer_8023DE58(i) == layer &&
                        fn_80239C00(i, &box, field_0x4) &&
                        fn_8022F9E4(i, mSerial, true, false)) {
                        fn_80205E0C(3);
                        break;
                    }
                }
            }
        }
        break;
    }
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    fn_8020FADC();
}

// FIXME: 99.91% (reloc-name). Differs only in literal symbol relocations.
void Thief_804A5A30::vfunc_0x88() {
    vfunc_0x84();
    if (fn_8020609C()) {
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
    } else {
        JGeometry::TVec2f dir;
        switch (mState) {
        case 3:
        case 7:
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            break;
        case 4:
            break;
        case 5:
            dir = field_0x35c.vel;
            field_0x3c.x = dir.x;
            field_0x3c.y = dir.y;
            break;
        case 6:
        case 8:
        case 9:
            dir.x = JMath::sincosTable_.sinShort(field_0x33c);
            dir.y = JMath::sincosTable_.cosShort(field_0x33c);
            field_0x3c.x = dir.x * mSpeed;
            field_0x3c.y = dir.y * mSpeed;
            break;
        default:
            fn_801D01A0(&field_0x32c, &dir, mSpeed);
            field_0x3c.x = dir.x;
            field_0x3c.y = dir.y;
            break;
        }
    }
}

void Thief_804A5A30::vfunc_0x8C() {
    if (!fn_8020609C()) {
        switch (mState) {
        case 1:
        case 2:
        case 6:
        case 8:
            if (fn_80204638()->fn_802046FC(&field_0x3c, &mPos)) {
                if (mFlags & ENEMY_FLAG_NO_MAP_HIT) {
                    fn_801F2618(&mMapHitDir, lbl_8053EFC0);
                }
            }
            break;
        }
    }
}

// FIXME: 99.93% (reloc-name). Differs only in literal symbol relocations.
void Thief_804A5A30::vfunc_0x98() {
    if (mState == 3) {
        if (field_0xc4 != 0) {
            field_0x98 = fn_80230CFC(field_0xc8);
            field_0x33c = fn_801F6874(field_0x98);
            JGeometry::TVec3f* pos = fn_8023E7B0(field_0xc8);
            mPos.x = pos->x;
            mPos.y = pos->y;
            mPos.z = pos->z;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            if (field_0x4 != getPlayerLayer_8023DE58(field_0xc8)) {
                field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
                mPathCount = 0;
                s32 player = field_0xc8;
                if (fn_8022F514(player)) {
                    player = fn_8022F3DC(player);
                }
                JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(player);
                field_0x32c.x = playerPos->x;
                field_0x32c.y = playerPos->y;
                field_0x32c.z = playerPos->z;
            }
        }
    } else if ((mFlags & 7) == 0) {
        fn_801CE32C();
    }
}

void Thief_804A5A30::vfunc_0xA0() {
    if (mState != 9 && mState != 8) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

bool ActorBase_804A5770::fn_80204D44() {
    bool result = false;
    if (field_0x94 == 1 || field_0xc4 != 0) {
        result = true;
    }
    return result;
}
