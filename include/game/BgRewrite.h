#ifndef GAME_BGREWRITE_H
#define GAME_BGREWRITE_H

// *gBgRewrite_8053AC98 (53 units): the object that rewrites the stage's background
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
// The variable gBgRewrite_8053AC98 sits in the .sbss that dtk gives
// game/tu_8022AD64.cpp, but only this unit's constructor and destructor
// store it.
//
// Members: functions other units call with gBgRewrite_8053AC98 in r3. Names stay
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
// object gGameRoot_8053AAF8->field_0x4 points to.
class BgRewrite_804A7C98 {
public:
    // fn_802A3E2C (1 unit): stores the vtable, sets gBgRewrite_8053AC98 = this and
    // field_0x4 = a new ActorSoundObj_804578F4 (__nw__(0x5C); its
    // constructor fn_804578F4 gets 0, 0, 8).
    BgRewrite_804A7C98();
    // fn_802A3DA4: clears gBgRewrite_8053AC98 and deletes field_0x4 (a virtual
    // destructor call through the vtable pointer at its offset 0x8).
    /* 0x08 */ virtual ~BgRewrite_804A7C98();

    // 2 units (tu_8022AD64, act_knight_of_hyrule). Saves the view's
    // field_0x34, sets it to value (GameView_804A9950::fn_802D7970), calls
    // GameRoot::fn_80138B7C on gGameRoot_8053AAF8 and restores it. this unused.
    void fn_8029D898(s32 value);
    // 1 unit (act_liftable_wood, 2 calls). Writes 4 x 4 blocks of tile codes
    // from lbl_8046C5A0 (on layer + layerOffset) and lbl_8046C528 (on layer)
    // indexed by pattern * 0x28 around pos. this unused.
    void fn_8029D8F0(const JGeometry::TVec3f* pos, s32 layer, s32 layerOffset, s32 pattern);
    // 4 units (Player_804A6F38::fn_802870C8, act_block, act_somaria_block,
    // tu_8028FBA0). Reads pos x/y; player and layer follow.
    bool fn_8029E508(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    // 15 units / 78 calls. Writes tile code `tile` into the cell at pos on
    // `layer` (fn_80226BC4(pos', layer & 0xFE), then fn_802F9974(layer,
    // cell, tile)); nothing else. this unused. pos: callers pass
    // &mPos; the body copies x and y to a stack TVec3f for fn_80226BC4.
    // tile: u16, as for fn_802A3884.
    // `const TVec3f*` (here and below) is a guess: the asm can't tell it from
    // a reference or a non-const pointer.
    void fn_802A381C(const JGeometry::TVec3f* pos, s32 layer, u16 tile);
    // 2 units (act_item_pedestal, tu_802D39B8). Writes a pair of tiles (tileA, then the cell
    // 16 columns left, or 0x110 back at a screen edge, gets tileB) on layer + layerOffset and
    // marks the view cell.
    void fn_8029EC84(s32 cell, s32 layer, s32 layerOffset, u16 tileA, u16 tileB);
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
    // &mPos, a player (-1, or a word field), the layer, a u32 (spawnActor_801F9484 argument) and an s32
    // (0, 0 in tu_801BEDBC and tu_802233A4, 1, 1 in
    // act_spreading_fire); types from the body.
    void fn_802A20F0(const JGeometry::TVec3f* pos, s32 player, s32 layer, u32 a, s32 b);
    // 1 unit (tu_802957CC); body unwritten (tu_8029D898).
    void fn_802A1050(const JGeometry::TVec3f* pos);
    // 1 unit (tu_80256C74, the dig of state 0x47 at frame 12). Reads the map tile
    // under pos (fn_80226BC4 into field_0x20, fn_802F9908 into field_0x24) and
    // returns a result code that the caller tests for 1-4 and 8.
    s32 fn_802A1844(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    // 1 unit (tu_80256C74, the ground smash of state 0x3C). Reads the tile under pos
    // like fn_802A1844 (fields 0x20 / 0x24); returns li 1 / li 0 (the caller ignores it).
    bool fn_802A1D44(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    // 1 unit (tu_8025EB18 fn_80260C08, the pick-up in front). Read the tile
    // under pos like fn_802A1844 (fields 0x20 / 0x24); fn_8029E888 returns the
    // tile code == 0x30 as a u8 (subfic / cntlzw / extrwi; the caller tests 1),
    // fn_802A01F0 1 for code 0x71 or fn_802A0800's result for 0x16 / 0x17
    // (clrlwi.), fn_802A02A8 the kind of object on the tile (-1, 0-0x27; the
    // caller switches on it).
    u8 fn_8029E888(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    bool fn_802A01F0(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    s32 fn_802A02A8(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    // 1 unit (tu_8025EB18 fn_80263860, the held thing's position with z 0 in
    // state 0x15). Reads the tile under pos like the above; returns 0 or 1 (li
    // into r30), tested with clrlwi.
    bool fn_802A3164(const JGeometry::TVec3f* pos, s32 player, s32 layer);
    // 1 unit (act_door, 3 calls). Writes the two tile codes a, b into the
    // cell and its right neighbour (vertical == 0, wrapping +0xF0 into the
    // next 16-column block) or the cell below (+0x10, +0x110 across the
    // 0x100 boundary). this unused.
    void fn_8029DEBC(s32 layer, s32 cell, s32 vertical, s32 a, s32 b);
    // 2 units (see above). As fn_8029DEBC for four (DD50) or six (DB2C) tile codes in a row
    // or column (u16 each, clrlwi 16). this unused.
    void fn_8029DD50(s32 layer, s32 cell, s32 vertical, u16 a, u16 b, u16 c, u16 d);
    void fn_8029DB2C(s32 layer, s32 cell, s32 vertical, u16 a, u16 b, u16 c, u16 d, u16 e, u16 f);
    // 2 units (act_door, tu_802D39B8). The 2 x 2 block of tile codes a..d at
    // cell (right neighbour and the row below as in fn_8029DEBC). this unused.
    void fn_8029DF80(s32 cell, s32 layer, s32 a, s32 b, s32 c, s32 d);
    // 1 unit (act_block). fn_802A3948 on gBgRewrite_8053AC98 with player 0;
    // this unused.
    void fn_8029E928(const JGeometry::TVec3f* pos, s32 layer, s32 tile);
    // 1 unit (act_block, init). Stores the tile it reads into this+0x20 / +0x24.
    // 0x8029E248 (game/tu_8029D898.cpp): the tile on pos is 0x19C / 0x19D (a pair of halves); rewrites it to 0x19E / 0x19F.
    bool fn_8029E248(const JGeometry::TVec3f* pos, s32 layer);
    void fn_8029E9EC(const JGeometry::TVec3f* pos, s32 layer, u16 tile, s32 arg4, u8 dir);
    // 1 unit (tu_802957CC, 2 calls each). Read the tile under pos on layer 0
    // into fields 0x20 / 0x24; true when its lbl_8046CB88 collision code is 0
    // or fn_8022461C gives 0x22 while GameView field_0x170 is clear
    // (fn_802A1554 also for a list of tile codes and the codes 0xE / 3).
    bool fn_802A1490(const JGeometry::TVec3f* pos);
    // 0x802A1688 (game/tu_8029D898.cpp): the tile on pos is 0x81: unless tag is 'SBAL', rewrites it to 0x9D and spawns
    // a 'KMRI' actor (layers 8 and up) or two emitters; plays sound 0x50045. Returns fn_802A3164(pos, -1, layer).
    bool fn_802A1688(const JGeometry::TVec3f* pos, s32 layer, u32 tag);
    bool fn_802A1554(const JGeometry::TVec3f* pos);
    // Called by fn_802A01F0 / fn_802A02A8 for tile codes 0x16 / 0x17 with
    // their player and layer (r4, r5); the body is not written yet (the
    // return type from fn_802A01F0, which returns its r3 as its bool).
    bool fn_802A0800(s32 player, s32 layer);

    /* 0x00 */ // vtable
    /* 0x04 */ ActorSoundObj_804578F4* field_0x4;
    /* 0x08 */ s32 field_0x8;  // delay, 10 calls
    /* 0x0C */ s32 field_0xc;  // frame, steps of 2, mod 0x10
    /* 0x10 */ s32 field_0x10; // delay
    /* 0x14 */ s32 field_0x14; // frame
    /* 0x18 */ s32 field_0x18; // delay
    /* 0x1C */ s32 field_0x1c; // frame, mod 8
    /* 0x20 */ s32 field_0x20; // the last cell written (fn_802A3884 / fn_802A3948)
    /* 0x24 */ u16 field_0x24; // the tile code at field_0x20 (fn_802F9908; sth / lhz)
    /* 0x26 */ u8 field_0x26[0x28 - 0x26];
}; // Size: 0x28

extern BgRewrite_804A7C98* gBgRewrite_8053AC98;

#endif
