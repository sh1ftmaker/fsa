#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/act/act_bomb.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_dodongo.h"

extern const f32 lbl_804A3108[];
extern const f32 lbl_804A31C8[];
extern const char* lbl_804A3208[];
extern const s32 lbl_804689C0[];
u16 lbl_8053A110[] = { 0x168, 0x167, 0x166, 0x169 };
extern s32 lbl_8053ABE0;

void Dodongo_804A3334::fn_801AE5AC() {
    if (mTimers[0] != 0) {
        return;
    }

    switch (field_0x290) {
    case 0:
        mTimers[0] = 6;
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98 * 3 + field_0x24c;
        field_0x24c++;
        if (field_0x24c > 2) {
            field_0x24c = 0;
            startSound_801F0E34(0x50029, 0);
        }
        break;
    case 1:
        mTimers[0] = 9;
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98 * 3 + field_0x24c;
        if (field_0x24c == 0) {
            startSound_801F0E34(0x5002A, 0);
        }
        field_0x24c++;
        if (field_0x24c > 2) {
            field_0x24c = 2;
            field_0x298 = 1;
        }
        break;
    case 2:
        mTimers[0] = 9;
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98 * 2 + field_0x24c;
        field_0x24c++;
        if (field_0x24c > 1) {
            field_0x24c = 1;
            field_0x298 = 1;
        }
        break;
    case 3:
        field_0x258 = field_0x28c;
        field_0x258 += field_0x98;
        break;
    case 4:
        if (field_0x24c > 2) {
            field_0x24c = 3;
            field_0x298 = 1;
        } else {
            field_0x258 = field_0x28c;
            field_0x258 = field_0x258 + field_0x24c;
            mTimers[0] = 10;
            field_0x24c++;
        }
        break;
    }
}

void Dodongo_804A3334::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;

    field_0x234 = field_0x90 & 0xF;
    field_0x294 = (field_0x90 >> 4) & 0xF;

    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    mTimers[3] = 0;
    mTimers[4] = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    field_0x274[0] = 1.0f;
    field_0x274[1] = 1.0f;
    field_0x274[2] = 1.0f;
    field_0x274[3] = 1.0f;
    field_0x274[4] = 1.0f;
    field_0x274[5] = 1.0f;
    field_0x264 = -1;
    field_0x28c = 0;
    field_0x290 = -1;
    mTargetSerial = -1;
    field_0x298 = 0;

    field_0x98 = (s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x98 &= 3;

    setAnim(0);

    fn_801AE5AC();

    field_0x268.z = 0.0f;
    field_0x268.y = 0.0f;
    field_0x268.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    mLife = 10000;
    field_0x25c = 3;

    if (field_0x234 == 0 || field_0x234 == 2) {
        field_0xb8 = 1;
        field_0xb0 = 0x0A040004;
    }

    switch (field_0x234) {
    case 0:
        field_0x230 = 1;
        break;
    case 1:
        field_0x230 = 12;
        break;
    case 2:
        if (lbl_8053ABE0 == 0) {
            gAudioMgr_8053AF80->fn_80458F9C(0x3c);
            field_0x230 = 0;
            lbl_8053ABE0++;
        } else {
            field_0x230 = 1;
        }
        break;
    }
}

