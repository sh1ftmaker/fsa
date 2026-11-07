#include "game/GameAudio.h"
#include "game/act/act_magic_circle_generator.h"
#include "game/GameView.h"
#include "game/act/act_small_key.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "std/math.h"

static const s32 lbl_8046F270[4] = {0, 0x30, 5, 0x36};

void MagicCircleGenerator_804A9FB0::init() {
    field_0x248 = field_0x90 & 7;
    field_0x24c = 0;
    field_0x264 = 0;
    field_0x254 = 0;
    field_0x268 = 0;
    field_0x258 = 0;
    field_0x26c = 0;
    field_0x25c = 0;
    field_0x270 = 0;
    field_0x260 = 0;
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    mEmitter23c = NULL;
    mEmitter240 = NULL;
    if (!getGameView_802D6A9C()->fn_802D75BC(field_0x16c)) {
        field_0x244 = 0;
    } else if (field_0x248 == 0 || field_0x248 == 3) {
        field_0x244 = 1;
        field_0x24c = 0x14;
    } else {
        mAlive = false;
    }
}

MagicCircleGenerator_804A9FB0::~MagicCircleGenerator_804A9FB0() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        releaseEmitter_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        releaseEmitter_801EE434(mEmitter23c);
    }
    if (mEmitter240 != NULL) {
        releaseEmitter_801EE434(mEmitter240);
    }
}

