#ifndef GAME_GAMEAUDIO_H
#define GAME_GAMEAUDIO_H

// The game's shared sound header. Nearly every game unit (about 523) has the
// same __sinit, which constructs nine JAudio memory pools. Include this
// header in every game unit; its includes and declarations instantiate
// exactly those pools in FSA's order.
//
// MWCC emits the pool constructors grouped by template (the _MultiThreaded
// pools first), and each group in reverse order of instantiation (checked
// with test compiles, progress/game__tu_802BA334.md). The plain pools must
// therefore be instantiated in this order: JAISe, JAISeq, JAISoundChild,
// JAIStream, cls_804B21EC, JAUDopplerAudible<4>. Their .bss copies sit in
// the reverse order: 0x804B21DC (Doppler) .. 0x804B222C (JAISe).
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAISoundChild.h"
#include "JSystem/JAudio/Interface/JAIStreamMgr.h"
#include "JSystem/JAudio/JAUAudience.h"

// A pool-allocated class whose JASPoolAllocObject<>::memPool_ is at
// 0x804B21EC (guard 0x8053A2B0, JASMemPool dtor 0x80021BBC). Nothing in the
// DOL allocates one, so its members and size are unknown. Named after its
// memPool_.
//
// It is probably JAUAudible<4>, but that is not proven, so the name stays.
// - JSystem's JAUAudioMgr.cpp builds this pool but not the
//   JAUDopplerAudible<4> one. mkdd's JAUAudioMgr has a JAUAudience<4>
//   (non-Doppler) member, whose Audible is JAUAudible<4>.
// - JAUAudience.cpp builds both pools.
// - mkdd's symbol map has exactly two JAU pools in game files:
//   JAUAudible<4> and JAUDopplerAudible<4>.
// No construct in our JAudio headers makes JAUAudible<4> complete in
// JAUAudioMgr.cpp, which is why it can't be shown yet.
class cls_804B21EC : public JASPoolAllocObject<cls_804B21EC> {};

// FIXME: stand-in. Something in FSA's real game sound header makes
// JAUDopplerAudible<4> a complete type after cls_804B21EC, so every game
// unit instantiates its pool. Only that instantiation matters here; this
// function is never called.
// The likely source is the game's audience class, built in
// game/tu_80457DFC.cpp: it derives from JAUDopplerAudience<4> (its vtable
// is at 0x804B2074, dtor fn_8045A338, and it overrides slot 6 with
// fn_8045A3D8). If that class's header has an inline member that touches
// JAUAudience_::isActive(), deleteAudible() or new, that member does the
// instantiation. In a test, an inline that calls isActive() reproduces it.
// Which member it really is stays unknown until tu_80457DFC is decompiled;
// replace this stand-in with that class.
inline void unk_deleteAudible(JAUDopplerAudible<4>* audible) {
    delete audible;
}

#endif
