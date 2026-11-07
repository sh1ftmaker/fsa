#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/tu_8030AE84.h"
#include <float.h>

cls_804AA8D8::cls_804AA8D8() {
    field_0x28c = 0;
    field_0x2b4 = -1;
    field_0x2b0 = -1;
    field_0x288 = -1;
    field_0x2b8 = 0;
    field_0x2ba = 0;
    field_0x2bc = 0xFF;
    field_0x284 = 0;
}

// The deleting form (__dl__FPv when asked) is the compiler's; nothing to clean up.
cls_804AA8D8::~cls_804AA8D8() {}

void cls_804AA8D8::fn_8030BD3C(s32 player) {
    field_0x288 = player;
    if (field_0x2b0 < 0) {
        field_0x2b0 = field_0x288;
    }
    field_0x28c = getPlayerLayer_8023DE58(player);
    fn_8030B97C();
    field_0x2b4 = fn_8030B570(&field_0x290, &field_0x294);
    field_0x298 = 0;
    field_0x29c = 0;
}

s32 cls_804AA8D8::fn_8030B570(s32* outA, s32* outB) {
    s32 player = fn_8022EA0C(field_0x288);
    if (fn_80234950(player)) {
        player = field_0x2b4;
    }
    if (getPlayerLayer_8023DE58(player) != field_0x28c) {
        player = field_0x2b0;
    }
    if (getPlayerLayer_8023DE58(player) != field_0x28c) {
        player = field_0x288;
    }

    s32 x;
    s32 y;
    if (getPlayerLayer_8023DE58(player) >= 8) {
        x = (s32)getPlayerPos_8023E724(player)->x;
        y = (s32)getPlayerPos_8023E724(player)->y;
        if (fn_80234ECC(player)) {
            y -= (s32)getPlayerPos_8023E724(player)->z;
        }
    } else {
        // Each axis fetches the view again, as the original does (the calls
        // aren't CSE'd across the position call).
        GameView_804A9950* viewA = getGameView_802D6A9C();
        GameView_804A9950* viewB = getGameView_802D6A9C();
        x = (s32)(getPlayerPos_8023E724(player)->x - viewA->mVisibleBox.i.x + viewB->field_0xdc.i.x);
        viewA = getGameView_802D6A9C();
        viewB = getGameView_802D6A9C();
        y = (s32)(getPlayerPos_8023E724(player)->y - viewA->mVisibleBox.i.y + viewB->field_0xdc.i.y);
    }

    // The x test uses the position before the half-width is taken off, as
    // the original does; the y test uses the offset top edge.
    s32 right = x + 0x78;
    y -= 0x50;
    x -= 0x78;
    if (right >= field_0x2a0) {
        x = field_0x2a0 - 0xF0;
    } else if (x <= field_0x2a4) {
        x = field_0x2a4;
    }
    if (y + 0xA0 >= field_0x2a8) {
        y = field_0x2a8 - 0xA0;
    } else if (y <= field_0x2ac) {
        y = field_0x2ac;
    }

    x += field_0x298;
    y += field_0x29c;
    if (x < 0) {
        x = 0;
    } else if (x > 0x110) {
        x = 0x110;
    }
    if (y < 0) {
        y = 0;
    } else if (y > 0x160) {
        y = 0x160;
    }

    *outA = x;
    *outB = y;
    return player;
}

