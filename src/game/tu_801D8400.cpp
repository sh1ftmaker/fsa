#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801D8400.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/tu_801DAC6C.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

void fn_801CCCC4(JGeometry::TVec3f* pos);

// 0x801DA504. The sequencer's constructor runs before the ceiling is set,
// and the twelve TVec2f are built after it (the member initialisers run in
// declaration order); the body then zeroes the fields at 0x334-0x33F and the
// four emitters.
cls_804A4C58::cls_804A4C58() : mSeq2a8(), field_0x2d0(0.0f) {
    field_0x334 = 0;
    field_0x336 = 0;
    field_0x338 = 0;
    field_0x33a = 0;
    field_0x33c = 0;
    field_0x33e = 0;
    field_0x33f = 0;
    mEmitter2c[0] = NULL;
    mEmitter2c[1] = NULL;
    mEmitter2c[2] = NULL;
    mEmitter2c[3] = NULL;
}

// Only scheduling differs: ours loads -1 for the serial before the fn_801DAC6C call.
// FIXME: 95.98 %, scheduling+regalloc; behaviour clean.
cls_804A4C58::~cls_804A4C58() {
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitter2c[i]);
        mEmitter2c[i] = NULL;
    }
    fn_801DAC6C()->mSerial0 = -1;
    getGameView_802D6A9C()->fn_802D7440(true);
}

// FIXME(shotgun): 59.02 %, missing/extra + reloc-name; behaviour drift.
void cls_804A4C58::init() {
    EnemyBase_804A4808::init();
    mBox.i.x = 1.0f;
    mBox.i.y = 1.0f;
    mBox.f.x = -1.0f;
    mBox.f.y = -1.0f;
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.i.x = 1.0f;
    field_0x80.i.y = 1.0f;
    field_0x80.f.x = -1.0f;
    field_0x80.f.y = -1.0f;
    field_0xb0 = 0x40004;
    field_0x98 = 2;
    mFlags |= 0x2A0;
    mLife = 140;
    mLifeBackup = mLife;
    field_0x114 = 0;
    field_0x196 = 1;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = &view->mVisibleBox;
    if (field_0x4 >= 8) {
        box = &view->field_0xdc;
    }
    JGeometry::TVec3f p;
    p.x = mPos.x;
    p.y = mPos.y;
    while (p.y < box->f.y) {
        JGeometry::TVec3f probe(p.x, p.y, 0.0f);
        if (vfunc_0x4C(field_0x4, &probe)) {
            fn_801CCCC4(&p);
            p.y += -24.0f;
            break;
        }
        p.y += 16.0f;
    }
    field_0x2d0 = p.y;
    f32 x0 = mSpawnPos.x - 120.0f;
    f32 y0 = 32.0f + mSpawnPos.y;
    f32 step = 0.5f * (field_0x2d0 - 48.0f - y0);
    f32 y = y0;
    for (s32 row = 0; row < 3; row++) {
        f32 x = x0;
        for (s32 col = 0; col < 4; col++) {
            field_0x2d4[row * 4 + col].x = x;
            field_0x2d4[row * 4 + col].y = y;
            x += 80.0f;
        }
        y += step;
    }
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mSeq2a8.fn_801DA9E8(0, 2);
    fn_801E63D8();
    fn_801DAC6C()->fn_801DAF44(mSerial);
    getGameView_802D6A9C()->fn_802D7440(false);
}

// Vtable slot 0x54 (0x801DA0FC): EnemyBase's start hook; asks
// gAudioMgr_8053AF80 for fn_80458F9C(60).
void cls_804A4C58::vfunc_0x54() {
    gAudioMgr_8053AF80->fn_80458F9C(60);
}

