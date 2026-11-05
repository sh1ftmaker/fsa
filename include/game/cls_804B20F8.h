#ifndef GAME_CLS_804B20F8_H
#define GAME_CLS_804B20F8_H

#include "game/ActorManager.h"
#include "JSystem/JAudio/Interface/JAISound.h"

// A sound object with its own startSound (vtable 0x804B20F8, defined in
// game/tu_8045B010.cpp). Built on ActorSoundObj_804578F4: the constructor (0x8045B46C) calls
// the base one and then stores this vtable at 0x8 and vtable + 0x24 at
// 0x18. Every slot but startSound (0x14) is inherited; the destructor
// (__dt__12cls_804B20F8Fv, weak, kept by game/tu_8023FBF0.cpp) stores the
// vtable again.
class cls_804B20F8 : public ActorSoundObj_804578F4 {
public:
    // 0x8045B46C: r4 = the byte stored at 0x5C (signed: every caller passes
    // it through extsb), r5-r7 = the base
    // constructor's position, heap and handle count.
    cls_804B20F8(s8 kind, JGeometry::TVec3f* pos, JKRHeap* heap, u8 numHandles);

    // 0x8045B154: stops or retargets some sounds before starting `id`, then
    // starts it and detunes its pitch by the kind in field_0x5c.
    virtual JAISoundHandle* startSound(JAISoundID id);

    /* 0x5C */ s8 field_0x5c; // pitch kind: < 0 or >= 4 leaves the pitch alone
}; // Size: 0x60

// Plays entry `index` of lbl_8052DEE0 on `obj`, then writes `port9` to port
// 9 of the started sound's track when it is non-zero. Called by
// tu_803944C8, tu_803A8DFC, tu_803B7DCC, tu_803E1668, tu_803F0A54 and
// tu_80403D84 (not written yet).
void fn_8045B030(ActorSoundObj_804578F4* obj, u8 index, s8 port9);

// Sound IDs indexed by `index` above; defined in game/tu_8045B6F0.cpp.
extern JAISoundID lbl_8052DEE0[];

#endif
