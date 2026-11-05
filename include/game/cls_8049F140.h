#ifndef GAME_CLS_8049F140_H
#define GAME_CLS_8049F140_H

#include "types.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Vtable 0x8049F140: the 'PEFF' spawner (game/tu_8013C5C8.cpp). execute,
// every 8 frames while the stage allows it, spawns a 'PEFF' actor at player
// field_0x4's position; field_0x8 (0 or 1) picks the spawn parameter,
// field_0xc counts the frames. Player_804A6F38 holds two (0x1398, 0x13A8);
// its constructor and destructor inline the constructor (both vtables
// stored, field_0xc zeroed) and the destructor (vtable stored, then
// __dt__18JPAEmitterCallBackFv), so both are inline here.
class cls_8049F140 : public JPAEmitterCallBack {
public:
    cls_8049F140() : field_0xc(0) {}
    virtual ~cls_8049F140() {}
    virtual void execute(JPABaseEmitter* emitter);

    /* 0x4 */ s32 field_0x4;
    /* 0x8 */ s32 field_0x8;
    /* 0xC */ s32 field_0xc;
}; // Size: 0x10

#endif
