#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/ActorManager.h"
#include "game/cls_804B1C58.h"
#include "game/Unk_8045A080.h"
#include "game/act/act_shooting_gallery.h"
#include "JSystem/JMath/JMath.h"

s32 lbl_804A2B08[4] = {0, 1, 2, 1};

s32 lbl_804C6330[4];

static inline bool talk(s32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
    return fn_8038AD24(player, msg, serial, layer, pos, flags);
}

static inline void facePlayer(ShootingGallery_804A2B18* self, s32 player) {
    JGeometry::TVec3f diff;
    f32 dist;
    self->fn_801EE8F4(&diff, &dist, player);
    if (dist < 100.0f) {
        u32 angle = (u16)(JMath::atanTable_.atan2_(-diff.x, -diff.y) *
                          (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        if (angle < 0x2000 || angle >= 0xe000) {
            self->field_0x98 = 3;
        } else if (angle >= 0x2000 && angle < 0x6000) {
            self->field_0x98 = 1;
        } else if (angle >= 0x6000 && angle <= 0xa000) {
            self->field_0x98 = 2;
        } else {
            self->field_0x98 = 0;
        }
    }
}

static inline void setCameraScriptBox(const JGeometry::TBox2f& box) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 5;
        camera->field_0x808.set(box);
        camera->field_0x7f6 = 0;
    }
}

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

