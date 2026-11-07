#include "game/GameAudio.h"
#include "game/act/tu_8028DD78.h"
#include "game/Player.h"
#include "game/GameGlobals.h"
#include "game/cls_804AE9B0.h"

// FIXME(shotgun): 98.6 %, reloc-name
void cls_804A7A68::init() {
    switch (field_0x90 & 0xF) {
    case 0:
    case 1:
    case 4:
        mBox.i.x = -40.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 8.0f;
        break;
    case 5:
        mBox.i.x = -40.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 10.0f;
        break;
    case 6:
        mBox.i.x = -8.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 40.0f;
        break;
    case 7:
        mBox.i.x = -10.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 40.0f;
        break;
    }
    mLife = 0;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x268 = 0xFF;
}

// Moves the view one screen towards pos when it lies outside the visible box.
static inline void scrollTo(JGeometry::TVec2f pos) {
    s32 col = getGameView_802D6A9C()->field_0x14;
    s32 row = getGameView_802D6A9C()->field_0x18;
    JGeometry::TBox2f box = getGameView_802D6A9C()->mVisibleBox;
    if (pos.x < box.i.x) {
        col--;
    } else if (pos.x >= box.f.x) {
        col++;
    }
    if (pos.y < box.i.y) {
        row--;
    } else if (pos.y >= box.f.y) {
        row++;
    }
    getGameView_802D6A9C()->fn_802D9FF4(col, row);
}

// The signed 7-bit offsets of field_0x90 (bits 16-22 with sign bit 23, bits 8-14 with sign
// bit 15), in tiles.
static inline s32 offsetX(u32 f) {
    s32 v = (f >> 16) & 0x7F;
    if (f & 0x800000) {
        v = -v;
    }
    return v;
}
static inline s32 offsetY(u32 f) {
    s32 v = (f >> 8) & 0x7F;
    if (f & 0x8000) {
        v = -v;
    }
    return v;
}

// The signed 4-bit value at bits 4-7 of field_0x90.
static inline s32 field4(u32 f) {
    return (f & 0x80) ? (s32)((f >> 4) & 7) - 8 : (s32)((f >> 4) & 7);
}

