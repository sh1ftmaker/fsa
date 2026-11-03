#ifndef GAME_CLS_80217128_H
#define GAME_CLS_80217128_H

#include "types.h"
#include "JSystem/JGeometry.h"

class ShadowLink2_804A43E8;

// A 0x18-byte attack hit box, defined in game/tu_8021693C.cpp. No vtable;
// named after its constructor (fn_80217128). Embedded in the shadow Link
// (act_shadow_link_2, +0x528), which points mOwner back at itself; its update
// (fn_80216A8C) places the box in front of Link's sword swing from the
// animation sequencer at the owner's +0x340, and fn_8021693C asks which
// players the box overlaps.
class cls_80217128 {
public:
    // Empty box (1, 1)-(-1, -1), no owner, type 0.
    cls_80217128();
    // Which of the players in `mask` (bit n = player n) have a box on the
    // owner's layer that the hit box overlaps; the bits that do. 0 for an
    // empty mask or an empty box.
    u8 fn_8021693C(u8 mask);
    // Places the box for the owner's current animation: empty unless the
    // sequencer is in state 2, 3 or 6 far enough into the swing; mType is
    // the swing direction (0 right, 1 left, 2 down, 3 up).
    void fn_80216A8C();

    /* 0x00 */ ShadowLink2_804A43E8* mOwner;
    /* 0x04 */ JGeometry::TBox2f mBox; // relative to the owner's position
    /* 0x14 */ s32 mType;
}; // Size: 0x18

#endif