// The bounds of the screen-sized area around the player: 0x2A0/0x2A4 in x, 0x2A8/0x2AC in y.
void cls_804AA8D8::fn_8030B97C() {
    s32 layer = getPlayerLayer_8023DE58(field_0x288);
    if (layer < 8) {
        layer = 8;
    }
    JGeometry::TVec3f v = *getPlayerPos_8023E724(field_0x288);
    u32 cellX = (u32)v.x & ~7;
    u32 cellY = (u32)v.y & ~7;
    v.set((f32)cellX, (f32)cellY, 0.0f);
    // The bounds of the walkable run around the player, in 8-unit steps, up to 64 steps.
    for (int i = 0; i < 64; i++) {
        if (v.x >= 512.0f) {
            v.x = 512.0f;
            break;
        }
        u16 tile = fn_80226850(&v, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            break;
        }
        v.x += 8.0f;
    }
    field_0x2a0 = (s32)v.x;
    if (field_0x2a0 < 0xF0) {
        field_0x2a0 = 0xF0;
    }
    v.x = (f32)cellX;
    for (int i = 0; i < 64; i++) {
        if (v.x < 8.0f) {
            v.x = 0.0f;
            break;
        }
        u16 tile = fn_80226850(&v, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            v.x += 8.0f;
            break;
        }
        v.x -= 8.0f;
    }
    field_0x2a4 = (s32)v.x;
    if (field_0x2a4 < 0) {
        field_0x2a4 = 0;
    }
    if (field_0x2a0 - field_0x2a4 < 0xF0) {
        field_0x2a4 = field_0x2a0 - 0xF0;
    }
    v.set((f32)cellX, (f32)cellY, 0.0f);
    for (int i = 0; i < 64; i++) {
        if (v.y >= 512.0f) {
            v.y = 512.0f;
            break;
        }
        u16 tile = fn_80226850(&v, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            break;
        }
        v.y += 8.0f;
    }
    field_0x2a8 = (s32)v.y;
    if (field_0x2a8 < 0xA0) {
        field_0x2a8 = 0xA0;
    }
    v.y = (f32)cellY;
    for (int i = 0; i < 64; i++) {
        if (v.y < 8.0f) {
            v.y = 0.0f;
            break;
        }
        u16 tile = fn_80226850(&v, layer);
        if (tile >= 8 && tile < 16 && (tile & 1)) {
            v.y += 8.0f;
            break;
        }
        v.y -= 8.0f;
    }
    field_0x2ac = (s32)v.y;
    if (field_0x2ac < 0) {
        field_0x2ac = 0;
    }
    if (field_0x2a8 - field_0x2ac < 0xA0) {
        field_0x2ac = field_0x2a8 - 0xA0;
    }
}

// FIXME(shotgun): 94.5 %, different (epsilon compare shape)
void cls_804AA8D8::fn_8030B774() {
    if (!fn_8023F264(field_0x288)) {
        return;
    }
    if (field_0x28c != getPlayerLayer_8023DE58(field_0x288)) {
        field_0x28c = getPlayerLayer_8023DE58(field_0x288);
        fn_8030B97C();
    }
    s32 y;
    s32 x;
    field_0x2b4 = fn_8030B570(&x, &y);
    if (field_0x2b4 != field_0x2b0) {
        JGeometry::TVec2f d;
        d.set((f32)(y - field_0x294), (f32)(x - field_0x290));
        // d is (dy, dx) so that the squared length sums in the order the target has
        f32 len = d.length();
        if (len > 16.0f) {
            // Move 16 units a frame towards the target.
            f32 sq = d.squared();
            f32 dy = d.x;
            f32 dx = d.y;
            if (sq > 32.0f * FLT_EPSILON) {
                f32 inv = JGeometry::TUtil<f32>::inv_sqrt(sq);
                dx *= inv;
                dy *= inv;
            }
            field_0x290 = (s32)((f32)field_0x290 + 16.0f * dx);
            field_0x294 = (s32)((f32)field_0x294 + 16.0f * dy);
        } else {
            field_0x290 = x;
            field_0x294 = y;
            field_0x2b0 = field_0x2b4;
        }
    } else {
        field_0x290 = x;
        field_0x294 = y;
    }
}