// FIXME(shotgun): 84.4 %, missing/extra: the box temporaries and the key-search loop are scheduled differently
void MagicCircleGenerator_804A9FB0::update() {
    switch (field_0x244) {
    case 0: {
        field_0x80.i.x = -9.0f;
        field_0x80.i.y = -9.0f;
        field_0x80.f.x = 9.0f;
        field_0x80.f.y = 9.0f;
        JGeometry::TBox2f probe(field_0x80);
        probe.i.x += mPos.x;
        probe.i.y += mPos.y;
        probe.f.x += mPos.x;
        probe.f.y += mPos.y;
        mBox.i.x = -32.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 32.0f;
        JGeometry::TBox2f area;
        area.i.x = mBox.i.x + mPos.x;
        area.i.y = mBox.i.y + mPos.y;
        area.f.x = mBox.f.x + mPos.x;
        area.f.y = mBox.f.y + mPos.y;
        field_0x250 = fn_8023AF14(&area, field_0x4, false, 0.0f);
        if (field_0x250 >= 0) {
            if (mEmitter234 == NULL) {
                JGeometry::TVec3f pos = mPos;
                mEmitter234 = spawnEmitter_8013CC50(field_0x4, &pos, 0x2A7, 1, 0, NULL, NULL, true, 1.0f);
                gGameRoot_8053AAF8->fn_80134DE4(9, 0);
                if (getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 1 &&
                    !getGameView_802D6A9C()->fn_802D46D8(field_0x250, 0x24E, false)) {
                    lbl_8053AED0->fn_8043D67C(0x24E, field_0x250, 0);
                    getGameView_802D6A9C()->fn_802D46D8(field_0x250, 0x24E, true);
                }
            }
            startSound_801F0E34(0x30050, 0);
        } else {
            if (mEmitter230 != NULL) {
                releaseEmitter_801EE434(mEmitter230);
                mEmitter230 = NULL;
            }
            if (mEmitter234 != NULL) {
                releaseEmitter_801EE434(mEmitter234);
                mEmitter234 = NULL;
            }
        }
        fn_802E7DB8();
        if (field_0x244 != 1) {
            mBox.i.x = -20.0f;
            mBox.i.y = -10.0f;
            mBox.f.x = 20.0f;
            mBox.f.y = 40.0f;
            JGeometry::TBox2f keyBox;
            keyBox.i.x = mBox.i.x + mPos.x;
            keyBox.i.y = mBox.i.y + mPos.y;
            keyBox.f.x = mBox.f.x + mPos.x;
            keyBox.f.y = mBox.f.y + mPos.y;
            for (s32 player = 0; player < 4; player++) {
                if (fn_8023B0F4(player, &keyBox, field_0x4, false, 0.0f) == 1 && fn_8023EEB4(player) == ACTOR_KEY0) {
                    SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(
                        getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023EF50(player)));
                    if (key != NULL && key->field_0x280 == 6) {
                        JGeometry::TVec3f pos = key->mPos;
                        field_0x244 = 1;
                        field_0x24c = 0x14;
                        if (field_0x4 >= 8) {
                            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &pos, 0, -1, -1);
                        } else {
                            JGeometry::TVec3f effectPos = pos;
                            if (mEmitter230 != NULL) {
                                releaseEmitter_801EE434(mEmitter230);
                            }
                            if (mEmitter234 != NULL) {
                                releaseEmitter_801EE434(mEmitter234);
                            }
                            if (mEmitter238 != NULL) {
                                releaseEmitter_801EE434(mEmitter238);
                            }
                            if (mEmitter23c != NULL) {
                                releaseEmitter_801EE434(mEmitter23c);
                            }
                            if (mEmitter240 != NULL) {
                                releaseEmitter_801EE434(mEmitter240);
                            }
                            mEmitter230 = NULL;
                            mEmitter234 = NULL;
                            mEmitter238 = NULL;
                            mEmitter23c = NULL;
                            mEmitter240 = NULL;
                            spawnEmitter_8013CC50(field_0x4, &effectPos, 0x85, 0, 0, NULL, NULL, true, 1.0f);
                            spawnEmitter_8013CC50(field_0x4, &effectPos, 0x86, 0, 0, NULL, NULL, true, 1.0f);
                            spawnEmitter_8013CC50(field_0x4, &effectPos, 0x87, 0, 0, NULL, NULL, true, 1.0f);
                            startSound_801F0E34(0x30069, 0);
                        }
                        key->fn_802C5284();
                        fn_8023EE28(player);
                        fn_801F34DC();
                        break;
                    }
                }
            }
        }
        fn_802E80A0();
        break;
    }
    case 1:
        if (field_0x24c == 0) {
            field_0x24c = 2;
            startSound_801F0E34(0x3004F, 0);
            field_0x244 = 2;
        } else {
            field_0x24c--;
        }
        break;
    case 2:
        field_0x24c--;
        if (field_0x24c == 0) {
            u32 param = (field_0x90 & ~0xFF) | lbl_8046F270[field_0x248];
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_CIRC, field_0x4, &mPos, param, -1, -1);
            field_0x244 = 3;
            field_0x24c = 0x3C;
        }
        break;
    case 3:
        field_0x24c--;
        if (field_0x24c == 0) {
            fn_801F0D20(3);
            if (mEmitter230 != NULL) {
                mEmitter230->becomeInvalidEmitter();
                mEmitter230 = NULL;
            }
            if (mEmitter234 != NULL) {
                mEmitter234->becomeInvalidEmitter();
                mEmitter234 = NULL;
            }
            if (mEmitter238 != NULL) {
                mEmitter238->becomeInvalidEmitter();
                mEmitter238 = NULL;
            }
            if (mEmitter23c != NULL) {
                mEmitter23c->becomeInvalidEmitter();
                mEmitter23c = NULL;
            }
            if (mEmitter240 != NULL) {
                mEmitter240->becomeInvalidEmitter();
                mEmitter240 = NULL;
            }
            mAlive = false;
        }
        break;
    }
}

