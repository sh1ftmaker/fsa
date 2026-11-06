#include <float.h>
#include "game/Camera.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_horse_race_man.h"

static const s16 lbl_80469C88[6] = {0, 20, 1, 15, -1, -1};
static const s16 lbl_80469C94[6] = {2, 20, 3, 15, -1, -1};
static const s16 lbl_80469CA0[6] = {4, 20, 5, 15, -1, -1};
static const s16 lbl_80469CAC[6] = {6, 20, 7, 15, -1, -1};
static const s32 lbl_80469CB8[4] = {0x366, 0x367, 0x368, 0x369};

static const s16* lbl_804A5620[4] = {
    lbl_80469C88, lbl_80469C94, lbl_80469CA0, lbl_80469CAC,
};

HorseRaceMan_804A5630::HorseRaceMan_804A5630() {
    field_0x2a8 = gGameRoot_8053AAF8->fn_80138A30();
    field_0x2ac = 0;
    field_0x2b0 = -1;
    field_0x2b4 = 0;
    mCountdown.fn_801ED974();
    fn_80216178(&mTable);
    lbl_8053A160 = -1;
}

HorseRaceMan_804A5630::~HorseRaceMan_804A5630() {
    lbl_8053A160 = -1;
}

void HorseRaceMan_804A5630::init() {
    EnemyBase_804A4808::init();
    field_0x2ac = (field_0x90 >> 22) & 0x1F;
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A5620, 4);
    mAnim.startAnim_801D22DC(field_0x98, 2, false);
}

static inline void shuffle(s32 count, s32* order, s32* pool) {
    if (count > 1) {
        for (s32 i = 0, rem = count; i < count; i++, rem--) {
            s32 idx = fn_801CD664(rem);
            order[i] = pool[idx];
            for (s32 j = idx; j < rem - 1; j++) {
                pool[j] = pool[j + 1];
            }
        }
    }
}

void HorseRaceMan_804A5630::fn_801ECB08(s32 state) {
    switch (state) {
    case 0:
        field_0x2b0 = -1;
        field_0x2b4 = 0;
        mFlags &= ~0x2000;
        break;
    case 1:
    case 2:
        field_0x98 = fn_801D0020(&mPos, field_0x2b0, false);
        break;
    case 6:
        fn_80119DD0()->fn_80119998(0x78, 0, 2, 0, 0, 0, 0);
        gAudioMgr_8053AF80->fn_80458F9C(0x3c);
        break;
    case 7: {
        s32 i;
        s32 viewId = field_0x2ac;
        if (viewId != 0) {
            getGameView_802D6A9C()->fn_802D8050(viewId, true, 0xff);
        }
        for (i = 0; i < field_0x2a8; i++) {
            if (fn_80236B50(i)) {
                fn_8023764C(i, 4);
            }
        }
        fn_8022B064();
        s32 layer = field_0x4;
        GameView_804A9950* view = getGameView_802D6A9C();
        const JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
        s32 count = field_0x2a8;
        JGeometry::TVec3f pos;
        s32 order[4];
        s32 pool[4];
        for (i = 0; i < 4; i++) {
            order[i] = i;
            pool[i] = i;
        }
        if (count >= 2) {
            shuffle(count, order, pool);
        }
        for (i = 0; i < 4; i++) {
            s32 p = fn_8022F514(i) ? fn_8022F3DC(i) : i;
            pos.set(72.0f + box->i.x, box->i.y + 16.0f * (order[p] + 3.5f), 0.0f);
            fn_80230498(i, &pos, 0x356, 0xff, -0x80, -0x80, true);
            fn_80230BF4(i, 0);
        }
        field_0x98 = 2;
        fn_802160F4(&mTable);
        break;
    }
    case 9:
        mCountdown.fn_801ED968();
        break;
    case 10:
        fn_80230534(false);
        fn_801F0D20(0x3a);
        gAudioMgr_8053AF80->fn_80458FF0(0x1000007, true, 0);
        mTable.fn_8021600C();
        break;
    case 11: {
        s32 winner = mTable.fn_80215E58(0);
        if (winner >= 0) {
            field_0x2b0 = winner;
        }
        break;
    }
    case 12:
        fn_80119DD0()->fn_80119998(0x78, 0, 2, 0, 0, 0, 0);
        gAudioMgr_8053AF80->fn_80458F9C(0x3c);
        break;
    case 13: {
        JGeometry::TVec3f pos;
        s32 i;
        s32 viewId = field_0x2ac;
        if (viewId != 0) {
            getGameView_802D6A9C()->fn_802D8050(viewId, false, 0xff);
        }
        fn_8022AF78();
        for (i = 0; i < 4; i++) {
            s32 p = fn_8022F514(i) ? fn_8022F3DC(i) : i;
            pos.set(mPos.x + 16.0f * (f32)p, mPos.y + 32.0f, 0.0f);
            fn_80230498(i, &pos, 0x3f, 0xff, -0x80, -0x80, true);
            fn_80230BF4(i, 3);
        }
        gGameRoot_8053AAF8->fn_80138B7C();
        break;
    }
    case 14:
    case 15:
    case 16:
        break;
    }
    mStateTimer = 0;
    mState = state;
}

