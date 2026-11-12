#include "game/GameAudio.h"
#include "game/act/tu_802957CC.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/ActorId.h"
#include "game/act/act_warp.h"
#include "game/act/act_door.h"
#include "game/Camera.h"
#include "game/ViewLayer.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Player.h"

// Snapshots the 6x6 tiles under the actor and clears them from the map (once).
// Inlined by fn_802961AC, fn_80299AD0 and its three siblings.
inline void cls_804A7AE0::bakeTiles() {
    if (!field_0x24c) {
        field_0x24c = 1;
        JGeometry::TVec3f p = mPos;
        p.x += mBox.i.x;
        p.y += mBox.i.y;
        p.x += 8.0f;
        p.y += 8.0f;
        p.z = 0.0f;
        f32 baseY = p.y;
        for (s32 row = 0; row < 6; row++) {
            p.y = baseY;
            for (s32 col = 0; col < 6; col++) {
                gBgRewrite_8053AC98->fn_802A381C(&p, 0, mTiles[row][col]);
                gBgRewrite_8053AC98->fn_802A381C(&p, 1, 0);
                p.y += 16.0f;
            }
            p.x += 16.0f;
        }
    }
}

// Camera focus offsets (right, left, down, up).
static const f32 lbl_8046C238[4][3] = {
    {128.0f, 0.0f, 0.0f}, {-128.0f, 0.0f, 0.0f}, {0.0f, 128.0f, 0.0f}, {0.0f, -128.0f, 0.0f},
};

// Per-direction offsets (right, left, down, up).
static const f32 lbl_8046C268[4][3] = {
    {28.0f, 0.0f, 0.0f}, {-28.0f, 0.0f, 0.0f}, {0.0f, 28.0f, 0.0f}, {0.0f, -28.0f, 0.0f},
};
static const f32 lbl_8046C298[4][3] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f},
};

cls_804A7AE0::~cls_804A7AE0() {
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            releaseEmitter_801EE434(mEmitters[i][j]);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2[i]);
    }
}

// FIXME(shotgun): 95.65 %, scheduling+regalloc + reloc-name; behaviour clean.
void cls_804A7AE0::init() {
    mBox.i.x = -48.0f;
    mBox.i.y = -48.0f;
    mBox.f.x = 48.0f;
    mBox.f.y = 48.0f;
    field_0x80.i.x = -48.0f;
    field_0x80.i.y = -48.0f;
    field_0x80.f.x = 48.0f;
    field_0x80.f.y = 48.0f;
    field_0xb8 = 1;
    field_0xbc = 3;
    field_0x1a4 = 4;
    mLife = 10000;
    field_0x250.zero();
    field_0x2ac = 0;
    field_0x24c = 0;
    field_0x248 = 0;
    field_0x54.set(1.0f, 1.0f, 1.0f);
    field_0x230 = 0;
    field_0x244 = field_0x90 & 3;
    field_0x2b9 = (field_0x90 >> 2) & 1;
    field_0x2b5 = 0;
    field_0x2b8 = 0;
    field_0x2b0 = (field_0x90 >> 3) & 3;
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
    field_0x238.set(mPos.x, mPos.y, mPos.z);
}

// FIXME(shotgun): 82.39 %, missing/extra + reloc-name; behaviour near.
// initial position/box arithmetic is scheduled differently, 5 instructions
void cls_804A7AE0::vfunc_0x10() {
    JGeometry::TVec3f origin = mPos;
    origin.x += mBox.i.x;
    origin.y += mBox.i.y;
    origin.x += 8.0f;
    origin.y += 8.0f;
    JGeometry::TVec3f p = origin;
    p.z = 0.0f;
    f32 baseY = p.y;
    for (s32 row = 0; row < 6; row++) {
        p.y = baseY;
        for (s32 col = 0; col < 6; col++) {
            mTilesA[row][col] = fn_80226850(&p, 1);
            mTilesB[row][col] = fn_80226850(&p, 0);
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
    f32 minX = mBox.i.x + mPos.x;
    f32 minY = mBox.i.y + mPos.y;
    f32 maxX = mBox.f.x + mPos.x;
    f32 maxY = mBox.f.y + mPos.y;
    s32 serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_WARP, serial)) >= 0) {
        Warp_804A6DC8* warp = (Warp_804A6DC8*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (warp == NULL) {
            continue;
        }
        if (getGameView_802D6A9C()->field_0x14 != warp->field_0x244 ||
            getGameView_802D6A9C()->field_0x18 != warp->field_0x248) {
            continue;
        }
        s32 v = (field_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        if (warp->field_0x4 == layer) {
            s32 w = (warp->field_0x90 >> 24) & 7;
            if (w != 0) {
                w += 7;
            }
            if (field_0x4 == w) {
                if (mSerialCountA >= 4) {
                    continue;
                }
                mSerialsA[mSerialCountA] = serial;
                mSerialCountA++;
            }
        } else {
            s32 w = (warp->field_0x90 >> 24) & 7;
            if (w != 0) {
                w += 7;
            }
            if (layer == w && field_0x4 == warp->field_0x4) {
                if (mSerialCountB >= 4) {
                    continue;
                }
                mSerialsB[mSerialCountB] = serial;
                mSerialCountB++;
                JGeometry::TVec3f pos = warp->mPos;
                switch (warp->field_0x90 & 0x7F) {
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
    }
    for (s32 i = 0; i < mSerialCountA; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[i]);
        if (actor != NULL) {
            field_0x2dc[i].x = actor->mPos.x;
            field_0x2dc[i].y = actor->mPos.y;
        }
    }
    serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_FLME, serial)) >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            continue;
        }
        if (mSerialCountC >= 4) {
            break;
        }
        mSerialsC[mSerialCountC] = serial;
        mSerialCountC++;
    }
    serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_DOOR, serial)) >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
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
        mSerialsD[mSerialCountD] = serial;
        mSerialCountD++;
    }
}

