#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/BgRewrite.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/PlayerBit.h"
#include "game/GameMath.h"
#include "game/BoxTest.h"
#include "game/cls_804A71A8.h"
#include "game/tu_80379F84.h"
#include "game/act/tu_803D9944.h"
#include "JSystem/JMath/JMath.h"

// gGameRoot_8053AAF8->field_0x4c == 7 as a u8: fn_8027057C and fn_80277F98 test the materialised
// byte (subfic / cntlzw / extrwi.); a plain `return x == 7;` folds into cmpwi / bne.
static inline u8 isGameMode7() {
    return gGameRoot_8053AAF8->field_0x4c == 7 ? true : false;
}

// The current save row's byte 0x7E while the game root's field_0x7f is set, else 0x7F.
static inline u8 saveRowFlag() {
    return gGameRoot_8053AAF8->field_0x7f ? lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x7e
                                    : lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x7f;
}

// The angle of the hit as an s16 (pos -> player).
static inline s16 hitAngle(const Player_804A6F38* player, const JGeometry::TVec2f* pos) {
    JGeometry::TVec2f diff(player->mPos.x, player->mPos.y);
    diff.x -= pos->x;
    diff.y -= pos->y;
    return JMath::atanTable_.atan2_(diff.y, diff.x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

// `box` made relative to the top-left corner of `layer`'s view box.
static inline void toView(JGeometry::TBox2f& box, s32 layer) {
    const JGeometry::TVec2f& origin = layerBox(layer)->i;
    JGeometry::TVec2f offset;
    offset.x = -origin.x;
    offset.y = -origin.y;
    box.addPos(offset);
}

// Whether two boxes overlap (the intersection is not empty); the result is
// the compare chain itself, not TBox::intersect's `? true : false`.
static inline bool overlaps(const JGeometry::TBox2f& a, const JGeometry::TBox2f& b) {
    JGeometry::TBox2f overlap(a);
    overlap.i.setMax(b.i);
    overlap.f.setMin(b.f);
    bool result = false;
    if (overlap.f.x >= overlap.i.x && overlap.f.y >= overlap.i.y) {
        result = true;
    }
    return result;
}

// `box` (on `layer`) against the player's box 0x4d0 (on its layer 0x3b4),
// both made relative to their layer's view corner (fn_802724E0, fn_80272E98).
static inline bool overlapsInView(const Player_804A6F38* player, const JGeometry::TBox2f& box, s32 layer) {
    // The offsets are spelled out (not toView): through toView the original's extra
    // f27 spill is lost (tried, 99 %).
    JGeometry::TBox2f other(box);
    const JGeometry::TVec2f& origin = layerBox(layer)->i;
    JGeometry::TVec2f offset;
    offset.x = -origin.x;
    offset.y = -origin.y;
    other.addPos(offset);
    JGeometry::TBox2f own(player->field_0x4d0);
    const JGeometry::TVec2f& ownOrigin = layerBox(player->field_0x3b4)->i;
    JGeometry::TVec2f ownOffset;
    ownOffset.x = -ownOrigin.x;
    ownOffset.y = -ownOrigin.y;
    own.addPos(ownOffset);
    own.i.setMax(other.i);
    own.f.setMin(other.f);
    return own.f.x >= own.i.x && own.f.y >= own.i.y;
}

// Points 12 (fn_80272768) and 16 (fn_802728AC) in front of the player, by facing field_0x3dc.
static const f32 lbl_8046BD68[4][2] = {{12.0f, 0.0f}, {-12.0f, 0.0f}, {0.0f, 12.0f}, {0.0f, -12.0f}};
static const f32 lbl_8046BD48[4][2] = {{16.0f, 0.0f}, {-16.0f, 0.0f}, {0.0f, 16.0f}, {0.0f, -16.0f}};

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

// The box in front of the player for direction `dir` (0 +x, 1 -x, 2 +y, 3 -y)
// into field_0x5ec; fn_80276078 inlines it with field_0x3dc and with constants.
static inline void setFrontBox(Player_804A6F38* player, s32 dir) {
    JGeometry::TVec3f& pos = player->mPos;
    switch (dir) {
    case 0:
        player->field_0x5ec.set(pos.x + 8.0f, pos.y - 8.0f, pos.x + 16.0f, pos.y + 8.0f);
        player->field_0x3ec = 0;
        player->field_0xb98 = 1;
        break;
    case 1:
        player->field_0x5ec.set(pos.x - 16.0f, pos.y - 8.0f, pos.x - 8.0f, pos.y + 8.0f);
        player->field_0x3ec = 1;
        player->field_0xb98 = 1;
        break;
    case 2:
        player->field_0x5ec.set(pos.x - 8.0f, pos.y + 8.0f, pos.x + 8.0f, pos.y + 16.0f);
        player->field_0x3ec = 2;
        player->field_0xb98 = 1;
        break;
    case 3:
        player->field_0x5ec.set(pos.x - 8.0f, pos.y - 16.0f, pos.x + 8.0f, pos.y - 8.0f);
        player->field_0x3ec = 3;
        player->field_0xb98 = 1;
        break;
    }
}

// Emitter offsets from mPos by facing (fn_80274CB8 is its first user, fn_8027D320).
static const f32 lbl_8046BB48[4][3] = {{-1.0f, 0.0f, -8.0f}, {1.0f, 0.0f, -8.0f}, {5.0f, 0.0f, -8.0f}, {-5.0f, 0.0f, -8.0f}};

// lbl_8046BCA8 (0xA0 bytes) is one object: only its start is referenced (by fn_80275D9C,
// the push per facing) and the linker kept the rest with it. The two later members are
// typed by their contents only (bytes 0 2 4 6 / 0 8 10 12; float triples with z = 2);
// nothing in this unit reads them.
static const struct {
    f32 push[4][2];
    u8 field_0x20[8][4];
    f32 field_0x40[8][3];
} lbl_8046BCA8 = {
    {{1.5f, 0.0f}, {-1.5f, 0.0f}, {0.0f, 1.5f}, {0.0f, -1.5f}},
    {
        {0, 2, 4, 6},
        {0, 2, 4, 6},
        {0, 2, 4, 6},
        {0, 2, 4, 6},
        {0, 2, 4, 6},
        {0, 8, 10, 12},
        {0, 2, 4, 6},
        {0, 8, 10, 12},
    },
    {
        {18.0f, 0.0f, 2.0f},
        {-18.0f, 0.0f, 2.0f},
        {4.0f, 18.0f, 2.0f},
        {-4.0f, -18.0f, 2.0f},
        {-24.0f, 3.0f, 2.0f},
        {24.0f, 3.0f, 2.0f},
        {1.0f, -24.0f, 2.0f},
        {-5.0f, 22.0f, 2.0f},
    },
};

// All four direction hold timers field_0x117c[0..3] running.
static inline bool allDirectionsHeld(const Player_804A6F38* player) {
    for (s32 i = 0; i < 4; i++) {
        if (player->field_0x117c[i] <= 0) {
            return false;
        }
    }
    return true;
}

// A row of the press history field_0x119c: any direction column set.
static inline bool rowPressed(const Player_804A6F38* player, s32 i) {
    bool pressed = false;
    for (s32 j = 0; j < 4; j++) {
        if (player->field_0x119c[i][j] == 1) {
            pressed = true;
        }
    }
    return pressed;
}

// How many of the newest press history entries (timer field_0x117c[4 + i] running and a
// direction column of field_0x119c[i] set) are in a row.
static inline s32 recentPresses(const Player_804A6F38* player) {
    for (s32 i = 0; i < 4; i++) {
        if (player->field_0x117c[i + 4] <= 0) {
            return i;
        }
        if (!rowPressed(player, i)) {
            return i;
        }
    }
    return 4;
}

// The boxes for facing `dir` (0 +x, 1 -x, 2 +y, 3 -y): 0x4B0 a 16x16 box 8-24 ahead (the
// attack box), 0x4A0 the body box stretched 4 back / 16 ahead, 0x30C the point 16 ahead,
// 0x3F0 = dir. fn_80276BB8 uses them for the opposite facing, fn_80276E5C for the facing.
static inline void setBoxesPosX(Player_804A6F38* player) {
    player->field_0x4b0.set(player->mPos.x + 16.0f, player->mPos.y - 8.0f, player->mPos.x + 32.0f, player->mPos.y + 8.0f);
    player->field_0x4a0.set(player->mPos.x - 4.0f, player->mPos.y - 8.0f, player->mPos.x + 16.0f, player->mPos.y + 8.0f);
    player->field_0x30c.set(16.0f, 0.0f, 0.0f);
    player->field_0x30c.add(player->mPos);
    player->field_0x3f0 = 0;
}

static inline void setBoxesNegX(Player_804A6F38* player) {
    player->field_0x4b0.set(player->mPos.x - 32.0f, player->mPos.y - 8.0f, player->mPos.x - 16.0f, player->mPos.y + 8.0f);
    player->field_0x4a0.set(player->mPos.x - 16.0f, player->mPos.y - 8.0f, player->mPos.x + 4.0f, player->mPos.y + 8.0f);
    player->field_0x30c.set(-16.0f, 0.0f, 0.0f);
    player->field_0x30c.add(player->mPos);
    player->field_0x3f0 = 1;
}

static inline void setBoxesPosY(Player_804A6F38* player) {
    player->field_0x4b0.set(player->mPos.x - 8.0f, player->mPos.y + 16.0f, player->mPos.x + 8.0f, player->mPos.y + 32.0f);
    player->field_0x4a0.set(player->mPos.x - 8.0f, player->mPos.y - 4.0f, player->mPos.x + 8.0f, player->mPos.y + 16.0f);
    player->field_0x30c.set(0.0f, 16.0f, 0.0f);
    player->field_0x30c.add(player->mPos);
    player->field_0x3f0 = 2;
}

static inline void setBoxesNegY(Player_804A6F38* player) {
    player->field_0x4b0.set(player->mPos.x - 8.0f, player->mPos.y - 32.0f, player->mPos.x + 8.0f, player->mPos.y - 16.0f);
    player->field_0x4a0.set(player->mPos.x - 8.0f, player->mPos.y - 16.0f, player->mPos.x + 8.0f, player->mPos.y + 4.0f);
    player->field_0x30c.set(0.0f, -16.0f, 0.0f);
    player->field_0x30c.add(player->mPos);
    player->field_0x3f0 = 3;
}

// 0x11B4 = mPos moved 16 toward player `field_0x4`'s own side (4 on flagged layers), the
// centre of the spin states 0x09 / 0x0A (enters fn_8027BA2C / fn_8027B304).
static inline void setSpinCenter(Player_804A6F38* player) {
    player->field_0x11b4 = player->mPos;
    switch (player->field_0x4) {
    case 0:
        if (viewLayerFlag(player->field_0x3b4)) {
            player->field_0x11b4.y -= 4.0f;
        } else {
            player->field_0x11b4.y += 16.0f;
        }
        break;
    case 1:
        player->field_0x11b4.x += 16.0f;
        break;
    case 2:
        player->field_0x11b4.x -= 16.0f;
        break;
    case 3:
        if (viewLayerFlag(player->field_0x3b4)) {
            player->field_0x11b4.y += 4.0f;
        } else {
            player->field_0x11b4.y -= 16.0f;
        }
        break;
    }
}

// Whether the life (the leader's under fn_80240808, as life()) is at its maximum.
static inline bool lifeFull(Player_804A6F38* player) {
    if (player->fn_80240808()) {
        if (fn_80237774(player->fn_80240644()) < fn_80237810(player->fn_80240644())) {
            return false;
        }
    } else if (player->fn_8024189C() < player->fn_802417CC()) {
        return false;
    }
    return true;
}

// Where the 'SWBM' actor of the spin (fn_8027B648) starts, by facing.
static const f32 lbl_8046BD88[4][2] = {{24.0f, 0.0f}, {-24.0f, 0.0f}, {2.0f, 24.0f}, {-2.0f, -18.0f}};

// The push 0x344 handed on to 0x368 (x / y) and cleared.
static inline void passOnPush(Player_804A6F38* player) {
    player->field_0x368.set(player->field_0x344.x, player->field_0x344.y, 0.0f);
    player->field_0x344.set(0.0f, 0.0f, 0.0f);
}

// Landing of state 0x07: state 8 (fn_80246464) or 0.
static inline void landState7(Player_804A6F38* player) {
    if (player->fn_80246464()) {
        if (player->fn_80251AEC(8)) {
            passOnPush(player);
        }
    } else if (player->fn_80251AEC(0)) {
        passOnPush(player);
    }
}

void Player_804A6F38::fn_8027D4F4() {
    s32 count;
    if (field_0x117c[4] <= 0) {
        count = 0;
    } else if (field_0x117c[5] <= 0) {
        count = 1;
    } else if (field_0x117c[6] <= 0) {
        count = 2;
    } else if (field_0x117c[7] <= 0) {
        count = 3;
    } else {
        count = 4;
    }
    if (count >= 4) {
        fn_8027023C();
    }
    if (field_0x45c == 0 || field_0x45c == 1 || field_0x45c == 2) {
        field_0x11cc = 20;
        field_0x11d0 = field_0x3dc;
    } else {
        field_0x11cc = 0;
    }
}

// Enter of state 0x05: pushed down (0x344.z = -4), lifted to 0.5 on a flagged layer, three
// sounds, the emitters 0xCB / 0xCC (callbacks 0x1278 / 0x1290, immortal) and a 'PEFF' actor
// (serial 0x12C0) at 0x12A4 = mPos + the facing's offset.
void Player_804A6F38::fn_8027D320() {
    field_0x458 = 0;
    field_0x460 = true;
    field_0x436 = true;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = -4.0f;
    if (viewLayerFlag(field_0x3b4) && mPos.z < 0.5f) {
        mPos.z = 0.5f;
    }
    fn_8024F0E8(0x10001);
    fn_8024F0E8(0x20001);
    fn_8024F0E8(0x2000e);
    const f32* offset = lbl_8046BB48[field_0x3dc];
    field_0x12a4.set(offset[0], offset[1], offset[2]);
    field_0x12a4.add(mPos);
    if (field_0x1274 == NULL) {
        field_0x1274 = spawnEmitter_8013CC50(field_0x3b4, &field_0x12a4, 0xcb, 0, -1, NULL, &field_0x1278, true, 1.0f);
        if (field_0x1274 != NULL) {
            field_0x1274->setStatus(JPAEMIT_Immortal);
        }
    }
    if (field_0x128c == NULL) {
        field_0x128c = spawnEmitter_8013CC50(field_0x3b4, &field_0x12a4, 0xcc, 0, -1, NULL, &field_0x1290, true, 1.0f);
        if (field_0x128c != NULL) {
            field_0x128c->setStatus(JPAEMIT_Immortal);
        }
    }
    field_0x12c0 = getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x3b4, &field_0x12a4, (field_0x4 & 3) | 8, -1, -1);
}

void Player_804A6F38::fn_8027CBB0() {
    mPos.z = 0.0f;
    field_0x460 = false;
    field_0x436 = false;
    if (field_0x1274 != NULL) {
        field_0x1274->clearStatus(JPAEMIT_Immortal);
        field_0x1274->becomeInvalidEmitter();
        field_0x1274 = NULL;
    }
    if (field_0x128c != NULL) {
        field_0x128c->clearStatus(JPAEMIT_Immortal);
        field_0x128c->becomeInvalidEmitter();
        field_0x128c = NULL;
    }
    s32 serial = field_0x12c0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x12c0 = -1;
    }
}

