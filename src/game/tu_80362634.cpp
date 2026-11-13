#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/tu_80362634.h"
#include "game/Effect.h"
#include "game/SpriteParams.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "JSystem/JMath/JMath.h"
#include <float.h>
#include <math.h>

// FIXME(shotgun): tables not written out (gate.py literal tripwire); extern-declared, sizes from symbols.txt.
extern const s32 lbl_80474320[76];
static const s32 lbl_80474450[42] = {20, 2, 20, 2, 12, 2, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 20, 0, 20, 0, 20, 0, 15, 2};
extern const s32 lbl_804745B8[150];
static const s32 lbl_80474598[4] = {6, 9, 3, 0};
static const s32 lbl_80474A5C[3] = {907, 904, 910};
static const s32 lbl_80474B58[4] = {937, 945, 929, 921};
static const s32 lbl_80474B68[8] = {3, 3, 4, 3, 3, 3, 4, 3};
static const s32 lbl_804745A8[4] = {910, 913, 907, 904};
static const s32 lbl_80474A98[16] = {4, 4, 6, 4, 4, 4, 6, 4, 4, 4, 6, 4, 4, 4, 6, 4};
static const s32 lbl_80474AD8[32] = {0, 1, 2, 3, 9, 6, 7, 8, 0, 1, 2, 3, 9, 6, 7, 8,
                                     5, 6, 7, 8, 4, 1, 2, 3, 5, 6, 7, 8, 4, 1, 2, 3};
static const s32 lbl_80474BC4[4] = {0, 1, 0, 2};
static const s32 lbl_80474B88[6] = {7, 10, 10, 7, 10, 10};
static const s32 lbl_80474BA0[6] = {0, 1, 2, 0, 3, 4};

// Read by fn_80364A88: the direction/animation words per heading (16-byte rows), then a row of
// field_0x26c values for the idle state (all inside the original's lbl_80474534 / lbl_80474A5C ranges).
static const s32 lbl_80474548[4][4] = {
    {851, 854, 848, 845},
    {904, 907, 901, 898},
    {865, 868, 862, 859},
    {879, 882, 876, 873},
};
static const s32 lbl_80474858[4] = {959, 962, 956, 953}; // sprite per heading (the original's table is 0x204 long)
static const s32 lbl_80474588[4] = {843, 904, 857, 871};
static const s32 lbl_80474A68[4] = {0, 1, 2, 1};
static const s32 lbl_80474A78[4] = {9, 7, 9, 7};
static const s32 lbl_80474A88[4] = {10, 7, 10, 7};

static const s8 lbl_80542AD8[8] = {10, 7, 10, 7, -1, 0, 0, 0};
static const s8 lbl_80542AC8[4] = {0, 1, 2, 1};

static JGeometry::TVec3f lbl_8050C028[8];
// bss 0x8050C088: four unit directions, filled by the unit's static initialiser.
static JGeometry::TVec3f lbl_8050C088[4];

// Destructor at 0x80366D44.
// FIXME(shotgun): 97.82 %, regalloc-only; behaviour clean.
cls_804ADF28::~cls_804ADF28() {
    if (field_0x234 == 3) {
        deleteEmitter(&field_0x300);
    }
    if (field_0x234 == 0x10) {
        deleteEmitter(&field_0x304);
        deleteEmitter(&field_0x308);
        for (int i = 0; i < 4; i++) {
            deleteEmitter(&field_0x30c[i]);
        }
    }
}

// Table lbl_804ADC38: member function pointers (0xC bytes each, 0x1B0 bytes in the original); contents not written yet.
extern void (cls_804ADF28::*lbl_804ADC38[36])();

void cls_804ADF28::fn_80365AF0() {
    if (field_0x230 == 0) {
        if (field_0x248 != 0) {
            field_0x248--;
        }
        if (field_0x25c != 0) {
            field_0x25c--;
        }
        (this->*lbl_804ADC38[field_0x234])();
    }
}

// Table lbl_804ADDE8: member function pointers (0xC bytes each, 0x140 bytes in the original); contents not written yet.
extern void (cls_804ADF28::*lbl_804ADDE8[64])();

void cls_804ADF28::fn_80365AB8() {
    (this->*lbl_804ADDE8[field_0x234])();
}

