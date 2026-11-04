#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/tu_8030AE84.h"

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
