#ifndef GAME_ACT_TU_802F3B74_H
#define GAME_ACT_TU_802F3B74_H

// Code: game/tu_802F3B74.cpp.

#include "game/ActorBase.h"
#include "game/Message.h"

// Named after its vtable, 0x804AA6A0; the spawner allocates 0x240 bytes
// (__nw__ before fn_801FF9CC, its out-of-line constructor). Its 4CC is not
// identified yet. When its two player fields agree it spawns a PEFF
// ("Player effect") actor and opens message field_0x230 for that player
// (fn_8038BD08), and removes itself once the window is idle again.
class cls_804AA6A0 : public ActorBase_804A5770 {
public:
    cls_804AA6A0() {}
    /* 0x08 */ virtual ~cls_804AA6A0();   // fn_802F3B74
    /* 0x0C */ virtual void init();       // fn_802F3C8C
    /* 0x14 */ virtual void update();     // fn_802F3BD8
    /* 0x20 */ virtual void draw();       // fn_802F3BD4, empty
    /* 0x24 */ virtual void vfunc_0x24(); // fn_802F3C24

    // Takes the position by value, as the original's call does; mPlayer and
    // mMsg are read inside, after the copy of the position.
    bool open(s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038BD08(mPlayer, mMsg, layer, pos, flags);
    }

    /* 0x230 */ s32 mMsg;        // spawn parameter bits 0-11
    /* 0x234 */ s32 field_0x234; // bits 12-15
    /* 0x238 */ s32 mPlayer;     // bits 16-19
    /* 0x23C */ s32 mEffectSerial; // the PEFF actor's serial, or -1
}; // Size: 0x240

#endif
