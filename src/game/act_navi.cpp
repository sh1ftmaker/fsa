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
#include "game/SpriteParams.h"
#include "game/StageFlag.h"
#include "game/tu_8021F064.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"

static const s16 lbl_8046A770[] = {
    0, 1,
    1, 1,
    2, 1,
    3, 1,
    4, 1,
    5, 1,
    -1, -1,
};

static const u16 lbl_8046A78C[16] = {
    0x0513, 0x0514, 0x0515, 0x0516,
    0x051B, 0x051C, 0x051D, 0x051E,
    0x050F, 0x0510, 0x0511, 0x0512,
    0x0517, 0x0518, 0x0519, 0x051A,
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

// FIXME: 96.8%. Only branch shapes differ: the original has bge; b over the stage >= 1 test (0x8021EA3C) and
// beq; b over li 1 for the two spawnEmitter_8013CC50 flag arguments, ours blt/bne; the .sdata2 literals are not split.
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
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
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
            // Flag local set in a branch, as in vfunc_0x9C.
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

// FIXME: 96.5%. Only the two setEmitterPos_8013CB44 flag arguments differ in branch shape (ours bne over li 1,
// the original beq; b over li 1), and the .sdata2 literals are not split.
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
    // The flag is a local set in a branch: the original compiles its mState == 9 as li/cmpwi/beq/b/li.
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
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;

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
        JGeometry::TVec2f vec;
        fn_801D01A0((const JGeometry::TVec3f*)&field_0x2c0, &vec, FLT_MAX);
        field_0x3c.x = vec.x;
        field_0x3c.y = vec.y;
        break;
    }
    case 10:
        field_0x3c.x = 0.116666675f;
        field_0x3c.y = -0.40000004f;
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

// FIXME: 91.05% (different: same length, 19 opcodes differ in SpriteParams_801F5AE0 register allocation).
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
    fn_801F02BC("enemy_both2_sob.bin", mAnim.mCel + lbl_8046A7C0[field_0x2ac], &params, 0x407, 0, false, 1.0f, 1.0f);
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
