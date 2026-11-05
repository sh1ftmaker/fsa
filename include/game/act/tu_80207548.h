#ifndef GAME_ACT_TU_80207548_H
#define GAME_ACT_TU_80207548_H

// Code: game/tu_80207548.cpp.
// A sub-base of EnemyBase_804A4808, named after its vtable (0x804A5B28).
// Its own functions (0x80207548-0x80207B5C) override the destructor
// (0x80207548) and add two empty virtual slots (0xA4 = 0x80207ABC,
// 0xA8 = 0x80207AB8); the constructor is 0x80207AC0. Four per-player
// slots (one request per player, 0x24 bytes each) are filled by
// fn_80207830 and answered by fn_802075D8.

#include "game/EnemyBase.h"

class cls_804A5B28 : public EnemyBase_804A4808 {
public:
    // One request per player (0x80207B2C is its constructor).
    struct Slot {
        Slot();
        /* 0x00 */ s32 mSerial;  // the actor fn_802075D8 looks up (findActorBySerial_801F666C)
        /* 0x04 */ u8 mUsed;     // set by fn_80207830
        /* 0x05 */ u8 pad_0x5[3];
        /* 0x08 */ s32 mPlayer;  // player index 0-3, -1 = none
        /* 0x0C */ s32 field_0xc;
        /* 0x10 */ JGeometry::TVec3f mTarget;
        /* 0x1C */ s32 field_0x1c;
        /* 0x20 */ u8 mDone;     // set by fn_802075D8
        /* 0x21 */ u8 pad_0x21[3];
    }; // Size: 0x24

    cls_804A5B28();
    virtual ~cls_804A5B28() {}
    /* 0xA4 */ virtual void vfunc_0xA4();
    /* 0xA8 */ virtual void vfunc_0xA8();

    // 0x802075D8
    void fn_802075D8(u16 slot);
    // 0x80207830
    void fn_80207830(u16 slot, s32 player, s32 arg3, const JGeometry::TVec3f* target, s32 arg5);
    // 0x80207A60
    void fn_80207A60();

    /* 0x2A8 */ u8 mActive;
    /* 0x2A9 */ u8 mMask;   // bit per player picked by fn_80207830
    /* 0x2AA */ u8 mTimer;
    /* 0x2AB */ u8 pad_0x2ab;
    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ Slot mSlots[4];
}; // Size: 0x340

#endif
