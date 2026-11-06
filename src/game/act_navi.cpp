#include "game/GameAudio.h"
#include "game/act/act_navi.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "game/tu_8021F064.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"

extern "C" s32 fn_801CCE3C(const JGeometry::TVec3f* pos, s32 layer);
extern "C" bool fn_801CD754(JGeometry::TVec2f* cur, const JGeometry::TVec2f* target, f32 f1, f32 f2, f32 f3);
bool fn_801CD8E4(f32* value, f32 target, f32 factor, f32 threshold);

static const s16 lbl_8046A770[] = {
    0, 1,
    1, 1,
    2, 1,
    3, 1,
    4, 1,
    5, 1,
    -1, -1,
};

static const u16 lbl_8046A78C[4][4] = {
    {0x0513, 0x0514, 0x0515, 0x0516},
    {0x051B, 0x051C, 0x051D, 0x051E},
    {0x050F, 0x0510, 0x0511, 0x0512},
    {0x0517, 0x0518, 0x0519, 0x051A},
};

struct ColorRGB {
    u8 r, g, b;
};

static const ColorRGB lbl_8046A7AC[6] = {
    {0x4B, 0xFF, 0x2D},
    {0x2D, 0x78, 0xFF},
    {0xDC, 0x1E, 0xDC},
    {0xFF, 0x41, 0x37},
    {0xC8, 0xA0, 0x28},
    {0x1E, 0xA0, 0xBE},
};

static const s32 lbl_8046A7C0[6] = {
    0x1D0, 0x1D6, 0x1DC, 0x1E2, 0x1E8, 0x1EE,
};

static const char lbl_8046A7D8[] = "enemy_both2_sob.bin";

static const s16* lbl_8053A188[] = {lbl_8046A770, NULL};
s32 lbl_8053A190 = -1;

static const char lbl_8053FD50[] = "xi<P_s";

cls_804A6708::cls_804A6708() {
    field_0x2a8 = 0;
    field_0x2ac = 0;
    field_0x2ad = 0;
    field_0x2b0 = 0;
    field_0x2b4 = -1;
    field_0x2b8 = 255.0f;
    field_0x2bc = 0.0f;
    field_0x2c0 = 0.0f;
    field_0x2c4 = 0.0f;
    field_0x2c8 = 0.0f;
    field_0x2cc = 0.0f;
    mEmitter2D0 = NULL;
    mEmitter2D4 = NULL;
}

cls_804A6708::~cls_804A6708() {
    releaseEmitter_801EE434(mEmitter2D0);
    releaseEmitter_801EE434(mEmitter2D4);
    if (mFlags & 0x4000) {
        fn_8021F064()->fn_8021F1F0(mSerial);
    }
}

void cls_804A6708::init() {
    EnemyBase_804A4808::init();
    field_0x2a8 = (field_0x90 & 0xFFFF) + 0x12b;
    s32 param = (field_0x90 >> 16) & 7;
    if (param > 5) {
        param = 5;
    } else if (param < 0) {
        param = 0;
    }
    field_0x2ac = param;

    if (!((field_0x90 >> 19) & 1)) {
        mFlags |= 0x10000;
    }

    field_0x2ad = (field_0x90 >> 20) & 0x3F;
    if (field_0x2ad < 1) {
        field_0x2ad = 1;
    }

    field_0x2b0 = (field_0x90 >> 26) & 0x1F;

    if (lbl_80529DEC.field_0x118 == 6 && lbl_80529DEC.field_0x11c == 0 && (field_0x90 >> 31)) {
        mFlags |= 0x4000;
        if (field_0x2ac == 0) {
            mFlags |= 0x2000;
        }
    } else if (lbl_80529DEC.field_0x118 == 6 && lbl_80529DEC.field_0x11c == 1 && (field_0x90 >> 31)) {
        mFlags |= 0xA000;
        mFlags &= ~0x10000;
    } else {
        mFlags |= 0x2000;
    }

    if ((field_0x90 >> 31) && lbl_80529DEC.field_0x11c == 0 && lbl_80529DEC.field_0x118 < 7) {
        if (lbl_80529DEC.field_0x118 >= 1) {
            mFlags |= 0x20000;
            mFlags &= ~0x10000;
        }
    }

    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;

    if (mFlags & 0x20000) {
        mPos.z = 18.0f;
    }

    field_0x2c8 = mPos.x;
    field_0x2cc = mPos.y;

    if (mFlags & 0x20000) {
        mState = 0;
    } else {
        mState = 3;
    }

    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;

    mAnim.fn_801D2608(lbl_8053A188, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();

    if (mFlags & 0x4000) {
        fn_8021F064()->fn_8021F25C(mSerial);
    }

    if (getGameView_802D6A9C()->field_0x131f9 == 2) {
        fn_801F3668();
    } else if (mFlags & 0x20000) {
        if (isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c)) {
            fn_801F3668();
        } else {
            bool flag = false;
            if (mState == 9) {
                flag = true;
            }
            mEmitter2D0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, 0, -1, NULL, NULL, flag, 1.0f);
            bool flag2 = false;
            if (mState == 9) {
                flag2 = true;
            }
            mEmitter2D4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, -1, NULL, NULL, flag2, 1.0f);
            if (mEmitter2D0 != NULL) {
                mEmitter2D0->setGlobalEnvColor(lbl_8046A7AC[field_0x2ac].r, lbl_8046A7AC[field_0x2ac].g, lbl_8046A7AC[field_0x2ac].b);
            }
        }
    }
}

