#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/EmitterDelete.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/act/tu_801A908C.h"
#include "JSystem/JMath/JMath.h"

// A counter of the live instances in state 5 (defined elsewhere).
extern s32 lbl_8053ABD0;
extern const u16 lbl_804A2F18[10];
extern const u16 lbl_8053A108[4];

// FIXME(shotgun): 100.00 %, match
void cls_804A2FB0::fn_801AA8F4() {
    if (field_0x234 == 0) {
        field_0x24c = field_0x25c + field_0x240;
        switch (field_0x260) {
        case 0:
            field_0x234 = 6;
            field_0x240++;
            field_0x240 &= 1;
            if (field_0x240 != 0) {
                startSound_801F0E34(0x5002C, 0);
            }
            break;
        case 1:
            field_0x234 = 5;
            field_0x240++;
            if (field_0x240 > 3) {
                field_0x240 = 3;
                field_0x2b0 = 1;
            }
            break;
        case 2:
            field_0x234 = 5;
            field_0x240--;
            if (field_0x240 < 0) {
                field_0x240 = 0;
                field_0x2b0 = 1;
            }
            break;
        case 3:
            field_0x234 = 6;
            field_0x240++;
            if (field_0x240 > 3) {
                field_0x240 = 3;
                field_0x2b0 = 1;
            }
            break;
        case 4:
            field_0x234 = 6;
            field_0x240--;
            if (field_0x240 < 0) {
                field_0x240 = 0;
                field_0x2b0 = 1;
            }
            break;
        }
    }
}

// FIXME(shotgun): 86.86 %, missing/extra + reloc-name
void cls_804A2FB0::init() {
    mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    for (int i = 0; i < 4; i++) {
        field_0x270[i] = -1;
        field_0x280[i] = mPos;
    }
    field_0xb0 = 0x5040004;
    mLife = 10000;
    field_0x258 = field_0x90 & 0xF;
    field_0x268 = -1;
    field_0x25c = 0;
    field_0x260 = -1;
    field_0x264 = 0;
    field_0x254 = 0;
    field_0x2b4 = 0.0f;
    field_0x2b2 = (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 4.0f);
    field_0x2b2 &= 3;
    field_0x2b2 <<= 14;
    field_0x2b2 += 0x2000;
    field_0x2b8 = 0;
    field_0x2b0 = 0;
    field_0x24c = 0;
    field_0x250 = 1;
    field_0x26c = 0;
    if (field_0x258 != 0 && field_0x258 != 5) {
        field_0x264 = field_0x90 - 1;
        setMode(0);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x230 = 5;
    } else {
        field_0x250 = 4;
        field_0x240 = field_0x250;
        field_0x80.set(-16.0f, -16.0f, 16.0f, 16.0f);
        field_0x2b8 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053A108[0], 0, 0, 0, 0, true, 1.0f);
        for (int i = 0; i < 4; i++) {
            field_0x270[i] = getActorManager_801F856C()->spawnActor_801F9484('TTAT', field_0x4, &field_0x280[i], i + 1, -1, -1);
            if (field_0x270[i] >= 0) {
                cls_804A2FB0* child = static_cast<cls_804A2FB0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x270[i]));
                if (child != 0) {
                    child->field_0x268 = mSerial;
                }
            }
        }
        field_0x244 = (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 2.0f);
        field_0x244 &= 1;
        field_0x248 = 0x100;
        if (field_0x258 == 0 || lbl_8053ABD0 != 0) {
            field_0x230 = 1;
        } else if (lbl_8053ABD0 == 0) {
            gAudioMgr_8053AF80->fn_80458F9C(0x3C);
            field_0x230 = 0;
            lbl_8053ABD0++;
        }
    }
}

// FIXME(shotgun): 100.00 %, match
cls_804A2FB0::~cls_804A2FB0() {
    releaseEmitter_801EE434(field_0x2b8);
    if (field_0x258 == 0 && gGameRoot_8053AAF8->field_0x4c != 9) {
        gGameRoot_8053AAF8->fn_80138B7C();
    }
    if (field_0x258 == 5) {
        lbl_8053ABD0--;
    }
}

