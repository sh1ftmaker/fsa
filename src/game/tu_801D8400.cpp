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
#include "game/act/singleton_801DAC6C.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// Hits before the knock-back state, read by tu_801DD1AC; the first object of
// this unit's .sdata2 (0x8053F1C8).
extern const u8 lbl_8053F1C8 = 4;

static inline bool isColumnRight(Frostare_804A4C58* self) {
    bool right = false;
    if (self->mPos.x < self->field_0x2d4[self->field_0x334].x ||
        (self->mPos.x == self->field_0x2d4[self->field_0x334].x && (self->mFlags & 0x2000))) {
        right = true;
    }
    return right;
}

static inline bool needsTurn(Frostare_804A4C58* self) {
    bool turn = false;
    if (isColumnRight(self)) {
        if (!(self->mFlags & 0x2000)) {
            turn = true;
        }
    } else {
        if (self->mFlags & 0x2000) {
            turn = true;
        }
    }
    return turn;
}

// 0x801DA504. The sequencer's constructor runs before the ceiling is set,
// and the twelve TVec2f are built after it (the member initialisers run in
// declaration order); the body then zeroes the fields at 0x334-0x33F and the
// four emitters.
Frostare_804A4C58::Frostare_804A4C58() : mSeq2a8(), field_0x2d0(0.0f) {
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

Frostare_804A4C58::~Frostare_804A4C58() {
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitter2c[i]);
        mEmitter2c[i] = NULL;
    }
    Singleton_801DAC6C* frostare = fn_801DAC6C();
    frostare->mSerial0 = -1;
    getGameView_802D6A9C()->fn_802D7440(true);
}

