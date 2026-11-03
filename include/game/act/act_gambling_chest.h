#ifndef GAME_ACT_ACT_GAMBLING_CHEST_H
#define GAME_ACT_ACT_GAMBLING_CHEST_H

// Code: game/act_gambling_chest.cpp. act_gambling_shop calls its non-virtual
// members on the chest it finds by id 'GMTB' (0x803104F8).

#include "game/ActorBase.h"

// GMTB ("Gambling shop chest", docs/names/actors.tsv). Named after its
// vtable (0x804AAAE8); the spawner allocates 0x258 bytes (__nw__ before
// fn_80200904, its out-of-line constructor: base constructor and vtable).
class GamblingShopChest_804AAAE8 : public ActorBase_804A5770 {
public:
    GamblingShopChest_804AAAE8() {}
    /* 0x08 */ virtual ~GamblingShopChest_804AAAE8(); // fn_80310DE8
    /* 0x0C */ virtual void init();   // fn_80311504
    /* 0x14 */ virtual void update(); // fn_80311214
    /* 0x20 */ virtual void draw();   // fn_80310FC8

    // act_gambling_shop. Closes the chest: tile 0x10 at mPos
    // (BgRewrite_804A7C98::fn_802A3948), field_0x234 = -1, state 0.
    void fn_80310E48();
    // act_gambling_shop (0x80310730, 0x803107E0). State 1: tile 0x10, a
    // sound, saves mPos in field_0x248 and starts the 0x5A timer (state 2);
    // state 3: fn_8023061C(field_0x234, 0) and back to state 0.
    void fn_80310E98();
    // act_gambling_shop (0x80310500). Opens the chest: tile 0x30
    // (BgRewrite_804A7C98::fn_802A3884) and stores the three arguments
    // (the shop's 0x248 word, a word it points at and its mSerial), state 1.
    // s32 for the three words is a guess (lwz at the call site).
    void fn_80310F50(s32 a, s32 b, s32 serial);

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // fn_80310F50's a; -1 when closed
    /* 0x238 */ s32 field_0x238; // fn_80310F50's b
    /* 0x23C */ s32 field_0x23c; // fn_80310F50's serial
    /* 0x240 */ s32 field_0x240; // timer (0x5A, fn_80310E98)
    /* 0x244 */ s32 field_0x244; // counts up while the player is lifted (draw offset 0xED + n)
    /* 0x248 */ JGeometry::TVec3f field_0x248; // mPos saved by fn_80310E98
    /* 0x254 */ bool field_0x254; // update state 2: true while the player is held
}; // Size: 0x258

#endif