// FIXME: 95.30 %. missing/extra + reloc-name: ours has 14 fewer instrs (1216 vs 1230). Register allocation and minor instruction scheduling differences.
void Dodongo_804A3334::fn_801ACFF4() {
    const f32* data = lbl_804A3108;
    f32 posY = mPos.y;
    f32 posX = mPos.x;
    f32 boxIX = mBox.i.x;
    f32 boxIY = mBox.i.y;
    f32 boxFX = mBox.f.x;
    f32 boxFY = mBox.f.y;
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    boxIX += posX;
    boxIY += posY;
    boxFX += posX;
    boxFY += posY;

    f32 scale = 1.0f;
    if (field_0x294 == 1) {
        scale = 1.75f;
    }

    const f32* boxData = &data[16 + field_0x98 * 4];
    mBox.i.x = scale * boxData[0];
    mBox.i.y = scale * boxData[1];
    mBox.f.x = scale * boxData[2];
    mBox.f.y = scale * boxData[3];
    field_0x80 = mBox;

    addCalc2_801F71A4(&field_0x3c.x, field_0x268.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x268.y, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x274[0], field_0x274[3], 0.1f, 0.1f);
    fn_801F3D94(0);
    field_0x274[1] = field_0x274[0];
    mPos += field_0x3c;

    if (mTimers[3] == 0) {
        if (field_0x230 == 2 || field_0x230 == 5) {
            fn_801F2B7C(-100, -1, 1);
        }
    }
    fn_801EF510();
    u8 hitWall = fn_801F2618(NULL, false);

    f32 centerX = (boxIX - boxFX) * 0.5f + boxFX;
    f32 centerY = (boxIY - boxFY) * 0.5f + boxFY;

    for (int i = 0; i < 4; i++) {
        if (fn_801F285C(NULL, NULL, NULL, i) >= 0) {
            JGeometry::TVec2f center(centerX, centerY);
            if (fn_80237948(0, &center)) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x400D1, 0);
            }
        }
    }

    switch (field_0x230) {
    case 0:
        gAudioMgr_8053AF80->fn_80458FF0(0x10004, 0, 0);
        field_0x230 = 1;
        // fallthrough
    case 1:
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        switch (field_0x98) {
        case 0:
            field_0x268.x = 0.7f;
            field_0x268.y = 0.0f;
            field_0x268.z = 0.0f;
            break;
        case 1:
            field_0x268.x = -0.7f;
            field_0x268.y = 0.0f;
            field_0x268.z = 0.0f;
            break;
        case 2:
            field_0x268.x = 0.0f;
            field_0x268.y = 0.7f;
            field_0x268.z = 0.0f;
            break;
        case 3:
            field_0x268.x = 0.0f;
            field_0x268.y = -0.7f;
            field_0x268.z = 0.0f;
            break;
        }
        mTimers[1] = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
        field_0xbc = 3;
        if (field_0x294 == 1) {
            field_0xbc = 4;
        }
        if (mTimers[3] == 0) {
            field_0x250++;
            if (field_0x250 > 3) {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    s32 dir = -1;
                    s32 r28 = field_0x98;
                    field_0x250 = 0;
                    if (field_0x294 == 1) {
                        r28 = 3;
                        if (field_0x258 < 3) {
                            r28 = 0;
                        }
                        if (field_0x258 >= 3 && field_0x258 <= 5) {
                            r28 = 1;
                        }
                        if (field_0x258 >= 6 && field_0x258 <= 8) {
                            r28 = 2;
                        }
                    }
                    fn_801EEC98(mSerial, 0, &dir, 0, 0, 0, 0);
                    if (dir >= 0) {
                        field_0x98 = dir;
                        mTimers[0] = 2;
                    }
                    if (field_0x294 == 0 || field_0x98 != 3) {
                        field_0x230 = 4;
                    } else {
                        field_0x230 = 10;
                        break;
                    }
                    if (field_0x294 == 1) {
                        if ((r28 == 1 && field_0x98 == 0) || (r28 == 0 && field_0x98 == 1)) {
                            setAnim(4);
                        }
                    }
                    field_0x230 = 3;
                    break;
                }
            }
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
                field_0x230 = 10;
                break;
            }
        }
        setAnim(0);
        field_0x230 = 2;
        // fallthrough
    case 2:
        if (hitWall != 0 || mTimers[1] == 0) {
            field_0x98 ^= 2;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x98 ^= 1;
            }
            JGeometry::TVec3f savedPos;
            savedPos.x = mPos.x;
            savedPos.y = mPos.y;
            savedPos.z = mPos.z;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            switch (field_0x98) {
            case 0:
                field_0x268.x = 16.0f;
                field_0x268.y = 0.0f;
                field_0x268.z = 0.0f;
                break;
            case 1:
                field_0x268.x = -16.0f;
                field_0x268.y = 0.0f;
                field_0x268.z = 0.0f;
                break;
            case 2:
                field_0x268.x = 0.0f;
                field_0x268.y = 16.0f;
                field_0x268.z = 0.0f;
                break;
            case 3:
                field_0x268.x = 0.0f;
                field_0x268.y = -16.0f;
                field_0x268.z = 0.0f;
                break;
            }
            mPos += field_0x268;
            if (fn_801F2618(NULL, false)) {
                field_0x98 ^= 1;
            }
            mPos.x = savedPos.x;
            mPos.y = savedPos.y;
            mPos.z = savedPos.z;
            if (field_0x294 == 1) {
                s32 r3 = 3;
                if (field_0x258 < 3) {
                    r3 = 0;
                }
                if (field_0x258 >= 3 && field_0x258 <= 5) {
                    r3 = 1;
                }
                if (field_0x258 >= 6 && field_0x258 <= 8) {
                    r3 = 2;
                }
                if ((r3 == 1 && field_0x98 == 0) || (r3 == 0 && field_0x98 == 1)) {
                    setAnim(4);
                    field_0x230 = 3;
                    break;
                }
            }
            mTimers[0] = 2;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            switch (field_0x98) {
            case 0:
                field_0x268.x = 0.7f;
                field_0x268.y = 0.0f;
                field_0x268.z = 0.0f;
                break;
            case 1:
                field_0x268.x = -0.7f;
                field_0x268.y = 0.0f;
                field_0x268.z = 0.0f;
                break;
            case 2:
                field_0x268.x = 0.0f;
                field_0x268.y = 0.7f;
                field_0x268.z = 0.0f;
                break;
            case 3:
                field_0x268.x = 0.0f;
                field_0x268.y = -0.7f;
                field_0x268.z = 0.0f;
                break;
            }
            field_0x230 = 1;
            break;
        }
        break;
    case 3:
        field_0x268.z = 0.0f;
        field_0x268.y = 0.0f;
        field_0x268.x = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        if (field_0x298 == 0) {
            break;
        }
        field_0x230 = 4;
        // fallthrough
    case 4:
        field_0x268.z = 0.0f;
        field_0x268.y = 0.0f;
        field_0x268.x = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        setAnim(1);
        if (field_0x298 == 0) {
            break;
        }
        mTimers[1] = 120;
        field_0x230 = 5;
        // fallthrough
    case 5:
        if (mTimers[1] != 0) {
            if (mTimers[1] == 1) {
                JGeometry::TVec3f spawnPos;
                spawnPos.x = mPos.x;
                spawnPos.y = mPos.y;
                spawnPos.z = mPos.z;
                if (field_0x294 == 1) {
                    const f32* p = &data[8];
                    spawnPos.x += p[field_0x98 * 2];
                    spawnPos.y += p[field_0x98 * 2 + 1];
                } else {
                    const f32* p = &data[0];
                    spawnPos.x += p[field_0x98 * 2];
                    spawnPos.y += p[field_0x98 * 2 + 1];
                }
                mTargetSerial = getActorManager_801F856C()->spawnActor_801F9484('DDGO', field_0x4, &spawnPos, (field_0x294 << 4) | 1, -1, -1);
                if (mTargetSerial >= 0) {
                    ActorBase_804A5770* child = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
                    if (child != NULL) {
                        mTimers[2] = 120;
                        child->field_0x98 = field_0x98;
                    } else {
                        mTargetSerial = -1;
                    }
                }
                break;
            } else {
                f32 f4 = 1.0f;
                if (field_0x294 == 1) {
                    f4 = 1.2f;
                }
                const f32* p = &data[32 + field_0x98 * 4];
                mBox.i.x = f4 * p[0];
                mBox.i.y = f4 * p[1];
                mBox.f.x = f4 * p[2];
                mBox.f.y = f4 * p[3];
                field_0x264 = fn_801F2718('HBMB');
                if (field_0x264 >= 0) {
                    ActorBase_804A5770* bombBase = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264);
                    if (bombBase != NULL && bombBase->field_0x94 == 0) {
                        if (field_0x294 != 1 || bombBase->field_0x90 == 8 || bombBase->field_0x90 == 9) {
                            f32 dx = bombBase->mPos.x - mPos.x;
                            f32 dy = bombBase->mPos.y - mPos.y;
                            s16 angle = (s16)(JMath::atanTable_.atan2_(dx, dy) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                            if (fn_801F681C((u16)angle) == field_0x98) {
                                Bomb_8049F784* bomb = static_cast<Bomb_8049F784*>(bombBase);
                                bomb->field_0x280 = 1;
                                field_0x230 = 6;
                                break;
                            }
                        }
                    }
                }
                break;
            }
        } else {
            if (mTimers[2] == 10) {
                if (mTargetSerial >= 0) {
                    ActorBase_804A5770* child = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
                    if (child != NULL) {
                        mTargetSerial = -1;
                        child->mLife = 0;
                    }
                }
            }
            if (mTimers[2] == 0) {
                field_0x250 = 0;
                field_0x230 = 8;
            }
            break;
        }
        break;
    case 6:
        setAnim(2);
        if (field_0x264 >= 0) {
            ActorBase_804A5770* bombBase = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264);
            if (bombBase != NULL && bombBase->field_0x94 == 0) {
                Bomb_8049F784* bomb = static_cast<Bomb_8049F784*>(bombBase);
                f32 f4 = 1.0f;
                f32 bombScale = bomb->field_0x274;
                JGeometry::TVec3f bombPos;
                bombPos.x = bomb->mPos.x;
                bombPos.y = bomb->mPos.y;
                bombPos.z = bomb->mPos.z;
                JGeometry::TVec3f mouthPos;
                mouthPos.x = mPos.x;
                mouthPos.y = mPos.y;
                mouthPos.z = mPos.z;
                if (field_0x294 == 1) {
                    f4 = 1.5f;
                }
                const f32* p = &data[field_0x98 * 2];
                mouthPos.x += f4 * p[0];
                mouthPos.y += f4 * p[1];
                if ((field_0x98 & 1) == 0) {
                    mouthPos.y += 12.0f * f4;
                }
                addCalc2_801F71A4(&bombPos.x, mouthPos.x, 0.3f, 2.0f);
                addCalc2_801F71A4(&bombPos.y, mouthPos.y, 0.3f, 2.0f);
                addCalc2_801F71A4(&bombScale, 0.0f, 0.5f, 0.5f);
                bomb->mPos = bombPos;
                bomb->field_0x274 = bombScale;
            }
        }
        if (field_0x298 != 0) {
            if (field_0x264 >= 0) {
                ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x264);
                if (bomb != NULL && bomb->field_0x94 == 0) {
                    bomb->mAlive = false;
                }
            }
            startSound_801F0E34(0x5002B, 0);
            mTimers[1] = 60;
            field_0x230 = 7;
            // fallthrough
        } else {
            break;
        }
    case 7:
        if (mTimers[1] == 1) {
            field_0x274[0] = 2.0f;
            field_0x274[1] = 2.0f;
            field_0x274[2] = 2.0f;
            setAnim(3);
            field_0x25c--;
            if (field_0x25c <= 0) {
                s32 type = 7;
                if (field_0x294 == 1) {
                    type = 13;
                }
                s32 serial = getActorManager_801F856C()->spawnActor_801F9484('HBMB', field_0x4, &mPos, type, -1, -1);
                if (serial >= 0) {
                    ActorBase_804A5770* bombBase = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (bombBase != NULL) {
                        Bomb_8049F784* bomb = static_cast<Bomb_8049F784*>(bombBase);
                        if (field_0x294 == 1) {
                            bomb->field_0x27c = 2.5f;
                        } else {
                            bomb->field_0x27c = 1.5f;
                        }
                    }
                }
                gGameRoot_8053AAF8->fn_80134DE4(1, 0);
            } else {
                startSound_801F0E34(0x40005, 0);
            }
        }
        if (mTimers[1] == 0) {
            f32 targetScale = (field_0x294 == 1) ? 2.0f : 1.0f;
            if (field_0x274[0] < 1.1f) {
                spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053A110[field_0x98], 0, 0, NULL, NULL, true, targetScale);
                if (field_0x25c > 0) {
                    field_0x250 = 0;
                    field_0x230 = 11;
                    mTimers[1] = 63;
                } else {
                    vfunc_0x24();
                }
            }
        }
        break;
    case 8:
        setAnim(2);
        field_0x230 = 9;
        // fallthrough
    case 9:
        if (field_0x298 != 0) {
            field_0x250 = 0;
            field_0x230 = 1;
        }
        break;
    case 10:
        field_0x268.z = 0.0f;
        field_0x268.y = 0.0f;
        field_0x268.x = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        mTimers[1] = 30;
        setAnim(0);
        fn_801AE5AC();
        field_0x230 = 11;
        // fallthrough
    case 11:
        if (mTimers[1] == 0) {
            if (field_0x290 != 0) {
                mTimers[3] = 100;
            }
            field_0x230 = 1;
        }
        break;
    }

    if (field_0x230 != 11) {
        fn_801AE5AC();
    }
}