// FIXME: 96.36%. missing/extra + reloc-name: ours has 1 fewer instr (935 vs 936). The box setup at the
// top and in case 3 schedules the mPos.x/mPos.y adds and stores in a different order; the stack slots of
// the by-value position temporaries for fn_8038AD24 and fn_801EE8F4 and the loop registers in cases 0, 3,
// and 5 differ; lbl_8053ABB8 is referenced via @sda21 in the original (split into act_all_enemies_defeated.cpp).
void ShootingGallery_804A2B18::fn_8019F3AC() {
    JGeometry::TVec3f pos = mPos;
    pos.y -= 50.0f;
    JGeometry::TVec3f savedPos(mPos.x, mPos.y, mPos.z);
    // Unused copy of mPos at 0xa0(r1) (like camPos in act_digging_game.cpp).
    JGeometry::TVec3f unusedPos = mPos;
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    JGeometry::TBox2f visibleBox;

    fn_80239914(&box, field_0x4, mSerial);

    box = mBox2;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;

    switch (mState) {
    case 0: {
        s32 notSameLayer = 0;
        for (s32 i = 0; i < 4; i++) {
            if (getField_0x4() != getPlayerLayer_8023DE58(i)) {
                notSameLayer = 1;
                break;
            }
        }
        if (notSameLayer != 0) {
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            field_0x250 = fn_8022FD8C(mSerial);
            if (field_0x250 < 0) {
                break;
            }
            if (!fn_802300EC(field_0x250)) {
                break;
            }
            if (!talk(field_0x250, 0x342, mSerial, field_0x4, pos, 0)) {
                break;
            }
            facePlayer(this, field_0x250 + 1);
            mState = 8;
        } else {
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            field_0x250 = fn_8022FD8C(mSerial);
            if (field_0x250 < 0) {
                break;
            }
            if (!talk(field_0x250, 0x334, mSerial, field_0x4, pos, 0)) {
                break;
            }
            facePlayer(this, field_0x250 + 1);
            mState = 1;
        }
        break;
    }
    case 1:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (!fn_803865B8(field_0x250)) {
            break;
        }
        switch (fn_8037EFB4(field_0x250)) {
        case 0:
            fn_8038470C(field_0x250, 0x335);
            mState = 2;
            break;
        case 1:
            fn_8038470C(field_0x250, 0x336);
            mState = 8;
            break;
        }
        break;
    case 2:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x250, mSerial)) {
            fn_80230534(false);
            fn_801F0D20(0x3a);
            gAudioMgr_8053AF80->fn_80458FF0(0x1000007, true, 0);
            fn_8022B194();
            field_0x98 = 2;
            field_0x24c = 0;
            field_0x250 = 0;
            field_0x254 = 0;
            field_0x258 = 0;
            field_0x25c = 0;
            field_0x260 = 0;
            mTimers[1] = 120;
            mTimers[2] = 0;
            mTimers[3] = 0;
            facePlayer(this, 2);
            field_0x28c = 1;
            mState = 3;
        }
        break;
    case 3: {
        savedPos.set(mPos.x, mPos.y, mPos.z);
        mPos.x = 128.0f;
        mPos.y = 138.0f;
        box = mBox2;
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;
        fn_80239914(&box, field_0x4, mSerial);
        mPos.x = savedPos.x;
        mPos.y = savedPos.y;
        mPos.z = savedPos.z;

        s32 anyOnScreen = 0;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) < 8) {
                anyOnScreen = 1;
            }
        }
        if (anyOnScreen == 0) {
            visibleBox = getGameView_802D6A9C()->mVisibleBox;
            setCameraScriptBox(visibleBox);
            if (field_0x260 == 0) {
                for (s32 i = 0; i < 4; i++) {
                    field_0x26c[i] = fn_802373EC(i);
                    field_0x27c[i] = fn_8023725C(i);
                    fn_802372F8(i, 4, 1);
                }
                field_0x260 = 1;
            }
        }

        s32 doneCount = 0;
        for (int i = 0; i < 4; i++) {
            s32 slot = fn_8022F340(i);
            if (lbl_804C6330[slot] >= 10) {
                lbl_8053ABB8[i] = 1;
            }
            if (lbl_8053ABB8[i] == 1 || fn_8022F514(i)) {
                doneCount++;
            }
        }
        if (doneCount >= 4) {
            field_0x25c++;
            if (field_0x25c > 70) {
                field_0x2ac = -1;
                if (getActorManager_801F856C()->fn_801F68E0() != 0) {
                    for (int i = 0; i < 4; i++) {
                        fn_80230C6C(i);
                    }
                    fn_801F0D20(0x3b);
                    gAudioMgr_8053AF80->fn_80458508(0);
                    field_0x98 = 2;
                    field_0x24c = 0;
                    field_0x250 = 0;
                    field_0x254 = 0;
                    field_0x258 = 0;
                    field_0x25c = 0;
                    mTimers[1] = 0;
                    mTimers[2] = 0;
                    mTimers[3] = 0;
                    field_0x28c = 0;
                    mState = 4;
                }
                break;
            }
        }

        if (field_0x258 == 0) {
            s32 halfCount = 0;
            for (int i = 0; i < 4; i++) {
                if (lbl_804C6330[i] >= 5 || fn_8022F514(i)) {
                    halfCount++;
                }
            }
            if (halfCount >= 4) {
                fn_801F0D20(0x3f);
                gAudioMgr_8053AF80->fn_80458508(1);
                field_0x258 = 1;
            }
        }

        if (mTimers[1] == 0) {
            s32 dir;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                savedPos.x = 360.0f;
                dir = 1;
            } else {
                savedPos.x = 150.0f;
                dir = 0;
            }
            savedPos.y = 824.0f;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('SYKY', 0, &savedPos, 1, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x98 = dir;
                }
            }
            mTimers[1] = 120;
        }

        if (mTimers[2] == 0) {
            savedPos.x = 360.0f;
            savedPos.y = 846.0f;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('SYKY', 0, &savedPos, 2, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x98 = 1;
                }
            }
            mTimers[2] = 60;
            field_0x250++;
            if (field_0x250 >= 3) {
                field_0x250 = 0;
                mTimers[2] = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 90;
            }
        }

        if (mTimers[3] == 0) {
            savedPos.x = 150.0f;
            savedPos.y = 866.0f;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('SYKY', 0, &savedPos, 3, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x98 = 0;
                }
            }
            mTimers[3] = 100;
            field_0x254++;
            if (field_0x254 >= 3) {
                field_0x254 = 0;
            }
        }
        break;
    }
    case 4:
        field_0x24c++;
        if (field_0x24c > 50) {
            field_0x24c = 51;
            if (field_0x2ac < 0) {
                for (int i = 0; i < 4; i++) {
                    if (!fn_8022F478(i)) {
                        field_0x2ac = i;
                        break;
                    }
                }
            }
            fn_8022FE80(field_0x2ac, getPlayerLayer_8023DE58(field_0x2ac), mSerial, false, false);
            if (field_0x2ac == fn_8022FD8C(mSerial)) {
                talk(field_0x2ac, 0x337, mSerial, field_0x4, pos, 2);
                mState = 5;
            }
        }
        break;
    case 5:
        fn_8022FE80(field_0x2ac, getPlayerLayer_8023DE58(field_0x2ac), mSerial, false, false);
        if (fn_803865B8(field_0x2ac) && fn_80384C58(field_0x2ac, mSerial)) {
            getActorManager_801F856C()->fn_801F6884();
            for (int i = 0; i < 4; i++) {
                if (!fn_8022F514(i)) {
                    fn_802372F8(i, field_0x26c[i], field_0x27c[i]);
                }
            }
            gGameRoot_8053AAF8->fn_80138B7C();
            vfunc_0x30(0xff, true);
            fn_8022B17C();
            mState = 6;
        }
        break;
    case 6:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x250 = fn_8022FD8C(mSerial);
        if (field_0x250 < 0) {
            break;
        }
        if (!fn_802300EC(field_0x250)) {
            break;
        }
        if (!talk(field_0x250, 0x338, mSerial, field_0x4, pos, 0)) {
            break;
        }
        facePlayer(this, field_0x250 + 1);
        mState = 7;
        break;
    case 7:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x250, mSerial)) {
            field_0x98 = 2;
            mState = 6;
        }
        break;
    case 8:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x250, mSerial)) {
            mState = 0;
        }
        break;
    }

    for (int i = 0; i < 4; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }

    if (mTimers[0] == 0) {
        field_0x264 = lbl_804A2B08[field_0x248];
        mTimers[0] = 15;
        field_0x248++;
        field_0x248 &= 3;
    }
}

// FIXME: 94.81%. missing/extra: ours has 2 fewer instrs. The original stores p (0x24(r1)) before the box
// copy is offset and reloads mPos.x/y for the addPos; ours adds in registers. Also gGameRoot_8053AAF8 and
// 0x19660D swap r4/r5 in mRandom.get_ufloat_1(), and r29/r30 for this.
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
