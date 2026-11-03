#ifndef GAME_TU_8012F3E4_H
#define GAME_TU_8012F3E4_H

// Exports of game/tu_8012F3E4.cpp (owner unit, not written yet). Called by
// tu_8012F8AC, act_ice_floor_check_npc and tu_802D39B8.

// The guarded-static object at 0x804BDDFC: an s32 count and 32 s32 slots, all
// -1 after construction.
struct Unk_804BDDFC;

extern "C" {
// Getter of the guarded static at 0x804BDDFC (returns its address).
Unk_804BDDFC* fn_8012F554();
// True once every slot of the object passes (checked through fn_801F666C and
// fn_8012F008).
bool fn_8012F3E4(Unk_804BDDFC*);
}

#endif
