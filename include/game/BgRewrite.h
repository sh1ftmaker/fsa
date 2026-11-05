#ifndef GAME_BGREWRITE_H
#define GAME_BGREWRITE_H

// *lbl_8053AC98 (53 units): the object that rewrites the stage's background
// tiles when actors change them (doors opening, blocks and walls bombed,
// grass burnt, switches pressed), from game/tu_8029D898.cpp.
//
// "BgRewrite_804A7C98" is a provisional descriptive name carrying its
// vtable address (docs/PLAN.md, "Placeholder names"). Evidence for "BG
// rewrite": nearly every function of the unit writes tile codes into the
// map's tile layers (gGameRoot2_8053AB10->0x20, TileMap_804AA858::fn_802F9974, 0x2C
// calls in fn_802A20F0 alone; the packed cell index comes from fn_80226BC4)
// and then sets GameView_804A9950::field_0x131de through fn_802D782C(1),
// which the BGWC actor ("BG-rewrite check switch", docs/names/actors.tsv,
// game/act_bg_rewrite_check_switch.cpp) polls; the per-screen actor masks
// are updated in the same breath (fn_802D4C10 / fn_802D7D00). fn_802A3A0C,
// called once a frame by tu_80139BE8, animates tiles with three (delay,
// frame) counters.
//
// The variable lbl_8053AC98 sits in the .sbss that dtk gives
// game/tu_8022AD64.cpp, but only this unit's constructor and destructor
// store it.
//
// Members: functions other units call with lbl_8053AC98 in r3. Names stay
// dtk's placeholders; symbols.txt carries the mangled names, so a changed
// parameter type is a symbols.txt rename too. "N units" counts the other
// game units calling the function. "layer" is an actor layer
// (ActorBase_804A5770::field_0x4). Not declared yet: the 2-unit tile
// pattern writers fn_8029DB2C, fn_8029DD50, fn_8029DEBC, fn_8029DF80,
// fn_8029EC84 (act_door, tu_802D39B8),
// fn_802A20F0 (10 units; spreads fire over burnable tiles) and the 1-unit
// ones; their argument types need their bodies read first.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function showing it.

#include "types.h"
#include "JSystem/JGeometry.h"

class ActorSoundObj_804578F4; // game/ActorManager.h

// Vtable 0x804A7C98: one slot, the destructor. tu_80139BE8 allocates it with
// __nw__(0x28, heap, 0) (0x8013A60C) and stores it at offset 0x10 of the
// object lbl_8053AAF8->field_0x4 points to.
class BgRewrite_804A7C98 {
public:
    // fn_802A3E2C (1 unit): stores the vtable, sets lbl_8053AC98 = this and
    // field_0x4 = a new ActorSoundObj_804578F4 (__nw__(0x5C); its
    // constructor fn_804578F4 gets 0, 0, 8).
    BgRewrite_804A7C98();
    // fn_802A3DA4: clears lbl_8053AC98 and deletes field_0x4 (a virtual
    // destructor call through the vtable pointer at its offset 0x8).
    /* 0x08 */ virtual ~BgRewrite_804A7C98();

