#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/GameState.h"
#include "game/act/tu_802E2074.h"
#include <stdio.h>

// Actor serials, 0x50 slots of the 0x90-word table at 0x804D4768 (the rest
// of the 0x240-byte bss range is not touched by this unit).
s32 lbl_804D4768[0x90];
// Frame counter of the update; 0 after init.
s32 lbl_8053ACB0;
// Resource name format: "d_enemy_map<stage>_<tens><ones>.bin".
const char lbl_8046E9F8[] = "d_enemy_map%s_%d%d.bin";

cls_804A9BC8::~cls_804A9BC8() {}

// FIXME: 82.07 %. The original fills lbl_804D4768 with two passes of 40
// unrolled stores (ctr 2, stride 0xA0); this loop compiles to one store per
// pass. The archive type is built as 0x42202020 with the first two chars
// reduced by 0x20 (u32 'B' + name chars, not a multichar literal), and this,
// stage and view sit in r30/r29/r31 where the original has r29/r31/r30.
void cls_804A9BC8::init() {
    for (s32 i = 0; i < 0x50; i++) {
        lbl_804D4768[i] = -1;
    }
    lbl_8053ACB0 = 0;
    field_0x234 = (field_0x90 >> 23) & 0xF;
    field_0x238 = (field_0x90 >> 19) & 0xF;

    s32 stage = fn_802D6A9C()->field_0x4;
    GameView_804A9950* view = fn_802D6A9C();
    const char* name = fn_802D6A9C()->fn_802D9C7C(stage, view->field_0x8);
    s32 tile = fn_802D6A9C()->field_0x60[field_0x238][field_0x234];
    if (tile == 0xFF) {
        tile = 0;
    }
    char buf[0x20];
    sprintf(buf, lbl_8046E9F8, name, tile / 10, tile % 10);
    u32 type = 'B' << 24 | name[0] << 16 | name[1] << 8 | name[2];
    field_0x244 = (u8*)fn_804032E8(type, buf);
    field_0x248 = 0;
    field_0x23c = (field_0x90 >> 16) & 7;
    field_0x24c = 0;
    field_0x24e = 8;
    field_0x240 = 0;
    field_0x230 = 2;
}

void cls_804A9BC8::draw() {}
