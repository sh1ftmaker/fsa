#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/JKernel/JKRArchive.h"

// Two per-index word tables (fn_80403070 / fn_8040305C).
static const s32 lbl_8048C160[4] = {0, 300, 600, 900};
static const s32 lbl_8048C170[3] = {2, 4, 6};

// The 640x448 render modes main hands to JFWSystem (tu_801344C0).
const GXRenderModeObj lbl_8048C17C = {
    VI_TVMODE_NTSC_INT, 608, 448, 448, 27, 16, 666, 448, VI_XFBMODE_DF, 0, 0,
    {{6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}},
    {8, 8, 10, 12, 10, 8, 8},
};

const GXRenderModeObj lbl_8048C1B8 = {
    VI_TVMODE_NTSC_PROG, 608, 448, 448, 27, 16, 666, 448, VI_XFBMODE_SF, 0, 0,
    {{6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}},
    {0, 0, 21, 22, 21, 0, 0},
};

GameState_804B0FE0 lbl_80529DEC;

// Moves the pane so that, after resizing it to (field_0x18 * sx, field_0x1c * sy),
// its centre stays where it was.
static inline void resizeKeepCentre(cls_804AF86C* self, f32 sx, f32 sy) {
    f32 w = self->mPane->getWidth();
    f32 x = self->mPane->getBounds()->i.x;
    f32 h = self->mPane->getHeight();
    f32 y = self->mPane->getBounds()->i.y;
    self->mPane->resize(self->field_0x18 * sx, self->field_0x1c * sy);
    self->mPane->move(0.5f * w + x - 0.5f * self->mPane->getWidth(),
                      0.5f * h + y - 0.5f * self->mPane->getHeight());
}

// Moves the pane along one axis, keeping the other coordinate.
static inline void moveY(cls_804AF86C* self, f32 centreY) {
    f32 y = centreY - 0.5f * self->mPane->getHeight();
    f32 x = self->mPane->getBounds()->i.x;
    self->mPane->move(x, y);
}

static inline void moveX(cls_804AF86C* self, f32 centreX) {
    f32 x = centreX - 0.5f * self->mPane->getWidth();
    f32 y = self->mPane->getBounds()->i.y;
    self->mPane->move(x, y);
}

void cls_804AF86C::fn_804023D0(J2DPane* pane, u8 alpha) {
    pane->setAlpha(alpha * ((f32)mPane->getAlpha() / (f32)field_0x28));
}

// FIXME: 98.50. Regalloc/scheduling only: the squared terms are multiplied (mullw) before the
// float conversion of the other operand in the original.
// Steps field_0x26 and fades the pane's alpha from `from` to `to`, mode 1 easing in and
// out, any other mode up and back.
bool cls_804AF86C::fn_8040245C(s16 frames, u8 from, u8 to, u8 mode) {
    field_0x26++;
    if (field_0x26 >= frames) {
        field_0x26 = 0;
    }
    f32 t;
    if (mode == 1) {
        s32 half = frames / 2;
        if (field_0x26 < half) {
            f32 h = frames * 0.5f;
            t = (f32)(field_0x26 * field_0x26) / (h * h);
        } else {
            s32 d = field_0x26 - half;
            f32 h = frames * 0.5f;
            t = 1.0f - (f32)(d * d) / (h * h);
        }
    } else {
        s32 half = frames / 2;
        s32 steps = frames - field_0x26;
        if (field_0x26 < half) {
            steps = field_0x26;
        }
        t = (f32)steps / (frames * 0.5f);
    }
    mPane->setAlpha(from + t * (to - from));
    return false;
}

bool cls_804AF86C::fn_80402628(s16 frames, u8 from, u8 to) {
    if (field_0x26 < frames - 1) {
        field_0x26++;
        f32 t = (f32)field_0x26 / (f32)frames;
        mPane->setAlpha(from + t * (to - from));
    } else {
        field_0x22 = frames;
        mPane->setAlpha(to);
        return true;
    }
    return false;
}