// FIXME(shotgun): 77.94 %, missing/extra + reloc-name; behaviour drift.
// same shape as fn_80299AD0 (direction/serial selection and the doubled 32 * epsilon constant)
void cls_804A7AE0::fn_8029A974() {
    bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
    if (!still) {
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
        s32 r;
        if (fn_80239108(mSerial) >= 4 && getPlayerLayer_8023DE58(0) == field_0x4 &&
            (r = fn_80238BC4(mSerial), r >= 0 && r < 4)) {
            field_0x2cc = 1;
        } else {
            r = -1;
        }
        dir = r;
        if (dir == -1) {
            JGeometry::TBox2f box;
            box.i.x = field_0x80.i.x;
            box.i.y = field_0x80.i.y;
            box.f.x = field_0x80.f.x;
            box.f.y = field_0x80.f.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            if (fn_80239470(&box, field_0x4, mSerial) >= 0 && fn_80238A68(mSerial) >= 4) {
                s32 q = fn_8023885C(mSerial);
                if (q >= 0 && q < 4) {
                    if (q == 0) {
                        r = 1;
                    } else if (q == 1) {
                        r = 0;
                    } else if (q == 2) {
                        r = 3;
                    } else if (q == 3) {
                        r = 2;
                    }
                    field_0x2cc = -1;
                } else {
                    r = -1;
                }
            } else {
                r = -1;
            }
            dir = r;
        }
    }
    if (dir != 3) {
        return;
    }
    s32 layer = field_0x4;
    JGeometry::TBox2f view = *layerBox(layer);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.x += 8.0f;
    p.y += mBox.i.y;
    p.y -= 8.0f;
    p.z = 0.0f;
    f32 minX = view.i.x + 32.0f;
    f32 minY = view.i.y + 32.0f;
    f32 maxX = view.f.x - 32.0f;
    f32 maxY = view.f.y - 32.0f;
    bool ok = true;
    for (s32 k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) ||
            fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &p, mSerial, NULL)) {
            ok = false;
            break;
        }
        bool inside = false;
        if (p.x >= minX && p.y >= minY && p.x < maxX && p.y < maxY) {
            inside = true;
        }
        if (!inside) {
            ok = false;
            break;
        }
        p.x += 16.0f;
    }
    if (!fn_802957CC(3)) {
        ok = false;
    }
    if (ok) {
        if (dir == 3) {
            fn_80238E08(mSerial);
            field_0x3c.y = -0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.y = -16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// FIXME(shotgun): 78.63 %, missing/extra + reloc-name; behaviour drift.
// same shape as fn_80299AD0 (direction/serial selection and the doubled 32 * epsilon constant)
void cls_804A7AE0::fn_8029A494() {
    bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
    if (!still) {
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
        s32 r;
        if (fn_80239108(mSerial) >= 4 && getPlayerLayer_8023DE58(0) == field_0x4 &&
            (r = fn_80238BC4(mSerial), r >= 0 && r < 4)) {
            field_0x2cc = 1;
        } else {
            r = -1;
        }
        dir = r;
        if (dir == -1) {
            JGeometry::TBox2f box;
            box.i.x = field_0x80.i.x;
            box.i.y = field_0x80.i.y;
            box.f.x = field_0x80.f.x;
            box.f.y = field_0x80.f.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            if (fn_80239470(&box, field_0x4, mSerial) >= 0 && fn_80238A68(mSerial) >= 4) {
                s32 q = fn_8023885C(mSerial);
                if (q >= 0 && q < 4) {
                    if (q == 0) {
                        r = 1;
                    } else if (q == 1) {
                        r = 0;
                    } else if (q == 2) {
                        r = 3;
                    } else if (q == 3) {
                        r = 2;
                    }
                    field_0x2cc = -1;
                } else {
                    r = -1;
                }
            } else {
                r = -1;
            }
            dir = r;
        }
    }
    if (dir != 2) {
        return;
    }
    s32 layer = field_0x4;
    JGeometry::TBox2f view = *layerBox(layer);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.x += 8.0f;
    p.y += mBox.f.y;
    p.y += 8.0f;
    p.z = 0.0f;
    f32 minX = view.i.x + 32.0f;
    f32 minY = view.i.y + 32.0f;
    f32 maxX = view.f.x - 32.0f;
    f32 maxY = view.f.y - 32.0f;
    bool ok = true;
    for (s32 k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) ||
            fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &p, mSerial, NULL)) {
            ok = false;
            break;
        }
        bool inside = false;
        if (p.x >= minX && p.y >= minY && p.x < maxX && p.y < maxY) {
            inside = true;
        }
        if (!inside) {
            ok = false;
            break;
        }
        p.x += 16.0f;
    }
    if (!fn_802957CC(2)) {
        ok = false;
    }
    if (ok) {
        if (dir == 2) {
            fn_80238E08(mSerial);
            field_0x3c.y = 0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.y = 16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// FIXME(shotgun): 77.97 %, missing/extra + reloc-name; behaviour drift.
// same shape as fn_80299AD0 (direction/serial selection and the doubled 32 * epsilon constant)
void cls_804A7AE0::fn_80299FB0() {
    bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
    if (!still) {
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
        s32 r;
        if (fn_80239108(mSerial) >= 4 && getPlayerLayer_8023DE58(0) == field_0x4 &&
            (r = fn_80238BC4(mSerial), r >= 0 && r < 4)) {
            field_0x2cc = 1;
        } else {
            r = -1;
        }
        dir = r;
        if (dir == -1) {
            JGeometry::TBox2f box;
            box.i.x = field_0x80.i.x;
            box.i.y = field_0x80.i.y;
            box.f.x = field_0x80.f.x;
            box.f.y = field_0x80.f.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            if (fn_80239470(&box, field_0x4, mSerial) >= 0 && fn_80238A68(mSerial) >= 4) {
                s32 q = fn_8023885C(mSerial);
                if (q >= 0 && q < 4) {
                    if (q == 0) {
                        r = 1;
                    } else if (q == 1) {
                        r = 0;
                    } else if (q == 2) {
                        r = 3;
                    } else if (q == 3) {
                        r = 2;
                    }
                    field_0x2cc = -1;
                } else {
                    r = -1;
                }
            } else {
                r = -1;
            }
            dir = r;
        }
    }
    if (dir != 1) {
        return;
    }
    s32 layer = field_0x4;
    JGeometry::TBox2f view = *layerBox(layer);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.x -= 8.0f;
    p.y += mBox.i.y;
    p.y += 8.0f;
    p.z = 0.0f;
    f32 minX = view.i.x + 32.0f;
    f32 minY = view.i.y + 32.0f;
    f32 maxX = view.f.x - 32.0f;
    f32 maxY = view.f.y - 32.0f;
    bool ok = true;
    for (s32 k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) ||
            fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &p, mSerial, NULL)) {
            ok = false;
            break;
        }
        bool inside = false;
        if (p.x >= minX && p.y >= minY && p.x < maxX && p.y < maxY) {
            inside = true;
        }
        if (!inside) {
            ok = false;
            break;
        }
        p.y += 16.0f;
    }
    if (!fn_802957CC(1)) {
        ok = false;
    }
    if (ok) {
        if (dir == 1) {
            fn_80238E08(mSerial);
            field_0x3c.x = -0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.x = -16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// FIXME(shotgun): 78.63 %, missing/extra + reloc-name; behaviour drift.
// 15 instructions (the 32 * epsilon constant is folded twice, direction/serial selection scheduled differently)
void cls_804A7AE0::fn_80299AD0() {
    bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
    if (!still) {
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
        s32 r;
        if (fn_80239108(mSerial) >= 4 && getPlayerLayer_8023DE58(0) == field_0x4 &&
            (r = fn_80238BC4(mSerial), r >= 0 && r < 4)) {
            field_0x2cc = 1;
        } else {
            r = -1;
        }
        dir = r;
        if (dir == -1) {
            JGeometry::TBox2f box;
            box.i.x = field_0x80.i.x;
            box.i.y = field_0x80.i.y;
            box.f.x = field_0x80.f.x;
            box.f.y = field_0x80.f.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            if (fn_80239470(&box, field_0x4, mSerial) >= 0 && fn_80238A68(mSerial) >= 4) {
                s32 q = fn_8023885C(mSerial);
                if (q >= 0 && q < 4) {
                    if (q == 0) {
                        r = 1;
                    } else if (q == 1) {
                        r = 0;
                    } else if (q == 2) {
                        r = 3;
                    } else if (q == 3) {
                        r = 2;
                    }
                    field_0x2cc = -1;
                } else {
                    r = -1;
                }
            } else {
                r = -1;
            }
            dir = r;
        }
    }
    if (dir != 0) {
        return;
    }
    s32 layer = field_0x4;
    JGeometry::TBox2f view = *layerBox(layer);
    JGeometry::TVec3f p = mPos;
    p.x += mBox.f.x;
    p.x += 8.0f;
    p.y += mBox.i.y;
    p.y += 8.0f;
    p.z = 0.0f;
    f32 minX = view.i.x + 32.0f;
    f32 minY = view.i.y + 32.0f;
    f32 maxX = view.f.x - 32.0f;
    f32 maxY = view.f.y - 32.0f;
    bool ok = true;
    for (s32 k = 0; k < 6; k++) {
        if (!gBgRewrite_8053AC98->fn_802A1554(&p) ||
            fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &p, mSerial, NULL)) {
            ok = false;
            break;
        }
        bool inside = false;
        if (p.x >= minX && p.y >= minY && p.x < maxX && p.y < maxY) {
            inside = true;
        }
        if (!inside) {
            ok = false;
            break;
        }
        p.y += 16.0f;
    }
    if (!fn_802957CC(0)) {
        ok = false;
    }
    if (ok) {
        if (dir == 0) {
            fn_80238E08(mSerial);
            field_0x3c.x = 0.32f;
            field_0x2b8 = 1;
        }
        startSound_801F0E34(0x30004, 0);
        field_0x250.x = 16.0f;
        field_0x248 = 0;
        bakeTiles();
    }
}

// FIXME(shotgun): 96.88 %, missing/extra + reloc-name; behaviour drift.
// layer expression compiles branch-free (cntlzw) in the original, 4 instructions
void cls_804A7AE0::fn_80299720() {
    if (field_0x250.y < 0.0f) {
        field_0x248++;
        s32 v = (field_0x90 >> 7) & 0x1F;
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
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, field_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.x += 16.0f;
        }
        JGeometry::TVec3f focus = mPos;
        s32 idx = 3;
        if (mSerialCountC > 0 && field_0x2cc == -1) {
            idx ^= 1;
        }
        JGeometry::TVec3f offset(lbl_8046C238[idx][0], lbl_8046C238[idx][1], lbl_8046C238[idx][2]);
        focus.add(offset);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        if (field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon()) {
            JGeometry::TVec3f push(0.0f, -16.0f, 0.0f);
            for (s32 i = 0; i < mSerialCountD; i++) {
                Door_804A8D88* door = (Door_804A8D88*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsD[i]);
                if (door != NULL) {
                    JGeometry::TVec3f q = door->mPos;
                    q.add(push);
                    door->mPos = q;
                    door->fn_802C59B0(&push);
                }
            }
            field_0x248 = 0;
            field_0x2b8 = 0;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (field_0x90 >> 27) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (field_0x90 >> 22) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (field_0x90 >> 27) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (field_0x90 >> 22) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        }
    }
}

