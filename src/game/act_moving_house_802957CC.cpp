#include "game/GameAudio.h"
#include "game/act/act_moving_house_802957CC.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/ActorId.h"
#include "game/act/act_warp.h"
#include "game/act/act_door.h"
#include "game/Camera.h"
#include "game/BoxTest.h"
#include "game/ViewLayer.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/VecEpsilon.h"

static inline bool testBox(s32 layer, const JGeometry::TVec3f* pos, s32 serial, const u32* ignoreIds) {
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    return boxes->mLists[boxes->field_0x1].containsPointOfOther_8020D6AC(layer, pos, serial, ignoreIds);
}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

// The layer the warp's spawn parameter selects (0 stays 0, else 7 + layer).
static inline s32 warpLayer(u32 param) {
    s32 layer = (param >> 24) & 7;
    if (layer == 0) {
        return layer;
    }
    return layer + 7;
}

static inline s32 getPushDir(MovingHouse_804A7AE0* self) {
    if (fn_80239108(self->mSerial) >= 4 && getPlayerLayer_8023DE58(0) == self->getField_0x4()) {
        s32 dir = fn_80238BC4(self->mSerial);
        if (dir >= 0 && dir < 4) {
            self->field_0x2cc = 1;
            return dir;
        }
    }
    return -1;
}

static inline s32 getPullDir(MovingHouse_804A7AE0* self) {
    JGeometry::TBox2f box(self->mMapBox_0x80);
    box.i.x += self->mPos.x;
    box.i.y += self->mPos.y;
    box.f.x += self->mPos.x;
    box.f.y += self->mPos.y;
    if (fn_80239470(&box, self->mLayer_0x4, self->mSerial) >= 0 && fn_80238A68(self->mSerial) >= 4) {
        s32 dir = fn_8023885C(self->mSerial);
        if (dir >= 0 && dir < 4) {
            if (dir == 0) {
                dir = 1;
            } else if (dir == 1) {
                dir = 0;
            } else if (dir == 2) {
                dir = 3;
            } else if (dir == 3) {
                dir = 2;
            }
            self->field_0x2cc = -1;
            return dir;
        }
    }
    return -1;
}

// Kills the actors registered in both serial lists.
// Inlined by fn_802961AC and update.
inline void MovingHouse_804A7AE0::killSerialActors() {
    for (s32 i = 0; i < mSerialCountB; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsB[i]);
        if (actor != NULL) {
            actor->mAlive = false;
        }
    }
    for (s32 i = 0; i < mSerialCountA; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[i]);
        if (actor != NULL) {
            actor->mAlive = false;
        }
    }
}

// Snapshots the 6x6 tiles under the actor and clears them from the map (once).
// Inlined by fn_802961AC, fn_80299AD0 and its three siblings.
inline void MovingHouse_804A7AE0::bakeTiles() {
    if (!field_0x24c) {
        field_0x24c = 1;
        JGeometry::TVec3f origin = mPos;
        origin.x += mBox.i.x;
        origin.x += 8.0f;
        origin.y += mBox.i.y;
        origin.y += 8.0f;
        origin.z = 0.0f;
        JGeometry::TVec3f p = origin;
        for (int row = 0; row < 6; row++) {
            p.y = origin.y;
            for (int col = 0; col < 6; col++) {
                gBgRewrite_8053AC98->writeTile_802A381C(&p, 0, mTiles[row][col]);
                gBgRewrite_8053AC98->writeTile_802A381C(&p, 1, 0);
                p.y += 16.0f;
            }
            p.x += 16.0f;
        }
    }
}

// Camera focus offsets (right, left, down, up).
static const Vec lbl_8046C238[4] = {
    {128.0f, 0.0f, 0.0f}, {-128.0f, 0.0f, 0.0f}, {0.0f, 128.0f, 0.0f}, {0.0f, -128.0f, 0.0f},
};

// Per-direction offsets (right, left, down, up).
static const Vec lbl_8046C268[4] = {
    {28.0f, 0.0f, 0.0f}, {-28.0f, 0.0f, 0.0f}, {0.0f, 28.0f, 0.0f}, {0.0f, -28.0f, 0.0f},
};
static const Vec lbl_8046C298[4] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f},
};

MovingHouse_804A7AE0::~MovingHouse_804A7AE0() {
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            releaseEmitter_801EE434(mEmitters[i][j]);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2[i]);
    }
}

void MovingHouse_804A7AE0::init() {
    mBox.i.x = -48.0f;
    mBox.i.y = -48.0f;
    mBox.f.x = 48.0f;
    mBox.f.y = 48.0f;
    mMapBox_0x80.i.x = -48.0f;
    mMapBox_0x80.i.y = -48.0f;
    mMapBox_0x80.f.x = 48.0f;
    mMapBox_0x80.f.y = 48.0f;
    field_0xb8 = 1;
    field_0xbc = 3;
    field_0x1a4 = 4;
    mLife = 10000;
    field_0x250.set(0.0f, 0.0f, 0.0f);
    field_0x2ac = 0;
    field_0x24c = 0;
    field_0x248 = 0;
    field_0x54.set(1.0f, 1.0f, 1.0f);
    field_0x230 = 0;
    field_0x244 = mSpawnParam_0x90 & 3;
    field_0x2b9 = (mSpawnParam_0x90 >> 2) & 1;
    field_0x2b5 = 0;
    field_0x2b8 = 0;
    field_0x2b0 = (mSpawnParam_0x90 >> 3) & 3;
    field_0x2b4 = 1;
    field_0x2b6 = 1;
    field_0x2b7 = 1;
    if (field_0x244 == 0) {
        field_0x2b6 = 0;
    } else if (field_0x244 == 1) {
        field_0x2b6 = 0;
        field_0x2b7 = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        mSerialsA[i] = -1;
        mSerialsB[i] = -1;
    }
    for (s32 i = 0; i < 4; i++) {
        mSerialsC[i] = -1;
    }
    for (s32 i = 0; i < 4; i++) {
        mSerialsD[i] = -1;
    }
    mSerialCountA = 0;
    mSerialCountB = 0;
    mSerialCountC = 0;
    mSerialCountD = 0;
    field_0x2cc = 0;
    field_0x2c0 = 0;
    field_0x2ba = 0;
    field_0x2bb = 0;
    for (s32 i = 0; i < 4; i++) {
        mPlayerFlags[i] = 0;
    }
    mTimer = 0;
    field_0x234 = 0;
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            mEmitters[i][j] = NULL;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        mEmitters2[i] = NULL;
    }
    field_0x30c = 0;
    field_0x238.set(mPos);
}