void Player_804A6F38::fn_8027CB9C() {
    field_0x458 = 0;
    field_0x436 = 1;
}

void Player_804A6F38::fn_8027CB44() {
    if (field_0x3fa < 15) {
        field_0x3fa++;
    } else {
        fn_80251AEC(0);
    }
    field_0x3fb = 2;
    fn_802685D4();
}

void Player_804A6F38::fn_8027CB24() {
    if (field_0x1244 != 0xf) {
        mPos.z = 0.0f;
    }
    field_0x436 = 0;
}

// FIXME: 78.55. Same instructions; in the original the float store and the li 8 / lis come
// before the argument setup (lfs, stfs, li, lis, stw, mr, addi), ours hoists lis/li/mr above the stfs.
void Player_804A6F38::fn_8027CAA8() {
    field_0x458 = 0;
    field_0x436 = 1;
    if (viewLayerFlag(field_0x3b4)) {
        field_0x344.y = 0.0f;
        field_0xbf4 = 8;
        fn_8024F0E8(0x1000b);
    }
}

// Update of state 0x07 (a fall / knock-up): gravity 0.25 on 0x344.z (down to -8); on flagged
// layers the push moves x / -y at height 0.5 and ends outside the view box less 16 (or with
// cls_804A71A8 0x4A), else mPos moves by the push and lands at 0. Pose by the frame 0x3FA;
// from frame 15 the landing (fn_8022C76C / tile 2 test on flagged layers) to state 8 or 0.
// FIXME: 99.88. Registers only: f0 / f1 swapped for mPos.x / mPos.y in the inlined
// containsPoint test.
void Player_804A6F38::fn_8027C6C4() {
    field_0x344.z -= 0.25f;
    if (field_0x344.z < -8.0f) {
        field_0x344.z = -8.0f;
    }
    if (viewLayerFlag(field_0x3b4)) {
        mPos.x += field_0x344.x;
        mPos.y -= field_0x344.z;
        mPos.z = 0.5f;
        JGeometry::TBox2f box;
        box = *layerBox(field_0x3b4);
        growBox(box, -16.0f);
        if (field_0x344.z < 0.0f && (field_0xbf0->field_0x48[2] || !containsPoint(box, mPos))) {
            mPos.z = 0.0f;
        }
    } else {
        mPos.add(field_0x344);
        if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
            mPos.z = 0.0f;
        }
    }
    if (field_0x3fa < 6) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 7) {
        field_0x458 = 1;
        field_0x3fb = 1;
    } else if (field_0x3fa < 8) {
        field_0x3fb = 2;
    } else if (field_0x3fa < 9) {
        field_0x3fb = 3;
    } else if (field_0x3fa < 11) {
        field_0x3fb = 4;
    } else {
        field_0x3fb = 5;
    }
    if (field_0x3fa >= 15) {
        if (viewLayerFlag(field_0x3b4) && fn_8022C76C(field_0x4) != 0.0f) {
            JGeometry::TVec3f pos;
            fn_8024B298(2, &pos);
            pos.y += 2.0f;
            if (fn_8022461C(&pos, field_0x3b4) != 2) {
                landState7(this);
            }
        }
        if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
            landState7(this);
        }
    } else {
        field_0x3fa++;
    }
    fn_80276E5C();
    fn_802685D4();
}

void Player_804A6F38::fn_8027C6B8() {
    field_0x436 = 0;
}

// Enter of state 0x08 (fn_8027BE74 is its update): 0xCAD / 0x403 start once,
// the push 0x344 is cleared, and the emitter 0x26F (below 0x30 frames of 0x403) or, at life 2,
// 0x271 / 0x273 (else 0x270 / 0x272) by facing pair is started at 0x12A4.
void Player_804A6F38::fn_8027C4FC() {
    field_0xbf4 = 2;
    field_0x458 = 1;
    field_0xcae = 0;
    field_0xcaf = 0;
    if (!field_0xcad) {
        field_0xcad = true;
        field_0x403 = 0;
    }
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    if (field_0x403 < 0x30) {
        if (field_0x12b0 == NULL) {
            field_0x12b0 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x26f, 0, 1, NULL, NULL, true, 1.0f);
        }
    } else if (field_0x12b4 == NULL) {
        if (fn_80241544() == 2) {
            if (field_0x3dc == 0 || field_0x3dc == 1) {
                field_0x12b4 = spawnEmitter_8013CC50(field_0x3b4, &field_0x12a4, 0x271, 0, -1, NULL, NULL, true, 1.0f);
            } else {
                field_0x12b4 = spawnEmitter_8013CC50(field_0x3b4, &field_0x12a4, 0x273, 0, -1, NULL, NULL, true, 1.0f);
            }
        } else if (field_0x3dc == 0 || field_0x3dc == 1) {
            field_0x12b4 = spawnEmitter_8013CC50(field_0x3b4, &field_0x12a4, 0x270, 0, -1, NULL, NULL, true, 1.0f);
        } else {
            field_0x12b4 = spawnEmitter_8013CC50(field_0x3b4, &field_0x12a4, 0x272, 0, -1, NULL, NULL, true, 1.0f);
        }
    }
    field_0x404 = 0;
    field_0x40c = 0;
}

// Exit of state 0x08 (from the state switch in fn_802870C8): stops its emitters and the
// 'PEFF' actor 0x12C0; with 0xCAE it calls fn_802356B8 for the rest of the group.
void Player_804A6F38::fn_8027BC90() {
    if (field_0x1274 != NULL) {
        field_0x1274->clearStatus(JPAEMIT_Immortal);
        field_0x1274->becomeInvalidEmitter();
        field_0x1274 = NULL;
    }
    if (field_0x128c != NULL) {
        field_0x128c->clearStatus(JPAEMIT_Immortal);
        field_0x128c->becomeInvalidEmitter();
        field_0x128c = NULL;
    }
    if (field_0x12b0 != NULL) {
        field_0x12b0->becomeInvalidEmitter();
        field_0x12b0 = NULL;
    }
    if (field_0x12b4 != NULL) {
        field_0x12b4->becomeInvalidEmitter();
        field_0x12b4 = NULL;
    }
    if (field_0x12b8 != NULL) {
        field_0x12b8->becomeInvalidEmitter();
        field_0x12b8 = NULL;
    }
    s32 serial = field_0x12c0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x12c0 = -1;
    }
    if (field_0x1244 != 0xf) {
        mPos.z = 0.0f;
    }
    if (field_0xcae == 1) {
        for (s32 i = 0; i < 4; i++) {
            if (i != field_0x4 && fn_8022E438(field_0x4, i)) {
                fn_802356B8(i);
            }
        }
    }
    if (field_0xcae == 1 || field_0xcaf == 1 || (field_0x1244 != 0x38 && field_0x1244 != 0x37)) {
        field_0xcad = false;
        field_0x403 = 0;
    }
    field_0xcae = 0;
    field_0xcaf = 0;
    field_0x40c = 0;
}

// Enter of state 0x09 (fn_80275D9C requests it): 0xBF4 by the reason 0x11AC, three sounds,
// the input history cleared, the spin centre 0x11B4 (setSpinCenter), the emitter 0x26A.
void Player_804A6F38::fn_8027BA2C() {
    switch (field_0x11ac) {
    case 1:
        field_0xbf4 = 8;
        break;
    case 2:
        field_0xbf4 = 6;
        break;
    case 3:
        field_0xbf4 = 4;
        break;
    case 4:
        field_0xbf4 = 8;
        break;
    default:
        field_0xbf4 = 4;
        break;
    }
    field_0x458 = 1;
    fn_8027EB04();
    fn_8024F0E8(0x10001);
    fn_8024F0E8(0x20001);
    fn_8024F0E8(0x2000e);
    fn_8027023C();
    setSpinCenter(this);
    field_0x11c0 = 0;
    if (field_0x12bc == NULL) {
        field_0x12bc = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x26a, 0, 1, NULL, NULL, true, 1.0f);
    }
}

// Update of state 0x09 (the spin): fn_80274988 steps the animation; in group mode 3 (without
// fn_8023434C) frames 11-34 walk toward fn_8027FA08's spot at 6 per frame. At frame 10, with
// no 'SWBM' actor yet (0xB38), one is spawned in front when the life is full (or with
// fn_80241284 and 0x11AC == 1 at 2+ life); the emitter 0x12BC circles mPos at 20 by 0x11C0.
// FIXME: 96.22. The inlined lifeFull result is materialised (li r0, 0 / 1; clrlwi.) where the
// original branches straight from its two compares into the spawn block (bge / blt), and with
// the extra live value the saved registers are r29-r31 shifted by one (this in r30, not r31).
void Player_804A6F38::fn_8027B648() {
    fn_80274988();
    if (fn_802486A0() == 3 && !fn_8023434C(field_0x4) && field_0x3fa >= 11 && field_0x3fa < 35) {
        JGeometry::TVec3f to;
        fn_8027FA08(&to);
        JGeometry::TVec3f dir = to - mPos;
        dir.z = 0.0f;
        if (magnitude(dir) >= 6.0f) {
            dir.normalize();
            dir.scale(6.0f);
            mPos += dir;
        } else {
            mPos.set(to);
        }
    }
    gBgRewrite_8053AC98->fn_802A20F0(&field_0x30c, field_0x4, field_0x3b4, 0, 1);
    fn_80268B3C();
    if (field_0x3fa == 10) {
        s32 facing = field_0x3dc;
        if (field_0xb38 < 0 && ((fn_80241284() && field_0x11ac == 1 && fn_80241544() >= 2) || lifeFull(this))) {
            JGeometry::TVec3f pos;
            pos.set(mPos);
            pos.x += lbl_8046BD88[facing][0];
            pos.y += lbl_8046BD88[facing][1];
            if (fn_80241284() && field_0x11ac == 1) {
                field_0xb38 = getActorManager_801F856C()->spawnActor_801F9484('SWBM', field_0x3b4, &pos, ((facing & 3) << 4) | 0x100 | (field_0x4 & 3), -1, field_0x4);
            } else {
                field_0xb38 = getActorManager_801F856C()->spawnActor_801F9484('SWBM', field_0x3b4, &pos, ((facing & 3) << 4) | (field_0x4 & 3), -1, field_0x4);
            }
            if (field_0xb38 >= 0) {
                field_0xb6c = field_0x3b4;
            }
        }
    }
    if (field_0x12bc != NULL) {
        JGeometry::TVec3f pos = mPos;
        pos.x += 20.0f * JMath::sincosTable_.cosShort(field_0x11c0);
        pos.y += 20.0f * JMath::sincosTable_.sinShort(field_0x11c0);
        setEmitterPos_8013CB44(field_0x12bc, &pos, 0, 1, true);
    }
}