// FIXME(shotgun): 85.21 %, missing/extra + reloc-name
// The four-player gate sequence: 0 waits until every player stands in mBox (on this layer),
// then lines them up (fn_8028DDD8) at 16-unit spacing; 1 fades field_0x268 out and walks them
// half a unit per frame for 64 (40) frames; 2 sends them 384 units on (kinds 0 / 1) or to the
// tile offset in field_0x90 (kinds 4-7), scrolls the view and sets the music; 3 fades back in
// and walks them on; 4 hands control back.
void cls_804A7A68::update() {
    switch (field_0x230) {
    case 0: {
        if (vfunc_0x3C() != 0 && !vfunc_0x34()) {
            break;
        }
        f32 x0 = mBox.i.x + mPos.x;
        f32 x1 = mBox.f.x + mPos.x;
        f32 y0 = mBox.i.y + mPos.y;
        f32 y1 = mBox.f.y + mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (getPlayerPos_8023E724(i)->z > 0.0f && !fn_8023EA28(i)) {
                return;
            }
            if (getPlayerLayer_8023DE58(i) != field_0x4) {
                return;
            }
            JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
            bool in = false;
            if (p->x >= x0 && p->y >= y0 && p->x < x1 && p->y < y1) {
                in = true;
            }
            if (in) {
                fn_802381B8(i);
            }
        }
        s32 inside = 0;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                if (!fn_802300EC(i)) {
                    return;
                }
                if (fn_80230188(i)) {
                    return;
                }
                if (fn_80236B50(i)) {
                    return;
                }
                if (getPlayerPos_8023E724(i)->z > 0.0f && !fn_8023EA28(i)) {
                    return;
                }
                if (getPlayerLayer_8023DE58(i) != field_0x4) {
                    return;
                }
                JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
                bool in = false;
                if (p->x >= x0 && p->y >= y0 && p->x < x1 && p->y < y1) {
                    in = true;
                }
                if (in && !fn_8022F478(i)) {
                    inside++;
                }
            }
        }
        if (gGameRoot_8053AAF8->fn_80138A30() > inside) {
            return;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023ED8C(i)) {
                fn_80230C6C(i);
            }
        }
        s32 order[4];
        fn_8028DDD8(order);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(order[i])) {
                lbl_8053AD70->fn_8037AE00();
                fn_80230AD4(order[i]);
                JGeometry::TVec3f p;
                switch (field_0x90 & 0xF) {
                case 0:
                case 1:
                case 4:
                    fn_80230BF4(order[i], 3);
                    fn_80230FD0(i, 0x20018);
                    p = mPos;
                    p.x = mPos.x + (16.0f * i - 24.0f);
                    fn_802303FC(order[i], &p, (u8)field_0x268);
                    field_0x238[order[i]] = p;
                    break;
                case 5:
                    fn_80230BF4(order[i], 2);
                    fn_80230FD0(i, 0x2001A);
                    p = mPos;
                    p.x = mPos.x + (16.0f * i - 24.0f);
                    fn_802303FC(order[i], &p, (u8)field_0x268);
                    field_0x238[order[i]] = p;
                    break;
                case 6:
                    fn_80230BF4(order[i], 0);
                    fn_80230FD0(i, 0x2001A);
                    p = mPos;
                    p.y = mPos.y + (16.0f * i - 24.0f);
                    fn_802303FC(order[i], &p, (u8)field_0x268);
                    field_0x238[order[i]] = p;
                    break;
                case 7:
                    fn_80230BF4(order[i], 1);
                    fn_80230FD0(i, 0x2001A);
                    p = mPos;
                    p.y = mPos.y + (16.0f * i - 24.0f);
                    fn_802303FC(order[i], &p, (u8)field_0x268);
                    field_0x238[order[i]] = p;
                    break;
                }
            }
        }
        field_0x230 = 1;
        break;
    }
    case 1:
        if (!(field_0x90 & 0x1000000)) {
            field_0x268 -= 7;
            if (field_0x268 < 0) {
                field_0x268 = 0;
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                JGeometry::TVec3f start = *getPlayerPos_8023E724(i);
                JGeometry::TVec3f pos = start;
                JGeometry::TVec3f ahead;
                switch (field_0x90 & 0xF) {
                case 0:
                case 1:
                    pos.y -= 0.5f;
                    break;
                case 4:
                    if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                        pos.y -= 0.5f;
                    }
                    fn_8022E088(i, 3, &ahead);
                    break;
                case 5:
                    if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                        pos.y += 0.5f;
                    }
                    fn_8022E088(i, 2, &ahead);
                    break;
                case 6:
                    pos.x += 0.5f;
                    fn_8022E088(i, 0, &ahead);
                    break;
                case 7:
                    pos.x -= 0.5f;
                    fn_8022E088(i, 1, &ahead);
                    break;
                }
                if ((field_0x90 & 0xF) >= 4 && fn_802265DC(&ahead, getPlayerLayer_8023DE58(i), true)) {
                    pos = start;
                }
                fn_802303FC(i, &pos, (u8)field_0x268);
            }
        }
        field_0x234++;
        if ((field_0x90 & 0xF) >= 4) {
            if (field_0x234 >= 0x28) {
                field_0x234 = 0;
                field_0x230 = 2;
            }
        } else if (field_0x234 >= 0x40) {
            field_0x234 = 0;
            field_0x230 = 2;
        }
        break;
    case 2:
        switch (field_0x90 & 0xF) {
        case 0:
            for (s32 i = 0; i < 4; i++) {
                if (fn_80230188(i)) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    p.y -= 384.0f;
                    fn_80230498(i, &p, fn_8023CBC0(i), 0xFF, -0x80, -0x80, true);
                    fn_80230BF4(i, 2);
                }
            }
            scrollTo(JGeometry::TVec2f(field_0x238[0].x, field_0x238[0].y - 384.0f));
            break;
        case 1:
            for (s32 i = 0; i < 4; i++) {
                if (fn_80230188(i)) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    p.y += 384.0f;
                    fn_80230498(i, &p, fn_8023CBC0(i), 0xFF, -0x80, -0x80, true);
                    fn_80230BF4(i, 2);
                }
            }
            scrollTo(JGeometry::TVec2f(field_0x238[0].x, 384.0f + field_0x238[0].y));
            break;
        case 4:
            for (s32 i = 0; i < 4; i++) {
                if (fn_80230188(i)) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    p.x = 16.0f * offsetX(field_0x90) + field_0x238[i].x;
                    p.y = 16.0f + (16.0f * offsetY(field_0x90) + field_0x238[i].y);
                    fn_80230498(i, &p, fn_8023CBC0(i), 0, -0x80, -0x80, true);
                    fn_80230BF4(i, 3);
                }
            }
            scrollTo(JGeometry::TVec2f(16.0f * offsetX(field_0x90) + field_0x238[0].x,
                                       16.0f * offsetY(field_0x90) + field_0x238[0].y));
            break;
        case 5:
            for (s32 i = 0; i < 4; i++) {
                if (fn_80230188(i)) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    p.x = 16.0f * offsetX(field_0x90) + field_0x238[i].x;
                    p.y = (16.0f * offsetY(field_0x90) + field_0x238[i].y) - 16.0f;
                    fn_80230498(i, &p, fn_8023CBC0(i), 0, -0x80, -0x80, true);
                    fn_80230BF4(i, 2);
                }
            }
            scrollTo(JGeometry::TVec2f(16.0f * offsetX(field_0x90) + field_0x238[0].x,
                                       16.0f * offsetY(field_0x90) + field_0x238[0].y));
            break;
        case 6:
            for (s32 i = 0; i < 4; i++) {
                if (fn_80230188(i)) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    p.x = (16.0f * offsetX(field_0x90) + field_0x238[i].x) - 16.0f;
                    p.y = 16.0f * offsetY(field_0x90) + field_0x238[i].y;
                    fn_80230498(i, &p, fn_8023CBC0(i), 0, -0x80, -0x80, true);
                    fn_80230BF4(i, 0);
                }
            }
            scrollTo(JGeometry::TVec2f(16.0f * offsetX(field_0x90) + field_0x238[0].x,
                                       16.0f * offsetY(field_0x90) + field_0x238[0].y));
            break;
        case 7:
            for (s32 i = 0; i < 4; i++) {
                if (fn_80230188(i)) {
                    JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
                    p.x = 16.0f + (16.0f * offsetX(field_0x90) + field_0x238[i].x);
                    p.y = 16.0f * offsetY(field_0x90) + field_0x238[i].y;
                    fn_80230498(i, &p, fn_8023CBC0(i), 0, -0x80, -0x80, true);
                    fn_80230BF4(i, 1);
                }
            }
            scrollTo(JGeometry::TVec2f(16.0f * offsetX(field_0x90) + field_0x238[0].x,
                                       16.0f * offsetY(field_0x90) + field_0x238[0].y));
            break;
        }
        // FIXME(shotgun): the original also passes 0 in r5 to fn_8043ABCC (a second
        // parameter the header does not declare).
        if (field4(field_0x90) == -8) {
            lbl_8053AED0->fn_8043A608();
        } else if (field4(field_0x90) != 0) {
            lbl_8053AED0->fn_8043ABCC(field4(field_0x90));
        }
        gGameRoot_8053AAF8->fn_80136798(0x13, 0);
        field_0x230 = 3;
        break;
    case 3:
        if (!(field_0x90 & 0x1000000)) {
            field_0x268 += 7;
            if (field_0x268 > 0xFF) {
                field_0x268 = 0xFF;
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                switch (field_0x90 & 0xF) {
                case 0:
                case 1:
                    if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
                        if (field_0x234 == 0) {
                            pos.y += 8.0f;
                        }
                    } else {
                        pos.y += 0.5f;
                    }
                    break;
                case 4:
                    if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
                        if (field_0x234 == 0) {
                            pos.y -= 8.0f;
                        }
                    } else {
                        pos.y -= 0.5f;
                    }
                    break;
                case 5:
                    if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
                        if (field_0x234 == 0) {
                            pos.y += 8.0f;
                        }
                    } else {
                        pos.y += 0.5f;
                    }
                    break;
                case 6:
                    pos.x += 0.5f;
                    break;
                case 7:
                    pos.x -= 0.5f;
                    break;
                }
                fn_802303FC(i, &pos, (u8)field_0x268);
            }
        }
        field_0x234++;
        if ((field_0x90 & 0xF) >= 4) {
            if (field_0x234 >= 0x28) {
                field_0x234 = 0;
                field_0x230 = 4;
            }
        } else if (field_0x234 >= 0x62) {
            field_0x234 = 0;
            field_0x230 = 4;
        }
        break;
    case 4:
        fn_80231368();
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                switch (field_0x90 & 0xF) {
                case 0:
                case 1:
                    fn_8022CA6C(i, 5, false);
                    break;
                case 4:
                case 5:
                    fn_8022CA6C(i, 5, false);
                    break;
                case 6:
                case 7:
                    fn_8022CA6C(i, 5, false);
                    fn_80230224(i, 0);
                    break;
                }
            }
        }
        mAlive = false;
        lbl_8053AD70->fn_8037AE30();
        break;
    }
}

