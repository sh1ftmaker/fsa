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
#include "game/act/act_strange_block.h"
#include "game/act/act_obj_lift.h"
#include "game/act/act_seesaw_lift.h"

// The push directions fn_802C3AE4 gives a kicked kind-6 key, by field_0x2ae - 1.
static const f32 lbl_8046D560[4][2] = {{0.5f, 0.0f}, {-0.5f, 0.0f}, {0.0f, 0.5f}, {0.0f, -0.5f}};

// 0x802C52DC.
// FIXME(shotgun): 95.50 %, missing/extra + reloc-name
void SmallKey_804A8CD8::init() {
    field_0x280 = field_0x90 & 0x1F;
    field_0x27c = (field_0x90 >> 7) & 0xF;
    field_0x278 = (field_0x90 >> 12) & 0x1F;
    field_0x270 = (field_0x90 >> 22) & 0x1F;
    field_0x274 = (field_0x90 >> 17) & 0x1F;
    u32 held = (field_0x90 >> 5) & 1;
    field_0x26c = field_0x4;
    field_0x290 = -1;
    field_0x28c = -1;
    if (field_0x27c != 1) {
        field_0xb0 = 0x20000;
    }
    field_0x284 = 2;
    field_0x2b1 = 0;
    field_0x2b5 = 0;
    if ((field_0x90 & 0x800) == 0) {
        field_0x230.x = mPos.x;
        field_0x230.y = mPos.y;
        s32 row = getGameView_802D6A9C()->field_0x18;
        s32 col = getGameView_802D6A9C()->field_0x14;
        field_0x238 = ((col & 0xFF) << 16) | ((field_0x280 << 24) & 0x1F000000) | ((row & 0xFF) << 8) |
                      (field_0x4 & 0xFF);
        field_0x23c = 0;
    } else if (held != 0) {
        field_0x2b5 = 1;
    }
    switch (field_0x27c) {
    case 0:
        if (getGameView_802D6A9C()->fn_802D7544(field_0x16c)) {
            mAlive = false;
            break;
        }
        getGameView_802D6A9C()->fn_802D7584(field_0x16c, 1);
        field_0x240 = 2;
        break;
    case 1:
        if (fn_8022F9E4(field_0x278, mSerial, false, false)) {
            field_0x240 = 4;
        } else {
            field_0x240 = 8;
            field_0x23c = field_0x278;
        }
        break;
    case 2:
        if (!getGameView_802D6A9C()->fn_802D800C(field_0x270)) {
            field_0x240 = 0;
        } else {
            mAlive = false;
        }
        break;
    case 3:
        field_0x240 = 6;
        break;
    case 4:
        if (!getGameView_802D6A9C()->fn_802D800C(field_0x270)) {
            field_0x240 = 1;
        } else {
            mAlive = false;
        }
        break;
    case 5: {
        field_0x240 = 4;
        field_0x2b1 = 1;
        u32 value = (field_0x23c << 30) | (field_0x2b5 << 29) | ((field_0x280 << 24) & 0x1F000000) |
                    (field_0x4 & 0xFF);
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            s32 row = getGameView_802D6A9C()->field_0x18;
            s32 col = getGameView_802D6A9C()->field_0x14;
            value |= ((row & 0xFF) << 8) | ((col & 0xFF) << 16);
        } else {
            value = field_0x2aa << 8;
        }
        JGeometry::TVec2f a(field_0x230.x, field_0x230.y);
        JGeometry::TVec2f b(mPos.x, mPos.y);
        s32 layer = field_0x238;
        getGameView_802D6A9C()->fn_802D6D70(a, b, value, layer);
        break;
    }
    case 6:
        if (getGameView_802D6A9C()->fn_802D7544(field_0x16c)) {
            mAlive = false;
            break;
        }
        field_0x240 = 2;
        field_0xb0 = 0;
        field_0x240 = 9;
        break;
    default:
        field_0x240 = 2;
        break;
    }
    field_0x2ad = 0xFF;
    field_0x2b4 = 1;
    field_0x260 = mPos;
    field_0x26c = field_0x4;
    if (field_0x280 == 0) {
        mBox.i.x = -4.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 4.0f;
        mBox.f.y = 6.0f;
    } else if (field_0x280 == 1 || field_0x280 >= 6) {
        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
    } else {
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
    }
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    field_0x288 = 0;
    field_0x2b2 = 0;
    field_0x2b3 = 0;
    field_0x2af = 0;
    field_0x294 = 0;
    field_0x2ae = 0;
    field_0x2b0 = false;
    field_0x2a8 = 0xFFFF;
    field_0x298 = -1;
    field_0x29c = -1;
    s32 row = getGameView_802D6A9C()->field_0x18;
    s32 col = getGameView_802D6A9C()->field_0x14;
    field_0x2aa = (row & 0xFF) | ((col << 8) & 0xFF00);
    field_0x2a0 = -1;
    if (held == 0) {
        field_0x2a4 = -1;
    }
}

