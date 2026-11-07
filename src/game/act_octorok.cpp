#include "game/GameAudio.h"
#include "game/act/act_octorok.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "JSystem/JMath/JMath.h"

static f32 lbl_8049F1A0[4][2] = {
    {4.0f, 2.0f},
    {-4.0f, 2.0f},
    {0.0f, 4.0f},
    {0.0f, -4.0f},
};
static s32 lbl_8049F1C0[8] = {0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x20, 0x1F, 0x21};
static s32 lbl_8049F1E0[6] = {0x1C, 0x1E, 0x20, 0x1D, 0x21, 0x1F};
static s32 lbl_8049F1F8[4] = {1, 2, 0, 3};

s32 lbl_8053AB28;

// FIXME: 97.41 % (scheduling+regalloc). The 1.0f literal load (lfs f5) is scheduled earlier than in the
// original, and the zero, LCG multiplier (0x660d) and 3 constants take r8/r7/r6 instead of r7/r6/r5.
void Octorok_8049F208::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, 0.0f, 8.0f, 8.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x260 = 1.0f;
    field_0x264 = 1.0f;
    field_0x268 = 1.0f;
    field_0x26c = 0.3f;
    field_0x274 = lbl_8053AB28 & 0x1F;
    lbl_8053AB28++;
    field_0x90 &= 0xFF;
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3;
    field_0xb0 = 0x11101;
    field_0x270 = 0;
    field_0x25c = 1.0f;
    field_0x278 = 3;
    field_0x234 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
    field_0x238 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 100;
    s32 type = field_0x90;
    // An empty case 3 matches the original's branch tree.
    switch (type) {
    case 0:
        break;
    case 1:
        field_0x25c = 0.35f;
        field_0xc0 = 3;
        mLife = 48;
        field_0x260 = 0.7f;
        field_0x264 = 0.7f;
        field_0x268 = 0.7f;
        field_0x278 = 4;
        mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
        field_0x80.set(-12.0f, -4.0f, 12.0f, 12.0f);
        break;
    case 2:
        field_0x238 = 10000;
        field_0x270 = 400;
        field_0xb8 = 1;
        field_0x278 = 5;
        break;
    case 3:
        break;
    }
    switch (field_0x98) {
    case 0:
        field_0x27c.set(field_0x25c, 0.0f, 0.0f);
        break;
    case 1:
        field_0x27c.set(-field_0x25c, 0.0f, 0.0f);
        break;
    case 2:
        field_0x27c.set(0.0f, field_0x25c, 0.0f);
        break;
    case 3:
        field_0x27c.set(0.0f, -field_0x25c, 0.0f);
        break;
    }
    field_0x230 = 0;
}