// FIXME(shotgun): 97.94 %, missing/extra + reloc-name; behaviour drift.
void cls_804A4C58::fn_801D9C58(int state) {
    field_0x336 = 0;
    if (mState == 0x11) {
        mFlags &= ~0x80;
    }
    switch (state) {
    case 2:
        mSeq2a8.fn_801DA9E8(3, 0);
        startSound_801F0E34(0x40052, 0);
        break;
    case 3:
        mSeq2a8.fn_801DA9E8(4, 0);
        startSound_801F0E34(0x4005D, 0);
        break;
    case 4:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA994(0, 2);
        break;
    case 5:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        if (mState >= 6 && mState < 8) {
            mSeq2a8.fn_801DA9E8(2, 0);
        } else {
            mSeq2a8.fn_801DA9E8(1, 0);
        }
        startSound_801F0E34(0x40052, 0);
        break;
    case 6:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(3, 0);
        startSound_801F0E34(0x40052, 0);
        break;
    case 7:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(4, 0);
        startSound_801F0E34(0x4005D, 0);
        if (mState == 5) {
            mFlags |= 0x4000;
        } else {
            mFlags &= ~0x4000;
        }
        break;
    case 8:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(5, 0);
        startSound_801F0E34(0x40056, 0);
        fn_801E6394();
        fn_801E631C();
        break;
    case 9:
        mFlags &= ~0x280;
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(6, 0);
        startSound_801F0E34(0x40057, 0);
        fn_801E631C();
        break;
    case 10:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(7, 0);
        break;
    case 11:
        field_0x250 = -1;
        mTargetSpeed = 1.2f;
        mSeq2a8.fn_801DA994(8, 2);
        if (mState != 0xC) {
            field_0x33c = fn_801CD664(0xF1) + 0x78;
        }
        break;
    case 12:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(9, 0);
        break;
    case 13:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(0xE, 0);
        break;
    case 14:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(0xF, 0);
        break;
    case 15:
        field_0x33f = 0;
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(0xA, 0);
        break;
    case 16:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(0xB, 0);
        break;
    case 17:
        mFlags |= 0x80;
        field_0x250 = -1;
        field_0x336 = fn_801CD664(1) + 0xF0;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA994(0xC, 2);
        startSound_801F0E34(0x40057, 0);
        fn_801E631C();
        break;
    case 18:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA9E8(0xD, 0);
        break;
    case 19:
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mBox2 = mBox;
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0xd8 = 0x10;
        field_0xdc = 0x10;
        startSound_801F0E34(0x4005A, 0);
        break;
    case 20:
        fn_801DAC6C()->fn_801DAE24();
        break;
    }
    mStateTimer = 0;
    mState = state;
}

// FIXME(shotgun): 75.96 %, missing/extra + reloc-name; behaviour drift.
void cls_804A4C58::fn_801D9960() {
    if (field_0x33c != 0) {
        field_0x33c--;
    }
    if (field_0x33c == 0) {
        s32 target = -1;
        JGeometry::TVec3f from;
        if (mFlags & 0x2000) {
            from.x = mPos.x - -30.0f;
        } else {
            from.x = -30.0f + mPos.x;
        }
        from.y = -6.0f + mPos.y;
        from.z = 0.0f;
        s16 angle = -0x2000;
        if (mFlags & 0x2000) {
            angle = 0x2000;
        }
        s32 player = fn_801D0338(&from, angle);
        if (player >= 0) {
            f32 dy = getPlayerPos_8023E724(player)->y - from.y;
            f32 dx = getPlayerPos_8023E724(player)->x - from.x;
            s32 heading = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
            f32 diff = (f32)(s16)(heading - angle);
            if (0.000030517578f * std::fabsf(diff) <= 0.3f) {
                target = player;
            }
        }
        if (target >= 0) {
            if (mSeq2a8.fn_801DA894()) {
                fn_801D9C58(0xD);
            }
        }
    }
    if (mState == 0xB) {
        field_0x338--;
        if (field_0x338 == 0) {
            if (field_0x33f == 3) {
                s32 columns[6];
                columns[0] = 1;
                columns[1] = 2;
                columns[2] = 5;
                columns[3] = 6;
                columns[4] = 9;
                columns[5] = 10;
                field_0x334 = columns[fn_801CD664(6)];
            } else {
                field_0x334 = fn_801CD664(12);
            }
            field_0x338 = fn_801CD664(0x3D) + 0x3C;
            bool left = false;
            if (mPos.x < field_0x2d4[field_0x334].x ||
                (mPos.x == field_0x2d4[field_0x334].x && (mFlags & 0x2000))) {
                left = true;
            }
            bool turn = false;
            if (left) {
                if (!(mFlags & 0x2000)) {
                    turn = true;
                }
            } else {
                if (mFlags & 0x2000) {
                    turn = true;
                }
            }
            if (turn) {
                fn_801D9C58(0xC);
            }
        }
    }
}

