#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"

BgRewrite_804A7C98::BgRewrite_804A7C98() {
    gBgRewrite_8053AC98 = this;
    field_0x4 = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

BgRewrite_804A7C98::~BgRewrite_804A7C98() {
    gBgRewrite_8053AC98 = NULL;
    delete field_0x4;
    field_0x4 = NULL;
}

void BgRewrite_804A7C98::fn_802A3D80() {
    field_0x8 = 10;
    field_0xc = 0;
    field_0x10 = 10;
    field_0x14 = 0;
    field_0x18 = 10;
    field_0x1c = 0;
}

// FIXME: 98.8. Only the clrlwi r6, r30, 16 on tile before fn_802F9974 is missing
// (ours passes the u16 parameter with mr); an s32 / s16 tile matches here but
// drops the u16-passing callers (tu_8012DAC0, act_shutter, act_burning_block).
void BgRewrite_804A7C98::fn_802A3948(const JGeometry::TVec3f* pos, s32 player, s32 layer, u16 tile) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer & 0xFE);
    s32 cell = field_0x20;
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
    s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, field_0x20);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
    }
}

// FIXME: 98.8. As fn_802A3948: only the clrlwi r6, r30, 16 on tile is missing.
void BgRewrite_804A7C98::fn_802A3884(const JGeometry::TVec3f* pos, s32 player, s32 layer, u16 tile) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer & 0xFE);
    s32 cell = field_0x20;
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
    s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, field_0x20);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, false);
    }
}

// FIXME: 97.7. As fn_802A3948: only the clrlwi r6, r31, 16 on tile is missing.
void BgRewrite_804A7C98::fn_802A381C(const JGeometry::TVec3f* pos, s32 layer, u16 tile) {
    JGeometry::TVec2f p(pos->x, pos->y);
    s32 cell = fn_80226BC4(&p, layer & 0xFE);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
}

bool BgRewrite_804A7C98::fn_802A1554(const JGeometry::TVec3f* pos) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, 0);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(0, field_0x20);
    u16 tile = field_0x24;
    if (lbl_8046CB88[tile] == 0) {
        return true;
    }
    if (tile == 0x75 || tile == 0x68 || tile == 0x69 || tile == 0x6A || tile == 0x6B || tile == 0x82 ||
        tile == 0x5F || tile == 0x70 || tile == 0x71 || tile == 0x72 || tile == 0x40 || tile == 0x41 ||
        tile == 0x42 || tile == 0x43 || tile == 0x44 || tile == 0xE5 || tile == 0x58) {
        return true;
    }
    s32 code = fn_8022461C(pos, 0);
    if (code == 0xE || code == 3 || (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0)) {
        return true;
    }
    return false;
}

bool BgRewrite_804A7C98::fn_802A1490(const JGeometry::TVec3f* pos) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, 0);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(0, field_0x20);
    if (lbl_8046CB88[field_0x24] == 0) {
        return true;
    }
    s32 code = fn_8022461C(pos, 0);
    if (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0) {
        return true;
    }
    return false;
}

bool BgRewrite_804A7C98::fn_802A01F0(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    bool result = false;
    switch (field_0x24) {
    case 0x16:
    case 0x17:
        result = fn_802A0800(player, layer);
        break;
    case 0x71:
        result = true;
        break;
    }
    return result;
}

// FIXME: 99.1. Register allocation only: the original keeps gBgRewrite_8053AC98 in
// r31 and the cell / slot in r30 (ours swaps them).
void BgRewrite_804A7C98::fn_8029E928(const JGeometry::TVec3f* pos, s32 layer, s32 tile) {
    gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, tile);
}

u8 BgRewrite_804A7C98::fn_8029E888(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
    return field_0x24 == 0x30;
}

// FIXME: 93.4. The original copies cell into r31 before the first write and steps
// that copy in place (addi r31, r31, 1); ours computes the right neighbour into
// r5 after the call (2 instructions fewer, registers r26-r31 shifted).
void BgRewrite_804A7C98::fn_8029DF80(s32 cell, s32 layer, s32 a, s32 b, s32 c, s32 d) {
    s32 index = cell;
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, index, a);
    index++;
    if ((index & 0xF) == 0) {
        index += 0xF0;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, index, b);
    index = cell + 0x10;
    if ((cell & 0x100) != (index & 0x100)) {
        index = cell + 0x110;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, index, c);
    s32 next = index + 1;
    if ((next & 0xF) == 0) {
        next += 0xF0;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, next, d);
}

// FIXME: 92.4. Register allocation and placement only: the original saves b in
// r30 and cell in r29 up front and copies layer into r31 inside each branch
// (mr r31, r4 twice); ours saves layer in r29 before the branch.
void BgRewrite_804A7C98::fn_8029DEBC(s32 layer, s32 cell, s32 vertical, s32 a, s32 b) {
    if (vertical == 0) {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
    } else {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        s32 below = cell + 0x10;
        if ((cell & 0x100) != (below & 0x100)) {
            below = cell + 0x110;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, below, b);
    }
}

void BgRewrite_804A7C98::fn_8029D898(s32 value) {
    s32 saved = getGameView_802D6A9C()->field_0x34;
    getGameView_802D6A9C()->fn_802D7970(value);
    gGameRoot_8053AAF8->fn_80138B7C();
    getGameView_802D6A9C()->fn_802D7970(saved);
}
