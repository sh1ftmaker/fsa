#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/act/act_force_gem_spawner.h"

s32 lbl_804AB630[12] = {
    1, 5, 20, 50, 100, 150, 200, 300, 1000, 1500, 2000, 3000,
};


ForceGemSpawner_804AB660::~ForceGemSpawner_804AB660() {
    releaseEmitter_801EE434(mEmitter24c);
    releaseEmitter_801EE434(mEmitter250);
    releaseEmitter_801EE434(mEmitter254);
}

// FIXME(shotgun): 91.6 %, scheduling + reloc-name
void ForceGemSpawner_804AB660::init() {
    mEmitter24c = NULL;
    mEmitter250 = NULL;
    mEmitter254 = NULL;
    field_0x238 = field_0x90 & 0xFFFF;
    field_0x234 = 0;
    field_0x244 = 0.0f;
    field_0x248 = 1.0f;
    field_0x240 = 11;
    field_0x23c = (field_0x90 >> 20) & 0xF;
    switch ((field_0x90 >> 16) & 0xF) {
    case 1:
        field_0x230 = 1;
        break;
    case 2:
        field_0x238 = 0;
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x230 = 3;
        break;
    case 3:
        field_0x230 = 4;
        break;
    case 4:
    case 7:
        field_0x230 = 5;
        break;
    case 0:
    case 5:
    case 6:
    default:
        field_0x230 = 0;
        break;
    }
    if (field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
        if (vfunc_0x34()) {
            fn_801F3668();
        }
    }
}

