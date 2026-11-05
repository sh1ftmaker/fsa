#ifndef GAME_CLS_804AF878_H
#define GAME_CLS_804AF878_H

#include "types.h"

class cls_804B0B00;
class ConnectScreen_804B0F10;

// Vtable 0x804AF878 (one slot, the destructor fn_803A87A8), defined in
// game/tu_803A85E4.cpp. tu_80134C04 (not written yet) allocates it with
// __nw__(0x10, heap, 0) and the constructor fn_803A8884 (0x80135264-0x80135270)
// and stores it at GameRoot + 0x2C. The constructor stores `this` in
// lbl_8053AD98. Two objects of the same kind, one for the single-player
// layout (field_0x4, used while lbl_8053AAF8->field_0x7f is set) and one for
// the other (field_0x8); field_0xc mirrors the state one of them reports.
class cls_804AF878 {
public:
    cls_804AF878();
    /* 0x08 */ virtual ~cls_804AF878();
    // tu_80134C04 (0x801378BC, 0x801378C8): fn_803A86A4 once when its state
    // word 0x60 is 0, then fn_803A8600 each frame.
    void fn_803A8600();
    // tu_80134C04 (0x801368DC).
    void fn_803A8664();
    void fn_803A86A4();

    /* 0x04 */ cls_804B0B00* field_0x4;
    /* 0x08 */ ConnectScreen_804B0F10* field_0x8;
    /* 0x0C */ s32 field_0xc;
}; // Size: 0x10

// The state word of lbl_8053AD98's object, 4 when there is no object.
// tu_80134C04 calls it with no arguments (0x801378D4-0x80137920).
s32 fn_803A85E4();

// Data of game/tu_803A85E4.cpp.
extern cls_804AF878* lbl_8053AD98;

#endif