// Snapshots the 6x6 tiles of both layers under the actor (vfunc_0x10).
inline void MovingHouse_804A7AE0::snapshotTiles() {
    JGeometry::TVec3f origin = mPos;
    origin.x += mBox.i.x;
    origin.y += mBox.i.y;
    origin.x += 8.0f;
    origin.y += 8.0f;
    origin.z = 0.0f;
    JGeometry::TVec3f p = origin;
    for (s32 row = 0; row < 6; row++) {
        p.y = origin.y;
        for (s32 col = 0; col < 6; col++) {
            mTilesA[row][col] = getRawTile_80226850(&p, 1);
            mTilesB[row][col] = getRawTile_80226850(&p, 0);
            if (!field_0x2b5) {
                if (field_0x244 == 0 || field_0x244 == 2) {
                    mTiles[row][col] = 0x78;
                } else {
                    mTiles[row][col] = 0x360;
                }
            }
            p.y += 16.0f;
        }
        p.x += 16.0f;
    }
    field_0x2b5 = 1;
}

// FIXME(shotgun): 99.27 %, regalloc-only; behaviour clean.
// initial position/box arithmetic is scheduled differently, 5 instructions
void MovingHouse_804A7AE0::vfunc_0x10() {
    snapshotTiles();
    f32 minX = mBox.i.x + mPos.x;
    f32 minY = mBox.i.y + mPos.y;
    f32 maxX = mBox.f.x + mPos.x;
    f32 maxY = mBox.f.y + mPos.y;
    s32 serialWarp = -1;
    s32 serialFlme = -1;
    s32 serialDoor = -1;
    while (true) {
        serialWarp = getActorManager_801F856C()->findNextActorById_801F78F4(ACTOR_WARP, serialWarp);
        if (serialWarp < 0) {
            break;
        }
        Warp_804A6DC8* warp = (Warp_804A6DC8*)getActorManager_801F856C()->findActorBySerial_801F666C(serialWarp);
        if (warp == NULL) {
            continue;
        }
        if (getGameView_802D6A9C()->mScreenX_0x14 != warp->field_0x244 ||
            getGameView_802D6A9C()->mScreenY_0x18 != warp->field_0x248) {
            continue;
        }
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        if (warp->mLayer_0x4 == (v == 0 ? 0 : v + 7) && mLayer_0x4 == warpLayer(warp->mSpawnParam_0x90)) {
            if (mSerialCountA >= 4) {
                continue;
            }
            mSerialsA[mSerialCountA] = serialWarp;
            mSerialCountA++;
        } else if ((v == 0 ? 0 : v + 7) == warpLayer(warp->mSpawnParam_0x90) && mLayer_0x4 == warp->mLayer_0x4) {
            if (mSerialCountB >= 4) {
                continue;
            }
            mSerialsB[mSerialCountB] = serialWarp;
            mSerialCountB++;
            JGeometry::TVec3f pos = warp->mPos;
            switch (warp->mSpawnParam_0x90 & 0x7F) {
            case 0:
            case 3:
                pos.y -= 2.0f;
                break;
            case 1:
            case 2:
                pos.y += 2.0f;
                break;
            case 0x17:
                pos.x += 2.0f;
                break;
            }
            warp->mPos = pos;
        }
    }
    for (s32 i = 0; i < mSerialCountA; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[i]);
        if (actor != NULL) {
            field_0x2dc[i].x = actor->mPos.x;
            field_0x2dc[i].y = actor->mPos.y;
        }
    }
    while (true) {
        serialFlme = getActorManager_801F856C()->findNextActorById_801F78F4(ACTOR_FLME, serialFlme);
        if (serialFlme < 0) {
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serialFlme);
        if (actor == NULL) {
            continue;
        }
        if (mSerialCountC >= 4) {
            break;
        }
        mSerialsC[mSerialCountC] = serialFlme;
        mSerialCountC++;
    }
    while (true) {
        serialDoor = getActorManager_801F856C()->findNextActorById_801F78F4(ACTOR_DOOR, serialDoor);
        if (serialDoor < 0) {
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serialDoor);
        if (actor == NULL) {
            continue;
        }
        bool inside = false;
        if (actor->mPos.x >= minX && actor->mPos.y >= minY && actor->mPos.x < maxX && actor->mPos.y < maxY) {
            inside = true;
        }
        if (!inside) {
            continue;
        }
        if (mSerialCountD >= 4) {
            break;
        }
        mSerialsD[mSerialCountD] = serialDoor;
        mSerialCountD++;
    }
}