// FIXME(shotgun): 71.73 %, missing/extra + reloc-name; behaviour drift.
bool cls_804A4C58::fn_801D95F4() {
    bool spawned = false;
    JGeometry::TVec3f pos;
    if (mFlags & 0x2000) {
        pos.x = mPos.x - -30.0f;
    } else {
        pos.x = -30.0f + mPos.x;
    }
    pos.y = -6.0f + mPos.y;
    pos.z = 0.0f;
    s32 serial = getActorManager_801F856C()->spawnActor_801F9484('BOIB', field_0x4, &pos, 0, -1, -1);
    if (serial >= 0) {
        ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (bomb != NULL) {
            s32 target = -1;
            JGeometry::TVec3f from;
            if (mFlags & 0x2000) {
                from.x = mPos.x - -30.0f;
            } else {
                from.x = -30.0f + mPos.x;
            }
            from.y = -6.0f + mPos.y;
            from.z = 0.0f;
            s16 angle = (mFlags & 0x2000) ? 0x2000 : -0x2000;
            s32 player = fn_801D0338(&from, angle);
            if (player >= 0) {
                f32 dy = getPlayerPos_8023E724(player)->y - from.y;
                f32 dx = getPlayerPos_8023E724(player)->x - from.x;
                s32 heading = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
                if (0.000030517578f * std::fabsf((f32)(s16)(heading - angle)) <= 0.3f) {
                    target = player;
                }
            }
            f32 vx;
            f32 vy;
            if (target >= 0) {
                f32 dy = getPlayerPos_8023E724(target)->y - pos.y;
                f32 dx = getPlayerPos_8023E724(target)->x - pos.x;
                JGeometry::TVec2f dir(dx, dy);
                if (dir.normalize() == 0.0f) {
                    dir.x = 1.0f;
                    dir.y = 0.0f;
                }
                vx = dir.x;
                vy = dir.y;
            } else {
                s16 facing = (mFlags & 0x2000) ? 0x2000 : -0x2000;
                f32 s = JMath::sincosTable_.sinShort(-facing);
                f32 c = JMath::sincosTable_.cosShort(-facing);
                vx = 0.0f * c - 1.0f * s;
                vy = 0.0f * s + 1.0f * c;
            }
            s16 jitter = fn_801CD664(0xE39) - 0x71C;
            f32 s = JMath::sincosTable_.sinShort(-jitter);
            f32 c = JMath::sincosTable_.cosShort(-jitter);
            JGeometry::TVec3f vel((vx * c - vy * s) * 2.4f, (vx * s + vy * c) * 2.4f, 0.0f);
            bomb->field_0x3c.set(vel);
            spawned = true;
        }
    }
    return spawned;
}

