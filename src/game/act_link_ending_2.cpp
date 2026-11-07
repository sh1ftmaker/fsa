#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/GameGlobals.h"
#include "game/act/act_link_ending_2.h"
#include "game/act/act_kaepora_gaebora_ending.h"
#include "game/act/tu_802E9428.h"
#include "game/act/tu_80362634.h"

// Sparkle effect ids, indexed by a random 0-4 (update).
static const u16 lbl_80474EC0[5] = {0x56E, 0x570, 0x56D, 0x571, 0x56F};

// FIXME(shotgun): 83.8 %, different
Link2Ending_804AE338::~Link2Ending_804AE338() {
    fn_8036E544();
}

// FIXME(shotgun): 67.0 %, missing/extra
void Link2Ending_804AE338::init() {
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x238 = -1;
    field_0x23C = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24C = 0;
    field_0x2A8 = NULL;
    field_0x2AC = 0;
    field_0x2B0 = 0;
    for (s32 i = 0; i < 20; i++) {
        field_0x258[i] = NULL;
    }
    field_0x2B4 = 0;
    field_0x254 = NULL;
    field_0x250 = NULL;
    field_0x230 = 0;
    field_0x3c.x = 0.0f;
    field_0x3c.y = -0.35f;
    field_0x3c.z = 0.0f;
    field_0x24C = 0x18;
    field_0x240 = 10;
    field_0x23C = 10;
    field_0x244 = 0;
    fn_8023077C();
    fn_80230498(0, &mPos, field_0x24C + field_0x244, 0xFF, -0x80, -0x80, true);
    for (s32 i = 1; i < 4; i++) {
        fn_80230498(i, &mPos, 0, 0, -0x80, -0x80, true);
    }
    JGeometry::TVec3f pos;
    pos.z = 0.0f;
    pos.y = 384.0f * (s32)(mPos.y / 384.0f) + 192.0f;
    pos.x = 512.0f * (s32)(mPos.x * 0.001953125f) + 256.0f;
    field_0x2A8 = spawnEmitter_8013CC50(field_0x4, &pos, 0x274, 0, 0, NULL, NULL, true, 1.0f);
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_EOWL && actor->mAlive == true) {
            field_0x250 = (KaeporaGaeboraEnding_804ADAC8*)actor;
            break;
        }
    }
    end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_ENDM && actor->mAlive == true) {
            field_0x254 = (UnknownENDM_804AA0FC*)actor;
            break;
        }
    }
}

// FIXME(shotgun): 88.8 %, missing/extra
void Link2Ending_804AE338::update() {
    if (field_0x234 != 0) {
        field_0x234--;
    }
    if (field_0x23C != 0) {
        field_0x23C--;
    }
    if (field_0x2B0 != 0) {
        field_0x2B0--;
    }
    field_0x250->mPos = mPos;
    s32 live = 0;
    JGeometry::TVec3f center;
    center.z = 0.0f;
    center.x = JUTVideo::getManager()->getFbWidth() * 0.5f;
    center.y = JUTVideo::getManager()->getEfbHeight() * 0.5f;
    for (s32 i = 0; i < 20; i++) {
        if (field_0x258[i] != NULL) {
            live++;
            if (field_0x258[i]->isEnableDeleteEmitter()) {
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x258[i]);
                field_0x258[i] = NULL;
            }
        }
    }
    if (live >= field_0x2B4) {
        field_0x2B4 = live;
    }
    JGeometry::TVec3f pos;
    switch (field_0x230) {
    case 0: {
        mPos += field_0x3c;
        if (field_0x23C == 0) {
            field_0x23C = field_0x240;
            field_0x244++;
            if (field_0x244 >= 8) {
                field_0x244 = 0;
            }
        }
        if (field_0x238 < 0) {
            s32 end = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < end; i++) {
                cls_804ADF28* actor = (cls_804ADF28*)getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mActorId == ACTOR_EETC && actor->field_0x234 == 4 &&
                    (actor->field_0x238 == 0x26 || actor->field_0x238 == 0x27 || actor->field_0x238 == 0x28 ||
                     actor->field_0x238 == 0x29)) {
                    field_0x238 = i;
                    break;
                }
            }
        }
        s32 slot = field_0x238;
        if (slot >= 0) {
            if (mPos.y - getActorManager_801F856C()->mActors[slot]->mPos.y < 20.0f) {
                field_0x230++;
                field_0x234 = 50;
                field_0x244 = 0;
            }
        }
        break;
    }
    case 1:
        if (field_0x234 == 0) {
            pos = center + JGeometry::TVec3f(180.0f, -64.0f, 0.0f);
            field_0x24C = 0x10;
            field_0x244 = 0;
            field_0x230++;
            field_0x234 = 0x41;
            spawnSparkles(pos);
        }
        break;
    case 2:
    case 3:
        if (field_0x234 != 0) {
            return;
        }
        if (field_0x230 == 2) {
            pos = center + JGeometry::TVec3f(-160.0f, 0.0f, 0.0f);
            field_0x234 = 0x41;
            spawnSparkles2(pos, 1);
        } else {
            pos = center + JGeometry::TVec3f(60.0f, 80.0f, 0.0f);
            field_0x234 = 0x5A;
            spawnSparkles(pos);
        }
        field_0x230++;
        break;
    case 4:
        if (field_0x234 != 0) {
            return;
        }
        field_0x230++;
        field_0x234 = 0x91;
        field_0x2B0 = 0;
    case 5:
        if (field_0x254->field_0x234 >= 0x4A49) {
            field_0x250->field_0x230 = 3;
            field_0x254->field_0x231 = 1;
            field_0x230++;
        }
    case 6:
        if (field_0x2B0 == 0) {
            s32 area;
            do {
                area = fn_801CD664(4);
            } while (area == field_0x2AC);
            static JGeometry::TBox2f sAreas[4] = {
                JGeometry::TBox2f(30.0f, 30.0f, 274.0f, 129.0f),
                JGeometry::TBox2f(334.0f, 30.0f, 578.0f, 129.0f),
                JGeometry::TBox2f(30.0f, 139.0f, 274.0f, 269.0f),
                JGeometry::TBox2f(304.0f, 139.0f, 578.0f, 269.0f),
            };
            JGeometry::TBox2f box = sAreas[area];
            JGeometry::TVec3f p;
            bool inside;
            do {
                f32 y = fn_801CD664(0x1A2);
                f32 x = fn_801CD664(0x242);
                p.y = y;
                p.z = 0.0f;
                p.x = x;
                inside = false;
                if (x >= box.i.x && y >= box.i.y && x < box.f.x && y < box.f.y) {
                    inside = true;
                }
            } while (!inside);
            s32 kind = fn_801CD664(8);
            bool spawned;
            if (kind >= 5) {
                spawned = spawnSparkles(p);
            } else {
                spawned = spawnSparkles2(p, kind);
            }
            if (spawned == true) {
                field_0x2B0 = fn_801CD664(6) + 0x1E;
                field_0x2AC = area;
            } else {
                field_0x2B0 = 0;
            }
        }
        break;
    }
    fn_80230498(0, &mPos, field_0x24C + field_0x244, 0xFF, -0x80, -0x80, true);
}