// Remaining: one loop pointer copy in `bakeTiles`.
void MovingHouse_804A7AE0::fn_8029A974() {
    if (!field_0x250.isZero()) {
        return;
    }
    if (field_0x2c0 == 1) {
        field_0x2c0 = 0;
        return;
    }
    if (field_0x2b4 == 0) {
        return;
    }
    s32 dir = -1;
    if (field_0x2b9 == 0) {
        dir = getPushDir(this);
        if (dir == -1) {
            dir = getPullDir(this);
        }
    }
    if (dir != 3) {
        return;
    }
    JGeometry::TBox2f view;
    view = *layerBox(mLayer_0x4);
    growBox(view, -32.0f);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.x += 8.0f;
    p.y += mBox.i.y;
    p.y -= 8.0f;
    p.z = 0.0f;
    u8 ok = 1;
    for (int k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) || testBox(mLayer_0x4, &p, getSerial(), NULL) == 1) {
            ok = 0;
            break;
        }
        if (!contains(view, p)) {
            ok = 0;
            break;
        }
        p.x += 16.0f;
    }
    if (!fn_802957CC(3)) {
        ok = 0;
    }
    if (ok == 1) {
        if (dir == 3) {
            fn_80238E08(mSerial);
            mVel_0x3c.y = -0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.y = -16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// Remaining: one loop pointer copy in `bakeTiles`.
void MovingHouse_804A7AE0::fn_8029A494() {
    if (!field_0x250.isZero()) {
        return;
    }
    if (field_0x2c0 == 1) {
        field_0x2c0 = 0;
        return;
    }
    if (field_0x2b4 == 0) {
        return;
    }
    s32 dir = -1;
    if (field_0x2b9 == 0) {
        dir = getPushDir(this);
        if (dir == -1) {
            dir = getPullDir(this);
        }
    }
    if (dir != 2) {
        return;
    }
    JGeometry::TBox2f view;
    view = *layerBox(mLayer_0x4);
    growBox(view, -32.0f);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.x += 8.0f;
    p.y += mBox.f.y;
    p.y += 8.0f;
    p.z = 0.0f;
    u8 ok = 1;
    for (int k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) || testBox(mLayer_0x4, &p, getSerial(), NULL) == 1) {
            ok = 0;
            break;
        }
        if (!contains(view, p)) {
            ok = 0;
            break;
        }
        p.x += 16.0f;
    }
    if (!fn_802957CC(2)) {
        ok = 0;
    }
    if (ok == 1) {
        if (dir == 2) {
            fn_80238E08(mSerial);
            mVel_0x3c.y = 0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.y = 16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// Remaining: one loop pointer copy in `bakeTiles`.
void MovingHouse_804A7AE0::fn_80299FB0() {
    if (!field_0x250.isZero()) {
        return;
    }
    if (field_0x2c0 == 1) {
        field_0x2c0 = 0;
        return;
    }
    if (field_0x2b4 == 0) {
        return;
    }
    s32 dir = -1;
    if (field_0x2b9 == 0) {
        dir = getPushDir(this);
        if (dir == -1) {
            dir = getPullDir(this);
        }
    }
    if (dir != 1) {
        return;
    }
    JGeometry::TBox2f view;
    view = *layerBox(mLayer_0x4);
    growBox(view, -32.0f);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.x -= 8.0f;
    p.y += mBox.i.y;
    p.y += 8.0f;
    p.z = 0.0f;
    u8 ok = 1;
    for (int k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) || testBox(mLayer_0x4, &p, getSerial(), NULL) == 1) {
            ok = 0;
            break;
        }
        if (!contains(view, p)) {
            ok = 0;
            break;
        }
        p.y += 16.0f;
    }
    if (!fn_802957CC(1)) {
        ok = 0;
    }
    if (ok == 1) {
        if (dir == 1) {
            fn_80238E08(mSerial);
            mVel_0x3c.x = -0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.x = -16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// Remaining: one loop pointer copy in `bakeTiles`.
void MovingHouse_804A7AE0::fn_80299AD0() {
    if (!field_0x250.isZero()) {
        return;
    }
    if (field_0x2c0 == 1) {
        field_0x2c0 = 0;
        return;
    }
    if (field_0x2b4 == 0) {
        return;
    }
    s32 dir = -1;
    if (field_0x2b9 == 0) {
        dir = getPushDir(this);
        if (dir == -1) {
            dir = getPullDir(this);
        }
    }
    if (dir != 0) {
        return;
    }
    JGeometry::TBox2f view;
    view = *layerBox(mLayer_0x4);
    growBox(view, -32.0f);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.f.x;
    p.x += 8.0f;
    p.y += mBox.i.y;
    p.y += 8.0f;
    p.z = 0.0f;
    bool ok = true;
    for (int k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) || testBox(mLayer_0x4, &p, getSerial(), NULL) == 1) {
            ok = false;
            break;
        }
        if (!contains(view, p)) {
            ok = false;
            break;
        }
        p.y += 16.0f;
    }
    if (!fn_802957CC(0)) {
        ok = false;
    }
    if (ok == 1) {
        if (dir == 0) {
            fn_80238E08(mSerial);
            mVel_0x3c.x = 0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.x = 16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// The camera focus point: mPos moved 128 units in direction `idx` of lbl_8046C238 (the opposite
// one while field_0x2cc is -1). No copy of its own: fn_80299720 inlines it (its two vectors lie
// below the caller's on the stack, in reverse order).
inline void MovingHouse_804A7AE0::addCameraFocus(s32 idx) {
    JGeometry::TVec3f focus = mPos;
    if (mSerialCountC > 0 && field_0x2cc == -1) {
        idx ^= 1;
    }
    const Vec& dir = lbl_8046C238[idx];
    JGeometry::TVec3f offset;
    offset.x = dir.x;
    offset.y = dir.y;
    offset.z = dir.z;
    focus.add(offset);
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
}

// Moves every door of mSerialsD by `delta`. No copy of its own: fn_80299720 inlines it (its
// counter and the caller's vector take the registers and stack slots of an inlined body).
inline void MovingHouse_804A7AE0::moveDoors(const JGeometry::TVec3f& delta) {
    for (s32 i = 0; i < mSerialCountD; i++) {
        Door_804A8D88* door = (Door_804A8D88*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsD[i]);
        if (door != NULL) {
            door->mPos = door->mPos + delta;
            door->fn_802C59B0(&delta);
        }
    }
}

void MovingHouse_804A7AE0::fn_80299720() {
    if (field_0x250.y < 0.0f) {
        field_0x248++;
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        getGameView_802D6A9C()->fn_802D79E8(layer, 1.0f, 0.0f, true);
        if (field_0x250.y < -0.32f) {
            mPos.y -= 0.32f;
            field_0x250.y += 0.32f;
        } else {
            mPos.y += field_0x250.y;
            field_0x250.zero();
        }
        JGeometry::TVec3f p = mPos;
        p.x += mBox.i.x;
        p.x += 8.0f;
        p.y += mBox.i.y;
        p.y += 2.0f;
        p.z = 0.0f;
        for (s32 k = 0; k < 6; k++) {
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, mLayer_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.x += 16.0f;
        }
        addCameraFocus(3);
        if (field_0x250.isZero()) {
            moveDoors(JGeometry::TVec3f(0.0f, -16.0f, 0.0f));
            field_0x248 = 0;
            field_0x2b8 = 0;
            mVel_0x3c.zero();
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (mSpawnParam_0x90 >> 27) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (mSpawnParam_0x90 >> 22) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (mSpawnParam_0x90 >> 27) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (mSpawnParam_0x90 >> 22) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        }
    }
}

void MovingHouse_804A7AE0::fn_80299374() {
    if (field_0x250.y > 0.0f) {
        field_0x248++;
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        getGameView_802D6A9C()->fn_802D79E8(layer, 1.0f, 0.0f, true);
        if (field_0x250.y > 0.32f) {
            mPos.y += 0.32f;
            field_0x250.y -= 0.32f;
        } else {
            mPos.y += field_0x250.y;
            field_0x250.zero();
        }
        JGeometry::TVec3f p = mPos;
        p.x += mBox.i.x;
        p.x += 8.0f;
        p.y += mBox.f.y;
        p.y -= 2.0f;
        p.z = 0.0f;
        for (s32 k = 0; k < 6; k++) {
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, mLayer_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.x += 16.0f;
        }
        addCameraFocus(2);
        if (field_0x250.isZero()) {
            moveDoors(JGeometry::TVec3f(0.0f, 16.0f, 0.0f));
            field_0x248 = 0;
            field_0x2b8 = 0;
            mVel_0x3c.zero();
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (mSpawnParam_0x90 >> 22) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (mSpawnParam_0x90 >> 27) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (mSpawnParam_0x90 >> 22) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (mSpawnParam_0x90 >> 27) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        }
    }
}

void MovingHouse_804A7AE0::fn_80298FC4() {
    if (field_0x250.x < 0.0f) {
        field_0x248++;
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        getGameView_802D6A9C()->fn_802D79E8(layer, 1.0f, 0.0f, true);
        if (field_0x250.x < -0.32f) {
            mPos.x -= 0.32f;
            field_0x250.x += 0.32f;
        } else {
            mPos.x += field_0x250.x;
            field_0x250.zero();
        }
        JGeometry::TVec3f p = mPos;
        p.x += mBox.i.x;
        p.x += 2.0f;
        p.y += mBox.i.y;
        p.y += 8.0f;
        p.z = 0.0f;
        for (s32 k = 0; k < 6; k++) {
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, mLayer_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.y += 16.0f;
        }
        addCameraFocus(1);
        if (field_0x250.isZero()) {
            moveDoors(JGeometry::TVec3f(-16.0f, 0.0f, 0.0f));
            field_0x248 = 0;
            field_0x2b8 = 0;
            mVel_0x3c.zero();
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (mSpawnParam_0x90 >> 17) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (mSpawnParam_0x90 >> 12) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (mSpawnParam_0x90 >> 17) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (mSpawnParam_0x90 >> 12) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        }
    }
}

void MovingHouse_804A7AE0::fn_80298C18() {
    if (field_0x250.x > 0.0f) {
        field_0x248++;
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        getGameView_802D6A9C()->fn_802D79E8(layer, 1.0f, 0.0f, true);
        if (field_0x250.x > 0.32f) {
            mPos.x += 0.32f;
            field_0x250.x -= 0.32f;
        } else {
            mPos.x += field_0x250.x;
            field_0x250.zero();
        }
        JGeometry::TVec3f p = mPos;
        p.x += mBox.f.x;
        p.x -= 2.0f;
        p.y += mBox.i.y;
        p.y += 8.0f;
        p.z = 0.0f;
        for (s32 k = 0; k < 6; k++) {
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, mLayer_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.y += 16.0f;
        }
        addCameraFocus(0);
        if (field_0x250.isZero()) {
            JGeometry::TVec3f push(16.0f, 0.0f, 0.0f);
            moveDoors(push);
            field_0x248 = 0;
            field_0x2b8 = 0;
            mVel_0x3c.zero();
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (mSpawnParam_0x90 >> 12) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (mSpawnParam_0x90 >> 17) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->setSwitchBit_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (mSpawnParam_0x90 >> 12) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (mSpawnParam_0x90 >> 17) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->setSwitchBit_802D8050(n, true, 0xFF);
            }
        }
    }
}

// FIXME(shotgun): 95.45 %, scheduling+regalloc; behaviour clean.
// still differs: order of the pos.x/pos.y stores against the mSpawnParam_0x90 load
void MovingHouse_804A7AE0::fn_80298978() {
    for (s32 i = 0; i < mSerialCountA; i++) {
        Warp_804A6DC8* warp = (Warp_804A6DC8*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[i]);
        s32 blocked = 0;
        if (warp == NULL) {
            continue;
        }
        if (field_0x250.isZero() == false) {
            blocked = 1;
        } else {
            for (s32 k = 0; k < 4; k++) {
                JGeometry::TVec3f pos = warp->field_0x238;
                pos.x = 8.0f * ((k & 1) > 0) - 4.0f + pos.x;
                pos.y = 8.0f * ((k & 2) > 0) - 4.0f + pos.y;
                s32 kind = warp->mSpawnParam_0x90 & 0x7F;
                if (kind == 0 || kind == 3) {
                    pos.y += -16.0f;
                } else if (kind == 1 || kind == 2) {
                    pos.y += 16.0f;
                } else if (kind == 0x17) {
                    pos.x += 16.0f;
                } else if (kind == 0x18) {
                    pos.x += -16.0f;
                }
                if (isBlocked_0x4C(warpLayer(warp->mSpawnParam_0x90), &pos) == 1) {
                    blocked = 1;
                    break;
                }
            }
        }
        if (blocked == 1) {
            JGeometry::TBox2f box(-10.0f, -10.0f, 10.0f, 10.0f);
            box.addPos(field_0x2dc[i]);
            s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
            fn_80239914(&box, v == 0 ? 0 : v + 7, mSerial);
        }
    }
}

// Registers a collision box (given relative to mPos) for this actor.
inline void registerBox_802984B0(MovingHouse_804A7AE0* actor, JGeometry::TBox2f& box, f32 x0, f32 y0, f32 x1, f32 y1) {
    box.i.x = x0;
    box.i.y = y0;
    box.f.x = x1;
    box.f.y = y1;
    box.i.x += actor->mPos.x;
    box.i.y += actor->mPos.y;
    box.f.x += actor->mPos.x;
    box.f.y += actor->mPos.y;
    fn_80239914(&box, actor->mLayer_0x4, actor->mSerial);
}

void MovingHouse_804A7AE0::fn_802984B0() {
    JGeometry::TBox2f box1;
    JGeometry::TBox2f box2;
    switch (field_0x2b0) {
    case 0:
        box1.i.x = mBox.i.x;
        box1.i.y = mBox.i.y;
        box1.f.x = mBox.f.x;
        box1.f.y = mBox.f.y;
        box1.i.x += mPos.x;
        box1.i.y += mPos.y;
        box1.f.x += mPos.x;
        box1.f.y += mPos.y;
        box1.f.y -= 40.0f;
        fn_80239914(&box1, mLayer_0x4, mSerial);
        break;
    case 1:
        registerBox_802984B0(this, box1, mBox.i.x, mBox.i.y, mBox.i.x + 42.0f, mBox.i.y + 42.0f);
        registerBox_802984B0(this, box2, mBox.f.x - 42.0f, mBox.i.y, mBox.f.x, mBox.i.y + 42.0f);
        break;
    case 2:
        registerBox_802984B0(this, box1, mBox.i.x, mBox.i.y, mBox.i.x + 42.0f, mBox.f.y - 40.0f);
        registerBox_802984B0(this, box2, mBox.f.x - 42.0f, mBox.i.y, mBox.f.x, mBox.f.y - 40.0f);
        break;
    }
    JGeometry::TBox2f box3;
    JGeometry::TBox2f box4;
    switch (field_0x244) {
    case 0:
    case 1:
        registerBox_802984B0(this, box3, mBox.i.x, mBox.f.y - 42.0f, mBox.i.x + 42.0f, mBox.f.y);
        registerBox_802984B0(this, box4, mBox.f.x - 42.0f, mBox.f.y - 42.0f, mBox.f.x, mBox.f.y);
        break;
    case 2:
        registerBox_802984B0(this, box3, mBox.i.x, mBox.f.y - 42.0f, mBox.i.x + 58.0f, mBox.f.y);
        registerBox_802984B0(this, box4, mBox.f.x - 26.0f, mBox.f.y - 42.0f, mBox.f.x, mBox.f.y);
        break;
    }
    if (mSerialCountD > 0) {
        if (!field_0x2b6) {
            JGeometry::TBox2f box;
            registerBox_802984B0(this, box, mBox.i.x + 40.0f, mBox.f.y - 16.0f, mBox.f.x - 40.0f, mBox.f.y);
        }
        if (!field_0x2b7) {
            JGeometry::TBox2f box;
            registerBox_802984B0(this, box, mBox.i.x + 40.0f, mBox.i.y, mBox.f.x - 40.0f, mBox.i.y + 16.0f);
        }
    }
}

// Puts the snapshotted tiles of both layers back under the actor, remembering what was there
// (update inlines it twice, each time with the whole test).
inline void MovingHouse_804A7AE0::restoreTiles() {
    if (field_0x24c != 0 && field_0x2ac <= 0) {
        JGeometry::TVec3f origin = mPos;
        origin.x += mBox.i.x;
        origin.y += mBox.i.y;
        origin.x += 8.0f;
        origin.y += 8.0f;
        origin.z = 0.0f;
        JGeometry::TVec3f p = origin;
        for (s32 row = 0; row < 6; row++) {
            p.y = origin.y;
            for (s32 col = 0; col < 6; col++) {
                mTiles[row][col] = getRawTile_80226850(&p, 0);
                gBgRewrite_8053AC98->writeTile_802A381C(&p, 0, mTilesB[row][col]);
                gBgRewrite_8053AC98->writeTile_802A381C(&p, 1, mTilesA[row][col]);
                p.y += 16.0f;
            }
            p.x += 16.0f;
        }
        field_0x24c = 0;
    }
}

// True when no active player stands inside the house's box on its layer, nor on the
// house's warp layer. No copy of its own: update inlines it.
inline bool MovingHouse_804A7AE0::isClearOfPlayers() {
    f32 minX = mBox.i.x + mPos.x;
    f32 maxX = mBox.f.x + mPos.x;
    f32 minY = mBox.i.y + mPos.y;
    f32 maxY = mBox.f.y + mPos.y;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022D75C(i) == 1) {
            if (mLayer_0x4 == getPlayerLayer_8023DE58(i)) {
                const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(i);
                bool inside = false;
                if (pPos->x >= minX && pPos->y >= minY && pPos->x < maxX && pPos->y < maxY) {
                    inside = true;
                }
                if (inside == 1) {
                    return false;
                }
            }
            s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
            s32 layer = v == 0 ? 0 : v + 7;
            if (getPlayerLayer_8023DE58(i) == layer) {
                return false;
            }
        }
    }
    return true;
}

// FIXME: 96.47 %, missing/extra + reloc-name; behaviour near.
void MovingHouse_804A7AE0::update() {
    mPos.set(field_0x238);
    for (s32 i = 0; i < 4; i++) {
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        if (getPlayerLayer_8023DE58(i) == layer) {
            JGeometry::TBox2f box;
            box.i.x = mBox.i.x;
            box.i.y = mBox.i.y;
            box.f.x = mBox.f.x;
            box.f.y = mBox.f.y;
            box.i.x += mPos.x;
            box.i.y += mPos.y;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            gGameRoot2_8053AB10->field_0x24->fn_8030C058(&box);
            break;
        }
    }
    if (field_0x2bb == 1) {
        bool done = true;
        for (s32 i = 0; i < 4; i++) {
            if (mPlayerFlags[i] == 1 && isPlayerActive_8023F264(i) == 1 && !hasPlayerLeader_8022F514(i)) {
                if (!fn_8022D75C(i) &&
                    hitPlayerGroup_80238548(i, 1, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 0) == 1) {
                    mPlayerFlags[i] = 0;
                } else {
                    done = false;
                }
            }
        }
        if (done == 1) {
            mAlive = false;
        }
        return;
    }
    switch (field_0x230) {
    case 0: {
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x;
        box.i.y = mBox.i.y;
        box.f.x = mBox.f.x;
        box.f.y = mBox.f.y;
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        fn_802984B0();
        if (field_0x250.isZero()) {
            for (s32 i = 0; i < 4; i++) {
                if (isPlayerActive_8023F264(i) == 1 && fn_80236E3C(i) == 1 && fn_80239C00(i, &box, mLayer_0x4) &&
                    fn_8022F9E4(i, mSerial, true, false)) {
                    field_0x234 = 50;
                    killSerialActors();
                    field_0x230 = 1;
                    break;
                }
            }
        }
        if (field_0x230 == 0) {
            if (field_0x234 == 0) {
                u32 hitActorId = ACTOR_NULL;
                s32 hitSerial = getActorManager_801F856C()->findOverlappingActor_801F82CC(this, 0, 32.0f);
                if (hitSerial >= 0) {
                    ActorBase_804A5770* hitActor =
                        getActorManager_801F856C()->findActorBySerial_801F666C(hitSerial);
                    if (hitActor != NULL) {
                        hitActorId = hitActor->mActorId;
                    }
                }
                if ((field_0x244 != 1 || (field_0x2b6 == 1 && field_0x2b7 == 1)) &&
                    (fn_8023A30C() || hitActorId == 'BBMB') &&
                    field_0x250.isZero()) {
                    field_0x234 = 50;
                    bakeTiles();
                    field_0x230 = 3;
                    startSound_801F0E34(0x3000F, 0);
                    mVel_0x3c.zero();
                    mVel_0x3c.z = 4.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    for (s32 i = 0; i < 4; i++) {
                        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
                        s32 layer = v == 0 ? 0 : v + 7;
                        if (getPlayerLayer_8023DE58(i) == layer) {
                            fn_8023B484(i);
                            getGameView_802D6A9C()->fn_802D79A4(i, mVel_0x3c.z, 0.3f, 0.5f);
                        }
                    }
                    break;
                }
                if (field_0x250.isZero() && field_0x24c == 0 &&
                    field_0x2ac == 0) {
                    if (field_0x2b6 == 0) {
                        JGeometry::TVec3f p1 = mPos;
                        p1.x -= 8.0f;
                        p1.y += 40.0f;
                        JGeometry::TVec3f p2 = p1;
                        p2.x += 16.0f;
                        if (getRawTile_80226850(&p1, 0) == 0x19E || getRawTile_80226850(&p2, 0) == 0x19F ||
                            getRawTile_80226850(&p1, 0) == 0x338 || getRawTile_80226850(&p2, 0) == 0x339) {
                            snapshotTiles();
                            field_0x2b6 = 1;
                        }
                    }
                    if (field_0x2b7 == 0) {
                        JGeometry::TVec3f p1 = mPos;
                        p1.x -= 8.0f;
                        p1.y -= 40.0f;
                        JGeometry::TVec3f p2 = p1;
                        p2.x += 16.0f;
                        if (getRawTile_80226850(&p1, 0) == 0x32A || getRawTile_80226850(&p2, 0) == 0x32B) {
                            snapshotTiles();
                            field_0x2b7 = 1;
                        }
                    }
                }
                if (field_0x2ba == 0) {
                    fn_802961AC();
                }
                if (field_0x2ba == 1) {
                    fn_80295EE8();
                }
                if (field_0x2ba == 0) {
                    fn_80298978();
                    for (s32 i = 0; i < mSerialCountB; i++) {
                        ActorBase_804A5770* actor =
                            getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsB[i]);
                        if (actor != NULL &&
                            !field_0x250.isZero()) {
                            JGeometry::TBox2f b;
                            b.i.x = -10.0f;
                            b.i.y = -10.0f;
                            b.f.x = 10.0f;
                            b.f.y = 10.0f;
                            b.i.x += actor->mPos.x;
                            b.i.y += actor->mPos.y;
                            b.f.x += actor->mPos.x;
                            b.f.y += actor->mPos.y;
                            fn_80239914(&b, 0, mSerial);
                        }
                    }
                    field_0x2b4 = isClearOfPlayers();
                    if (field_0x244 != 1 || (field_0x2b6 == 1 && field_0x2b7 == 1)) {
                        fn_8029A974();
                        fn_8029A494();
                        fn_80299FB0();
                        fn_80299AD0();
                        fn_80299720();
                        fn_80299374();
                        fn_80298FC4();
                        fn_80298C18();
                    }
                    if (!field_0x250.isZero()) {
                        if (field_0x244 == 1) {
                            if (mEmitters2[0] == NULL) {
                                u16 effId;
                                if ((field_0x250.x > 0.0f && field_0x2cc == 1) ||
                                    (field_0x250.x < 0.0f && field_0x2cc == -1)) {
                                    effId = 0x316;
                                } else if ((field_0x250.x > 0.0f && field_0x2cc == -1) ||
                                           (field_0x250.x < 0.0f && field_0x2cc == 1)) {
                                    effId = 0x317;
                                } else if ((field_0x250.y > 0.0f && field_0x2cc == 1) ||
                                           (field_0x250.y < 0.0f && field_0x2cc == -1)) {
                                    effId = 0x318;
                                } else if ((field_0x250.y > 0.0f && field_0x2cc == -1) ||
                                           (field_0x250.y < 0.0f && field_0x2cc == 1)) {
                                    effId = 0x315;
                                }
                                mEmitters2[0] = spawnEmitter_8013CC50(mLayer_0x4, &mPos, effId, 0, 0, NULL, NULL, true, 1.0f);
                            } else {
                                setEmitterPos_8013CB44(mEmitters2[0], &mPos, 0, 0, true);
                            }
                        } else {
                            u16 idsA[4] = {0x321, 0x322, 0x323, 0x324};
                            u16 idsB[4] = {0x31D, 0x31E, 0x31F, 0x320};
                            u8 flag = getGameView_802D6A9C()->field_0x131f2;
                            for (s32 i = 0; i < 4; i++) {
                                if (mEmitters2[i] == NULL) {
                                    u16 effId = idsA[i];
                                    if (flag == 1) {
                                        effId = idsB[i];
                                    }
                                    mEmitters2[i] = spawnEmitter_8013CC50(mLayer_0x4, &mPos, effId, 0, 0, NULL, NULL, true, 1.0f);
                                } else {
                                    setEmitterPos_8013CB44(mEmitters2[i], &mPos, 0, 0, true);
                                }
                            }
                        }
                    } else {
                        for (s32 i = 0; i < 4; i++) {
                            if (mEmitters2[i] != NULL) {
                                mEmitters2[i]->becomeInvalidEmitter();
                                mEmitters2[i] = NULL;
                            }
                        }
                    }
                    if (field_0x250.isZero()) {
                        if (field_0x2ac > 0) {
                            field_0x2ac -= 4;
                            if (field_0x2ac <= 0) {
                                field_0x2ac = 0;
                                restoreTiles();
                            }
                        }
                    } else if (field_0x2ac < 0x80) {
                        field_0x2ac += 8;
                        if (field_0x2ac >= 0x80) {
                            field_0x2ac = 0x80;
                        }
                    }
                    if (field_0x2ac > 0) {
                        fn_801F2B20(6);
                    }
                }
            } else {
                if (field_0x2ba == 0) {
                    fn_80298978();
                    for (s32 i = 0; i < mSerialCountB; i++) {
                        ActorBase_804A5770* actor =
                            getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsB[i]);
                        if (actor != NULL &&
                            !field_0x250.isZero()) {
                            JGeometry::TBox2f b;
                            b.i.x = -10.0f;
                            b.i.y = -10.0f;
                            b.f.x = 10.0f;
                            b.f.y = 10.0f;
                            b.i.x += actor->mPos.x;
                            b.i.y += actor->mPos.y;
                            b.f.x += actor->mPos.x;
                            b.f.y += actor->mPos.y;
                            fn_80239914(&b, 0, mSerial);
                        }
                    }
                }
                field_0x234--;
            }
        }
        break;
    }
    case 3:
        fn_802984B0();
        mPos.add(mVel_0x3c);
        if (mPos.z > 0.0f) {
            mVel_0x3c.z -= 0.3f;
        } else if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            restoreTiles();
            s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
            s32 layer = v == 0 ? 0 : v + 7;
            getGameView_802D6A9C()->fn_802D79E8(layer, 8.0f, 0.0f, true);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x263, 0, 0, NULL, NULL, true, 1.0f);
            field_0x230 = 0;
            field_0x234 = 60;
        }
        break;
    case 1:
        field_0x234--;
        if (field_0x234 == 0) {
            if (field_0x30c == NULL) {
                JGeometry::TVec3f p = *fn_8023E7B0(field_0xc8);
                field_0x30c = spawnEmitter_8013CC50(mLayer_0x4, &p, 0x27E, 0, 0, NULL, NULL, true, 1.0f);
            }
            bakeTiles();
            field_0x234 = 60;
            field_0x230 = 2;
        }
        break;
    case 2:
        if (field_0xc4 != 0) {
            for (s32 i = 0; i < mSerialCountA; i++) {
                JGeometry::TBox2f b;
                b.i.x = -4.0f;
                b.i.y = -4.0f;
                b.f.x = 4.0f;
                b.f.y = 4.0f;
                b.i.x += field_0x2dc[i].x;
                b.i.y += field_0x2dc[i].y;
                b.f.x += field_0x2dc[i].x;
                b.f.y += field_0x2dc[i].y;
                s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
                fn_80239914(&b, v == 0 ? 0 : v + 7, mSerial);
            }
            mPos.set(*fn_8023E7B0(field_0xc8));
            if (!epsilonEquals(mPos, field_0x238, JGeometry::TUtilf::epsilon())) {
                for (s32 i = 0; i < 4; i++) {
                    if (!hasPlayerLeader_8022F514(i)) {
                        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
                        s32 layer = v == 0 ? 0 : v + 7;
                        if (getPlayerLayer_8023DE58(i) == layer) {
                            getGameView_802D6A9C()->fn_802D79A4(i, 0.8f, 0.08f, 1.0f);
                        }
                    }
                }
            }
            if (field_0x234 > 0) {
                field_0x234--;
                if (field_0x234 == 0) {
                    if (field_0x30c != NULL) {
                        field_0x30c->becomeInvalidEmitter();
                    }
                } else {
                    field_0x30c->setGlobalTranslation(*fn_8023E7B0(field_0xc8));
                }
            }
        } else {
            if (field_0x30c != NULL) {
                field_0x30c->becomeInvalidEmitter();
            }
            mVel_0x3c.z += -0.15f;
            mPos.add(mVel_0x3c);
            if (mPos.z < 0.0f) {
                for (s32 i = 0; i < 4; i++) {
                    s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
                    s32 layer = v == 0 ? 0 : v + 7;
                    if (getPlayerLayer_8023DE58(i) == layer && !hasPlayerLeader_8022F514(i)) {
                        hitPlayerGroup_80238548(i, 1, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 0);
                    }
                }
                die_0x24();
            } else if (damagePlayersInBox_801F2B7C(0, field_0xc8, 6) || fn_801F2B20(0)) {
                for (s32 i = 0; i < 4; i++) {
                    s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
                    s32 layer = v == 0 ? 0 : v + 7;
                    if (getPlayerLayer_8023DE58(i) == layer && !hasPlayerLeader_8022F514(i)) {
                        hitPlayerGroup_80238548(i, 1, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 0);
                    }
                }
                die_0x24();
            }
        }
        break;
    }
    JGeometry::TVec3f delta = mPos - mPrevPos;
    if (!delta.isZero()) {
        for (s32 i = 0; i < mSerialCountA; i++) {
            Warp_804A6DC8* warp =
                (Warp_804A6DC8*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[i]);
            if (warp != NULL) {
                warp->field_0x238.add(delta);
            }
        }
        for (s32 i = 0; i < mSerialCountB; i++) {
            ActorBase_804A5770* actor =
                getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsB[i]);
            if (actor != NULL) {
                actor->mPos.add(delta);
            }
        }
        for (s32 i = 0; i < mSerialCountC; i++) {
            ActorBase_804A5770* actor =
                getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsC[i]);
            if (actor != NULL) {
                actor->mPos = actor->mPos + delta;
            }
        }
    }
    field_0x238.set(mPos);
}

