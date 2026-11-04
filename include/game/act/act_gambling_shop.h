#ifndef GAME_ACT_ACT_GAMBLING_SHOP_H
#define GAME_ACT_ACT_GAMBLING_SHOP_H

// Code: game/act_gambling_shop.cpp.

#include "game/ActorBase.h"

// The GMBL actor ("Gambling shop", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80200940, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AAA70.
class GamblingShop_804AAA70 : public ActorBase_804A5770 {
public:
    GamblingShop_804AAA70() {}
    /* 0x08 */ virtual ~GamblingShop_804AAA70();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // act_gambling_chest's update (0x80311364) calls it on the shop that
    // answers its serial: field_0x25c = value, field_0x234 = 0x3C, state 6
    // (stw 0x25c / 0x234 / 0x230, 0x8030FDC0).
    void fn_8030FDC0(s32 value);
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234; // 0x3C from fn_8030FDC0
    /* 0x238 */ s32 field_0x238; // 2 at init
    /* 0x23C */ s32 field_0x23c; // 0x14 at init
    /* 0x240 */ s32 field_0x240; // toggled by the 0x244 counter
    /* 0x244 */ s32 field_0x244; // counter, 0x10 / 0x14 wraps
    /* 0x248 */ s32 field_0x248; // -1 at init; set to an actor's id
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250[3]; // -1 at init; slots of the GMTB chests found (vfunc_0x10)
    /* 0x25C */ s32 field_0x25c; // fn_8030FDC0's value; -1 at init
    /* 0x260 */ s32 field_0x260; // 0x12C or 0x64 at init (from field_0x264)
    /* 0x264 */ u32 field_0x264; // bit 31 of field_0x90
    /* 0x268 */ u32 field_0x268; // bits 24-27 of field_0x90
    /* 0x26C */ u32 field_0x26c; // bits 0-3 of field_0x90
    /* 0x270 */ u32 field_0x270; // field_0x26c + bits 4-7
    /* 0x274 */ u32 field_0x274; // field_0x270 + bits 8-11
    /* 0x278 */ s32 field_0x278[4];
    /* 0x288 */ bool field_0x288;
    /* 0x289 */ u8 field_0x289[3]; // not yet known
}; // Size: 0x28C

#endif