// FIXME(shotgun): 52.86 %, missing/extra + reloc-name; behaviour drift.
// (cases 1-3 of the switch are not written; the function-local TVec3f statics are guarded with an out-of-line constructor here, the original initialises them inline; case 0 checked against the asm)
void cls_804ADF28::fn_803653EC() {
    if (field_0x238 < 0) {
        if (field_0x248 >= 0x80) {
            if (field_0x25c == 0) {
                field_0x25c = field_0x260;
                field_0x268++;
                if (field_0x268 >= 4) {
                    field_0x268 = 0;
                }
                field_0x264 = lbl_80474BC4[field_0x268];
            }
            mPos.add(field_0x3c);
            field_0x27c[0].add(field_0x3c);
            field_0x27c[1].add(field_0x3c);
            field_0x27c[2].add(field_0x3c);
            if (field_0x248 == 0x80) {
                field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                field_0x26c = 0x388;
                field_0x264 = 0;
            }
            return;
        }
        if (field_0x248 != 0) {
            return;
        }
        field_0x238 = 0;
    }
    if (field_0x25c == 0) {
        field_0x25c = field_0x260;
        field_0x268++;
        if (field_0x268 >= 4) {
            field_0x268 = 0;
        }
        field_0x264 = lbl_80474BC4[field_0x268];
    }
    if (field_0x248 == 0) {
        field_0x248 = 0x80;
        switch (field_0x238 & 3) {
        case 0: {
            field_0x26c = 0x391;
            static JGeometry::TVec3f offsets[3] = {
                JGeometry::TVec3f(-16.0f, 16.0f, 0.0f),
                JGeometry::TVec3f(16.0f, 16.0f, 0.0f),
                JGeometry::TVec3f(0.0f, 32.0f, 0.0f),
            };
            for (int i = 0; i < 3; i++) {
                field_0x2a0[i] = mPos + offsets[i];
                field_0x2c4[i].set((field_0x2a0[i].x - field_0x27c[i].x) / 10.0f,
                                   (field_0x2a0[i].y - field_0x27c[i].y) / 10.0f, 0.0f);
                field_0x2e8[i] = lbl_80474A5C[i];
            }
            break;
        }
        case 1:
        case 2:
        case 3:
            // FIXME(shotgun): cases 1-3 not written (the three formation set-ups; sprites 0x388, 0x38E, 0x391 respectively).
            break;
        }
        field_0x238++;
    } else {
        field_0x27c[0].add(field_0x2c4[0]);
        field_0x27c[1].add(field_0x2c4[1]);
        field_0x27c[2].add(field_0x2c4[2]);
        if (field_0x248 == 0x76) {
            field_0x27c[0] = field_0x2a0[0];
            field_0x2c4[0].x = field_0x2c4[0].y = field_0x2c4[0].z = 0.0f;
            field_0x27c[1] = field_0x2a0[1];
            field_0x2c4[1].x = field_0x2c4[1].y = field_0x2c4[1].z = 0.0f;
            field_0x27c[2] = field_0x2a0[2];
            field_0x2c4[2].x = field_0x2c4[2].y = field_0x2c4[2].z = 0.0f;
        }
    }
}

// FIXME(shotgun): 92.69 %, scheduling+regalloc + reloc-name; behaviour clean.
void cls_804ADF28::fn_80365274() {
    mPos.add(field_0x3c);
    switch (field_0x238) {
    case 0:
    case 2:
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x268++;
            if (field_0x268 >= 4) {
                field_0x268 = 0;
            }
            field_0x264 = lbl_80474BC4[field_0x268];
        }
        if (field_0x248 == 0) {
            field_0x248 = 0xc6;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x26c = 0x3ac;
            field_0x264 = 0;
            if (field_0x238 == 2) {
                mAlive = false;
            } else {
                field_0x238++;
            }
        }
        break;
    case 1:
        if (field_0x248 == 0x8a) {
            field_0x26c = 0x3a4;
        } else if (field_0x248 == 0x72) {
            field_0x26c = 0x3a8;
        } else if (field_0x248 == 0x5a) {
            field_0x26c = 0x3a4;
        } else if (field_0x248 == 0) {
            field_0x238++;
            JGeometry::TVec3f bob(0.0f, -0.25f, 0.0f);
            field_0x3c = bob;
            field_0x26c = 0x3b1;
            field_0x264 = 0;
            field_0x248 = 100;
        }
        break;
    }
}

// FIXME: 99.52 %, reloc-name; behaviour clean.
// Instructions identical; only the relocation of the 0.03f literal is @NNN here
// against lbl_80542BD4 (the unit's sdata2 is not complete yet).
void cls_804ADF28::fn_80365220() {
    if (field_0x248 == 0) {
        field_0x248 = 0x40;
        field_0x238++;
    }
    if ((field_0x238 & 1) == 0) {
        mPos.y += 0.03f;
    } else {
        mPos.y -= 0.03f;
    }
}

// FIXME(shotgun): 99.90 %, reloc-name; behaviour clean.
void cls_804ADF28::fn_8036509C() {
    if (field_0x248 != 0) {
        if (field_0x248 <= 14) {
            if (field_0x248 == 14) {
                if (field_0x300 != NULL) {
                    field_0x300->becomeInvalidEmitter();
                    field_0x300 = NULL;
                }
                field_0x258 = 0;
                field_0x264 = 11;
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x248 == 10) {
                field_0x264 = 10;
            } else if (field_0x248 == 8) {
                field_0x264 = 9;
            } else if (field_0x248 == 6) {
                field_0x264 = 8;
            } else if (field_0x248 == 4) {
                field_0x264 = 2;
            } else if (field_0x248 == 2) {
                field_0x264 = 1;
            }
        }
        if (field_0x248 == 1) {
            field_0x25c = fn_801CD664(9);
            field_0x264 = 0;
        }
    } else if (field_0x25c == 0) {
        field_0x25c = 8;
        field_0x264++;
        if (field_0x264 >= 4) {
            field_0x264 = 0;
        }
    }
}

