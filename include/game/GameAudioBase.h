#ifndef GAME_GAMEAUDIOBASE_H
#define GAME_GAMEAUDIOBASE_H

// The game's base sound header: the JAudio includes and the JAUAudible<4>
// pool that give a game unit's __sinit its eight pools without
// JAUDopplerAudible<4>. tu_80457C08 and tu_8045B6F0 have exactly those
// eight. Every other game unit has nine and includes game/GameAudio.h,
// which includes this file.
//
// Include it before any other JAudio header. Its include order fixes the
// pool order: with JAIStreamMgr.h included first, for example, the JAIStream
// pool moves from 6th to last (shown in the v2 review of the shared
// headers). Including the other game/ headers first is harmless.
//
// MWCC emits the pool constructors grouped by template (the _MultiThreaded
// pools first), and each group in reverse order of instantiation. Game-unit
// workers checked this with test compiles; see the hdr/actor reviews. So
// the plain pools are instantiated in this order: JAISe, JAISeq,
// JAISoundChild, JAIStream, JAUAudible<4> (and in GameAudio.h,
// JAUDopplerAudible<4>). Their .bss copies sit in reverse order, from
// 0x804B21DC (Doppler) to 0x804B222C (JAISe).
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAISoundChild.h"
#include "JSystem/JAudio/Interface/JAIStreamMgr.h"
#include "JSystem/JAudio/JAUAudience.h"

// FIXME: stand-in. Do not call it, and do not copy it into a .cpp.
// Something in FSA's real game sound header makes JAUAudible<4> a complete
// type here, so every game unit instantiates its pool (memPool_ at
// 0x804B21EC, guard 0x8053A2B0, JASMemPool dtor 0x80021BBC). Only that
// instantiation matters: the function is never called and emits nothing.
// The pool is JAUAudible<4>'s: JSystem's JAUAudioMgr.cpp, compiled from
// mkdd's source (JAUAudience<4> member, so JAUAudible<4>), builds exactly
// FSA's seven pools with this one fourth, all at FSA's addresses, and links
// byte-identical under that name. Replace this with the real construct
// (likely the game's audience class, see GameAudio.h) once it is known.
inline void FIXME_instantiateAudiblePool(JAUAudible<4>* audible) {
    delete audible;
}

#endif
