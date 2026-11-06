#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/ActorManager.h"
#include "game/cls_804B1C58.h"
#include "game/Unk_8045A080.h"
#include "game/act/act_shooting_gallery.h"

s32 lbl_804A2B08[4] = {0, 1, 2, 1};

s32 lbl_804C6330[4];

// FIXME: 99.86%. reloc-name.
void ShootingGallery_804A2B18::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(-10.0f, -10.0f, 10.0f, 10.0f);

    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;

    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    mTimers[3] = 0;

    field_0x98 = 2;
    field_0x264 = 0;
    field_0x290 = 1.0f;
    field_0x294 = 0.0f;
    field_0x298 = 255.0f;
    field_0x28c = 0;
    field_0x2a8 = 0.0f;
    field_0x2a4 = 0.0f;
    field_0x2a0 = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x29c = mPos.y;

    field_0x268 = field_0x90 & 0xF;
    switch (field_0x268) {
    case 0:
        lbl_8053ABB8[0] = 0;
        lbl_804C6330[0] = 0;
        lbl_8053ABB8[1] = 0;
        lbl_804C6330[1] = 0;
        lbl_8053ABB8[2] = 0;
        lbl_804C6330[2] = 0;
        lbl_8053ABB8[3] = 0;
        lbl_804C6330[3] = 0;
        field_0xb0 = 0x0A040004;
        if (vfunc_0x34()) {
            mState = 6;
        } else {
            mState = 0;
        }
        break;
    case 1:
    case 2:
        mState = 9;
        break;
    case 3:
        mLife = 10000;
        field_0xb0 = 0xF;
        mState = 14;
        break;
    case 4:
        mState = 11;
        break;
    }
}

// FIXME: 99.08%. regalloc-only: r30/r31 swapped in loop.
ShootingGallery_804A2B18::~ShootingGallery_804A2B18() {
    if (field_0x268 == 0) {
        if (mState == 3 || mState == 4) {
            fn_8022B17C();
            if (gGameRoot_8053AAF8->field_0x4c != 9) {
                gGameRoot_8053AAF8->fn_80138B7C();
                gAudioMgr_8053AF80->fn_80458508(0);
            }
            for (int i = 0; i < 4; i++) {
                if (!fn_8022F514(i)) {
                    fn_802372F8(i, field_0x26c[i], field_0x27c[i]);
                }
            }
        }
    }
}

