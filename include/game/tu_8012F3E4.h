#ifndef GAME_TU_8012F3E4_H
#define GAME_TU_8012F3E4_H

// Exports of game/tu_8012F3E4.cpp (owner unit, not written yet). Called by
// tu_8012F8AC, act_ice_floor_check_npc and tu_802D39B8.

// The guarded-static object at 0x804BDDFC: an s32 count and 32 s32 slots, all
// -1 after construction.
// No vtable; the constructor is inlined in fn_8012F554. Named after its
// destructor fn_8012F64C, which is passed to __register_global_object (0x8012F628).
struct cls_8012F64C;
class ActorBase_804A5770;

extern "C" {
// Getter of the guarded static at 0x804BDDFC (returns its address).
cls_8012F64C* fn_8012F554();
// Appends the actor's serial (mSerial) to the object's slots (owner unit:
// game/tu_8012F3E4.cpp; called by act_ice_floor_check_npc's init).
void fn_8012F530(cls_8012F64C*, ActorBase_804A5770*);
// True once every slot of the object passes (checked through fn_801F666C and
// fn_8012F008).
bool fn_8012F3E4(cls_8012F64C*);
}

#endif
