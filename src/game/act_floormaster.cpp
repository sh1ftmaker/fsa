#include "game/GameAudio.h"
#include "game/act/act_floormaster.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"

static const s16 lbl_8046A3D8[] = { 0, 5, 1, 5, 2, 5, -1, -1 };
static const s16 lbl_8046A3E8[] = { 3, 5, 4, 10, -1, -1 };
static const s16 lbl_8046A3F4[] = { 2, 10, 5, 5, -1, -1 };
static const s16 lbl_8053FA20[] = { 3, 40, -1, -1 };
static const s16 lbl_8046A400[] = { 6, 5, 7, 5, 8, 5, -1, -1 };
static const s16 lbl_8046A410[] = { 2, 10, 5, 5, -1, -1 };
static const s16 lbl_8046A41C[] = { 3, 5, 6, 5, 7, 5, 8, 5, -1, -1 };

static const s16* lbl_804A5FE8[] = {
    lbl_8046A3D8,
    lbl_8046A3E8,
    lbl_8046A3F4,
    lbl_8053FA20,
    lbl_8046A400,
    lbl_8046A410,
    lbl_8046A41C,
};

Floormaster_804A6004::Floormaster_804A6004() {
    field_0x2a8 = 0.0f;
    field_0x2ac = 0.0f;
    mEmitter2b0 = NULL;
    field_0x2b4 = 0.0f;
    field_0x2b8 = -1;
    field_0x2bc = 0;
    field_0x2c0 = -1;
}

Floormaster_804A6004::~Floormaster_804A6004() {
    releaseEmitter_801EE434(mEmitter2b0);
}

// FIXME: 98.61%. scheduling+regalloc + reloc-name (literal pool and float conversion stack order)
void Floormaster_804A6004::init() {
    EnemyBase_804A4808::init();
    if (field_0x90 & 1) {
        mFlags |= 0x2000;
        u32 arg = field_0x90;
        u32 y = ((arg >> 8) & 0x3F) * 8;
        u32 x = ((arg >> 16) & 0x3F) * 8;
        field_0x2a8 = 8.0f + (f32)x;
        field_0x2ac = 8.0f + (f32)y;
    }
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x50303;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(61) + 60;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A5FE8, 7);
    mAnim.startAnim_801D22DC(0, 0, false);
}

// FIXME: 98.69%. regalloc-only + reloc-name
void Floormaster_804A6004::vfunc_0x94() {
    if (mState == 4) return;
    EnemyBase_804A4808::vfunc_0x94();
    if (!(mBox.f.x - mBox.i.x > 0.0f)) return;
    if (!(mBox.f.y - mBox.i.y > 0.0f)) return;
    switch (mAnim.mCel) {
    case 2:
        mBox.set(-7.0f, -12.0f, 16.0f, 8.0f);
        break;
    case 3:
        mBox.set(-12.0f, -22.0f, 2.0f, 8.0f);
        break;
    case 4:
        mBox.set(-12.0f, -20.0f, 1.0f, 8.0f);
        break;
    case 5:
        mBox.set(-4.0f, -16.0f, 16.0f, 8.0f);
        break;
    }
    if (mFlags & 0x8000) {
        f32 minX = -mBox.f.x;
        f32 maxX = -mBox.i.x;
        mBox.i.x = minX;
        mBox.f.x = maxX;
    }
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
}

// FIXME: 99.64%. reloc-name
void Floormaster_804A6004::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    switch (mState) {
    case 1:
        if (field_0x2b8 >= 0) {
            JGeometry::TVec2f vec;
            fn_801D00B4(field_0x2b8, &vec, 1.0f);
            field_0x3c.x = vec.x;
            field_0x3c.y = vec.y;
        }
        break;
    }
}

// FIXME: 99.91%. reloc-name
bool Floormaster_804A6004::vfunc_0x60() {
    bool result = false;
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        result = fn_801F2DB4(mActorId, 'NULL', NULL);
        if (result) {
            startSound_801F0E34(0x5007D, 0);
            bool change = false;
            switch (mState) {
            case 3:
            case 5:
            case 6:
            case 8:
                change = true;
                break;
            }
            if (change) {
                fn_8020ED5C(4);
            }
        }
    }
    return result;
}

// FIXME: 99.88%. reloc-name
bool Floormaster_804A6004::vfunc_0x5C() {
    bool result = false;
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        for (int i = 0; i < 4; i++) {
            s32 flag;
            JGeometry::TVec2f pos;
            if (i == fn_801F285C(&pos, &flag, true, i)) {
                fn_8023B6CC(i);
                if (field_0x1dc.fn_801EAC20(i)) {
                    JGeometry::TVec3f effectPos(pos.x, pos.y, 0.0f);
                    result = true;
                    fn_801CB538(field_0x4, &effectPos, 0, 0);
                }
            }
        }

        if (result) {
            startSound_801F0E34(0x5007D, 0);
            bool change = false;
            switch (mState) {
            case 3:
            case 5:
            case 6:
            case 8:
                change = true;
                break;
            }
            if (change) {
                fn_8020ED5C(4);
            }
        }
    }
    return result;
}

void Floormaster_804A6004::vfunc_0xA0() {
    if (mState != 0) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

void Floormaster_804A6004::draw() {
    switch (mState) {
    default: {
        JGeometry::TVec3f pos = mPos;
        if (mState == 4 && mStateTimer >= 60) {
            if (mStateTimer % 4 < 2) {
                pos.x += 1.0f;
            } else {
                pos.x -= 1.0f;
            }
        }
        SpriteParams_801F5AE0 params;
        params.field_0x1c = 0;
        params.field_0x1d = 0;
        Mtx mtx;
        if (mFlags & 0x8000) {
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            mtx[0][0] = -1.0f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 1.0f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 1.0f;
            params.mMtx = mtx;
        }
        s32 frame = mAnim.mCel + 0x321;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
        }
        break;
    }
    case 0:
    case 1:
    case 10:
        break;
    }
}
