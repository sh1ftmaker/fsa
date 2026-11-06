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

// FIXME: 96.17. Structurally close: the target reads field_0x234 before the getGameView
// call of the tile lookup (field_0x238 after it, as if the column came through an inlined
// accessor) and copies the tile byte from r0 to r9 after the 0xFF test; the format string
// waits for the unit's .rodata split (lbl_8046E9F8). The archive type is 'B   ' plus the
// stage name's chars less ' ', added after the sprintf (GameView fn_802D8328's form).
void cls_804A9BC8::init() {
    for (int i = 0; i < 0x50; i++) {
        lbl_804D4768[i] = -1;
    }
    lbl_8053ACB0 = 0;
    field_0x234 = (field_0x90 >> 23) & 0xF;
    field_0x238 = (field_0x90 >> 19) & 0xF;

    s32 stage = getGameView_802D6A9C()->field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    const char* name = getGameView_802D6A9C()->fn_802D9C7C(stage, view->field_0x8);
    s32 tile = getGameView_802D6A9C()->field_0x60[field_0x238][field_0x234];
    if (tile == 0xFF) {
        tile = 0;
    }
    char buf[0x20];
    u32 type = 'B   ';
    sprintf(buf, "d_enemy_map%s_%d%d.bin", name, tile / 10, tile % 10);
    type += ((name[0] - ' ') << 16) + ((name[1] - ' ') << 8) + (name[2] - ' ');
    field_0x244 = (u8*)fn_804032E8(type, buf);
    field_0x248 = 0;
    field_0x23c = (field_0x90 >> 16) & 7;
    field_0x24c = 0;
    field_0x24e = 8;
    field_0x240 = 0;
    field_0x230 = 2;
}

void cls_804A9BC8::draw() {}

cls_804A9BC8::~cls_804A9BC8() {}
