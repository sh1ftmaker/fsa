#include "JSystem/JParticle/JPAEmitter.h"
#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/act/act_wizzrobe.h"
#include "JSystem/JMath/JMath.h"

extern const ActorId lbl_80466838[7];
const ActorId lbl_80466838[7] = { ACTOR_WIZR, ACTOR_FIRE, ACTOR_BLZE, ACTOR_DDGO, ACTOR_FLME, ACTOR_THCL, ACTOR_NULL };

s32 lbl_804A0D50[20] = { 0, 1, 2, 3, 2, 4, 5, 6, 7, 6, 8, 9, 10, 11, 10, 12, 13, 14, 15, 14 };
s32 lbl_804A0DA0[23] = { 2, 1, 20, 20, 10, 16, 17, 18, 19, 16, 17, 18, 19, 20, 2, 3, 3, 4, 4, 5, 6, 7, 10 };
s32 lbl_804A0DFC[20] = { 20, 2, 1, 0, 20, 6, 5, 4, 20, 10, 9, 8, 20, 14, 13, 12, 2, 1, 1, 10 };
s32 lbl_804A0E4C[16] = { 0, 1, 2, 20, 4, 5, 6, 20, 8, 9, 10, 20, 12, 13, 14, 20 };
s32 lbl_804A0E8C[4] = { 10, 1, 1, 2 };
s32 lbl_804A0E9C[4] = { 16, 17, 18, 19 };
s32 lbl_804A0EAC[4] = { 0x4000, 0xC000, 0x8000, 0 };
s32 lbl_804A0EBC[8] = { 1, 5, 20, 50, 100, 150, 200, 300 };

// Not auto-inlined: fn_80176B88 calls fn_80178F10 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_80178F10(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80178F00(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// Out-of-line copy of JPABaseEmitter::setEmitterTranslation(const TVec3&).
// Out-of-line copy of JPABaseEmitter member at 0x18 set(const TVec3&).
void fn_80178EE4(JPABaseEmitter* emitter, const JGeometry::TVec3f* trans) {
    emitter->mLocalDir.set(*trans);
}

// Out-of-line copy of TVec3<s16>::set(x, y, z).
void fn_80178ED4(JGeometry::TVec3<s16>* v, s16 x, s16 y, s16 z) {
    v->set(x, y, z);
}

// Out-of-line copy of JPABaseEmitter::setGlobalTranslation(const TVec3&).
void fn_80178EB8(JPABaseEmitter* emitter, const JGeometry::TVec3f* trans) {
    emitter->setGlobalTranslation(*trans);
}
#pragma auto_inline reset

Wizzrobe_804A0EDC::~Wizzrobe_804A0EDC() {
    releaseEmitter_801EE434(mEmitters[0]);
    releaseEmitter_801EE434(mEmitters[1]);
    releaseEmitter_801EE434(mEmitters[2]);
    if (field_0x94 == 0 || (field_0x94 == 4 || field_0x94 == 5) || (field_0x94 == 1 || field_0x94 == 2) || field_0x94 == 3) {
        s32 serial = field_0x268;
        if (serial >= 0) {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (target != NULL) {
                field_0x268 = -1;
                target->mLife = 0;
            }
        }
    }
}

void Wizzrobe_804A0EDC::vfunc_0x44() {
    if (field_0x94 == 0 || (field_0x94 == 4 || field_0x94 == 5) || (field_0x94 == 1 || field_0x94 == 2) || field_0x94 == 3) {
        s32 serial = field_0x268;
        if (serial >= 0) {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (target != NULL) {
                field_0x268 = -1;
                target->mLife = 0;
            }
        }
    }
    field_0x238 = 0;
    field_0x2e4 = 0;
    field_0x244 = 0;
    field_0x230 = 8;
}