static inline void approach(f32& cur, f32 target, f32 remaining) {
    if (remaining < FLT_EPSILON) {
        cur = target;
    } else {
        cur += (1.0f / remaining) * (target - cur);
    }
}

void cls_804A6708::fn_8021E4F8() {
    switch (mState) {
    case 4:
        approach(field_0x2bc, 18.0f, 120 - mStateTimer);
        break;
    case 7:
        approach(field_0x2bc, 1.0f, 120 - mStateTimer);
        break;
    }

    JGeometry::TVec2f diff(field_0x2c0 - field_0x2c8, field_0x2c4 - field_0x2cc);
    f32 sq = diff.squared();
    if (sq > 32.0f * FLT_EPSILON) {
        f32 invSqrt = JGeometry::TUtil<f32>::inv_sqrt(sq);
        diff.x *= invSqrt;
        diff.y *= invSqrt;
    }

    diff.x *= field_0x2bc;
    diff.y *= field_0x2bc;

    bool usePlayer = (field_0x2b4 >= 0 && (mFlags & 0x30000));
    f32 targetX, targetY;
    if (usePlayer) {
        const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2b4);
        targetX = pPos->x;
        targetY = pPos->y;
    } else {
        targetX = mSpawnPos.x;
        targetY = mSpawnPos.y;
    }

    fn_801CD8E4(&field_0x2c8, targetX, 1.0f, 0.0f);
    fn_801CD8E4(&field_0x2cc, targetY, 1.0f, 0.0f);

    JGeometry::TVec2f pos(field_0x2c8 + diff.x, field_0x2cc + diff.y);
    if (mState == 7 && usePlayer) {
        fn_801CD754((JGeometry::TVec2f*)&field_0x2c0, &pos, 0.3f, 1.0f, 4.0f);
    } else {
        field_0x2c0 = pos.x;
        field_0x2c4 = pos.y;
    }

    JGeometry::TVec2f curDiff(field_0x2c0 - field_0x2c8, field_0x2c4 - field_0x2cc);
    f32 dist = JGeometry::TUtil<f32>::sqrt(curDiff.squared());

    s32 angle = 0x622;
    if (dist > 0.001f) {
        angle = (s32)((1570.0f * field_0x2bc) * (1.0f / dist));
        if (angle > 0x622) {
            angle = 0x622;
        } else if (angle < 0x200) {
            angle = 0x200;
        }
    }

    s16 rot = angle;
    if (mState == 7) {
        rot = -rot;
    }

    const std::pair<f32, f32>& entry = JMath::sincosTable_.mTable[static_cast<u16>(-rot) >> 5];
    f32 s = entry.first;
    f32 c = entry.second;

    f32 rotX = curDiff.y * c - curDiff.x * s;
    f32 rotY = curDiff.y * s + curDiff.x * c;

    field_0x2c0 = field_0x2c8 + rotX;
    field_0x2c4 = field_0x2cc + rotY;
}