// FIXME: 99.56. Regalloc only: the original keeps the old width in f31 and the old y in f28, ours
// gets the four saved floats in a different order.
u8 cls_804AF86C::fn_8040271C(s16 frames, f32 x, f32 y) {
    if (field_0x24 < frames - 1) {
        field_0x24++;
        f32 t = (f32)(field_0x24 * field_0x24) / (f32)(frames * frames);
        f32 scale = x + t * (y - x);
        resizeKeepCentre(this, scale, scale);
    } else {
        field_0x24 = frames;
        resizeKeepCentre(this, y, y);
        return 1;
    }
    return 0;
}

u8 cls_804AF86C::fn_80402940(s16 frames, u8 mode, f32 x, f32 y) {
    if (field_0x22 < frames - 1) {
        field_0x22++;
        f32 t;
        if (mode == 0) {
            t = (f32)(field_0x22 * field_0x22) / (f32)(frames * frames);
        } else if (mode == 1) {
            t = (f32)field_0x22 / (f32)frames;
        } else {
            s32 steps = frames - field_0x22;
            t = (f32)(steps * steps) / (f32)(frames * frames);
            f32 tmp = x;
            x = y;
            y = tmp;
        }
        moveY(this, field_0x14 + (x + t * (y - x)));
    } else {
        field_0x22 = frames;
        moveY(this, field_0x14 + y);
        return 1;
    }
    return 0;
}

u8 cls_804AF86C::fn_80402B10(s16 frames, u8 mode, f32 x, f32 y) {
    if (field_0x20 < frames - 1) {
        field_0x20++;
        f32 t;
        if (mode == 0) {
            t = (f32)(field_0x20 * field_0x20) / (f32)(frames * frames);
        } else if (mode == 1) {
            t = (f32)field_0x20 / (f32)frames;
        } else {
            s32 steps = frames - field_0x20;
            t = (f32)(steps * steps) / (f32)(frames * frames);
            f32 tmp = x;
            x = y;
            y = tmp;
        }
        moveX(this, field_0x10 + (x + t * (y - x)));
    } else {
        field_0x20 = frames;
        moveX(this, field_0x10 + y);
        return 1;
    }
    return 0;
}

// FIXME: 99.59. Regalloc only: saved floats f29/f31/f28 are assigned in a different order.
void cls_804AF86C::fn_80402CE0(f32 x, f32 y) {
    resizeKeepCentre(this, x, y);
}

void cls_804AF86C::fn_80402E08(f32 x, f32 y) {
    mPane->move(field_0x10 + x - 0.5f * mPane->getWidth(), field_0x14 + y - 0.5f * mPane->getHeight());
}

void cls_804AF86C::fn_80402E70() {
    field_0x8 = mPane->getBounds()->i.x;
    field_0xc = mPane->getBounds()->i.y;
    field_0x18 = mPane->getWidth();
    field_0x1c = mPane->getHeight();
    field_0x10 = field_0x8 + 0.5f * field_0x18;
    field_0x14 = field_0xc + 0.5f * field_0x1c;
}

void cls_804AF86C::fn_80402F04(J2DPane* pane) {
    mPane = pane;
    field_0x8 = pane->getBounds()->i.x;
    field_0xc = pane->getBounds()->i.y;
    field_0x18 = pane->getWidth();
    field_0x1c = pane->getHeight();
    field_0x10 = field_0x8 + 0.5f * field_0x18;
    field_0x14 = field_0xc + 0.5f * field_0x1c;
    field_0x28 = pane->getAlpha();
    field_0x20 = 0;
    field_0x22 = 0;
    field_0x24 = 0;
    field_0x26 = 0;
}

// fn_80402FBC (the player's window position store) is not written: best attempt 66 %.

s32 fn_8040305C(s32 index) {
    return lbl_8048C170[index];
}

s32 fn_80403070(s32 index) {
    return lbl_8048C160[index];
}

void* fn_80403084(u32 type, const char* name) {
    return lbl_80529DEC.field_0x4c->getResource(type, name);
}

void* fn_804030C4(u32 type, const char* name) {
    return lbl_80529DEC.field_0x20->getResource(type, name);
}