void Wizzrobe_804A0EDC::init() {
    field_0x2c8 = (field_0x90 >> 22) & 0x1F;
    field_0x94 = field_0x90 & 0xFF;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    for (int i = 0; i < 20; i++) {
        field_0x270[i] = -1;
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    field_0x260 = 0;
    field_0x2e4 = 0;
    field_0x2d0 = 0.0f;
    field_0x2d4 = 0.0f;
    field_0x2d8 = 5.0f;
    field_0x268 = -1;
    field_0x26c = -1;
    field_0x2e0 = 0.0f;
    field_0x2cc = 0;
    field_0x264 = 0;
    field_0x234 = 0;
    field_0x2c0 = 0;
    field_0x2c4 = 200;
    field_0x2dc = 1.0f;
    field_0x2e8.z = 0.0f;
    field_0x2e8.y = 0.0f;
    field_0x2e8.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x2f4.set(mPos);
    field_0x2e5 = 0;
    switch (field_0x94) {
    case 0:
    default:
        field_0x250 = 3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x250 &= 3;
        if (field_0x90 >> 27) {
            if (!vfunc_0x34()) {
                field_0x1a8 = 0;
                field_0x2e5 = 1;
            }
        }
        field_0x230 = 0;
        break;
    case 10:
    case 11:
        field_0x1a8 = 0;
        mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
        field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
        mLife = 10000;
        field_0x238 = 10;
        field_0x23c = 10;
        field_0x240 = 200;
        mPos.z = 8.0f;
        field_0x230 = 12;
        break;
    case 20:
        field_0x2dc = 1.2f;
        // fallthrough
    case 21:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x506, 0, 1, NULL, NULL, true, field_0x2dc);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x507, 0, 1, NULL, NULL, true, field_0x2dc);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x508, 0, 1, NULL, NULL, true, 1.0f);
        field_0x1a8 = 0;
        field_0x238 = 10;
        mLife = 10000;
        field_0x230 = 13;
        break;
    case 22:
    case 31:
        field_0x2dc = 1.2f;
        // fallthrough
    case 23:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x506, 0, 1, NULL, NULL, true, field_0x2dc);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x507, 0, 1, NULL, NULL, true, field_0x2dc);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x508, 0, 1, NULL, NULL, true, 1.0f);
        field_0x1a8 = 0;
        field_0x238 = 30;
        mLife = 10000;
        field_0x230 = 15;
        break;
    case 30:
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x50d, 1, 1, NULL, NULL, true, field_0x2dc);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x50e, 1, 1, NULL, NULL, true, field_0x2dc);
        field_0x1a8 = 0;
        field_0x23c = 40;
        mLife = 10000;
        field_0x230 = 16;
        break;
    }
}

