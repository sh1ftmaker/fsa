#ifndef GAME_CLS_804B146C_H
#define GAME_CLS_804B146C_H

#include "types.h"

// Named after its vtable, 0x804B146C (stored by its destructor,
// fn_80426AD4 in game/tu_8042165C.cpp; the constructor is fn_804257E4).
// The object cls_804B1920 keeps at 0x294. Only the members that
// cls_804B1920 calls are declared.
class cls_804B146C {
public:
    // Owner: game/tu_8042165C.cpp (0x80425128). Called with only the object.
    void fn_80425128();
    // Owner: game/tu_8042165C.cpp (0x80425234). Called with the three
    // arguments cls_804B1920::fn_8043D874 received.
    void fn_80425234(s32 id, s32 arg1, u32 arg2);
};

#endif