// FIXME(shotgun): 96.06 %, missing/extra + reloc-name
void cls_804A2FB0::fn_801A9CF4() {
    mLife = 10000;
    fn_801F3D94(0);
    if (field_0x250 > 0 && field_0x26c == 0) {
        if (field_0x268 >= 0) {
            cls_804A2FB0* parent = static_cast<cls_804A2FB0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268));
            if (parent != 0 && parent->field_0x250 > 0) {
                mBox.set(-10.0f, -10.0f, 10.0f, 10.0f);
                s32 hit = fn_801F285C(0, 0, true, -1);
                s32 idx = field_0x264 << 1;
                if (hit >= 0) {
                    if (hit == field_0x264) {
                        fn_801F37AC(0, &mPos, 3, 0, -1);
                        mBox.set(1.0f, -1.0f, 1.0f, -1.0f);
                        startSound_801F0E34(0x5002D, 0);
                        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A2F18[idx], 0, 0, 0, 0, true, 2.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A2F18[idx + 1], 0, 0, 0, 0, true, 2.0f);
                        field_0x250 = 0;
                        parent->field_0x250--;
                    } else {
                        gAudioMgr_8053AF80->fn_80458880(6, -1);
                        field_0x244 = 1;
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x482, 0, 0, 0, 0, true, 2.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x483, 0, 0, 0, 0, true, 2.0f);
                        parent->field_0x26c = 1;
                    }
                    fn_8023B6CC(hit);
                    setMode(3);
                    field_0x230 = 7;
                }
            }
        }
        fn_801F2B7C(-100, -1, 1);
    }
    switch (field_0x230) {
    case 5:
        field_0x244 = 0;
        setMode(0);
        field_0x230 = 6;
        // fallthrough
    case 6:
        if (field_0x268 >= 0) {
            cls_804A2FB0* parent = static_cast<cls_804A2FB0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268));
            if (parent != 0 && parent->field_0x250 <= 0) {
                break;
            }
        }
        if (field_0x26c == 1) {
            setMode(1);
            field_0x230 = 10;
        }
        break;
    case 7:
        if (field_0x2b0 != 0 && field_0x26c == 1) {
            field_0x230 = 8;
        }
        break;
    case 8:
        if (field_0x250 == 0) {
            if (field_0x268 >= 0) {
                cls_804A2FB0* parent = static_cast<cls_804A2FB0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268));
                if (parent != 0) {
                    parent->field_0x250++;
                }
            }
            field_0x250 = 1;
        }
        setMode(4);
        field_0x230 = 9;
        // fallthrough
    case 9:
        if (field_0x2b0 != 0) {
            setMode(1);
            field_0x230 = 10;
        } else {
            break;
        }
        // fallthrough
    case 10:
        if (field_0x2b0 != 0) {
            if (field_0x244 == 0) {
                startSound_801F0E34(0x5002E, 0);
                s32 serial = getActorManager_801F856C()->spawnActor_801F9484('OKTM', field_0x4, &mPos, 4, -1, -1);
                if (serial >= 0) {
                    ActorBase_804A5770* oktm = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (oktm != 0) {
                        f32 ang = 32768.0f + field_0x2b4;
                        JGeometry::TVec3f vel(
                            3.0f * JMASSin((s32)ang),
                            3.0f * JMASCos((s32)ang),
                            0.0f
                        );
                        oktm->field_0x3c = vel;
                        oktm->field_0x98 = fn_801F681C((s32)field_0x2b4);
                    }
                }
            }
            setMode(2);
            field_0x230 = 11;
        }
        break;
    case 11:
        if (field_0x2b0 == 1) {
            field_0x244 = 0;
            field_0x26c = 0;
            field_0x230 = 5;
        }
        break;
    }
    fn_801AA8F4();
}

