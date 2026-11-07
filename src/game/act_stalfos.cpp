#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_stalfos.h"

s32 lbl_804A0C10[24] = {
    0,  1,  0,  2,  3,  2,   //
    6,  7,  6,  8,  9,  8,   //
    12, 13, 12, 14, 15, 14,  //
    18, 19, 18, 20, 21, 20,  //
};
s32 lbl_804A0C70[6] = {3, 10, 3, 3, 10, 3};
s32 lbl_804A0C88[16] = {
    4,  5,  4,  0,   //
    10, 11, 10, 6,   //
    16, 17, 16, 12,  //
    22, 23, 22, 18,  //
};
s32 lbl_804A0CC8[4] = {2, 10, 2, 10};

// FIXME: 99.37%, missing/extra. The original has one more beq (0x224) in the
// WIZR loop's null check on the actor; the natural single check compiles without it.
void Stalfos_804A0CD8::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(-4.0f, -4.0f, 4.0f, 4.0f);
    field_0x80.set(-8.0f, 0.0f, 8.0f, 8.0f);
    field_0x264 = (field_0x90 >> 4) & 1;
    if (field_0x264 != 0) {
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
    }
    field_0x90 &= 0xF;
    field_0x94 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    mTimers[3] = 0;
    mTimers[4] = 0;
    field_0x268 = 0;
    field_0x258 = 0;
    field_0x278.x = 1.0f;
    field_0x278.y = 1.0f;
    field_0x278.z = 1.0f;
    field_0xb0 = 0x42205;
    field_0x98 = 3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    if (viewLayerFlag(field_0x4)) {
        field_0x98 &= 1;
    }
    field_0x250 = field_0x98;
    field_0x26c.x = 0.0f;
    field_0x26c.y = 0.0f;
    field_0x26c.z = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x260 = mLife;
    field_0x25c = field_0x260;
    switch (field_0x90) {
    case 3:
        mTimers[2] = 30;
        field_0xb0 = 0;
        field_0x90 = 0;
        break;
    case 5: {
        field_0x260 = 4;
        field_0x25c = 4;
        field_0x190 = 0;
        mLife = 1000;
        ActorBase_804A5770* actors[ACTOR_MANAGER_MAX_ACTORS];
        s32 count = getActorManager_801F856C()->fn_801F7818(ACTOR_WIZR, field_0x4, actors, -1);
        for (int i = 0; i < count; i++) {
            ActorBase_804A5770* actor = actors[i];
            if (actor != NULL && actor->field_0x90 == 1) {
                field_0x230 = 0;
                return;
            }
        }
        field_0x230 = 7;
        break;
    }
    case 4:
        field_0xb0 = 0;
        break;
    }
    field_0x230 = 0;
}

