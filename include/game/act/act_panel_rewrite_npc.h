#ifndef GAME_ACT_ACT_PANEL_REWRITE_NPC_H
#define GAME_ACT_ACT_PANEL_REWRITE_NPC_H

// Code: game/act_panel_rewrite_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8E40.
class cls_804A8E40 : public ActorBase_804A5770 {
public:
    cls_804A8E40() {}
    /* 0x08 */ virtual ~cls_804A8E40();
    /* 0x0C */ virtual void init();
    // 0x10: the tile-grid write for this panel's cell (fn_802F9908 / fn_802F9974,
    // the BgRewrite tile calls) and its state machine; ends in update() or mAlive.
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x54 (appended after ActorBase's last slot): the panel's effect, played
    // on the layer at mPos (one of the KMRI / effect-id sets by field_0x244);
    // arg 1 also plays the field_0x240 sound / action.
    /* 0x54 */ virtual void vfunc_0x54(u8 arg);
    // 0x58: true when a SmallKey (KEY0) on this layer is in range of this
    // panel's position; the key is then told (fn_802C1970, fn_802C2B64).
    /* 0x58 */ virtual bool vfunc_0x58();
    // 0x5C: |a - b| <= c as two float compares; r3 unused.
    /* 0x5C */ virtual bool vfunc_0x5C(f32 a, f32 b, f32 c);

    /* 0x230 */ s32 field_0x230; // state: 0 (waiting), 1 (written), 3 (reset); set by vfunc_0x10 / update
    /* 0x234 */ s32 field_0x234; // type / tile code, field_0x90 & 0x3FF (init)
    /* 0x238 */ s32 field_0x238; // (field_0x90 >> 16) & 0xF (init); nonzero: a layer tile write
    /* 0x23C */ s32 field_0x23c; // (field_0x90 >> 20) & 7 (init); 2 keeps the panel live
    /* 0x240 */ s32 field_0x240; // (field_0x90 >> 23) & 7 (init); 2 / 3 select the vfunc_0x54 sound
    /* 0x244 */ s32 field_0x244; // (field_0x90 >> 12) & 0xF (init); indexes the effect tables
    /* 0x248 */ s32 field_0x248; // (field_0x90 >> 10) & 3 (init); gates by the game flag
    /* 0x24C */ bool field_0x24c; // field_0x90 bit 26 (vfunc_0x10); gates vfunc_0x54
    /* 0x250 */ s32 field_0x250; // map cell index (fn_80226BC4), passed as u16
    /* 0x254 */ u16 field_0x254; // tile code of the cell (fn_802F9908)
    /* 0x256 */ u8 field_0x256;  // layer offset for the tile writes: 0, 1 or 8
}; // Size: at least 0x257

#endif