void HorseRaceMan_804A5630::fn_801EC7B8() {
    u8 crossed[4];
    f32 times[4];
    for (s32 i = 0; i < 4; i++) {
        crossed[i] = 0;
        times[i] = 1.0f;
    }
    s32 layer = field_0x4;
    s32 count = 0;

    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    f32 finish_x = 88.0f + box->i.x;
    f32 finish_y = 0.5f * (box->i.y + box->f.y);

    for (s32 i = 0; i < field_0x2a8; i++) {
        bool finished = mTable.fn_80215E48(i) >= 0;
        if (!finished) {
            layer = field_0x4;
            if (getPlayerLayer_8023DE58(i) == layer) {
            if (getPlayerPos_8023E724(i)->y > finish_y && getPlayerPos_8023E724(i)->x < finish_x) {
                crossed[i] = 1;
                f32 cur_x = getPlayerPos_8023E724(i)->x;
                f32 prev_x = fn_8023E578(i)->x;
                f32 step = 0.6f;
                f32 dx = step * (cur_x - prev_x);
                f32 f4 = 1.0f;
                f32 f1 = cur_x - dx;
                while (f1 < finish_x) {
                    f4 -= step;
                    f1 -= dx;
                    if (f4 <= 0.0f) {
                        break;
                    }
                }
                if (f4 > 1.0f) {
                    f4 = 1.0f;
                } else if (f4 < 0.0f) {
                    f4 = 0.0f;
                }
                times[i] = f4;
                count++;
            }
        }
    }
}

    if (count > 0) {
        fn_801F0D20(0x3a);
        s32 rank = mTable.fn_80215E98();
        while (count > 0) {
            f32 minTime = FLT_MAX;
            s32 bestPlayer = -1;
            for (s32 j = 0; j < field_0x2a8; j++) {
                if (crossed[j] && times[j] <= minTime) {
                    bestPlayer = j;
                    minTime = times[j];
                }
            }
            mTable.fn_80215FB8(bestPlayer, rank, times[bestPlayer]);
            mTable.fn_80215D8C(bestPlayer);
            if (rank == 0 && fn_80215C6C(&mTable, bestPlayer)) {
                mFlags |= 0x2000;
            }
            count--;
            crossed[bestPlayer] = 0;
            rank++;
        }
    }

    bool allFinished = mTable.fn_80215E98() == mTable.mCount;
    if (allFinished) {
        fn_801ECB08(11);
        return;
    }

    if (mTable.fn_8021606C()) {
        for (s32 k = 0; k < field_0x2a8; k++) {
            bool finished = mTable.fn_80215E48(k) >= 0;
            if (!finished) {
                mTable.fn_80215D8C(k);
            }
        }
        fn_801F0D20(0x3b);
        fn_801ECB08(11);
    }
}