void Player_804A6F38::fn_8027B5F0() {
    field_0x11c0 = 0;
    field_0x11ac = 0;
    deleteEmitter(&field_0x12bc);
}

// Enter of state 0x0A (update fn_8027AAD0): sounds 0x10001 / 0x20028, the input history
// cleared, 0x11B0 = 0, the emitters 0x26A and 0x266-0x269 (by player index), the spin centre.
void Player_804A6F38::fn_8027B304() {
    field_0xbf4 = 6;
    field_0x458 = 1;
    fn_8024F0E8(0x10001);
    fn_8024F0E8(0x20028);
    fn_8027EB04();
    fn_8027023C();
    field_0x11b0 = 0;
    if (field_0x12bc == NULL) {
        field_0x12bc = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x26a, 0, 1, NULL, NULL, true, 1.0f);
    }
    if (field_0x12b8 == NULL) {
        switch (field_0x4) {
        case 0:
            field_0x12b8 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x267, 0, -1, NULL, NULL, true, 1.0f);
            break;
        case 1:
            field_0x12b8 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x269, 0, -1, NULL, NULL, true, 1.0f);
            break;
        case 2:
            field_0x12b8 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x266, 0, -1, NULL, NULL, true, 1.0f);
            break;
        case 3:
            field_0x12b8 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x268, 0, -1, NULL, NULL, true, 1.0f);
            break;
        }
    }
    setSpinCenter(this);
    field_0x11c0 = 0;
}

void Player_804A6F38::fn_8027AA10() {
    deleteEmitter(&field_0x12b8);
    deleteEmitter(&field_0x12bc);
    deleteEmitter(&field_0x1374);
    deleteEmitter(&field_0x1378);
    field_0x11c0 = 0;
}

void Player_804A6F38::fn_8027A908() {
    field_0xbf4 = 8;
    field_0x458 = 0;
    field_0xbb9 = 0;
    field_0xbba = 0;
    field_0xbbc = 3.0f;
    mPos.z = 0.0f;
    fn_8027EB04();
    fn_8027023C();
    if (field_0x490 == 1) {
        fn_8024F0E8(0x40075);
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x503, 1, 1, NULL, NULL, true, 1.0f);
    } else {
        field_0x3fd = 1;
    }
}

void Player_804A6F38::fn_8027A178() {
    if (field_0x1374 != NULL) {
        field_0x1374->becomeInvalidEmitter();
        field_0x1374 = NULL;
    }
    if (field_0x1378 != NULL) {
        field_0x1378->becomeInvalidEmitter();
        field_0x1378 = NULL;
    }
    field_0x11c0 = 0;
    field_0xbb9 = 0;
    field_0xbd4 = 0;
    field_0xbba = 0;
}

// Enter of state 0x0C: the emitter 0x565 (callback field_0x130c), a 'PEFF' actor and the
// sound 0x2002A.
void Player_804A6F38::fn_8027A0D4() {
    field_0x458 = 0;
    if (field_0x1308 == NULL) {
        field_0x1308 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x565, 0, -1, NULL, &field_0x130c, true, 0.5f);
    }
    getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x3b4, &mPos, (field_0x4 & 3) | 0x10, -1, -1);
    fn_8024F0E8(0x2002a);
}

// Update of state 0x0C: back to state 0 after 120 frames (GameRoot 0x7F) or 90, the pose
// 0x3FD cycles every 6 frames, and the emitter 0x1308's callback follows mPos raised by 16.
void Player_804A6F38::fn_80279FE8() {
    field_0x3fc++;
    if (gGameRoot_8053AAF8->field_0x7f) {
        if (field_0x3fc >= 120) {
            fn_80251AEC(0);
        }
    } else if (field_0x3fc >= 90) {
        fn_80251AEC(0);
    }
    field_0x3fd = (field_0x3fc / 6) % 4;
    fn_80268B3C();
    if (field_0x1308 != NULL) {
        field_0x1320.set(mPos.x, mPos.y, mPos.z + 16.0f);
        field_0x130c.field_0x4.set(field_0x1320);
    }
}

void Player_804A6F38::fn_80279F94() {
    deleteEmitter(&field_0x1308);
    field_0x464 = 60;
}

// By the last hit kind 0x11E4: kinds 7-9 and 0x1C start the emitters 0x135C (0x246-0x249) and
// 0x132C (0x242-0x245, callback 0x1330) by facing with the sounds 0x10014 / 0x2004C; kind 3
// plays 0x10016 / 0x2004D, the others 0x10004.
void Player_804A6F38::fn_80279CFC() {
    field_0x464 = 60;
    field_0x458 = 0;
    if (field_0x11e4 == 7 || field_0x11e4 == 8 || field_0x11e4 == 9 || field_0x11e4 == 0x1c) {
        if (field_0x135c == NULL) {
            switch (field_0x3dc) {
            case 0:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x248, 0, 0, NULL, NULL, true, 1.0f);
                break;
            case 1:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x247, 0, 0, NULL, NULL, true, 1.0f);
                break;
            case 2:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x246, 0, -1, NULL, NULL, true, 1.0f);
                break;
            case 3:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x249, 0, 0, NULL, NULL, true, 1.0f);
                break;
            }
        }
        if (field_0x132c == NULL) {
            switch (field_0x3dc) {
            case 0:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x244, 0, 0, NULL, &field_0x1330, true, 1.0f);
                break;
            case 1:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x243, 0, 0, NULL, &field_0x1330, true, 1.0f);
                break;
            case 2:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x242, 0, -1, NULL, &field_0x1330, true, 1.0f);
                break;
            case 3:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x245, 0, 0, NULL, &field_0x1330, true, 1.0f);
                break;
            }
        }
        fn_8024F0E8(0x10014);
        fn_8024F0E8(0x2004c);
    } else if (field_0x11e4 == 3) {
        fn_8024F0E8(0x10016);
        fn_8024F0E8(0x2004d);
    } else {
        fn_8024F0E8(0x10004);
    }
}

// By the last hit kind 0x11E4 (fn_802710C8): 4 / 7, 5 / 8, 6 / 9 each have their own reaction
// (tu_8023FBF0), for the acting player of a group or a lone one; then four emitters stop.
void Player_804A6F38::fn_802796B8() {
    if ((fn_802486A0() == 4 && fn_80240888() == fn_80248778()) || (fn_802486A0() != 4 && !fn_80240808())) {
        s32 kind = field_0x11e4;
        if (kind == 4 || kind == 7) {
            fn_8024E46C();
        } else if (kind == 5 || kind == 8) {
            fn_8024E3D8();
        } else if (kind == 6 || kind == 9) {
            fn_8024E344();
        }
    }
    if (field_0x132c != NULL) {
        field_0x132c->becomeInvalidEmitter();
        field_0x132c = NULL;
    }
    if (field_0x135c != NULL) {
        field_0x135c->becomeInvalidEmitter();
        field_0x135c = NULL;
    }
    if (field_0x136c != NULL) {
        field_0x136c->becomeInvalidEmitter();
        field_0x136c = NULL;
    }
    if (field_0x1370 != NULL) {
        field_0x1370->becomeInvalidEmitter();
        field_0x1370 = NULL;
    }
}

void Player_804A6F38::fn_8027966C() {
    field_0x464 = 60;
    field_0x458 = 0;
    fn_8024F0E8(0x10004);
    field_0x350.z = 0.0f;
}

void Player_804A6F38::fn_802791AC() {}

// Update of state 0x10 (thrown back): the push 0x350 with gravity 0.25 (down to -4) inside
// the view box less 16; leaving it, or blocked (cls_804A71A8 0x48), from frame 20 the player
// bounces back (0x344 = the push flattened, normalised, times -2, z 4; 0x3FC = 1) or goes down
// (state 0x33) with no life left. The bounce lands in state 0 with 0x344 handed on to 0x368.
// FIXME: 99.98. Code identical; the -2.0f literal is reloc-name until the earlier .sdata2
// users (fn_802765C0, fn_8027776C, fn_80277F98: 24, -24, -12, ...) are written.
void Player_804A6F38::fn_802791B0() {
    if (field_0x3fc == 0) {
        mPos.add(field_0x350);
        if (mPos.z < 0.0f) {
            mPos.z = 0.0f;
        }
        field_0x350.z -= 0.25f;
        if (field_0x350.z < -4.0f) {
            field_0x350.z = -4.0f;
        }
        field_0x11e9 = 1;
        JGeometry::TBox2f box;
        box = *layerBox(field_0x3b4);
        growBox(box, -16.0f);
        if (field_0xbf0->anyFlag48() || !contains(box, mPos) ||
            (fn_80240808() && fn_802486A0() != 4 && !fn_80234B24(fn_80240644()))) {
            if (field_0x3fa >= 20) {
                if (fn_8024189C() <= 0) {
                    fn_80251AEC(0x33);
                } else {
                    field_0x344.x = field_0x350.x;
                    field_0x344.y = field_0x350.y;
                    field_0x344.z = field_0x350.z;
                    field_0x344.z = 0.0f;
                    field_0x344.normalize();
                    field_0x344.scale(-2.0f);
                    field_0x344.z = 4.0f;
                    field_0x3fc++;
                }
            }
        }
    } else {
        mPos.add(field_0x344);
        if (mPos.z < 0.0f && field_0x344.z < 0.0f) {
            mPos.z = 0.0f;
            passOnPush(this);
            fn_80251AEC(0);
        } else if (field_0x344.z >= -4.0f) {
            field_0x344.z -= 0.25f;
            if (field_0x344.z < -4.0f) {
                field_0x344.z = -4.0f;
            }
        }
    }
    if (field_0x3fa < 20) {
        field_0x3fa++;
    }
    if (field_0x3fa < 10) {
        field_0x3fd = 0;
    } else if (field_0x3fa < 14) {
        field_0x3fd = 1;
    } else {
        field_0x3fd = 2;
    }
    fn_80268B3C();
}


void Player_804A6F38::fn_802790C0() {
    if (fn_8025F448() && !fn_8025F418()) {
        fn_8025FEDC();
    }
    fn_8025F698(false);
    if (field_0xc64 < 99999999) {
        field_0xc64++;
    }
    field_0x124d = 1;
    field_0x464 = 0;
    field_0x494 = 0;
    field_0x498 = 1;
    field_0x458 = 0;
    field_0xcfd = 0;
    field_0xd08 = -1;
    field_0xd0c = 0;
    field_0xc00 = 1;
    fn_8024F0E8(0x10005);
    fn_8024EFF8(10);
    if (!fn_80240808()) {
        fn_802382D0(field_0x4, 0);
    }
    mPos.z = 0.0f;
    field_0xba4 = 0;
}

void Player_804A6F38::fn_80277EBC() {
    deleteEmitter(&field_0x1308);
    if (fn_8025F418()) {
        fn_80231194(field_0xd70);
    }
    field_0x494 = 0;
    field_0x498 = 0;
    field_0xcfc = 0;
    field_0xd08 = -1;
    field_0xd0c = 0;
    field_0xd34 = 0;
    field_0x499 = 0;
    field_0xba4 = 0;
    s32 serial = field_0x49c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x49c = -1;
    }
    if (field_0x1244 != 0x34) {
        field_0xd2d = 0;
        field_0xc00 = 0;
    }
}

void Player_804A6F38::fn_80277E18() {
    field_0x124d = 1;
    field_0xd30 = 0;
    if (field_0xd35 == 0) {
        if (fn_80241544() <= 1) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3ae, 1, 1, NULL, NULL, true, 1.0f);
        } else {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3af, 1, 1, NULL, NULL, true, 1.0f);
        }
    }
}

// Tingle's effects for state 0x34: facing down (2), emitters 0x3B0 / 0x3B1 (fn_80241544() <= 1)
// or 0x3B2 / 0x3B3 and 0x3A7-0x3AB at the player, and a 'TNGL' actor.
static inline void startTingle(Player_804A6F38* player) {
    player->field_0x3dc = 2;
    player->field_0x3e8 = 2;
    if (player->fn_80241544() <= 1) {
        spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3b0, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3b1, 1, 1, NULL, NULL, true, 1.0f);
    } else {
        spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3b2, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3b3, 1, 1, NULL, NULL, true, 1.0f);
    }
    spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3a7, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3a8, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3a9, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3aa, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(player->field_0x3b4, &player->mPos, 0x3ab, 1, 1, NULL, NULL, true, 1.0f);
    getActorManager_801F856C()->spawnActor_801F9484('TNGL', player->field_0x3b4, &player->mPos, 1, -1, player->field_0x4);
}

// Frame 420 of state 0x34: with no life left, the heal (fn_80270DDC) up to 4 below the maximum.
static inline void tingleHeal(Player_804A6F38* player) {
    if (player->field_0xd30 == 420) {
        player->field_0xc00 = 0;
        if (player->fn_8024189C() <= 0) {
            player->fn_80270DDC(player->fn_802417CC() - 4, false);
        }
    }
}

