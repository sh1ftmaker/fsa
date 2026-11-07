#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_life_meter.h"
#include "game/act/act_princess_zelda_motion_spawner.h"
#include "game/act/tu_801F9484.h"
#include "std/math.h"

// The 'ZLD4' actor, cls_804A3698.

// FIXME(shotgun): table not written out (gate.py literal tripwire); extern-declared, size
// from symbols.txt (0x38). update indexes it.
struct Tables_804A3660 {
    /* 0x00 */ s32 mDirs[4];   // the turn order of states 0xD and 0xE
    /* 0x10 */ s32 mFrames[5]; // states 0xF-0x13: field_0x320 by field_0x30c
    /* 0x24 */ s32 mTimes[5];  // and field_0x300
}; // Size: 0x38
extern Tables_804A3660 lbl_804A3660;

// 0x801B3CD0.
// FIXME(shotgun): 100 %, matches
cls_804A3698::~cls_804A3698() {
    lbl_8053ABE8 = 0;
    lbl_8053ABF0 = 0;
}

// 0x801B3B7C.
// FIXME(shotgun): 97.36 %, different + reloc-name
void cls_804A3698::init() {
    lbl_8053ABF0 = 0;
    field_0x80.set(-4.0f, -8.0f, 4.0f, 8.0f);
    field_0x30c = 0;
    field_0x310 = 0;
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x31c = 0;
    field_0x300 = 0;
    field_0x304 = 0;
    field_0x308 = 0;
    field_0x330 = field_0x90 & 7;
    field_0x328 = 5;
    field_0x324 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LIMT, field_0x4, &mPos, 0, -1, -1);
    field_0x11e = 1;
    fn_8022ADE4();
    s32 serial = field_0x324;
    if (serial >= 0) {
        LifeMeter_804B174C* meter =
            static_cast<LifeMeter_804B174C*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (meter != NULL) {
            meter->field_0x250 = mSerial;
            meter->field_0x254 = -28.0f;
            meter->field_0x24c = field_0x328;
        }
    }
    lbl_8053AED0->fn_8043A450(6, 3);
    field_0x320 = 1;
    field_0x334 = field_0x338 = field_0x33c = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x340 = mPos.z;
    lbl_8053ABEC = 0;
    field_0x2fc = 0;
    field_0x32d = 0;
    field_0x32c = 0;
    field_0x1a8 = false;
    lbl_8053ABE8 = 0;
    field_0x2f8 = 0;
}