// FIXME: 99.70%, scheduling+regalloc. The two mPos.z == 0.0f style float compares load
// the 0.0f literal before/after the field (0x1A0, 0x4B0), one add has swapped operands
// (0x8BC) and r28/r30 are swapped around the fn_801F681C call (0x970).
void Stalfos_804A0CD8::update() {
    u16 angle;
    s32 dir;
    s32 playerIdx = -1;
    f32 dist = 0.0f;
    JGeometry::TVec3f push;

    if (field_0x90 == 4 || (field_0x90 == 5 && mLife == 1000)) {
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
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        if (field_0x90 == 4) {
            field_0x90 = 0;
        } else {
            mLife = 10000;
        }
    }

    if (field_0x90 == 5 && mLife != 1000) {
        mLife = 10000;
    }

    if (field_0x90 == 5 && field_0x94 != 0 && field_0x230 != 8) {
        field_0x230 = 7;
    }

    if (fn_801F3D94(1)) {
        fn_801F2618(NULL, false);
        return;
    }

    if (mTimers[2] == 0 && field_0x230 != 6 && field_0x230 != 8) {
        // Unsigned compare (cmplwi) in the original; field_0xb0 is s32 in ActorBase.
        if ((u32)field_0xb0 == 0) {
            field_0xb0 = 0x42205;
        }
        if ((field_0x90 == 0 || field_0x230 != 4) && mPos.z == 0.0f && fn_801F305C()) {
            if (field_0x90 == 5) {
                field_0x25c -= 10000 - mLife;
                if (field_0x25c <= 0) {
                    field_0x26c.z = 0.0f;
                    field_0x26c.y = 0.0f;
                    field_0x26c.x = 0.0f;
                    field_0x230 = 5;
                }
                if (field_0x25c > 0) {
                    startSound_801F0E34(0x4009D, 0);
                } else {
                    startSound_801F0E34(0x4009E, 0);
                }
            }
            fn_801F2618(NULL, false);
            return;
        }
    }

    if (viewLayerFlag(field_0x4)) {
        field_0x26c.y = 3.0f;
    }
    addCalc2_801F71A4(&field_0x3c.x, field_0x26c.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x26c.y, 0.3f, 1.0f);
    mPos += field_0x3c;

    if (mTimers[2] == 0 && field_0x230 != 4 && field_0x230 != 8) {
        fn_801F2B7C(-100, -1, 1);
    }

    u8 blocked = fn_801F2618(&push, false);
    fn_801EEC98(mSerial, &angle, &dir, &dist, &playerIdx, 0, 0);

    if ((field_0x90 == 1 || field_0x90 == 2) && field_0x230 == 1 && dist < 50.0f) {
        for (int i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getField_0x4() == getPlayerLayer_8023DE58(i) &&
                !fn_80230188(i) && getPlayerLayer_8023DE58(i) != 8 && fn_8022C3AC(i)) {
                field_0x230 = 3;
                break;
            }
        }
    }

    switch (field_0x230) {
    case 0: {
        if (field_0x90 == 5) {
            ActorBase_804A5770* actors[ACTOR_MANAGER_MAX_ACTORS];
            if (getActorManager_801F856C()->fn_801F7818(ACTOR_WIZR, field_0x4, actors, -1) == 0) {
                field_0x230 = 7;
                break;
            }
        }
        if (mTimers[1] == 0) {
            mTimers[1] = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
        }
        if (viewLayerFlag(field_0x4) && blocked && push.y != 0.0f) {
            blocked = 0;
        }
        if (blocked || gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.3f || dir < 0) {
            if (!viewLayerFlag(field_0x4)) {
                field_0x98 ^= 2;
            }
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x98 ^= 1;
            }
        } else {
            if (!viewLayerFlag(field_0x4)) {
                field_0x98 = dir;
            } else if ((dir & 2) == 0) {
                field_0x98 = dir;
            } else if (angle < 0x8000) {
                field_0x98 = 0;
            } else {
                field_0x98 = 1;
            }
        }
        if (field_0x250 != field_0x98) {
            mTimers[0] = 0;
            field_0x24c = 0;
        }
        field_0x250 = field_0x98;
        switch (field_0x98) {
        case 0:
            field_0x26c.x = 0.7f;
            field_0x26c.y = 0.0f;
            field_0x26c.z = 0.0f;
            break;
        case 1:
            field_0x26c.x = -0.7f;
            field_0x26c.y = 0.0f;
            field_0x26c.z = 0.0f;
            break;
        case 2:
            field_0x26c.x = 0.0f;
            field_0x26c.y = 0.7f;
            field_0x26c.z = 0.0f;
            break;
        case 3:
            field_0x26c.x = 0.0f;
            field_0x26c.y = -0.7f;
            field_0x26c.z = 0.0f;
            break;
        }
        field_0x230 = 1;
        // fallthrough
    }
    case 1:
        if (mTimers[0] == 0) {
            field_0x258 = lbl_804A0C10[field_0x98 * 6 + field_0x24c];
            mTimers[0] = lbl_804A0C70[field_0x24c];
            field_0x24c++;
            if (field_0x24c > 5) {
                field_0x24c = 0;
            }
        }
        if (mTimers[1] == 0) {
            field_0x26c.x = 0.0f;
            field_0x26c.y = 0.0f;
            field_0x26c.z = 0.0f;
            field_0x230 = 0;
        }
        break;
    case 2:
        if (mTimers[1] == 0) {
            field_0x230 = 0;
        }
        break;
    case 3:
        if (dir >= 0) {
            if (!viewLayerFlag(field_0x4)) {
                field_0x98 = dir;
            } else if ((dir & 2) == 0) {
                field_0x98 = dir;
            } else if (angle < 0x8000) {
                field_0x98 = 0;
            } else {
                field_0x98 = 1;
            }
        }
        field_0x26c.x = 2.0f * JMASinShort(angle + 0x8000);
        if (!viewLayerFlag(field_0x4)) {
            field_0x3c.z = 5.0f;
            field_0x26c.y = 2.0f * JMACosShort(angle + 0x8000);
        } else {
            field_0x3c.y = -7.0f;
        }
        mTimers[0] = 0;
        field_0x24c = 0;
        startSound_801F0E34(0x4009C, 0);
        field_0x230 = 4;
        break;
    case 4:
        if (!viewLayerFlag(field_0x4)) {
            field_0x3c.z -= 0.4f;
        }
        if (field_0x24c < 4 && mTimers[0] == 0) {
            field_0x258 = lbl_804A0C88[field_0x24c + field_0x98 * 4];
            mTimers[0] = lbl_804A0CC8[field_0x24c];
            field_0x24c++;
        }
        if ((!viewLayerFlag(field_0x4) && mPos.z < 0.0f) || (viewLayerFlag(field_0x4) && push.y < 0.0f)) {
            if (field_0x90 == 2) {
                ActorBase_804A5770* rock = getActorManager_801F856C()->findActorBySerial_801F666C(
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OKTM, field_0x4, &mPos, 2, -1, -1));
                if (rock != NULL) {
                    JGeometry::TVec3f vel;
                    vel.x = 1.8f * JMASinShort(angle);
                    vel.y = 1.8f * JMACosShort(angle);
                    rock->field_0x3c = vel;
                }
            }
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x26c.z = 0.0f;
            field_0x26c.y = 0.0f;
            field_0x26c.x = 0.0f;
            mTimers[1] = (s32)(5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 5;
            field_0x230 = 2;
        }
        break;
    case 5:
        field_0x24c = 0;
        field_0x250 = 0;
        mTimers[0] = 0;
        mTimers[1] = 0;
        mTimers[3] = 750;
        field_0x25c = field_0x260;
        field_0x98 = 2;
        field_0xb0 = 0;
        field_0x230 = 6;
        // fallthrough
    case 6:
        switch (field_0x250) {
        case 0:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x35C, 1, 1, NULL, NULL, true, fn_801EE4F0());
            mTimers[4] = 690;
            field_0x250 = 1;
            break;
        case 1:
            field_0x258 = field_0x98 + 28;
            if (mTimers[4] <= 30) {
                if (mPos.z <= 0.0f) {
                    mPos.z = 0.0f;
                    if (mTimers[3] == 0) {
                        field_0x26c.z = 0.0f;
                        field_0x26c.y = 0.0f;
                        field_0x26c.x = 0.0f;
                        field_0x3c.z = 0.0f;
                        field_0x3c.y = 0.0f;
                        field_0x3c.x = 0.0f;
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x35C, 1, 1, NULL, NULL, true, fn_801EE4F0());
                        field_0x250 = 0;
                        field_0x230 = 0;
                    } else {
                        s32 randAngle = 65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                        field_0x3c.z = 4.0f;
                        field_0x26c.x = 1.3f * JMASinShort(randAngle);
                        field_0x26c.y = 1.3f * JMACosShort(randAngle);
                        field_0x98 = fn_801F681C(randAngle);
                    }
                } else {
                    field_0x3c.z -= 1.0f;
                }
            }
            break;
        }
        break;
    case 7:
        field_0x24c = 0;
        mTimers[0] = 0;
        mTimers[4] = 130;
        field_0x26c.z = 0.0f;
        field_0x26c.y = 0.0f;
        field_0x26c.x = 0.0f;
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x35C, 1, 1, NULL, NULL, true, fn_801EE4F0());
        field_0x98 = 2;
        field_0xb0 = 0;
        field_0x230 = 8;
        // fallthrough
    case 8:
        field_0x258 = field_0x98 + 28;
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
            if (field_0x3c.z < -10.0f) {
                field_0x3c.z = -10.0f;
            }
        } else {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        if (mTimers[4] == 0) {
            vfunc_0x24();
        }
        break;
    }

    for (s32 i = 0; i < 5; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

void Stalfos_804A0CD8::draw() {
    s32 flag = 0;
    JGeometry::TVec3f oldPos;
    oldPos.set(mPos);
    if (mTimers[4] > 30 && mTimers[4] < 130) {
        if ((mTimers[4] & 2) == 0) {
            mPos.x += 2.0f;
        } else {
            mPos.x -= 2.0f;
        }
    }
    if (field_0x264 != 0) {
        flag = 1;
    }
    fn_801F02BC("enemy_both2_sob.bin", field_0x258 + 0x158, NULL, 7, flag, false, 1.0f, 1.0f);
    mPos.set(oldPos);
}

Stalfos_804A0CD8::~Stalfos_804A0CD8() {}
