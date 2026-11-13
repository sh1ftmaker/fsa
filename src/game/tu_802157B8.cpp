#include "game/GameAudio.h"


#include "game/cls_802157B8.h"

void cls_802157B8::fn_80215864() {
    field_0x0 = 0;
}

void cls_802157B8::fn_802157B8(u8 arg) {
    field_0x0 |= 0x48;
    if (arg >= 5) {
        field_0x0 |= 0x10;
        if (arg >= 6) {
            field_0x0 |= 0x20;
        }
    }

    switch (arg) {
    case 1:
        field_0x0 |= 0x1100;
        break;
    case 3:
        field_0x0 |= 0x404;
        break;
    case 5:
        field_0x0 |= 0xB00;
        break;
    case 6:
        field_0x0 |= 0x404;
        break;
    case 7:
        field_0x0 |= 0x7;
        break;
    }
}