// FIXME(shotgun): 94.62 %, different
void cls_804A2FB0::fn_801A96B4() {
    f32 speed = 0.0f;
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    field_0x80.set(-40.0f, -40.0f, 40.0f, 40.0f);
    if (field_0x240 != field_0x250 && field_0x250 > 0) {
        field_0x238 = 50;
        field_0x240 = field_0x250;
        deleteEmitter(&field_0x2b8);
        field_0x2b8 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053A108[4 - field_0x250], 0, 0, 0, 0, true, 1.0f);
    }
    setEmitterPos_8013CB44(field_0x2b8, &mPos, 0, 0, true);
    switch (field_0x250) {
    case 4:
        field_0x248 = 0x100;
        break;
    case 3:
        field_0x248 = 0x200;
        break;
    case 2:
        field_0x248 = 0x300;
        break;
    case 1:
        field_0x248 = 0x400;
        break;
    case 0:
        if (field_0x230 != 4) {
            deleteEmitter(&field_0x2b8);
            field_0x230 = 3;
        }
        break;
    }
    if (field_0x230 != 4 && field_0x230 != 3) {
        if (field_0x238 == 0) {
            if (field_0x244 == 0) {
                field_0x2b4 -= (f32)field_0x248;
            } else {
                field_0x2b4 += (f32)field_0x248;
            }
        }
        s32 stepAngle = 0;
        for (int i = 0; i < 4; i++) {
            if (field_0x270[i] != 0) {
                cls_804A2FB0* child = static_cast<cls_804A2FB0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x270[i]));
                if (child != 0) {
                    if (field_0x26c == 1) {
                        child->field_0x26c = 1;
                        field_0x238 = 50;
                    }
                    field_0x280[i].x = mPos.x;
                    field_0x280[i].y = mPos.y;
                    u16 a = (u16)((s32)field_0x2b4 + (u16)stepAngle);
                    field_0x280[i].z = mPos.z;
                    child->field_0x2b4 = (f32)(s32)(a + 0x8000);
                    field_0x280[i].x += 32.0f * JMASSin(a);
                    field_0x280[i].y += 32.0f * JMASCos(a);
                    child->mPos = field_0x280[i];
                }
            }
            switch (field_0x250) {
            case 4:
                speed = 0.5f;
                break;
            case 3:
                speed = 1.0f;
                break;
            case 2:
                speed = 1.5f;
                break;
            case 1:
                speed = 2.0f;
                break;
            }
            if (field_0x238 == 0) {
                field_0x3c.x = speed * JMASSin(field_0x2b2);
                field_0x3c.y = speed * JMASCos(field_0x2b2);
            }
            stepAngle += 0x4000;
        }
        field_0x26c = 0;
        mPos.add(field_0x3c);
        fn_801F2B7C(-100, -1, 1);
    }
    if (field_0x254 == 1 && gGameRoot_8053AAF8->field_0x7f == 1) {
        if (field_0x258 != 5 && lbl_8053ABD0 == 0) {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
        } else {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000004, false, 0);
        }
    } else {
        field_0x254++;
        if (field_0x254 == 3) {
            field_0x254 = 2;
        }
    }
    switch (field_0x230) {
    case 0:
        field_0x230 = 1;
        // fallthrough
    case 1:
        field_0x244 = (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 2.0f);
        field_0x244 &= 1;
        field_0x230 = 2;
        // fallthrough
    case 2:
        if (fn_801F2618(0, false)) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            if (field_0x244 == 0) {
                field_0x2b2 -= 0x4000;
            } else {
                field_0x2b2 += 0x4000;
            }
            field_0x230 = 1;
        }
        if (field_0x238 != 0) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            if (field_0x238 == 1) {
                field_0x230 = 1;
            }
        }
        break;
    case 3:
        field_0x234 = 0;
        field_0x244 = 0;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x230 = 4;
        // fallthrough
    case 4:
        if (field_0x234 == 0) {
            if (field_0x244 > 3) {
                vfunc_0x24();
            } else if (field_0x270[field_0x244] >= 0) {
                ActorBase_804A5770* child = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x270[field_0x244]);
                if (child != 0) {
                    child->vfunc_0x24();
                    field_0x234 = 10;
                    field_0x244++;
                }
            }
        }
        break;
    }
}

// FIXME(shotgun): 96.95 %, missing/extra + reloc-name
void cls_804A2FB0::update() {
    switch (field_0x258) {
    case 0:
    case 5:
        fn_801A96B4();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        fn_801A9CF4();
        break;
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
}

// FIXME(shotgun): 99.00 %, regalloc-only
void cls_804A2FB0::fn_801A9374() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1d = 2;
    JGeometry::TVec3f rot(0.0f, field_0x2b4 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A2F2C[field_0x24c]);
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
}

// FIXME: 99.0 %. regalloc-only: the original keeps the angle (and then
// cos y) in f30 and cos x in f31, ours the other way round, and sin x / sin z
// get f29 / f27 there, f27 / f29 here.
void cls_804A2FB0::fn_801A90D4() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1d = 1;
    JGeometry::TVec3f rot(0.0f, field_0x2b4 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A2F2C[32]);
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
}

void cls_804A2FB0::draw() {
    switch (field_0x258) {
    case 0:
    case 5:
        fn_801A90D4();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        fn_801A9374();
        break;
    }
}
