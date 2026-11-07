#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"

// Offsets of the four ghost positions from mPos (x, y pairs), .data at 0x804A0410.
static const f32 lbl_804A0410[8] = {-80.0f, -40.0f, 80.0f, -40.0f, -80.0f, 40.0f, 80.0f, 40.0f};

// FIXME(shotgun): table not written out (gate.py literal tripwire); 8 colour sets of three
// RGBA colours (96 bytes), then the three texture names; extern-declared, size from symbols.txt.
extern const JUtility::TColor lbl_804661E0[24];

// A flag of the ghost's attacks, .sbss at 0x8053AB58.
s32 lbl_8053AB58;

// 0x80164118.
cls_804A0440::~cls_804A0440() {
    releaseEmitter_801EE434(field_0x2e8);
    if (field_0x268 == 2) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
}

// 0x80163E80.
// FIXME(shotgun): 72.84 %, missing/extra + reloc-name
void cls_804A0440::init() {
    field_0x94 = 0;
    mBox.set(-60.0f, -60.0f, 60.0f, 60.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x258 = 0;
    field_0x25c = 5;
    field_0x27c = 1.0f;
    field_0x280 = 1.0f;
    field_0x284 = 0.4f;
    field_0x274 = 0.0f;
    field_0x278 = 0.05f;
    field_0x254 = 0;
    field_0x294[8] = 0.0f;
    field_0x294[7] = 0.0f;
    field_0x294[6] = 0.0f;
    field_0x294[0] = mPos.x;
    field_0x294[1] = mPos.y;
    field_0x294[2] = mPos.z;
    field_0x294[3] = mPos.x;
    field_0x294[4] = mPos.y;
    field_0x294[5] = mPos.z;
    field_0x260 = 0;
    field_0x288 = 0.0f;
    field_0x28c = 0.0f;
    lbl_8053AB58 = 0;
    field_0x2e8 = NULL;
    field_0x2b8[0] = mPos;
    field_0x2b8[0].x += lbl_804A0410[0];
    field_0x2b8[0].y += lbl_804A0410[1];
    field_0x2b8[1] = mPos;
    field_0x2b8[1].x += lbl_804A0410[2];
    field_0x2b8[1].y += lbl_804A0410[3];
    field_0x2b8[2] = mPos;
    field_0x2b8[2].x += lbl_804A0410[4];
    field_0x2b8[2].y += lbl_804A0410[5];
    field_0x2b8[3] = mPos;
    field_0x2b8[3].x += lbl_804A0410[6];
    field_0x2b8[3].y += lbl_804A0410[7];
    field_0x2ec = -1;
    field_0x2f0 = -1;
    field_0x2f4[0].set(0);
    field_0x2f4[1].set(0);
    field_0x2f4[2].set(0);
    field_0x26c = -1;
    field_0x290 = 0;
    field_0x270 = (field_0x90 >> 2) & 0x1F;
    field_0x196 = 1;
    field_0x268 = field_0x90 & 3;
    mLife = 10000;
    switch (field_0x268) {
    case 0:
        getGameView_802D6A9C()->fn_802D7440(false);
        field_0x230 = 0;
        break;
    case 1:
        field_0x290 = 0x8000;
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        field_0x288 = 1.0f;
        field_0x230 = 12;
        break;
    case 2:
        field_0x230 = 14;
        break;
    }
}

// 0x80161F98.
// FIXME(shotgun): 62.92 %, missing/extra
void cls_804A0440::fn_80161F98(s32 set, s32 frames) {
    if (field_0x2ec != set) {
        if (field_0x2ec < 0) {
            const JUtility::TColor* row = lbl_804661E0 + set * 3;
            field_0x2f4[0] = row[0];
            field_0x2f4[1] = row[1];
            field_0x2f4[2] = row[2];
        }
        field_0x2ec = set;
        if (field_0x2f0 < 0) {
            field_0x2f0 = 0;
        } else {
            field_0x2f0 = frames;
        }
    } else if (field_0x2f0 > 0) {
        f32 weight = 1.0f / field_0x2f0;
        for (s32 i = 0; i < 3; i++) {
            JUtility::TColor& c = field_0x2f4[i];
            const JUtility::TColor& target = lbl_804661E0[set * 3 + i];
            c.r = (u8)((f32)c.r * (1.0f - weight) + weight * (f32)target.r);
            c.g = (u8)((f32)c.g * (1.0f - weight) + weight * (f32)target.g);
            c.b = (u8)((f32)c.b * (1.0f - weight) + weight * (f32)target.b);
            c.a = (u8)((f32)c.a * (1.0f - weight) + weight * (f32)target.a);
        }
        field_0x2f0--;
    }
}

// 0x80161F20.
// FIXME(shotgun): 99.67 %, reloc-name
void cls_804A0440::vfunc_0x24() {
    startSound_801F0E34(0x40015, 0);
    vfunc_0x28(7, 0.0f, 16.0f);
    s32 bit = field_0x270;
    if (bit != 0) {
        getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
    }
}
