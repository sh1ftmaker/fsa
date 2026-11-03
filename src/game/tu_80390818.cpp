#include "game/GameAudio.h"
#include "game/Message.h"

u8 MessageWindow_804AF490::fn_80390BA8() {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x19e;
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x19e;
    }
    return 0xFF;
}

u8 MessageWindow_804AF490::fn_80390894(s32 index) {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x3b2[index];
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x3b2[index];
    }
    return 0;
}

s32 MessageWindow_804AF490::fn_80390C38() {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x148;
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x148;
    }
    return -1;
}
