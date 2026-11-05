#ifndef GAME_CLS_804B19F0_H
#define GAME_CLS_804B19F0_H

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

// Named after its vtable, 0x804B19F0 (one virtual, the destructor).
// Defined in game/tu_80441A1C.cpp; the constructor stores `this` in a
// file-static pointer of that unit. The state machine is fn_80441A1C.
class cls_804B19F0 {
public:
    cls_804B19F0();
    /* 0x08 */ virtual ~cls_804B19F0();

    // Owner: game/tu_80441A1C.cpp. Draws field_0x4 at the origin with the
    // ortho context of gGameRoot2_8053AB10, when field_0x18 is not 0.
    void fn_80442200();
    // Owner: game/tu_80441A1C.cpp. Resets the pane and counters for a new
    // text window: field_0x10 = arg, field_0x14 = index - 1, field_0x18 = 1.
    void fn_80442254(s32 arg, s32 index);
    // Owner: game/tu_80441A1C.cpp (0x80441A1C). The state machine on field_0x18
    // (cases 1-6, 11, 13; the rest return 0x16). Returns 1 at the end of
    // state 13, field_0x10 when state 11 has run out, else 0x16.
    s32 fn_80441A1C();

    /* 0x04 */ J2DScreen* field_0x4;     // "gba_connect_error_window.blo", new in the constructor
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8;  // pane wrapper, tag "Pwblk01"
    /* 0x0C */ PaneCtrl_804AF86C* field_0xc;  // pane wrapper, tag "Ndatag01"
    /* 0x10 */ s32 field_0x10;           // 0x16 at construction
    /* 0x14 */ s32 field_0x14;           // player index (0-based)
    /* 0x18 */ s32 field_0x18;           // state; fn_80441A1C switches on it
    /* 0x1C */ s32 field_0x1c;           // frame counter
    /* 0x20 */ u8 field_0x20[0x28 - 0x20];
}; // Size: 0x28 (GameRoot_8049EF20's constructor, tu_80134C04: __nw__(0x28))

// .sbss pointer the constructor stores `this` in (0x8053AEE0, inside
// game/tu_8043A0E8.cpp's .sbss range); GameRoot's fn_80136828 / fn_80137300
// call fn_80442200 through it.
extern cls_804B19F0* lbl_8053AEE0;

#endif
