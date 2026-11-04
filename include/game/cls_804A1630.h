#ifndef GAME_CLS_804A1630_H
#define GAME_CLS_804A1630_H

#include "game/ActorBase.h"

// An ActorBase-derived actor whose vtable is lbl_804A1630 (0x54 bytes, the 21
// base slots). Its constructor (fn_8020231C, not written yet) stores the vtable
// and builds its members; named after the vtable, like the other unnamed ones.
// Its destructor (game/tu_8017EF40.cpp) releases the two emitters at 0x318 and
// 0x31C and then runs the base destructor.
class cls_804A1630 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~cls_804A1630();
    // 0x8017EF40: starts sound 0x40038, then vfunc_0x28(2, 0.0f, -24.0f).
    /* 0x24 */ virtual void vfunc_0x24();

    /* 0x230 */ u8 field_0x230[0x318 - 0x230]; // not read by the destructor
    /* 0x318 */ JPABaseEmitter* mEmitter318[2];
};

#endif