// Update of state 0x34 (Tingle revives the player): from frame 60 his effects, at 420 the heal,
// at 480 back to state 0. Outside GameRoot 0x7E it waits for a 'TNGL' actor's 0x251 first.
// The second fn_80240808() call is in the original (an inline's own test, source unknown).
// FIXME: 99.75. One instruction missing: after the actor's null test the original has a second,
// dead `beq` to the function's end (cmplwi r3, 0 / beq / beq / lbz 0x251).
void Player_804A6F38::fn_8027776C() {
    if (!gGameRoot_8053AAF8->field_0x7e) {
        s32 serial = getActorManager_801F856C()->fn_801F7A08('TNGL');
        cls_804B0478* tingle = static_cast<cls_804B0478*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (tingle != NULL && !tingle->field_0x251) {
            return;
        }
        if (field_0xd30 < 60) {
            field_0xd30 = 60;
            startTingle(this);
        }
        tingleHeal(this);
        if (field_0xd30 >= 480) {
            field_0x124d = 0;
            fn_80251AEC(0);
        } else {
            field_0xd30++;
        }
    } else {
        if (field_0xd30 == 60) {
            field_0xd30++;
            startTingle(this);
        }
        tingleHeal(this);
        if (field_0xd30 >= 480) {
            if (!fn_80240808() || !(fn_80240808() && fn_80236B50(fn_80240644()))) {
                field_0x124d = 0;
                fn_80251AEC(0);
            }
        } else {
            field_0xd30++;
        }
    }
}

void Player_804A6F38::fn_8027774C() {
    field_0x464 = 0xb4;
    field_0xd2d = 0;
    field_0xd30 = 0;
    field_0xd35 = 0;
    field_0xc00 = 0;
}

void Player_804A6F38::fn_8027769C() {
    field_0x458 = 0;
    if (field_0x137c == NULL) {
        field_0x137c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x285, 0, -1, NULL, NULL, true, 1.0f);
    }
    if (field_0x1380 == NULL) {
        field_0x1380 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x286, 0, 3, NULL, NULL, true, 1.0f);
    }
    fn_8024F0E8(0x10013);
}

void Player_804A6F38::fn_8027761C() {
    if (field_0x3fc >= 0x18) {
        field_0x3fd = 3;
        fn_80251AEC(0xf);
    } else {
        field_0x3fd = (field_0x3fc >> 1) % 3;
    }
    field_0x3fc++;
    fn_80268B3C();
}

void Player_804A6F38::fn_802775C8() {
    if (field_0x137c != NULL) {
        field_0x137c->becomeInvalidEmitter();
        field_0x137c = NULL;
    }
    if (field_0x1380 != NULL) {
        field_0x1380->becomeInvalidEmitter();
        field_0x1380 = NULL;
    }
}

void Player_804A6F38::fn_80277580() {
    if (field_0x1248 != 4) {
        field_0x45c = 0;
        field_0x3fa = 7;
    } else if (field_0x439 >= 7) {
        field_0x3fa = 7;
    } else {
        field_0x3fa = field_0x439;
    }
    field_0x458 = 0;
}

// Update of state 0x36: moves by 0x350 (x / y), walks at 1.5 on the ground, back to state 0
// after 20 frames; the pose 0x3FB by the sub-mode 0x45C as in fn_802756E4, and on the ground
// the step emitter 0x3D4 on tile 0x39 (else 0x3D3) follows mPos.
void Player_804A6F38::fn_80277220() {
    field_0x350.z = 0.0f;
    mPos.add(field_0x350);
    if (mPos.z <= 0.0f) {
        fn_802808E8(1.5f);
    }
    field_0x3fc++;
    if (field_0x3fc >= 20) {
        fn_80251AEC(0);
    }
    fn_80268B3C();
    switch (field_0x45c) {
    case 0:
        if (field_0x3fa < 4) {
            field_0x3fb = field_0x3fa;
        } else {
            field_0x3fb = field_0x3fa - 1;
        }
        if (field_0x3fb >= 7) {
            field_0x3fb = 6;
        }
        break;
    case 1:
        if (field_0x3fa < 4) {
            field_0x3fb = field_0x3fa;
        } else {
            field_0x3fb = field_0x3fa - 1;
        }
        if (field_0x3fb >= 8) {
            field_0x3fb = 7;
        }
        break;
    case 2:
        if (field_0x3fa < 1) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 5) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 6) {
            field_0x3fb = 2;
        } else {
            field_0x3fb = 3;
        }
        break;
    case 3:
        if (field_0x3fa < 2) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 4) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 13) {
            field_0x3fb = 2;
        } else {
            field_0x3fb = 3;
        }
        break;
    case 4:
        if (field_0x3fa < 6) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 7) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 8) {
            field_0x3fb = 2;
        } else if (field_0x3fa < 9) {
            field_0x3fb = 3;
        } else if (field_0x3fa < 11) {
            field_0x3fb = 4;
        } else {
            field_0x3fb = 5;
        }
        break;
    }
    field_0x3fa++;
    if (mPos.z <= 0.0f) {
        if (fn_8022461C(&mPos, field_0x3b4) == 0x39) {
            if (field_0x1370 == NULL) {
                field_0x1370 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3d4, 0, -1, &field_0x13a8, NULL, true, 1.0f);
            } else {
                setEmitterPos_8013CB44(field_0x1370, &mPos, 0, -1, true);
            }
            if (field_0x136c != NULL) {
                field_0x136c->becomeInvalidEmitter();
                field_0x136c = NULL;
            }
        } else {
            if (field_0x136c == NULL) {
                field_0x136c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3d3, 0, -1, &field_0x1398, NULL, true, 1.0f);
            } else {
                setEmitterPos_8013CB44(field_0x136c, &mPos, 0, -1, true);
            }
            if (field_0x1370 != NULL) {
                field_0x1370->becomeInvalidEmitter();
                field_0x1370 = NULL;
            }
        }
    }
}

void Player_804A6F38::fn_802771C8() {
    field_0xd5c = 0;
    if (field_0x136c != NULL) {
        field_0x136c->becomeInvalidEmitter();
        field_0x136c = NULL;
    }
    if (field_0x1370 != NULL) {
        field_0x1370->becomeInvalidEmitter();
        field_0x1370 = NULL;
    }
}

