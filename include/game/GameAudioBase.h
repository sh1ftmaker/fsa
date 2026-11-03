#ifndef GAME_GAMEAUDIOBASE_H
#define GAME_GAMEAUDIOBASE_H

// The game's base sound header: the JAudio includes and the pooled class
// cls_804B21EC that give a game unit's __sinit its eight pools without
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
// JAISoundChild, JAIStream, cls_804B21EC (and in GameAudio.h,
// JAUDopplerAudible<4>). Their .bss copies sit in reverse order, from
// 0x804B21DC (Doppler) to 0x804B222C (JAISe).
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAISoundChild.h"
#include "JSystem/JAudio/Interface/JAIStreamMgr.h"

// A pool-allocated class. Its JASPoolAllocObject<>::memPool_ is at
// 0x804B21EC (memPool___34JASPoolAllocObject<12cls_804B21EC>), its guard
// at 0x8053A2B0 and its JASMemPool dtor at 0x80021BBC. Nothing in the DOL
// allocates one, so its members and size are unknown. Named after its
// memPool_.
//
// It is probably JAUAudible<4>, but that is not proven, so the name stays:
// - JSystem's JAUAudioMgr.cpp builds this pool but not the
//   JAUDopplerAudible<4> one. mkdd's JAUAudioMgr has a JAUAudience<4>
//   (non-Doppler) member, whose Audible is JAUAudible<4>.
// - JAUAudience.cpp builds both pools.
// - mkdd's symbol map has exactly two JAU pools in game files:
//   JAUAudible<4> and JAUDopplerAudible<4>.
// No construct in our JAudio headers makes JAUAudible<4> complete in
// JAUAudioMgr.cpp yet.
class cls_804B21EC : public JASPoolAllocObject<cls_804B21EC> {};

#endif
