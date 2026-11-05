#ifndef GAME_GAMEAUDIO_H
#define GAME_GAMEAUDIO_H

// The game's sound header. 526 game units have the same __sinit, which
// constructs nine JAudio memory pools; those units include this header,
// and it instantiates exactly those pools in FSA's order. The two units
// with eight pools (no JAUDopplerAudible<4>), tu_80457C08 and tu_8045B6F0,
// include game/GameAudioBase.h instead. That header explains the order.
//
// Include it before any other JAudio header, or the pools reorder (see
// GameAudioBase.h).
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "game/GameAudioBase.h"
#include "JSystem/JAudio/JAUAudience.h"

// FIXME: stand-in. Do not call it, and do not copy it into a .cpp.
// Something in FSA's real game sound header makes JAUDopplerAudible<4> a
// complete type after JAUAudible<4>, so every one of these units
// instantiates its pool (memPool_ at 0x804B21DC). Only that instantiation
// matters: the function is never called and emits nothing.
// The likely source is the game's audience class, built in
// game/tu_80457DFC.cpp. It derives from JAUDopplerAudience<4> (vtable
// 0x804B2074, dtor fn_8045A338) and overrides slot 6 with fn_8045A3D8. In a
// test, an inline member that calls JAUAudience_::isActive() reproduces the
// instantiation. Replace this with that class once tu_80457DFC is
// decompiled, and check that the pool order still holds for units that
// include more JAU headers.
inline void FIXME_instantiateDopplerAudiblePool(JAUDopplerAudible<4>* audible) {
    delete audible;
}

// The game's audio manager (gAudioMgr_8053AF80).
#include "game/Unk_8045A080.h"

#endif