void Player_804A6F38::fn_802771BC() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80277104() {
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 6) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 10) {
        field_0x3fb = 2;
    } else {
        field_0x3fb = 3;
    }
    if (field_0x3fa >= 14) {
        if (field_0xcad) {
            fn_80251AEC(8);
        } else {
            fn_80251AEC(0);
        }
    } else {
        field_0x3fa++;
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_80277100() {}

// The boxes of the facing 0x3DC.
void Player_804A6F38::fn_80276E5C() {
    switch (field_0x3dc) {
    case 0:
        setBoxesPosX(this);
        break;
    case 1:
        setBoxesNegX(this);
        break;
    case 2:
        setBoxesPosY(this);
        break;
    case 3:
        setBoxesNegY(this);
        break;
    }
}

// The boxes of the opposite of the facing 0x3DC.
void Player_804A6F38::fn_80276BB8() {
    switch (field_0x3dc) {
    case 0:
        setBoxesNegX(this);
        break;
    case 1:
        setBoxesPosX(this);
        break;
    case 2:
        setBoxesNegY(this);
        break;
    case 3:
        setBoxesPosY(this);
        break;
    }
}

// The facing boxes (as fn_80276E5C) for the s16 angle 0x11C0 in eight directions. On a
// diagonal the facing 0x3F0 becomes the vertical one: the original's `!= a || != b` test
// (compiled as such: cmpwi a / bne, cmpwi b / beqlr) is always true.
void Player_804A6F38::fn_802765C0() {
    s16 angle = field_0x11c0;
    if (angle < -0x7000) {
        setBoxesNegX(this);
    } else if (angle < -0x5000) {
        field_0x4b0.set(mPos.x - 24.0f, mPos.y - 24.0f, mPos.x - 8.0f, mPos.y - 8.0f);
        field_0x4a0.set(mPos.x - 16.0f, mPos.y - 16.0f, mPos.x, mPos.y);
        field_0x30c.set(-16.0f, -16.0f, 0.0f);
        field_0x30c.add(mPos);
        if (field_0x3f0 != 3 || field_0x3f0 != 1) {
            field_0x3f0 = 3;
        }
    } else if (angle < -0x3000) {
        setBoxesNegY(this);
    } else if (angle < -0x1000) {
        field_0x4b0.set(mPos.x + 8.0f, mPos.y - 24.0f, mPos.x + 24.0f, mPos.y - 8.0f);
        field_0x4a0.set(mPos.x, mPos.y - 16.0f, mPos.x + 16.0f, mPos.y);
        field_0x30c.set(16.0f, -16.0f, 0.0f);
        field_0x30c.add(mPos);
        if (field_0x3f0 != 3 || field_0x3f0 != 0) {
            field_0x3f0 = 3;
        }
    } else if (angle < 0x1000) {
        setBoxesPosX(this);
    } else if (angle < 0x3000) {
        field_0x4b0.set(mPos.x + 8.0f, mPos.y + 8.0f, mPos.x + 24.0f, mPos.y + 24.0f);
        field_0x4a0.set(mPos.x, mPos.y, mPos.x + 16.0f, mPos.y + 16.0f);
        field_0x30c.set(16.0f, 16.0f, 0.0f);
        field_0x30c.add(mPos);
        if (field_0x3f0 != 2 || field_0x3f0 != 0) {
            field_0x3f0 = 2;
        }
    } else if (angle < 0x5000) {
        setBoxesPosY(this);
    } else if (angle < 0x7000) {
        field_0x4b0.set(mPos.x - 24.0f, mPos.y + 8.0f, mPos.x - 8.0f, mPos.y + 24.0f);
        field_0x4a0.set(mPos.x - 16.0f, mPos.y, mPos.x, mPos.y + 16.0f);
        field_0x30c.set(-16.0f, 16.0f, 0.0f);
        field_0x30c.add(mPos);
        if (field_0x3f0 != 2 || field_0x3f0 != 1) {
            field_0x3f0 = 2;
        }
    } else {
        setBoxesNegX(this);
    }
}

// FIXME: 99.56. Code identical; the 7 / -15 / 15 literals are reloc-name until the earlier
// .sdata2 users (fn_8027439C, fn_802756E4, fn_80275D9C) are written.
void Player_804A6F38::fn_80276538() {
    field_0x4c0.set(mPos.x - 7.0f, mPos.y - 7.0f, mPos.x + 7.0f, mPos.y + 7.0f);
    field_0x4d0.set(-15.0f, -15.0f, 15.0f, 15.0f);
    field_0x4d0.i.x += mPos.x;
    field_0x4d0.i.y += mPos.y;
    field_0x4d0.f.x += mPos.x;
    field_0x4d0.f.y += mPos.y;
}

// FIXME: 99.77. Code identical; the 15 literal is reloc-name until the earlier .sdata2 users
// (fn_8027439C, fn_802756E4, fn_80275D9C: 0.2, 20, 12, -16, 16, -4) are written.
void Player_804A6F38::fn_80276078() {
    field_0xb98 = 0;
    if (!fn_802413F0()) {
        return;
    }
    if (field_0x468 > 0) {
        field_0x5ec.set(mPos.x - 15.0f, mPos.y - 15.0f, mPos.x + 15.0f, mPos.y - 15.0f);
        field_0x3ec = -1;
        field_0xb98 = 1;
    } else if (field_0x1240 == 0 || field_0x1240 == 2 || field_0x1240 == 3) {
        setFrontBox(this, field_0x3dc);
    } else if (fn_80241284() || fn_80241230()) {
        field_0x5ec.set(mPos.x - 15.0f, mPos.y - 15.0f, mPos.x + 15.0f, mPos.y - 15.0f);
        field_0x3ec = -1;
        field_0xb98 = 1;
    } else if (fn_80241338()) {
        switch (field_0x3dc) {
        case 0:
            setFrontBox(this, 3);
            break;
        case 1:
            setFrontBox(this, 3);
            break;
        case 2:
            setFrontBox(this, 1);
            break;
        case 3:
            setFrontBox(this, 0);
            break;
        }
    } else if (fn_802412E0()) {
        switch (field_0x3dc) {
        case 0:
            setFrontBox(this, 1);
            break;
        case 1:
            setFrontBox(this, 0);
            break;
        case 2:
            setFrontBox(this, 3);
            break;
        case 3:
            setFrontBox(this, 2);
            break;
        }
    }
}

// The attack input check of the move updates (true ends them early, tu_8027E5BC): with
// fn_802466B4 pressed, all four directions held -> state 9 (0x11AC = 2); three or more recent
// presses in the history 0x117C[4..7] / 0x119C -> state 9 (3); 0x11D4 running -> state 9 (4);
// else with cls_804A71A8 0xB4 set state 7 (pushed by 1.5 along the facing, z -4) under
// fn_80243234, otherwise state 5; without it state 4.
// FIXME: 98.61. Registers only in the inlined recentPresses: the original keeps the count in
// r4 and the row result in r3 (ours swap them) and sets up the base pointer r6 after the
// held test instead of before it.
bool Player_804A6F38::fn_80275D9C() {
    if (field_0xc9c > 0) {
        return false;
    }
    if (fn_80240808() && fn_802486A0() == 5) {
        return false;
    }
    if (fn_80240808() && fn_8023EA28(fn_80240644()) && fn_802486A0() != 4) {
        return false;
    }
    if (fn_802DEBF8()) {
        return false;
    }
    if (fn_80241544() <= 0) {
        return false;
    }
    if (fn_802466B4()) {
        if (allDirectionsHeld(this)) {
            if (fn_80251AEC(9)) {
                field_0x11ac = 2;
                return true;
            }
        } else {
            if (recentPresses(this) >= 3) {
                if (fn_80251AEC(9)) {
                    field_0x11ac = 3;
                    return true;
                }
            } else if (field_0x11d4 > 0) {
                if (fn_80251AEC(9)) {
                    field_0x11ac = 4;
                    return true;
                }
            } else if (field_0xbf0->field_0xb4) {
                if (fn_80243234()) {
                    if (fn_80251AEC(7)) {
                        field_0x344.x = lbl_8046BCA8.push[field_0x3dc][0];
                        field_0x344.y = lbl_8046BCA8.push[field_0x3dc][1];
                        field_0x344.z = -4.0f;
                        return true;
                    }
                } else if (fn_80251AEC(5)) {
                    return true;
                }
            } else if (fn_80251AEC(4)) {
                return true;
            }
        }
    }
    return false;
}

// Pose step of state 0x0E's sub-mode 0x45C (from fn_8027D5F0): the frame counter 0x3FA
// picks the pose 0x3FB; at the sub-mode's last frame the state returns to 8 (fn_80246464)
// or 0. Sub-modes 0 / 1 also turn the pose into an angle by the facing 0x3DC and put the
// point 0x30C 16 away from mPos in that (8-way) direction.
void Player_804A6F38::fn_802756E4() {
    s32 angle = 0;
    switch (field_0x45c) {
    case 0:
        if (field_0x3fa < 4) {
            field_0x3fb = field_0x3fa;
        } else {
            field_0x3fb = field_0x3fa - 1;
        }
        if (field_0x3fb >= 7) {
            field_0x3fb = 6;
        }
        if (field_0x3fa >= 7) {
            if (fn_80246464()) {
                fn_80251AEC(8);
            } else {
                fn_80251AEC(0);
            }
        } else {
            field_0x3fa++;
        }
        switch (field_0x3dc) {
        case 0:
            angle = (s16)(field_0x3fb << 12) - 0x4000;
            break;
        case 1:
            angle = (s16)(-field_0x3fb << 12) - 0x4000;
            break;
        case 2:
            angle = (s16)(-field_0x3fb << 12) - 0x8000;
            break;
        case 3:
            angle = (s16)(-field_0x3fb << 12);
            break;
        }
        break;
    case 1:
        if (field_0x3fa < 4) {
            field_0x3fb = field_0x3fa;
        } else {
            field_0x3fb = field_0x3fa - 1;
        }
        if (field_0x3fb >= 8) {
            field_0x3fb = 7;
        }
        if (field_0x3fa >= 8) {
            if (fn_80246464()) {
                fn_80251AEC(8);
            } else {
                fn_80251AEC(0);
            }
        } else {
            field_0x3fa++;
        }
        switch (field_0x3dc) {
        case 0:
            angle = (s16)(-field_0x3fb << 12) + 0x4000;
            break;
        case 1:
            angle = (s16)(field_0x3fb << 12) + 0x4000;
            break;
        case 2:
            angle = (s16)(field_0x3fb << 12);
            break;
        case 3:
            angle = (s16)(field_0x3fb << 12) - 0x8000;
            break;
        }
        break;
    case 2:
        if (field_0x3fa < 1) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 5) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 6) {
            field_0x3fb = 2;
        } else {
            field_0x3fb = 3;
        }
        if (field_0x3fa >= 7) {
            if (fn_80246464()) {
                fn_80251AEC(8);
            } else {
                fn_80251AEC(0);
            }
        } else {
            field_0x3fa++;
        }
        fn_80276E5C();
        break;
    case 3:
        if (field_0x3fa < 2) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 4) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 13) {
            field_0x3fb = 2;
        } else {
            field_0x3fb = 3;
        }
        if (field_0x3fa >= 15) {
            if (fn_80246464()) {
                fn_80251AEC(8);
            } else {
                fn_80251AEC(0);
            }
        } else {
            field_0x3fa++;
        }
        fn_80276E5C();
        break;
    case 4:
        if (field_0x3fa < 6) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 7) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 8) {
            field_0x3fb = 2;
        } else if (field_0x3fa < 9) {
            field_0x3fb = 3;
        } else if (field_0x3fa < 11) {
            field_0x3fb = 4;
        } else {
            field_0x3fb = 5;
        }
        if (field_0x3fa >= 30) {
            if (fn_80246464()) {
                fn_80251AEC(8);
            } else {
                fn_80251AEC(0);
            }
        } else {
            field_0x3fa++;
        }
        fn_80276E5C();
        break;
    }
    switch (field_0x45c) {
    case 0:
    case 1:
        fn_80276E5C();
        s16 dir = angle; // the angle is compared as an s16 (extsh at the compare only)
        if (dir <= -0x7000) {
            field_0x30c.set(-16.0f, 0.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= -0x5000) {
            field_0x30c.set(-16.0f, -16.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= -0x3000) {
            field_0x30c.set(0.0f, -16.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= -0x1000) {
            field_0x30c.set(16.0f, -16.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= 0x1000) {
            field_0x30c.set(16.0f, 0.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= 0x3000) {
            field_0x30c.set(16.0f, 16.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= 0x5000) {
            field_0x30c.set(0.0f, 16.0f, 0.0f);
            field_0x30c.add(mPos);
        } else if (dir <= 0x7000) {
            field_0x30c.set(-16.0f, 16.0f, 0.0f);
            field_0x30c.add(mPos);
        } else {
            field_0x30c.set(-16.0f, 0.0f, 0.0f);
            field_0x30c.add(mPos);
        }
        break;
    case 2:
    case 3:
    case 4:
        fn_80276E5C();
        break;
    }
}

void Player_804A6F38::fn_80274988() {
    field_0x3fa++;
    field_0x11c0 = 0;
    if (fn_802486A0() == 3 && !fn_8023434C(field_0x4)) {
        if (field_0x3fa < 3) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 7) {
            field_0x3fb = 8;
            field_0x11c0 = -0x7000;
        } else if (field_0x3fa < 8) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 9) {
            field_0x3fb = 10;
            field_0x11c0 = -0x4000;
        } else if (field_0x3fa < 10) {
            field_0x3fb = 0;
            field_0x11c0 = -0x2000;
        } else if (field_0x3fa < 31) {
            field_0x3fb = ((field_0x3fa - 10) / 3 + 1) & 7;
            field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
        } else if (field_0x3fa < 34) {
            field_0x3fb = 11;
            field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
        } else {
            field_0x3fb = 11;
            gBgRewrite_8053AC98->fn_802A20F0(&mPos, field_0x4, field_0x3b4, 0, 1);
            fn_80251AEC(0);
        }
    } else {
        if (field_0x3fa < 3) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 7) {
            field_0x3fb = 8;
            field_0x11c0 = -0x7000;
        } else if (field_0x3fa < 8) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 9) {
            field_0x3fb = 10;
            field_0x11c0 = -0x4000;
        } else if (field_0x3fa < 10) {
            field_0x3fb = 0;
            field_0x11c0 = -0x2000;
        } else if (field_0x3fa < 24) {
            field_0x3fb = ((field_0x3fa - 10) / 2 + 1) & 7;
            field_0x11c0 = (field_0x3fa - 10) * 0x1000;
        } else if (field_0x3fa < 26) {
            field_0x3fb = 11;
            field_0x11c0 = (field_0x3fa - 10) * 0x1000;
        } else {
            field_0x3fb = 11;
            gBgRewrite_8053AC98->fn_802A20F0(&mPos, field_0x4, field_0x3b4, 0, 1);
            fn_80251AEC(0);
        }
    }
    switch (field_0x3dc) {
    case 0:
        break;
    case 1:
        field_0x11c0 += 0x8000;
        break;
    case 2:
        field_0x11c0 += 0x4000;
        break;
    case 3:
        field_0x11c0 -= 0x4000;
        break;
    }
    if (viewLayerFlag(field_0x3b4)) {
        field_0x11c0 = -field_0x11c0;
    }
    fn_802765C0();
}

void Player_804A6F38::fn_80274728() {
    if (fn_802486A0() == 3 && !fn_8023434C(field_0x4)) {
        field_0x3fa++;
    } else if (field_0x3fa >= 14 && field_0x3fa < 37) {
        field_0x3fa += 2;
        if (field_0x3fa > 37) {
            field_0x3fa = 37;
        }
    } else {
        field_0x3fa++;
    }
    field_0x11c0 = 0;
    if (field_0x3fa < 6) {
        field_0x3fb = 8;
    } else if (field_0x3fa < 8) {
        field_0x3fb = 9;
    } else if (field_0x3fa < 9) {
        field_0x3fb = 10;
    } else if (field_0x3fa < 10) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 31) {
        field_0x3fb = ((field_0x3fa - 10) / 3 + 1) & 7;
        field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
    } else if (field_0x3fa < 33) {
        field_0x3fb = ((field_0x3fa - 10) / 3 + 1) & 7;
        field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
    } else {
        gBgRewrite_8053AC98->fn_802A20F0(&mPos, field_0x4, field_0x3b4, 0, 1);
        field_0x11b0++;
        s32 limit;
        if (fn_802486A0() == 3 && !fn_8023434C(field_0x4)) {
            limit = 6;
        } else {
            limit = 12;
        }
        if (field_0x11b0 < limit) {
            field_0x3fa = 10;
            field_0x3fb = 0;
        } else {
            field_0x3fb = 11;
            fn_80251AEC(12);
        }
        field_0x11c0 = 0;
    }
    switch (field_0x3dc) {
    case 0:
        break;
    case 1:
        field_0x11c0 += 0x8000;
        break;
    case 2:
        field_0x11c0 += 0x4000;
        break;
    case 3:
        field_0x11c0 -= 0x4000;
        break;
    }
    fn_802765C0();
}

// Pose step of state 0x0B (fn_8027A1E0): the frame counter 0x3FA picks the pose 0x3FB,
// in turn 0x3FD's two fixed sequences (0: eight poses of 7 frames, 2: six) end in 0x3FD = 1;
// otherwise a 19-frame loop, twice as fast (0xBBA) while 0xBC0 > 0 or pad 0x200 is held
// (unless fn_8022AF5C); past pose 0 it slows 0xBBC by 0.2 down to 2 (reset to 4 / 3).
// Then the boxes 0x4B0 (+-20) and 0x4A0 (+-12) round mPos, 0x30C = mPos, 0x3F0 = -1.
void Player_804A6F38::fn_8027439C() {
    if (field_0x3fd == 0) {
        field_0x3fa++;
        if (field_0x3fa <= 7) {
            field_0x3fb = 0;
        } else if (field_0x3fa <= 14) {
            field_0x3fb = 1;
        } else if (field_0x3fa <= 21) {
            field_0x3fb = 2;
        } else if (field_0x3fa <= 28) {
            field_0x3fb = 3;
        } else if (field_0x3fa <= 35) {
            field_0x3fb = 0;
        } else if (field_0x3fa <= 42) {
            field_0x3fb = 1;
        } else if (field_0x3fa <= 49) {
            field_0x3fb = 2;
        } else if (field_0x3fa <= 56) {
            field_0x3fb = 3;
        } else {
            field_0x3fd = 1;
            field_0x3fa = 0;
            field_0x3fb = 0;
            if (field_0x490 == 1) {
                lbl_8053AED0->fn_8043D67C(0x190, field_0x4, 0);
                field_0x490 = 0;
            }
        }
    } else if (field_0x3fd == 2) {
        field_0x3fa++;
        if (field_0x3fa <= 7) {
            field_0x3fb = 0;
        } else if (field_0x3fa <= 14) {
            field_0x3fb = 1;
        } else if (field_0x3fa <= 21) {
            field_0x3fb = 2;
        } else if (field_0x3fa <= 28) {
            field_0x3fb = 3;
        } else if (field_0x3fa <= 35) {
            field_0x3fb = 0;
        } else if (field_0x3fa <= 42) {
            field_0x3fb = 1;
        } else {
            field_0x3fd = 1;
            field_0x3fa = 0;
            field_0x3fb = 0;
        }
    } else {
        if (field_0xbc0 > 0 || (fn_80246D1C(0x200) != 0 && !fn_8022AF5C())) {
            field_0xbba = 1;
        }
        if (field_0xbba == 1) {
            field_0x3fa += 2;
        } else {
            field_0x3fa++;
        }
        if (field_0x3fa >= 19) {
            field_0x3fb = 0;
            field_0x3fa = 0;
            if (field_0xbba == 1) {
                field_0xbbc = 4.0f;
            } else {
                field_0xbbc = 3.0f;
            }
            field_0xbba = 0;
        }
        if (field_0x3fa < 7) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 12) {
            field_0x3fb = 1;
        } else {
            field_0x3fb = 2;
        }
        if (field_0x3fb >= 1) {
            field_0xbbc -= 0.2f;
            if (field_0xbbc < 2.0f) {
                field_0xbbc = 2.0f;
            }
        }
        if (mPos.z > 0.0f) {
            if (field_0x344.z > 1.0f) {
                field_0x3fb = 2;
            } else {
                field_0x3fb = 0;
            }
        }
    }
    field_0x4b0.set(mPos.x - 20.0f, mPos.y - 20.0f, mPos.x + 20.0f, mPos.y + 20.0f);
    field_0x4a0.set(mPos.x - 12.0f, mPos.y - 12.0f, mPos.x + 12.0f, mPos.y + 12.0f);
    field_0x30c.x = 0.0f;
    field_0x30c.y = 0.0f;
    field_0x30c.z = 0.0f;
    field_0x30c.add(mPos);
    field_0x3f0 = -1;
}