// FIXME(shotgun): 91.68 %, missing/extra + reloc-name; behaviour drift.
// layer expression compiles branch-free (cntlzw) in the original, 4 instructions
void cls_804A7AE0::fn_80299374() {
    if (field_0x250.y > 0.0f) {
        field_0x248++;
        s32 v = (field_0x90 >> 7) & 0x1F;
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
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, field_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.x += 16.0f;
        }
        JGeometry::TVec3f focus = mPos;
        s32 idx = 2;
        if (mSerialCountC > 0 && field_0x2cc == -1) {
            idx ^= 1;
        }
        JGeometry::TVec3f offset(lbl_8046C238[idx][0], lbl_8046C238[idx][1], lbl_8046C238[idx][2]);
        focus.add(offset);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        if (field_0x250.squared() <= JGeometry::TUtil<f32>::epsilon()) {
            JGeometry::TVec3f push(0.0f, 16.0f, 0.0f);
            for (s32 i = 0; i < mSerialCountD; i++) {
                Door_804A8D88* door = (Door_804A8D88*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsD[i]);
                if (door != NULL) {
                    door->mPos = door->mPos + push;
                    door->fn_802C59B0(&push);
                }
            }
            field_0x248 = 0;
            field_0x2b8 = 0;
            field_0x3c.zero();
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (field_0x90 >> 22) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (field_0x90 >> 27) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (field_0x90 >> 22) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (field_0x90 >> 27) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        }
    }
}