void cls_804A7A68::draw() {}

// 0x8028DDD8. Orders the players for the four marker slots: out[] gets the player indices
// sorted by their x (y for the vertical variants 6 and 7), grouped by team for two players.
// FIXME(shotgun): 92.25 %, missing/extra
void cls_804A7A68::fn_8028DDD8(s32* out) {
    f32 key[4];
    for (s32 i = 0; i < 4; i++) {
        if ((field_0x90 & 0xF) == 6 || (field_0x90 & 0xF) == 7) {
            key[i] = getPlayerPos_8023E724(i)->y;
        } else {
            key[i] = getPlayerPos_8023E724(i)->x;
        }
    }
    switch (gGameRoot_8053AAF8->fn_80138A30()) {
    case 2: {
        s32 n = 1;
        if (key[0] <= key[1]) {
            out[0] = 0;
            if (fn_8022E970(0) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 0) {
                        out[n] = i;
                        n++;
                    }
                }
            }
            out[n] = 1;
            n++;
            if (fn_8022E970(1) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 1) {
                        out[n] = i;
                        n++;
                    }
                }
            }
        } else {
            out[0] = 1;
            if (fn_8022E970(1) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 1) {
                        out[n] = i;
                        n++;
                    }
                }
            }
            out[n] = 0;
            n++;
            if (fn_8022E970(0) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 0) {
                        out[n] = i;
                        n++;
                    }
                }
            }
        }
        break;
    }
    case 3: {
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        for (s32 i = 0; i < 2; i++) {
            s32 a = out[i];
            s32 b = out[i + 1];
            if (key[a] > key[b]) {
                out[i] = b;
                out[i + 1] = a;
                i = -1;
            }
        }
        s32 leader = fn_8022F3DC(3);
        for (s32 i = 0; i < 3; i++) {
            if (leader == out[i]) {
                if (i == 2) {
                    out[3] = 3;
                } else {
                    for (s32 m = 3; m > i + 1; m--) {
                        out[m] = out[m - 1];
                    }
                    out[i + 1] = 3;
                }
            }
        }
        break;
    }
    case 4:
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        for (s32 i = 0; i < 3; i++) {
            s32 a = out[i];
            s32 b = out[i + 1];
            if (key[a] > key[b]) {
                out[i] = b;
                out[i + 1] = a;
                i = -1;
            }
        }
        break;
    default:
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        break;
    }
}

cls_804A7A68::~cls_804A7A68() {}
