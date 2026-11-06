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
};

#endif
