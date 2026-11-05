#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_small_key.h"

SmallKey_804A8CD8::~SmallKey_804A8CD8() {}

bool SmallKey_804A8CD8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x288 == 0) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            return true;
        }
        return fn_802C1970(layer, pos);
    }
    field_0x288--;
    return false;
}

// FIXME: 85.34 %. The original keeps the tile-code compares as one linear
// chain of cmpwi/beq to the shared return (no switch, no range folding); the
// switch here is a binary search, and the 0x10-0x17 / 0x18-0x1B chains fold
// into one range check each (the original splits off 0x17 and 0x1B). pos and layer take r30/r28 the other way
// round (the original copies pos to r30 before this, layer to r28 after), and
// the mPos.z / field_0x3c float compares load their operands in another order.
bool SmallKey_804A8CD8::fn_802C1970(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tileCode = fn_8022461C(pos, layer);
    s32 rawCode = fn_80226850(pos, layer);
    if (rawCode == 0x230 || rawCode == 0x231) {
        return false;
    }
    if (rawCode == 0x19E || rawCode == 0x19F || rawCode == 0x350 || rawCode == 0x351 || rawCode == 0x342 ||
        rawCode == 0x343 || rawCode == 0x345 || rawCode == 0x346 || rawCode == 0x355 || rawCode == 0x356 ||
        rawCode == 0x2EE || rawCode == 0x2EF || rawCode == 0x2FE || rawCode == 0x2FF || rawCode == 0x338 ||
        rawCode == 0x339 || rawCode == 0x32A || rawCode == 0x32B || rawCode == 0x32D || rawCode == 0x32E ||
        rawCode == 0x33D || rawCode == 0x33E || rawCode == 0x2CE || rawCode == 0x2CF || rawCode == 0x2DE ||
        rawCode == 0x2DF || rawCode == 0x3B0 || rawCode == 0x3B1 || rawCode == 0x3A2 || rawCode == 0x3A3 ||
        rawCode == 0x3A5 || rawCode == 0x3A6 || rawCode == 0x3B6 || rawCode == 0x398 || rawCode == 0x399 ||
        rawCode == 0x38A || rawCode == 0x38B || rawCode == 0x39D || rawCode == 0x38D || rawCode == 0x38E ||
        rawCode == 0x39E) {
        return true;
    }
    if ((field_0x280 == 0 || field_0x280 == 1 || field_0x280 == 2 || field_0x280 == 6 || field_0x280 == 9) &&
        (rawCode == 0x1A8 || rawCode == 0x1A9 || rawCode == 0x1AA)) {
        return true;
    }
    if (viewLayerFlag(field_0x4) && rawCode == 0x14) {
        return false;
    }
    switch (tileCode) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 2:
    case 3:
    case 0x50:
    case 0xE:
    case 0xC:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x20:
    case 0xD:
    case 0x39:
    case 0x3A:
    case 0x3B:
    case 0x4C:
    case 0x4D:
    case 0x4E:
    case 0x4F:
    case 0x24:
        return false;
    }
    if (tileCode == 0x10 || tileCode == 0x11 || tileCode == 0x12 || tileCode == 0x13 ||
        tileCode == 0x14 || tileCode == 0x15 || tileCode == 0x16 || tileCode == 0x17) {
        if (field_0x280 == 7 || field_0x280 == 8) {
            return false;
        }
        if (mPos.z > 15.0f && field_0x3c.x == 0.0f) {
            field_0x288 = 300;
            if (mPos.z < 18.0f) {
                mPos.z = 18.0f;
            }
            return false;
        }
    }
    if (rawCode == 0x3A) {
        return true;
    }
    if (rawCode == 0x3D) {
        return false;
    }
    if (tileCode == 0x25) {
        return getGameView_802D6A9C()->field_0x171 == 0;
    }
    if (tileCode == 0x26) {
        return getGameView_802D6A9C()->field_0x171 == 1;
    }
    if (tileCode == 0x18 || tileCode == 0x19 || tileCode == 0x1A || tileCode == 0x1B) {
        return false;
    }
    if (tileCode == 0x22) {
        return getGameView_802D6A9C()->field_0x170 == 1;
    }
    return true;
}

void SmallKey_804A8CD8::fn_802C1D30(const JGeometry::TVec3f& pos, s32 layer) {
    field_0x260 = pos;
    field_0x26c = layer;
    s32 row = getGameView_802D6A9C()->field_0x18;
    s32 col = getGameView_802D6A9C()->field_0x14;
    field_0x2aa = ((col & 0xFF) << 8) | (row & 0xFF);
}

void SmallKey_804A8CD8::fn_802C1D90() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
    mAlive = false;
}

void SmallKey_804A8CD8::fn_802C1DFC() {
    s32 layer = field_0x4;
    if (layer >= 8) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, layer, &mPos, 0, -1, -1);
    } else {
        spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
    }
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
    mAlive = false;
}

bool SmallKey_804A8CD8::fn_802C2300(u8 playerBits) {
    if (field_0x280 != 0) {
        return false;
    }
    if (!fn_802DE7CC()) {
        return false;
    }

    u8 player = 0;
    if (playerBits & 2) {
        player = 1;
    } else if (playerBits & 4) {
        player = 2;
    } else if (playerBits & 8) {
        player = 3;
    }

    if (fn_8022CAF8(player) == 4) {
        if (fn_8022F478(player)) {
            return false;
        }
        field_0x2a4 = player;
        u8 index = player;
        if (fn_8022F514(player)) {
            index = fn_8022F3DC(player);
        }
        field_0x2a0 = index;
    } else {
        u8 index = player;
        if (fn_8022F514(player)) {
            index = fn_8022F3DC(player);
        }
        field_0x2a4 = index;
        field_0x2a0 = index;
    }

    if (!getGameView_802D6A9C()->fn_802D46D8(field_0x2a0, 0x168, false)) {
        if (lbl_8053AAF8->field_0x7e == 1) {
            if (fn_802308E0(field_0x2a0) == true) {
                fn_8022FE80(field_0x2a4, field_0x4, mSerial, 0, 0);
            }
        } else {
            if (fn_8023077C() == true) {
                fn_8022FE80(field_0x2a4, field_0x4, mSerial, 0, 0);
            }
        }
        return true;
    }
    return false;
}

void SmallKey_804A8CD8::fn_802C2B64() {
    if (field_0x240 == 2) {
        field_0x2b0 = true;
        JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
        s32 layer = field_0x238;
        getGameView_802D6A9C()->fn_802D6C10(pos, layer);
    }
}

void SmallKey_804A8CD8::fn_802C2BD0() {
    field_0x2b0 = true;
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
}

void SmallKey_804A8CD8::fn_802C5284() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
}