// FIXME(shotgun): 99.43 %, regalloc-only + reloc-name; behaviour clean.
void cls_804ADF28::fn_80364F60() {
    if (field_0x238 != 5) {
        s32 oldAnim = field_0x264;
        if (field_0x25c == 0) {
            field_0x268++;
            if (field_0x270[field_0x268] < 0) {
                field_0x268 = 0;
            }
            field_0x25c = field_0x270[field_0x268];
            field_0x264 = field_0x274[field_0x268];
        }
        if (oldAnim != field_0x264 && field_0x238 >= 4 && field_0x238 <= 7 && field_0x264 == 0) {
            field_0x25c += fn_801CD664(5) * 6;
        }
        if (field_0x238 == 0x1d || field_0x238 == 0x1e) {
            if (field_0x268 == 2) {
                if (field_0x25c == 12) {
                    field_0x3c.z = 2.7692308f;
                } else {
                    field_0x3c.z -= 0.42603552f;
                }
                mPos.z += field_0x3c.z;
            } else {
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
            }
        }
    }
}

void cls_804ADF28::fn_80364F00() {
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x270[field_0x268] < 0) {
            field_0x268 = 0;
        }
        field_0x25c = field_0x270[field_0x268];
        field_0x264 = field_0x274[field_0x268];
    }
}

// FIXME(shotgun): 95.74 %, different + reloc-name; behaviour drift.
void cls_804ADF28::fn_80364D0C() {
    s32 slot = field_0x24c;
    if (slot < 0) {
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_EOWL) {
                field_0x24c = i;
                break;
            }
        }
    } else {
        s32 oldDir = field_0x23c;
        ActorBase_804A5770* owl = getActorManager_801F856C()->mActors[slot];
        if (owl == NULL) {
            field_0x24c = -1;
        } else {
            if (PSVECDistance(&mPos, &owl->mPos) >= 80.0f) {
                field_0x23c = 0;
            } else {
                field_0x240 = (s16)(JMath::atanTable_.atan2_(owl->mPos.y - mPos.y, owl->mPos.x - mPos.x) * 10430.378f);
                u16 angle = field_0x240;
                if (angle >= 0x2000 && angle < 0x6000) {
                    field_0x23c = 0;
                } else if (angle >= 0x6000 && angle < 0xa000) {
                    field_0x23c = 2;
                } else if (angle >= 0xa000 && angle < 0xe000) {
                    field_0x23c = 1;
                } else {
                    field_0x23c = 3;
                }
            }
        }
        if (field_0x23c != oldDir) {
            field_0x26c = lbl_80474320[field_0x238 * 4 + field_0x23c];
            field_0x25c = lbl_80474450[field_0x238 * 2];
        }
    }
    if (field_0x25c == 0) {
        field_0x25c = field_0x260;
        field_0x264 ^= 1;
    }
}

// FIXME(shotgun): 98.18 %, missing/extra + reloc-name; behaviour drift.
void cls_804ADF28::fn_80364A88() {
    if (field_0x248 == 0) {
        f32 speed = PSVECMag(&field_0x3c);
        mPos.add(field_0x3c);
        field_0x2f8 += speed;
        if (field_0x2f4 != NULL) {
            if (field_0x2f8 >= *field_0x2f4) {
                field_0x2f8 = 0.0f;
                field_0x2f4++;
                field_0x23c++;
                if (field_0x23c == 1) {
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = -1.0f * speed;
                    field_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][1];
                } else if (field_0x23c == 2) {
                    field_0x3c.x = speed;
                    field_0x3c.y = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][3];
                } else if (field_0x23c == 3) {
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = speed;
                    field_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][0];
                } else if (field_0x23c == 4) {
                    field_0x3c.x = speed;
                    field_0x3c.y = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][3];
                } else {
                    field_0x26c = lbl_80474588[field_0x238];
                    field_0x260 = 15;
                    field_0x25c = 15;
                    if (field_0x238 == 1) {
                        field_0x264 = 1;
                    } else {
                        field_0x264 = 0;
                    }
                    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                    field_0x2f4 = NULL;
                }
            }
            if (field_0x25c == 0) {
                field_0x268++;
                if (field_0x268 >= 4) {
                    field_0x268 = 0;
                }
                if (field_0x238 == 1) {
                    field_0x25c = lbl_80474A78[field_0x268];
                } else {
                    field_0x25c = lbl_80474A88[field_0x268];
                }
                field_0x264 = lbl_80474A68[field_0x268];
            }
        } else if (field_0x238 != 1 && field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x264 ^= 1;
        }
    }
}

