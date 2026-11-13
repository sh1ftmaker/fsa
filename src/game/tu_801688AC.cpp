#include "game/GameAudio.h"
#include "game/act/tu_801688AC.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"

extern s32 lbl_804A0718[]; // per-variant angle (s16 units)

// Scans from the 8-pixel grid cell of pos along each axis for a wall tile
// (tile codes 8-15 with bit 0 set), at most 64 steps, and stores the four edges in out;
// layer 0 gives the view box.

// 0x80169DCC.
// FIXME(shotgun): 95.54 %, missing/extra + reloc-name; behaviour drift.
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
        field_0x274[i].x0 = mPos.x;
        field_0x274[i].y0 = mPos.y;
        field_0x274[i].layer0 = field_0x4;
    }
    for (s32 i = 0; i < 5; i++) {
        field_0x2ec[i].x1 = getPlayerPos_8023E724(field_0x90)->x;
        field_0x2ec[i].y1 = getPlayerPos_8023E724(field_0x90)->y;
        field_0x2ec[i].layer1 = getPlayerLayer_8023DE58(field_0x90);
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
    fn_8016890C(&mPos, field_0x4, (JGeometry::TBox2f*)field_0x370);
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

// Turns d into a direction and sets the velocity: 9 per frame for the fast variant, else 6.
static inline void aimVelocity(cls_804A0738* self, JGeometry::TVec2f d, bool fast) {
    d.normalize();
    if (fast) {
        self->field_0x3c.x = 9.0f * d.x;
        self->field_0x3c.y = 9.0f * d.y;
    } else {
        self->field_0x3c.x = 6.0f * d.x;
        self->field_0x3c.y = 6.0f * d.y;
    }
}

// The |dx|, |dy| <= 16 tests and the other fabs conditions were re-checked against the full asm.
// 0x80169030. A chaser: it follows the trail of positions the player recorded, then the
// player, and is caught by the player (state 1) or lost when it leaves the room.
// FIXME(shotgun): 69.45 %, missing/extra + reloc-name; behaviour drift.
void cls_804A0738::update() {
    s32 target = field_0x90;
    if (fn_8022F478(field_0x90) == 1 && fn_8022F514(field_0x90) == 1) {
        target = fn_8022F3DC(field_0x90);
    }
    if (field_0x23c == 0) {
        if (fabs(field_0x3c.x - field_0x364[0]) > 0.01f) {
            addCalc2_801F71A4(&field_0x3c.x, field_0x364[0], 0.1f, 0.2f);
        } else {
            field_0x364[0] = field_0x3c.x;
        }
        if (fabs(field_0x3c.y - field_0x364[1]) > 0.01f) {
            addCalc2_801F71A4(&field_0x3c.y, field_0x364[1], 0.1f, 0.2f);
        } else {
            field_0x364[1] = field_0x3c.y;
        }
    }
    if (fn_801F6758(&mPos, field_0x4, 300.0f)) {
        mAlive = false;
    }
    if (field_0x268 < 5) {
        JGeometry::TVec3f p;
        p.set(0.0f, 0.0f, 0.0f);
        p.x = field_0x2ec[field_0x268].x1;
        p.y = field_0x2ec[field_0x268].y1;
        if (fn_8022BC38(target, field_0x2ec[field_0x268].layer1, &p) == 1) {
            field_0x2ec[field_0x268].x1 = getPlayerPos_8023E724(target)->x;
            field_0x2ec[field_0x268].y1 = getPlayerPos_8023E724(target)->y;
        } else {
            field_0x2ec[field_0x268].layer0 = getPlayerLayer_8023DE58(target);
            field_0x2ec[field_0x268].x0 = getPlayerPos_8023E724(target)->x;
            field_0x2ec[field_0x268].y0 = getPlayerPos_8023E724(target)->y;
            field_0x268++;
            if (field_0x268 < 5) {
                field_0x2ec[field_0x268].layer1 = getPlayerLayer_8023DE58(target);
                field_0x2ec[field_0x268].x1 = getPlayerPos_8023E724(target)->x;
                field_0x2ec[field_0x268].y1 = getPlayerPos_8023E724(target)->y;
            }
        }
    }
    mPos += field_0x3c;
    switch (field_0x230) {
    case 0:
        if (fabs(field_0x3c.x - field_0x364[0]) <= 1.0f && fabs(field_0x3c.y - field_0x364[1]) <= 1.0f) {
            field_0x174 = field_0x180;
            field_0x3c.x = field_0x364[0];
            field_0x3c.y = field_0x364[1];
            field_0x3c.z = field_0x364[2];
            if (field_0x244) {
                field_0x26c = field_0x264;
                field_0x230 = 1;
                field_0x174 = 0;
                if (field_0x4 != getPlayerLayer_8023DE58(field_0x90)) {
                    JGeometry::TVec2f d;
                    if (field_0x264 > 0) {
                        d.set(field_0x274[field_0x264 - 1].x1 - mPos.x, field_0x274[field_0x264 - 1].y1 - mPos.y);
                    } else if (field_0x268 > 0) {
                        d.set(field_0x2ec[0].x1 - mPos.x, field_0x2ec[0].y1 - mPos.y);
                    } else {
                        field_0x26c = field_0x264;
                        field_0x230 = 1;
                        field_0x174 = 0;
                        break;
                    }
                    aimVelocity(this, d, field_0x25c != 0);
                }
                break;
            }
            JGeometry::TVec2f d;
            d.set(getPlayerPos_8023E724(field_0x90)->x - mPos.x, getPlayerPos_8023E724(field_0x90)->y - mPos.y);
            if (d.x * d.x + d.y * d.y < 240.0f) {
                mPos.x = getPlayerPos_8023E724(field_0x90)->x;
                mPos.y = getPlayerPos_8023E724(field_0x90)->y;
                mPos.z = 0.0f;
                mAlive = false;
                break;
            }
        } else if (field_0x264 < 5) {
            if (!field_0x17d) {
                field_0x274[field_0x264].x0 = mPos.x;
                field_0x274[field_0x264].y0 = mPos.y;
            } else {
                field_0x274[field_0x264].layer1 = field_0x4;
                field_0x274[field_0x264].x1 = mPos.x;
                field_0x274[field_0x264].y1 = mPos.y;
                field_0x264++;
                if (field_0x264 < 5) {
                    field_0x274[field_0x264].layer0 = field_0x4;
                    field_0x274[field_0x264].x0 = mPos.x;
                    field_0x274[field_0x264].y0 = mPos.y;
                }
                fn_8016890C(&mPos, field_0x4, (JGeometry::TBox2f*)field_0x370);
            }
        }
        {
            bool stop = false;
            if (field_0x4 == 0 && fn_801F6758(&mPos, field_0x4, -16.0f) == 1) {
                stop = true;
            } else if (field_0x4 >= 8) {
                if (mPos.x <= field_0x370[0] + 9.0f || mPos.x >= field_0x370[2] - 9.0f ||
                    mPos.y <= field_0x370[1] + 9.0f || mPos.y >= field_0x370[3] - 9.0f) {
                    stop = true;
                }
            }
            if (stop) {
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x364[2] = 0.0f;
                field_0x364[1] = 0.0f;
                field_0x364[0] = 0.0f;
            }
        }
        if (!fn_8022C274(target)) {
            field_0x244 = 1;
        }
        if (!(field_0x8 == getPlayerLayer_8023DE58(field_0x90) && fn_802373EC(field_0x90) == 3)) {
            field_0x244 = 1;
        }
        if (fn_8022CAF8(field_0x90) == 4 && fn_8022F478(field_0x90) == 1) {
            field_0x244 = 1;
        }
        if (fn_80168C38() == 1 || field_0x254 != 0) {
            field_0x3c.x *= -1.0f;
            field_0x3c.y *= -1.0f;
            field_0x3c.z *= -1.0f;
            if (field_0x254 == 0) {
                field_0x254 = 1;
            }
            if (field_0x4 != getPlayerLayer_8023DE58(field_0x90)) {
                JGeometry::TVec2f d;
                if (field_0x264 > 0) {
                    d.set(field_0x274[field_0x264 - 1].x1 - mPos.x, field_0x274[field_0x264 - 1].y1 - mPos.y);
                } else if (field_0x268 > 0) {
                    d.set(field_0x2ec[0].x1 - mPos.x, field_0x2ec[0].y1 - mPos.y);
                } else {
                    field_0x174 = 0;
                    field_0x26c = field_0x264;
                    field_0x230 = 1;
                    break;
                }
                aimVelocity(this, d, field_0x25c != 0);
            }
            field_0x174 = 0;
            field_0x26c = field_0x264;
            field_0x230 = 1;
        }
        break;
    case 1:
        if (fn_8022BC38(field_0x90, field_0x4, &mPos) == 1) {
            if (fn_8022D75C(target) == 1 || (fn_8022CAF8(field_0x90) == 4 && target != field_0x90)) {
                JGeometry::TVec2f d;
                d.set(getPlayerPos_8023E724(field_0x90)->x - mPos.x, getPlayerPos_8023E724(field_0x90)->y - mPos.y);
                if (field_0x260 == 1) {
                    mAlive = false;
                } else if (d.x * d.x + d.y * d.y < 240.0f) {
                    mPos.x = getPlayerPos_8023E724(field_0x90)->x;
                    mPos.y = getPlayerPos_8023E724(field_0x90)->y;
                    mPos.z = 0.0f;
                    field_0x260 = 1;
                } else {
                    aimVelocity(this, d, field_0x25c != 0 && field_0x261 == 0);
                }
                fn_80168C38();
                break;
            }
        }
        {
            JGeometry::TVec2f d;
            if (field_0x26c > 0) {
                d.set(field_0x274[field_0x26c - 1].x1 - mPos.x, field_0x274[field_0x26c - 1].y1 - mPos.y);
            } else if (field_0x270 < field_0x268) {
                d.set(field_0x2ec[field_0x270].x1 - mPos.x, field_0x2ec[field_0x270].y1 - mPos.y);
            } else {
                if (field_0x4 != getPlayerLayer_8023DE58(target)) {
                    break;
                }
                d.set(getPlayerPos_8023E724(target)->x - mPos.x, getPlayerPos_8023E724(target)->y - mPos.y);
            }
            aimVelocity(this, d, field_0x25c > 0 && field_0x261 == 0);
            if ((field_0x264 > 0 && field_0x26c > 0) || (field_0x268 > 0 && field_0x270 < field_0x268)) {
                bool fromFirst;
                Elem_80203594* pt;
                if (field_0x264 > 0 && field_0x26c > 0) {
                    pt = &field_0x274[field_0x26c - 1];
                    fromFirst = true;
                } else {
                    pt = &field_0x2ec[field_0x270];
                    fromFirst = false;
                }
                if (fabs(pt->x1 - mPos.x) <= 16.0f && fabs(pt->y1 - mPos.y) <= 16.0f) {
                    JGeometry::TVec3f p;
                    p.set(0.0f, 0.0f, 0.0f);
                    p.x = pt->x0;
                    p.y = pt->y0;
                    field_0x4 = pt->layer0;
                    mPos = p;
                    if (fromFirst) {
                        field_0x26c--;
                    } else {
                        field_0x270++;
                    }
                }
            }
            fn_80168C38();
        }
        break;
    }
    {
        JGeometry::TVec3f focus(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244) {
        field_0x258--;
        if (field_0x258 <= 0) {
            mAlive = false;
        }
    }
    if (field_0x234 == 0) {
        field_0x250 = (field_0x250 + 1) & 3;
        field_0x234 = 2;
        if (field_0x250 == 0) {
            startSound_801F0E34(0x3008D, 0);
        }
    }
    if (!fn_801F6758(&mPos, field_0x4, 0.0f)) {
        gBgRewrite_8053AC98->fn_802A20F0(&mPos, field_0x90, field_0x4, 0, 0);
    }
}

// 0x80168E78.
// FIXME(shotgun): 95.91 %, missing/extra; behaviour near.
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


// 0x80168C38. Whether a player standing in the (grown) hit box gets caught; every other
// player is then pushed along.
// FIXME(shotgun): 73.06 %, missing/extra; behaviour drift.
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
    if (found >= 0) {
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
    return false;
}

// 0x8016890C.
// FIXME(shotgun): 98.32 %, missing/extra + reloc-name; behaviour near.
void cls_804A0738::fn_8016890C(const JGeometry::TVec3f* pos, s32 layer, JGeometry::TBox2f* out) {
    if (layer == 0) {
        GameView_804A9950* view = getGameView_802D6A9C();
        const JGeometry::TBox2f& box = (layer < 8) ? view->mVisibleBox : view->field_0xdc;
        out->set(box);
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

// The original calls both of these with bl (update -> fn_80168C38, init/update -> fn_8016890C);
// our deferred build would inline them.
cls_804A0738::~cls_804A0738() {}
