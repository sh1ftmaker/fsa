#ifndef GAME_MAP_H
#define GAME_MAP_H

// The stage map and its collision, from three units:
//
// * game/tu_802F5040.cpp: cls_804AA858, the object at lbl_8053AB10->0x20
//   (124 consumer units). It holds the screen's tile layers (u16 tile codes
//   on a 32 x 32 grid of 16-unit cells, fn_802F9908 / fn_802F9974) and
//   draws the "SOB " sprites (fn_802F86CC, fn_802F745C).
// * game/tu_8022461C.cpp: the tile queries for a position (112 consumer
//   units): the position, relative to the view's visible box
//   (GameView_804A9950::mVisibleBox, or field_0xdc for layers 8 and up), is
//   clamped to [0, 0x200) and packed into a cell index, x & 0x100 |
//   (y & 0x100) << 1 | y & 0xF0 | x >> 4 & 0xF, whose tile comes from
//   fn_802F9908; positions outside the box give 0xF.
// * game/tu_8020D58C.cpp: the actor collision boxes (42 consumer units),
//   double-buffered lists of boxes that actors register each frame and
//   ActorBase_804A5770::vfunc_0x4C tests a position against after the tile
//   check.
//
// Conventions follow GameView.h: members for functions called with the
// object in r3 (mangled names in symbols.txt), extern "C" for free
// functions (dtk's names stay the link names). "N units" counts the other
// game units calling the function or touching the field. "layer" is an
// actor layer (ActorBase_804A5770::field_0x4); 8 and up use the view's
// second box. Positions are JGeometry::TVec3f: the queries read x and y
// only, and callers pass &mPos or a stack vector.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function showing it.

#include "types.h"
#include "JSystem/JGeometry.h"

class JKRHeap;
struct SpriteParams_801F5AE0; // game/ActorBase.h

// lbl_8053AB10->0x20. Named after its vtable (0x804AA858, one slot);
// allocated with __nw__(0xE464, heap, 0) in game/tu_8013AAD4.cpp
// (0x8013C230-0x8013C25C).
class cls_804AA858 {
public:
    // fn_802FFDEC; tu_8013AAD4 passes the heap it allocated the object from
    // (r30, also __nw__'s heap). Proven: fn_802FFDEC calls getTotalFreeSize__7JKRHeapFv on it.
    cls_804AA858(JKRHeap* heap);
    /* 0x08 */ virtual ~cls_804AA858(); // fn_802FFBF8

    // 61 units / 144 calls. Draws frame `frame` of the "SOB " resource `res`
    // at pos, scaled by (scaleX, scaleY), when field_0x2364 is set; params
    // (a SpriteParams_801F5AE0 or NULL) selects variants (its bytes 0x21 /
    // 0x22 divert to fn_802F654C / fn_802F6298). The body reads res
    // bytewise at res + frame * 2 + 2..5 (0x802F87FC), so its real type is
    // a byte-based format: void* (callers pass getResource's result
    // unchanged, act_arrow 0x8016564C) is a guess. Callers pass the same
    // float twice (fmr f2, f1).
    void fn_802F86CC(const JGeometry::TVec3f* pos, void* res, s32 frame,
                     SpriteParams_801F5AE0* params, f32 scaleX, f32 scaleY);
    // Draws the sprite `res` (a "TIMG" resource, act_life_meter's heart image
    // from fn_804030C4) at pos with the stack SpriteParams_801F5AE0 `params`
    // (fn_801EF830 calls it the same way, 0x801EFC74).
    void fn_802F745C(const JGeometry::TVec3f* pos, void* res, SpriteParams_801F5AE0* params);
    // 5 units (act_shutter, act_red_blue_switch, act_spike_floor_check,
    // tu_802D39B8, tu_8029D898) plus BgRewrite's tile animation. Decodes
    // the 'SCH ' resource `res` (JKRDecomp::checkCompressed / decode, size
    // from getResSize when `size` is 0) and rewrites the 4-bit tile data
    // at this + 4 + slot * 4 + dstOffset from res + srcOffset (0x802FA160).
    // Callers pass (res, 0 / 0xC0 / 0x180, 0, 0x40, 0xC0).
    void fn_802FA160(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size);
    // 0x802F9714 (game/tu_802F5040.cpp, not written yet): the same arguments
    // and tile-data rewrite as fn_802FA160 (checkCompressed / getResSize for
    // size 0, then the slot rows at this + 0x98 + slot * 4); tu_8039003C's
    // constructor passes the 'SCH ' resources (slots 0, 0, 2, 3), as does
    // tu_803A85E4's (slots 0, 0, 2, 3; dstOffset 0 / 0x7000 / 0x4000 / 0).
    void fn_802F9714(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size);
    // 0x802F93F4 (game/tu_802F5040.cpp, not written yet): the 'SCL ' form of
    // fn_802F9714 with a trailing byte flag (clrlwi. 24 at 0x802F9568); the
    // constructor of tu_8039003C and tu_803A85E4's pass false.
    void fn_802F93F4(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size, bool flag);
    // 0x802FF798 (game/tu_802F5040.cpp, not written yet): no arguments;
    // called once by tu_8039003C's and tu_803A85E4's constructors before it loads the tile
    // resources.
    void fn_802FF798();
    // 17 units. The tile code of cell `index` on `layer` (lhzx from the
    // layer's u16 grid, a pointer at 0x28 + layer * 4; the body has three
    // identical branches for layers 0-3, 4-7 and the rest).
    u16 fn_802F9908(s32 layer, s32 index);
    // 9 units / 198 calls. Sets it (sthx; callers clrlwi 16 the value,
    // act_panel_rewrite_npc 0x802C8AC4).
    void fn_802F9974(s32 layer, s32 index, u16 value);
    // 0x802FF0EC (game/tu_802F5040.cpp, not written yet): r3 only (saved
    // in r21, fields 0x148+, 0x214, 0x230 read). Called by
    // GameRoot2_8049F108::fn_8013B538 / fn_8013B618 with field_0x20.
    void fn_802FF0EC();