void SmallKey_804A8CD8::fn_802C5284() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
}

// FIXME(shotgun): 90.8 %, missing/extra + reloc-name
// The key on the floor: kind 6 slides with friction; a player touching it picks it up
// (kinds 3-5 spawn a 'TKRA' instead), players walking into it push it (field_0x2ae the
// direction), pits and water drop it; keeps its place registered with the view.
void SmallKey_804A8CD8::fn_802C3AE4() {
    if (field_0x2b2 == 0) {
        return;
    }
    if (field_0x280 == 6) {
        mPos.add(field_0x3c);
        if (field_0x3c.x > 0.05f) {
            field_0x3c.x -= 0.05f;
        } else if (field_0x3c.x < -0.05f) {
            field_0x3c.x += 0.05f;
        } else {
            field_0x3c.x = 0.0f;
        }
        if (field_0x3c.y > 0.05f) {
            field_0x3c.y -= 0.05f;
        } else if (field_0x3c.y < -0.05f) {
            field_0x3c.y += 0.05f;
        } else {
            field_0x3c.y = 0.0f;
        }
    }
    JGeometry::TBox2f box = mBox;
    box.addPos(mPos.x, mPos.y);
    for (s32 i = 0; i < 4; i++) {
        if (!fn_8023F264(i) || !fn_80239C00(i, &box, field_0x4)) {
            continue;
        }
        s32 kind = field_0x280;
        if (kind == 3 || kind == 4 || kind == 5) {
            getActorManager_801F856C()->spawnActor_801F9484('TKRA', field_0x4, &mPos,
                                                            kind + (i << 12) + 0x210, -1, -1);
            JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
            s32 layer = field_0x238;
            getGameView_802D6A9C()->fn_802D6C10(pos, layer);
            fn_801F3668();
            return;
        }
        if (fn_8022F9E4(i, mSerial, true, false)) {
            field_0x284 = 10;
            field_0x240 = 3;
            JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
            s32 layer = field_0x238;
            getGameView_802D6A9C()->fn_802D6C10(pos, layer);
            field_0x2ae = 0;
            field_0x23c = i;
            break;
        }
    }
    if (field_0x240 != 2) {
        return;
    }
    u16 code = fn_80226850(&mPos, field_0x4 + (field_0x4 < 8 ? 1 : 8));
    if (code != 0 && code != 0x39) {
        field_0x2ad = 0x80;
        field_0x2ac = 2;
    }
    {
        JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
        s32 layer = field_0x238;
        getGameView_802D6A9C()->fn_802D6C10(pos, layer);
    }
    u8 bit = 1;
    u8 players = fn_801F2740(-1);
    if (players != 0) {
        if (fn_802C2300(players) == true) {
            field_0x240 = 0xB;
            return;
        }
        field_0x2b5 = 1;
        u16 tile = fn_8022461C(&mPos, field_0x4);
        if (tile == 0xD || tile == 0x3B) {
            s32 player;
            if (players & 1) {
                player = 0;
            } else if (players & (bit <<= 1)) {
                player = 1;
            } else if (players & (bit <<= 1)) {
                player = 2;
            } else if (players & (bit <<= 1)) {
                player = 3;
            } else {
                player = 4;
            }
            if (fn_8022F6E8(player, mSerial) == true) {
                field_0x29c = player;
                field_0x240 = 10;
                field_0x2b2 = 0;
            }
        } else {
            for (s32 i = 0; i < 4; i++, bit <<= 1) {
                if (!(players & bit)) {
                    continue;
                }
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(i);
                s32 dx = (s32)fabs(mPos.x - playerPos.x);
                s32 dy = (s32)fabs(mPos.y - playerPos.y);
                s32 dir = fn_80230CFC(i);
                if ((dx >= 0 && dx <= 15 && dir == 0) || dir == 1) {
                    if (mPos.x > playerPos.x) {
                        mPos.x += (15 - dx) >> 1;
                        field_0x2ae = 1;
                    } else {
                        mPos.x -= (15 - dx) >> 1;
                        field_0x2ae = 2;
                    }
                }
                if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
                    if (mPos.x > playerPos.x) {
                        mPos.x += (15 - dx) >> 1;
                    } else {
                        mPos.x -= (15 - dx) >> 1;
                    }
                } else if ((dy >= 0 && dy <= 15 && dir == 3) || dir == 2) {
                    if (mPos.y > playerPos.y) {
                        mPos.y += (15 - dy) >> 1;
                        field_0x2ae = 3;
                    } else {
                        mPos.y -= (15 - dy) >> 1;
                        field_0x2ae = 4;
                    }
                }
                if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
                    field_0x2ae = 0;
                }
                field_0x23c = i;
            }
        }
    } else if (field_0x280 == 6 && field_0x2ae != 0) {
        field_0x3c.x = lbl_8046D560[field_0x2ae - 1][0];
        field_0x3c.y = lbl_8046D560[field_0x2ae - 1][1];
        field_0x2ae = 0;
    }
    JGeometry::TVec3f wall(0.0f, 0.0f, 0.0f);
    fn_801F2618(&wall, false);
    if (wall.y != 0.0f && wall.x != 0.0f) {
        mPos.y = (u32)((s32)mPos.y & ~0xF);
        if (wall.y < 0.0f) {
            mPos.y += 10.0f;
        } else {
            mPos.y += 6.0f;
        }
        mPos.x = (u32)((s32)mPos.x & ~0xF);
        if (wall.x < 0.0f) {
            mPos.x += 10.0f;
        } else {
            mPos.x += 6.0f;
        }
    }
    if (field_0x240 != 10) {
        u32 value = (field_0x23c << 30) | (field_0x2b5 << 29);
        value |= (field_0x280 << 24) & 0x1F000000;
        value |= field_0x4 & 0xFF;
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            s32 row = getGameView_802D6A9C()->field_0x18;
            s32 col = getGameView_802D6A9C()->field_0x14;
            value |= ((row & 0xFF) << 8) | ((col << 16) & 0xFF0000);
        } else {
            value = field_0x2aa << 8;
        }
        JGeometry::TVec2f home(field_0x230.x, field_0x230.y);
        s32 layer = field_0x238;
        JGeometry::TVec2f pos(mPos.x, mPos.y);
        getGameView_802D6A9C()->fn_802D6D70(pos, home, value, layer);
    }
    u16 tile = fn_8022461C(&mPos, field_0x4);
    if (field_0x298 != -1) {
        tile = 0;
    }
    bool fell;
    if (tile == 3) {
        if (fn_801EE24C() == true) {
            field_0x240 = 4;
            fell = true;
        } else {
            fell = true;
            field_0x2b0 = true;
        }
    } else if (tile == 0xE) {
        if (fn_802DE9A0()) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5005F, 0);
        } else if (fn_802DEB58()) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x45E, -1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x45F, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5003F, 0);
        }
        fell = true;
        field_0x2b0 = true;
    } else {
        fell = false;
    }
    if (fell == true && getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
        field_0x2b2 = 0;
        fn_802C2154();
    }
}