// FIXME: 96.62 %. different: same length, 3 opcodes differ. Register allocation (f30/f31, r29/r30) and boxData indexing.
void Dodongo_804A3334::fn_801ACCF0() {
    f32 scale = 1.0f;
    if (field_0x294 == 1) {
        scale = 1.5f;
    }

    switch (field_0x230) {
    case 12:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1ac, 1, 1, NULL, NULL, true, scale);
        if (mEmitters[0] == NULL) {
            return;
        }
        mTimers[0] = 8;
        field_0x230 = 13;
        // fallthrough
    case 13:
        if (mEmitters[0] != NULL) {
            s32 rotX = 0;
            if (field_0x98 == 2) {
                rotX = 0x4000;
            }
            setEmitterPos_8013CB44(mEmitters[0], &mPos, 1, 1, true);
            JPAGetXYZRotateMtx(rotX, lbl_804689C0[field_0x98], -0x8000, mEmitters[0]->mGlobalRot);
            startSound_801F0E34(0x30011, 0);
        }

        if (mTimers[0] == 0) {
            const f32* boxData = &lbl_804A31C8[field_0x98 * 4];
            mBox.i.x = boxData[0] * scale;
            mBox.i.y = boxData[1] * scale;
            mBox.f.x = boxData[2] * scale;
            mBox.f.y = boxData[3] * scale;
            fn_801F2B7C(field_0x114, -1, 9);
            fn_801F2B20(5);

            JGeometry::TVec3f pos = mPos;
            if (field_0x254 == 0) {
                field_0x254 = 1;
                f32 stepX = (mBox.f.x - mBox.i.x) * 0.125f;
                f32 stepY = (mBox.f.y - mBox.i.y) * 0.125f;
                pos.x = mPos.x + mBox.i.x;
                for (s32 i = 0; i < 8; i++) {
                    pos.y = mPos.y + mBox.i.y;
                    for (s32 j = 0; j < 8; j++) {
                        if ((s32)fn_8022461C(&pos, field_0x4) == 0x21 || fn_8022517C(&pos, field_0x4) == 1) {
                            getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &pos, 0xE0C0, -1, -1);
                        }
                        pos.y += stepY;
                    }
                    pos.x += stepX;
                }
            }
        }

        if (mLife <= 0) {
            for (s32 i = 0; i < 2; i++) {
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->becomeInvalidEmitter();
                    mEmitters[i] = NULL;
                }
            }
            mAlive = false;
        }
        break;
    }
}