    /* 0x0000 vtable */
    /* 0x0004 */ u8 field_0x4[0x214 - 0x4];
    /* 0x0214 */ u8 field_0x214; // 2 units compare it with 1 (act_player_sword_level_check_npc 0x802F1350)
    /* 0x0215 */ u8 field_0x215;
    /* 0x0216 */ u8 field_0x216; // 9 units set it to 1 (act_four_switch_monitor 0x802A9E78)
    /* 0x0217 */ u8 field_0x217[4]; // tu_80418B64 stores them (0x8041AEBC, 0x8041F43C-0x8041F48C)
    /* 0x021B */ u8 field_0x21b[0x230 - 0x21B];
    /* 0x0230 */ s32 field_0x230; // 2 units read it (tu_80432638 0x80432BE4)
    /* 0x0234 */ u8 field_0x234[0x235C - 0x234];
    // Per-player bytes: tu_80139BE8 (0x80139C24), tu_802D39B8, tu_803A8DFC
    // and tu_803B7DCC store them; Camera_804AA9E4::fn_8030C85C /
    // fn_8030C928 copy them to field_0x2360.
    /* 0x235C */ u8 field_0x235c[4];
    /* 0x2360 */ u8 field_0x2360[4];
    // fn_802F86CC draws nothing while it is 0; 6 units set it to 1
    // (tu_802D39B8 0x802D9BFC).
    /* 0x2364 */ u8 field_0x2364;
    // Camera_804AA9E4::fn_8030C928 moves field_0x2368 to field_0x2374 and
    // stores the new box's corner (x0, y0, constant) in field_0x2368;
    // tu_8016A2B4 reads it (0x8016E014).
    /* 0x2368 */ JGeometry::TVec3f field_0x2368;
    /* 0x2374 */ JGeometry::TVec3f field_0x2374;
    // Written by tu_802D39B8 (0x802D92D4), read as ints by tu_8016A2B4
    // (0x8016AE44).
    /* 0x2380 */ s32 field_0x2380;
    /* 0x2384 */ s32 field_0x2384;
    /* 0x2388 */ u8 field_0x2388[0xE464 - 0x2388];
}; // Size: 0xE464