void SmallKey_804A8CD8::fn_802C2BD0() {
    field_0x2b0 = true;
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
}

void SmallKey_804A8CD8::fn_802C2B64() {
    if (field_0x240 == 2) {
        field_0x2b0 = true;
        JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
        s32 layer = field_0x238;
        getGameView_802D6A9C()->fn_802D6C10(pos, layer);
    }
}

// 0x802C2840.
// FIXME(shotgun): 87.92 %, missing/extra + reloc-name
void SmallKey_804A8CD8::fn_802C2840() {
    s32 layer = field_0x4;
    if (getGameView_802D6A9C()->field_0x131df[layer] != 0 || mPos.z > 2.0f || field_0x3c.z > 0.0f) {
        field_0x298 = -1;
        return;
    }
    if (field_0x280 == 0) {
        mBox.i.x = -2.0f;
        mBox.i.y = -3.0f;
        mBox.f.x = 2.0f;
        mBox.f.y = 3.0f;
    } else if (field_0x280 == 1 || field_0x280 >= 6) {
        mBox.i.x = -3.0f;
        mBox.i.y = -3.0f;
        mBox.f.x = 3.0f;
        mBox.f.y = 3.0f;
    } else {
        mBox.i.x = -4.0f;
        mBox.i.y = -4.0f;
        mBox.f.x = 4.0f;
        mBox.f.y = 4.0f;
    }
    JGeometry::TBox2f box(mBox);
    if (field_0x280 == 0) {
        mBox.i.x = -4.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 4.0f;
        mBox.f.y = 6.0f;
    } else if (field_0x280 == 1 || field_0x280 >= 6) {
        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
    } else {
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
    }
    box.addPos(mPos.x, mPos.y);
    s32 viewLayer = field_0x4;
    JGeometry::TBox2f* screen = viewLayer < 8 ? &getGameView_802D6A9C()->mVisibleBox : &getGameView_802D6A9C()->field_0xdc;
    box.addPos(-screen->i.x, -screen->i.y);
    s32 slot = field_0x298;
    if (slot != -1) {
        ActorBase_804A5770* carrier = getActorManager_801F856C()->mActors[slot];
        if (fn_802C2684(carrier, &box)) {
            mPos.z = 0.0f;
            JGeometry::TVec3f d = carrier->mPos - field_0x254;
            mPos += d;
            field_0x254 = carrier->mPos;
            return;
        }
    }
    field_0x298 = -1;
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        if (i != mSerial) {
            ActorBase_804A5770* other = getActorManager_801F856C()->mActors[i];
            if (fn_802C2684(other, &box)) {
                field_0x298 = i;
                mPos.z = 0.0f;
                field_0x254 = other->mPos;
            }
        }
    }
}