// FIXME(shotgun): 98.57 %, missing/extra + reloc-name; behaviour near.
void cls_804ADF28::fn_80364900() {
    if (field_0x238 == 0) {
        mPos.add(field_0x3c);
        if (field_0x25c != 0) {
            return;
        }
        field_0x268++;
        if (field_0x268 >= 0x10) {
            field_0x238 = 1;
            field_0x264 = 0;
            field_0x268 = 0;
            field_0x26c = 0x2b1;
            field_0x3c.z = 5.0f;
            field_0x3c.x = 0.0f;
            return;
        }
        field_0x25c = lbl_80474A98[field_0x268];
        if (field_0x3c.x < 0.0f) {
            field_0x264 = lbl_80474AD8[field_0x268 + 16];
        } else {
            field_0x264 = lbl_80474AD8[field_0x268];
        }
    } else {
        field_0x3c.z -= 0.5f;
        mPos.add(field_0x3c);
        if (mPos.z >= 0.0f) {
            return;
        }
        field_0x238 = 0;
        mPos.z = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x26c = 0x38e;
        field_0x25c = 4;
        if (field_0x23c == 3) {
            field_0x23c = 2;
            field_0x3c.x = -0.8f;
            field_0x264 = 5;
            field_0x25c = 4;
        } else {
            field_0x23c = 3;
            field_0x3c.x = 0.8f;
            field_0x26c = 0x38e;
            field_0x264 = 0;
        }
    }
}

// FIXME(shotgun): 97.27 %, different + reloc-name; behaviour drift.
void cls_804ADF28::fn_803645B4() {
    if (field_0x248 == 0 || field_0x238 != 0) {
        if (field_0x238 == 0) {
            s32 slot = field_0x24c;
            if (slot >= 0) {
                ActorBase_804A5770* owl = getActorManager_801F856C()->mActors[slot];
                if (owl != NULL) {
                    owl->mAlive = false;
                }
            }
            s32 end = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < end; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mActorId == 'RUPY') {
                    JGeometry::TVec3f target = actor->mPos;
                    target.y += 4.0f;
                    if (actor->mPos.z == 0.0f && PSVECDistance(&mPos, &target) < 80.0f &&
                        PSVECDistance(&mPos, &target) > 30.0f) {
                        field_0x238 = 1;
                        field_0x3c.x = (target.x - mPos.x) / 40.0f;
                        field_0x3c.y = (target.y - mPos.y) / 40.0f;
                        field_0x24c = i;
                        field_0x264 = 0;
                        field_0x268 = 0;
                        field_0x25c = 3;
                        u16 angle = (s16)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
                        if (angle >= 0x2000 && angle < 0x6000) {
                            field_0x23c = 0;
                        } else if (angle >= 0x6000 && angle < 0xa000) {
                            field_0x23c = 2;
                        } else if (angle >= 0xa000 && angle < 0xe000) {
                            field_0x23c = 1;
                        } else {
                            field_0x23c = 3;
                        }
                        field_0x26c = lbl_80474B58[field_0x23c];
                        field_0x248 = 0x3c;
                        break;
                    }
                }
            }
        } else {
            mPos.add(field_0x3c);
            fn_801F2618(NULL, false);
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[field_0x24c];
            if (actor == NULL) {
                field_0x24c = -1;
                field_0x238 = 0;
                field_0x264 = 0;
                field_0x268 = 0;
            } else {
                if (field_0x25c == 0) {
                    field_0x264++;
                    if (field_0x264 >= 8) {
                        field_0x264 = 0;
                    }
                    field_0x25c = lbl_80474B68[field_0x264];
                }
                JGeometry::TVec3f target = actor->mPos;
                target.y += 4.0f;
                if (PSVECDistance(&mPos, &target) < 4.0f) {
                    field_0x238 = 0;
                    field_0x248 = 30;
                    field_0x264 = 0;
                } else if (field_0x248 == 0) {
                    field_0x238 = 0;
                    field_0x248 = 10;
                    field_0x264 = 0;
                }
            }
        }
    }
}

// FIXME(shotgun): 94.07 %, missing/extra + reloc-name; behaviour drift.
void cls_804ADF28::fn_80364340() {
    if (field_0x248 == 0) {
        JGeometry::TVec3f oldPos = mPos;
        JGeometry::TVec3f off;
        off.x = (f32)fn_801CD664(380) - 190.0f;
        off.y = (f32)fn_801CD664(380) - 140.0f;
        off.z = 0.0f;
        mPos.add(off);
        bool blocked;
        if (mPos.x < 2600.0f || mPos.x > 3016.0f) {
            blocked = true;
        } else if (mPos.y < 40.0f || mPos.y > 328.0f) {
            blocked = true;
        } else if (mPos.x + mPos.y < 2704.0f) {
            blocked = true;
        } else if (mPos.x > 2984.0f && mPos.y < 120.0f) {
            blocked = true;
        } else {
            JGeometry::TVec3f c(2688.0f, 120.0f, 0.0f);
            if (PSVECDistance(&mPos, &c) < 78.0f) {
                blocked = true;
            } else {
                c.set(2944.0f, 216.0f, 0.0f);
                if (PSVECDistance(&mPos, &c) < 78.0f) {
                    blocked = true;
                } else {
                    blocked = false;
                }
            }
        }
        if (blocked) {
            mPos = oldPos;
        } else {
            field_0x248 = fn_801CD664(50) + 60;
            off.z = 100.0f;
            mPos = oldPos;
            u32 param = fn_801CD664(8) | 0x250000;
            JGeometry::TVec3f spawnPos = mPos + off;
            getActorManager_801F856C()->spawnActor_801F9484('RUPY', field_0x4, &spawnPos, param, -1, -1);
        }
    }
}