void* fn_80403104(u32 type, const char* name) {
    return lbl_80529DEC.field_0x3c->getResource(type, name);
}

void* fn_80403144(u32 type, const char* name) {
    return lbl_80529DEC.field_0x38->getResource(type, name);
}

void* fn_80403184(u32 type, const char* name) {
    return lbl_80529DEC.field_0x34->getResource(type, name);
}

void* fn_804031C4(u32 type, const char* name) {
    return lbl_80529DEC.field_0x28->getResource(type, name);
}

void* fn_80403204(u32 type, const char* name) {
    return lbl_80529DEC.field_0x24->getResource(type, name);
}

void* fn_80403244(u32 type, const char* name) {
    return lbl_80529DEC.field_0x18->getResource(type, name);
}

u32 fn_80403284(const char* name) {
    return lbl_80529DEC.field_0x10->getResSize(lbl_80529DEC.field_0x10->getResource('????', name));
}

void* fn_804032E8(u32 type, const char* name) {
    return lbl_80529DEC.field_0x10->getResource(type, name);
}

void* fn_80403328(const char* name) {
    return lbl_80529DEC.field_0x10->getResource('????', name);
}

s32 fn_8040336C(s32 player) {
    if (lbl_8053AAF8->field_0x7f) {
        if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 0) {
            return 0;
        }
        return lbl_80529DEC.field_0x88[player];
    }
    return lbl_80529DEC.field_0xb8[player];
}

s32 fn_804033D4(s32 player) {
    if (lbl_8053AAF8->field_0x7f) {
        return lbl_80529DEC.field_0x78[player];
    }
    return lbl_80529DEC.field_0xa8[player];
}

s32 fn_80403414(s32 player) {
    if (lbl_8053AAF8->field_0x7f) {
        return lbl_80529DEC.field_0x68[player];
    }
    return lbl_80529DEC.field_0x98[player];
}

void fn_80403454(s32 player, s32 value) {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_80529DEC.field_0x88[player] = value;
    } else {
        lbl_80529DEC.field_0xb8[player] = value;
    }
}

void fn_80403494(s32 player, s32 value) {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_80529DEC.field_0x78[player] = value;
    } else {
        lbl_80529DEC.field_0xa8[player] = value;
    }
}

void fn_804034D4(s32 player, s32 value) {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_80529DEC.field_0x68[player] = value;
    } else {
        lbl_80529DEC.field_0x98[player] = value;
    }
}

// GameState_804B0FE0::fn_80403514 (the stage picker over lbl_8050EC80) is not written: best attempt 66 %.

void GameState_804B0FE0::fn_80403680() {
    for (s32 i = 0; i < 4; i++) {
        field_0x68[i] = 0;
        field_0x78[i] = 1;
        field_0x88[i] = 2;
        field_0x98[i] = 0;
        field_0xa8[i] = 1;
        field_0xb8[i] = 2;
        field_0xe8[i] = 0;
        field_0xd8[i] = 4;
        field_0xc8[i] = 4;
        field_0x108[i] = 0x80;
        field_0xf8[i] = 0;
        field_0x130[i] = 0;
        field_0x140[i] = 0;
        field_0x150[i] = 0;
        field_0x160[i] = 0;
        field_0x170[i] = 0;
        field_0x180[i] = 0;
        field_0x190[i] = 0;
        field_0x1c4[i].x = 0.0f;
        field_0x1c4[i].y = 0.0f;
        field_0x1c4[i].z = 0.0f;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x1b4[i] = 0;
        field_0x180[i] = 0;
    }
    field_0x1b0 = 0;
    field_0x214 = 0;
    field_0x215 = 0;
    field_0x64 = -1;
    field_0x1a8 = -1;
    field_0x1a4 = -1;
    field_0x236 = 0;
    field_0x118 = 0;
    field_0x11c = 0;
    field_0x120 = 0;
    field_0x124 = 0;
    field_0x128 = 0;
    field_0x12c = 0;
    field_0x239 = 0;
    field_0x1ac = 0;
    field_0x253 = 0;
    field_0x254[0] = 0;
    field_0x237 = 0;
    for (u32 i = 0; i < sizeof(field_0x1fc); i++) {
        field_0x1fc[i] = 0xFF;
    }
    field_0x242 = 0;
    field_0x243[0] = 0;
    field_0x23e[0] = 0;
    field_0x240 = 0;
    field_0x249[1] = 0;
    field_0x24b = 0;
    field_0x24e = 0;
    field_0x24f = 0;
    field_0x24d = 0;
    field_0x252 = 0;
}

