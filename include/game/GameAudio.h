#ifndef GAME_GAMEAUDIO_H
#define GAME_GAMEAUDIO_H

// Game-side audio types shared by the sound units (0x80455CA4..0x8045BA84)
// and the units whose __sinit builds the same JAudio pools.
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "JSystem/JAudio/System/JASHeapCtrl.h"

// A pool-allocated class whose JASPoolAllocObject<>::memPool_ sits at
// 0x804B21EC (guard 0x8053A2B0, JASMemPool dtor 0x80021BBC,
// __dt__26JASMemPool<12cls_804B21EC>Fv). The __sinit of tu_80021840,
// JAUAudioMgr, JAUAudience, tu_80455CA4, tu_80457C08, tu_80457DFC,
// tu_8045A640, tu_8045B010, tu_8045B6F0 and tu_8045BA84 constructs its pool,
// always between the JASChannel and JAIStream pools. Nothing in the DOL
// allocates one, so its members and size are unknown. Named after its
// memPool_ (progress/game__tu_80457C08.md). Include this header where the
// unit declared the class before, so the pools keep their order.
class cls_804B21EC : public JASPoolAllocObject<cls_804B21EC> {};

#endif
