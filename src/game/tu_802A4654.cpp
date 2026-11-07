#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"

// FIXME(shotgun): tables not written out (gate.py literal tripwire); extern-declared, sizes
// from symbols.txt. lbl_8046C658 / lbl_8046C6C8: the x and y shake offsets of four steps per
// shake kind (s32, 16 bytes per kind).
extern const s32 lbl_8046C658[28];
extern const s32 lbl_8046C6C8[88];

// 0x802A6600.
// FIXME(shotgun): 71.43 %, missing/extra + reloc-name; the player-mask test needs a bool form and the
// 1000-subtraction loop is unrolled here but not in the original
void cls_804A7EB0::init() {
    mBox.set(-3.0f, -3.0f, 3.0f, 3.0f);
    field_0x230 = 0;
    field_0x3c.z = 0.0f;
    field_0x24c = 9;
    field_0x250 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x2a4[i] = -1;
        field_0x254[i] = -1;
        for (s32 j = 0; j < 5; j++) {
            mEmitters[j][i] = NULL;
        }
        field_0x264[i].set(0.0f, 0.0f, 0.0f);
    }
    field_0x304 = 0;
    field_0x30c = 0;
    field_0x310 = 1;
    field_0x238 = field_0x90 & 0x1F;
    field_0x23c = (field_0x90 >> 17) & 0x1F;
    field_0x240 = (field_0x90 >> 22) & 0x1F;
    field_0x234 = (field_0x90 >> 12) & 0x1F;
    u32 flags = (field_0x90 >> 7) & 0x1F;
    if (flags & 1) {
        mPos.x += 8.0f;
    }
    if (flags & 2) {
        mPos.y += 8.0f;
    }
    getGameView_802D6A9C()->fn_802D752C(true);
    getGameView_802D6A9C()->fn_802D7440(false);
    fn_8022ADE4();
    s32 total = 0;
    for (s32 p = 0; p < 4; p++) {
        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << p)) != 0) {
            total += fn_80236228(p);
        }
    }
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x31c = 1;
    while (total >= 1000) {
        total -= 1000;
        field_0x314++;
    }
}

// 0x802A64E8.
cls_804A7EB0::~cls_804A7EB0() {
    for (s32 i = 0; i < 4; i++) {
        deleteEmitterKeep(&mEmitters[0][i]);
        deleteEmitterKeep(&mEmitters[1][i]);
        deleteEmitterKeep(&mEmitters[2][i]);
        deleteEmitterKeep(&mEmitters[3][i]);
        deleteEmitterKeep(&mEmitters[4][i]);
    }
}

// 0x802A4654.
// FIXME(shotgun): 90.76 %, different
void cls_804A7EB0::vfunc_0x54() {
    f32 x = mPos.x;
    f32 y = mPos.y;
    f32 z = mPos.z;
    for (s32 i = 0; i < 4; i++) {
        mPos.x = x + (f32)lbl_8046C658[field_0x234 * 4 + i];
        mPos.y = y + (f32)lbl_8046C6C8[field_0x234 * 4 + i];
        startSound_801F0E34(0x30084, 0);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
}
