#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_grass.h"

Grass_804A92D0::~Grass_804A92D0() {}

void Grass_804A92D0::vfunc_0x48() {
    vfunc_0x24();
}

bool Grass_804A92D0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return fn_801F132C(layer, pos, &field_0x23c);
}

void Grass_804A92D0::fn_802CEC10() {
    if (field_0x234 == 1) {
        if (field_0x230 == 4) {
            if (fn_801CD664(100) < 60) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
            }
        } else if (field_0x238 == 3) {
            // FIXME: 99.27 %. Only the float registers of the first jittered
            // position differ (the original has the constant in f2 and the
            // loaded mPos.x in f3; ours swaps them).
            JGeometry::TVec3f a(mPos.x + (fn_801CD664(13) - 6), mPos.y, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &a, 0, -1, -1);
            JGeometry::TVec3f b(mPos.x + (fn_801CD664(13) - 6), mPos.y, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &b, 0, -1, -1);
        } else if (field_0x238 == 2) {
            s32 dy = fn_801CD664(13) - 6;
            JGeometry::TVec3f c(mPos.x + (fn_801CD664(13) - 6), mPos.y + dy, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &c, 0x262D01, -1, -1);
        } else {
            s32 dy = fn_801CD664(13) - 6;
            JGeometry::TVec3f d(mPos.x + (fn_801CD664(13) - 6), mPos.y + dy, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &d, 0x262D00, -1, -1);
        }
    } else if (field_0x238 != 2 && !fn_802DEBF8()) {
        if (field_0x230 < 1) {
            s32 r = fn_801CD664(100);
            if (field_0x238 == 0) {
                if (r >= 40 && r <= 60) {
                    if (r & 1) {
                        if (mPos.z > 0.0f) {
                            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 0x260A00, -1, -1);
                        } else {
                            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 0x260000, -1, -1);
                        }
                    } else {
                        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
                    }
                }
            } else if (r <= 10) {
                if (r & 1) {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 0x260000, -1, -1);
                } else {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
                }
            }
        } else {
            if (field_0x230 == 1) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
            } else if (field_0x230 == 2) {
                getActorManager_801F856C()->spawnActor_801F9484('MTOB', field_0x4, &mPos, 0, -1, -1);
            } else if (field_0x230 == 3) {
                JGeometry::TVec3f e(mPos.x + (fn_801CD664(13) - 6), mPos.y, 0.0f);
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &e, 0, -1, -1);
                JGeometry::TVec3f f(mPos.x + (fn_801CD664(13) - 6), mPos.y, 0.0f);
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &f, 0, -1, -1);
            }
        }
    }
    mAlive = false;
}

void Grass_804A92D0::vfunc_0x24() {
    if (field_0x4 >= 8) {
        JGeometry::TVec3f pos(mPos);
        pos.z = 0.0f;
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &pos, 4, -1, -1);
        if (field_0x238 == 0) {
            startSound_801F0E34(0x30006, 0);
        } else if (field_0x238 == 1) {
            startSound_801F0E34(0x5002F, 0);
        }
    } else if (field_0x238 == 0) {
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
            fn_802CF598();
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x357, 0, 0, NULL, NULL, true, 0.15f);
            startSound_801F0E34(0x30006, 0);
            field_0x234 = 2;
            fn_802CEC10();
        }
    } else if (field_0x238 == 1) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x358, 0, 0, NULL, NULL, true, 0.15f);
        startSound_801F0E34(0x5002F, 0);
        fn_802CEC10();
    }
    mAlive = false;
}

void Grass_804A92D0::draw() {
    if (field_0x238 == 2) {
        return;
    }
    s32 flags = 7;
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = field_0x244;
    if (field_0xc4) {
        params.field_0x20 = 0;
        flags = -1;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    fn_801F02BC("enemy_both2_sob.bin", 0x27, &params, flags, 0, true, 1.0f, 1.0f);
}

bool Grass_804A92D0::fn_802CF598() {
    // FIXME: 98.73 %. The original copies the search result into the
    // previous-serial register (`mr r29, r30`) before the sign test; ours
    // coalesces the two into one register, so that copy is missing.
    s32 found;
    s32 prev = -1;
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x, mPos.y);
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* screen = (layer < 8) ? &view->mVisibleBox : &view->field_0xdc;
    box.addPos(-screen->i.x, -screen->i.y);

    for (;;) {
        found = getActorManager_801F856C()->fn_801F78F4(ACTOR_BLZ2, prev);
        prev = found;
        if (found < 0) {
            return false;
        }
        if (found == mSerial) {
            continue;
        }
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(found);
        if (other == NULL) {
            continue;
        }
        JGeometry::TBox2f otherBox(other->mBox);
        otherBox.addPos(other->mPos.x, other->mPos.y);
        s32 otherLayer = other->field_0x4;
        view = getGameView_802D6A9C();
        screen = (otherLayer < 8) ? &view->mVisibleBox : &view->field_0xdc;
        otherBox.addPos(-screen->i.x, -screen->i.y);
        if (otherBox.intersect(box)) {
            startSound_801F0E34(0x3002F, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x1A2, 1, 0, NULL, NULL, true, 1.0f);
            return true;
        }
    }
}

void Grass_804A92D0::update() {
    if (field_0x238 != 2) {
        field_0x244 = 0xFF;
        if (field_0xc4) {
            mPos.set(*fn_8023E7B0(field_0xc8));
            field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            field_0x244 = fn_8023BCD4(field_0xc8);
        } else {
            s32 layer = field_0x4;
            if (viewLayerFlag(layer) == 0) {
                field_0x3c.z += -0.1f;
                mPos += field_0x3c;
                s32 damage = field_0x114;
                if (gGameRoot_8053AAF8->field_0x7f) {
                    damage = 0;
                }
                if (mPos.z < 0.0f || fn_801F2618(NULL, false)) {
                    vfunc_0x24();
                } else if (fn_801F2B7C(damage, field_0xc8, 1) || fn_801F2B20(0)) {
                    vfunc_0x24();
                }
            } else {
                if (field_0x245 == 0) {
                    mPos.y -= mPos.z;
                    mPos.z = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x245 = 1;
                }
                field_0x3c.y += 0.1f;
                mPos += field_0x3c;
                s32 damage = field_0x114;
                if (gGameRoot_8053AAF8->field_0x7f) {
                    damage = 0;
                }
                if (fn_801F2618(NULL, false)) {
                    vfunc_0x24();
                } else if (fn_801F2B7C(damage, field_0xc8, 1) || fn_801F2B20(0)) {
                    vfunc_0x24();
                }
            }
        }
    } else {
        field_0x240 -= 1;
        if (field_0x240 == 0) {
            s32 tile = fn_80226AE0(&mPos, field_0x4);
            getGameView_802D6A9C()->fn_802D7310(tile, false);
            mAlive = false;
        }
    }
}

void Grass_804A92D0::init() {
    field_0x230 = field_0x90 & 0xF;
    field_0x234 = (field_0x90 >> 16) & 0xF;
    field_0x238 = (field_0x90 >> 20) & 0xF;
    field_0x244 = 0xFF;
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;
    field_0x80.i.x = -4.0f;
    field_0x80.i.y = -4.0f;
    field_0x80.f.x = 4.0f;
    field_0x80.f.y = 4.0f;
    field_0x23c = 0;
    field_0x245 = 0;
    if (field_0x238 == 2) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D1, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D2, 0, 0, NULL, NULL, true, 1.0f);
        field_0x240 = 30;
    }
}