// FIXME(shotgun): the original divides by (6.283184f * dist) with fres (estimate), we emit fdivs; literal names differ.
void cls_804ADF28::fn_80363FFC() {
    s32 tileX;
    s32 tileY;
    if (field_0x3c.x == 0.0f) {
        tileX = (s32)mPos.x;
        if (field_0x3c.y < 0.0f) {
            tileY = (s32)(mPos.y - 24.0f);
        } else {
            tileY = (s32)(24.0f + mPos.y);
        }
    } else {
        tileY = (s32)mPos.y;
        if (field_0x3c.x < 0.0f) {
            tileX = (s32)(mPos.x - 24.0f);
        } else {
            tileX = (s32)(24.0f + mPos.x);
        }
    }
    if (field_0x3c.z == 0.0f && mPos.z == 0.0f) {
        s32 cell = (tileX & 0x100) | ((tileY & 0x100) << 1) | (tileY & 0xF0) | ((tileX >> 4) & 0xF);
        field_0x27a = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, cell) & 0x3FF;
        if (field_0x27a == 0x1a8) {
            field_0x264 = 0;
            field_0x3c.z = 3.0f;
        }
    } else {
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 0.125f;
        } else {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        field_0x264 = 0;
        field_0x25c = 1;
    }
    JGeometry::TVec3f center(1808.0f, 3296.0f, 0.0f);
    if (mPos.x >= center.x) {
        if (mPos.y < center.y) {
            s32 angle = (JMath::atanTable_.atan2_(mPos.y - center.y, mPos.x - center.x) * 10430.378f);
            f32 dist = PSVECDistance(&mPos, &center);
            s32 delta = 65536.0f * (1.3f / (6.283184f * dist));
            u16 a = angle + delta;
            mPos.x = center.x + dist * JMath::sincosTable_.cosShort(a);
            mPos.y = center.y + dist * JMath::sincosTable_.sinShort(a);
            if (a >= 0xe000) {
                field_0x23c = 0;
                if (field_0x238 == 0) {
                    field_0x26c = 0x32f;
                } else {
                    field_0x26c = 0x33b;
                }
                field_0x26c += lbl_80474598[field_0x23c];
            }
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
        } else {
            field_0x3c.set(lbl_8050C028[0]);
        }
    }
    mPos.add(field_0x3c);
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x270[field_0x268] < 0) {
            field_0x268 = 0;
        }
        field_0x25c = field_0x270[field_0x268];
        field_0x264 = field_0x274[field_0x268];
    }
}

void cls_804ADF28::fn_80363F7C() {
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x268 >= 6) {
            field_0x268 = 0;
        }
        field_0x25c = lbl_80474B88[field_0x268];
        field_0x264 = lbl_80474BA0[field_0x268];
    }
    mPos.add(field_0x3c);
}