    // 2 units (tu_8022AD64, act_knight_of_hyrule). Saves the view's
    // field_0x34, sets it to value (GameView_804A9950::fn_802D7970), calls
    // GameRoot::fn_80138B7C on lbl_8053AAF8 and restores it. this unused.
    void fn_8029D898(s32 value);
    // 4 units (Player_804A6F38::fn_802870C8, act_block, act_somaria_block,
    // tu_8028FBA0). Reads pos x/y; player and layer follow.
    void fn_8029E508(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    // 15 units / 78 calls. Writes tile code `tile` into the cell at pos on
    // `layer` (fn_80226BC4(pos', layer & 0xFE), then fn_802F9974(layer,
    // cell, tile)); nothing else. this unused. pos: callers pass
    // &mPos; the body copies x and y to a stack TVec3f for fn_80226BC4.
    // tile: u16, as for fn_802A3884.
    // `const TVec3f*` (here and below) is a guess: the asm can't tell it from
    // a reference or a non-const pointer.
    void fn_802A381C(const JGeometry::TVec3f* pos, s32 layer, u16 tile);
    // 8 units / 31 calls. The same write, remembering the cell in
    // field_0x20, then fn_802D782C(1) on the view, and when the tile code
    // is an actor id slot of the layer (fn_802D4C10(layer, (u16)cell) !=
    // -1) clears that slot's bit in the screen mask (fn_802D7D00(layer,
    // slot, false)). `player` is not read by the body: callers pass 0
    // (25 sites), -1 (1) or a word field (act_switch 0x250 at 0x802A7564);
    // its name and type are guesses. tile: callers pass lhz halfwords
    // (act_switch 0x802A756C) or clrlwi 16 a word first
    // (act_panel_rewrite_npc), so it is a u16.
    void fn_802A3884(const JGeometry::TVec3f* pos, s32 player, s32 layer, u16 tile);
    // 1 unit besides tu_8029D898 (act_gravestone). Writes the four tile
    // codes a..d (u16, clrlwi 16 / lhz) into the cells at and after pos on
    // `layer` (two fn_802F9974 calls per pair, like fn_802A3948).
    void fn_8029E050(const JGeometry::TVec3f* pos, s32 layer, u16 a, u16 b, u16 c, u16 d);
    // 2 units (act_bomb, act_bombchu), 4 calls. Called with pos = &mPos,
    // player 0 and layer = the actor's field_0x4; the fourth argument is a
    // 12-byte stack slot the body may write, handed back to the caller
    // (act_bomb passes the same slot on to fn_8014D868). The return value
    // is tested with clrlwi 24 and cmplwi 1 in act_bomb, so it is a u8.
    // out: its type is a guess from the slot's size; the body is not read.
    u8 fn_8029EDB8(const JGeometry::TVec3f* pos, s32 player, s32 layer, JGeometry::TVec3f* out);
    // 17 units / 62 calls. fn_802A3884 with fn_802D7D00(..., true): sets
    // the bit. `player` as above (0 at 41 sites, 1 at 3, act_gravestone's
    // 0x238 player index).
    void fn_802A3948(const JGeometry::TVec3f* pos, s32 player, s32 layer, u16 tile);
    // 1 unit (tu_80139BE8, once a frame). Steps the tile animations: every
    // 10 calls field_0x8 wraps and field_0xc advances by 2 (mod 0x10) and
    // the 'SCH ' resources are redrawn with fn_802FA160; field_0x18 /
    // field_0x1c likewise (mod 8).
    void fn_802A3A0C();
    // 1 unit (tu_80139BE8). Resets the counters: 0x8/0x10/0x18 = 10,
    // 0xc/0x14/0x1c = 0.
    void fn_802A3D80();
    // 10 units. Spreads fire over burnable tiles (see above). Callers pass
    // &mPos, a player (-1, or a word field), the layer, a u32 (fn_801F9484 argument) and an s32
    // (0, 0 in tu_801BEDBC and tu_802233A4, 1, 1 in
    // act_spreading_fire); types from the body.
    void fn_802A20F0(const JGeometry::TVec3f* pos, s32 player, s32 layer, u32 a, s32 b);
    // 1 unit (tu_80256C74, the dig of state 0x47 at frame 12). Reads the map tile
    // under pos (fn_80226BC4 into field_0x20, fn_802F9908 into field_0x24) and
    // returns a result code that the caller tests for 1-4 and 8.
    s32 fn_802A1844(const JGeometry::TVec3f* pos, s32 player, s32 layer);

    /* 0x00 */ // vtable
    /* 0x04 */ ActorSoundObj_804578F4* field_0x4;
    /* 0x08 */ s32 field_0x8;  // delay, 10 calls
    /* 0x0C */ s32 field_0xc;  // frame, steps of 2, mod 0x10
    /* 0x10 */ s32 field_0x10; // delay
    /* 0x14 */ s32 field_0x14; // frame
    /* 0x18 */ s32 field_0x18; // delay
    /* 0x1C */ s32 field_0x1c; // frame, mod 8
    /* 0x20 */ s32 field_0x20; // the last cell written (fn_802A3884 / fn_802A3948)
    /* 0x24 */ u8 field_0x24[0x28 - 0x24];
}; // Size: 0x28

extern BgRewrite_804A7C98* lbl_8053AC98;

#endif