#ifdef NONMATCHING
// FIXME(shotgun): 89.28 %, missing/extra + reloc-name.
void cls_804A4C58::vfunc_0x90(u8 mapHit) {
    (void)mapHit;
    switch (mState) {
    case 0:
        if (fn_801CD228(field_0x4)) {
            s32 p = 0;
            for (; p < 4; p++) {
                if (std::fabsf(getPlayerPos_8023E724(p)->x - mPos.x) <= 80.0f &&
                    std::fabsf(getPlayerPos_8023E724(p)->y - mPos.y) <= 80.0f) {
                    break;
                }
            }
            if (p < 4 && getActorManager_801F856C()->fn_801F68E0() != 0) {
                fn_801D9C58(1);
            }
        }
        break;
    case 1:
        if (mStateTimer >= 180) {
            fn_801D9C58(2);
        }
        break;
    case 2:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801D9C58(3);
        }
        break;
    case 3:
        if (mSeq2a8.field_0x14 & 1) {
            getActorManager_801F856C()->fn_801F6884();
            gAudioMgr_8053AF80->fn_80458FF0(0x1000004, true, 0);
            field_0x33a = fn_801CD664(0x1E1) + 0x258;
            fn_801D9C58(4);
        } else if (mStateTimer == 6) {
            mFlags &= ~0x6000;
            if (!(mFlags & 0x4000) && !(fn_801DAC6C()->field_0x84 & 1)) {
                s32 n = 30 - fn_801DAC6C()->mCount;
                s32 count = 10;
                if (n < 10) {
                    count = n;
                }
                if (count > 0) {
                    JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
                    for (s32 i = 0; i < count; i++) {
                        getActorManager_801F856C()->spawnActor_801F9484('ICHL', field_0x4, &pos, 0, -1, -1);
                    }
                    startSound_801F0E34(0x40053, 0);
                }
            }
        }
        break;
    case 4:
        field_0x33a--;
        if (field_0x33a == 0) {
            field_0x33a = fn_801CD664(0x1E1) + 0x258;
            bool ok = false;
            if (fn_801D05F8(&mPos, 0.0f) >= 0 && fn_801DAC6C()->mCount < 30 && !(fn_801DAC6C()->field_0x84 & 1)) {
                ok = true;
            }
            if (ok) {
                fn_801D9C58(6);
            }
        }
        break;
    case 5:
    case 6:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801D9C58(7);
        }
        break;
    case 7:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801D9C58(4);
        } else if (mStateTimer == 6) {
            if (!(mFlags & 0x4000) && !(fn_801DAC6C()->field_0x84 & 1)) {
                s32 n = 30 - fn_801DAC6C()->mCount;
                s32 count = 10;
                if (n < 10) {
                    count = n;
                }
                if (count > 0) {
                    JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
                    for (s32 i = 0; i < count; i++) {
                        getActorManager_801F856C()->spawnActor_801F9484('ICHL', field_0x4, &pos, 0, -1, -1);
                    }
                    startSound_801F0E34(0x40053, 0);
                }
            }
        }
        break;
    case 8:
        if (mPos.y >= field_0x2d0) {
            mPos.y = field_0x2d0;
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 12.0f, 0.0f, false);
            gGameRoot_8053AAF8->fn_80134DE4(2, 0);
            fn_801D9C58(9);
        }
        break;
    case 9:
        if (mStateTimer >= 180) {
            if (field_0x33f == 3) {
                s32 columns[6];
                columns[0] = 1;
                columns[1] = 2;
                columns[2] = 5;
                columns[3] = 6;
                columns[4] = 9;
                columns[5] = 10;
                s32 n = 6;
                if (n > 0) {
                    field_0x334 = columns[fn_801CD664(6)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = fn_801CD664(12);
            }
            field_0x338 = fn_801CD664(0x3D) + 0x3C;
            bool left = false;
            if (mPos.x < field_0x2d4[field_0x334].x ||
                (mPos.x == field_0x2d4[field_0x334].x && (mFlags & 0x2000))) {
                left = true;
            }
            if (left) {
                mFlags |= 0x2000;
            } else {
                mFlags &= ~0x2000;
            }
            fn_801D9C58(10);
        }
        break;
    case 10:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801E6234();
            fn_801D9C58(11);
        }
        break;
    case 11:
        fn_801D9960();
        break;
    case 12:
        if (mSeq2a8.field_0x14 & 1) {
            mFlags ^= 0x2000;
            fn_801D9C58(11);
        }
        break;
    case 13:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801D9C58(14);
        }
        break;
    case 14:
        if (mSeq2a8.field_0x14 & 1) {
            s32 target = -1;
            JGeometry::TVec3f from;
            if (mFlags & 0x2000) {
                from.x = mPos.x - -30.0f;
                from.y = -6.0f + mPos.y;
                from.z = 0.0f;
            } else {
                from.x = -30.0f + mPos.x;
                from.y = -6.0f + mPos.y;
                from.z = 0.0f;
            }
            s16 angle = (mFlags & 0x2000) ? 0x2000 : -0x2000;
            s32 player = fn_801D0338(&from, angle);
            if (player >= 0) {
                f32 dy = getPlayerPos_8023E724(player)->y - from.y;
                f32 dx = getPlayerPos_8023E724(player)->x - from.x;
                s32 heading = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
                if (0.000030517578f * std::fabsf((f32)(s16)(angle - heading)) <= 0.3f) {
                    target = player;
                }
            }
            if (target >= 0) {
                if (field_0x33e < 2 ||
                    (field_0x33e < 3 && gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.6f)) {
                    fn_801D9C58(14);
                    break;
                }
            }
            if (field_0x33f == 3) {
                s32 columns[6];
                columns[0] = 1;
                columns[1] = 2;
                columns[2] = 5;
                columns[3] = 6;
                columns[4] = 9;
                columns[5] = 10;
                s32 n = 6;
                if (n > 0) {
                    field_0x334 = columns[fn_801CD664(6)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = fn_801CD664(12);
            }
            field_0x338 = fn_801CD664(0x3D) + 0x3C;
            bool left = false;
            if (mPos.x < field_0x2d4[field_0x334].x ||
                (mPos.x == field_0x2d4[field_0x334].x && (mFlags & 0x2000))) {
                left = true;
            }
            bool turn = false;
            if (left) {
                if (!(mFlags & 0x2000)) {
                    turn = true;
                }
            } else {
                if (mFlags & 0x2000) {
                    turn = true;
                }
            }
            if (turn) {
                fn_801D9C58(12);
            } else {
                fn_801D9C58(11);
            }
            field_0x33e = 0;
        } else if (mStateTimer == 10) {
            if (fn_801D95F4()) {
                field_0x33e++;
                startSound_801F0E34(0x30035, 0);
            }
        }
        break;
    case 15:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801D9C58(16);
        }
        break;
    case 16:
        startSound_801F0E34(0x40059, 0);
        if (mPos.y >= field_0x2d0) {
            mPos.y = field_0x2d0;
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 6.0f, 0.0f, false);
            gGameRoot_8053AAF8->fn_80134DE4(2, 0);
            fn_801D9C58(17);
        }
        break;
    case 17:
        if (field_0x336 == 0) {
            fn_801D9C58(18);
        }
        break;
    case 18:
        if (mSeq2a8.field_0x14 & 1) {
            if (field_0x33f == 3) {
                s32 columns[6];
                columns[0] = 1;
                columns[1] = 2;
                columns[2] = 5;
                columns[3] = 6;
                columns[4] = 9;
                columns[5] = 10;
                s32 n = 6;
                if (n > 0) {
                    field_0x334 = columns[fn_801CD664(6)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = fn_801CD664(12);
            }
            field_0x338 = fn_801CD664(0x3D) + 0x3C;
            bool left = false;
            if (mPos.x < field_0x2d4[field_0x334].x ||
                (mPos.x == field_0x2d4[field_0x334].x && (mFlags & 0x2000))) {
                left = true;
            }
            bool turn = false;
            if (left) {
                if (!(mFlags & 0x2000)) {
                    turn = true;
                }
            } else {
                if (mFlags & 0x2000) {
                    turn = true;
                }
            }
            if (turn) {
                fn_801D9C58(12);
            } else {
                fn_801D9C58(11);
            }
        }
        break;
    case 19:
        if (getActorManager_801F856C()->fn_801F68E0() != 0) {
            releaseEmitter_801EE434(mEmitter2c[2]);
            releaseEmitter_801EE434(mEmitter2c[3]);
            mEmitter2c[2] = NULL;
            mEmitter2c[3] = NULL;
            vfunc_0x28(0, 0.0f, -16.0f);
            fn_801D9C58(20);
        }
        break;
    case 20:
        break;
    }
}
#endif

