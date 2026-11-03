#ifndef GAME_ACT_ACT_EYEBALL_SWITCH_H
#define GAME_ACT_ACT_EYEBALL_SWITCH_H

// Code: game/act_eyeball_switch.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Named after its vtable, 0x804AE678 (still lbl_804AE678 in symbols.txt).
class cls_804AE678 : public ActorBase_804A5770 {
public:
    /* 0x24 */ virtual void vfunc_0x24(); // deletes the emitter and dies

    /* 0x230 */ JPABaseEmitter* mEmitter;
}; // Size: not yet known

#endif