// FIXME(shotgun): 91.5 %, regalloc/scheduling (227 vs 226 instrs)
s32* cls_804AA8D8::fn_8030B1E8(u8 layer, u8 mode, u16 scroll) {
    if (layer != field_0x2bc) {
        if (mode == 0 || mode == 6) {
            field_0x2b8 = 0;
            field_0x2ba = 0;
            field_0x2bc = 0xFF;
            field_0x284 = 0;
        } else {
            if (mode == 4) {
                field_0x2b8 = 0x60;
            } else if (mode == 5) {
                field_0x2b8 = 3;
            } else {
                field_0x2b8 = 8;
            }
            field_0x2ba = 0;
            field_0x2bc = layer;
            field_0x284 = mode;
        }
    } else if (mode != 6 && mode != field_0x284) {
        if (mode == 4) {
            field_0x2b8 = 0x60;
        } else if (mode == 5) {
            field_0x2b8 = 3;
        } else {
            field_0x2b8 = 8;
        }
        field_0x2ba = 0;
        field_0x2bc = layer;
        field_0x284 = mode;
    }
    if (field_0x2b8 == 0) {
        return 0;
    }
    switch (field_0x284) {
    case 2:
        field_0x2ba += (0x160 - scroll) * 7 + 0xC0;
        break;
    case 3:
        field_0x2ba += scroll * 7 + 0xC0;
        break;
    case 5:
        field_0x2ba += 0x120;
        break;
    default:
        field_0x2ba += field_0x2b8 * 16 + 0x100;
        break;
    }
    s16 angle = field_0x2ba;
    s32* out = field_0x4;
    for (int i = 0; i < 20; i++) {
        for (int k = 0; k < 8; k++) {
            *out++ = (s32)(field_0x2b8 * JMASSin((s16)(angle + k * 0x200)));
        }
        angle += 0x1000;
    }
    if (field_0x284 == 0 || field_0x284 == 4) {
        field_0x2b8--;
    }
    return field_0x4;
}

bool cls_804AA8D8::fn_8030B110(s32 index, JGeometry::TVec3f pos) {
    if (!fn_8023F264(field_0x288) || index != field_0x28c) {
        return false;
    }
    if (index < 8) {
        return true;
    }
    s32 x = (s32)pos.x;
    if (x < field_0x2a4 || x > field_0x2a0) {
        return false;
    }
    s32 y = (s32)pos.y;
    if (y < field_0x2ac || y > field_0x2a8) {
        return false;
    }
    return true;
}

bool cls_804AA8D8::fn_8030B084(s32 index, JGeometry::TVec3f pos) {
    if (index < 8 || index != field_0x28c) {
        return false;
    }
    s32 x = (s32)pos.x;
    if (x < field_0x290 || x > field_0x290 + 0xF0) {
        return false;
    }
    s32 y = (s32)pos.y;
    if (y < field_0x294 || y > field_0x294 + 0xA0) {
        return false;
    }
    return true;
}

bool cls_804AA8D8::fn_8030AF10(s32 index, const JGeometry::TBox2f* box, bool overlap) {
    if (index < 8 || index != field_0x28c) {
        return false;
    }
    if (overlap) {
        if ((s32)box->f.x < field_0x290) {
            return false;
        }
        if ((s32)box->i.x > field_0x290 + 0xF0) {
            return false;
        }
        if ((s32)box->f.y < field_0x294) {
            return false;
        }
        if ((s32)box->i.y > field_0x294 + 0xA0) {
            return false;
        }
    } else {
        s32 x = (s32)box->i.x;
        if (x < field_0x290 || x > field_0x290 + 0xF0) {
            return false;
        }
        x = (s32)box->f.x;
        if (x < field_0x290 || x > field_0x290 + 0xF0) {
            return false;
        }
        s32 y = (s32)box->i.y;
        if (y < field_0x294 || y > field_0x294 + 0xA0) {
            return false;
        }
        y = (s32)box->f.y;
        if (y < field_0x294 || y > field_0x294 + 0xA0) {
            return false;
        }
    }
    return true;
}

void cls_804AA8D8::fn_8030AE84(JGeometry::TBox2f* out) {
    out->i.x = (f32)field_0x290;
    out->i.y = (f32)field_0x294;
    out->f.x = (f32)(field_0x290 + 0xF0);
    out->f.y = (f32)(field_0x294 + 0xA0);
}
