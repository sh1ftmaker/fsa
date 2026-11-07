#ifndef GAME_ACT_TU_801688AC_H
#define GAME_ACT_TU_801688AC_H

// Code: game/tu_801688AC.cpp.

#include "game/ActorBase.h"
#include "game/act/tu_801F9484_elems.h"

// Named after its vtable, 0x804A0738.
class cls_804A0738 : public ActorBase_804A5770 {
public:
    cls_804A0738() {}
    /* 0x08 */ virtual ~cls_804A0738();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x24];
    /* 0x254 */ s32 field_0x254; // set to 1 by fn_801CC538 (tu_801CB270)
    /* 0x258 */ u8 field_0x258[0x1C];
    /* 0x274 */ Elem_80203594 field_0x274[5];
    /* 0x2EC */ Elem_80203594 field_0x2ec[5];
    /* 0x364 */ u8 field_0x364[0x1C];
}; // Size: 0x380

#endif