u8 Player_804A6F38::fn_80274238(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag) {
    field_0x492 = 0;
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f viewBox(*box);
    toView(viewBox, layer);
    bool hit = fn_802738F0(&viewBox, pos, flag, true);
    if (hit == true) {
        pos->add(layerBox(layer)->i);
    }
    if (!hit) {
        hit = fn_80273FB8(box, layer, pos);
    }
    return hit;
}

// Not auto-inlined: fn_80274238 calls fn_80273FB8 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// fn_802730D8 with a height of 8 (inlined), while fn_80247BD4() is running.
// FIXME: 99.62. Only the return differs: the original returns `clrlwi r3, r27, 24` (a u8 return
// type; 100 % as u8), but fn_80274238 then converts the u8 (neg / or / srwi) where its original
// takes r3 as is, so both stay bool until that caller's form is known.
bool Player_804A6F38::fn_80273FB8(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    field_0x492 = 0;
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    bool hit = false;
    if (fn_80247BD4() > 0) {
        hit = fn_802730D8(box, layer, false, 8.0f);
        if (hit == true) {
            out->x = mPos.x;
            out->y = mPos.y;
        }
        field_0x492 = 1;
    }
    if (hit == true) {
        out->add(layerBox(layer)->i);
    }
    return hit;
}
#pragma auto_inline reset

u8 Player_804A6F38::fn_80273E7C(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f viewBox(*box);
    toView(viewBox, layer);
    bool hit = fn_802738F0(&viewBox, out, flag, false);
    if (hit == true) {
        out->add(layerBox(layer)->i);
    }
    return hit;
}

bool Player_804A6F38::fn_80273D44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (fn_80274238(box, layer, out, flag)) {
        return true;
    }
    if (fn_802576CC(box, layer, out)) {
        return true;
    }
    if (fn_802575B8(box, layer, out)) {
        return true;
    }
    if (fn_802574CC(box, layer, out)) {
        return true;
    }
    if (fn_802573E0(box, layer, out)) {
        return true;
    }
    if (fn_802570BC(box, layer, out)) {
        return true;
    }
    return fn_80256F44(box, layer, out);
}

bool Player_804A6F38::fn_80273C30(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (fn_80274238(box, layer, out, flag)) {
        return true;
    }
    if (fn_802576CC(box, layer, out)) {
        return true;
    }
    if (fn_802575B8(box, layer, out)) {
        return true;
    }
    if (fn_802574CC(box, layer, out)) {
        return true;
    }
    if (fn_802570BC(box, layer, out)) {
        return true;
    }
    return fn_80256F44(box, layer, out);
}

bool Player_804A6F38::fn_802738F0(const JGeometry::TBox2f* box, JGeometry::TVec2f* out, bool flag, bool center) {
    if (!fn_802413F0()) {
        return false;
    }
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (field_0x458 == 1) {
        JGeometry::TBox2f a(field_0x4a0);
        toView(a, field_0x3b4);
        JGeometry::TBox2f b(field_0x4b0);
        toView(b, field_0x3b4);
        if (overlaps(a, *box)) {
            out->x = (a.i.x + a.f.x + b.i.x + b.f.x) / 4;
            out->y = (a.i.y + a.f.y + b.i.y + b.f.y) / 4;
            return true;
        }
        if (flag && overlaps(b, *box)) {
            if (center) {
                out->x = (a.i.x + a.f.x + b.i.x + b.f.x) / 4;
                out->y = (a.i.y + a.f.y + b.i.y + b.f.y) / 4;
                return true;
            }
            out->x = b.f.x + 0.5f * (b.i.x - b.f.x);
            out->y = b.f.y + 0.5f * (b.i.y - b.f.y);
            return true;
        }
    }
    return false;
}

// FIXME: 99.82. Registers only (r29 / r30 swapped for the actor and the view layer in the
// actor branch).
bool Player_804A6F38::fn_80273518(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (!fn_802413F0()) {
        return false;
    }
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f viewBox(*box);
    toView(viewBox, layer);
    if (!(layer == field_0x3b4 || (field_0x3b4 == 8 && layer < 8))) {
        return false;
    }
    if (field_0x459) {
        JGeometry::TBox2f carried;
        if (fn_8025F590()) {
            carried.set(-8.0f, -8.0f, 8.0f, 8.0f);
            toView(carried, field_0x3b4);
            JGeometry::TVec2f pos;
            pos.x = field_0x2f4.x;
            pos.y = field_0x2f4.y;
            carried.addPos(pos);
            if (overlaps(carried, viewBox)) {
                out->x = carried.f.x + 0.5f * (carried.i.x - carried.f.x);
                out->y = carried.f.y + 0.5f * (carried.i.y - carried.f.y);
                return true;
            }
        } else {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != NULL) {
                carried.set(actor->mBox);
                toView(carried, field_0x3b4);
                JGeometry::TVec2f pos;
                pos.x = field_0x2f4.x;
                pos.y = field_0x2f4.y;
                carried.addPos(pos);
                if (overlaps(carried, viewBox)) {
                    out->x = carried.f.x + 0.5f * (carried.i.x - carried.f.x);
                    out->y = carried.f.y + 0.5f * (carried.i.y - carried.f.y);
                    actor->vfunc_0x48();
                    return true;
                }
            }
        }
    }
    return false;
}

bool Player_804A6F38::fn_802732C4(const JGeometry::TBox2f* box, s32 layer) {
    if (fn_80270C88()) {
        return false;
    }
    if (field_0x42c) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (fn_80270A8C()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!viewLayerFlag(field_0x3b4) && mPos.z > 0.0f) {
        return false;
    }
    if (field_0xcee) {
        return false;
    }
    if (field_0x1216) {
        return false;
    }
    if (field_0x1215) {
        return false;
    }
    if (field_0xcf0) {
        return false;
    }
    if (field_0x1238 > 0) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    return overlaps(field_0x4c0, *box);
}

bool Player_804A6F38::fn_802730D8(const JGeometry::TBox2f* box, s32 layer, bool flag, f32 height) {
    if (layer != field_0x3b4) {
        return false;
    }
    if (!flag && fn_80270A8C()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!viewLayerFlag(field_0x3b4) && mPos.z > height) {
        return false;
    }
    if (field_0xcee) {
        return false;
    }
    if (field_0x1216) {
        return false;
    }
    if (field_0x1215) {
        return false;
    }
    if (field_0xcf0) {
        return false;
    }
    if (field_0x1238 > 0) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    return overlaps(field_0x4c0, *box);
}

bool Player_804A6F38::fn_80272E98(const JGeometry::TBox2f* box, s32 layer) {
    if (layer != field_0x3b4) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (!fn_80245938()) {
        return false;
    }
    if (!fn_8025F2CC()) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (!fn_80251BFC(0x11)) {
        return false;
    }
    return overlapsInView(this, *box, layer);
}

bool Player_804A6F38::fn_80272DCC(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (fn_80249068()) {
        return false;
    }
    for (int i = 0; i < 0x50; i++) {
        if (field_0xdb0[i] < 0) {
            field_0xdb0[i] = serial;
            field_0x1030[i] = layer;
            field_0x5fc[i].set(*box);
            return true;
        }
    }
    return false;
}

// FIXME: 97.24. Two extra instructions: the original leaves the point loop straight for the
// slot store (no `i >= 8` test after it; with the store inside the loop, or the points in an
// inline, the layout differs more).
// fn_80272DCC's slot store (without its fn_80249068 test), unless one of the eight points fn_8024B298 gives is inside
// `box` grown by 2.5 on every side.
bool Player_804A6F38::fn_80272C20(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_8024BBB0()) {
        return false;
    }
    if (field_0xcee == 1) {
        return false;
    }
    JGeometry::TBox2f grown;
    grown.i.x = box->i.x - 2.5f;
    grown.i.y = box->i.y - 2.5f;
    grown.f.x = box->f.x + 2.5f;
    grown.f.y = box->f.y + 2.5f;
    s32 i;
    for (i = 0; i < 8; i++) {
        JGeometry::TVec3f p;
        fn_8024B298(i, &p);
        if (!containsPoint(grown, p)) {
            break;
        }
    }
    if (i >= 8) {
        return false;
    }
    for (int j = 0; j < 0x50; j++) {
        if (field_0xdb0[j] < 0) {
            field_0xdb0[j] = serial;
            field_0x1030[j] = layer;
            field_0x5fc[j].set(*box);
            return true;
        }
    }
    return false;
}

bool Player_804A6F38::fn_80272B4C(const JGeometry::TVec3f* pos) {
    for (s32 i = 0; i < 0x50; i++) {
        if (field_0xdb0[i] >= 0 && fn_802413F0() && field_0x1030[i] == field_0x3b4 && contains(field_0x5fc[i], *pos)) {
            return true;
        }
    }
    return false;
}

// FIXME: 81.51. The original unrolls the 72 entries as 8 loop trips of 9 compares, bumping
// the base register by 4 per entry; ours unrolls 9 trips of 8 with fixed offsets.
bool Player_804A6F38::fn_802729F0(s32 serial) {
    for (s32 i = 0; i < 72; i++) {
        if (field_0xef0[i] >= 0 && field_0xef0[i] == serial) {
            return true;
        }
    }
    return false;
}

// fn_80272768 with the point 16 in front, fn_8024FF6C / fn_8024FEFC and field_0xda4.
bool Player_804A6F38::fn_802728AC(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (fn_8024FF6C() >= 0 && serial != fn_8024FF6C()) {
        return false;
    }
    if (!fn_8024FEFC()) {
        return false;
    }
    JGeometry::TVec2f p(mPos.x, mPos.y);
    p.x += lbl_8046BD48[field_0x3dc][0];
    p.y += lbl_8046BD48[field_0x3dc][1];
    if (!containsPoint(*box, p)) {
        return false;
    }
    if (field_0xda4 < 0) {
        field_0xda4 = serial;
    }
    return true;
}

// Whether the point 12 in front of the player is in `box` (on `layer`, serial matching
// fn_8024FF84 when that is set, fn_80250D30); then remembers serial in field_0xd94.
bool Player_804A6F38::fn_80272768(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (fn_8024FF84() >= 0 && serial != fn_8024FF84()) {
        return false;
    }
    if (!fn_80250D30()) {
        return false;
    }
    JGeometry::TVec2f p(mPos.x, mPos.y);
    p.x += lbl_8046BD68[field_0x3dc][0];
    p.y += lbl_8046BD68[field_0x3dc][1];
    if (!containsPoint(*box, p)) {
        return false;
    }
    if (field_0xd94 < 0) {
        field_0xd94 = serial;
    }
    return true;
}

bool Player_804A6F38::fn_802724E0(const JGeometry::TBox2f* box, s32 layer) {
    if (!fn_802413F0()) {
        return false;
    }
    if (!(layer == field_0x3b4 || (field_0x3b4 < 8 && layer == 8))) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (!(fn_80270A8C() && fn_80270A04()) && !fn_80251BFC(0x19)) {
        return false;
    }
    return overlapsInView(this, *box, layer);
}