// 0x802C2684.
// FIXME(shotgun): 91.07 %, missing/extra
bool SmallKey_804A8CD8::fn_802C2684(ActorBase_804A5770* other, JGeometry::TBox2f* box) {
    if (other == NULL) {
        return false;
    }
    u32 id = other->mActorId;
    if (id == ACTOR_LIFT || id == ACTOR_LIF2 || id == ACTOR_CLOD ||
        (id == ACTOR_OBLF && static_cast<OBJLift_804ACA00*>(other)->field_0x230 != 0) ||
        (id == ACTOR_SSLF && (static_cast<SeesawLift_804AD348*>(other)->field_0x230 & 0x7FFF) == 0)) {
        JGeometry::TBox2f theirs(other->mBox);
        theirs.addPos(other->mPos.x, other->mPos.y);
        s32 layer = other->field_0x4;
        JGeometry::TBox2f* screen = layer < 8 ? &getGameView_802D6A9C()->mVisibleBox : &getGameView_802D6A9C()->field_0xdc;
        theirs.addPos(-screen->i.x, -screen->i.y);
        if (theirs.intersect(*box)) {
            return true;
        }
    }
    return false;
}

// 0x802C24A8.
// FIXME(shotgun): 98.70 %, regalloc-only + reloc-name
void SmallKey_804A8CD8::fn_802C24A8() {
    JGeometry::TBox2f ours(mBox);
    ours.addPos(mPos.x, mPos.y);
    s32 serial = -1;
    while (true) {
        serial = getActorManager_801F856C()->fn_801F78F4('STBL', serial);
        if (serial < 0) {
            return;
        }
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (other == NULL || field_0x4 != other->field_0x4) {
            continue;
        }
        s32 state = static_cast<StrangeBlock_804AC2D8*>(other)->field_0x25c;
        if (state != 0 && state != 2) {
            continue;
        }
        JGeometry::TBox2f theirs(other->mBox);
        theirs.addPos(other->mPos.x, other->mPos.y);
        if (theirs.intersect(ours)) {
            field_0x3c.x = -field_0x3c.x;
            field_0x3c.y = -field_0x3c.y;
            mPos.add(field_0x3c);
            field_0x3c.x /= 3.0f;
            field_0x3c.y /= 3.0f;
            return;
        }
    }
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
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
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

// 0x802C2154.
// FIXME(shotgun): 97.93 %, different
bool SmallKey_804A8CD8::fn_802C2154() {
    JGeometry::TBox2f ours(mBox);
    ours.i.x -= 4.0f;
    ours.i.y -= 4.0f;
    ours.f.x += 4.0f;
    ours.f.y += 4.0f;
    ours.addPos(mPos.x, mPos.y);
    s32 serial = -1;
    while (true) {
        serial = getActorManager_801F856C()->fn_801F78F4('WARP', serial);
        if (serial < 0) {
            return false;
        }
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (other == NULL || field_0x4 != other->field_0x4) {
            continue;
        }
        if ((other->field_0x90 & 0x7F) != 4) {
            continue;
        }
        JGeometry::TBox2f theirs(other->mBox);
        theirs.addPos(other->mPos.x, other->mPos.y);
        if (theirs.intersect(ours)) {
            field_0x2b0 = true;
            return true;
        }
    }
}

// 0x802C1EC4.
// FIXME(shotgun): 93.93 %, missing/extra + reloc-name
void SmallKey_804A8CD8::draw() {
    if (field_0x27c == 8 || field_0x2b4 == 0 || field_0x280 >= 16 || field_0x240 < 2) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = field_0x2ad;
    if (field_0x294 >= 0x2BD4) {
        if (field_0x294 & 2) {
            params.field_0x10.a = 0;
        }
    } else if (field_0x294 >= 0x2B20) {
        if (field_0x294 & 4) {
            params.field_0x10.a = 0;
        }
    } else if (field_0x294 >= 0x2AA8) {
        if (field_0x294 & 8) {
            params.field_0x10.a = 0;
        }
    } else if (field_0x294 >= 0x2A30) {
        if (field_0x294 & 0x10) {
            params.field_0x10.a = 0;
        }
    }
    if (field_0xc4) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    } else {
        params.field_0x1c = field_0x2ac;
    }
    s32 frame = field_0x280;
    if (field_0x2ac != 0) {
        frame += 10;
    }
    params.field_0x2d = 1;
    u8 flag = field_0xc4;
    if (field_0x240 == 7 || field_0x240 == 10 || (field_0x27c == 2 && field_0xc4 == 0)) {
        params.field_0x1d = 7;
        flag = 1;
    }
    s32 flags = 7;
    if (field_0xc4) {
        flags = -1;
    }
    fn_801F02BC("enemy_both2_sob.bin", frame + 0x2D, &params, flags, 0, flag, 1.0f, 1.0f);
    if (field_0x240 == 7) {
        field_0xc4 = 0;
    }
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

void SmallKey_804A8CD8::fn_802C1D90() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    getGameView_802D6A9C()->fn_802D6C10(pos, layer);
    mAlive = false;
}

