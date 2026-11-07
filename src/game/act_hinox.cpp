#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_hinox.h"
#include "JSystem/JMath/JMath.h"

struct HinoxTables {
    s32 dropAmounts[8];
    s32 walkFrames[10];
    s32 walkTimers[5];
    s32 laughFrames[4];
    s32 throwFrames[22];
    s32 throwTimers[11];
    s32 chargeFrames[12];
    s32 chargeTimers[6];
    s32 grabFrames[10];
    s32 grabTimers[5];
};

// FIXME(shotgun): data table lbl_8049F840 not split into individual arrays
extern HinoxTables lbl_8049F840;

// FIXME(shotgun): 98.16 %, missing/extra + reloc-name.
void Hinox_8049F9B4::init() {
    mBox.i.x = -14.0f;
    mBox.i.y = -14.0f;
    mBox.f.x = 14.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x90 &= 1;
    field_0x264 = 0;
    field_0x268 = -1;
    if (field_0x90 != 1) {
        field_0xb0 = 0x9042012;
    }
    field_0xbc = 3;
    field_0xb8 = 1;
    field_0x98 = 2;
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.5f) {
        field_0x98 = 3;
    }
    field_0x26c = 0;
    field_0x278 = 0.0f;
    field_0x274 = 0.0f;
    field_0x270 = 0.0f;
    field_0x284 = 0.0f;
    field_0x280 = 0.0f;
    field_0x27c = 0.0f;
    field_0x26e = 0;
    field_0x250 = -1;
    field_0x254 = -1;
    mState = 0;
}

// FIXME(shotgun): 90.05 %, different + reloc-name.
Hinox_8049F9B4::~Hinox_8049F9B4() {
    JGeometry::TVec3f dir(0.0f, 0.0f, 0.0f);
    if (field_0x98 == 2) {
        dir.set(0.0f, 15.0f, 0.0f);
    } else {
        dir.set(0.0f, -15.0f, 0.0f);
    }
    if (field_0x254 >= 0) {
        f32 x = mPos.x;
        f32 y = mPos.y;
        f32 z = mPos.z;
        mPos.x = getPlayerPos_8023E724(field_0x254)->x;
        mPos.y = getPlayerPos_8023E724(field_0x254)->y;
        mPos.z = getPlayerPos_8023E724(field_0x254)->z;
        if (fn_801F2618(NULL, false)) {
            f32 angle = JMath::atanTable_.atan2_(x - mPos.x, y - mPos.y) * 10430.378f;
            dir.x = 3.0f * JMath::sincosTable_.sinShort((s16)(s32)angle);
            dir.y = 3.0f * JMath::sincosTable_.cosShort((s16)(s32)angle);
        }
        mPos.x = x;
        mPos.y = y;
        mPos.z = z;
        fn_8022F784(field_0x254, mSerial, &dir);
    }
    s32 serial = field_0x250;
    if (serial >= 0) {
        ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (held != NULL) {
            held->field_0x3c.set(0.0f, 0.0f, 3.0f);
            field_0x250 = -1;
            startSound_801F0E34(0x40008, 0);
        }
    }
}

// FIXME(shotgun): 88.19 %, missing/extra + reloc-name.
void Hinox_8049F9B4::fn_8014F76C() {
    s32 serial = field_0x250;
    if (serial >= 0) {
        ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (held != NULL) {
            held->field_0x3c.set(0.0f, 0.0f, 3.0f);
            field_0x250 = -1;
            startSound_801F0E34(0x40008, 0);
        }
    }
}