void cls_804A4C58::vfunc_0x94() {
    bool play = false;
    s32 before = mSeq2a8.fn_801DA96C();
    mSeq2a8.fn_801DA7C0();
    switch (mSeq2a8.field_0x0) {
    case 8:
        if (before != 0x1E && mSeq2a8.fn_801DA96C() == 0x1E) {
            play = true;
        }
        break;
    case 14:
        if (before != 0x2C && mSeq2a8.fn_801DA96C() == 0x2C) {
            play = true;
        }
        break;
    case 15:
        if (before != 0x2F && mSeq2a8.fn_801DA96C() == 0x2F) {
            play = true;
        }
        break;
    }
    if (play) {
        startSound_801F0E34(0x40058, 0);
    }
    fn_801DE12C();
}

// Vtable slot 0x9C (0x801D89BC). Any hit bit in mFlags skips the step
// (vfunc_0x94 only in state 0xF); otherwise EnemyBase's state step runs.
// Then the countdown at 0x336 and the effect pair at 0x2C8.
void cls_804A4C58::vfunc_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        fn_801CE2BC(mapHit);
    } else if (mState == 0xF) {
        vfunc_0x94();
    }
    if (field_0x336 != 0) {
        field_0x336--;
    }
    fn_801E61C4();
}

// Vtable slot 0x80 (0x801D8980). A position above the ceiling in
// field_0x2d0 is clamped to it, its vertical speed is zeroed, and then
// EnemyBase's floor check runs.
void cls_804A4C58::vfunc_0x80() {
    if (mPos.y > field_0x2d0) {
        mPos.y = field_0x2d0;
        field_0x3c.y = 0.0f;
    }
    EnemyBase_804A4808::vfunc_0x80();
}