static inline void setCameraScript(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 4;
        camera->field_0x7fc.x = pos.x;
        camera->field_0x7fc.y = pos.y;
        camera->field_0x7fc.z = pos.z;
        camera->field_0x7f6 = 0;
    }
}

static inline void setCameraFocus(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x840 == 0) {
        camera->field_0x840 = 1;
        camera->field_0x844.i.x = camera->field_0x844.f.x = pos.x;
        camera->field_0x844.i.y = camera->field_0x844.f.y = pos.y;
        camera->field_0x854 = 1.0f;
        camera->field_0x1d4 = -1.0f;
        camera->field_0x1d8 = -1.0f;
    }
}

void HorseRaceMan_804A5630::vfunc_0x90(u8 mapHit) {
    s32 i;
    JGeometry::TBox2f box(mPos.x - 10.0f, mPos.y - 10.0f, mPos.x + 10.0f, mPos.y + 10.0f);

    switch (mState) {
    case 0: {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x2b0 = fn_8022FD8C(mSerial);
        if (field_0x2b0 >= 0) {
            s32 msg = 0x364;
            bool flag = vfunc_0x34();
            if (!flag) {
                msg = 0x35D;
            }
            if (fn_8038AD24(field_0x2b0, msg, mSerial, field_0x4, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 0)) {
                if (flag) {
                    fn_801ECB08(1);
                } else {
                    fn_801ECB08(2);
                }
            }
        }
        break;
    }
    case 1:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x2b0) && fn_80384C58(field_0x2b0, mSerial)) {
            field_0x98 = 2;
            fn_801ECB08(0);
        }
        break;
    case 2:
        setCameraScript(mPos);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x2b0)) {
            if (fn_8037EFB4(field_0x2b0) == 0) {
                if (fn_8038470C(field_0x2b0, 0x35E)) {
                    fn_801ECB08(3);
                }
            } else {
                if (fn_8038470C(field_0x2b0, 0x35F)) {
                    fn_801ECB08(4);
                }
            }
        }
        break;
    case 3:
        setCameraScript(mPos);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x2b0) && fn_80384C58(field_0x2b0, mSerial)) {
            if (fn_8023077C()) {
                fn_801ECB08(6);
            } else {
                fn_801ECB08(5);
            }
        }
        break;
    case 4:
        setCameraScript(mPos);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x2b0) && fn_80384C58(field_0x2b0, mSerial)) {
            field_0x98 = 2;
            fn_801ECB08(0);
        }
        break;
    case 5:
        setCameraScript(mPos);
        if (fn_8023077C()) {
            fn_801ECB08(6);
        }
        break;
    case 6:
        setCameraScript(mPos);
        if (mStateTimer >= 60) {
            fn_801ECB08(7);
        }
        break;
    case 7:
        if (mStateTimer >= 60) {
            fn_801ECB08(8);
        }
        break;
    case 8:
        if (mStateTimer >= 45) {
            fn_801ECB08(9);
        }
        break;
    case 9:
        if (mStateTimer >= 180) {
            fn_801ECB08(10);
        } else if (mStateTimer >= 120) {
            s32 val = mTable.field_0x94.fn_8021C024();
            for (i = 0; i < 4; i++) {
                fn_80230498(i, getPlayerPos_8023E724(i), val + 0x33A, 0xFF, -0x80, -0x80, true);
            }
        }
        break;
    case 10:
        mTable.fn_80215DB8();
        fn_801EC7B8();
        break;
    case 11:
        mTable.fn_80215DB8();
        if (mStateTimer >= 30) {
            if (mTable.fn_80215D38()) {
                fn_801ECB08(12);
            }
        } else {
            bool allFinished = mTable.fn_80215E98() == mTable.mCount;
            if (allFinished && mStateTimer == 15) {
                fn_801F0D20(0x3A);
            }
        }
        break;
    case 12:
        if (mStateTimer >= 60) {
            fn_801ECB08(13);
        }
        break;
    case 13:
        setCameraFocus(mPos);
        if (mStateTimer >= 60) {
            fn_801ECB08(14);
        }
        break;
    case 14: {
        setCameraFocus(mPos);
        fn_8022FE80(field_0x2b0, field_0x4, mSerial, false, false);
        if (field_0x2b0 == fn_8022FD8C(mSerial)) {
            if (mTable.fn_80215E98() == 0) {
                if (fn_8038AD24(field_0x2b0, 0x365, mSerial, field_0x4, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 2)) {
                    fn_801ECB08(16);
                }
            } else {
                s32 rank = mTable.fn_80215E58(0);
                fn_803894BC(field_0x2b0, rank + 1);
                if (fn_8038AD24(field_0x2b0, 0x366, mSerial, field_0x4, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 2)) {
                    field_0x2b4++;
                    fn_801ECB08(15);
                }
            }
        }
        break;
    }
    case 15:
        setCameraFocus(mPos);
        fn_8022FE80(field_0x2b0, field_0x4, mSerial, false, false);
        if (fn_803865B8(field_0x2b0)) {
            mTable.fn_80215B6C(mTable.fn_80215E58(field_0x2b4 - 1));
            if (field_0x2b4 >= mTable.fn_80215E98()) {
                if (fn_8038470C(field_0x2b0, 0x36A)) {
                    fn_801ECB08(16);
                }
            } else {
                s32 key2 = mTable.fn_80215E58(field_0x2b4);
                fn_803894BC(field_0x2b0, key2 + 1);
                if (fn_8038470C(field_0x2b0, lbl_80469CB8[field_0x2b4])) {
                    field_0x2b4++;
                }
            }
        }
        break;
    case 16:
        setCameraFocus(mPos);
        fn_8022FE80(field_0x2b0, field_0x4, mSerial, false, false);
        if (fn_803865B8(field_0x2b0) && fn_80384C58(field_0x2b0, mSerial)) {
            fn_80230534(false);
            for (i = 0; i < 4; i++) {
                getGameView_802D6A9C()->fn_802D7844(0, i);
            }
            if (mFlags & 0x2000) {
                i = field_0x2b0;
                if (fn_8022F514(i)) {
                    i = fn_8022F3DC(field_0x2b0);
                }
                mTable.fn_80215BCC(i);
            }
            vfunc_0x30(0xFF, true);
            fn_801ECB08(0);
        }
        break;
    }
}

void HorseRaceMan_804A5630::vfunc_0x94() {
    mAnim.fn_801D2264(field_0x98, 2);
    EnemyBase_804A4808::vfunc_0x94();
}

void HorseRaceMan_804A5630::vfunc_0x9C(u8 mapHit) {
    mCountdown.fn_801ED8F0();
    fn_80215F30(&mTable);
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    JGeometry::TBox2f box(mPos.x - 10.0f, mPos.y - 10.0f, mPos.x + 10.0f, mPos.y + 10.0f);
    fn_80239914(&box, field_0x4, mSerial);
    s32 serial = mSerial;
    s32 layer = field_0x4;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    boxes->mLists[boxes->field_0x0].fn_8020D8C0(&box, layer, serial);
    lbl_8053A160 = fn_80215A94(&mTable);
}

void HorseRaceMan_804A5630::draw() {
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, NULL, 0, "kage_sob.bin", 0, NULL);
    }
    s32 frame = mAnim.mCel + 0x319;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, NULL, 1.0f, 1.0f);
    }
    mCountdown.fn_801ED828();
    if (mState >= 17 || mState < 7) {
        return;
    }
    mTable.fn_80215ECC();
}