void SmallKey_804A8CD8::fn_802C1D30(const JGeometry::TVec3f& pos, s32 layer) {
    field_0x260 = pos;
    field_0x26c = layer;
    s32 row = getGameView_802D6A9C()->field_0x18;
    s32 col = getGameView_802D6A9C()->field_0x14;
    field_0x2aa = ((col & 0xFF) << 8) | (row & 0xFF);
}

// The tile codes as act_bombchu's vfunc_0x4C (a != chain, the 0x25 / 0x26 results narrowed as
// bytes).
// FIXME: 98.52. Scheduling and register allocation only: pos and layer take r30 / r28 the
// other way round (the target copies pos to r30 in the prologue, layer to r28 after the
// first argument move), and for field_0x3c.x == 0.0f the target loads the field before the
// constant.
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
    if (tileCode != 0 && tileCode != 4 && tileCode != 5 && tileCode != 6 && tileCode != 7 &&
        tileCode != 2 && tileCode != 3 && tileCode != 0x50 && tileCode != 0xE && tileCode != 0xC &&
        tileCode != 0x2C && tileCode != 0x2D && tileCode != 0x2E && tileCode != 0x2F &&
        tileCode != 0x20 && tileCode != 0xD && tileCode != 0x39 && tileCode != 0x3A &&
        tileCode != 0x3B && tileCode != 0x4C && tileCode != 0x4D && tileCode != 0x4E &&
        tileCode != 0x4F && tileCode != 0x24) {
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
            return (u8)(getGameView_802D6A9C()->field_0x171 == 0);
        }
        if (tileCode == 0x26) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
        }
        if (tileCode == 0x18 || tileCode == 0x19 || tileCode == 0x1A || tileCode == 0x1B) {
            return false;
        }
        if (tileCode != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
            return true;
        }
    }
    return false;
}

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

