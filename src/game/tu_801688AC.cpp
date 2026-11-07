#include "game/GameAudio.h"
#include "game/act/tu_801688AC.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"

extern s32 lbl_804A0718[]; // per-variant angle (s16 units)

// Scans from the 8-pixel grid cell of pos along each axis for a wall tile
// (tile codes 8-15 with bit 0 set), at most 64 steps, and stores the four edges in out;
// layer 0 gives the view box.

// 0x80169DCC.
// FIXME(shotgun): 95.45 %, missing/extra + reloc-name
void cls_804A0738::init() {
    field_0x98 = (field_0x90 >> 7) & 7;
    field_0x25c = (field_0x90 >> 8) & 1;
    field_0x90 = field_0x90 & 3;
    field_0x80.i.x = -4.0f;
    field_0x80.i.y = -4.0f;
    field_0x80.f.x = 4.0f;
    field_0x80.f.y = 4.0f;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    for (s32 i = 0; i < 5; i++) {
        field_0x274[i].x = mPos.x;
        field_0x274[i].y = mPos.y;
        field_0x274[i].layer = field_0x4;
    }
    for (s32 i = 0; i < 5; i++) {
        field_0x2ec[i].x = getPlayerPos_8023E724(field_0x90)->x;
        field_0x2ec[i].y = getPlayerPos_8023E724(field_0x90)->y;
        field_0x2ec[i].layer = getPlayerLayer_8023DE58(field_0x90);
    }
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x250 = 0;
    field_0x364[2] = 0.0f;
    field_0x364[1] = 0.0f;
    field_0x364[0] = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    fn_8016890C(&mPos, field_0x4, &field_0x370);
    s32 angle = lbl_804A0718[field_0x98];
    JGeometry::TVec2f dir(1000.0f * JMASinShort(angle), 1000.0f * JMACosShort(angle));
    dir.normalize();
    if (field_0x25c) {
        field_0x3c.x = 9.0f * dir.x;
        field_0x3c.y = 9.0f * dir.y;
    } else {
        field_0x3c.x = 6.0f * dir.x;
        field_0x3c.y = 6.0f * dir.y;
    }
    field_0x94 = 0;
    field_0x23c = 4;
    if (field_0x98 > 3) {
        field_0x3c.x *= 1.2f;
        field_0x3c.y *= 1.2f;
    }
    field_0x254 = 0;
    field_0x260 = 0;
    field_0x261 = 0;
    field_0x258 = 300;
    field_0x180 = 60;
    field_0x184 = 80.0f;
    field_0x230 = 0;
}

// UPDATE_HERE

// 0x8016890C.
// FIXME(shotgun): 93.09 %, missing/extra + reloc-name
void cls_804A0738::fn_8016890C(const JGeometry::TVec3f* pos, s32 layer, JGeometry::TBox2f* out) {
    if (layer == 0) {
        GameView_804A9950* view = getGameView_802D6A9C();
        *out = (layer < 8) ? view->mVisibleBox : view->field_0xdc;
        return;
    }
    JGeometry::TVec3f p = *pos;
    u32 gx = (u32)p.x & ~7;
    u32 gy = (u32)p.y & ~7;
    p.x = (f32)gx;
    p.y = (f32)gy;
    p.z = 0.0f;
    for (s32 n = 0; n < 64; n++) {
        if (p.x >= 512.0f) {
            p.x = 512.0f;
            break;
        }
        u16 tile = fn_80226850(&p, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            break;
        }
        p.x += 8.0f;
    }
    out->f.x = p.x;
    p.x = (f32)gx;
    for (s32 n = 0; n < 64; n++) {
        if (p.x < 8.0f) {
            p.x = 0.0f;
            break;
        }
        u16 tile = fn_80226850(&p, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            p.x += 8.0f;
            break;
        }
        p.x -= 8.0f;
    }
    out->i.x = p.x;
    p.x = (f32)gx;
    p.y = (f32)gy;
    for (s32 n = 0; n < 64; n++) {
        if (p.y >= 512.0f) {
            p.y = 512.0f;
            break;
        }
        u16 tile = fn_80226850(&p, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            break;
        }
        p.y += 8.0f;
    }
    out->f.y = p.y;
    p.y = (f32)gy;
    for (s32 n = 0; n < 64; n++) {
        if (p.y < 8.0f) {
            p.y = 0.0f;
            break;
        }
        u16 tile = fn_80226850(&p, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            p.y += 8.0f;
            break;
        }
        p.y -= 8.0f;
    }
    out->i.y = p.y;
}

// 0x80168C38. Whether a player standing in the (grown) hit box gets caught; every other
// player is then pushed along.
// FIXME(shotgun): 70.14 %, missing/extra + reloc-name
bool cls_804A0738::fn_80168C38() {
    s32 found = -1;
    JGeometry::TBox2f box;
    box.i.x = mBox.i.x + mPos.x - 4.0f;
    box.i.y = mBox.i.y + mPos.y - 4.0f;
    box.f.x = mBox.f.x + mPos.x + 4.0f;
    box.f.y = mBox.f.y + mPos.y + 4.0f;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023EA28(i) != 1) {
            if (!fn_8022E438(i, field_0x90) ||
                (fn_8022E438(i, field_0x90) == 1 && i != field_0x90 && fn_80235580(i) == 1)) {
                if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                    found = i;
                }
            }
        }
    }
    if (found < 0) {
        return false;
    }
    if (fn_8022CAF8(found) != 4) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8022E438(i, found) == 1) {
                field_0x261 = 1;
                fn_8022E23C(i, mSerial);
                fn_8022CC20(i);
                if (fn_8022F514(i) == 1) {
                    JGeometry::TVec3f d = mPos;
                    d -= *getPlayerPos_8023E724(i);
                    fn_8023DFF4(i, &d);
                }
            }
        }
        return true;
    }
    field_0x261 = 1;
    return fn_8022E23C(found, mSerial);
}

// 0x80168E78.
// FIXME(shotgun): 95.86 %, missing/extra + reloc-name
void cls_804A0738::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x30 = 0x11;
    s32 frame = field_0x250 + 0x69;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, 0, 2, -1, false, 0, 0);
}

cls_804A0738::~cls_804A0738() {}