// FIXME(shotgun): 96.89 %, missing/extra + reloc-name; behaviour drift.
// layer expression compiles branch-free (cntlzw) in the original, 4 instructions
void cls_804A7AE0::fn_80298FC4() {
    if (field_0x250.x < 0.0f) {
        field_0x248++;
        s32 v = (field_0x90 >> 7) & 0x1F;
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
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, field_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.y += 16.0f;
        }
        JGeometry::TVec3f focus = mPos;
        s32 idx = 1;
        if (mSerialCountC > 0 && field_0x2cc == -1) {
            idx ^= 1;
        }
        JGeometry::TVec3f offset(lbl_8046C238[idx][0], lbl_8046C238[idx][1], lbl_8046C238[idx][2]);
        focus.add(offset);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
        if (still) {
            JGeometry::TVec3f push(-16.0f, 0.0f, 0.0f);
            for (s32 i = 0; i < mSerialCountD; i++) {
                Door_804A8D88* door = (Door_804A8D88*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsD[i]);
                if (door != NULL) {
                    JGeometry::TVec3f q = door->mPos;
                    q.add(push);
                    door->mPos = q;
                    door->fn_802C59B0(&push);
                }
            }
            field_0x248 = 0;
            field_0x2b8 = 0;
            field_0x3c.zero();
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (field_0x90 >> 17) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (field_0x90 >> 12) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (field_0x90 >> 17) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (field_0x90 >> 12) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        }
    }
}

// FIXME(shotgun): 96.89 %, missing/extra + reloc-name; behaviour drift.
// layer expression compiles branch-free (cntlzw) in the original, 4 instructions
void cls_804A7AE0::fn_80298C18() {
    if (field_0x250.x > 0.0f) {
        field_0x248++;
        s32 v = (field_0x90 >> 7) & 0x1F;
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
            gBgRewrite_8053AC98->fn_802A20F0(&p, -1, field_0x4, 0, 1);
            gBgRewrite_8053AC98->fn_802A1050(&p);
            p.y += 16.0f;
        }
        JGeometry::TVec3f focus = mPos;
        s32 idx = 0;
        if (mSerialCountC > 0 && field_0x2cc == -1) {
            idx ^= 1;
        }
        JGeometry::TVec3f offset(lbl_8046C238[idx][0], lbl_8046C238[idx][1], lbl_8046C238[idx][2]);
        focus.add(offset);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
        if (still) {
            JGeometry::TVec3f push(16.0f, 0.0f, 0.0f);
            for (s32 i = 0; i < mSerialCountD; i++) {
                Door_804A8D88* door = (Door_804A8D88*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsD[i]);
                if (door != NULL) {
                    JGeometry::TVec3f q = door->mPos;
                    q.add(push);
                    door->mPos = q;
                    door->fn_802C59B0(&push);
                }
            }
            field_0x248 = 0;
            field_0x2b8 = 0;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x250.zero();
            if (field_0x2cc == 1) {
                s32 n = (field_0x90 >> 12) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            } else {
                s32 n = (field_0x90 >> 17) & 0x1F;
                if (n > 0) {
                    getGameView_802D6A9C()->fn_802D8050(n, false, 0xFF);
                }
            }
            field_0x2cc = 0;
            field_0x2c0 = 1;
        } else if (field_0x2cc == 1) {
            s32 n = (field_0x90 >> 12) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        } else {
            s32 n = (field_0x90 >> 17) & 0x1F;
            if (n > 0) {
                getGameView_802D6A9C()->fn_802D8050(n, true, 0xFF);
            }
        }
    }
}

// FIXME(shotgun): 76.50 %, missing/extra + reloc-name; behaviour drift.
// still differs: the still-vector bool (cror/mfcr), the (k & bit) conversions and the warp-layer branch
void cls_804A7AE0::fn_80298978() {
    for (s32 i = 0; i < mSerialCountA; i++) {
        Warp_804A6DC8* warp = (Warp_804A6DC8*)getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[i]);
        s32 blocked = 0;
        if (warp == NULL) {
            continue;
        }
        bool still = field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon();
        if (!still) {
            blocked = 1;
        } else {
            for (s32 k = 0; k < 4; k++) {
                JGeometry::TVec3f pos = warp->field_0x238;
                pos.x += 8.0f * (f32)((k & 1) != 0) - 4.0f;
                pos.y += 8.0f * (f32)((k & 2) != 0) - 4.0f;
                s32 kind = warp->field_0x90 & 0x7F;
                if (kind == 0 || kind == 3) {
                    pos.y += -16.0f;
                } else if (kind == 1 || kind == 2) {
                    pos.y += 16.0f;
                } else if (kind == 0x17) {
                    pos.x += 16.0f;
                } else if (kind == 0x18) {
                    pos.x += -16.0f;
                }
                s32 w = (warp->field_0x90 >> 24) & 7;
                s32 warpLayer = w;
                if (w != 0) {
                    warpLayer = w + 7;
                }
                if (vfunc_0x4C(warpLayer, &pos) == 1) {
                    blocked = 1;
                    break;
                }
            }
        }
        if (blocked == 1) {
            JGeometry::TBox2f box;
            box.i.x = -10.0f;
            box.i.y = -10.0f;
            box.f.x = 10.0f;
            box.f.y = 10.0f;
            box.i.x += field_0x2dc[i].x;
            box.f.x += field_0x2dc[i].x;
            box.i.y += field_0x2dc[i].y;
            box.f.y += field_0x2dc[i].y;
            s32 v = (field_0x90 >> 7) & 0x1F;
            fn_80239914(&box, v == 0 ? 0 : v + 7, mSerial);
        }
    }
}