// FIXME: 99.94 % (regalloc-only). r4/r5 swapped for the LCG multiplier and seed loads in the inlined
// mRandom.get_ufloat_1() calls (0x8B8-0x8C4, 0x910-0x928).
void Octorok_8049F208::update() {
    JGeometry::TVec3f focusPos = mPos;
    s32 dir = -1;
    f32 dist = 0.0f;
    u16 angle = 0;
    if (field_0x90 == 2) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    addCalc2_801F71A4(&field_0x260, field_0x264, 0.1f, field_0x26c);
    bool flag = true;
    if (field_0x90 == 1 && mLife > 0) {
        flag = false;
    }
    if (field_0x90 == 1) {
        field_0x268 = 0.7f + (f32)(48 - mLife) / 80.0f;
        f32 boxHalf = 4.0f + (f32)(48 - mLife) / 6.0f;
        mBox.set(-boxHalf, -boxHalf, boxHalf, boxHalf);
        if (field_0x268 > 1.0f) {
            field_0x268 = 1.0f;
        }
        if (field_0x230 != 1) {
            field_0x264 = field_0x268;
            if (field_0xd8 == 0) {
                if (field_0x248 == 0) {
                    field_0x248 = 4;
                    mLife++;
                    if (mLife > 48) {
                        mLife = 48;
                    }
                }
                field_0x26c = 0.05f;
            } else {
                field_0x26c = 0.5f;
            }
        }
    }
    if (fn_801F3D94(flag) != 0) {
        if (field_0x90 == 1 && mLife > 0) {
            field_0x248 = 4;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
        } else {
            fn_801F2618(NULL, false);
            return;
        }
    }
    if (fn_801F305C()) {
        if (field_0x90 == 1 && mLife > 0) {
            field_0x248 = 4;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x260 = 1.3f * field_0x268;
        } else {
            fn_801F2618(NULL, false);
            return;
        }
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
    if (field_0x244 > 0) {
        field_0x244--;
    }
    if (field_0x248 > 0) {
        field_0x248--;
    }
    addCalc2_801F71A4(&field_0x3c.x, field_0x27c.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x27c.y, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);
    u8 mapHit = fn_801F2618(NULL, false);
    switch (field_0x230) {
    case 0:
        if (mapHit || field_0x234 == 0) {
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            if (field_0x90 != 2) {
                if ((field_0x98 & 2) == 0) {
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                        field_0x27c.set(0.0f, field_0x25c, 0.0f);
                        field_0x98 = 2;
                    } else {
                        field_0x27c.set(0.0f, -field_0x25c, 0.0f);
                        field_0x98 = 3;
                    }
                } else {
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                        field_0x27c.set(field_0x25c, 0.0f, 0.0f);
                        field_0x98 = 0;
                    } else {
                        field_0x27c.set(-field_0x25c, 0.0f, 0.0f);
                        field_0x98 = 1;
                    }
                }
            }
            if (field_0x234 == 0) {
                field_0x234 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
            }
        }
        if (field_0x90 == 0 || field_0x90 == 3) {
            fn_801EEC98(mSerial, NULL, &dir, NULL, NULL, 0, 0);
            if (dir == field_0x98) {
                field_0x238 = 0;
            }
        }
        if (field_0x90 == 2) {
            fn_801EEC98(mSerial, &angle, &dir, &dist, NULL, 0, 0);
            if (dir != -1) {
                field_0x98 = dir;
                if (dist < 64.0f) {
                    angle += 0x8000;
                    angle += 0x2000 - (field_0x274 & 0xF) * 0x444;
                    field_0x27c.x = 1.2f * JMASinShort(angle);
                    field_0x27c.y = 1.2f * JMACosShort(angle);
                } else {
                    field_0x27c.z = 0.0f;
                    field_0x27c.y = 0.0f;
                    field_0x27c.x = 0.0f;
                }
            }
            if (gGameRoot_8053AAF8->field_0x4->field_0x34 % field_0x270 == 0) {
                field_0x238 = 0;
                field_0x23c = 0;
            }
        }
        if (field_0x90 != 3 && field_0x238 == 0 && field_0x23c == 0) {
            field_0x24c = 0;
            field_0x250 = 0;
            field_0x254 = 0;
            field_0x258 = 0;
            switch (field_0x98) {
            case 0:
                field_0x254 = 2;
                break;
            case 1:
                field_0x254 = 0;
                break;
            case 2:
                field_0x254 = 1;
                break;
            case 3:
                field_0x254 = 3;
                break;
            }
            field_0x27c.set(0.0f, 0.0f, 0.0f);
            field_0x23c = 8;
            if (field_0x90 == 0) {
                field_0x250 = 3;
                field_0x23c = 20;
            }
            if (field_0x90 == 2 || field_0x90 == 1) {
                field_0x264 = 1.3f * field_0x268;
                field_0x26c = 0.3f;
                field_0x230 = 1;
            } else {
                field_0x230 = 2;
            }
        }
        break;
    case 1:
        if (FABS(field_0x260 - field_0x264) > 0.01f) {
            break;
        }
        field_0x250++;
        field_0x24c++;
        field_0x244 = 0;
        field_0x258 = 0;
        field_0x264 = field_0x268;
        field_0x260 = field_0x268;
        field_0x230 = 2;
        // fallthrough
    case 2:
        if (field_0x244 == 0) {
            field_0x244 = 4;
            field_0x258++;
            if (field_0x258 > 5) {
                field_0x258 = 0;
            }
        }
        if (field_0x23c == 0) {
            field_0x23c = 8;
            if (field_0x24c == 0) {
                field_0x250++;
                field_0x24c++;
                if (field_0x250 > 4) {
                    switch (field_0x98) {
                    case 0:
                        field_0x27c.set(field_0x25c, 0.0f, 0.0f);
                        break;
                    case 1:
                        field_0x27c.set(-field_0x25c, 0.0f, 0.0f);
                        break;
                    case 2:
                        field_0x27c.set(0.0f, field_0x25c, 0.0f);
                        break;
                    case 3:
                        field_0x27c.set(0.0f, -field_0x25c, 0.0f);
                        break;
                    }
                    field_0x234 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                    field_0x238 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 100;
                    field_0x23c = (s32)(0.5 * field_0x238);
                    if (field_0x90 == 2) {
                        field_0x238 = 10000;
                    }
                    field_0x230 = 0;
                }
            } else {
                if (field_0x90 != 2) {
                    field_0x98 = lbl_8049F1F8[field_0x254];
                }
                JGeometry::TVec3f rockVel;
                JGeometry::TVec3f rockPos;
                switch (field_0x98) {
                case 0:
                    rockVel.set(3.0f, 0.0f, 0.0f);
                    break;
                case 1:
                    rockVel.set(-3.0f, 0.0f, 0.0f);
                    break;
                case 2:
                    rockVel.set(0.0f, 3.0f, 0.0f);
                    break;
                case 3:
                    rockVel.set(0.0f, -3.0f, 0.0f);
                    break;
                }
                field_0x254++;
                field_0x254 &= 3;
                if (field_0x90 == 0) {
                    field_0x23c = 20;
                }
                rockPos.x = mPos.x;
                rockPos.y = mPos.y;
                rockPos.z = mPos.z;
                rockPos.x += lbl_8049F1A0[field_0x98][0];
                rockPos.y += lbl_8049F1A0[field_0x98][1];
                startSound_801F0E34(0x4000A, 0);
                s32 spawned;
                switch ((s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1())) {
                case 0:
                    spawned = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &rockPos, 0x20002);
                    break;
                default:
                    spawned = getActorManager_801F856C()->spawnActor_801F9484('OKTM', field_0x4, &rockPos, 0, -1, -1);
                    break;
                }
                if (spawned >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(spawned);
                    if (actor != NULL) {
                        actor->field_0x3c = rockVel;
                        actor->field_0x98 = field_0x98;
                    }
                }
                field_0x24c = 0;
            }
        }
        break;
    }
}