// FIXME(shotgun): 87.2 %, missing/extra: sqrtf's expansion (the original calls the guarded software form) and the emitter alpha stores
void MagicCircleGenerator_804A9FB0::fn_802E80A0() {
    s32 bestSerial = -1;
    s32 bestDist = -1;
    s32 serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_KEY0, serial)) >= 0) {
        SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (key == NULL || key->field_0x280 != 6 || key->field_0x240 == 0) {
            continue;
        }
        JGeometry::TVec3f keyPos = key->mPos;
        f32 dy = keyPos.y - mPos.y;
        f32 dx = keyPos.x - mPos.x;
        s32 dist = (s32)std::sqrtf(dy * dy + dx * dx);
        if (dist > 64) {
            dist = 64;
        }
        if (bestDist < 0 || dist < bestDist) {
            bestDist = dist;
            bestSerial = serial;
        }
    }
    if (bestSerial >= 0) {
        SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(bestSerial));
        u8 alpha = (64 - bestDist) * 4;
        JGeometry::TVec3f pos = key->mPos;
        if (mEmitter238 == NULL) {
            mEmitter238 = spawnEmitter_8013CC50(field_0x4, &pos, 0x314, 0, 2, NULL, NULL, true, 1.0f);
        }
        if (mEmitter23c == NULL) {
            mEmitter23c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x312, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitter240 == NULL) {
            mEmitter240 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x313, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitter238 != NULL) {
            if (key->field_0xc4) {
                pos = *fn_8023E7B0(key->field_0xc8);
                setEmitterPos_8013CB44(mEmitter238, &pos, -1, 3, true);
            } else {
                pos.z += 0.1f;
                setEmitterPos_8013CB44(mEmitter238, &pos, 0, 0, true);
            }
            mEmitter238->setGlobalAlpha(alpha);
        }
        if (mEmitter23c != NULL) {
            mEmitter23c->setGlobalAlpha(alpha);
        }
        if (mEmitter240 != NULL) {
            mEmitter240->setGlobalAlpha(alpha);
        }
    }
}

// FIXME: 93.51 %. The -20.0f / 20.0f literals are @508 / @509, not
// lbl_80541260 / lbl_80541264 (data split), and the search keeps its loop
// value in r29 where the original has li r28, -1 (r28 = previous serial).
bool MagicCircleGenerator_804A9FB0::fn_802E7DB8() {
    mBox.set(-20.0f, -20.0f, 20.0f, 20.0f);
    f32 left = mBox.i.x + mPos.x;
    f32 top = mBox.i.y + mPos.y;
    f32 right = mBox.f.x + mPos.x;
    f32 bottom = mBox.f.y + mPos.y;

    s32 serial = -1;
    for (;;) {
        s32 found = getActorManager_801F856C()->fn_801F78F4(ACTOR_KEY0, serial);
        serial = found;
        if (found < 0) {
            return false;
        }
        SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(found));
        if (key == NULL) {
            continue;
        }
        if (key->field_0x280 != 6) {
            continue;
        }
        JGeometry::TVec3f pos = key->mPos;
        if (pos.z != 0.0f) {
            continue;
        }
        f32 keyLeft = key->mBox.i.x + pos.x;
        f32 keyTop = key->mBox.i.y + pos.y;
        f32 keyRight = key->mBox.f.x + pos.x;
        f32 keyBottom = key->mBox.f.y + pos.y;
        if (keyLeft <= left) {
            keyLeft = left;
        }
        if (keyTop <= top) {
            keyTop = top;
        }
        if (keyRight >= right) {
            keyRight = right;
        }
        if (keyBottom >= bottom) {
            keyBottom = bottom;
        }
        bool overlap = false;
        if (keyRight >= keyLeft && keyBottom >= keyTop) {
            overlap = true;
        }
        if (!overlap) {
            continue;
        }

        field_0x244 = 1;
        field_0x24c = 0x14;
        JGeometry::TVec3f effectPos = pos;
        if (mEmitter230 != NULL) {
            releaseEmitter_801EE434(mEmitter230);
        }
        if (mEmitter234 != NULL) {
            releaseEmitter_801EE434(mEmitter234);
        }
        if (mEmitter238 != NULL) {
            releaseEmitter_801EE434(mEmitter238);
        }
        if (mEmitter23c != NULL) {
            releaseEmitter_801EE434(mEmitter23c);
        }
        if (mEmitter240 != NULL) {
            releaseEmitter_801EE434(mEmitter240);
        }
        mEmitter230 = NULL;
        mEmitter234 = NULL;
        mEmitter238 = NULL;
        mEmitter23c = NULL;
        mEmitter240 = NULL;
        spawnEmitter_8013CC50(field_0x4, &effectPos, 0x85, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &effectPos, 0x86, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &effectPos, 0x87, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30069, 0);
        key->fn_802C5284();
        key->mAlive = false;
        fn_801F34DC();
        return true;
    }
}

void MagicCircleGenerator_804A9FB0::draw() {}