Dodongo_804A3334::~Dodongo_804A3334() {
    s32 serial;
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }

    serial = mTargetSerial;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            mTargetSerial = -1;
            actor->mLife = 0;
        }
    }

    if (field_0x234 == 2) {
        lbl_8053ABE0--;
    }
}

void Dodongo_804A3334::update() {
    switch (field_0x234) {
    case 0:
    case 2:
        fn_801ACFF4();
        break;
    case 1:
        fn_801ACCF0();
        break;
    }

    for (s32 i = 0; i < 5; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

void Dodongo_804A3334::fn_801AC930() {
    SpriteParams_801F5AE0 params;
    f32 scale = 1.0f;
    s32 texIdx = field_0x258;
    if (mTimers[3] & 2) {
        return;
    }

    if ((u32)texIdx <= 2 || (u32)(texIdx - 12) <= 2 || (u32)(texIdx - 24) <= 1 || texIdx == 32) {
        scale = -1.0f;
    }

    Mtx mtx;
    f32 scaleX = field_0x274[0] * scale;
    f32 scaleY = field_0x274[1];
    mtx[0][0] = scaleX;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scaleY;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scaleY;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;

    void* res;
    if (field_0x294 == 1) {
        s32 index = texIdx + 0x24;
        res = fn_804032E8('TIMG', lbl_804A3208[index]);
    } else {
        res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A3208[texIdx]);
    }
    fn_801EFD34(res, &params, 7, 0.0f, -2.0f, 0.0f);
}

void Dodongo_804A3334::draw() {
    switch (field_0x234) {
    case 0:
    case 2:
        fn_801AC930();
        break;
    }
}
