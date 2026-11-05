#ifndef GAME_CLS_804B1600_H
#define GAME_CLS_804B1600_H

#include "types.h"

class J2DScreen;
class cls_804AF86C;

// Named after its vtable, 0x804B1600 (one virtual, the destructor fn_8042EEC8).
// Defined in game/tu_8042E964.cpp. cls_804B1634's constructor (fn_80431D84)
// allocates it (0x14 bytes, inline constructor) and keeps it in its field_0x4;
// it stores `this` in lbl_8053AEC4. A window that slides in and out:
// field_0x4 is its J2DScreen, field_0x8 and field_0xc the two moving panes,
// field_0x10 the frame counter and field_0x12 the "open" flag.
class cls_804B1600 {
public:
    /* 0x08 */ virtual ~cls_804B1600();

    // Owner: game/tu_8042E964.cpp. Resets the counter and the flag.
    void fn_8042EB14();
    void fn_8042EB24();
    void fn_8042EB98();
    BOOL fn_8042EBEC();
    BOOL fn_8042EC7C();
    bool fn_8042EDAC(u32 message, u8 value);

    /* 0x04 */ J2DScreen* field_0x4;
    /* 0x08 */ cls_804AF86C* field_0x8;
    /* 0x0C */ cls_804AF86C* field_0xc;
    /* 0x10 */ s16 field_0x10;
    /* 0x12 */ u8 field_0x12;
}; // Size: 0x14

// .sbss of game/tu_8042E964.cpp (the constructor sets it).
extern cls_804B1600* lbl_8053AEC4;

#endif
