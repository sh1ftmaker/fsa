#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/RotMatrix.h"
#include "game/act/act_buzz_blob.h"

s32 lbl_8049F408[4] = {0, 1, 2, 1};
s32 lbl_8049F418[11] = {4, 8, 6, 7, 5, 9, 4, 7, 8, 7, 9};

void BuzzBlob_8049F444::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x90 &= 1;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0xb0 = 0x11101;
    field_0x244 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x254 = 0.0f;
    field_0x250 = 1.0f;
    field_0x258.set(1.0f, 1.0f, 1.0f);
    field_0x230 = 0;
}

// FIXME: 99.82% (regalloc-only). Target vs ours: r5->r4, r4->r5 in the mRandom
// loads after dir.scale(0.3f) in case 0.
void BuzzBlob_8049F444::update() {
    if (fn_801F3D94(1) != 0) {
        field_0x258.set(1.0f, 1.0f, 1.0f);
        fn_801F2618(NULL, false);
        return;
    }

    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }

    mPos += field_0x3c;
    u8 landed = 0;
    if (field_0x230 != 4) {
        s32 code = fn_8022461C(&mPos, field_0x4);
        if (code == 0xe) {
            startSound_801F0E34(0x4001A, 0);
            field_0x230 = 4;
        } else {
            JGeometry::TBox2f box(mBox);
            JGeometry::TVec2f pos(mPos.x, mPos.y);
            box.addPos(pos);
            s32 player = fn_8023B004(&box, field_0x4);
            if (player >= 0 && !fn_8023E854(player)) {
                JGeometry::TVec2f center(box.f.x + 0.5f * (box.i.x - box.f.x),
                                         box.f.y + 0.5f * (box.i.y - box.f.y));
                fn_80238548(player, 1, &center, 10);
                startSound_801F0E34(0x4000C, 0);
                field_0x258.set(1.0f, 1.0f, 1.0f);
                field_0x244 = 0;
                field_0x23c = 4;
                field_0x3c.set(0.0f, 0.0f, 0.0f);
                field_0x258.set(1.0f, 1.0f, 1.0f);
                field_0x230 = 3;
            }
            for (int i = 0; i < 4; i++) {
                s32 p = fn_801F285C(NULL, NULL, true, i);
                if (p >= 0) {
                    if (!fn_8023E854(p)) {
                        JGeometry::TVec2f pos2d(mPos.x, mPos.y);
                        fn_80238548(p, 1, &pos2d, 10);
                        startSound_801F0E34(0x4000C, 0);
                        if (field_0x230 != 3) {
                            field_0x244 = 0;
                            field_0x23c = 4;
                            field_0x3c.x = 0.0f;
                            field_0x3c.y = 0.0f;
                            field_0x258.set(1.0f, 1.0f, 1.0f);
                            field_0x230 = 3;
                        }
                    } else {
                        fn_801F2FAC();
                    }
                }
            }
            fn_801F2DB4('NULL', 'NULL', NULL);
            landed = fn_801F2618(NULL, false);
        }
    }

    switch (field_0x230) {
    case 0:
        if (landed || field_0x238 == 0) {
            field_0x238 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
            JGeometry::TVec3f dir;
            fn_801EE8F4(&dir, NULL, 0);
            dir.scale(0.3f);
            field_0x3c.x = 0.3f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + dir.x;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x3c.x *= -1.0f;
            }
            field_0x3c.y = 0.3f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + dir.y;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x3c.y *= -1.0f;
            }
        }
        if (field_0x90 == 1 && field_0x240 == 0) {
            u16 angle = 0;
            f32 dist = 0.0f;
            s32 player = -1;
            s32 dir = -1;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
            if (dir >= 0 && (FABS(getPlayerPos_8023E724(player)->y - mPos.y) < 24.0f ||
                             FABS(getPlayerPos_8023E724(player)->x - mPos.x) < 24.0f))
            {
                if (!fn_801F23BC(field_0x4, &mPos, getPlayerPos_8023E724(player), &field_0x80, NULL, NULL, false)) {
                    field_0x98 = dir;
                    field_0x230 = 1;
                }
            }
        }
        break;
    case 1:
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        switch (field_0x98) {
        case 0:
            field_0x244 = 0;
            break;
        case 1:
            field_0x244 = 2;
            break;
        default:
            field_0x244 = 1;
            break;
        }
        field_0x240 = 20;
        field_0x230 = 2;
        // fall through
    case 2:
        if (field_0x240 != 0) {
            addCalc2_801F71A4(&field_0x258.x, 1.2f, 0.1f, 0.2f);
            addCalc2_801F71A4(&field_0x258.y, 0.6f, 0.1f, 0.2f);
            if (field_0x240 == 1) {
                f32 speed = 3.0f;
                switch (field_0x98) {
                case 0:
                    field_0x3c.x = speed;
                    field_0x3c.y = 0.0f;
                    field_0x3c.z = 5.0f;
                    break;
                case 1:
                    field_0x3c.x = -speed;
                    field_0x3c.y = 0.0f;
                    field_0x3c.z = 5.0f;
                    break;
                case 2:
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = speed;
                    field_0x3c.z = 5.0f;
                    break;
                case 3:
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = -speed;
                    field_0x3c.z = 5.0f;
                    break;
                }
            }
        } else {
            addCalc2_801F71A4(&field_0x258.x, 1.0f, 0.1f, 0.2f);
            addCalc2_801F71A4(&field_0x258.y, 1.0f, 0.1f, 0.2f);
            if (mPos.z > 0.0f) {
                field_0x3c.z -= 0.5f;
            } else {
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x258.set(1.0f, 1.0f, 1.0f);
                field_0x240 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
                field_0x230 = 0;
            }
        }
        break;
    case 3:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 0.5f;
        } else {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        if (field_0x23c == 0) {
            field_0x244++;
            field_0x23c = 4;
            if (field_0x244 > 10) {
                field_0x244 = 0;
                field_0x230 = 0;
            }
        }
        break;
    case 4:
        field_0x254 += 0.1f;
        field_0x250 -= 0.02f;
        if (field_0x250 < 0.1f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, -1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5005F, 0);
            fn_801F3668();
        }
        break;
    }

    if (field_0x230 != 3 && field_0x230 != 1 && field_0x230 != 2 && field_0x230 != 4) {
        if (field_0x234 <= 0) {
            field_0x234 = 8;
            field_0x244++;
            field_0x244 &= 3;
        }
    }
}

