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
    // 0x8026F9E8 (tu_802684A4): r4 = the owning Player.
    cls_804A71A8(Player_804A6F38* player);

    // 0x8026F998: r4 = the delete flag; > 0 frees the object (__dl__FPv).
    virtual ~cls_804A71A8();

    // tu_802684A4: copies the owner's mPos into *pos, then, while the owner
    // is in state 2, offsets x and y by the corner table entry for its
    // fn_80248778 index (y's sign follows viewLayerFlag of its layer).
    void fn_8026947C(JGeometry::TVec3f* pos);
    // 0x80268BC4 (not written yet): called by the owner's fn_802685D4 each
    // frame; reads and shifts the bytes at 0x30-0x4C.
    void fn_80268BC4();
    // tu_8026942C area (not written yet); the owner's fn_8024B274 / fn_8024B298 forward to them.
    void fn_80269420();
    void fn_8026942C(s32 index, JGeometry::TVec3f* out);
    // 0x8026F710 (tu_802684A4, not written yet): clears the byte flags at
    // 0x30-0x37, 0x48-0x4F, 0xB4 and 0xB6; called by the owner's reset fn_80254744.
    void fn_8026F710();

    /* 0x4 */ Player_804A6F38* field_0x4;
    /* 0x8 */ u8 field_0x8[0x48 - 0x8]; // not yet known
    // Bytes tested for 1 by the owner's fn_802517F8 (tu_8023FBF0).
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    // Bytes tested for 1 by the owner's fn_802518AC / fn_8025190C (tu_8023FBF0).
    /* 0x4A */ u8 field_0x4a;
    /* 0x4B */ u8 field_0x4b;
    /* 0x4C */ u8 field_0x4c;
    /* 0x4D */ u8 field_0x4d;
    /* 0x4E */ u8 field_0x4e;
    /* 0x4F */ u8 field_0x4f;
    /* 0x50 */ u8 field_0x50[0x98 - 0x50]; // not yet known
    // The owner's fn_8024B24C is true when either is set.
    /* 0x98 */ u8 field_0x98;
    /* 0x99 */ u8 field_0x99;
    /* 0x9A */ u8 field_0x9a[0xA0 - 0x9A]; // not yet known
    // Tested for 0 by the owner's fn_8027EEB4 (tu_8027E5BC) before it resets
    // the walk-animation counter.
    /* 0xA0 */ s32 field_0xa0;
    /* 0xA4 */ u8 field_0xa4[0xB4 - 0xA4]; // not yet known
    // Returned by the owner's fn_8024F630 (lbz; blr).
    /* 0xB4 */ bool field_0xb4;
    /* 0xB5 */ u8 field_0xb5[0xB7 - 0xB5]; // not yet known
    // Byte the owner's fn_8027EEB4 (tu_8027E5BC) tests for 0.
    /* 0xB7 */ u8 field_0xb7;
    /* 0xB8 */ u8 field_0xb8[0xC0 - 0xB8]; // not yet known
}; // Size: 0xC0

#endif