// FIXME(shotgun): 95.0 %, missing/extra: callee-saved register order in the prologue, 3 instructions
inline void drawSmp_802965BC(s32 layer, const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, const char* name) {
    if (layer < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', name);
        gGameRoot2_8053AB10->field_0x20->drawBlockMap_802F6CF4(pos, res, params);
    }
}

void MovingHouse_804A7AE0::draw() {
    if (field_0x24c == 0 && field_0x2ac == 0) {
        return;
    }
    if (field_0x2bb == 1) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx m;
    PSMTXTrans(m, mBox.i.x, mBox.i.y, 0.0f);
    params.mMtx = m;
    params.field_0x1d = -2;
    if (field_0xc4) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    JGeometry::TVec3f pos = mPos;
    if (field_0x244 == 0) {
        drawSmp_802965BC(mLayer_0x4, &pos, &params, field_0x2b6 == 0 ? "house2_smp.bin" : "house_smp.bin");
        params.field_0x1c++;
        drawSmp_802965BC(mLayer_0x4, &pos, &params, "house1_smp.bin");
        params.field_0x1c--;
    } else if (field_0x244 == 1) {
        const char* a;
        const char* b;
        if (field_0x2b7 == 0) {
            a = "room2_u_smp.bin";
            b = "room3_u_smp.bin";
        } else {
            a = "room_u_smp.bin";
            b = "room1_u_smp.bin";
        }
        drawSmp_802965BC(mLayer_0x4, &pos, &params, a);
        params.field_0x1c++;
        drawSmp_802965BC(mLayer_0x4, &pos, &params, b);
        params.field_0x1c--;
        pos.y += 48.0f;
        if (field_0x2b6 == 0) {
            a = "room2_smp.bin";
            b = "room3_smp.bin";
        } else {
            a = "room_smp.bin";
            b = "room1_smp.bin";
        }
        drawSmp_802965BC(mLayer_0x4, &pos, &params, a);
        params.field_0x1c++;
        drawSmp_802965BC(mLayer_0x4, &pos, &params, b);
        params.field_0x1c--;
    } else {
        const char* a = "house3_smp.bin";
        drawSmp_802965BC(mLayer_0x4, &pos, &params, a);
        params.field_0x1c++;
        drawSmp_802965BC(mLayer_0x4, &pos, &params, "house31_smp.bin");
        params.field_0x1c--;
    }
}