// FIXME: 94.81%. missing/extra: ours has 2 fewer instrs; regalloc in LCG mRandom.get_ufloat_1().
void ShootingGallery_804A2B18::fn_8019EE74() {
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2f p(mPos.x, mPos.y);
    box.addPos(p);

    addCalc2_801F71A4(&field_0x3c.x, field_0x2a0, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x2a4, 0.3f, 1.0f);
    mPos += field_0x3c;

    switch (mState) {
    case 9:
        mTimers[0] = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 60;
        mTimers[1] = (s32)(80.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 80;
        field_0x2a0 = 0.7f;
        if (field_0x98 != 0) {
            field_0x2a0 = -0.7f;
        }
        mState = 10;
        // fallthrough
    case 10:
        field_0xb0 = 3;
        if (mTimers[2] == 1) {
            mAlive = false;
        } else if (mTimers[2] == 0) {
            if (fn_801A6108(mSerial, &box, &p)) {
                field_0xb0 = 7;
                field_0x248 = fn_8023AE34(&box, field_0x4, &p);
                if (field_0x268 == 1) {
                    lbl_8053AED0->fn_8043D874(field_0x248, 300, 0);
                    for (int i = 0; i < 20; i++) {
                        JGeometry::TVec3f spawnPos(mPos.x, mPos.y, mPos.z);
                        spawnPos.x += 8.0f - 16.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                        spawnPos.y += 8.0f - 16.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                        getActorManager_801F856C()->spawnActor_801F9484('SYKY', field_0x4, &spawnPos, 4, -1, -1);
                    }
                } else {
                    lbl_8053AED0->fn_8043D874(field_0x248, 20, 0);
                }
                mTimers[2] = 2;
                fn_801F0D20(4);
                return;
            }
        }
        break;
    }

    if (field_0x268 == 1) {
        if (mTimers[1] != 0) {
            if (mTimers[1] == 1) {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.8f) {
                    field_0x98 ^= 1;
                    field_0x2a0 = 0.7f;
                    if (field_0x98 != 0) {
                        field_0x2a0 = -0.7f;
                    }
                }
            }
        }
    } else {
        if (field_0x98 == 0) {
            if (mPos.x > 360.0f) {
                mAlive = false;
            }
        } else {
            if (mPos.x < 150.0f) {
                mAlive = false;
            }
        }
    }

    if (mTimers[0] == 0) {
        field_0x264++;
        field_0x264 &= 3;
        if (field_0x264 == 0) {
            mTimers[0] = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 60;
        } else {
            mTimers[0] = 2;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

void ShootingGallery_804A2B18::vfunc_0x44() {
    if (field_0x268 == 3 && mState != 15) {
        field_0x290 = 1.5f;
        mPos.y -= 16.0f;
        gAudioMgr_8053AF80->fn_80458880(6, -1);
        mState = 15;
    }
}

void ShootingGallery_804A2B18::fn_8019EC04() {
    addCalc2_801F71A4(&field_0x3c.x, field_0x2a0, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x2a4, 0.3f, 1.0f);
    mLife = 10000;
    mPos += field_0x3c;

    if (field_0x98 == 0) {
        if (mPos.x > 360.0f) {
            mAlive = false;
        }
    } else {
        if (mPos.x < 150.0f) {
            mAlive = false;
        }
    }

    switch (mState) {
    case 14:
        field_0x2a0 = 0.7f;
        if (field_0x98 != 0) {
            field_0x2a0 = -0.7f;
        }
        field_0x29c = mPos.y;
        if (mTimers[0] == 0) {
            mTimers[0] = 4;
            field_0x264++;
            field_0x264 &= 1;
        }
        break;
    case 15:
        addCalc2_801F71A4(&field_0x290, 1.0f, 0.2f, 0.1f);
        addCalc2_801F71A4(&mPos.y, field_0x29c, 0.3f, 2.0f);
        if (mTimers[0] == 0) {
            mTimers[0] = 2;
            field_0x264++;
            field_0x264 &= 1;
        }
        if ((f32)fabs(mPos.y - field_0x29c) < 0.1f) {
            field_0x290 = 1.0f;
            mState = 14;
        }
        break;
    }

    for (int i = 0; i < 4; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

// FIXME: 99.44%. regalloc-only: r4/r5 swapped for gGameRoot_8053AAF8 vs 0x19660D in mRandom.get_ufloat_1().
void ShootingGallery_804A2B18::update() {
    switch (field_0x268) {
    case 0:
        fn_8019F3AC();
        break;
    case 1:
    case 2:
        fn_8019EE74();
        break;
    case 3:
        fn_8019EC04();
        break;
    case 4:
        addCalc2_801F71A4(&field_0x3c.x, field_0x2a0, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x2a4, 0.3f, 1.0f);
        mPos += field_0x3c;
        if ((mSerial & 1) == 0) {
            field_0x294 += 4096.0f;
        } else {
            field_0x294 -= 4096.0f;
        }
        switch (mState) {
        case 11:
            field_0x2a0 = 2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            field_0x2a4 = 2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            field_0x3c.z = 10.0f + 5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            field_0x290 = 0.5f + 0.3f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            mState = 12;
            break;
        case 12:
            field_0x3c.z -= 1.0f;
            if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
                field_0x3c.z = 2.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                mState = 13;
            }
            break;
        case 13:
            field_0x3c.z -= 1.0f;
            if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
                mAlive = false;
            }
            break;
        }
        addCalc2_801F71A4(&field_0x298, 1.0f, 0.3f, 5.0f);
        if (field_0x298 < 1.0f) {
            mAlive = false;
        }
        break;
    }
}

// Not auto-inlined: draw calls fn_8019E7A4 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME: 99.23%. regalloc-only: f0 and f3 swapped for 0.0f vs field_0x290.
void ShootingGallery_804A2B18::fn_8019E7A4() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = field_0x290;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = field_0x290;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = field_0x290;
    params.mMtx = mtx;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 8, &params, 0, 1.0f, 1.0f);
}
#pragma auto_inline reset

// FIXME: 98.68%. regalloc-only: r30/r31 and f28-f31 in setRotScaleMtx.
void ShootingGallery_804A2B18::fn_8019E480() {
    SpriteParams_801F5AE0 params;
    s32 frame = 0;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    JGeometry::TVec3f rot(0.0f, field_0x294 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, field_0x290);
    params.mMtx = mtx;

    switch (field_0x268) {
    case 4:
        params.field_0x30 = 0x11;
        frame = 0xd;
        break;
    case 2:
        frame = 9;
        break;
    case 1:
        frame = 0x1d;
        break;
    }

    params.field_0x10.a = field_0x298;

    frame += field_0x264;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
}

void ShootingGallery_804A2B18::draw() {
    switch (field_0x268) {
    case 0:
        fn_801F02BC("enemy_both2_sob.bin", field_0x264 + 0x1AB, NULL, 0, 0, false, 1.0f, 1.0f);
        if (field_0x28c) {
            for (int i = 0; i < 4; i++) {
                if (!fn_8022F478(i)) {
                    s32 slot = fn_8022F340(i);
                    lbl_8053AF10->field_0x2c |= (u8)(1 << slot);
                    lbl_8053AF10->field_0x28[slot] = 10 - lbl_804C6330[slot];
                }
            }
        }
        break;
    case 1:
    case 2:
    case 4:
        fn_8019E480();
        break;
    case 3:
        fn_8019E7A4();
        break;
    }
}