// FIXME(shotgun): 94.00 %, missing/extra + reloc-name; behaviour drift.
void cls_804ADF28::fn_80363880() {
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    s32 slot = field_0x24c;
    if (slot >= 0) {
        ActorBase_804A5770* owl = getActorManager_801F856C()->mActors[slot];
        if (owl != NULL) {
            JGeometry::TVec3f target = owl->mPos;
            if (PSVECDistance(&mPos, &target) >= 100.0f) {
                field_0x24c = -1;
                field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                field_0x264 = 1;
                field_0x268 = 0;
                field_0x248 = 0x20;
            } else {
                target.z = 0.0f;
                if (PSVECDistance(&mPos, &target) >= 10.0f) {
                    field_0x240 = (s16)(JMath::atanTable_.atan2_(target.y - mPos.y, target.x - mPos.x) * 10430.378f);
                    field_0x3c.x = 0.5f * sincos.cosShort(field_0x240);
                    field_0x3c.y = 0.5f * sincos.sinShort(field_0x240);
                }
                u16 angle = (s32)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
                if (angle >= 0x2000 && angle < 0x6000) {
                    field_0x23c = 0;
                } else if (angle >= 0x6000 && angle < 0xa000) {
                    field_0x23c = 2;
                } else if (angle >= 0xa000 && angle < 0xe000) {
                    field_0x23c = 1;
                } else {
                    field_0x23c = 3;
                }
                field_0x26c = lbl_80474858[field_0x23c];
            }
        } else {
            field_0x24c = -1;
            field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
            field_0x264 = 1;
            field_0x268 = 0;
            field_0x248 = 0x20;
        }
    } else {
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            EndingOwl_804ADF28* actor = static_cast<EndingOwl_804ADF28*>(getActorManager_801F856C()->mActors[i]);
            if (actor != NULL && actor->mActorId == ACTOR_EOWL && actor->mAlive == true && actor->field_0x278 == 0) {
                if (PSVECDistance(&mPos, &actor->mPos) >= 100.0f) {
                    break;
                }
                field_0x24c = i;
                JGeometry::TVec3f target = actor->mPos;
                field_0x240 = (s16)(JMath::atanTable_.atan2_(actor->mPos.y - mPos.y, actor->mPos.x - mPos.x) * 10430.378f);
                field_0x3c.x = 0.8f * sincos.cosShort(field_0x240);
                field_0x3c.y = 0.8f * sincos.sinShort(field_0x240);
                u16 angle = (s32)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
                if (angle >= 0x2000 && angle < 0x6000) {
                    field_0x23c = 0;
                } else if (angle >= 0x6000 && angle < 0xa000) {
                    field_0x23c = 2;
                } else if (angle >= 0xa000 && angle < 0xe000) {
                    field_0x23c = 1;
                } else {
                    field_0x23c = 3;
                }
                field_0x26c = lbl_80474858[field_0x23c];
                field_0x270 = lbl_80542AD8;
                field_0x274 = lbl_80542AC8;
                field_0x25c = field_0x270[0];
                field_0x264 = field_0x274[0];
                field_0x258 = 0;
                break;
            }
        }
        if (field_0x24c < 0) {
            if (field_0x248 == 0) {
                s32 r = fn_801CD664(3);
                if (r == 0) {
                    field_0x24c = -1;
                    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                    field_0x264 = 1;
                    field_0x268 = 0;
                    field_0x248 = 0x20;
                } else {
                    if (field_0x3c.squared() <= 32.0f * FLT_EPSILON) {
                        field_0x240 = fn_801CD664(0x10000);
                    } else if (r == 1) {
                        field_0x240 += 0x1000;
                    } else {
                        field_0x240 -= 0x1000;
                    }
                    field_0x3c.x = 0.5f * sincos.cosShort(field_0x240);
                    field_0x3c.y = 0.5f * sincos.sinShort(field_0x240);
                    field_0x248 = 0x40;
                }
            }
            if (PSVECDistance(&mPos, &field_0x27c[0]) >= 60.0f) {
                field_0x248 = 0x80;
                field_0x240 = (s16)(JMath::atanTable_.atan2_(field_0x27c[0].y - mPos.y, field_0x27c[0].x - mPos.x) * 10430.378f);
                field_0x3c.x = 0.5f * sincos.cosShort(field_0x240);
                field_0x3c.y = 0.5f * sincos.sinShort(field_0x240);
            }
        }
    }
    if (!(field_0x3c.squared() <= 32.0f * FLT_EPSILON)) {
        u16 angle = (s32)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
        field_0x26c = lbl_80474858[field_0x23c];
        mPos.add(field_0x3c);
        if (field_0x25c == 0) {
            field_0x268++;
            if (field_0x270[field_0x268] < 0) {
                field_0x268 = 0;
            }
            field_0x25c = field_0x270[field_0x268];
            field_0x264 = field_0x274[field_0x268];
        }
    }
}

// FIXME(shotgun): 95.59 %, missing/extra + reloc-name; behaviour drift.
void cls_804ADF28::fn_80363664() {
    if (field_0x248 == 0) {
        if (!(field_0x3c.squared() <= 32.0f * FLT_EPSILON)) {
            s32 oldFrame = field_0x26c;
            do {
                field_0x23c = fn_801CD664(4);
                field_0x3c = lbl_8050C088[field_0x23c];
                field_0x248 = fn_801CD664(20) + 30;
                field_0x26c = lbl_804745B8[field_0x23c + 20];
            } while (field_0x26c == oldFrame);
        } else {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x248 = fn_801CD664(10) + 15;
            field_0x264 = 0;
        }
    }
    if (field_0x3c.squared() > 32.0f * FLT_EPSILON) {
        if (fn_801F2618(NULL, false)) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x248 = fn_801CD664(10) + 15;
            field_0x264 = 0;
        } else {
            mPos.add(field_0x3c);
            if (field_0x25c == 0) {
                field_0x268++;
                if (field_0x270[field_0x268] < 0) {
                    field_0x268 = 0;
                }
                field_0x25c = field_0x270[field_0x268];
                field_0x264 = field_0x274[field_0x268];
            }
        }
    }
}

