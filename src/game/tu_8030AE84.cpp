#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/tu_8030AE84.h"

void cls_804AA8D8::fn_8030AE84(JGeometry::TBox2f* out) {
    out->i.x = (f32)field_0x290;
    out->i.y = (f32)field_0x294;
    out->f.x = (f32)(field_0x290 + 0xF0);
    out->f.y = (f32)(field_0x294 + 0xA0);
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

bool cls_804AA8D8::fn_8030B084(s32 index, const JGeometry::TVec2f* pos) {
    if (index < 8 || index != field_0x28c) {
        return false;
    }
    s32 x = (s32)pos->x;
    if (x < field_0x290 || x > field_0x290 + 0xF0) {
        return false;
    }
    s32 y = (s32)pos->y;
    if (y < field_0x294 || y > field_0x294 + 0xA0) {
        return false;
    }
    return true;
}

bool cls_804AA8D8::fn_8030B110(s32 index, const JGeometry::TVec2f* pos) {
    if (!fn_8023F264(field_0x288) || index != field_0x28c) {
        return false;
    }
    if (index < 8) {
        return true;
    }
    s32 x = (s32)pos->x;
    if (x < field_0x2a4 || x > field_0x2a0) {
        return false;
    }
    s32 y = (s32)pos->y;
    if (y < field_0x2ac || y > field_0x2a8) {
        return false;
    }
    return true;
}

s32 cls_804AA8D8::fn_8030B570(s32* outA, s32* outB) {
    s32 player = fn_8022EA0C(field_0x288);
    if (fn_80234950(player)) {
        player = field_0x2b4;
    }
    if (fn_8023DE58(player) != field_0x28c) {
        player = field_0x2b0;
    }
    if (fn_8023DE58(player) != field_0x28c) {
        player = field_0x288;
    }

    s32 x;
    s32 y;
    if (fn_8023DE58(player) >= 8) {
        x = (s32)fn_8023E724(player)->x;
        y = (s32)fn_8023E724(player)->y;
        if (fn_80234ECC(player)) {
            y -= (s32)fn_8023E724(player)->z;
        }
    } else {
        // Each axis fetches the view again, as the original does (the calls
        // aren't CSE'd across the position call).
        GameView_804A9950* viewA = fn_802D6A9C();
        GameView_804A9950* viewB = fn_802D6A9C();
        x = (s32)(fn_8023E724(player)->x - viewA->mVisibleBox.i.x + viewB->field_0xdc.i.x);
        viewA = fn_802D6A9C();
        viewB = fn_802D6A9C();
        y = (s32)(fn_8023E724(player)->y - viewA->mVisibleBox.i.y + viewB->field_0xdc.i.y);
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

void cls_804AA8D8::fn_8030BD3C(s32 player) {
    field_0x288 = player;
    if (field_0x2b0 < 0) {
        field_0x2b0 = field_0x288;
    }
    field_0x28c = fn_8023DE58(player);
    fn_8030B97C();
    field_0x2b4 = fn_8030B570(&field_0x290, &field_0x294);
    field_0x298 = 0;
    field_0x29c = 0;
}

// The deleting form (__dl__FPv when asked) is the compiler's; nothing to clean up.
cls_804AA8D8::~cls_804AA8D8() {}

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
