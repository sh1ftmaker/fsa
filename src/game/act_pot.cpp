#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/ViewLayer.h"
#include "game/act/act_pot.h"
#include "std/math.h"

// x offsets [0..4] and y offsets [5..9] of the 5 flames spawned on layers >= 8;
// x offsets [10..22] and y offsets [23..35] of the 13 flames spawned below.
static const f32 lbl_8046E070[36] = {
    0.0f, -16.0f, 0.0f, 16.0f, 0.0f,
    -16.0f, 0.0f, 0.0f, 0.0f, 16.0f,
    0.0f, -16.0f, 0.0f, 16.0f, -32.0f, -16.0f, 0.0f, 16.0f, 32.0f, -16.0f, 0.0f, 16.0f, 0.0f,
    -32.0f, -16.0f, -16.0f, -16.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 16.0f, 16.0f, 16.0f, 32.0f,
};

// The pot's trail emitter: replace it with the one for movement direction `dir`
// (the five direction cases of update() repeat this).
static inline void respawnEmitter(Pot_804A9428* pot, s32 dir, s32 effect) {
    if (pot->field_0x244 != dir) {
        deleteEmitter(&pot->mEmitter);
        pot->mEmitter = spawnEmitter_8013CC50(pot->field_0x4, &pot->mPos, effect, 0, 0, NULL, NULL, true, 1.0f);
        pot->field_0x244 = dir;
    }
}

// Spawn a fire actor (BLZE) on the tile under `pos`, as the burning pot's loops below do.
static inline void igniteTile(Pot_804A9428* pot, JGeometry::TVec3f* pos, JGeometry::TVec3f* tilePos) {
    s32 tileIdx = fn_80226AE0(pos, pot->field_0x4);
    gGameRoot2_8053AB10->field_0x20->fn_802F9908(pot->field_0x4, tileIdx);
    fn_80226CA8(tileIdx, pot->field_0x4, tilePos);
    tilePos->x += 8.0f;
    tilePos->y += 8.0f;
    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLZE, pot->field_0x4, tilePos, 0xF, -1, -1);
}

// Spawn a rupee actor of `kind` (the low bits of its parameter); 0xA00 is added while the pot is above the ground (mPos.z > 0).
static inline void spawnRupy(Pot_804A9428* pot, JGeometry::TVec3f* pos, s32 kind) {
    if (pot->mPos.z > 0.0f) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, pot->field_0x4, pos, 0x260A00 + kind, -1, -1);
    } else {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, pot->field_0x4, pos, 0x260000 + kind, -1, -1);
    }
}

void Pot_804A9428::init() {
    field_0x240 = field_0x90 & 0x7F;
    field_0x234 = (field_0x90 >> 7) & 0x1F;
    field_0x238 = 0;
    field_0x23c = 0;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -4.0f;
    field_0x80.i.y = -4.0f;
    field_0x80.f.x = 4.0f;
    field_0x80.f.y = 4.0f;
    field_0x254 = 0xFF;
    field_0x94 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    mEmitter = NULL;
    field_0x244 = 0;
    if (field_0x234 == 1) {
        mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A8, 0, 0, NULL, NULL, true, 1.0f);
    }
    field_0x250 = 4;
}

Pot_804A9428::~Pot_804A9428() {
    if (fn_8022461C(&mPos, field_0x4) != 0xD) {
        lbl_804C0EA0[0] = mPos.x;
        lbl_804C0EA0[1] = mPos.y;
        lbl_804C0EA0[2] = mPos.z;
        lbl_8053AB68 = field_0x4;
    }
    if (mEmitter) {
        releaseEmitter_801EE434(mEmitter);
    }
}

// FIXME: 99.01 % (missing/extra: duplicate null check in blz2 loop, emitter direction branch order)
void Pot_804A9428::update() {
    field_0x254 = 0xFF;
    if (field_0xc4) {
        mPos.set(*fn_8023E7B0(field_0xc8));
        field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
        field_0x254 = fn_8023BCD4(field_0xc8);
        if (field_0x234 == 1) {
            field_0x250--;
            if (field_0x250 == 0) {
                field_0x250 = 4;
                field_0x23c++;
                if (field_0x23c >= 3) {
                    field_0x23c = 0;
                }
            }
            if (field_0x4 >= 8) {
                field_0x238 = field_0x23c + 2;
            } else {
                field_0x238 = 2;
            }
        }
    } else {
        s32 damage = field_0x114;
        if (gGameRoot_8053AAF8->field_0x7f) {
            damage = 0;
        }
        if (!viewLayerFlag(field_0x4)) {
            field_0x3c.z += -0.15f;
            mPos += field_0x3c;
            if (mPos.z < 0.0f || fn_801F2618(NULL, false)) {
                if (mPos.z < 0.0f) {
                    if (fn_80226850(&mPos, field_0x4) == 0x90) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
                    } else if (fn_80226850(&mPos, field_0x4) == 0x91 || fn_80226850(&mPos, field_0x4) == 0x22E) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
                    }
                }
                if (field_0x234 == 2) {
                    ActorBase_804A5770* actors[ACTOR_MANAGER_MAX_ACTORS];
                    s32 count = getActorManager_801F856C()->fn_801F7818('BLZ2', field_0x4, actors, -1);
                    for (s32 i = 0; i < count; i++) {
                        ActorBase_804A5770* actor = actors[i];
                        if (actor != NULL) {
                            f32 dy = actor->mPos.y - mPos.y;
                            f32 dx = actor->mPos.x - mPos.x;
                            if (std::sqrtf(dx * dx + dy * dy) < 40.0f) {
                                actor->field_0x94 = 2;
                            }
                        }
                    }
                }
                vfunc_0x24();
            } else if (fn_801F2B7C(damage, field_0xc8, 1) || fn_801F2B20(0)) {
                vfunc_0x24();
            }
        } else {
            if (!field_0x24c) {
                mPos.y -= mPos.z;
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x24c = 1;
            }
            field_0x3c.y += 0.15f;
            mPos += field_0x3c;
            if (fn_801F2B7C(damage, field_0xc8, 1) || fn_801F2B20(0) || fn_801F2618(NULL, false)) {
                vfunc_0x24();
            }
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    if (field_0x234 == 1) {
        if (mPos.x != mPrevPos.x || mPos.y != mPrevPos.y) {
            if (mPos.x < mPrevPos.x) {
                respawnEmitter(this, 1, 0x1A7);
            } else if (mPos.x > mPrevPos.x) {
                respawnEmitter(this, 2, 0x1A9);
            } else if (mPos.y < mPrevPos.y) {
                respawnEmitter(this, 3, 0x1AA);
            } else {
                respawnEmitter(this, 4, 0x1A6);
            }
        } else {
            respawnEmitter(this, 0, 0x1A8);
        }
        if (mEmitter != NULL) {
            setEmitterPos_8013CB44(mEmitter, &mPos, 0, 1, true);
            mEmitter->setGlobalAlpha(field_0x254);
        }
    }
}