// FIXME: 99.65 %, regalloc-only; behaviour clean.
void cls_804A4C58::vfunc_0x88() {
    switch (mState) {
    case 0xB: case 0xC: case 0xD: case 0xE: {
        JGeometry::TVec2f dir(field_0x2d4[field_0x334].x - mPos.x, field_0x2d4[field_0x334].y - mPos.y);
        dir.normalize();
        dir.scale(mSpeed);
        addCalc2_801F71A4(&field_0x3c.x, dir.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, dir.y, 0.3f, 1.0f);
        break;
    }
    default:
    {
        field_0x3c.x = 0.0f;
        switch (mState) {
        case 0x10:
        case 8:
            field_0x3c.y += __fabsf(lbl_8053EFB8);
            break;
        default:
            field_0x3c.y = 0.0f;
            break;
        }
    }
    }
    field_0x3c.z = 0.0f;
}


// FIXME: 77.38 %, missing/extra; behaviour drift.
void cls_804A4C58::vfunc_0x74() {
    if (mState < 0xF && mState >= 0xB) {
        if (mState == 0xC) {
            if (field_0x33f == 3) {
                s32 columns[6];
                columns[0] = 1;
                columns[1] = 2;
                columns[2] = 5;
                columns[3] = 6;
                columns[4] = 9;
                columns[5] = 10;
                s32 n = 6;
                if (n > 0) {
                    field_0x334 = columns[fn_801CD664(6)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = fn_801CD664(12);
            }
            field_0x338 = fn_801CD664(0x3D) + 0x3C;
            bool left = false;
            if (mPos.x < field_0x2d4[field_0x334].x ||
                (mPos.x == field_0x2d4[field_0x334].x && (mFlags & 0x2000))) {
                left = true;
            }
            if (left) {
                mFlags &= ~0x2000;
            } else {
                mFlags |= 0x2000;
            }
        } else {
            if (field_0x33f == 3) {
                s32 columns[6];
                columns[0] = 1;
                columns[1] = 2;
                columns[2] = 5;
                columns[3] = 6;
                columns[4] = 9;
                columns[5] = 10;
                s32 n = 6;
                if (n > 0) {
                    field_0x334 = columns[fn_801CD664(6)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = fn_801CD664(12);
            }
            field_0x338 = fn_801CD664(0x3D) + 0x3C;
            bool left = false;
            if (mPos.x < field_0x2d4[field_0x334].x ||
                (mPos.x == field_0x2d4[field_0x334].x && (mFlags & 0x2000))) {
                left = true;
            }
            bool turn = false;
            if (left) {
                if (!(mFlags & 0x2000)) {
                    turn = true;
                }
            } else {
                if (mFlags & 0x2000) {
                    turn = true;
                }
            }
            if (turn) {
                fn_801D9C58(0xC);
            } else {
                fn_801D9C58(0xB);
            }
        }
    }
}

// Vtable slot 0xA0 (0x801D84E4), the last slot of the vtable.
void cls_804A4C58::vfunc_0xA0() {
    if (mState == 0) {
        return;
    }
    if (mState >= 0 && mState < 4) {
        cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
        if (!obj->field_0x284) {
            obj->field_0x284 = 1;
            obj->field_0x28c = 0.05f;
        }
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (!camera->field_0x7f5) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.set(mPos);
            camera->field_0x7f6 = 0;
        }
    } else {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (!gGameRoot2_8053AB10->field_0x24->field_0x179) {
            gGameRoot2_8053AB10->field_0x24->field_0x178 = 1;
        }
    }
}

void cls_804A4C58::vfunc_0x24() {}

// FIXME: 95.11 %, scheduling+regalloc; behaviour clean.
void cls_804A4C58::update() {
    if (vfunc_0x2C(0.0f, -16.0f, 50.0f)) {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x840 == 0) {
            camera->field_0x840 = 1;
            camera->field_0x844.set(mPos.x, mPos.y, mPos.x, mPos.y);
            camera->field_0x854 = 1.0f;
            camera->field_0x1d4 = -1.0f;
            camera->field_0x1d8 = -1.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
    } else {
        EnemyBase_804A4808::update();
    }
}

void cls_804A4C58::fn_801D8400() {
    if (mState < 8) {
        fn_801D9C58(8);
    }
}