void Wizzrobe_804A0EDC::update() {
    s32 unused_14 = 0;
    s32 unused_10 = 0;
    s16 unused_8 = 0;
    f32 unused_c = 0.0f;

    if (field_0x2e5 == 1) {
        if (field_0x90 >> 27) {
            if (vfunc_0x34()) {
                field_0x1a8 = 1;
                field_0x2e5 = 0;
            } else {
                return;
            }
        }
    }

    if (field_0x94 == 0 || (field_0x94 == 4 || field_0x94 == 5) || (field_0x94 == 1 || field_0x94 == 2) || field_0x94 == 3) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }

    if (fn_801F3D94(1)) {
        field_0xb0 = 0;
        fn_801F2618(NULL, false);
        return;
    }

    if (field_0x2e4 == 1) {
        u32 ids[7] = { ACTOR_WIZR, ACTOR_FIRE, ACTOR_BLZE, ACTOR_DDGO, ACTOR_FLME, ACTOR_THCL, ACTOR_NULL };
        s32 prevDamage = mLife;
        bool hitByActor = false;
        if (fn_801F2DB4(ACTOR_NULL, ACTOR_NULL, ids)) {
            if (prevDamage != mLife) {
                hitByActor = true;
            } else {
                field_0xd8 = 0;
                field_0xdc = 0;
            }
        }
        if (hitByActor || fn_801F2FAC()) {
            field_0x238 = 0;
            field_0x2e4 = 0;
            field_0x244 = 0;
            field_0x230 = 8;
            fn_801F2618(NULL, false);
            s32 serial = field_0x268;
            if (serial >= 0) {
                ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (target != NULL) {
                    target->mLife = 0;
                }
                field_0x268 = -1;
            }
            if (mLife > 0) {
                return;
            }
            if (field_0x94 == 1) {
                for (int i = 0; i < 20; i++) {
                    s32 childSerial = field_0x270[i];
                    if (childSerial >= 0) {
                        ActorBase_804A5770* child = getActorManager_801F856C()->findActorBySerial_801F666C(childSerial);
                        if (child != NULL) {
                            field_0x270[i] = -1;
                            child->field_0x94 = 1;
                        }
                    }
                }
            }
            if (field_0x2c8 != 0) {
                getGameView_802D6A9C()->fn_802D8050(field_0x2c8, 1, 0xFF);
            }
            if (field_0x94 == 3) {
                for (int i = 7, delay = 0; delay < 100 && i >= 0; delay++) {
                    if (field_0x2c4 <= 0) {
                        break;
                    }
                    if (field_0x2c4 >= lbl_804A0EBC[i]) {
                        field_0x2c4 -= lbl_804A0EBC[i];
                        s32 gemSerial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 0x220000 | i);
                        if (gemSerial >= 0) {
                            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(gemSerial);
                            if (gem != NULL) {
                                gem->field_0x3c.x = 2.0f - 4.0f * fn_80154744();
                                gem->field_0x3c.y = 2.0f - 4.0f * fn_80154744();
                                gem->field_0x3c.z = 10.0f + 5.0f * fn_80154744();
                            }
                        }
                    } else {
                        i--;
                    }
                }
            }
            return;
        }
    }

    if (field_0x94 != 10 && field_0x94 != 11 && field_0x94 != 22 && field_0x94 != 23 && field_0x94 != 31) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x2e8.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x2e8.y, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x2d0, field_0x2d4, 0.5f, field_0x2d8);
    }
    mPos += field_0x3c;
    if (field_0x2e4 == 1 || field_0x94 == 10 || field_0x94 == 11) {
        fn_801F2B7C(-100, -1, 1);
    }
    if (field_0x94 == 3) {
        fn_801F2618(NULL, false);
    }
    if (field_0x238 > 0) field_0x238--;
    if (field_0x23c > 0) field_0x23c--;
    if (field_0x240 > 0) field_0x240--;
    if (field_0x244 > 0) field_0x244--;
    if (field_0x248 > 0) field_0x248--;

    switch (field_0x230) {
    case 0:
        field_0x24c = 0;
        field_0x238 = 0;
        if (field_0x94 < 4) {
            if (field_0x94 == 0) {
                field_0x23c = (s32)(fn_80154744() * 30.0f) + 30;
            } else {
                field_0x23c = (s32)(fn_80154744() * 50.0f) + 100;
            }
        } else if (field_0x94 < 6) {
            field_0x23c = (s32)(fn_80154744() * 50.0f) + 50;
        } else {
            field_0x23c = (s32)(fn_80154744() * 50.0f) + 100;
        }
        field_0x2d4 = 0.0f;
        field_0x2d8 = 5.0f;
        field_0xb0 = 0;
        field_0x230 = 1;
        // fallthrough
    case 1:
        if (field_0x23c == 30) {
            s32 dir, player;
            fn_801EEC98(mSerial, NULL, &dir, NULL, &player, 0, 1);
            fn_80154590(&field_0x80, -2.0f, -2.0f, 2.0f, 2.0f);
            JGeometry::TVec3f prevPos;
            prevPos.set(mPos);
            if (player >= 0) {
                for (int i = 0; i < 4; i++) {
                    mPos.set(*getPlayerPos_8023E724(player));
                    mPos.x += (64.0f * fn_80154744() + 64.0f) * JMath::sincosTable_.cosShort((field_0x250 & 3) << 14);
                    mPos.y += (64.0f * fn_80154744() + 64.0f) * JMath::sincosTable_.sinShort((field_0x250 & 3) << 14);
                    mPos.z = 0.0f;
                    field_0x250 = (field_0x250 + (s32)(2.99f * fn_80154744()) + 1) & 3;
                    if (!fn_801F2618(NULL, false)) {
                        s32 dir2;
                        fn_801EEC98(mSerial, NULL, &dir2, NULL, NULL, 0, 1);
                        if (dir2 != -1) {
                            field_0x98 = dir2;
                        }
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x50a, 0, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x50b, 0, 1, NULL, NULL, true, 1.0f);
                        startSound_801F0E34(0x40077, 0);
                        fn_80154590(&field_0x80, -8.0f, -8.0f, 8.0f, 8.0f);
                        return;
                    }
                }
            }
            fn_80154590(&field_0x80, -8.0f, -8.0f, 8.0f, 8.0f);
            field_0x23c = 31;
            mPos.set(prevPos);
        }
        if (field_0x23c == 0) {
            field_0x260 = lbl_804A0DFC[field_0x98 * 4 + field_0x24c];
            field_0x2d0 = 255.0f;
            field_0x2d4 = 255.0f;
            field_0x2d8 = 5.0f;
            field_0x230 = 6;
            field_0x23c = 10;
            field_0x238 = 40;
        }
        break;
    case 6:
        field_0xb0 = 2;
        field_0x2e4 = 1;
        field_0x24c = 0;
        field_0x254 = 0;
        field_0x258 = 0;
        field_0x230 = 7;
        // fallthrough
    case 7: {
        fn_801F2618(NULL, false);
        u16 angle;
        s32 dir;
        f32 dist;
        s32 player;
        if (field_0x94 == 1 || field_0x94 == 2 || field_0x94 == 3) {
            if (field_0x24c <= 3) {
                s32 mode = (field_0x94 != 3);
                fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, mode);
                if (field_0x94 != 3) {
                    for (int i = 0; i < 20; i++) {
                        s32 childSerial = field_0x270[i];
                        if (childSerial >= 0) {
                            ActorBase_804A5770* child = getActorManager_801F856C()->findActorBySerial_801F666C(childSerial);
                            if (child == NULL) {
                                field_0x270[i] = -1;
                                if (field_0x264 > 0) field_0x264--;
                            } else {
                                dir = -1;
                            }
                        }
                    }
                }
                if (dir >= 0) {
                    field_0x98 = dir;
                }
                if (field_0x94 == 3) {
                    if (field_0x248 == 0) {
                        if (--field_0x2c4 < 0) field_0x2c4 = 0;
                        field_0x248 = 8;
                    }
                    if (dist < 64.0f) {
                        field_0x234 = 1;
                        s16 rot = angle + 0x8000;
                        field_0x2e8.x = 1.2f * JMath::sincosTable_.cosShort(rot);
                        field_0x2e8.y = 1.2f * JMath::sincosTable_.sinShort(rot);
                    } else {
                        field_0x234 = 0;
                        field_0x2e8.set(0.0f, 0.0f, 0.0f);
                    }
                }
            } else {
                field_0x234 = 0;
                field_0x2e8.set(0.0f, 0.0f, 0.0f);
            }
        }
        if (field_0x24c == 3 && field_0x94 == 3) {
            field_0x238 = 20;
            if (dir >= 0) {
                if (fn_80236228(player) > 0) {
                    if (dist < 48.0f) {
                        if (++field_0x254 > 30) {
                            if (field_0x268 >= 0) {
                                ActorBase_804A5770* t = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268);
                                if (t != NULL) {
                                    field_0x268 = -1;
                                    t->mLife = 0;
                                }
                            }
                            field_0x238 = 0;
                        }
                    } else {
                        field_0x254 = 0;
                    }
                } else {
                    if (++field_0x258 > 120) {
                        ActorBase_804A5770* t = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268);
                        if (t != NULL) {
                            field_0x268 = -1;
                            t->mLife = 0;
                        }
                        field_0x238 = 0;
                    }
                }
            } else {
                field_0x238 = 0;
            }
        }
        if (field_0x238 == 0) {
            field_0x238 = lbl_804A0DA0[field_0x24c++];
            if (field_0x24c == 3) {
                s32 type = field_0x94;
                s32 p4 = 10;
                s32 p7 = 21;
                if (type == 1 || type == 2) {
                    for (int i = 0; i < 20; i++) {
                        s32 childSerial = field_0x270[i];
                        if (childSerial >= 0) {
                            ActorBase_804A5770* child = getActorManager_801F856C()->findActorBySerial_801F666C(childSerial);
                            if (child == NULL) {
                                field_0x270[i] = -1;
                                if (field_0x264 > 0) field_0x264--;
                            } else {
                                type = 0;
                                break;
                            }
                        }
                    }
                }
                if (type == 3) {
                    p7 = 30;
                } else if (type == 0) {
                    startSound_801F0E34(0x40078, 0);
                    p7 = 30;
                } else if (type == 5) {
                    s32 child = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BOIB, field_0x4, &mPos, 0, -1, -1);
                    if (child >= 0) {
                        ActorBase_804A5770* b = getActorManager_801F856C()->findActorBySerial_801F666C(child);
                        startSound_801F0E34(0x30035, 0);
                        if (b != NULL) {
                            u16 a = fn_801F6874(field_0x98);
                            f32 speed = 2.5f;
                            b->field_0x3c.set(speed * JMath::sincosTable_.cosShort(a), speed * JMath::sincosTable_.sinShort(a), 0.0f);
                            b->field_0x98 = field_0x98;
                        }
                    }
                    field_0x244 = 60;
                    field_0x230 = 8;
                    goto skip_wizr_spawn;
                } else if (type == 4) {
                    p4 = 11;
                }
                if (type == 0 || type == 4) {
                    s32 child = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_WIZR, field_0x4, &mPos, p4, -1, -1);
                    if (child >= 0) {
                        ActorBase_804A5770* w = getActorManager_801F856C()->findActorBySerial_801F666C(child);
                        if (w != NULL) {
                            u16 a = fn_801F6874(field_0x98);
                            f32 speed = 2.5f;
                            w->field_0x3c.set(speed * JMath::sincosTable_.cosShort(a), speed * JMath::sincosTable_.sinShort(a), 0.0f);
                            w->field_0x98 = field_0x98;
                        }
                    }
                    startSound_801F0E34(0x40079, 0);
                    field_0x244 = 60;
                    field_0x230 = 8;
                } else {
                    if (type != 3) {
                        startSound_801F0E34(0x40078, 0);
                    }
                    JGeometry::TVec3f spawnPos;
                    spawnPos.set(mPos);
                    spawnPos.z = 24.0f;
                    if (type == 1) {
                        p7 = 20;
                        spawnPos.z = 20.0f;
                    } else if (type == 3) {
                        p7 = 30;
                    } else {
                        field_0x238 += 10;
                    }
                    field_0x268 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_WIZR, field_0x4, &spawnPos, p7, -1, -1);
                    if (field_0x268 >= 0) {
                        if (field_0x270[0] < 0) {
                            field_0x270[0] = field_0x268;
                        }
                        Wizzrobe_804A0EDC* w = static_cast<Wizzrobe_804A0EDC*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268));
                        if (w != NULL) {
                            w->field_0x238 = field_0x238;
                            w->field_0x26c = mSerial;
                        }
                    }
                }
            skip_wizr_spawn:;
            }
        }
        if (field_0x94 != 3) {
            if (field_0x24c == 4) {
                startSound_801F0E34(0x40079, 0);
            }
        }
        if (field_0x24c > 4) {
            if (field_0x94 == 3) {
                ActorBase_804A5770* t = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268);
                if (t != NULL) {
                    field_0x268 = -1;
                    t->mLife = 0;
                }
            }
            field_0x244 = 60;
            field_0x230 = 8;
            return;
        }
        if (field_0x23c == 0) {
            field_0x260 = lbl_804A0D50[field_0x98 * 5 + field_0x24c];
        }
        break;
    }
    case 8:
        field_0x2e8.set(0.0f, 0.0f, 0.0f);
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        if (field_0x244 == 0) {
            field_0x24c = 0;
            field_0x230 = 9;
        }
        break;
    case 9:
        if (field_0x238 == 0) {
            field_0x2e4 = 0;
            field_0xb0 = 0;
            field_0x260 = lbl_804A0E4C[field_0x98 * 4 + field_0x24c];
            field_0x238 = lbl_804A0E8C[field_0x24c++];
            field_0x2d4 = 0.0f;
            field_0x2d8 = 10.0f;
            if (field_0x24c > 3) {
                field_0x230 = 10;
            }
        }
        break;
    case 10:
        field_0x24c = 0;
        field_0x230 = 11;
        break;
    case 11:
        if (field_0x238 == 0) {
            field_0x238 = 3;
            field_0x260 = lbl_804A0E9C[field_0x24c++];
            if (field_0x24c > 3) {
                field_0x230 = 0;
            }
        }
        break;
    case 12:
        if (mEmitters[0] == NULL) {
            if (field_0x94 == 11) {
                mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x19b, 1, 1, NULL, NULL, true, 1.0f);
                mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x19b, 1, 1, NULL, NULL, true, 1.0f);
            } else if (field_0x94 == 10) {
                mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x50c, 1, 1, NULL, NULL, true, 1.0f);
            }
        }
        if (mEmitters[0] != NULL) {
            if (field_0x94 == 10) {
                JGeometry::TVec3f dirVec;
                if (field_0x98 == 2) {
                    fn_80178F00(&dirVec, 0.0f, 0.0f, 1.0f);
                    fn_80178EE4(mEmitters[0], &dirVec);
                } else if (field_0x98 >= 0) {
                    if (field_0x98 == 0) {
                        fn_80178F00(&dirVec, 1.0f, 0.0f, 0.0f);
                        fn_80178EE4(mEmitters[0], &dirVec);
                    } else if (field_0x98 < 2) {
                        fn_80178F00(&dirVec, -1.0f, 0.0f, 0.0f);
                        fn_80178EE4(mEmitters[0], &dirVec);
                    }
                } else if (field_0x98 == 3) {
                    fn_80178F00(&dirVec, 0.0f, 0.0f, -1.0f);
                    fn_80178EE4(mEmitters[0], &dirVec);
                }
            } else {
                JGeometry::TVec3<s16> rot;
                fn_80178ED4(&rot, 0, lbl_804A0EAC[field_0x98], 0);
                mEmitters[0]->setGlobalRotation(rot);
            }
        }
        if (field_0x94 == 11 && mEmitters[1] != NULL) {
            JGeometry::TVec3<s16> rot;
            fn_80178ED4(&rot, 0, lbl_804A0EAC[field_0x98], 0);
            mEmitters[1]->setGlobalRotation(rot);
        } else if (mEmitters[0] != NULL) {
            setEmitterPos_8013CB44(mEmitters[0], &mPos, 1, 1, true);
            if (field_0x94 == 11) {
                setEmitterPos_8013CB44(mEmitters[1], &mPos, 1, 1, true);
                if (field_0x238 == 0) {
                    JGeometry::TVec3f spawnPos;
                    spawnPos.set(mPos);
                    if ((field_0x98 & 1) == 0) {
                        spawnPos.z += 8.0f;
                    }
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLZE, field_0x4, &spawnPos, 0x1E0C0, -1, -1);
                    field_0x238 = 5;
                }
            }
        }
        fn_801F2618(NULL, false);
        if (field_0x240 != 0) {
            if (field_0x23c != 0) {
                if (!fn_801F6758(&mPos, field_0x4, 0.0f)) {
                    if (!fn_801F2618(NULL, false)) {
                        break;
                    }
                }
            } else {
                if (!fn_801F2618(NULL, false)) {
                    break;
                }
            }
        }
        mAlive = false;
        if (field_0x94 == 11) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x19d, 1, 1, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[0] != NULL) {
            mEmitters[0]->becomeInvalidEmitter();
        }
        if (mEmitters[1] != NULL) {
            mEmitters[1]->becomeInvalidEmitter();
        }
        break;
    case 13:
        if (mLife == 0) {
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
            }
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x509, 0, 1, NULL, NULL, true, field_0x2dc);
            mAlive = false;
            break;
        }
        if (field_0x238 == 0) {
            u16 angle; s32 dir; s32 player;
            fn_801EEC98(mSerial, &angle, &dir, NULL, &player, 0, 1);
            if (player >= 0) {
                field_0x2e8.x = 2.0f * JMath::sincosTable_.cosShort(angle);
                field_0x2e8.y = 2.0f * JMath::sincosTable_.sinShort(angle);
                field_0x24c = player;
                field_0x230 = 14;
            } else {
                if (mEmitters[0] != NULL) {
                    mEmitters[0]->becomeInvalidEmitter();
                }
                if (mEmitters[1] != NULL) {
                    mEmitters[1]->becomeInvalidEmitter();
                }
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x509, 0, 1, NULL, NULL, true, field_0x2dc);
                mAlive = false;
            }
        }
        break;
    case 14: {
        u16 angle; s32 dir; f32 dist; s32 player;
        fn_801EEC98(mSerial, &angle, &dir, &dist, &player, field_0x24c, 2);
        if (mEmitters[0] != NULL) {
            fn_80178EB8(mEmitters[0], &mPos);
        }
        if (mEmitters[1] != NULL) {
            fn_80178EB8(mEmitters[1], &mPos);
        }
        if (player < 0) {
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
            }
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x509, 0, 1, NULL, NULL, true, field_0x2dc);
            mAlive = false;
            break;
        }
        if (dir != -1) {
            field_0x2e8.x = 2.0f * JMath::sincosTable_.cosShort(angle);
            field_0x2e8.y = 2.0f * JMath::sincosTable_.sinShort(angle);
        }
        if (dist < 4.0f) {
            field_0x2e8.set(0.0f, 0.0f, 0.0f);
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x24c = 0;
            field_0x250 = 0;
            s32 id = (field_0x94 == 21) ? 23 : 22;
            s32 count = (field_0x94 == 21) ? 20 : 4;
            for (int i = 0; i < count; i++) {
                s32 s = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_WIZR, field_0x4, &mPos, id, -1, -1);
                field_0x268 = s;
                if (s >= 0) {
                    Wizzrobe_804A0EDC* a = static_cast<Wizzrobe_804A0EDC*>(getActorManager_801F856C()->findActorBySerial_801F666C(s));
                    if (a != NULL) {
                        a->field_0x2cc = (s16)(i * (0x10000 / count));
                        a->field_0x26c = field_0x26c;
                    }
                }
            }
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
            }
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x509, 0, 1, NULL, NULL, true, field_0x2dc);
            if (id == 22) {
                if (field_0x26c >= 0) {
                    Wizzrobe_804A0EDC* a = static_cast<Wizzrobe_804A0EDC*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c));
                    if (a != NULL) {
                        a->field_0x25c++;
                    }
                }
            }
            mAlive = false;
        }
        break;
    }
    case 15: {
        fn_80178F10(&mPos, &field_0x2f4);
        mPos.x += field_0x2e0 * JMath::sincosTable_.cosShort(field_0x2cc);
        mPos.y += field_0x2e0 * JMath::sincosTable_.sinShort(field_0x2cc);
        if (mEmitters[0] != NULL) {
            fn_80178EB8(mEmitters[0], &mPos);
        }
        if (mEmitters[1] != NULL) {
            fn_80178EB8(mEmitters[1], &mPos);
        }
        field_0x2e0 += 2.0f;
        field_0x2cc += 0x500;
        if (field_0x238 == 0) {
            JGeometry::TVec3f spawnPos;
            fn_80178F10(&spawnPos, &mPos);
            for (int i = 0; i < 4; i++) {
                if (!fn_801F2618(NULL, false) && !fn_801F6758(&spawnPos, field_0x4, 0.0f)) {
                    ActorId id = ACTOR_POPO;
                    s32 p4 = 1;
                    if (field_0x94 == 22) {
                        id = ACTOR_STAL;
                        p4 = 5;
                    } else if (field_0x94 == 31) {
                        id = ACTOR_POOH;
                        p4 = 0;
                    }
                    field_0x268 = getActorManager_801F856C()->spawnActor_801F9484(id, field_0x4, &spawnPos, p4, -1, -1);
                    if (field_0x268 >= 0) {
                        spawnEmitter_8013CC50(field_0x4, &spawnPos, 0x509, 0, 1, NULL, NULL, true, 1.0f);
                        if (field_0x94 == 22) {
                            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x268);
                            if (a != NULL) {
                                a->field_0x3c.set(0.0f, 0.0f, -0.1f);
                            }
                        }
                        if (field_0x26c >= 0) {
                            Wizzrobe_804A0EDC* parent = static_cast<Wizzrobe_804A0EDC*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c));
                            if (parent != NULL) {
                                parent->field_0x270[parent->field_0x264++] = field_0x268;
                            }
                        }
                        break;
                    }
                }
                fn_80178F10(&spawnPos, &mPos);
                spawnPos.x += 24.0f * JMath::sincosTable_.cosShort(((i + 1) & 3) << 14);
                spawnPos.y += 24.0f * JMath::sincosTable_.sinShort(((i + 1) & 3) << 14);
            }
            mAlive = false;
        }
        break;
    }
    case 16:
        if (mLife == 0) {
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
            }
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
            }
            mAlive = false;
            break;
        }
        startSound_801F0E34(0x4007C, 0);
        if (field_0x26c >= 0) {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
            if (a != NULL) {
                fn_80178F10(&mPos, &a->mPos);
                mPos.y -= 24.0f;
                if (mEmitters[0] != NULL) setEmitterPos_8013CB44(mEmitters[0], &mPos, 1, 1, true);
                if (mEmitters[1] != NULL) setEmitterPos_8013CB44(mEmitters[1], &mPos, 1, 1, true);
            }
        }
        if (field_0x23c == 0) {
            f32 minDist = -1.0f;
            s32 foundPlayer = -1;
            s32 bestPlayer = 0;
            s32 dir = -1;
            for (int p = 0; p < 4; p++) {
                if (fn_8023F264(p) && !fn_80230188(p) && !fn_8022DCD8(p) &&
                    getPlayerLayer_8023DE58(p) == field_0x4 && !fn_80236B50(p) && !fn_8022D534(p)) {
                    JGeometry::TVec3f* pPos = getPlayerPos_8023E724(p);
                    f32 dx = mPos.x - pPos->x;
                    f32 dy = mPos.y - pPos->y;
                    if (minDist == -1.0f) {
                        minDist = fn_80154378(dx * dx + dy * dy);
                    } else {
                        f32 dist = fn_80154378(dx * dx + dy * dy);
                        if (dist >= minDist) continue;
                        minDist = dist;
                    }
                    s32 angle = (s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
                    dir = fn_801F681C((u16)angle);
                    bestPlayer = p;
                    s32 tp = fn_8022F3DC(p);
                    if (tp < 0) tp = p;
                    foundPlayer = tp;
                }
            }
            if (dir != -1) {
                field_0x98 = dir;
            }
            if (foundPlayer >= 0) {
                if (fn_80236228(foundPlayer) > 0) {
                    fn_802362C4(foundPlayer, -1);
                    JGeometry::TVec3f* pPos = getPlayerPos_8023E724(bestPlayer);
                    s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, pPos, 0x1B, -1, -1);
                    if (serial >= 0) {
                        Wizzrobe_804A0EDC* r = static_cast<Wizzrobe_804A0EDC*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                        if (r != NULL) {
                            r->field_0x26c = mSerial;
                        }
                    }
                    field_0x254 = (field_0x254 + 1) & 0x3F;
                    if (field_0x254 == 0) {
                        startSound_801F0E34(0x4007D, 0);
                    }
                }
            }
            field_0x23c = 2;
        }
        break;
    }
}

void Wizzrobe_804A0EDC::draw() {
    if (field_0x94 == 20 || (field_0x94 == 21 || field_0x94 == 22 || field_0x94 == 23) || (field_0x94 == 30 || field_0x94 == 31)) {
        return;
    }
    if (field_0x230 == 1) {
        return;
    }
    if (field_0x230 == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    if (field_0x94 == 10 || field_0x94 == 11) {
        return;
    }
    params.field_0x10.a = (u8)(s32)field_0x2d0;
    if (field_0x234 == 1) {
        params.field_0x30 = 0x11;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x260 + 233, &params, 0, 1.0f, 1.0f);
}