// FIXME(shotgun): 98.5 %, different: 3 opcodes (scheduling of the first box)
// Registers a collision box (given relative to mPos) for this actor.
inline void registerBox_802984B0(cls_804A7AE0* actor, f32 x0, f32 y0, f32 x1, f32 y1) {
    JGeometry::TBox2f box;
    box.i.x = x0;
    box.i.y = y0;
    box.f.x = x1;
    box.f.y = y1;
    box.i.x += actor->mPos.x;
    box.i.y += actor->mPos.y;
    box.f.x += actor->mPos.x;
    box.f.y += actor->mPos.y;
    fn_80239914(&box, actor->field_0x4, actor->mSerial);
}

// FIXME(shotgun): 98.47 %, different + reloc-name; behaviour clean.
void cls_804A7AE0::fn_802984B0() {
    switch (field_0x2b0) {
    case 0: {
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x;
        box.i.y = mBox.i.y;
        box.f.x = mBox.f.x;
        box.f.y = mBox.f.y;
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        box.i.y += mPos.y;
        box.f.y -= 40.0f;
        fn_80239914(&box, field_0x4, mSerial);
        break;
    }
    case 1:
        registerBox_802984B0(this, mBox.i.x, mBox.i.y, mBox.i.x + 42.0f, mBox.i.y + 42.0f);
        registerBox_802984B0(this, mBox.f.x - 42.0f, mBox.i.y, mBox.f.x, mBox.i.y + 42.0f);
        break;
    case 2:
        registerBox_802984B0(this, mBox.i.x, mBox.i.y, mBox.i.x + 42.0f, mBox.f.y - 40.0f);
        registerBox_802984B0(this, mBox.f.x - 42.0f, mBox.i.y, mBox.f.x, mBox.f.y - 40.0f);
        break;
    }
    switch (field_0x244) {
    case 0:
    case 1:
        registerBox_802984B0(this, mBox.i.x, mBox.f.y - 42.0f, mBox.i.x + 42.0f, mBox.f.y);
        registerBox_802984B0(this, mBox.f.x - 42.0f, mBox.f.y - 42.0f, mBox.f.x, mBox.f.y);
        break;
    case 2:
        registerBox_802984B0(this, mBox.i.x, mBox.f.y - 42.0f, mBox.i.x + 58.0f, mBox.f.y);
        registerBox_802984B0(this, mBox.f.x - 26.0f, mBox.f.y - 42.0f, mBox.f.x, mBox.f.y);
        break;
    }
    if (mSerialCountD > 0) {
        if (!field_0x2b6) {
            registerBox_802984B0(this, mBox.i.x + 40.0f, mBox.f.y - 16.0f, mBox.f.x - 40.0f, mBox.f.y);
        }
        if (!field_0x2b7) {
            registerBox_802984B0(this, mBox.i.x + 40.0f, mBox.i.y, mBox.f.x - 40.0f, mBox.i.y + 16.0f);
        }
    }
}