// FIXME: 92.33 % (scheduling+regalloc). The view->mVisibleBox copy and the pos TVec3f stores are
// scheduled/allocated differently (0x1A8-0x210), and the (field_0x234 >> 3) & 1 / field_0x98 * 2 index
// arithmetic swaps registers and operand order (0x26C-0x330).
void Octorok_8049F208::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x0[3] = field_0x278;
    s32 step;
    if (field_0x230 == 2 || field_0x230 == 1) {
        step = field_0x24c + 2;
    } else if (field_0x90 == 1) {
        step = (field_0x234 >> 3) & 1;
    } else {
        step = (field_0x234 >> 2) & 1;
    }
    mtx[0][0] = field_0x260;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = field_0x260;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = field_0x260;
    params.mMtx = mtx;
    if (field_0x4 == 8) {
        s32 offset = 0;
        GameView_804A9950* view = getGameView_802D6A9C();
        JGeometry::TBox2f box;
        box = view->mVisibleBox;
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        pos.x += box.i.x;
        pos.y += box.i.y;
        if (field_0x90 == 1) {
            offset = 0x446;
        }
        fn_801EF574("enemy_gc_sob.bin", offset + (field_0x98 * 4 + step), &pos, &params);
        s32 frame;
        if (field_0x230 == 2 || field_0x230 == 1) {
            frame = lbl_8049F1E0[field_0x258];
        } else {
            frame = lbl_8049F1C0[field_0x98 * 2 + ((field_0x234 >> 3) & 1)];
        }
        fn_803075AC(mSerial, field_0x4, &mPos, frame + 0x158, 0, 2, -1, false, 0, 0);
    } else if (field_0x90 == 1) {
        fn_801F02BC("enemy_gc_sob.bin", (field_0x98 * 4 + step) + 0x446, &params, 7, 0, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_gc_sob.bin", field_0x98 * 4 + step, &params, 7, 0, false, 1.0f, 1.0f);
    }
}

Octorok_8049F208::~Octorok_8049F208() {}
