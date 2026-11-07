#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "game/act/act_zora.h"

static const s16 lbl_80469318[] = { 12, 6, 13, 6, 14, 6, 15, 6, -1, -1 };
static const s16 lbl_8046932C[] = { 10, 3, 9, 3, 8, 3, 0, 6, -1, -1 };
static const s16 lbl_80469340[] = { 0, 6, 1, 6, 2, 6, 3, 6, -1, -1 };
static const s16 lbl_80469354[] = { 4, 6, 5, 6, 6, 6, 7, 6, -1, -1 };
static const s16 lbl_80469368[] = { 0, 6, 8, 3, 9, 3, 10, 3, 11, 3, -1, -1 };

static const s16* lbl_804A3D68[5] = {
    lbl_80469318,
    lbl_8046932C,
    lbl_80469340,
    lbl_80469354,
    lbl_80469368,
};

class cls_801BB1A8 {
public:
    cls_801BB1A8() {}
    s32 x;
    s32 y;
};

ZoraEnemy_804A3D7C::ZoraEnemy_804A3D7C() {
    field_0x2a8 = 0;
}

ZoraEnemy_804A3D7C::~ZoraEnemy_804A3D7C() {
    releaseEmitter_801EE434(field_0x2a8);
    field_0x2a8 = NULL;
}

static inline u32 clamp(u32 val, u32 min, u32 max) {
    if (val > max) {
        val = max;
    } else if (val < min) {
        val = min;
    }
    return val;
}

void ZoraEnemy_804A3D7C::init() {
    EnemyBase_804A4808::init();
    fn_801D0E58();
    field_0x98 = 2;
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.set(-7.999f, -7.999f, 7.999f, 7.999f);
    if (mActorId == 'LTZR') {
        field_0xb0 = 0;
    } else {
        field_0xb0 = 0x10101;
    }
    mFlags |= ENEMY_FLAG_NO_ATTACK;
    field_0x54.set(0.0f, 0.0f, 0.0f);
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(121) + 60;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A3D68, 5);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();
    field_0x90 = clamp(field_0x90, 0U, 15U);
    vfunc_0xA4();
}

// FIXME: 99.44 %, regalloc-only. Minor register allocation differences in tile search loop (r22/r23/r24/r25/r27).
void ZoraEnemy_804A3D7C::fn_801BACE8(s32 state) {
    switch (state) {
    case 0:
        field_0x250 = fn_801CD664(121) + 60;
        field_0x54.set(0.0f, 0.0f, 0.0f);
        break;
    case 1:
        field_0x250 = fn_801CD664(121) + 60;
        mAnim.startAnim_801D22DC(0, 2, false);
        if (mActorId == 'ZORA') {
            JGeometry::TVec3f probePos;
            JGeometry::TVec3f spawnPos = mSpawnPos;
            s32 range = field_0x90;
            if (field_0x90 != 0) {
                s32 count = 0;
                cls_801BB1A8 tiles[961];
                for (s32 dx = -range; dx <= range; dx++) {
                    for (s32 dy = -range; dy <= range; dy++) {
                        probePos.set(mSpawnPos);
                        probePos.x += dx * 16.0f;
                        probePos.y += dy * 16.0f;
                        if (fn_8022461C(&probePos, field_0x4) == 0xD) {
                            tiles[count].x = dx;
                            tiles[count].y = dy;
                            count++;
                        }
                    }
                }
                if (count > 0) {
                    s32 pick = fn_801CD664(count);
                    spawnPos.x += tiles[pick].x * 16.0f;
                    spawnPos.y += tiles[pick].y * 16.0f;
                }
            }
            mPos = spawnPos;
            mPrevPos = spawnPos;
        }
        field_0x54.set(0.0f, 0.0f, 0.0f);
        field_0x2a8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x555, -1, NULL, NULL, NULL, true, 1.0f);
        break;
    case 2:
        field_0x250 = -1;
        mAnim.startAnim_801D22DC(1, 0, false);
        releaseEmitter_801EE434(field_0x2a8);
        field_0x2a8 = NULL;
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15B, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15C, -1, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x40050, 0);
        break;
    case 3:
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        mFlags &= ~ENEMY_FLAG_NO_ATTACK;
        field_0x250 = fn_801CD664(41) + 40;
        mAnim.fn_801D22A0(2, 2);
        break;
    case 4:
        field_0x250 = -1;
        mAnim.startAnim_801D22DC(3, 0, false);
        break;
    case 5:
        vfunc_0xA4();
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        mFlags |= ENEMY_FLAG_NO_ATTACK;
        field_0x250 = -1;
        mAnim.startAnim_801D22DC(4, 0, false);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15B, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15C, -1, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x50032, 0);
        break;
    }
    mStateTimer = 0;
    mState = state;
}