// FIXME(shotgun): 76.59 %, missing/extra + reloc-name
// Drops a Force Gem ('RUPY') next to the actor with serial `serial` and throws it: along `angle`
// (plus some random spread) when `flag` is set, otherwise in a random direction.
void fn_8032A4FC(s32 serial, s32 angle, bool flag) {
    ActorBase_804A5770* src = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (src == NULL) {
        return;
    }
    JGeometry::TVec3f pos(src->mPos);
    pos.x += 8.0f;
    pos.y += 12.0f;
    pos.z = 0.0f;
    u32 kind = (u32)(2.5 * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    s32 idx = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, src->field_0x4, &pos, kind);
    if (idx < 0) {
        return;
    }
    ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(idx);
    if (gem == NULL) {
        return;
    }
    if (flag) {
        angle += (s32)(1280.0f - 4096.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        f32 vx = 2.0f * JMASSin(angle);
        f32 vy = 2.0f * JMASCos(angle);
        f32 vz = 3.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        gem->field_0x3c.set(vx, vy, vz);
    } else {
        f32 vz = 3.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        f32 vy = 2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        f32 vx = 2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        gem->field_0x3c.set(vx, vy, vz);
    }
}

// FIXME(shotgun): 51.38 %, missing/extra + reloc-name
void ForceGemSpawner_804AB660::update() {
    switch (field_0x230) {
    case 0: {
        u16 baseAngle = fn_801F6874(field_0x98);
        JGeometry::TVec3f vel(0.0f, 0.0f, 0.0f);
        for (s32 i = 0; i < 100 && field_0x238 > 0; i++) {
            s32 idx = field_0x240;
            for (; idx > 0; idx--) {
                if (field_0x238 >= lbl_804AB630[idx]) {
                    break;
                }
            }
            field_0x240 = idx;
            u32 kind = (u32)((field_0x240 + 1) * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            if (field_0x238 < 20) {
                if (field_0x238 > 5 && kind == 2) {
                    kind = 1;
                } else {
                    kind = 0;
                }
            }
            if (field_0x238 < lbl_804AB630[kind]) {
                continue;
            }
            field_0x238 -= lbl_804AB630[kind];
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, kind);
            if (serial < 0) {
                continue;
            }
            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (gem == NULL) {
                continue;
            }
            u32 mode = (field_0x90 >> 16) & 0xF;
            if (mode == 0) {
                if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                    f32 vz = (0.5f * field_0x23c + 1.0f) *
                             (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 2.0f);
                    f32 vy = (0.5f * field_0x23c + 1.0f) *
                             (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    f32 vx = (0.5f * field_0x23c + 1.0f) *
                             (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    gem->field_0x3c.set(vx, vy, vz);
                } else {
                    f32 vy = (0.5f * field_0x23c + 1.0f) *
                             (-2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 2.0f);
                    f32 vx = (0.5f * field_0x23c + 1.0f) *
                             (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    gem->field_0x3c.set(vx, vy, 0.0f);
                }
            } else if (mode == 6) {
                if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                    s32 ang = (s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    f32 spd = 0.3f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.7f;
                    f32 vx = spd * JMASSin(ang);
                    f32 vy = spd * JMASCos(ang);
                    f32 scale = 0.5f * field_0x23c + 1.0f;
                    vx *= scale;
                    vy *= scale;
                    f32 vz = scale * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 2.0f);
                    gem->field_0x3c.set(vx, vy, vz);
                } else {
                    s32 s = 1 - 2 * ((s32)(2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) == 0);
                    f32 vy = (0.5f * field_0x23c + 1.0f) *
                             (-2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 2.0f);
                    f32 vx = (0.5f * field_0x23c + 1.0f) *
                             ((f32)s * (0.7f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.8f));
                    gem->field_0x3c.set(vx, vy, 0.0f);
                }
            } else {
                s32 ang = (s32)((f32)baseAngle + (8192.0f - 16384.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                f32 vx = (0.5f * field_0x23c + 1.0f) * (1.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                vel.x = vx * JMASSin(ang);
                f32 vy = (0.5f * field_0x23c + 1.0f) * (1.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                vel.y = vy * JMASCos(ang);
                vel.z = (0.25f * field_0x23c + 1.0f) *
                        (3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 3.0f;
                gem->field_0x3c = vel;
            }
        }
        fn_801F3668();
        break;
    }
    case 1: {
        if (vfunc_0x34() && field_0x234 == 0) {
            JGeometry::TVec3f pos(mPos);
            pos.x += 8.0f;
            pos.y += 12.0f;
            field_0x234 = 200;
            gAudioMgr_8053AF80->fn_80458880(0x1D, -1);
            mEmitter24c = spawnEmitter_8013CC50(field_0x4, &pos, 0x3D5, 1, 1, NULL, NULL, true, 1.0f);
            mEmitter250 = spawnEmitter_8013CC50(field_0x4, &pos, 0x3D6, 1, 1, NULL, NULL, true, 1.0f);
            mEmitter254 = spawnEmitter_8013CC50(field_0x4, &pos, 0x3D7, 1, 1, NULL, NULL, true, 1.0f);
        }
        if (field_0x234 > 0 && (field_0x234 & 7) == 0) {
            fn_8032A4FC(mSerial, 0, false);
        }
        if (field_0x234 != 0) {
            field_0x234--;
            if (field_0x234 <= 0) {
                field_0x244 = 255.0f;
                field_0x230 = 2;
            }
        }
        break;
    }
    case 2: {
        addCalc2_801F71A4(&field_0x244, 0.0f, 0.5f, 10.0f);
        if (field_0x244 < 1.0f) {
            if (mEmitter24c != NULL) {
                mEmitter24c->becomeInvalidEmitter();
            }
            if (mEmitter250 != NULL) {
                mEmitter250->becomeInvalidEmitter();
            }
            if (mEmitter254 != NULL) {
                mEmitter254->becomeInvalidEmitter();
            }
            fn_801F3668();
        }
        break;
    }
    case 3: {
        mLife = 1000;
        if (field_0x234 != 0) {
            field_0x234--;
            if (field_0x234 <= 0) {
                if (mEmitter24c != NULL) {
                    mEmitter24c->becomeInvalidEmitter();
                }
                fn_801F3668();
                break;
            }
        }
        addCalc2_801F71A4(&field_0x248, 1.0f, 0.2f, 0.4f);
        if (fn_801F3D94(1) != 0) {
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            if (field_0xe4 != 1 || field_0xe8 == 0) {
                break;
            }
        }
        if ((field_0xe4 == 1 && field_0xe8 > 0) || fn_801F305C()) {
            u16 outAngle = 0;
            field_0xd8 = 20;
            field_0xdc = 20;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x248 = 2.0f;
            JGeometry::TVec3f pos(mPos);
            pos.x += 8.0f;
            pos.y += 12.0f;
            if (field_0x238 == 0) {
                field_0x238++;
                field_0x234 = 500;
                vfunc_0x30(0xFF, true);
            }
            JGeometry::TVec3f savedPos(mPos);
            mPos.x += 8.0f;
            mPos.y += 8.0f;
            fn_801EEC98(mSerial, &outAngle, NULL, NULL, NULL, 0, 0);
            mPos = savedPos;
            fn_8032A4FC(mSerial, (u16)(outAngle + 0x8000), true);
        }
        break;
    }
    case 4: {
        if (field_0x234 == 0) {
            JGeometry::TVec3f pos(mPos);
            pos.x += 8.0f;
            pos.y += 12.0f;
            field_0x234 = 200;
            gAudioMgr_8053AF80->fn_80458880(0x1D, -1);
            mEmitter24c = spawnEmitter_8013CC50(field_0x4, &pos, 0x3D5, 1, 1, NULL, NULL, true, 1.0f);
            mEmitter250 = spawnEmitter_8013CC50(field_0x4, &pos, 0x3D6, 1, 1, NULL, NULL, true, 1.0f);
            mEmitter254 = spawnEmitter_8013CC50(field_0x4, &pos, 0x3D7, 1, 1, NULL, NULL, true, 1.0f);
        }
        if (field_0x234 > 0 && (field_0x234 & 7) == 0) {
            fn_8032A4FC(mSerial, 0, false);
        }
        if (field_0x234 != 0) {
            field_0x234--;
            if (field_0x234 <= 0) {
                field_0x244 = 255.0f;
                field_0x230 = 2;
            }
        }
        break;
    }
    case 5: {
        field_0x238 %= 12;
        s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, field_0x238);
        if (serial >= 0) {
            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (gem != NULL) {
                f32 vz = 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 2.0f;
                f32 vy = 1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                f32 vx = 1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                gem->field_0x3c.set(vx, vy, vz);
            }
        }
        fn_801F3668();
        break;
    }
    }
}

void ForceGemSpawner_804AB660::draw() {
    if (field_0x230 == 3) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXIdentity(mtx);
        mtx[0][0] = field_0x248;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x248;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x248;
        params.mMtx = mtx;
        JGeometry::TVec3f pos(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x2a, &params, 1.0f, 1.0f);
        }
    }
}