void cls_804A6708::fn_8021DFAC(s32 newState) {
    switch (newState) {
    case 1:
        fn_801F3604();
        if (mFlags & 0x2000) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) == field_0x4) {
                    JGeometry::TVec3f pos(getPlayerPos_8023E724(i)->x, getPlayerPos_8023E724(i)->y, 0.0f);
                    for (s32 j = 0; j < 4; j++) {
                        spawnEmitter_8013CC50(field_0x4, &pos, lbl_8046A78C[i][j], 0, 0, NULL, NULL, true, 1.0f);
                    }
                    fn_80230FD0(i, 0x20063);
                }
            }
        }
        break;

    case 2:
        if (mFlags & 0x2000) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) == field_0x4) {
                    JGeometry::TVec3f pos(getPlayerPos_8023E724(i)->x, getPlayerPos_8023E724(i)->y, 0.0f);
                    fn_80230498(i, &pos, fn_80230CFC(i) + 0x3c, 0xFF, -0x80, -0x80, true);
                }
            }
        }
        break;

    case 3:
        if (!(mFlags & 0x40000) && (mFlags & 0x2000)) {
            fn_80230534(false);
            mFlags |= 0x40000;
        }
        fn_801F3668();
        break;

    case 4:
        fn_801F3604();
        field_0x2bc = 1.0f;
        {
            bool usePlayer = false;
            if (field_0x2b4 >= 0 && (mFlags & 0x30000)) {
                usePlayer = true;
            }
            if (usePlayer) {
                const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2b4);
                field_0x2c8 = pPos->x;
                field_0x2cc = pPos->y;
            } else {
                field_0x2c8 = mSpawnPos.x;
                field_0x2cc = mSpawnPos.y;
            }
        }
        {
            const std::pair<f32, f32>& entry = JMath::sincosTable_.mTable[0x600];
            field_0x2c0 = field_0x2c8 + entry.first * field_0x2bc;
            field_0x2c4 = field_0x2cc + entry.second * field_0x2bc;
            mPos.x = field_0x2c0;
            mPos.y = field_0x2c4;
            mPos.z = 0.0f;
        }
        {
            bool flag = false;
            if (mState == 9) {
                flag = true;
            }
            mEmitter2D0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, 0, -1, NULL, NULL, flag, 1.0f);
            mEmitter2D4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, -1, NULL, NULL, flag, 1.0f);
        }
        if (mEmitter2D0 != NULL) {
            mEmitter2D0->setGlobalEnvColor(lbl_8046A7AC[field_0x2ac].r, lbl_8046A7AC[field_0x2ac].g, lbl_8046A7AC[field_0x2ac].b);
        }
        if (mFlags & 0x2000) {
            startSound_801F0E34(0x40073, 0);
        }
        break;

    case 6:
        if (mFlags & 0x2000) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) == field_0x4) {
                    s32 dir;
                    if (mFlags & 0x4000) {
                        dir = 3;
                    } else if (fn_8022F514(i)) {
                        dir = fn_80230CFC(fn_8022F3DC(i));
                    } else {
                        dir = fn_801CD354(fn_801D0020(&mPos, i, false));
                    }
                    fn_80230BF4(i, dir);
                    JGeometry::TVec3f pos(getPlayerPos_8023E724(i)->x, getPlayerPos_8023E724(i)->y, 0.0f);
                    fn_80230498(i, &pos, dir + 0x3c, 0xFF, -0x80, -0x80, true);
                }
            }
        }
        break;

    case 7:
        field_0x2bc = 18.0f;
        {
            bool usePlayer = false;
            if (field_0x2b4 >= 0 && (mFlags & 0x30000)) {
                usePlayer = true;
            }
            if (usePlayer) {
                const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2b4);
                field_0x2c8 = pPos->x;
                field_0x2cc = pPos->y;
            } else {
                field_0x2c8 = mSpawnPos.x;
                field_0x2cc = mSpawnPos.y;
            }
        }
        field_0x2c0 = mPos.x;
        field_0x2c4 = mPos.y;
        if (!(mFlags & 0x40000) && (mFlags & 0x2000)) {
            fn_80230534(false);
            mFlags |= 0x40000;
        }
        break;

    case 8:
        field_0x250 = lbl_8053FD50[field_0x2ac];
        if (!(mFlags & 0x40000) && (mFlags & 0x2000)) {
            fn_80230534(false);
            mFlags |= 0x40000;
        }
        break;

    case 10:
        break;
    }

    mStateTimer = 0;
    mState = newState;
}