void MovingHouse_804A7AE0::fn_802961AC() {
    s32 serial = findOverlappingActor_801F2718('SLR3');
    if (serial < 0) {
        return;
    }
    ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (other == NULL) {
        return;
    }
    if (other->field_0x94 != 5) {
        return;
    }
    field_0x2d0 = mPos - other->mPos;
    field_0x2d0.normalize();
    mVel_0x3c = field_0x2d0;
    startSound_801F0E34(0x30075, 0);
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            if (mEmitters[i][j] == NULL) {
                JGeometry::TVec3f pos = mPos;
                pos.x += 50.0f * (f32)i - 25.0f;
                pos.y += 50.0f * (f32)j - 25.0f;
                mEmitters[i][j] = spawnEmitter_8013CC50(mLayer_0x4, &pos, 0x3d3, 0, -1, NULL, NULL, true, 3.0f);
            }
        }
    }
    s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
    s32 layer = v == 0 ? 0 : v + 7;
    getGameView_802D6A9C()->fn_802D79E8(layer, 16.0f, 0.0f, true);
    killSerialActors();
    field_0x24c = 0;
    bakeTiles();
    mTimer = 120;
    mTimerMax = 120;
    field_0x2ba = 1;
}

void MovingHouse_804A7AE0::fn_80295EE8() {
    if (mTimer <= 0) {
        die_0x24();
        return;
    }
    if (mTimerMax - mTimer < 60) {
        mVel_0x3c = field_0x2d0;
        mVel_0x3c.scale(2.0f * ((f32)(mTimer + 60 - mTimerMax) / 60.0f));
        mPos.add(mVel_0x3c);
        for (s32 i = 0; i < 2; i++) {
            for (s32 j = 0; j < 2; j++) {
                if (mEmitters[i][j] != NULL) {
                    JGeometry::TVec3f pos = mPos;
                    pos.x += (50.0f * (f32)i - 25.0f) * field_0x54.x;
                    pos.y += (50.0f * (f32)j - 25.0f) * field_0x54.x;
                    setEmitterPos_8013CB44(mEmitters[i][j], &pos, 0, -1, true);
                }
            }
        }
    } else {
        for (s32 i = 0; i < 2; i++) {
            for (s32 j = 0; j < 2; j++) {
                if (mEmitters[i][j] != NULL) {
                    mEmitters[i][j]->becomeInvalidEmitter();
                    mEmitters[i][j] = NULL;
                }
            }
        }
        mVel_0x3c.zero();
    }
    for (s32 i = 0; i < 4; i++) {
        s32 v = (mSpawnParam_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        s32 playerLayer = getPlayerLayer_8023DE58(i);
        if (playerLayer == layer && !fn_80235274(i)) {
            hitPlayerGroup_80238548(i, 0, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 12);
        }
    }
    mTimer--;
}

void MovingHouse_804A7AE0::die_0x24() {
    s32 v;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive_8023F264(i) && !hasPlayerLeader_8022F514(i)) {
            v = (mSpawnParam_0x90 >> 7) & 0x1F;
            s32 layer = v == 0 ? 0 : v + 7;
            if (layer == getPlayerLayer_8023DE58(i)) {
                if (startPlayerTransition_802317F0(i, getField_0x4(), getPlayerPos_8023E724(i), &mPos, 0x1F)) {
                    mPlayerFlags[i] = 1;
                } else {
                    mPlayerFlags[i] = startPlayerTransition_802317F0(i, getField_0x4(), getPlayerPos_8023E724(i), &mPos, 0x21);
                }
            }
        }
    }
    const char* stage = getGameView_802D6A9C()->getStageBgName_802D9C4C();
    spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x82, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x83, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x84, 1, 0, NULL, NULL, true, 1.0f);
    if (stage[0] == 0x72 || stage[0] == 0x73) {
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x80, 1, 0, NULL, NULL, true, 1.0f);
    } else {
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x81, 1, 0, NULL, NULL, true, 1.0f);
    }
    startSound_801F0E34(0x30092, 0);
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            if (mEmitters[i][j] != NULL) {
                mEmitters[i][j]->becomeInvalidEmitter();
                mEmitters[i][j] = NULL;
            }
        }
    }
    for (s32 i = 0; i < mSerialCountC; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsC[i]);
        if (actor != NULL) {
            actor->mAlive = false;
        }
    }
    field_0x2bb = 1;
}