// Hit test of `box` against the 8x16 box in front of the player (field_0x5ec, fn_80276078);
// `dir` must face the player's direction field_0x3ec. On a hit: effects 0x117 / 0x118 at
// the front box's centre and sound 0x20003 (when asked and field_0x3f4 has run out),
// then field_0x3f4 = 5.
bool Player_804A6F38::fn_80272224(const JGeometry::TBox2f* box, s32 layer, s32 dir, bool effect, bool sound) {
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (!field_0xb98) {
        return false;
    }
    if (fn_80240808() && fn_802486A0() == 5) {
        return false;
    }
    if (fn_80241200()) {
        return false;
    }
    if (dir < 0) {
        return false;
    }
    if (field_0x3ec >= 0 && dir >= 0) {
        switch (field_0x3ec) {
        case 0:
            if (dir != 1) {
                return false;
            }
            break;
        case 1:
            if (dir != 0) {
                return false;
            }
            break;
        case 2:
            if (dir != 3) {
                return false;
            }
            break;
        case 3:
            if (dir != 2) {
                return false;
            }
            break;
        }
    }
    JGeometry::TBox2f front(field_0x5ec);
    if (front.intersect(*box)) {
        if (effect == true && field_0x3f4 <= 0) {
            JGeometry::TVec3f pos;
            pos.x = (field_0x5ec.i.x - field_0x5ec.f.x) * 0.5f + field_0x5ec.f.x;
            pos.y = (field_0x5ec.i.y - field_0x5ec.f.y) * 0.5f + field_0x5ec.f.y;
            pos.z = mPos.z;
            spawnEmitter_8013CC50(field_0x3b4, &pos, 0x117, 1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &pos, 0x118, 1, 1, NULL, NULL, true, 1.0f);
        }
        if (sound == true && field_0x3f4 <= 0) {
            fn_8024F0E8(0x20003);
        }
        field_0x3f4 = 5;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8027221C(s32 value) {
    field_0xbf4 = value;
}

s32 Player_804A6F38::fn_802721A8() {
    field_0xcae = 1;
    if (field_0x492 == 1) {
        return 4;
    }
    if (field_0xbf4 <= 0) {
        return 0;
    }
    if (fn_80241544() >= 2) {
        return field_0xbf4 + 2;
    }
    return field_0xbf4;
}

s32 Player_804A6F38::fn_8027218C() {
    if (field_0x1240 != 4) {
        return 0;
    }
    return field_0x45c;
}

s32 Player_804A6F38::fn_80272184() {
    return 4;
}

bool Player_804A6F38::fn_80272160(s32 damage, const JGeometry::TVec2f* pos, s32 arg) {
    return fn_80238548(field_0x4, damage, pos, arg);
}

// FIXME: 98.54. Structurally done but for the knockback direction: the original also stores
// the TVec2f dir (mPos copy, then the difference) to its stack slot, four dead stfs ours drops;
// in both hitAngle copies it loads mPos.x before mPos.y. The rest is register allocation
// (damage r30 / kind r28 / pos r27 there, r27 / r29 / r28 here; the scale constants in f29,
// here f31).
bool Player_804A6F38::fn_802710C8(s32 attacker, s32 damage, const JGeometry::TVec2f* pos, s32 kind) {
    if (fn_80240B5C()) {
        damage = 0;
    }
    if (fn_80241498()) {
        if (damage <= 0) {
            damage = 0;
        } else if (damage == 1) {
            damage = 1;
        } else {
            damage = damage / 2;
        }
    }
    if (fn_80270A8C()) {
        return false;
    }
    if (damage >= 4 && !gGameRoot_8053AAF8->field_0x7f && saveRowFlag() == 1) {
        fn_8024E500();
    }
    if (kind == 0x1b && gGameRoot_8053AAF8->field_0x7f && lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 2) {
        return false;
    }
    if (kind != 0x11 && !(kind == 0x12 || kind == 0x13)) {
        if (kind == 0xd || kind == 0xe || kind == 0xf) {
            if (fn_80240670()) {
                return false;
            }
        } else if (fn_80270C88()) {
            return false;
        }
    }
    if (kind == 0x16 && fn_80240B70()) {
        return false;
    }
    if (kind == 0xa && !fn_80251BFC(0x35)) {
        return false;
    }
    field_0x11e4 = kind;
    if (kind == 0x12) {
        field_0x499 = 1;
    }
    if (kind == 0x12 || kind == 0x13) {
        field_0xbfc = 0;
    } else if (attacker == field_0x4) {
        if (fn_80240808()) {
            fn_80236468(fn_80240644(), damage);
        } else {
            fn_80236468(field_0x4, damage);
        }
    }
    if (fn_8024189C() <= 0) {
        fn_80251AEC(0x33);
        return true;
    }
    if (damage > 0) {
        field_0x488 = 4;
    }
    if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && damage >= 4 && fn_8024189C() > 0) {
        lbl_8053AED0->fn_8043D67C(0x18f, field_0x4, 0);
    }
    if (damage >= 4) {
        gGameRoot_8053AAF8->fn_80134DE4(2, 0);
    } else if (damage >= 1) {
        gGameRoot_8053AAF8->fn_80134DE4(1, 0);
    }
    if (kind == 0x16) {
        if (fn_80240B5C()) {
            if (field_0x484 == 0) {
                field_0x484 = 1;
                field_0x480 = 60;
                JGeometry::TVec3f effectPos;
                effectPos.set(mPos);
                effectPos.y += 6.0f;
                spawnEmitter_8013CC50(field_0x3b4, &effectPos, 0x28d, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x3b4, &effectPos, 0x28e, 0, 1, NULL, NULL, true, 1.0f);
                fn_8024F0E8(0x2003d);
                return true;
            }
            return false;
        }
        if (damage > 0) {
            field_0x464 = 2;
        }
        if (fn_80251AEC(0x50)) {
            s16 angle = hitAngle(this, pos);
            if (angle < -0x6000) {
                field_0x3dc = 1;
                field_0x3e8 = 1;
            } else if (angle < -0x2000) {
                field_0x3dc = 3;
                field_0x3e8 = 3;
            } else if (angle < 0x2000) {
                field_0x3dc = 0;
                field_0x3e8 = 0;
            } else if (angle < 0x6000) {
                field_0x3dc = 2;
                field_0x3e8 = 2;
            } else {
                field_0x3dc = 1;
                field_0x3e8 = 1;
            }
            return true;
        }
        return false;
    }
    if (kind == 0x18) {
        if (fn_80251AEC(0x51)) {
            s16 angle = hitAngle(this, pos);
            if (viewLayerFlag(field_0x3b4)) {
                if (angle < -0x4000) {
                    field_0x3dc = 1;
                    field_0x3e8 = 1;
                } else if (angle < 0x4000) {
                    field_0x3dc = 0;
                    field_0x3e8 = 0;
                } else {
                    field_0x3dc = 1;
                    field_0x3e8 = 1;
                }
            } else if (angle < -0x6000) {
                field_0x3dc = 1;
                field_0x3e8 = 1;
            } else if (angle < -0x2000) {
                field_0x3dc = 3;
                field_0x3e8 = 3;
            } else if (angle < 0x2000) {
                field_0x3dc = 0;
                field_0x3e8 = 0;
            } else if (angle < 0x6000) {
                field_0x3dc = 2;
                field_0x3e8 = 2;
            } else {
                field_0x3dc = 1;
                field_0x3e8 = 1;
            }
            return true;
        }
        return false;
    }
    if (fn_80240B5C() && field_0x46c > 0) {
        field_0x46c -= 30;
        if (field_0x46c < 0) {
            field_0x46c = 0;
        }
        fn_8024E3D8();
        field_0x464 = 60;
        return true;
    }
    if (kind == 0x11 || kind == 0) {
        field_0x464 = 180;
        return true;
    }

    JGeometry::TVec2f dir(mPos.x, mPos.y);
    dir.x -= pos->x;
    dir.y -= pos->y;
    if (dir.squared() <= JGeometry::TUtilf::epsilon()) {
        dir.set(0.0f, 1.0f);
    } else {
        dir.normalize();
    }
    JGeometry::TVec3f knockback(dir.x, dir.y, 0.0f);
    if (kind == 1 || kind == 0x19 || kind == 0x1c) {
        knockback.scale(1.5f);
        if (damage > 4) {
            knockback.z = 4.0f;
        } else {
            knockback.z = 3.0f;
        }
    } else if (kind == 2 || kind == 0x17) {
        knockback.scale(3.0f);
        if (damage > 4) {
            knockback.z = 4.0f;
        } else {
            knockback.z = 3.0f;
        }
    } else if (kind == 3) {
        knockback.scale(6.0f);
        if (damage > 4) {
            knockback.z = 4.0f;
        } else {
            knockback.z = 3.0f;
        }
    } else if (kind == 4 || kind == 7) {
        knockback.scale(0.75f);
        knockback.z = 4.0f;
    } else if (kind == 5 || kind == 8) {
        knockback.scale(1.0f);
        knockback.z = 4.0f;
    } else if (kind == 6 || kind == 9 || kind == 0x13) {
        knockback.scale(1.5f);
        knockback.z = 4.0f;
    } else if (kind == 0x14 || kind == 0x15) {
        knockback.zero();
    } else if (kind == 0x1b) {
        knockback.scale(6.0f);
    } else if (kind == 0xb || kind == 0xc) {
        knockback.zero();
    } else if (kind == 0x1a) {
        knockback.scale(0.5f);
        knockback.z = 2.0f;
    }

    bool requested;
    if (kind == 0xa) {
        field_0x464 = 60;
        requested = fn_80251AEC(0x35);
    } else if (kind == 0xd) {
        if (field_0x35c.length() < 1.4f) {
            field_0x35c.set(mPos - JGeometry::TVec3f(pos->x, pos->y, 0.0f));
            field_0x35c.z = 0.0f;
            field_0x35c.normalize();
            field_0x35c.scale(1.4f);
        }
        if (field_0x1240 == 8) {
            fn_80251AEC(0x37);
        } else if (field_0x1240 != 2) {
            fn_80251AEC(2);
        }
    } else if (kind == 0xe) {
        if (field_0x35c.length() < 2.0f) {
            field_0x35c.set(mPos - JGeometry::TVec3f(pos->x, pos->y, 0.0f));
            field_0x35c.z = 0.0f;
            field_0x35c.normalize();
            field_0x35c.scale(2.0f);
        }
        if (field_0x1240 == 8) {
            fn_80251AEC(0x37);
        } else if (field_0x1240 != 2) {
            fn_80251AEC(2);
        }
    } else if (kind == 0xf) {
        if (field_0x35c.length() < 4.0f) {
            field_0x35c.set(mPos - JGeometry::TVec3f(pos->x, pos->y, 0.0f));
            field_0x35c.z = 0.0f;
            field_0x35c.normalize();
            field_0x35c.scale(4.0f);
        }
        if (field_0x1240 == 8) {
            fn_80251AEC(0x37);
        } else if (field_0x1240 != 2) {
            fn_80251AEC(2);
        }
    } else if (kind == 0x10) {
        knockback.scale(0.25f);
        requested = fn_80251AEC(0x37);
    } else if (kind == 0x12) {
        field_0x464 = 0;
        requested = fn_80251AEC(0x33);
    } else if (kind == 0x14) {
        requested = fn_80251AEC(0x40);
        field_0x464 = 60;
        if (requested == true) {
            if (fn_8022B1F8() || fn_8022B244()) {
                field_0xc9c = 60;
            } else {
                field_0xc9c = 300;
            }
        }
    } else if (kind == 0x15) {
        requested = fn_80251AEC(0x40);
        field_0x464 = 60;
        if (requested == true) {
            if (fn_8022B1F8() || fn_8022B244()) {
                field_0xc9c = 60;
            } else {
                field_0xc9c = 600;
            }
        }
    } else if (kind == 0x1b) {
        if (field_0xc9c <= 0) {
            fn_8024E694(100, 1);
        }
        requested = fn_80251AEC(0x10);
        field_0x464 = 60;
        if (requested == true) {
            field_0xc9c = 300;
            field_0xc84.z = 0.5f;
        }
    } else if (kind == 0xb || kind == 0xc) {
        requested = fn_80251AEC(0xc);
    } else {
        field_0x464 = 60;
        requested = fn_80251AEC(0xf);
    }
    if (requested == true) {
        field_0x350.set(knockback);
    }
    if (kind == 1 || kind == 2 || kind == 3 || kind == 4 || kind == 5 || kind == 6 || kind == 7 || kind == 8 ||
        kind == 9 || kind == 0x13 || kind == 0x14 || kind == 0x15 || kind == 0x1a || kind == 0x1b || kind == 0x1c) {
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x24a, 1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x24b, 1, 1, NULL, NULL, true, 1.0f);
        s32 layer = field_0x3b4;
        if (layer >= 8) {
            getActorManager_801F856C()->spawnActor_801F9484('PEFF', layer, &mPos, (field_0x4 & 3) | 0x28, -1, field_0x4);
        }
    }
    return true;
}

bool Player_804A6F38::fn_80271098(const JGeometry::TVec3f* pos) {
    // fn_80238548 reads only pos.x and pos.y
    return fn_80238548(field_0x4, 0, (const JGeometry::TVec2f*)pos, 0xd);
}

bool Player_804A6F38::fn_80271068(const JGeometry::TVec2f* pos) {
    return fn_80238548(field_0x4, 0, pos, 0xe);
}

bool Player_804A6F38::fn_80271038(const JGeometry::TVec2f* pos) {
    return fn_80238548(field_0x4, 0, pos, 0xf);
}

void Player_804A6F38::fn_80270F00(s32 arg) {
    field_0xbf8 += arg;
    if (field_0xbf8 > 32) {
        field_0xbf8 = 32;
    }
    fn_80270DDC(field_0xbf8, false);
}