// FIXME(shotgun): 99.15 %, missing/extra + reloc-name; behaviour near.
void cls_804ADF28::fn_80363360() {
    if (field_0x248 == 0) {
        s32 r = fn_801CD664(3);
        if (r == 0) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x264 = 0;
            field_0x268 = 0;
            field_0x248 = 0x20;
        } else {
            if (field_0x3c.squared() <= 32.0f * FLT_EPSILON) {
                field_0x240 = fn_801CD664(0x10000);
            } else if (r == 1) {
                field_0x240 += 0x1000;
            } else {
                field_0x240 -= 0x1000;
            }
            field_0x3c.x = 0.3f * JMath::sincosTable_.cosShort(field_0x240);
            field_0x3c.y = 0.3f * JMath::sincosTable_.sinShort(field_0x240);
            field_0x248 = 0x40;
        }
    }
    if (PSVECDistance(&mPos, &field_0x27c[0]) >= 45.0f) {
        field_0x248 = 0x80;
        field_0x240 = (s16)(JMath::atanTable_.atan2_(field_0x27c[0].y - mPos.y, field_0x27c[0].x - mPos.x) * 10430.378f);
        field_0x3c.x = 0.3f * JMath::sincosTable_.cosShort(field_0x240);
        field_0x3c.y = 0.3f * JMath::sincosTable_.sinShort(field_0x240);
    }
    if (field_0x3c.squared() > 32.0f * FLT_EPSILON) {
        u16 angle = (s16)(JMath::atanTable_.atan2_(field_0x3c.y, field_0x3c.x) * 10430.378f);
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
        field_0x26c = lbl_804745A8[field_0x23c];
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x268++;
            if (field_0x268 >= 4) {
                field_0x268 = 0;
            }
            field_0x264 = lbl_80474BC4[field_0x268];
        }
        mPos.add(field_0x3c);
    }
}

void cls_804ADF28::fn_8036335C() {}

// FIXME(shotgun): 84.35 %, missing/extra + reloc-name; behaviour drift.
void cls_804ADF28::fn_80362C44() {
    bool skip = false;
    JGeometry::TVec3f pos = mPos;
    switch (field_0x238) {
    case 0:
        if (field_0x248 == 0) {
            field_0x26c = 0x9b;
            field_0x25c = field_0x260;
            field_0x3c.x = 0.6f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x2f8 = 0.0f;
            field_0x238++;
        }
        break;
    case 1:
        mPos.add(field_0x3c);
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x264++;
            field_0x264 &= 7;
        }
        field_0x2f8 += field_0x3c.x;
        if (field_0x2f8 >= 160.0f) {
            field_0x238++;
            field_0x3c.x = 0.0f;
            field_0x3c.y = -0.6f;
            field_0x3c.z = 0.0f;
            field_0x26c = 0xb3;
            field_0x2f8 = 0.0f;
        }
        break;
    case 2:
        mPos.add(field_0x3c);
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x264++;
            field_0x264 &= 7;
        }
        field_0x2f8 += fabs(field_0x3c.y);
        if (field_0x2f8 >= 160.0f) {
            field_0x238++;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x264 = 0;
            field_0x248 = 0x3c;
        }
        break;
    case 3:
        if (field_0x248 == 0) {
            field_0x248 = 0x5c;
            field_0x2fc->field_0x294 = 1;
            field_0x238++;
        }
        break;
    case 4:
        if (field_0x248 != 0) {
            if (field_0x248 >= 0x3c) {
                pos.y -= (f32)(0x20 - (field_0x248 - 0x3c));
            } else {
                pos.y -= 32.0f;
            }
            if (field_0x248 == 1) {
                field_0x3c.x = 0.0f;
                field_0x3c.y = -0.4f;
                field_0x3c.z = 0.0f;
                field_0x26c = 0xb3;
                field_0x2f8 = 0.0f;
            }
        } else {
            mPos.add(field_0x3c);
            if (field_0x25c == 0) {
                field_0x25c = field_0x260;
                field_0x264++;
                field_0x264 &= 7;
            }
            skip = true;
            field_0x2f8 += fabs(field_0x3c.y);
            if (field_0x2f8 >= 80.0f) {
                field_0x248 = 300;
                field_0x250 = (f32)gGameRoot2_8053AB10->field_0x2c->field_0xc;
                field_0x254 = field_0x250 / (f32)field_0x248;
                JGeometry::TVec3f effectPos;
                effectPos.x = 0.0f;
                effectPos.y = 0.0f;
                effectPos.z = 0.0f;
                // FIXME(shotgun): the original positions the first four effects from mPos
                // (a stack-converted fctiwz pair); written with the plain position here.
                static const u16 lbl_80542AF0[4] = {0x4A4, 0x4A5, 0x4A6, 0x4A7};
                for (int i = 0; i < 4; i++) {
                    spawnEmitter_8013CC50(0, &effectPos, lbl_80542AF0[i], 0, 0, NULL, NULL, true, 1.0f);
                    deleteEmitter(&field_0x30c[i]);
                }
                spawnEmitter_8013CC50(0, &effectPos, 0x55a, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55b, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55c, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55d, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55e, 0, 0, NULL, NULL, true, 1.0f);
                field_0x304 = spawnEmitter_8013CC50(0, &effectPos, 0x558, 0, 0, NULL, NULL, true, 1.0f);
                field_0x308 = spawnEmitter_8013CC50(0, &effectPos, 0x559, 0, 0, NULL, NULL, true, 1.0f);
                field_0x238++;
            }
        }
        break;
    case 5:
        skip = true;
        if (field_0x250 > 0.0f) {
            field_0x250 -= field_0x254;
            if (field_0x250 <= 0.0f) {
                field_0x250 = 0.0f;
                field_0x254 = 0.0f;
            }
        }
        gGameRoot2_8053AB10->field_0x2c->field_0xc = (u8)field_0x250;
        if (field_0x248 == 0) {
            fn_80119DD0()->fn_80119958();
            gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
            field_0x238++;
            field_0x244 = 0;
            field_0x248 = 500;
        }
        break;
    case 6:
        field_0x244++;
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
        if (field_0x248 == 0) {
            skip = true;
            for (int i = 0; i < 4; i++) {
                if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x100) != 0) {
                    field_0x238++;
                    break;
                }
            }
        }
        break;
    case 7:
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
        skip = true;
        deleteEmitter(&field_0x304);
        deleteEmitter(&field_0x308);
        gGameRoot_8053AAF8->fn_8013670C(1);
        field_0x238++;
        break;
    default:
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
        skip = true;
        break;
    }
    if (!skip) {
        field_0x2fc->mPos = pos;
    }
}