void ZoraEnemy_804A3D7C::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (mActorId == 'LTZR') {
            mBox.set(-8.4f, -8.4f, 8.4f, 8.4f);
            mBox2.i.x = mBox.i.x;
            mBox2.i.y = mBox.i.y;
            mBox2.f.x = mBox.f.x;
            mBox2.f.y = mBox.f.y;
        }
        vfunc_0xA8();
        if (mStateTimer >= field_0x250) {
            if (fn_801D05F8(&mPos, 300.0f) >= 0) {
                fn_801BACE8(1);
            }
        }
        break;
    case 1:
        startSound_801F0E34(0x4006F, 0);
        if (mActorId == 'LTZR') {
            mBox.set(-8.4f, -8.4f, 8.4f, 8.4f);
            mBox2.i.x = mBox.i.x;
            mBox2.i.y = mBox.i.y;
            mBox2.f.x = mBox.f.x;
            mBox2.f.y = mBox.f.y;
        }
        vfunc_0xA8();
        if (field_0x54.x != 1.5f) {
            fn_801CD950(&field_0x54.x, 1.5f, 0.05f);
            field_0x54.y = field_0x54.x;
            field_0x54.z = field_0x54.x;
        } else if (mStateTimer >= field_0x250) {
            if (mAnim.fn_801D217C()) {
                fn_801BACE8(2);
                vfunc_0xB0();
            }
        }
        break;
    case 2:
        if (mStateTimer > 1) {
            if (mAnim.fn_801D21F0()) {
                fn_801BACE8(3);
            }
        }
        break;
    case 3:
        if (mStateTimer >= field_0x250) {
            if (mAnim.fn_801D217C()) {
                fn_801BACE8(4);
            }
        }
        break;
    case 4:
        if (mStateTimer > 1) {
            if (mActorId == 'ZORA') {
                if (mAnim.mFrameCtrl.checkPass(mAnim.mLength - 2)) {
                    if (fn_801CECF4('ZOSH', 1.5f, 300.0f)) {
                        startSound_801F0E34(0x50030, 0);
                    }
                }
            }
            if (mAnim.fn_801D21F0()) {
                fn_801BACE8(5);
            }
        }
        break;
    case 5:
        if (mActorId == 'LTZR') {
            mBox.set(-8.4f, -8.4f, 8.4f, 8.4f);
            mBox2.i.x = mBox.i.x;
            mBox2.i.y = mBox.i.y;
            mBox2.f.x = mBox.f.x;
            mBox2.f.y = mBox.f.y;
        }
        if (mStateTimer > 1) {
            if (mAnim.fn_801D21F0()) {
                fn_801BACE8(0);
            }
        }
        break;
    }
    vfunc_0xAC();
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 5:
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        break;
    default:
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        break;
    }
}

bool ZoraEnemy_804A3D7C::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (fn_8022461C(pos, layer) == 0xD) {
        return false;
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}

void ZoraEnemy_804A3D7C::vfunc_0xA0() {
    if (mState != 0) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

// FIXME: 93.96 %, missing/extra (3 instructions). The original materialises isZero() as a bool (mfcr; extrwi.; bne) and
// loads field_0x54.z, .y, .x ahead of the matrix stores (ours loads each just before its store); the mCel range test
// also branches differently (bge/b vs blt).
void ZoraEnemy_804A3D7C::draw() {
    switch (mState) {
    case 0:
    case 1:
        return;
    default:
        break;
    }
    if (field_0x54.isZero()) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = field_0x54.x;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = field_0x54.y;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = field_0x54.z;
    params.mMtx = mtx;
    if (mAnim.mCel < 16 && mAnim.mCel >= 9) {
        params.field_0x1c = -1;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0xD5, &params, -1, 1.0f, 1.0f);
}

void ZoraEnemy_804A3D7C::vfunc_0xA4() {}

void ZoraEnemy_804A3D7C::vfunc_0xA8() {}

bool ZoraEnemy_804A3D7C::vfunc_0xAC() {
    return false;
}

void ZoraEnemy_804A3D7C::vfunc_0xB0() {}