void Player_804A6F38::fn_80270DDC(s32 amount, bool flag) {
    if (field_0xd35) {
        return;
    }
    if (!gGameRoot_8053AAF8->field_0x7f && isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
        if (!(field_0xbfc > 0 && !fn_80270A8C())) {
            return;
        }
    }
    // Reproduces the original's overflow-trap shape on the life word (increment, compare, spin);
    // its source form is unexplained (docs/CRACK_PASS.md, Open questions).
    s32 life = field_0xbfc;
    field_0xbfc++;
    if (field_0xbfc == life) {
        for (;;) {
        }
    }
    field_0xbfc = life;
    if (fn_80240808()) {
        field_0xbfc = fn_802376D8(fn_80240644());
    } else {
        field_0xbfc += amount;
    }
    if (field_0xbfc > field_0xbf8) {
        field_0xbfc = field_0xbf8;
    }
    field_0xd35 = flag;
}

// The argument is subtracted from field_0xbfc.
void Player_804A6F38::fn_80270D34(s32 amount) {
    if (fn_80240808()) {
        field_0xbfc = fn_80237774(fn_80240644());
    } else {
        field_0xbfc -= amount;
    }
    if (field_0xbfc <= 0) {
        field_0xbfc = 0;
        fn_80251AEC(0x33);
    }
    if (amount > 0) {
        fn_802702EC();
    }
}

void Player_804A6F38::fn_80270D08() {
    if (field_0x464 > 0) {
        field_0x464--;
    }
    if (field_0x468 > 0) {
        field_0x468--;
    }
}

bool Player_804A6F38::fn_80270C88() {
    if (fn_80241200()) {
        return true;
    }
    if (fn_802486A0() == 4) {
        if (fn_80240888() != fn_80248778()) {
            return true;
        }
    }
    return fn_80270ABC();
}

// FIXME: 98.26. Only the first lbz of 0xce8 is hoisted above the prologue's stw r31 / mr in the
// original: the method is const (rules card), which needs fn_802412CC, fn_802411D8, fn_80240808,
// fn_80247BD4 (tu_8023FBF0) and fn_80270A58 / fn_80270A8C const too.
bool Player_804A6F38::fn_80270ABC() {
    if (field_0xce8 == 1) {
        return true;
    }
    if (field_0xcee == 1) {
        return true;
    }
    if (field_0x1216) {
        return true;
    }
    if (field_0x1215) {
        return true;
    }
    if (field_0xcf0 == 1) {
        return true;
    }
    if (fn_802412CC() == true) {
        return true;
    }
    if (fn_802411D8() == true) {
        return true;
    }
    if (fn_80270A8C()) {
        return true;
    }
    if (fn_80270A58()) {
        return true;
    }
    if (fn_80240808() && fn_802486A0() == 5) {
        return true;
    }
    if (field_0x1240 == 1) {
        return true;
    }
    if (field_0x1240 == 0x19 || field_0x1244 == 0x19) {
        return true;
    }
    if (fn_80247BD4() > 0) {
        return true;
    }
    if (field_0x468 > 0) {
        return true;
    }
    return field_0x464 > 0;
}

// State 0x33 (requested when the life 0xBFC reaches 0) requested or current, or 0x34.
bool Player_804A6F38::fn_80270A8C() {
    if (field_0x1244 == 0x33 || field_0x1240 == 0x33 || field_0x1240 == 0x34) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80270A58() {
    if (field_0xd2d || field_0x1244 == 0x34 || field_0x1240 == 0x34) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80270A04() {
    bool result = false;
    if (fn_80241864() <= 0 && field_0xcfc) {
        result = true;
    }
    return result;
}

void Player_804A6F38::fn_802709C8() {
    gBgRewrite_8053AC98->fn_802A20F0(&field_0x30c, field_0x4, field_0x3b4, 0, 1);
}

void Player_804A6F38::fn_80270948(const JGeometry::TVec3f* pos) {
    if (!fn_80270A8C()) {
        return;
    }
    if (field_0xd2c == 1) {
        return;
    }
    if (pos == NULL) {
        field_0xd10 = mPos;
    } else {
        field_0xd10 = *pos;
    }
    field_0xd2c = 1;
}

void Player_804A6F38::fn_802708E8() {
    field_0xd2d = 1;
    field_0xd30 = 0;
    field_0xcfc = 0;
    deleteEmitter(&field_0x1308);
}

void Player_804A6F38::fn_802708D0() {
    field_0xd2d = 1;
    field_0xd30 = 0;
    field_0xcfc = 0;
}

// Revive (update of state 0x33).
void Player_804A6F38::fn_8027057C() {
    if (!gGameRoot_8053AAF8->field_0x7f && isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7e && fn_80236890()) {
        return;
    }
    if (field_0xd2d == 1) {
        return;
    }
    if (field_0xd2c == 1) {
        field_0x124d = 0;
        if (!fn_80251AEC(0)) {
            return;
        }
        fn_80270DDC(fn_802417CC() - 4, false);
        field_0x464 = 180;
        field_0xd2c = 0;
        mPos = field_0xd10;
        field_0x3b4 = 0;
        field_0xcfc = 0;
        return;
    }
    if (field_0xcfd == 1 && mPos.z <= 0.0f && field_0x14.z > 0.0f && gGameRoot_8053AAF8->field_0x7e) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_80243194(i)) {
                fn_802363DC(i);
            }
        }
        return;
    }
    if (field_0xd35 == 1) {
        field_0x124d = 0;
        if (fn_80251AEC(0x34)) {
            field_0xcfc = 0;
            field_0xd34 = true;
        }
        return;
    }
    if (isGameMode7() && gGameRoot_8053AAF8->field_0x7f && gGameRoot_8053AAF8->field_0x7e) {
        if (fn_8025F448()) {
            return;
        }
        if (fn_80239D38()) {
            return;
        }
        if (field_0x494 > 0) {
            field_0x494--;
            if (field_0x494 == 0) {
                if (getActorManager_801F856C()->fn_801F7A08('TNGL') >= 0) {
                    field_0x494 = 1;
                } else {
                    field_0xcfc = 0;
                    for (s32 i = 0; i < 4; i++) {
                        if (fn_8022E438(field_0x4, i) && fn_80236B50(i) && !fn_80236AB4(i)) {
                            fn_80236350(i);
                        }
                    }
                }
            }
        }
    }
}

// Counts a defeated enemy by kind into field_0xc1c.
void Player_804A6F38::fn_80270360(s32 serial) {
    if (field_0xc50 < 9999999) {
        field_0xc50++;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        return;
    }
    s32 kind;
    switch (actor->mActorId) {
    case 'OKTA':
        kind = 0;
        break;
    case 'AMOS':
        kind = 1;
        break;
    case 'POOH':
        kind = 2;
        break;
    case 'BZBR':
        kind = 3;
        break;
    case 'DDRK':
        kind = 4;
        break;
    case 'DGTL':
        kind = 5;
        break;
    case 'HINO':
        kind = 6;
        break;
    case 'SLR1':
        kind = 7;
        break;
    case 'TKTA':
        kind = 8;
        break;
    case 'SLR2':
        kind = 7;
        break;
    case 'KETH':
        kind = 9;
        break;
    case 'BPOH':
        kind = 10;
        break;
    case 'MOZO':
        kind = 11;
        break;
    case 'Poon':
        kind = 12;
        break;
    default:
        kind = 13;
        break;
    }
    if (kind < 13 && field_0xc1c[kind] < 9999999) {
        field_0xc1c[kind]++;
    }
}

s32 Player_804A6F38::fn_80270358() {
    return field_0xc50;
}

s32 Player_804A6F38::fn_80270350() {
    return field_0xc64;
}

// The body never reads value (the forwarder keeps it live).
void Player_804A6F38::fn_80270318(s32 value) {
    field_0xc74++;
    if (field_0xc74 >= 9999) {
        field_0xc74 = 9999;
    }
    if (field_0xc70 < field_0xc74) {
        field_0xc70 = field_0xc74;
    }
}

s32 Player_804A6F38::fn_80270310() {
    return field_0xc70;
}

void Player_804A6F38::fn_802702EC() {
    field_0xc74 = 0;
    lbl_80529DEC.field_0x1f4[field_0x4] = 0;
}

s32 Player_804A6F38::fn_802702E4() {
    return field_0xc74;
}

void Player_804A6F38::fn_802702C4() {
    if (field_0xc78 < 99999999) {
        field_0xc78++;
    }
}

void Player_804A6F38::fn_802702A4() {
    if (field_0xc7c < 99999999) {
        field_0xc7c++;
    }
}

void Player_804A6F38::fn_8027023C() {
    for (s32 i = 0; i < 8; i++) {
        field_0x117c[i] = 0;
    }
    // The original walks the 4x4 byte table column by column.
    for (s32 i = 0; i < 4; i++) {
        for (s32 j = 0; j < 4; j++) {
            field_0x119c[j][i] = 0;
        }
    }
}

void Player_804A6F38::fn_80270088() {
    if (fn_80246F28() > 0.0f) {
        if (field_0x119c[0][0] == 0 && field_0x119c[0][1] == 0 && field_0x119c[0][2] == 0 &&
            field_0x119c[0][3] == 0) {
            field_0x117c[7] = 0;
            field_0x117c[6] = 0;
            field_0x117c[5] = 0;
            field_0x117c[4] = 0;
        }
    } else if (field_0x119c[0][0] == 1 || field_0x119c[0][1] == 1 || field_0x119c[0][2] == 1 ||
               field_0x119c[0][3] == 1) {
        field_0x117c[7] = 0;
        field_0x117c[6] = 0;
        field_0x117c[5] = 0;
        field_0x117c[4] = 0;
    }
    for (int i = 3; i > 0; i--) {
        field_0x117c[i + 4] = field_0x117c[i + 3];
    }
    field_0x117c[4] = 60;
    if (fn_80247330() > 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][0] = field_0x119c[i - 1][0];
        }
        field_0x119c[0][0] = 1;
    }
    if (fn_80247330() < 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][1] = field_0x119c[i - 1][1];
        }
        field_0x119c[0][1] = 1;
    }
    if (fn_8024712C() < 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][2] = field_0x119c[i - 1][2];
        }
        field_0x119c[0][2] = 1;
    }
    if (fn_8024712C() > 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][3] = field_0x119c[i - 1][3];
        }
        field_0x119c[0][3] = 1;
    }
}

void Player_804A6F38::fn_8026FD14() {
    if (fn_80246F28() > 0.0f) {
        if (field_0x11c8 >= 3) {
            field_0x11c4 = 3;
        } else if (field_0x11c4 > 0) {
            field_0x11c4--;
        }
        field_0x11c8 = 0;
    } else {
        if (field_0x11c8 < 3) {
            field_0x11c8++;
        }
        if (field_0x11c4 > 0) {
            field_0x11c4--;
        }
    }
    if (field_0x11d4 > 0) {
        field_0x11d4--;
    }
    if (field_0x11cc > 0) {
        field_0x11cc--;
    }

    if (fn_80247330() > 0.0f && fn_8024712C() == 0.0f) {
        field_0x117c[0] = 60;
        if (field_0x117c[2] < field_0x117c[1] && field_0x117c[3] < field_0x117c[1]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[0] > 0) {
        field_0x117c[0]--;
    }
    if (fn_80247330() < 0.0f && fn_8024712C() == 0.0f) {
        field_0x117c[1] = 60;
        if (field_0x117c[2] < field_0x117c[0] && field_0x117c[3] < field_0x117c[0]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[1] > 0) {
        field_0x117c[1]--;
    }
    if (fn_8024712C() < 0.0f && fn_80247330() == 0.0f) {
        field_0x117c[2] = 60;
        if (field_0x117c[0] < field_0x117c[3] && field_0x117c[1] < field_0x117c[3]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[2] > 0) {
        field_0x117c[2]--;
    }
    if (fn_8024712C() > 0.0f && fn_80247330() == 0.0f) {
        field_0x117c[3] = 60;
        if (field_0x117c[0] < field_0x117c[2] && field_0x117c[1] < field_0x117c[2]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[3] > 0) {
        field_0x117c[3]--;
    }

    if (fn_80247330() <= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][0] = 0;
        }
    }
    if (fn_80247330() >= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][1] = 0;
        }
    }
    if (fn_8024712C() >= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][2] = 0;
        }
    }
    if (fn_8024712C() <= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][3] = 0;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (field_0x117c[i + 4] > 0) {
            field_0x117c[i + 4]--;
        }
    }
}

bool Player_804A6F38::fn_8026FCE0() {
    bool result = false;
    if (field_0xb9a || field_0xb9c || field_0xb9d) {
        result = true;
    }
    return result;
}

JGeometry::TVec3f* Player_804A6F38::fn_8026FCC4() {
    if (field_0xb9a) {
        return &field_0x5c0;
    }
    return &mPos;
}

bool Player_804A6F38::fn_8026FCB0() {
    return field_0xc9c != 0;
}

void Player_804A6F38::fn_8026FC8C() {
    field_0xc9c = 0;
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
    field_0x468 = 10;
}

bool Player_804A6F38::fn_8026FC84() {
    return field_0xb9b;
}

s32 Player_804A6F38::fn_8026FC68() {
    if (field_0xb9c) {
        return field_0x3dc;
    }
    return -1;
}

s32 Player_804A6F38::fn_8026FC20() {
    if (fn_80240808()) {
        return fn_8023A1A4(fn_80240644());
    }
    return field_0xbac;
}