void cls_804A6708::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (mStateTimer >= 5) {
            fn_8021DFAC(1);
        }
        break;

    case 1:
        if (mStateTimer >= 45) {
            fn_8021DFAC(2);
        }
        break;

    case 2:
        if (mStateTimer >= 45) {
            field_0x2b4 = fn_801CCE3C(&mPos, field_0x4);
            if (field_0x2b4 < 0) {
                field_0x2b4 = 0;
            }
            fn_8021DFAC(5);
        }
        break;

    case 3: {
        field_0x2b4 = fn_801D05F8(&mPos, field_0x2ad * 16.0f);
        if (field_0x2b4 < 0) {
            break;
        }
        bool canSee = true;
        if (field_0x2b0 != 0) {
            canSee = getGameView_802D6A9C()->fn_802D800C(field_0x2b0);
        }
        if (canSee && fn_8023077C()) {
            fn_8021DFAC(4);
        }
        break;
    }

    case 4:
        if (mStateTimer >= 120) {
            fn_8021DFAC(5);
        } else {
            fn_8021E4F8();
        }
        break;

    case 5:
        if (mFlags & 0x2000) {
            if ((mFlags & 0x4000) && !fn_8021F064()->fn_8021F094()) {
                break;
            }
            fn_8022FE80(field_0x2b4, field_0x4, mSerial, false, false);
            if (field_0x2b4 == fn_8022FD8C(mSerial)) {
                JGeometry::TVec3f offset(0.0f, -10.0f, 0.0f);
                if (fn_8038C678(field_0x2b4, field_0x2a8, mSerial, field_0x4, offset, 0x4002)) {
                    if (mFlags & 0x4000) {
                        fn_8021F064()->mFlags |= 1;
                    } else {
                        fn_8021DFAC(6);
                    }
                }
            }
        }
        break;

    case 6:
        if (mFlags & 0x2000) {
            fn_8022FE80(field_0x2b4, field_0x4, mSerial, false, false);
            if (fn_803865B8(field_0x2b4) && fn_80384C58(field_0x2b4, mSerial)) {
                if (mFlags & 0x4000) {
                    fn_8021F064()->mFlags |= 2;
                } else if (mFlags & 0x8000) {
                    fn_8021DFAC(10);
                } else {
                    fn_8021DFAC(7);
                }
            }
        }
        break;

    case 7:
        if (mStateTimer >= 120) {
            fn_8021DFAC(3);
        } else {
            fn_8021E4F8();
        }
        break;

    case 8:
        if (mStateTimer >= field_0x250) {
            fn_8021DFAC(9);
        }
        break;

    case 9:
        if (mPos.z >= 350.0f) {
            fn_8021DFAC(3);
        }
        break;

    case 10:
        if (mStateTimer >= 240) {
            fn_8021DFAC(3);
        } else if (mStateTimer == 120) {
            if (!(mFlags & 0x40000) && (mFlags & 0x2000)) {
                fn_80230534(false);
                mFlags |= 0x40000;
            }
        }
        break;
    }
}

void cls_804A6708::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    if (mState == 9) {
        f32 alpha = (mPos.z - 268.0f) * -0.004901961f;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        } else if (alpha < 0.0f) {
            alpha = 0.0f;
        }
        field_0x2b8 = alpha * 255.0f;
    } else {
        field_0x2b8 = 255.0f;
    }
    // FIXME: 96.53%. missing/extra: the original emits beq; b over setting the bool flag to 1.
    bool flag1 = false;
    if (mState == 9) {
        flag1 = true;
    }
    setEmitterPos_8013CB44(mEmitter2D0, &mPos, 0, -1, flag1);

    bool flag2 = false;
    if (mState == 9) {
        flag2 = true;
    }
    setEmitterPos_8013CB44(mEmitter2D4, &mPos, 0, -1, flag2);
    if (mEmitter2D0 != NULL) {
        mEmitter2D0->setGlobalAlpha((u8)field_0x2b8);
    }
    if (mEmitter2D4 != NULL) {
        mEmitter2D4->setGlobalAlpha((u8)field_0x2b8);
    }
}

void cls_804A6708::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    switch (mState) {
    case 4:
        field_0x3c.z = 0.15f;
        break;
    case 7:
        field_0x3c.z = -0.15f;
        break;
    case 9:
        field_0x3c.z = 0.75f;
        break;
    }

    switch (mState) {
    case 4:
    case 7: {
        JGeometry::TVec2f vel;
        fn_801D01A0((const JGeometry::TVec3f*)&field_0x2c0, &vel, FLT_MAX);
        field_0x3c.x = vel.x;
        field_0x3c.y = vel.y;
        break;
    }
    case 10:
        field_0x3c.x = 0.116666675f;
        field_0x3c.y = -0.4f;
        break;
    case 9:
        field_0x3c.y = -0.75f;
        break;
    }
}

void cls_804A6708::vfunc_0x98() {
    EnemyBase_804A4808::vfunc_0x98();
    if (mState == 9 && mPos.z > 350.0f) {
        mPos.z = 350.0f;
    }
}

void cls_804A6708::vfunc_0xA0() {
    if (mState == 3) {
        return;
    }
    if ((mFlags & 0x2000) && !(mFlags & 0x40000)) {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
    }
    if (!(mFlags & 0x10000)) {
        JGeometry::TVec3f focus(field_0x2c8, field_0x2cc - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
    }
}

// FIXME: 91.05%. scheduling / regalloc: original emits beq; b over setting the state 9 flag.
void cls_804A6708::draw() {
    if (mState == 3) {
        return;
    }
    if (!(field_0x2b8 > 0.0f)) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = (u8)field_0x2b8;
    bool flag = false;
    if (mState == 9) {
        flag = true;
    }
    params.field_0x20 = flag;
    fn_801F02BC(lbl_8046A7D8, lbl_8046A7C0[field_0x2ac] + mAnim.mCel, &params, 0x407, 0, false, 1.0f, 1.0f);
}

bool cls_804A6708::fn_8021D794() {
    return mState == 5;
}

void cls_804A6708::fn_8021D764() {
    if (mState != 6) {
        fn_8021DFAC(6);
    }
}

void cls_804A6708::fn_8021D734() {
    if (mState != 8) {
        fn_8021DFAC(8);
    }
}