// FIXME(shotgun): 99.21 %, regalloc-only; behaviour clean.
void Frostare_804A4C58::init() {
    EnemyBase_804A4808::init();
    mBox.i.x = 1.0f;
    mBox.i.y = 1.0f;
    mBox.f.x = -1.0f;
    mBox.f.y = -1.0f;
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    mMapBox_0x80.i.x = 1.0f;
    mMapBox_0x80.i.y = 1.0f;
    mMapBox_0x80.f.x = -1.0f;
    mMapBox_0x80.f.y = -1.0f;
    field_0xb0 = 0x40004;
    mDir_0x98 = 2;
    mFlags |= 0x2A0;
    mLife = 140;
    mLifeBackup = mLife;
    mParamAtcp_0x114 = 0;
    field_0x196 = 1;
    s32 room = mLayer_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box;
    if (room < 8) {
        box = &view->mVisibleBox;
    } else {
        box = &view->field_0xdc;
    }
    JGeometry::TVec2f p(mPos.x, mPos.y);
    while (p.y < box->f.y) {
        JGeometry::TVec3f probe(p.x, p.y, 0.0f);
        if (isBlocked_0x4C(mLayer_0x4, &probe)) {
            // p is 8 bytes on the original's stack; fn_801CCCC4 only touches x and y.
            fn_801CCCC4((JGeometry::TVec3f*)&p);
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
    // Still differs: f4 and f6 are swapped in this loop.
    for (s32 row = 0; row < 3; row++, y += step) {
        f32 x = x0;
        for (s32 col = 0; col < 4; col++, x += 80.0f) {
            JGeometry::TVec2f& point = field_0x2d4[row * 4 + col];
            point.set(x, y);
        }
    }
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mSeq2a8.start_801DA9E8(0, 2);
    fn_801E63D8();
    fn_801DAC6C()->fn_801DAF44(mSerial);
    getGameView_802D6A9C()->fn_802D7440(false);
}

// Vtable slot 0x54 (0x801DA0FC): EnemyBase's start hook; asks
// gAudioMgr_8053AF80 for stopBgm_80458F9C(60).
void Frostare_804A4C58::onFirstUpdate_0x54() {
    gAudioMgr_8053AF80->stopBgm_80458F9C(60);
}

void Frostare_804A4C58::setState_801D9C58(int state) {
    field_0x336 = 0;
    if (mState == 0x11) {
        mFlags &= ~0x80;
    }
    switch (state) {
    case 2:
        mSeq2a8.start_801DA9E8(3, 0);
        startSound_801F0E34(0x40052, 0);
        break;
    case 3:
        mSeq2a8.start_801DA9E8(4, 0);
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
        switch (mState) {
        case 6:
        case 7:
            mSeq2a8.start_801DA9E8(2, 0);
            break;
        default:
            mSeq2a8.start_801DA9E8(1, 0);
            break;
        }
        startSound_801F0E34(0x40052, 0);
        break;
    case 6:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(3, 0);
        startSound_801F0E34(0x40052, 0);
        break;
    case 7:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(4, 0);
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
        mSeq2a8.start_801DA9E8(5, 0);
        startSound_801F0E34(0x40056, 0);
        fn_801E6394();
        fn_801E631C();
        break;
    case 9:
        mFlags &= ~0x280;
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(6, 0);
        startSound_801F0E34(0x40057, 0);
        fn_801E631C();
        break;
    case 10:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(7, 0);
        break;
    case 11:
        field_0x250 = -1;
        mTargetSpeed = 1.2f;
        mSeq2a8.fn_801DA994(8, 2);
        if (mState != 0xC) {
            field_0x33c = randInt_801CD664(0xF1) + 0x78;
        }
        break;
    case 12:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(9, 0);
        break;
    case 13:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(0xE, 0);
        break;
    case 14:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(0xF, 0);
        break;
    case 15:
        field_0x33f = 0;
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(0xA, 0);
        break;
    case 16:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(0xB, 0);
        break;
    case 17:
        mFlags |= 0x80;
        field_0x250 = -1;
        field_0x336 = randInt_801CD664(1) + 0xF0;
        mTargetSpeed = 0.0f;
        mSeq2a8.fn_801DA994(0xC, 2);
        startSound_801F0E34(0x40057, 0);
        fn_801E631C();
        break;
    case 18:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mSeq2a8.start_801DA9E8(0xD, 0);
        break;
    case 19:
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mBox2.set(mBox);
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        mVel_0x3c.set(0.0f, 0.0f, 0.0f);
        mHitTimer_0xd8 = 0x10;
        mHitTimerStart_0xdc = 0x10;
        startSound_801F0E34(0x4005A, 0);
        break;
    case 20:
        fn_801DAC6C()->fn_801DAE24();
        break;
    }
    mStateTimer = 0;
    mState = state;
}

void Frostare_804A4C58::fn_801D9960() {
    s32 target;
    s32 player;
    if (field_0x33c != 0) {
        field_0x33c--;
    }
    if (field_0x33c == 0) {
        target = -1;
        JGeometry::TVec3f from;
        if (mFlags & 0x2000) {
            from.set(mPos.x - -30.0f, -6.0f + mPos.y, 0.0f);
        } else {
            from.set(-30.0f + mPos.x, -6.0f + mPos.y, 0.0f);
        }
        s16 angle = (mFlags & 0x2000) ? 0x2000 : -0x2000;
        player = fn_801D0338(&from, angle);
        if (player >= 0) {
            f32 dy = getPlayerPos_8023E724(player)->y - from.y;
            f32 dx = getPlayerPos_8023E724(player)->x - from.x;
            s32 heading = (s32)(JMath::atanTable_.atan2_(dx, dy) *
                                (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
            if ((1.0f / 32768.0f) * JMAAbs((s16)(angle - heading)) <= 0.3f) {
                target = player;
            }
        }
        if (target >= 0) {
            if (mSeq2a8.fn_801DA894()) {
                setState_801D9C58(0xD);
            }
        }
    }
    if (mState == 0xB) {
        field_0x338--;
        if (field_0x338 == 0) {
            if (field_0x33f == 3) {
                s32 columns[12];
                s32 n = 0;
                for (s32 i = 0; i < 3; i++) {
                    for (s32 j = 1; j < 3; j++) {
                        columns[n] = i * 4 + j;
                        n++;
                    }
                }
                if (n > 0) {
                    field_0x334 = columns[randInt_801CD664(n)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = randInt_801CD664(12);
            }
            field_0x338 = randInt_801CD664(0x3D) + 0x3C;
            if (needsTurn(this)) {
                setState_801D9C58(0xC);
            }
        }
    }
}

static inline s32 findTarget_801D95F4(Frostare_804A4C58* self) {
    s16 angle;
    s32 player;
    s32 target = -1;
    JGeometry::TVec3f from;
    if (self->mFlags & 0x2000) {
        from.set(self->mPos.x - -30.0f, -6.0f + self->mPos.y, 0.0f);
    } else {
        from.set(-30.0f + self->mPos.x, -6.0f + self->mPos.y, 0.0f);
    }
    angle = (self->mFlags & 0x2000) ? 0x2000 : -0x2000;
    player = self->fn_801D0338(&from, angle);
    if (player >= 0) {
        f32 dy = getPlayerPos_8023E724(player)->y - from.y;
        f32 dx = getPlayerPos_8023E724(player)->x - from.x;
        s32 heading = (s32)(JMath::atanTable_.atan2_(dx, dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        if ((1.0f / 32768.0f) * JMAAbs((s16)(angle - heading)) <= 0.3f) {
            target = player;
        }
    }
    return target;
}

// FIXME(shotgun): 99.45 %, regalloc-only; behaviour clean.
bool Frostare_804A4C58::fn_801D95F4() {
    bool spawned = false;
    JGeometry::TVec3f pos;
    if (mFlags & 0x2000) {
        pos.set(mPos.x - -30.0f, -6.0f + mPos.y, 0.0f);
    } else {
        pos.set(-30.0f + mPos.x, -6.0f + mPos.y, 0.0f);
    }
    s32 serial = getActorManager_801F856C()->spawnActor_801F9484('BOIB', mLayer_0x4, &pos, 0, -1, -1);
    if (serial >= 0) {
        ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (bomb != NULL) {
            s32 target = findTarget_801D95F4(this);
            JGeometry::TVec2f dir;
            if (target >= 0) {
                dir.set(getPlayerPos_8023E724(target)->x - pos.x,
                        getPlayerPos_8023E724(target)->y - pos.y);
                if (dir.squared() <= JGeometry::TUtilf::epsilon()) {
                    dir.x = 1.0f;
                    dir.y = 0.0f;
                } else {
                    dir.normalize();
                }
            } else {
                s32 facing = (mFlags & 0x2000) ? 0x2000 : -0x2000;
                f32 s = JMath::sincosTable_.sinShort(-facing);
                f32 c = JMath::sincosTable_.cosShort(-facing);
                JGeometry::TVec2f base(0.0f, 1.0f);
                dir.set(base.x * c - base.y * s, base.x * s + base.y * c);
            }
            s16 jitter = randInt_801CD664(0xE39) - 0x71C;
            f32 s = JMath::sincosTable_.sinShort(-jitter);
            f32 c = JMath::sincosTable_.cosShort(-jitter);
            dir.set(dir.x * c - dir.y * s, dir.x * s + dir.y * c);
            dir.scale(2.4f);
            JGeometry::TVec3f vel(dir.x, dir.y, 0.0f);
            bomb->mVel_0x3c = vel;
            spawned = true;
        }
    }
    return spawned;
}

// Unrolled middle-column fill in states 9/14/18 emits stw instead of stwx for odd slots, plus regalloc and .sdata2 double alignment.
// FIXME: 99.80 %, regalloc-only; behaviour drift.
void Frostare_804A4C58::updateState_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (fn_801CD228(mLayer_0x4)) {
            s32 p = 0;
            for (; p < 4; p++) {
                if (JMAAbs(getPlayerPos_8023E724(p)->x - mPos.x) <= 80.0f ||
                    JMAAbs(getPlayerPos_8023E724(p)->y - mPos.y) <= 80.0f) {
                    break;
                }
            }
            if (p < 4 && getActorManager_801F856C()->fn_801F68E0() != 0) {
                setState_801D9C58(1);
            }
        }
        break;
    case 1:
        if (mStateTimer >= 180) {
            setState_801D9C58(2);
        }
        break;
    case 2:
        if (mSeq2a8.field_0x14 & 1) {
            setState_801D9C58(3);
        }
        break;
    case 3:
        if (mSeq2a8.field_0x14 & 1) {
            getActorManager_801F856C()->fn_801F6884();
            gAudioMgr_8053AF80->startBgm_80458FF0(0x1000004, true, 0);
            field_0x33a = randInt_801CD664(0x1E1) + 0x258;
            setState_801D9C58(4);
        } else if (mStateTimer == 6) {
            mFlags &= ~0x4000;
            if (!(mFlags & 0x4000) && !(fn_801DAC6C()->field_0x84 & 1)) {
                s32 n = 30 - fn_801DAC6C()->mCount;
                s32 count = 10;
                if (n < 10) {
                    count = n;
                }
                if (count > 0) {
                    JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
                    for (s32 i = 0; i < count; i++) {
                        getActorManager_801F856C()->spawnActor_801F9484('ICHL', mLayer_0x4, &pos, 0, -1, -1);
                    }
                    startSound_801F0E34(0x40053, 0);
                }
            }
        }
        break;
    case 4:
        field_0x33a--;
        if (field_0x33a == 0) {
            field_0x33a = randInt_801CD664(0x1E1) + 0x258;
            bool ok = false;
            if (findNearestPlayer_801D05F8(&mPos, 0.0f) >= 0 && fn_801DAC6C()->mCount < 30 && !(fn_801DAC6C()->field_0x84 & 1)) {
                ok = true;
            }
            if (ok) {
                setState_801D9C58(6);
            }
        }
        break;
    case 5:
    case 6:
        if (mSeq2a8.field_0x14 & 1) {
            setState_801D9C58(7);
        }
        break;
    case 7:
        if (mSeq2a8.field_0x14 & 1) {
            setState_801D9C58(4);
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
                        getActorManager_801F856C()->spawnActor_801F9484('ICHL', mLayer_0x4, &pos, 0, -1, -1);
                    }
                    startSound_801F0E34(0x40053, 0);
                }
            }
        }
        break;
    case 8:
        if (mPos.y >= field_0x2d0) {
            mPos.y = field_0x2d0;
            getGameView_802D6A9C()->fn_802D79E8(mLayer_0x4, 12.0f, 0.0f, false);
            gGameRoot_8053AAF8->startRumble_80134DE4(2, 0);
            setState_801D9C58(9);
        }
        break;
    case 9:
        if (mStateTimer >= 180) {
            if (field_0x33f == 3) {
                s32 columns[12];
                s32 n = 0;
                for (int row = 0; row < 3; row++) {
                    for (int col = 1; col < 3; col++) {
                        columns[n] = row * 4 + col;
                        n++;
                    }
                }
                if (n > 0) {
                    field_0x334 = columns[randInt_801CD664(n)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = randInt_801CD664(12);
            }
            field_0x338 = randInt_801CD664(0x3D) + 0x3C;
            if (isColumnRight(this)) {
                mFlags |= 0x2000;
            } else {
                mFlags &= ~0x2000;
            }
            setState_801D9C58(10);
        }
        break;
    case 10:
        if (mSeq2a8.field_0x14 & 1) {
            fn_801E6234();
            setState_801D9C58(11);
        }
        break;
    case 11:
        fn_801D9960();
        break;
    case 12:
        if (mSeq2a8.field_0x14 & 1) {
            mFlags ^= 0x2000;
            setState_801D9C58(11);
        }
        break;
    case 13:
        if (mSeq2a8.field_0x14 & 1) {
            setState_801D9C58(14);
        }
        break;
    case 14:
        if (mSeq2a8.field_0x14 & 1) {
            s32 target = -1;
            const f32 offsetX = -30.0f;
            const f32 offsetY = -6.0f;
            JGeometry::TVec3f from;
            if (mFlags & 0x2000) {
                from.set(mPos.x - offsetX, offsetY + mPos.y, 0.0f);
            } else {
                from.set(offsetX + mPos.x, offsetY + mPos.y, 0.0f);
            }
            s16 angle = (mFlags & 0x2000) ? 0x2000 : -0x2000;
            s32 player = fn_801D0338(&from, angle);
            if (player >= 0) {
                f32 dy = getPlayerPos_8023E724(player)->y - from.y;
                f32 dx = getPlayerPos_8023E724(player)->x - from.x;
                s32 heading = (s32)(JMath::atanTable_.atan2_(dx, dy) *
                                    (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                if ((1.0f / 32768.0f) * JMAAbs((s16)(angle - heading)) <= 0.3f) {
                    target = player;
                }
            }
            if (target >= 0) {
                if (field_0x33e < 2 ||
                    (field_0x33e < 3 && gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.6f)) {
                    setState_801D9C58(14);
                    break;
                }
            }
            if (field_0x33f == 3) {
                s32 columns[12];
                s32 n = 0;
                for (int row = 0; row < 3; row++) {
                    for (int col = 1; col < 3; col++) {
                        columns[n] = row * 4 + col;
                        n++;
                    }
                }
                if (n > 0) {
                    field_0x334 = columns[randInt_801CD664(n)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = randInt_801CD664(12);
            }
            field_0x338 = randInt_801CD664(0x3D) + 0x3C;
            if (needsTurn(this)) {
                setState_801D9C58(12);
            } else {
                setState_801D9C58(11);
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
            setState_801D9C58(16);
        }
        break;
    case 16:
        startSound_801F0E34(0x40059, 0);
        if (mPos.y >= field_0x2d0) {
            mPos.y = field_0x2d0;
            getGameView_802D6A9C()->fn_802D79E8(mLayer_0x4, 6.0f, 0.0f, false);
            gGameRoot_8053AAF8->startRumble_80134DE4(2, 0);
            setState_801D9C58(17);
        }
        break;
    case 17:
        if (field_0x336 == 0) {
            setState_801D9C58(18);
        }
        break;
    case 18:
        if (mSeq2a8.field_0x14 & 1) {
            if (field_0x33f == 3) {
                s32 columns[12];
                s32 n = 0;
                for (int row = 0; row < 3; row++) {
                    for (int col = 1; col < 3; col++) {
                        columns[n] = row * 4 + col;
                        n++;
                    }
                }
                if (n > 0) {
                    field_0x334 = columns[randInt_801CD664(n)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = randInt_801CD664(12);
            }
            field_0x338 = randInt_801CD664(0x3D) + 0x3C;
            if (needsTurn(this)) {
                setState_801D9C58(12);
            } else {
                setState_801D9C58(11);
            }
        }
        break;
    case 19:
        if (getActorManager_801F856C()->fn_801F68E0() != 0) {
            releaseEmitter_801EE434(mEmitter2c[2]);
            releaseEmitter_801EE434(mEmitter2c[3]);
            mEmitter2c[2] = NULL;
            mEmitter2c[3] = NULL;
            startBossDeath_0x28(0, 0.0f, -16.0f);
            setState_801D9C58(20);
        }
        break;
    case 20:
        break;
    }
}

void Frostare_804A4C58::updateAnim_0x94() {
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
// (updateAnim_0x94 only in state 0xF); otherwise EnemyBase's state step runs.
// Then the countdown at 0x336 and the effect pair at 0x2C8.
void Frostare_804A4C58::updateBehavior_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        fn_801CE2BC(mapHit);
    } else if (mState == 0xF) {
        updateAnim_0x94();
    }
    if (field_0x336 != 0) {
        field_0x336--;
    }
    fn_801E61C4();
}

// Vtable slot 0x80 (0x801D8980). A position above the ceiling in
// field_0x2d0 is clamped to it, its vertical speed is zeroed, and then
// EnemyBase's floor check runs.
void Frostare_804A4C58::checkFloor_0x80() {
    if (mPos.y > field_0x2d0) {
        mPos.y = field_0x2d0;
        mVel_0x3c.y = 0.0f;
    }
    EnemyBase_804A4808::checkFloor_0x80();
}

void Frostare_804A4C58::calcVelocity_0x88() {
    switch (mState) {
    case 0xB: case 0xC: case 0xD: case 0xE: {
        JGeometry::TVec2f dir(field_0x2d4[field_0x334].x - mPos.x, field_0x2d4[field_0x334].y - mPos.y);
        dir.normalize();
        dir.scale(mSpeed);
        addCalc2_801F71A4(&mVel_0x3c.x, dir.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&mVel_0x3c.y, dir.y, 0.3f, 1.0f);
        break;
    }
    default:
    {
        mVel_0x3c.x = 0.0f;
        switch (mState) {
        case 0x10:
        case 8:
            mVel_0x3c.y += JMAAbs(lbl_8053EFB8);
            break;
        default:
            mVel_0x3c.y = 0.0f;
            break;
        }
    }
    }
    mVel_0x3c.z = 0.0f;
}


void Frostare_804A4C58::onHitReactionEnd_0x74() {
    switch (mState) {
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
        if (mState == 0xC) {
            if (field_0x33f == 3) {
                s32 columns[12];
                s32 n = 0;
                for (s32 row = 0; row < 3; row++) {
                    for (s32 col = 1; col < 3; col++) {
                        columns[n] = row * 4 + col;
                        n++;
                    }
                }
                if (n > 0) {
                    field_0x334 = columns[randInt_801CD664(n)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = randInt_801CD664(12);
            }
            field_0x338 = randInt_801CD664(0x3D) + 0x3C;
            if (isColumnRight(this)) {
                mFlags &= ~0x2000;
            } else {
                mFlags |= 0x2000;
            }
        } else {
            if (field_0x33f == 3) {
                s32 columns[12];
                s32 n = 0;
                for (s32 row = 0; row < 3; row++) {
                    for (s32 col = 1; col < 3; col++) {
                        columns[n] = row * 4 + col;
                        n++;
                    }
                }
                if (n > 0) {
                    field_0x334 = columns[randInt_801CD664(n)];
                } else {
                    field_0x334 = 0;
                }
            } else {
                field_0x334 = randInt_801CD664(12);
            }
            field_0x338 = randInt_801CD664(0x3D) + 0x3C;
            if (needsTurn(this)) {
                setState_801D9C58(0xC);
            } else {
                setState_801D9C58(0xB);
            }
        }
        break;
    }
}

// Vtable slot 0xA0 (0x801D84E4), the last slot of the vtable.
void Frostare_804A4C58::vfunc_0xA0() {
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
        if (!camera->mShowWholeArea_0x7f5) {
            camera->mFocusOverride_0x7f4 = 1;
            camera->mFocusMode_0x7f8 = 4;
            camera->mFocusPos_0x7fc.set(mPos);
            camera->mOverrideBlend_0x7f6 = 0;
        }
    } else {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (!gGameRoot2_8053AB10->field_0x24->field_0x179) {
            gGameRoot2_8053AB10->field_0x24->field_0x178 = 1;
        }
    }
}

void Frostare_804A4C58::die_0x24() {}

void Frostare_804A4C58::update() {
    if (updateBossDeath_0x2C(0.0f, -16.0f, 50.0f)) {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->mBlendBoxOn_0x840 == 0) {
            camera->mBlendBoxOn_0x840 = 1;
            camera->mBlendBox_0x844.i.x = camera->mBlendBox_0x844.f.x = mPos.x;
            camera->mBlendBox_0x844.i.y = camera->mBlendBox_0x844.f.y = mPos.y;
            camera->mBlendWeight_0x854 = 1.0f;
            camera->mMargin_0x1d4 = -1.0f;
            camera->mMinHalfWidth_0x1d8 = -1.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
    } else {
        EnemyBase_804A4808::update();
    }
}

void Frostare_804A4C58::fn_801D8400() {
    if (mState < 8) {
        setState_801D9C58(8);
    }
}