inline bool Link2Ending_804AE338::spawnSparkles(JGeometry::TVec3f pos) {
    s32 slots[3] = {0, 0, 0};
    s32 n = 0;
    for (s32 i = 0; i < 20; i++) {
        if (field_0x258[i] == NULL) {
            slots[n++] = i;
            if (n == 3) {
                break;
            }
        }
    }
    if (n != 3) {
        return false;
    }
    field_0x258[slots[0]] = spawnEmitter_8013CC50(0, &pos, 0x569, 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x258[slots[0]] != NULL) {
        field_0x258[slots[0]]->setStatus(JPAEMIT_Immortal);
    }
    field_0x258[slots[1]] = spawnEmitter_8013CC50(0, &pos, 0x56A, 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x258[slots[1]] != NULL) {
        field_0x258[slots[1]]->setStatus(JPAEMIT_Immortal);
    }
    field_0x258[slots[2]] = spawnEmitter_8013CC50(0, &pos, 0x56B, 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x258[slots[2]] != NULL) {
        field_0x258[slots[2]]->setStatus(JPAEMIT_Immortal);
    }
    return true;
}

inline bool Link2Ending_804AE338::spawnSparkles2(JGeometry::TVec3f pos, s32 kind) {
    s32 slots[2] = {0, 0};
    s32 n = 0;
    for (s32 i = 0; i < 20; i++) {
        if (field_0x258[i] == NULL) {
            slots[n++] = i;
            if (n == 2) {
                break;
            }
        }
    }
    if (n != 2) {
        return false;
    }
    field_0x258[slots[0]] = spawnEmitter_8013CC50(0, &pos, 0x56C, 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x258[slots[0]] != NULL) {
        field_0x258[slots[0]]->setStatus(JPAEMIT_Immortal);
    }
    field_0x258[slots[1]] = spawnEmitter_8013CC50(0, &pos, lbl_80474EC0[kind], 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x258[slots[1]] != NULL) {
        field_0x258[slots[1]]->setStatus(JPAEMIT_Immortal);
    }
    return true;
}

void Link2Ending_804AE338::draw() {}

// FIXME(shotgun): 68.6 %, missing/extra
void Link2Ending_804AE338::fn_8036E544() {
    for (s32 i = 0; i < 20; i++) {
        if (field_0x258[i] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x258[i]);
            field_0x258[i] = NULL;
        }
    }
    if (field_0x2A8 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x2A8);
        field_0x2A8 = NULL;
    }
}