// The key's registration with the view (fn_802D6D70): the key's value packs its mode,
// flag, state, id and layer.
// FIXME(shotgun): 79.96 %, missing/extra
void SmallKey_804A8CD8::fn_802C1778() {
    if (field_0x4 >= 8 && field_0x2a8 != 0xFFFF) {
        bool registered = false;
        if (fn_8022F478(field_0x23c) == true) {
            if (fn_8023EF50(field_0x23c) == mSerial) {
                registered = true;
            }
        } else if (fn_8022F478(field_0xd0) == true) {
            if (field_0x240 == 10 && field_0xcc != 0) {
                registered = true;
            }
        }
        if (registered) {
            JGeometry::TVec2f a(field_0x230.x, field_0x230.y);
            JGeometry::TVec2f b(mPos.x, mPos.y);
            u32 value = (field_0x23c << 30) | (field_0x2b5 << 29) | ((field_0x280 << 24) & 0x1F000000) |
                        (field_0x2a8 << 8) | (field_0x4 & 0xFF);
            s32 layer = field_0x238;
            getGameView_802D6A9C()->fn_802D6D70(a, b, value, layer);
            field_0x2a8 = 0xFFFF;
        }
    }
}

// FIXME(shotgun): 83.45 %, different
void SmallKey_804A8CD8::fn_802C1674() {
    if (field_0x240 == 4 && field_0xc4 == 0 && field_0x2af != 0 && field_0x2a8 != 0xFFFF) {
        JGeometry::TVec2f a(field_0x230.x, field_0x230.y);
        JGeometry::TVec2f b(mPos.x, mPos.y);
        u32 value = (field_0x23c << 30) | (field_0x2b5 << 29) | ((field_0x280 << 24) & 0x1F000000) |
                    (field_0x2a8 << 8) | (field_0x4 & 0xFF);
        s32 layer = field_0x238;
        getGameView_802D6A9C()->fn_802D6D70(a, b, value, layer);
        field_0x2a8 = 0xFFFF;
        field_0x2af = 0;
    }
}

// 0x802C1614.
SmallKey_804A8CD8::~SmallKey_804A8CD8() {}