// 0x801B29EC.
// FIXME(shotgun): 89.37 %, different + reloc-name
void cls_804A3698::update() {
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* src;
    if (layer < 8) {
        src = &view->mVisibleBox;
    } else {
        src = &view->field_0xdc;
    }
    JGeometry::TBox2f box = *src;
    JGeometry::TVec3f step(0.0f, 0.0f, 0.0f);
    s32 railLayer = field_0x4;
    getGameView_802D6A9C()->fn_802D79E8(field_0x4, 1.0f, 0.0f, false);
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            getGameView_802D6A9C()->fn_802D79A4(i, 2.0f, 1.0f, 1.0f);
        }
    }
    if (field_0x2f8 != 0) {
        JGeometry::TVec3f pos = mPos;
        u16 angle;
        mRail.fn_800FDEA0(&pos, 20.0f, &step, &railLayer, &angle, 0x200);
    }
    fn_801F3D94(1);
    addCalc2_801F71A4(&field_0x3c.x, field_0x334, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x338, 0.3f, 1.0f);
    mPos += field_0x3c;
    s32 dir;
    f32 dist;
    fn_801EEC98(mSerial, NULL, &dir, &dist, NULL, 0, 0);
    JGeometry::TVec3f wall;
    wall.x = wall.y = wall.z = 0.0f;
    if (field_0x2f8 != 9) {
        fn_801F2618(&wall, false);
    }
    mLife = 10000;
    if (field_0x308 == 0 || field_0x2f8 == 7) {
        if (field_0x2f8 == 2 || field_0x2f8 == 6 || field_0x2f8 == 7) {
            if (field_0x314 == 0) {
                s32 count = getActorManager_801F856C()->mActorEnd;
                field_0x32c = 0;
                for (s32 i = 0; i < count; i++) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                    if (actor == NULL || actor->field_0x1a8 != 1) {
                        continue;
                    }
                    mBox.set(-8.0f, -14.0f, 8.0f, 10.0f);
                    if (field_0x308 == 0 && fn_801F2718(actor->mActorId) >= 0) {
                        field_0x334 = 0.0f;
                        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                        field_0x2f8 = 0xB;
                        break;
                    }
                    if (field_0x2f8 == 7) {
                        mBox.set(-256.0f, -48.0f, 256.0f, 48.0f);
                        if (fn_801F2718(actor->mActorId) >= 0) {
                            field_0x32c = 1;
                            break;
                        }
                        continue;
                    }
                    if (actor->mActorId == ACTOR_KETH) {
                        continue;
                    }
                    mBox.set(-64.0f, -48.0f, 64.0f, 48.0f);
                    if (fn_801F2718(actor->mActorId) < 0) {
                        continue;
                    }
                    field_0x304 = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 20;
                    if (field_0x2f8 != 6) {
                        startSound_801F0E34(0x400B5, 0);
                        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
                        pos.y = mPos.y - 16.0f;
                        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
                    }
                    dir = field_0x98;
                    field_0x334 = 0.0f;
                    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                    field_0x32d = 1;
                    field_0x2f8 = 6;
                    break;
                }
            }
            field_0x314++;
            field_0x314 &= 3;
        }
    }
    mBox.set(-8.0f, -14.0f, 8.0f, 10.0f);
    if (field_0x324 >= 0) {
        field_0x328 += lbl_8053ABEC / 4;
        lbl_8053ABEC = 0;
        if (field_0x328 > 5) {
            field_0x328 = 5;
        }
        LifeMeter_804B174C* meter =
            static_cast<LifeMeter_804B174C*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x324));
        if (meter != NULL) {
            meter->field_0x250 = mSerial;
            meter->field_0x254 = -28.0f;
            meter->field_0x24c = field_0x328;
        }
    }
    PrincessZeldaMotionObjectSpawner_804A375C* obj;
    switch (field_0x2f8) {
    case 0:
        if (!mRail.fn_800FE3EC(field_0x330, mPos, field_0x4)) {
            break;
        }
    case 1:
        field_0x310 = -1;
        field_0x2fc = 0;
        field_0x32d = 0;
        field_0x340 = mPos.y;
        field_0x31c = 0;
        field_0x2f8 = 2;
    case 2: {
        s32 serial = fn_801F2718(ACTOR_ZDCO);
        if (serial >= 0) {
            obj = static_cast<PrincessZeldaMotionObjectSpawner_804A375C*>(
                getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (obj != NULL && obj->field_0x94 == 0) {
                obj->field_0x25c = 1;
                JGeometry::TVec3f pos = mPos;
                obj->mHomePos.set(pos.x, pos.y, pos.z);
                field_0x310 = serial;
                switch (obj->mKind) {
                case 4:
                    field_0x2f8 = 3;
                    break;
                case 6:
                    field_0x318 = 0;
                    field_0x2f8 = 8;
                    break;
                case 2:
                    field_0x334 = 0.0f;
                    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                    field_0x320 = field_0x98 << 3;
                    field_0x304 = 100;
                    field_0x2f8 = 7;
                    break;
                default:
                    if (dir >= 0) {
                        field_0x98 = dir;
                        field_0x320 = field_0x98 << 3;
                        field_0x300 = 5;
                    }
                    field_0x2f8 = 5;
                    break;
                }
                break;
            }
        }
        if (wall.y == 0.0f) {
            if (field_0x308 == 0 && field_0x340 < mPos.y - 16.0f) {
                field_0x3c.x = 0.0f;
                field_0x334 = 0.0f;
                field_0x2fc = 1;
            }
        } else {
            field_0x340 = mPos.y;
            if (field_0x2fc == 1) {
                field_0x2f8 = 0xB;
                break;
            }
        }
        field_0x338 = 3.0f;
        field_0x334 = step.x;
        if (step.x != 0.0f) {
            if (fabs(mPos.x - mPrevPos.x) <= 0.7f) {
                field_0x31c++;
                if (field_0x31c > 10) {
                    field_0x2f8 = 0;
                }
            } else {
                field_0x31c = 0;
            }
        }
        field_0x98 = 1;
        if (field_0x334 > 0.0f) {
            field_0x98 = 0;
        }
        break;
    }
    case 3:
        field_0x334 = field_0x338 = field_0x33c = 0.0f;
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        field_0x304 = 0;
        field_0x2f8 = 4;
    case 4: {
        s32 serial = field_0x310;
        if (serial < 0) {
            break;
        }
        obj = static_cast<PrincessZeldaMotionObjectSpawner_804A375C*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (obj == NULL) {
            field_0x2f8 = 0;
            break;
        }
        JGeometry::TVec3f target = obj->mPos;
        target.x = obj->mPos.x + 8.0f;
        if (fabs(mPos.x - target.x) > 0.1f) {
            addCalc2_801F71A4(&mPos.x, target.x, 0.2f, 0.6f);
            field_0x98 = 0;
            if (mPos.x > target.x) {
                field_0x98 = 1;
            }
            field_0x304 = 20;
            break;
        }
        mPos.x = target.x;
        field_0x98 = 3;
        field_0x338 = 0.5f;
        if (field_0x304 == 0 && wall.y < 0.0f) {
            obj->mAlive = false;
            field_0x2f8 = 0;
        }
        break;
    }
    case 5:
        field_0x334 = 0.0f;
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        field_0x32d = 0;
        field_0x2f8 = 6;
    case 6:
        if (field_0x32d == 0 && dir >= 0) {
            field_0x98 = dir;
            field_0x320 = field_0x98 << 3;
        }
        if (wall.y == 0.0f) {
            if (field_0x308 == 0 && field_0x340 < mPos.y - 16.0f) {
                field_0x3c.x = 0.0f;
                field_0x334 = 0.0f;
                field_0x2fc = 1;
            }
        } else {
            field_0x340 = mPos.y;
            if (field_0x2fc == 1) {
                field_0x2f8 = 0xB;
                break;
            }
        }
        field_0x300 = 5;
        if (field_0x32d == 1) {
            if (dir >= 0 && field_0x304 == 0) {
                field_0x2f8 = 1;
            }
            break;
        }
        {
            s32 serial = field_0x310;
            if (serial >= 0) {
                obj = static_cast<PrincessZeldaMotionObjectSpawner_804A375C*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (obj != NULL && obj->field_0x94 == 0) {
                    break;
                }
                field_0x2f8 = 1;
                break;
            }
        }
        if (dir >= 0 && dist < 24.0f && field_0x304 == 0) {
            field_0x2f8 = 1;
        }
        break;
    case 7:
        if (field_0x304 == 0 && field_0x32c != 1) {
            s32 serial = field_0x310;
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->mAlive = false;
                }
            }
            gAudioMgr_8053AF80->fn_80458FF0(0x1000022, false, 0);
            field_0x310 = -1;
            field_0x2f8 = 1;
        }
        break;
    case 8: {
        field_0x334 = field_0x338 = field_0x33c = 0.0f;
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        if (dir >= 0) {
            field_0x98 = dir;
            field_0x320 = field_0x98 << 3;
        }
        s32 near = 0;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i) && fn_8022EA0C(i) == i &&
                !fn_80230188(i) && !fn_8022DCD8(i) && !fn_80236B50(i) && !fn_8022D534(i)) {
                f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
                f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
                if (std::sqrtf(dx * dx + dy * dy) < 128.0f) {
                    near++;
                }
            }
        }
        if (near < gGameRoot_8053AAF8->fn_80138A30() || dir < 0 || dist > 32.0f) {
            break;
        }
        field_0x2f8 = 9;
    }
    case 9: {
        s32 serial = field_0x310;
        if (serial < 0) {
            break;
        }
        obj = static_cast<PrincessZeldaMotionObjectSpawner_804A375C*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (obj == NULL) {
            break;
        }
        JGeometry::TVec3f target = obj->mPos;
        target.x = obj->mPos.x + 8.0f;
        if (fabs(mPos.x - target.x) > 0.1f) {
            addCalc2_801F71A4(&mPos.x, target.x, 0.2f, 0.6f);
            field_0x98 = 0;
            if (mPos.x > target.x) {
                field_0x98 = 1;
            }
            field_0x304 = 100;
            break;
        }
        mPos.x = target.x;
        field_0x98 = 3;
        field_0x338 = 1.0f;
        if (field_0x304 == 0 && mPos.y > 16.0f + box.f.y) {
            obj->mAlive = false;
            fn_8022ADCC();
            field_0x334 = field_0x338 = field_0x33c = 0.0f;
            field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
            field_0x2f8 = 0xA;
        }
        break;
    }
    case 0xA:
        if (getGameView_802D6A9C()->fn_802DCD0C()) {
            fn_8022ADE4();
            field_0x2f8 = 0;
        }
        break;
    case 0xB:
        field_0x334 = 0.0f;
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        startSound_801F0E34(0x400B6, 0);
        field_0x3c.z = 5.0f;
        field_0x2f8 = 0xC;
        break;
    case 0xC:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
            break;
        }
        mPos.z = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x328--;
        if (field_0x328 > 0) {
            field_0x308 = 300;
            s32 serial = field_0x310;
            if (serial >= 0) {
                obj = static_cast<PrincessZeldaMotionObjectSpawner_804A375C*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (obj != NULL && obj->mKind == 2) {
                    field_0x32c = 1;
                    field_0x2f8 = 7;
                    break;
                }
            }
            if (field_0x2fc == 1) {
                field_0x2f8 = 0;
            } else {
                field_0x2f8 = 1;
            }
        } else {
            field_0x2f8 = 0xD;
        }
        break;
    case 0xD:
        field_0x30c = 0;
        field_0x310 = 0;
        field_0x314 = 0;
        field_0x318 = 0;
        field_0x31c = 0;
        field_0x300 = 0;
        field_0x304 = 0;
        field_0x308 = 0;
        startSound_801F0E34(0x400B7, 0);
        for (s32 i = 0; i < 4; i++) {
            if (field_0x98 == lbl_804A3660.mDirs[i]) {
                field_0x30c = i;
                field_0x300 = 8;
                field_0x2f8 = 0xE;
                break;
            }
        }
        break;
    case 0xE:
        if (field_0x300 == 0) {
            field_0x30c++;
            field_0x30c &= 3;
            field_0x300 = 8;
            field_0x98 = lbl_804A3660.mDirs[field_0x30c];
            field_0x310++;
            if (field_0x310 >= 12 && field_0x98 == 0) {
                field_0x2f8 = 0xF;
                break;
            }
        }
        field_0x320 = field_0x98 << 3;
        break;
    case 0xF:
        field_0x30c = 0;
        field_0x300 = lbl_804A3660.mTimes[field_0x30c];
        field_0x320 = lbl_804A3660.mFrames[field_0x30c];
        field_0x2f8 = 0x10;
    case 0x10:
        if (field_0x300 == 0) {
            field_0x30c++;
            if (field_0x30c > 4) {
                field_0x30c = 0;
                lbl_8053ABF0 = 1;
                field_0x2f8 = 0x11;
            } else {
                field_0x300 = lbl_804A3660.mTimes[field_0x30c];
                field_0x320 = lbl_804A3660.mFrames[field_0x30c];
            }
        }
        break;
    case 0x11:
        if (lbl_8053ABE8 == 1) {
            lbl_8053ABE8 = 0;
            lbl_8053ABF0 = 0;
            field_0x2f8 = 0x12;
        }
        break;
    case 0x12:
        field_0x30c = 4;
        field_0x300 = lbl_804A3660.mTimes[field_0x30c];
        field_0x320 = lbl_804A3660.mFrames[field_0x30c];
        field_0x2f8 = 0x13;
    case 0x13:
        if (field_0x300 == 0) {
            field_0x328++;
            if (field_0x328 > 5) {
                field_0x328 = 5;
            }
            field_0x30c--;
            if (field_0x30c < 0) {
                field_0x328 = 5;
                field_0x2f8 = 1;
            } else {
                field_0x300 = lbl_804A3660.mTimes[field_0x30c];
                field_0x320 = lbl_804A3660.mFrames[field_0x30c];
            }
        }
        break;
    }
    s32 state = field_0x2f8;
    if ((state == 2 || state == 4 || state == 6 || state == 0xC || state == 9 || state == 0xA) &&
        field_0x300 == 0) {
        field_0x320 = (field_0x98 << 3) + field_0x30c;
        field_0x300 = 5;
        field_0x30c++;
        field_0x30c &= 7;
    }
    if (field_0x300 > 0) {
        field_0x300--;
    }
    if (field_0x304 > 0) {
        field_0x304--;
    }
    if (field_0x308 > 0) {
        field_0x308--;
    }
}

// 0x801B2998.
// FIXME(shotgun): 100 %, matches
void cls_804A3698::draw() {
    if (field_0x308 == 0 || (field_0x308 & 2)) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x320 + 0x9B, NULL, 7, 1.0f, 1.0f);
    }
}

// 0x801B27DC.
// FIXME(shotgun): 73.73 %, missing/extra
bool cls_804A3698::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u16 code = fn_8022461C(pos, layer);
    u16 tile = fn_80226850(pos, layer);
    if (tile == 0x230 || tile == 0x231 || tile == 0x234) {
        return false;
    }
    if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 && code != 3 &&
        code != 0x50 && code != 0xE && code != 0xC && code != 0x27 && code != 0x28 && code != 0x29 &&
        code != 0x2A && code != 0x2B && code != 0x2C && code != 0x2D && code != 0x2E && code != 0x2F &&
        code != 0x20 && code != 0xD && code != 0x39 && code != 0x3A && code != 0x3B && code != 0x4C &&
        code != 0x4D && code != 0x4E && code != 0x4F && code != 0x24) {
        if (code == 0x25) {
            return (u8)!getGameView_802D6A9C()->field_0x171;
        }
        if (code == 0x26) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
        }
        if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
            return true;
        }
    }
    return false;
}
