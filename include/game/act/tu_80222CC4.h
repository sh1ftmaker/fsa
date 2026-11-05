#ifndef GAME_ACT_TU_80222CC4_H
#define GAME_ACT_TU_80222CC4_H

#include "game/cls_804A6A00.h"

// A minor Shadow Link actor (sub-base ShadowLinkBase_804A6A00) that spawns a 'DDGO'
// actor and releases it again; code in game/tu_80222CC4.cpp. Not tied to a
// spawner ID. Named after its vtable (0x804A6C38). The constructor is
// fn_8022313C.
class cls_804A6C38 : public ShadowLinkBase_804A6A00 {
public:
    cls_804A6C38();
    virtual ~cls_804A6C38();
    /* 0x0C */ virtual void init();
    // Releases the 'DDGO' actor after the base's work.
    /* 0x6C */ virtual void vfunc_0x6C();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);

    // The 'DDGO' actor spawned in state 5 (-1 when none): its serial.
    /* 0x2B8 */ s32 mDDGOSerial;
}; // Size: 0x2BC

#endif