// FIXME: 99.32% (regalloc-only). Target vs ours: f29->f31, f28->f26, f26->f28,
// f31->f29 in setRotScaleMtx and f0/f1 swapped in mPos.z != 0.0f.
void BuzzBlob_8049F444::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x230 == 4) {
        SpriteParams_801F5AE0 params2;
        Mtx mtx2;
        mtx2[0][3] = 0.0f;
        mtx2[1][3] = 0.0f;
        mtx2[2][3] = 0.0f;
        JGeometry::TVec3f rot(0.0f, field_0x254, 0.0f);
        setRotScaleMtx(mtx2, rot, field_0x250);
        params2.mMtx = mtx2;
        s32 frame = lbl_8049F408[field_0x244] + 0x10;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params2, 1.0f, 1.0f);
        }
    } else if (field_0x230 != 3) {
        f32 sy = field_0x258.y;
        mtx[0][0] = field_0x258.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = sy;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = sy;
        params.mMtx = mtx;
        if (mPos.z != 0.0f) {
            params.field_0x30 = 0x11;
        }
        drawSprite_801F06F0("enemy_gc_sob.bin", lbl_8049F408[field_0x244] + 0x10, &params, 0, 1.0f, 1.0f);
    } else {
        drawSprite_801F06F0("enemy_gc_sob.bin", lbl_8049F418[field_0x244] + 0x10, NULL, 0, 1.0f, 1.0f);
    }
}

BuzzBlob_8049F444::~BuzzBlob_8049F444() {}