// FIXME(shotgun): 72.51 %, missing/extra + reloc-name; behaviour drift.
void cls_804A7AE0::update() {
    mPos.set(field_0x238.x, field_0x238.y, field_0x238.z);
    for (s32 i = 0; i < 4; i++) {
        s32 v = (field_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        if (getPlayerLayer_8023DE58(i) == layer) {
            JGeometry::TBox2f box;
            box.i.x = mBox.i.x;
            box.i.y = mBox.i.y;
            box.f.x = mBox.f.x;
            box.f.y = mBox.f.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            gGameRoot2_8053AB10->field_0x24->fn_8030C058(&box);
            break;
        }
    }
    if (field_0x2bb == 1) {
        bool done = true;
        for (s32 i = 0; i < 4; i++) {
            if (mPlayerFlags[i] == 1 && fn_8023F264(i) == 1 && !fn_8022F514(i)) {
                if (!fn_8022D75C(i) &&
                    fn_80238548(i, 1, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 0) == 1) {
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
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;
        fn_802984B0();
        if (field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon()) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) == 1 && fn_80236E3C(i) == 1 && fn_80239C00(i, &box, field_0x4) &&
                    fn_8022F9E4(i, mSerial, true, false)) {
                    field_0x234 = 50;
                    for (s32 j = 0; j < mSerialCountB; j++) {
                        ActorBase_804A5770* actor =
                            getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsB[j]);
                        if (actor != NULL) {
                            actor->mAlive = false;
                        }
                    }
                    for (s32 j = 0; j < mSerialCountA; j++) {
                        ActorBase_804A5770* actor =
                            getActorManager_801F856C()->findActorBySerial_801F666C(mSerialsA[j]);
                        if (actor != NULL) {
                            actor->mAlive = false;
                        }
                    }
                    field_0x230 = 1;
                    break;
                }
            }
        }
        if (field_0x230 == 0) {
            if (field_0x234 == 0) {
                u32 hitActorId = ACTOR_NULL;
                s32 hitSerial = getActorManager_801F856C()->fn_801F82CC(this, 0, 32.0f);
                if (hitSerial >= 0) {
                    ActorBase_804A5770* hitActor =
                        getActorManager_801F856C()->findActorBySerial_801F666C(hitSerial);
                    if (hitActor != NULL) {
                        hitActorId = hitActor->mActorId;
                    }
                }
                if ((field_0x244 != 1 || (field_0x2b6 == 1 && field_0x2b7 == 1)) &&
                    (fn_8023A30C() || hitActorId == 'BBMB') &&
                    field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon()) {
                    field_0x234 = 50;
                    bakeTiles();
                    field_0x230 = 3;
                    startSound_801F0E34(0x3000F, 0);
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0x3c.z = gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 4.0f;
                    for (s32 i = 0; i < 4; i++) {
                        s32 v = (field_0x90 >> 7) & 0x1F;
                        s32 layer = v == 0 ? 0 : v + 7;
                        if (getPlayerLayer_8023DE58(i) == layer) {
                            fn_8023B484(i);
                            getGameView_802D6A9C()->fn_802D79A4(i, field_0x3c.z, 0.3f, 0.5f);
                        }
                    }
                    break;
                }
                if (field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon() && field_0x24c == 0 &&
                    field_0x2ac == 0) {
                    if (field_0x2b6 == 0) {
                        JGeometry::TVec3f p1 = mPos;
                        p1.x -= 8.0f;
                        p1.y += 40.0f;
                        JGeometry::TVec3f p2 = p1;
                        p2.x += 16.0f;
                        if (fn_80226850(&p1, 0) == 0x19E || fn_80226850(&p2, 0) == 0x19F ||
                            fn_80226850(&p1, 0) == 0x338 || fn_80226850(&p2, 0) == 0x339) {
                            JGeometry::TVec3f p = mPos;
                            p.x += mBox.i.x;
                            p.y += mBox.i.y;
                            p.x += 8.0f;
                            p.y += 8.0f;
                            p.z = 0.0f;
                            f32 baseY = p.y;
                            for (s32 row = 0; row < 6; row++) {
                                p.y = baseY;
                                for (s32 col = 0; col < 6; col++) {
                                    mTilesA[row][col] = fn_80226850(&p, 1);
                                    mTilesB[row][col] = fn_80226850(&p, 0);
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
                            field_0x2b6 = 1;
                        }
                    }
                    if (field_0x2b7 == 0) {
                        JGeometry::TVec3f p1 = mPos;
                        p1.x -= 8.0f;
                        p1.y -= 40.0f;
                        JGeometry::TVec3f p2 = p1;
                        p2.x += 16.0f;
                        if (fn_80226850(&p1, 0) == 0x32A || fn_80226850(&p2, 0) == 0x32B) {
                            JGeometry::TVec3f p = mPos;
                            p.x += mBox.i.x;
                            p.y += mBox.i.y;
                            p.x += 8.0f;
                            p.y += 8.0f;
                            p.z = 0.0f;
                            f32 baseY = p.y;
                            for (s32 row = 0; row < 6; row++) {
                                p.y = baseY;
                                for (s32 col = 0; col < 6; col++) {
                                    mTilesA[row][col] = fn_80226850(&p, 1);
                                    mTilesB[row][col] = fn_80226850(&p, 0);
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
                            field_0x250.squared() > 32.0f * JGeometry::TUtil<f32>::epsilon()) {
                            JGeometry::TBox2f b;
                            b.i.x = -10.0f;
                            b.i.y = -10.0f;
                            b.f.x = 10.0f;
                            b.f.y = 10.0f;
                            b.i.x += actor->mPos.x;
                            b.f.x += actor->mPos.x;
                            b.i.y += actor->mPos.y;
                            b.f.y += actor->mPos.y;
                            fn_80239914(&b, 0, mSerial);
                        }
                    }
                    f32 minX = mBox.i.x + mPos.x;
                    f32 maxX = mBox.f.x + mPos.x;
                    f32 minY = mBox.i.y + mPos.y;
                    f32 maxY = mBox.f.y + mPos.y;
                    u8 clear = 1;
                    for (s32 i = 0; i < 4; i++) {
                        if (fn_8022D75C(i) == 1) {
                            if (field_0x4 == getPlayerLayer_8023DE58(i)) {
                                const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(i);
                                bool inside = false;
                                if (pPos->x >= minX && pPos->y >= minY && pPos->x < maxX && pPos->y < maxY) {
                                    inside = true;
                                }
                                if (inside == 1) {
                                    clear = 0;
                                    break;
                                }
                            }
                            s32 v = (field_0x90 >> 7) & 0x1F;
                            s32 layer = v == 0 ? 0 : v + 7;
                            if (getPlayerLayer_8023DE58(i) == layer) {
                                clear = 0;
                                break;
                            }
                        }
                    }
                    field_0x2b4 = clear;
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
                    if (field_0x250.squared() > 32.0f * JGeometry::TUtil<f32>::epsilon()) {
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
                                mEmitters2[0] = spawnEmitter_8013CC50(field_0x4, &mPos, effId, 0, 0, NULL, NULL, true, 1.0f);
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
                                    mEmitters2[i] = spawnEmitter_8013CC50(field_0x4, &mPos, effId, 0, 0, NULL, NULL, true, 1.0f);
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
                    if (field_0x250.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon()) {
                        if (field_0x2ac > 0) {
                            field_0x2ac -= 4;
                            if (field_0x2ac <= 0) {
                                field_0x2ac = 0;
                                if (field_0x24c != 0 && field_0x2ac <= 0) {
                                    JGeometry::TVec3f p = mPos;
                                    p.x += mBox.i.x;
                                    p.y += mBox.i.y;
                                    p.x += 8.0f;
                                    p.y += 8.0f;
                                    p.z = 0.0f;
                                    f32 baseY = p.y;
                                    for (s32 row = 0; row < 6; row++) {
                                        p.y = baseY;
                                        for (s32 col = 0; col < 6; col++) {
                                            mTiles[row][col] = fn_80226850(&p, 0);
                                            gBgRewrite_8053AC98->fn_802A381C(&p, 0, mTilesB[row][col]);
                                            gBgRewrite_8053AC98->fn_802A381C(&p, 1, mTilesA[row][col]);
                                            p.y += 16.0f;
                                        }
                                        p.x += 16.0f;
                                    }
                                    field_0x24c = 0;
                                }
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
                            field_0x250.squared() > 32.0f * JGeometry::TUtil<f32>::epsilon()) {
                            JGeometry::TBox2f b;
                            b.i.x = -10.0f;
                            b.i.y = -10.0f;
                            b.f.x = 10.0f;
                            b.f.y = 10.0f;
                            b.i.x += actor->mPos.x;
                            b.f.x += actor->mPos.x;
                            b.i.y += actor->mPos.y;
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
    case 1:
        field_0x234--;
        if (field_0x234 == 0) {
            if (field_0x30c == NULL) {
                JGeometry::TVec3f p = *fn_8023E7B0(field_0xc8);
                field_0x30c = spawnEmitter_8013CC50(field_0x4, &p, 0x27E, 0, 0, NULL, NULL, true, 1.0f);
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
                b.f.x += field_0x2dc[i].x;
                b.i.y += field_0x2dc[i].y;
                b.f.y += field_0x2dc[i].y;
                s32 v = (field_0x90 >> 7) & 0x1F;
                fn_80239914(&b, v != 0 ? v + 7 : 0, mSerial);
            }
            const JGeometry::TVec3f* pPos = fn_8023E7B0(field_0xc8);
            mPos.set(pPos->x, pPos->y, pPos->z);
            if (!mPos.equals(field_0x238)) {
                for (s32 i = 0; i < 4; i++) {
                    if (!fn_8022F514(i)) {
                        s32 v = (field_0x90 >> 7) & 0x1F;
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
            field_0x3c.z += -0.15f;
            mPos.add(field_0x3c);
            if (mPos.z < 0.0f) {
                for (s32 i = 0; i < 4; i++) {
                    s32 v = (field_0x90 >> 7) & 0x1F;
                    s32 layer = v == 0 ? 0 : v + 7;
                    if (getPlayerLayer_8023DE58(i) == layer && !fn_8022F514(i)) {
                        fn_80238548(i, 1, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 0);
                    }
                }
                vfunc_0x24();
            } else if (fn_801F2B7C(0, field_0xc8, 6) || fn_801F2B20(0)) {
                for (s32 i = 0; i < 4; i++) {
                    s32 v = (field_0x90 >> 7) & 0x1F;
                    s32 layer = v == 0 ? 0 : v + 7;
                    if (getPlayerLayer_8023DE58(i) == layer && !fn_8022F514(i)) {
                        fn_80238548(i, 1, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 0);
                    }
                }
                vfunc_0x24();
            }
        }
        break;
    case 3:
        fn_802984B0();
        mPos.add(field_0x3c);
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 0.3f;
        } else if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            if (field_0x24c != 0 && field_0x2ac <= 0) {
                JGeometry::TVec3f p = mPos;
                p.x += mBox.i.x;
                p.y += mBox.i.y;
                p.x += 8.0f;
                p.y += 8.0f;
                p.z = 0.0f;
                f32 baseY = p.y;
                for (s32 row = 0; row < 6; row++) {
                    p.y = baseY;
                    for (s32 col = 0; col < 6; col++) {
                        mTiles[row][col] = fn_80226850(&p, 0);
                        gBgRewrite_8053AC98->fn_802A381C(&p, 0, mTilesB[row][col]);
                        gBgRewrite_8053AC98->fn_802A381C(&p, 1, mTilesA[row][col]);
                        p.y += 16.0f;
                    }
                    p.x += 16.0f;
                }
                field_0x24c = 0;
            }
            s32 v = (field_0x90 >> 7) & 0x1F;
            s32 layer = v == 0 ? 0 : v + 7;
            getGameView_802D6A9C()->fn_802D79E8(layer, 8.0f, 0.0f, true);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x263, 0, 0, NULL, NULL, true, 1.0f);
            field_0x230 = 0;
            field_0x234 = 60;
        }
        break;
    }
    JGeometry::TVec3f delta = mPos;
    delta.sub(mPrevPos);
    if (delta.squared() > 32.0f * JGeometry::TUtil<f32>::epsilon()) {
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
                JGeometry::TVec3f q = actor->mPos;
                q.add(delta);
                actor->mPos = q;
            }
        }
    }
    field_0x238.set(mPos.x, mPos.y, mPos.z);
}

// FIXME(shotgun): 95.0 %, missing/extra: callee-saved register order in the prologue, 3 instructions
inline void drawSmp_802965BC(s32 layer, const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, const char* name) {
    if (layer < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(pos, res, params);
    }
}

// FIXME: 98.96 %, different + reloc-name; behaviour clean.
void cls_804A7AE0::draw() {
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
        drawSmp_802965BC(field_0x4, &pos, &params, field_0x2b6 == 0 ? "house2_smp.bin" : "house_smp.bin");
        params.field_0x1c++;
        drawSmp_802965BC(field_0x4, &pos, &params, "house1_smp.bin");
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
        drawSmp_802965BC(field_0x4, &pos, &params, a);
        params.field_0x1c++;
        drawSmp_802965BC(field_0x4, &pos, &params, b);
        params.field_0x1c--;
        pos.x += 48.0f;
        const char* c;
        const char* d;
        if (field_0x2b6 == 0) {
            c = "room2_smp.bin";
            d = "room3_smp.bin";
        } else {
            c = "room_smp.bin";
            d = "room1_smp.bin";
        }
        drawSmp_802965BC(field_0x4, &pos, &params, c);
        params.field_0x1c++;
        drawSmp_802965BC(field_0x4, &pos, &params, d);
        params.field_0x1c--;
    } else {
        drawSmp_802965BC(field_0x4, &pos, &params, "house3_smp.bin");
        params.field_0x1c++;
        drawSmp_802965BC(field_0x4, &pos, &params, "house31_smp.bin");
        params.field_0x1c--;
    }
}

// FIXME(shotgun): 84.68 %, missing/extra + reloc-name; behaviour drift.
// early-out branch layout, 10 instructions fewer
void cls_804A7AE0::fn_802961AC() {
    s32 serial = fn_801F2718('SLR3');
    if (serial < 0) {
        return;
    }
    ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (other == NULL || other->field_0x94 != 5) {
        return;
    }
    field_0x2d0.sub(mPos, other->mPos);
    f32 sq = field_0x2d0.squared();
    if (sq > 32.0f * JGeometry::TUtil<f32>::epsilon()) {
        field_0x2d0.scale(JGeometry::TUtil<f32>::inv_sqrt(sq));
    }
    field_0x3c = field_0x2d0;
    startSound_801F0E34(0x30075, 0);
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            if (mEmitters[i][j] == NULL) {
                JGeometry::TVec3f pos = mPos;
                pos.x += 50.0f * (f32)i - 25.0f;
                pos.y += 50.0f * (f32)j - 25.0f;
                mEmitters[i][j] = spawnEmitter_8013CC50(field_0x4, &pos, 0x3d3, 0, -1, NULL, NULL, true, 3.0f);
            }
        }
    }
    s32 v = (field_0x90 >> 7) & 0x1F;
    s32 layer = v == 0 ? 0 : v + 7;
    getGameView_802D6A9C()->fn_802D79E8(layer, 16.0f, 0.0f, true);
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
    field_0x24c = 0;
    bakeTiles();
    mTimer = 120;
    mTimerMax = 120;
    field_0x2ba = 1;
}

// FIXME(shotgun): 99.48 %, scheduling+regalloc + reloc-name; behaviour clean.
// 4 opcodes differ
void cls_804A7AE0::fn_80295EE8() {
    if (mTimer <= 0) {
        vfunc_0x24();
        return;
    }
    if (mTimerMax - mTimer < 60) {
        field_0x3c = field_0x2d0;
        f32 t = 2.0f * ((f32)(mTimer + 60 - mTimerMax) / 60.0f);
        field_0x3c.scale(t);
        mPos.add(field_0x3c);
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
        field_0x3c.set(0.0f, 0.0f, 0.0f);
    }
    for (s32 i = 0; i < 4; i++) {
        s32 v = (field_0x90 >> 7) & 0x1F;
        s32 layer = v == 0 ? 0 : v + 7;
        if (getPlayerLayer_8023DE58(i) == layer && !fn_80235274(i)) {
            fn_80238548(i, 0, (const JGeometry::TVec2f*)getPlayerPos_8023E724(i), 12);
        }
    }
    mTimer--;
}

// FIXME(shotgun): 94.44 %, missing/extra + reloc-name; behaviour clean.
// loop register assignment (this/index swapped) and 2 instructions
void cls_804A7AE0::vfunc_0x24() {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && !fn_8022F514(i)) {
            s32 v = (field_0x90 >> 7) & 0x1F;
            s32 layer = v == 0 ? 0 : v + 7;
            if (layer == getPlayerLayer_8023DE58(i)) {
                if (fn_802317F0(i, field_0x4, getPlayerPos_8023E724(i), &mPos, 0x1F)) {
                    mPlayerFlags[i] = 1;
                } else {
                    mPlayerFlags[i] = fn_802317F0(i, field_0x4, getPlayerPos_8023E724(i), &mPos, 0x21);
                }
            }
        }
    }
    const char* stage = getGameView_802D6A9C()->fn_802D9C4C();
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x82, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x83, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x84, 1, 0, NULL, NULL, true, 1.0f);
    if (stage[0] == 0x72 || stage[0] == 0x73) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x80, 1, 0, NULL, NULL, true, 1.0f);
    } else {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x81, 1, 0, NULL, NULL, true, 1.0f);
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

// FIXME(shotgun): 65.46 %, missing/extra + reloc-name; behaviour drift.
bool cls_804A7AE0::fn_802957CC(u8 dir) {
    JGeometry::TVec3f oldPos = mPos;
    bool ok = true;
    field_0x3c.set(lbl_8046C268[dir][0], lbl_8046C268[dir][1], lbl_8046C268[dir][2]);
    mPos.add(field_0x3c);
    mBox.i.x = -44.0f;
    mBox.i.y = -44.0f;
    mBox.f.x = 44.0f;
    mBox.f.y = 44.0f;
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* src = field_0x4 < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f view_box;
    view_box = *src;
    view_box.i.x -= 32.0f;
    view_box.i.y -= 32.0f;
    view_box.f.x += 32.0f;
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
            if (!gBgRewrite_8053AC98->fn_802A1490(&p) ||
                fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &p, mSerial, NULL)) {
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
            if (!gBgRewrite_8053AC98->fn_802A1490(&p) ||
                fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &p, mSerial, NULL)) {
                ok = false;
                break;
            }
            p.x += 16.0f;
        }
    }
    if (!ok) {
        ok = true;
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x + mPos.x;
        box.i.y = mBox.i.y + mPos.y;
        box.f.x = mBox.f.x + mPos.x;
        box.f.y = mBox.f.y + mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                JGeometry::TVec3f q = *getPlayerPos_8023E724(i);
                q.x += lbl_8046C298[dir][0];
                q.y += lbl_8046C298[dir][1];
                q.z += lbl_8046C298[dir][2];
                if (!gBgRewrite_8053AC98->fn_802A1490(&q) ||
                    fn_8020D58C()->mLists[fn_8020D58C()->field_0x1].fn_8020D6AC(field_0x4, &q, mSerial, NULL)) {
                    ok = false;
                }
            }
        }
    }
    mPos = oldPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mBox.i.x = -48.0f;
    mBox.i.y = -48.0f;
    mBox.f.x = 48.0f;
    mBox.f.y = 48.0f;
    return ok;
}