// FIXME(shotgun): 99.33 %, reloc-name; behaviour clean.
void cls_804ADF28::fn_80362B74() {
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x26c + field_0x264, NULL, 7, 1.0f, 1.0f);
    JGeometry::TVec3f savedPos = mPos;
    for (int i = 0; i < 3; i++) {
        mPos = field_0x27c[i];
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x2e8[i] + field_0x264, NULL, 7, 1.0f, 1.0f);
    }
    mPos = savedPos;
}

// FIXME: 99.58 %, reloc-name; behaviour clean.
// Instructions identical; only the relocation of the "fune.bti" string is @NNN here
// against lbl_80474C10 (the unit's rodata is not complete yet).
void cls_804ADF28::fn_80362B14() {
    void* res = fn_804032E8('TIMG', "fune.bti");
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, NULL);
    }
}

// FIXME(shotgun): 91.48 %, missing/extra + reloc-name; behaviour drift.
void cls_804ADF28::fn_803628D4() {
    JGeometry::TVec3f savedPos = mPos;
    Mtx mtx;
    SpriteParams_801F5AE0 params;
    if ((field_0x268 & 7) >= 5) {
        static const f32 lbl_80474BB8[3] = {6.0f, 8.0f, 6.0f};
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[0][3] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = -1.0f;
        mtx[2][3] = 0.0f;
        params.mMtx = mtx;
        mPos.y -= lbl_80474BB8[(field_0x268 & 7) - 5];
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x26c + field_0x264, &params, -1, 1.0f, 1.0f);
    mPos = savedPos;
    Mtx identity;
    identity[0][0] = 1.0f;
    identity[0][1] = 0.0f;
    identity[0][2] = 0.0f;
    identity[1][0] = 0.0f;
    identity[1][1] = 1.0f;
    identity[1][2] = 0.0f;
    identity[2][0] = 0.0f;
    identity[2][1] = 0.0f;
    identity[2][2] = 1.0f;
    params.mMtx = identity;
    s32 frame = field_0x26c + field_0x264;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
    }
}

// FIXME(shotgun): 99.19 %, operands + reloc-name; behaviour drift.
void cls_804ADF28::fn_80362680() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1f = 0;
    if (field_0x234 == 0x10 || field_0x234 == 8) {
        params.field_0x1d = 1;
    }
    if (field_0x234 == 4) {
        if (field_0x238 == 0x1d || field_0x238 == 0x1e) {
            params.field_0x26 = 0;
        }
        if (field_0x238 >= 0x26 && field_0x238 <= 0x29) {
            params.field_0x10.a = 0x66;
        }
    }
    if (field_0x278 == 1) {
        mtx[0][0] = -1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[0][3] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.0f;
        mtx[2][3] = 0.0f;
        params.mMtx = mtx;
    }
    if (field_0x258 == 0) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x26c + field_0x264, &params, 7, 1.0f, 1.0f);
    } else if (field_0x258 == 1) {
        drawSprite_801F06F0("enemy_both_sob.bin", field_0x26c + field_0x264, &params, 7, 1.0f, 1.0f);
    } else if (field_0x258 == 2) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x26c + field_0x264, &params, 7, 1.0f, 1.0f);
    }
}

void cls_804ADF28::fn_80362634() {
    deleteEmitter(&field_0x300);
}