bool MovingHouse_804A7AE0::fn_802957CC(u8 dir) {
    JGeometry::TVec3f oldPos = mPos;
    bool ok = true;
    mVel_0x3c.set(JGeometry::TVec3f(lbl_8046C268[dir].x, lbl_8046C268[dir].y, lbl_8046C268[dir].z));
    mPos.add(mVel_0x3c);
    mBox.set(-44.0f, -44.0f, 44.0f, 44.0f);
    // Computed into sp+0x3C..0x48 in the original asm (0x80295864-0x802958DC), unused below, as in fn_80279824.
    JGeometry::TBox2f view_box;
    view_box = *layerBox(mLayer_0x4);
    growBox(view_box, -32.0f);
    JGeometry::TVec3f p = mPos;
    p.z = 0.0f;
    if (dir == 0 || dir == 1) {
        if (dir == 0) {
            p.x += mBox.f.x;
            p.x += 8.0f;
        } else {
            p.x += mBox.i.x;
            p.x -= 8.0f;
        }
        p.y += mBox.i.y;
        p.y += 8.0f;
        for (s32 i = 0; i < 6; i++) {
            if (!gBgRewrite_8053AC98->fn_802A1490(&p) || testBox(mLayer_0x4, &p, getSerial(), NULL) == 1) {
                ok = false;
                break;
            }
            p.y += 16.0f;
        }
    } else {
        if (dir == 2) {
            p.y += mBox.f.y;
            p.y += 8.0f;
        } else {
            p.y += mBox.i.y;
            p.y -= 8.0f;
        }
        p.x += mBox.i.x;
        p.x += 8.0f;
        for (s32 i = 0; i < 6; i++) {
            if (!gBgRewrite_8053AC98->fn_802A1490(&p) || testBox(mLayer_0x4, &p, getSerial(), NULL) == 1) {
                ok = false;
                break;
            }
            p.x += 16.0f;
        }
    }
    if (!ok) {
        ok = true;
        JGeometry::TBox2f box = mBox;
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, mLayer_0x4, false, 0.0f)) {
                JGeometry::TVec3f q = *getPlayerPos_8023E724(i);
                q.add(JGeometry::TVec3f(lbl_8046C298[dir].x, lbl_8046C298[dir].y, lbl_8046C298[dir].z));
                if (gBgRewrite_8053AC98->fn_802A1490(&q) != 1 || testBox(mLayer_0x4, &q, getSerial(), NULL)) {
                    ok = false;
                }
            }
        }
    }
    mPos = oldPos;
    mVel_0x3c.set(0.0f, 0.0f, 0.0f);
    mBox.set(-48.0f, -48.0f, 48.0f, 48.0f);
    return ok;
}