// FIXME(shotgun): 82.97 %, missing/extra + reloc-name
void Hinox_8049F9B4::update() {
    JGeometry::TVec3f pos = mPos;
    s32 playerDir = 0;
    f32 playerDist = 0.0f;

    if (field_0x90 == 1) {
        if (mPos.z > 0.0f) {
            if (field_0x3c.z == 0.0f) {
                return;
            }
            field_0x3c.z -= 1.0f;
            if (field_0x3c.z < -10.0f) {
                field_0x3c.z = -10.0f;
            }
            mPos += field_0x3c;
            return;
        }
        mPos.z = 0.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0xb0 = 0x9042012;
        field_0x90 = 0;
    }

    JGeometry::TVec3f savedVel = field_0x3c;
    if (fn_801F3D94(mLife <= 0) != 0 && mLife <= 0) {
        fn_801F2618(NULL, false);
        return;
    }

    fn_801EEC98(mSerial, NULL, &playerDir, &playerDist, &field_0x268, 0, 0);

    savedVel = field_0x3c;
    s32 hitSerial = field_0x9c;
    bool hitByActor = false;
    if (hitSerial >= 0) {
        ActorBase_804A5770* hitActor = getActorManager_801F856C()->findActorBySerial_801F666C(hitSerial);
        if (hitActor != NULL) {
            hitByActor = true;
            if (hitActor->mActorId == 'BOYO') {
                field_0x3c.x *= 0.5f;
                field_0x3c.y *= 0.5f;
            }
        }
    }

    if (fn_801F2DB4('HBMB', 'NULL', NULL) || fn_801F2FAC() || hitByActor == true) {
        field_0xbc = 3;
        if (mLife > 0) {
            field_0x3c = savedVel;
        }
        if (mState == 9) {
            JGeometry::TVec3f releaseDir(0.0f, 0.0f, 0.0f);
            if (field_0x98 == 2) {
                releaseDir.set(0.0f, 15.0f, 0.0f);
            } else {
                releaseDir.set(0.0f, -15.0f, 0.0f);
            }
            bool released = false;
            if (field_0x254 >= 0) {
                f32 x = mPos.x;
                f32 y = mPos.y;
                f32 z = mPos.z;
                mPos.x = getPlayerPos_8023E724(field_0x254)->x;
                mPos.y = getPlayerPos_8023E724(field_0x254)->y;
                mPos.z = getPlayerPos_8023E724(field_0x254)->z;
                if (fn_801F2618(NULL, false)) {
                    f32 angle = JMath::atanTable_.atan2_(x - mPos.x, y - mPos.y) * 10430.378f;
                    releaseDir.x = 3.0f * JMath::sincosTable_.sinShort((s16)(s32)angle);
                    releaseDir.y = 3.0f * JMath::sincosTable_.cosShort((s16)(s32)angle);
                }
                mPos.x = x;
                mPos.y = y;
                mPos.z = z;
                if (fn_8022F784(field_0x254, mSerial, &releaseDir)) {
                    released = true;
                }
            }
            if (released) {
                field_0x254 = -1;
                field_0x24c = 0;
                field_0x98 ^= 1;
                field_0x248 = 4;
                field_0x234 = 8;
                mState = 1;
            }
        }
        fn_801F285C(NULL, NULL, true, -1);
        if (mState == 7) {
            s32 serial = field_0x250;
            if (serial >= 0) {
                ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (held != NULL) {
                    held->field_0x3c.set(0.0f, 0.0f, 3.0f);
                    field_0x250 = -1;
                    startSound_801F0E34(0x40008, 0);
                }
            }
            if (field_0x258 == 0) {
                field_0x258 = 1;
                if (playerDir != field_0x98) {
                    field_0x98 ^= 1;
                    playerDir = field_0x98;
                    field_0x248 = 4;
                    field_0x234 = 8;
                }
            }
            field_0x24c = 0;
            mState = 1;
        } else if (mState == 1) {
            if (field_0x258 == 0) {
                field_0x258 = 1;
                if (playerDir != field_0x98) {
                    field_0x98 ^= 1;
                    playerDir = field_0x98;
                    field_0x248 = 4;
                    field_0x234 = 8;
                }
            }
        }
        field_0x270 = 0.0f;
        field_0x274 = 0.0f;
        field_0x278 = 0.0f;
    }

    addCalc2_801F71A4(&field_0x3c.x, field_0x270, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x274, 0.3f, 1.0f);
    mPos += field_0x3c;

    if (field_0x240 == 0 && field_0x268 >= 0 && playerDist < 24.0f &&
        fn_80237774(field_0x268) > 0 && fn_80236228(field_0x268) > 0 &&
        !fn_80236BEC(field_0x268) && mState == 5 && mState != 9) {
        field_0x254 = field_0x268;
        field_0xbc = 4;
        mState = 8;
    }

    if (mState != 8 && mState != 9) {
        if (mState == 5) {
            fn_801F2B7C(4, -1, 3);
        } else {
            fn_801F2B7C(field_0x114, -1, 1);
        }
    }

    u8 hitWall = fn_801F2618(NULL, false);

    switch (mState) {
    case 0:
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x258 = 0;
        mState = 1;
        // fallthrough
    case 1:
        if (hitWall) {
            field_0x98 ^= 1;
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x270 = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x248 = 4;
            field_0x234 = 8;
        }
        if (field_0x234 == 0) {
            field_0x248 = (field_0x248 + 1) & 3;
            startSound_801F0E34(0x400A4, 0);
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x270 = 0.0f;
            if ((field_0x248 & 1) == 0) {
                s16 moveAngle = (field_0x98 & 1) << 15;
                field_0x270 = 1.2f * JMath::sincosTable_.sinShort(moveAngle);
                if (field_0x248 == 0) {
                    field_0x24c++;
                    if (field_0x24c > 2) {
                        field_0x24c = 0;
                        moveAngle += (s16)(s32)(12288.0f - 24576.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
                    }
                }
                field_0x274 = 1.2f * JMath::sincosTable_.cosShort(moveAngle);
            }
            field_0x234 = lbl_8049F840.walkTimers[field_0x248];
        }
        field_0x264 = lbl_8049F840.walkFrames[(field_0x98 == 2 ? 0 : 5) + field_0x248];
        if (field_0x264 == 28 || field_0x264 == 29) {
            break;
        }
        if ((field_0x240 == 0 && playerDir == field_0x98 && playerDist < 128.0f) || field_0x258 != 0) {
            field_0x258 = 0;
            if (playerDist > 48.0f && gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.5f) {
                field_0x24c = 0;
                mState = 6;
            } else {
                mState = 2;
            }
        }
        break;

    case 2:
        field_0x234 = 5;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x278 = 0.0f;
        field_0x274 = 0.0f;
        field_0x270 = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        startSound_801F0E34(0x40092, 0);
        mState = 3;
        // fallthrough
    case 3:
        if (field_0x234 == 0) {
            field_0x234 = 5;
            field_0x248 = (field_0x248 + 1) & 1;
            if (field_0x248 == 0) {
                field_0x24c++;
                if (field_0x24c > 3) {
                    mState = 4;
                }
            }
        }
        field_0x264 = lbl_8049F840.laughFrames[(field_0x98 == 2 ? 0 : 2) + field_0x248];
        break;

    case 4:
        field_0x234 = 0;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x278 = 0.0f;
        field_0x274 = 0.0f;
        field_0x270 = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        fn_801EEC98(mSerial, &field_0x26c, &playerDir, &playerDist, NULL, field_0x268, 2);
        if (playerDir == -1) {
            hitWall = 1;
        } else {
            if (field_0x26c > 0x4000 && field_0x26c < 0xC000) {
                field_0x98 = 3;
            } else {
                field_0x98 = 2;
            }
            field_0x23c = 70;
            mState = 5;
        }
        // fallthrough
    case 5:
        if (field_0x23c == 0 || hitWall) {
            field_0x234 = 0;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.5f) {
                field_0x98 ^= 1;
                field_0x248 = 4;
                field_0x234 = 8;
            }
            field_0x24c = 0;
            field_0x240 = 100;
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x270 = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            mState = 1;
            break;
        }
        field_0x270 = 2.0f * JMath::sincosTable_.sinShort(field_0x26c);
        field_0x274 = 2.0f * JMath::sincosTable_.cosShort(field_0x26c);
        if (field_0x234 == 0) {
            field_0x248++;
            if (field_0x248 > 5) {
                startSound_801F0E34(0x40093, 0);
                field_0x248 = 0;
            }
            field_0x234 = lbl_8049F840.chargeTimers[field_0x248];
        }
        field_0x264 = lbl_8049F840.chargeFrames[(field_0x98 == 2 ? 0 : 6) + field_0x248];
        break;

    case 6:
        field_0x248 = 0;
        field_0x250 = -1;
        field_0x234 = lbl_8049F840.throwTimers[field_0x248];
        field_0x278 = 0.0f;
        field_0x274 = 0.0f;
        field_0x270 = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        fn_801EEC98(mSerial, &field_0x26c, &playerDir, &playerDist, &field_0x268, 0, 0);
        mState = 7;
        // fallthrough
    case 7:
        if (field_0x234 == 0) {
            if (field_0x248 == 0) {
                pos = mPos;
                if (field_0x98 == 2) {
                    pos.x += -8.0f;
                    pos.y += -21.0f;
                    pos.z += 25.0f;
                } else {
                    pos.x += 8.0f;
                    pos.y += -31.0f;
                    pos.z += 15.0f;
                }
                field_0x250 = getActorManager_801F856C()->spawnActor_801F9484('OIWA', field_0x4, &pos, 1, -1, -1);
            }
            field_0x248++;
            if (field_0x248 == 8) {
                ActorBase_804A5770* rock = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x250);
                if (rock != NULL) {
                    s16 throwAngle = fn_801F6874(field_0x98);
                    f32 speed = 2.0f + 2.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                    throwAngle += (s16)(s32)(8192.0f - 16384.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
                    JGeometry::TVec3f vel(speed * JMath::sincosTable_.sinShort(throwAngle),
                                          speed * JMath::sincosTable_.cosShort(throwAngle),
                                          3.0f);
                    rock->field_0x3c = vel;
                    startSound_801F0E34(0x40093, 0);
                    startSound_801F0E34(0x40008, 0);
                }
            }
            if (field_0x248 > 10) {
                field_0x240 = 100;
                field_0x24c++;
                if (field_0x24c < 3) {
                    mState = 6;
                } else {
                    field_0x98 ^= 1;
                    field_0x248 = 4;
                    field_0x250 = -1;
                    field_0x234 = 8;
                    mState = 1;
                }
                return;
            }
            field_0x234 = lbl_8049F840.throwTimers[field_0x248];
        }
        field_0x264 = lbl_8049F840.throwFrames[(field_0x98 == 2 ? 0 : 11) + field_0x248];
        break;

    case 8:
        if (field_0x254 >= 0 && !fn_8022F948(field_0x254, mSerial)) {
            field_0x248 = 0;
            field_0x240 = 100;
            field_0xbc = 3;
            mState = 0;
            break;
        }
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x25c = 0;
        field_0x260 = 0;
        field_0x234 = lbl_8049F840.grabTimers[field_0x248];
        field_0x238 = 0;
        field_0x284 = 0.0f;
        field_0x280 = 0.0f;
        field_0x27c = 0.0f;
        if (fn_801F6758(&mPos, field_0x4, -130.0f)) {
            s32 layer = field_0x4;
            GameView_804A9950* view = getGameView_802D6A9C();
            JGeometry::TBox2f box = (layer < 8) ? view->mVisibleBox : view->field_0xdc;
            pos.x = (box.i.x + box.f.x) * 0.5f;
            pos.y = (box.i.y + box.f.y) * 0.5f;
            field_0x26e = (s16)(s32)(JMath::atanTable_.atan2_(pos.x - mPos.x, pos.y - mPos.y) * 10430.378f);
            field_0x27c = 15.0f * JMath::sincosTable_.sinShort(field_0x26e);
            field_0x280 = 15.0f * JMath::sincosTable_.cosShort(field_0x26e);
            if (mPos.y < pos.y) {
                field_0x98 = 2;
            } else {
                field_0x98 = 3;
            }
        }
        field_0x278 = 0.0f;
        field_0x274 = 0.0f;
        field_0x270 = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        startSound_801F0E34(0x40094, 0);
        mState = 9;
        // fallthrough
    case 9:
        pos = mPos;
        if (field_0x98 == 2) {
            pos.x += -8.0f;
            pos.y += -21.0f;
            pos.z += 25.0f;
        } else {
            pos.x += 8.0f;
            pos.y += -31.0f;
            pos.z += 15.0f;
        }
        pos.x += 4.0f - (f32)(field_0x24c * 8);
        if (field_0x254 >= 0 && fn_8022C5EC(field_0x254, 0xF) != 0 && field_0x25c < 10) {
            field_0x25c++;
        }
        if (field_0x238 == 0) {
            field_0x238 = 12;
            field_0x24c = (field_0x24c + 1) & 1;
            if (field_0x25c < 10 && field_0x254 >= 0 && fn_80236228(field_0x254) > 0) {
                s16 dropAngle = (s16)fn_801F6874(field_0x98) - (s16)(0x1000 - (field_0x24c << 13));
                s32 dropIdx = (s32)(7.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
                dropAngle -= (s16)(s32)(1280.0f - 4096.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
                JGeometry::TVec3f gemVel(1.0f + 1.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                         1.0f + 1.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                         2.0f);
                for (s32 i = 0; i < 8; i++) {
                    if (field_0x254 >= 0 && fn_80236228(field_0x254) >= lbl_8049F840.dropAmounts[dropIdx]) {
                        s32 gemSerial = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &pos, dropIdx);
                        if (gemSerial >= 0) {
                            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(gemSerial);
                            if (gem != NULL) {
                                if (field_0x27c == 0.0f && field_0x280 == 0.0f) {
                                    gemVel.x *= JMath::sincosTable_.sinShort(dropAngle);
                                    gemVel.y *= JMath::sincosTable_.cosShort(dropAngle);
                                } else {
                                    gemVel.x *= JMath::sincosTable_.sinShort(field_0x26e);
                                    gemVel.y *= JMath::sincosTable_.cosShort(field_0x26e);
                                }
                                gem->field_0x3c = gemVel;
                            }
                            fn_802362C4(field_0x254, -lbl_8049F840.dropAmounts[dropIdx]);
                            break;
                        }
                    }
                    dropIdx = (dropIdx + 1) & 7;
                }
            }
        }
        if (field_0x254 >= 0) {
            if (fn_80236B50(field_0x254) || fn_80230188(field_0x254) ||
                fn_8022DCD8(field_0x254) || fn_8022F478(field_0x254) ||
                fn_80237774(field_0x254) <= 0 || fn_8022D534(field_0x254)) {
                JGeometry::TVec3f releaseDir(0.0f, 0.0f, 0.0f);
                if (field_0x98 == 2) {
                    releaseDir.set(0.0f, 15.0f, 0.0f);
                } else {
                    releaseDir.set(0.0f, -15.0f, 0.0f);
                }
                bool released = false;
                if (field_0x254 >= 0) {
                    f32 x = mPos.x;
                    f32 y = mPos.y;
                    f32 z = mPos.z;
                    mPos.x = getPlayerPos_8023E724(field_0x254)->x;
                    mPos.y = getPlayerPos_8023E724(field_0x254)->y;
                    mPos.z = getPlayerPos_8023E724(field_0x254)->z;
                    if (fn_801F2618(NULL, false)) {
                        f32 angle = JMath::atanTable_.atan2_(x - mPos.x, y - mPos.y) * 10430.378f;
                        releaseDir.x = 3.0f * JMath::sincosTable_.sinShort((s16)(s32)angle);
                        releaseDir.y = 3.0f * JMath::sincosTable_.cosShort((s16)(s32)angle);
                    }
                    mPos.x = x;
                    mPos.y = y;
                    mPos.z = z;
                    if (fn_8022F784(field_0x254, mSerial, &releaseDir)) {
                        released = true;
                    }
                }
                if (released) {
                    field_0x254 = -1;
                    field_0x234 = 0;
                    field_0x248 = 5;
                    field_0x25c = 11;
                }
            }
            if (field_0x254 >= 0 && !fn_8022F8AC(field_0x254, mSerial)) {
                field_0x254 = -1;
                field_0x234 = 0;
                field_0x248 = 5;
                field_0x25c = 11;
            }
        }
        if (field_0x234 == 0) {
            field_0x248++;
            if ((field_0x248 & 1) == 0 && field_0x25c < 10 && field_0x254 >= 0 && fn_80236228(field_0x254) > 0) {
                field_0x248 = 0;
            }
            if (field_0x248 > 2) {
                JGeometry::TVec3f checkDir;
                if (field_0x98 == 2) {
                    checkDir.set(0.0f, 15.0f, 0.0f);
                } else {
                    checkDir.set(0.0f, -15.0f, 0.0f);
                }
                if (field_0x254 >= 0) {
                    pos = mPos;
                    mPos.x = getPlayerPos_8023E724(field_0x254)->x;
                    mPos.y = getPlayerPos_8023E724(field_0x254)->y;
                    mPos.z = getPlayerPos_8023E724(field_0x254)->z;
                    if (fn_801F2618(NULL, false)) {
                        checkDir.set(0.0f, 0.0f, 0.0f);
                    }
                    mPos = pos;
                }
                JGeometry::TVec3f releaseDir(0.0f, 0.0f, 0.0f);
                if (field_0x98 == 2) {
                    releaseDir.set(0.0f, 15.0f, 0.0f);
                } else {
                    releaseDir.set(0.0f, -15.0f, 0.0f);
                }
                bool released = false;
                if (field_0x254 >= 0) {
                    f32 x = mPos.x;
                    f32 y = mPos.y;
                    f32 z = mPos.z;
                    mPos.x = getPlayerPos_8023E724(field_0x254)->x;
                    mPos.y = getPlayerPos_8023E724(field_0x254)->y;
                    mPos.z = getPlayerPos_8023E724(field_0x254)->z;
                    if (fn_801F2618(NULL, false)) {
                        f32 angle = JMath::atanTable_.atan2_(x - mPos.x, y - mPos.y) * 10430.378f;
                        releaseDir.x = 3.0f * JMath::sincosTable_.sinShort((s16)(s32)angle);
                        releaseDir.y = 3.0f * JMath::sincosTable_.cosShort((s16)(s32)angle);
                    }
                    mPos.x = x;
                    mPos.y = y;
                    mPos.z = z;
                    if (fn_8022F784(field_0x254, mSerial, &releaseDir)) {
                        released = true;
                    }
                }
                if (released) {
                    field_0x254 = -1;
                }
            }
            if (field_0x248 > 4) {
                field_0x248 = 4;
                if (field_0x254 >= 0) {
                    JGeometry::TVec3f releaseDir(0.0f, 0.0f, 0.0f);
                    if (field_0x98 == 2) {
                        releaseDir.set(0.0f, 15.0f, 0.0f);
                    } else {
                        releaseDir.set(0.0f, -15.0f, 0.0f);
                    }
                    bool released = false;
                    if (field_0x254 >= 0) {
                        f32 x = mPos.x;
                        f32 y = mPos.y;
                        f32 z = mPos.z;
                        mPos.x = getPlayerPos_8023E724(field_0x254)->x;
                        mPos.y = getPlayerPos_8023E724(field_0x254)->y;
                        mPos.z = getPlayerPos_8023E724(field_0x254)->z;
                        if (fn_801F2618(NULL, false)) {
                            f32 angle = JMath::atanTable_.atan2_(x - mPos.x, y - mPos.y) * 10430.378f;
                            releaseDir.x = 3.0f * JMath::sincosTable_.sinShort((s16)(s32)angle);
                            releaseDir.y = 3.0f * JMath::sincosTable_.cosShort((s16)(s32)angle);
                        }
                        mPos.x = x;
                        mPos.y = y;
                        mPos.z = z;
                        if (fn_8022F784(field_0x254, mSerial, &releaseDir)) {
                            released = true;
                        }
                    }
                    if (released) {
                        field_0x254 = -1;
                    }
                }
                if (field_0x254 < 0) {
                    field_0x24c = 0;
                    field_0x254 = -1;
                    field_0x98 ^= 1;
                    field_0x248 = 4;
                    field_0x234 = 8;
                    field_0xbc = 3;
                    mState = 1;
                    break;
                }
            }
            field_0x234 = lbl_8049F840.grabTimers[field_0x248];
        }
        if (field_0x248 <= 2 && field_0x254 >= 0) {
            fn_8022F820(field_0x254, mSerial, &pos);
        }
        field_0x264 = lbl_8049F840.grabFrames[(field_0x98 == 2 ? 0 : 5) + field_0x248];
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
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
}

void Hinox_8049F9B4::draw() {
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 0x2B, NULL, 7, 1.0f, 1.0f);
}