// --- tile queries (game/tu_8022461C.cpp) ---
extern "C" {
// 73 units. The tile's collision code: the tile code (as in fn_80226850)
// mapped through the u16 table lbl_8046CB88, adjusted when the layer's
// GameView byte 0x131DF + layer is set (0 -> 2, 0x61 -> 0) and on stage
// (7, 2) / (7, 3) (0xE -> 0x51); 0xF outside the box. Callers clrlwi 16
// the result.
u16 fn_8022461C(const JGeometry::TVec3f* pos, s32 layer);
// 15 units. The same result as fn_8022461C; its 0x61 -> 0 case compiles
// branch-free (0x80226770), so the source differs. Callers compare it with
// codes such as 3 and 0x50 (act_bomb 0x8014BFF4).
u16 fn_802265FC(const JGeometry::TVec3f* pos, s32 layer);
// 46 units. The raw tile code (clrlwi 22 of fn_802F9908), 0xF outside the
// box; callers clrlwi 16 the result.
u16 fn_80226850(const JGeometry::TVec3f* pos, s32 layer);
// 13 units. The same as fn_80226850 (only the register allocation differs).
u16 fn_80226998(const JGeometry::TVec3f* pos, s32 layer);
// 7 units each. The packed cell index of pos (no lookup; clamped like the
// queries, no outside test in fn_80226AE0).
s32 fn_80226AE0(const JGeometry::TVec3f* pos, s32 layer);
s32 fn_80226BC4(const JGeometry::TVec2f* pos, s32 layer); // reads only x, y
// 5 units. The inverse: out = the corner of cell `index` (box corner + the
// unpacked x, y) with a constant z (psq_st/stfs to out 0/8, 0x80226D88).
void fn_80226CA8(s32 index, s32 layer, JGeometry::TVec3f* out);
// 2 units. Whether the tile at pos blocks: the collision code of
// fn_8022461C (computed inline), 0 / 2 / 0x50 never, 0x22 only while
// GameView field_0x170 is set; `flag` (clrlwi 24, 0x80226344) selects
// further cases. Returns 0 or 1.
bool fn_802260DC(const JGeometry::TVec2f* pos, s32 layer, bool flag); // reads pos+0/+4 only
// 19 units / 37 calls. A plain call of fn_802260DC with the same
// arguments (ActorBase_804A5770::vfunc_0x4C passes flag 0).
bool fn_802265DC(const JGeometry::TVec3f* pos, s32 layer, bool flag);
// 4-5 units each. Tile-code range tests at pos; return 0 or 1 (callers
// clrlwi 24). fn_80224B88 also reads GameView byte 0x131F1.
bool fn_8022517C(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80224B88(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80224E88(const JGeometry::TVec3f* pos, s32 layer);
}

// --- actor collision boxes (game/tu_8020D58C.cpp) ---

// One registered box. Its empty out-of-line constructor is fn_8020D65C
// (the list constructor's __construct_array, 0x50 x 0x18), hence the name.
struct CollisionBox_8020D65C {
    /* 0x00 */ JGeometry::TBox2f mBox;
    /* 0x10 */ s32 mLayer;  // the actor's field_0x4 (fn_8020D7D0)
    /* 0x14 */ s32 mSerial; // the actor's mSerial; fn_8020D6AC skips its own
}; // Size: 0x18

// Named after its constructor fn_8020D60C (count = 0, __construct_array of
// the boxes).
class CollisionBoxList_8020D60C {
public:
    // 20 units / 22 calls. Whether pos (x, y) lies in a box on `layer`
    // registered by an actor other than `serial`; with ignoreIds (a list
    // ending in 'NULL', cmplw against ActorBase_804A5770::mActorId of the
    // box's actor, fn_801F666C), boxes of those actors don't count.
    // Returns 0 or 1. Callers use the read list (field_0x1).
    bool fn_8020D6AC(s32 layer, const JGeometry::TVec3f* pos, s32 serial, const u32* ignoreIds);
    // 15 units / 16 calls. Registers the box of the actor `serial` (its
    // box at 0x80 moved by mPos.x / mPos.y, layer field_0x4) if the actor
    // has byte 0x11C set, the box is not empty and fewer than 0x50 are
    // listed. Callers use the write list (field_0x0).
    void fn_8020D7D0(s32 serial);
    // 7 units. Registers box on layer for serial (the same checks).
    void fn_8020D8C0(const JGeometry::TBox2f* box, s32 layer, s32 serial);

    /* 0x000 */ s32 mCount;
    /* 0x004 */ CollisionBox_8020D65C mBoxes[0x50];
}; // Size: 0x784

// The function-local static of fn_8020D58C (lbl_804CB8B0, guard
// lbl_8053AC68), built inline there: field_0x0 = 0, field_0x1 = 1, both
// counts 0. Named after that getter.
class CollisionBoxes_8020D58C {
public:
    // 1 unit (tu_801F6058, once a frame). Swaps field_0x0 and field_0x1
    // (xori 1) and empties the new write list.
    void fn_8020D660();
    // 1 unit (tu_802D39B8). Resets field_0x0 / field_0x1 to 0 / 1 and both
    // counts to 0.
    void fn_8020D690();

    // Descriptive name, not from the original. An inline wrapper: all 8
    // visible fn_8020D7D0 call sites emit the identical sequence (the serial
    // is loaded before fn_8020D58C is called, then the list is indexed by
    // field_0x0), e.g. act_invisible_npc_wall_tomi's update (0x80376448).
    void addBox(s32 serial) { mLists[field_0x0].fn_8020D7D0(serial); }

    /* 0x000 */ u8 field_0x0; // the list fn_8020D7D0 / fn_8020D8C0 fill
    /* 0x001 */ u8 field_0x1; // the list fn_8020D6AC reads
    /* 0x004 */ CollisionBoxList_8020D60C mLists[2];
}; // Size: 0xF0C

// 42 units. The collision boxes (see above).
extern "C" CollisionBoxes_8020D58C* fn_8020D58C(void);

#endif
