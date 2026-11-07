#ifndef GAME_CLS_804B1B60_H
#define GAME_CLS_804B1B60_H

#include "types.h"

// Vtable 0x804B1B60, defined in game/tu_80443B0C.cpp (not written yet). Its
// constructor fn_80444B68 stores the vtable, makes a J2DScreen and PaneCtrl
// panes; the map screen (MapScreen_804B1288, game/tu_80403D84.cpp) allocates it with
// __nw__(0x50) and keeps it in field_0x4.
class cls_804B1B60 {
public:
    /* 0x08 */ virtual ~cls_804B1B60();

    // 0x804446F0: stores r4 (a word) at 0x44 and sets up the text of the
    // text box 'Tmm01'. The map screen's fn_8040E298 passes 0.
    void fn_804446F0(s32 arg);
    // 0x804446A4: runs the state function lbl_804B1B30[field_0x4d] (__ptmf_scall)
    // unless the state is 4; true when it is 4 (the map screen's fn_8040B93C waits on it).
    bool fn_804446A4();
    // 0x80444650: unless the state (field_0x4d) is 4, draws its J2DScreen (0x4)
    // (the map screen's fn_80411230 calls it in state 0x10).
    void fn_80444650();

    /* 0x00 */ // vtable
    /* 0x04 */ u8 field_0x4[0x4C - 0x4];
    /* 0x4C */ u8 field_0x4c; // fn_8040B93C (MapScreen_804B1288) tests it
    /* 0x4D */ u8 field_0x4d; // state (fn_804446A4)
    /* 0x4E */ u8 field_0x4e[0x50 - 0x4E];
}; // Size: 0x50 (__nw__(0x50) in MapScreen_804B1288's constructor)

#endif