GameState_804B0FE0::~GameState_804B0FE0() {}

GameState_804B0FE0::GameState_804B0FE0() {
    field_0x4 = NULL;
    mHeap = NULL;
    field_0xc = NULL;
    field_0x10 = NULL;
    field_0x14 = NULL;
    field_0x18 = NULL;
    field_0x1c = NULL;
    field_0x20 = NULL;
    field_0x24 = NULL;
    field_0x28 = NULL;
    field_0x2c = NULL;
    field_0x30 = NULL;
    field_0x34 = NULL;
    field_0x38 = NULL;
    field_0x3c = NULL;
    field_0x40 = NULL;
    field_0x44 = NULL;
    field_0x48 = NULL;
    field_0x4c = NULL;
    field_0x50 = NULL;
    field_0x54 = NULL;
    field_0x58 = NULL;
    field_0x5c = NULL;
    field_0x60 = NULL;
    field_0x1b0 = 0;
    field_0x214 = 0;
    field_0x215 = 0;
    for (u32 i = 0; i < sizeof(mSaveRegion); i++) {
        mSaveRegion.bytes[i] = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x68[i] = 0;
        field_0x78[i] = 1;
        field_0x88[i] = 2;
        field_0x98[i] = 0;
        field_0xa8[i] = 1;
        field_0xb8[i] = 2;
        field_0xe8[i] = 0;
        field_0xd8[i] = 4;
        field_0xc8[i] = 4;
        field_0x108[i] = 0x80;
        field_0xf8[i] = 0;
        field_0x130[i] = 0;
        field_0x140[i] = 0;
        field_0x150[i] = 0;
        field_0x160[i] = 0;
        field_0x170[i] = 0;
        field_0x180[i] = 0;
        field_0x190[i] = 0;
        field_0x1c4[i].x = 0.0f;
        field_0x1c4[i].y = 0.0f;
        field_0x1c4[i].z = 0.0f;
    }
    field_0x64 = -1;
    field_0x1a8 = -1;
    field_0x236 = 0;
    field_0x118 = 0;
    field_0x11c = 0;
    field_0x120 = 0;
    field_0x124 = 0;
    field_0x128 = 0;
    field_0x12c = 0;
    field_0x237 = 0;
    field_0x238 = 0xFF;
    field_0x23a[0] = 0;
    field_0x23a[1] = 0;
    field_0x23c = 0;
    field_0x23d = 0;
    field_0x239 = 0;
    field_0x1a0 = 0;
    field_0x1a4 = -1;
    field_0x1ac = 0;
    field_0x253 = 0;
    field_0x254[0] = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x1b4[i] = 0;
        field_0x1f4[i] = 0;
    }
    for (u32 i = 0; i < sizeof(field_0x1fc); i++) {
        field_0x1fc[i] = 0xFF;
    }
    field_0x241[0] = 1;
    field_0x242 = 0;
    field_0x243[0] = 0;
    field_0x23e[0] = 0;
    field_0x23e[1] = 0;
    field_0x240 = 0;
    field_0x243[1] = 0;
    field_0x245 = 0;
    field_0x246[0] = 1;
    field_0x246[1] = 0;
    field_0x249[0] = 0;
    field_0x248 = 0xFF;
    field_0x249[1] = 0;
    field_0x24b = 0;
    field_0x24c = 0;
    field_0x24d = 0;
    field_0x24e = 0;
    field_0x24f = 0;
    field_0x250 = 0;
    field_0x251 = 0;
    field_0x252 = 0;
}
