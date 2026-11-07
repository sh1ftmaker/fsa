#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_8034AA40.h"

// .rodata 0x804720E8: the start timers (state kinds 0, 1 use words 0 and the .sdata2 pair
// {20, 15}), the frame indices by facing and sub-state (16 words), and the frames of kind 1 (8).
static const s32 lbl_804720E8[28] = {10, 7, 10, 7, 1, 0, 2, 0, 4, 3, 5, 3, 7, 6, 8, 6, 10, 9, 11, 9, 0, 1, 2, 3, 4, 5, 6, 7};

// 0x8034C464.
// FIXME(shotgun): 96.25 %, missing/extra + reloc-name
void cls_804AC8C0::init() {
    field_0x230 = 0;
    field_0xb0 = 0x1000;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    mLife = 10000;
    field_0x238 = 0;
    field_0x234 = 0;
    field_0x2c0 = 0;
    field_0x244 = field_0x90 & 0xF;
    field_0x248 = 0;
    field_0x24c = (field_0x90 >> 4) & 0xFF;
    field_0x270 = 0;
    field_0x250 = 60;
    field_0x254 = -1;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x28f = 0;
    field_0x28c[0] = 0;
    field_0x28c[1] = 0;
    field_0x28c[2] = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x274[i] = 0;
        field_0x284[i] = 0;
        field_0x260[i] = -1;
        field_0x288[i] = 0;
        field_0x290[i] = *getPlayerPos_8023E724(i);
    }
    mPos.x = 16.0f * (f32)((s32)mPos.x / 16) + 8.0f;
    mPos.y = 16.0f * (f32)((s32)mPos.y / 16) + 8.0f;
    JGeometry::TVec3f dir;
    f32 dist;
    fn_801EE558(&dir, &dist, 0);
    if (dist < 100.0f) {
        u16 angle = (s16)(JMath::atanTable_.atan2_(-dir.x, -dir.y) * 10430.378f);
        if (angle < 0x2000 || angle >= 0xE000) {
            field_0x23c = 3;
        } else if (angle < 0x6000) {
            field_0x23c = 1;
        } else if (angle <= 0xA000) {
            field_0x23c = 2;
        } else {
            field_0x23c = 0;
        }
    } else if (field_0x230 == 0xD) {
        field_0x23c = 1;
    } else {
        field_0x23c = 2;
    }
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    switch (field_0x244) {
    case 0:
        field_0x234 = lbl_804720E8[0];
        field_0x2c0 = 0x423;
        break;
    case 1:
        field_0x234 = 20;
        field_0x2c0 = 0x427;
        break;
    }
    if (vfunc_0x34() == true) {
        field_0x230 = 0xB;
    }
}

// 0x8034AFE8.
// FIXME(shotgun): 82.03 %, missing/extra + reloc-name
void cls_804AC8C0::draw() {
    const char* name;
    s32 frame;
    switch (field_0x244) {
    case 0:
        name = "enemy_both2_sob.bin";
        frame = lbl_804720E8[4 + field_0x23c * 4 + field_0x238] + 0x218;
        break;
    case 1:
        name = "enemy_gc_sob.bin";
        frame = lbl_804720E8[20 + field_0x23c * 2 + field_0x238] + 0x2E6;
        break;
    }
    drawSprite_801F06F0(name, frame, NULL, 0, 1.0f, 1.0f);
}

// Destructor at 0x8034AA40.
cls_804AC8C0::~cls_804AC8C0() {}
