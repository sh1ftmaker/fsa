#ifndef GAME_CLS_804A71A8_H
#define GAME_CLS_804A71A8_H

#include "game/Player.h"

// A heap object of 0xC0 bytes owned by a Player (vtable 0x804A71A8, defined
// in game/tu_802684A4.cpp). Its constructor (0x8026F9E8, not written yet)
// stores the Player at 0x4; the destructor clears it. The Player's
// constructor (game/tu_8023FBF0.cpp) allocates it and keeps it in
// field_0xbf0.
class cls_804A71A8 {
public:
    // 0x8026F9E8: r4 = the owning Player.
    cls_804A71A8(Player_804A6F38* player);

    // 0x8026F998: r4 = the delete flag; > 0 frees the object (__dl__FPv).
    virtual ~cls_804A71A8();

    /* 0x4 */ Player_804A6F38* field_0x4;
};

#endif