// FIXME: 91.51 % (scheduling+regalloc: frame base addition operand order)
void Pot_804A9428::draw() {
    s32 flags = 7;
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = field_0x254;
    if (field_0xc4) {
        params.field_0x20 = 0;
        flags = -1;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    if (viewLayerFlag(field_0x4)) {
        fn_801F02BC("enemy_both2_sob.bin", field_0x234 + field_0x238 + 0x1BB, &params, flags, 0, true, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_both2_sob.bin", field_0x234 + field_0x238 + 0x21, &params, flags, 0, true, 1.0f, 1.0f);
    }
    if (field_0x234 == 1) {
        drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 24.0f);
    }
}

// FIXME: 95.67 % (missing/extra: stack frame size 0xA0 vs target 0x80; local vector layout)
void Pot_804A9428::vfunc_0x24() {
    JGeometry::TVec3f pos(mPos);
    if (field_0x234 == 0) {
        pos.z = 0.0f;
        if (field_0x4 >= 8) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &pos, 0, -1, -1);
        }
        if (field_0x240 <= 1) {
            s32 r = fn_801CD664(100);
            if ((getGameView_802D6A9C()->field_0x4 == 5 && getGameView_802D6A9C()->field_0x8 == 3) ||
                (getGameView_802D6A9C()->field_0x4 == 3 && getGameView_802D6A9C()->field_0x8 == 3)) {
                s32 type;
                if (r < 25) {
                    type = 0;
                } else if (r < 50) {
                    type = 1;
                } else if (r < 75) {
                    type = 2;
                } else {
                    type = 1;
                }
                spawnRupy(this, &pos, type);
            } else if (r <= 25) {
                if (r & 1) {
                    spawnRupy(this, &pos, 0);
                } else {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
                }
            }
        } else if (field_0x240 == 2) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
        } else if (field_0x240 == 3) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_MTBO, field_0x4, &pos, 0, -1, -1);
        } else if (field_0x240 == 4) {
            JGeometry::TVec3f heartPos(pos.x + (f32)(fn_801CD664(13) - 6), pos.y, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &heartPos, 0, -1, -1);
            heartPos.x = pos.x + (f32)(fn_801CD664(13) - 6);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &heartPos, 0, -1, -1);
        } else if (field_0x240 == 5) {
            if (fn_801CD664(100) < 60) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
            }
        }
        if (fn_8022461C(&mPos, field_0x4) == 0xD || fn_8022461C(&mPos, field_0x4) == 0x39) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30022, 0);
        } else if (fn_8022461C(&mPos, field_0x4) == 0xE) {
            if (fn_802DE9A0()) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, 0, 0, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x5005F, 0);
            } else if (fn_802DEB58()) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x45E, -1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x45F, 0, 0, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x5003F, 0);
            }
        } else {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xAB, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xAC, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x3000B, 0);
        }
    } else if (field_0x234 == 1) {
        startSound_801F0E34(0x3000B, 0);
        JGeometry::TVec3f tilePos;
        if (field_0x4 >= 8) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &pos, 0, -1, -1);
            for (int i = 0; i < 5; i++) {
                pos.x = mPos.x + lbl_8046E070[i];
                pos.y = mPos.y + lbl_8046E070[i + 5];
                pos.z = 0.0f;
                igniteTile(this, &pos, &tilePos);
            }
        } else {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A3, 0, -1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A4, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A4, 0, 0, NULL, NULL, true, 1.0f);
            for (int i = 0; i < 13; i++) {
                pos.x = mPos.x + lbl_8046E070[10 + i];
                pos.y = mPos.y + lbl_8046E070[23 + i];
                pos.z = 0.0f;
                igniteTile(this, &pos, &tilePos);
            }
        }
    } else {
        if (field_0x4 >= 8) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &pos, 5, -1, -1);
        } else {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x4EF, 0, -1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F1, 0, 0, NULL, NULL, true, 1.0f);
        }
        startSound_801F0E34(0x3000B, 0);
    }
    mAlive = false;
}

bool Pot_804A9428::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return fn_801F132C(layer, pos, &field_0x248);
}

void Pot_804A9428::vfunc_0x48() {
    vfunc_0x24();
}
