#ifndef GAME_GAMEVIEW_H
#define GAME_GAMEVIEW_H

// The stage/screen state of game/tu_802D39B8.cpp: one instance, allocated
// with __nw__(0x13290, heap, 0) and built by its constructor fn_802DE64C in
// game/tu_80139BE8.cpp (0x8013A578-0x8013A594), stored at offset 0xC of the
// object lbl_8053AAF8->0x4 points to; reach it with fn_802D6A9C(). 275 game
// units use it.
//
// "GameView_804A9950" is a provisional name carrying its vtable address
// (docs/PLAN.md, "Placeholder names"; it was GameView_802D6A9C, after the
// getter, until the vtable was found). The 10 x 10 grid below is the stage's
// screens: field_0x14/field_0x18 are the current column/row (both checked
// against [0, 10) by fn_802D8128 and stepped by fn_802DCD1C's direction), and
// most per-screen tables are indexed [field_0x18][field_0x14]. field_0x4 and
// field_0x8 select the stage: fn_802D8128 indexes the 16-byte rows of the
// string table lbl_804A9530 with them to build "d_map%s_%d%d_mmm_2_%d.szs",
// and the free predicates at the end (fn_802DEB58, ...) compare them with
// pairs such as (3, 1).
//
// Members: every export of game/tu_802D39B8.cpp that other units call with
// the view in r3 (all of them pass fn_802D6A9C()'s result; several bodies
// ignore it and fetch the view again through lbl_8053AAF8, said at each as
// "(this unused)"). Names stay dtk's placeholders; symbols.txt carries the
// mangled names these declarations produce, so a changed parameter type is a
// symbols.txt rename too (the return type is not part of the name).
// "N units" counts the other game units calling the function. "layer" is an
// actor layer (ActorBase_804A5770::field_0x4): the tables indexed by it fold
// layers 0-8 into slot 0 and layer n > 8 into n - 7 (the subfc/subfe/andc
// clamp). Parameter types beyond the register class (u8 for a plain stb, s32
// for a word) are the narrowest the asm shows; where a body only stores a
// byte, the callers' li/clrlwi do not distinguish u8 from bool.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function showing it.

#include "types.h"
#include "JSystem/JGeometry.h"

class GameView_804A9950 {
public:
    // fn_802DE64C (1 unit): stores the vtable, constructs the four TVec2f
    // arrays (__construct_array with fn_801546E4, an empty 8-byte
    // constructor) and zeroes the words noted below.
    GameView_804A9950();

    // Virtual table at 0x804A9950: 4 slots. tu_80139BE8 calls 0xC
    // (0x8013A1D0), 0x10 (0x80139E2C) and 0x14 (0x80139CE8) through it; none
    // reads an argument.
    /* 0x08 */ virtual ~GameView_804A9950(); // fn_802DE604
    /* 0x0C */ virtual void vfunc_0xC();      // fn_802DD808, 0xDFC bytes
    // fn_802DD54C: copies field_0xf0[0] (negated when lbl_8053AAF8->0x5C has
    // bit 1 set) to lbl_8053AB10->0x24 + 0x1DC and walks the players.
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void vfunc_0x14(); // fn_802DD548, empty

    // --- per-stage state ---
    // 1 unit. field_0x131a8[index] = value (tu_801F6058 reads the four words).
    void fn_802D39B8(s32 index, s32 value);
    // 1 unit. field_0x14 == field_0xc && field_0x18 == field_0x10.
    bool fn_802D39CC();
    // 1 unit (the spawner, into ActorBase_804A5770::field_0x170).
    // field_0x60[field_0x18][field_0x14].
    u8 fn_802D39FC();
    // 1 unit / 1 call. sprintf of "d_map%s_%d%d_mmm_2_%d.szs" (lbl_8046E7E8)
    // into field_0x1321a for the current screen and `layer`; returns it, or
    // NULL off the grid (0x802D8210).
    const char* fn_802D8128(s32 layer);
    // 1 unit. The same with the second name format (0x802D8228).
    const char* fn_802D8228(s32 layer);
    // 1 unit. Entry field_0x44 of the 16-entry table lbl_804A96D0, or the
    // .sdata2 default lbl_805410DC.
    const char* fn_802D9C1C();
    // 9 units. Entry field_0x40 of the 28-entry table lbl_804A9660, or
    // lbl_8054106C.
    const char* fn_802D9C4C();
    // 1 unit (this unused). lbl_804A9530[a][b] when lbl_8053AAF8->0x7F == 1,
    // else lbl_80541128.
    const char* fn_802D9C7C(s32 a, s32 b);
    // 2 units. Calls fn_8030C85C on lbl_8053AB10->0x24 (this unused).
    void fn_802D9CB0();
    // 1 unit. 0x2E4 bytes; no arguments.
    void fn_802D9CD8();
    // 1 unit. field_0xc4 = 0x8CA0 if lbl_8053AAF8->0x7F == 1, else 0x2A30.
    void fn_802D9FBC();
    // 1 unit. field_0x1c = a, field_0x20 = b.
    void fn_802D9FE8(s32 a, s32 b);
    // 18 units / 26 calls. field_0x24 = a, field_0x28 = b.
    void fn_802D9FF4(s32 a, s32 b);
    // 1 unit (this unused). lbl_8053AB10->0x24's byte at 0x1E8, like
    // fn_802DCD0C.
    bool fn_802DA000();
    // 1 unit each; no arguments. fn_802DA630 returns 0 or 1 (it moves the four
    // players with fn_80230498 and walks the actor list).
    void fn_802DA010();
    bool fn_802DA630();
    void fn_802DA788();
    // 4 units. 0xE14 bytes; no arguments.
    void fn_802DA8DC();
    // 1 unit. 0x670 bytes; no arguments.
    void fn_802DB6F0();
    // 3 units / 6 calls. 0xFAC bytes; no arguments.
    void fn_802DBD60();
    // 31 units / 42 calls (this unused). lbl_8053AB10->0x24's byte at 0x1E8.
    bool fn_802DCD0C();
    // 2 units / 5 calls. Unless fn_802DCD0C(), steps field_0x14 by the sign
    // of dir[0] or field_0x18 by that of dir[1], reloads the screen
    // (fn_802D8D50, fn_802D8328, fn_8030C928 with &mVisibleBox); returns 0
    // or 1. Callers pass the address of a stack pair (lwz 0/4); const s32*
    // is a guess, a two-int struct passed by value would look the same.
    bool fn_802DCD1C(const s32* dir);
    // 1 unit. Clears the actors (fn_801F7CAC, fn_801F6204), field_0x131de and
    // the effects (fn_8013C8CC), and field_0xc9.
    void fn_802DCDF4();
    // 1 unit. The screen change of fn_802DCD1C with dir read the same way
    // (lwz 0/4; the same guess); returns 0 or 1.
    bool fn_802DCE5C(const s32* dir);
    // 1 unit. The same, after looking for a 'SMSM' actor; sets field_0xc8,
    // clears field_0xc9; returns 0 or 1. dir as in fn_802DCD1C (a guess).
    bool fn_802DD3D4(const s32* dir);

    // --- per-layer actor-id slots (field_0x2948) and their per-screen
    // 64-bit masks (field_0x1010) ---
    // 6 units / 20 calls (this unused). Bit `bit` of field_0x2d48[index]
    // (mask table lbl_8046E1CC): tests it (0 or 1), or sets it and returns 1.
    bool fn_802D46D8(s32 index, u32 bit, bool set);
    // 1 unit. Puts id in the last free slot of the layer's list (searching
    // down from slot 0x3C in steps of 4); *outSlot = 15 - slot / 4 (stw
    // srawi 31 and the value: a sign-extended 64-bit store, hence s64),
    // *outMask = the screen's mask (u64 like fn_802D4874's; its signedness
    // is not visible).
    void fn_802D4768(s32 layer, u16 id, s64* outSlot, u64* outMask);
    // 2 units. Puts id in the first free slot; *outBit = 1 << slot, *outMask =
    // the screen's mask (two words each).
    void fn_802D4874(s32 layer, u16 id, u64* outBit, u64* outMask);
    // 3 units. Puts id in the first free slot and tests its bit in the
    // screen's mask; returns 0 or 1.
    bool fn_802D49A8(s32 layer, u16 id);
    // 3 units (this unused). The same lookup, reading through lbl_8053AAF8.
    bool fn_802D4ADC(s32 layer, u16 id);
    // 4 units / 88 calls (this unused). The slot of id (cmplw against the
    // clrlwi 16 id) in the layer's list, or -1.
    s32 fn_802D4C10(s32 layer, u16 id);
    // 1 unit / 4 calls. The screen's mask for the layer (r3:r4).
    u64 fn_802D7AC4(s32 layer);
    // 1 unit. The 4-bit field `slot` (bits 60 - slot * 4) of the screen's
    // mask: __shr2u, then the low word (callers clrlwi 16).
    s32 fn_802D7B34(s32 layer, s32 slot);
    // 2 units / 7 calls. Sets that 4-bit field to value.
    void fn_802D7BE8(s32 layer, s32 slot, s32 value);
    // 4 units / 85 calls. Sets or clears bit `bit` (__shl2i(0, 1, bit)) of
    // the screen's mask; callers pass fn_802D4C10's slot.
    void fn_802D7D00(s32 layer, s32 bit, bool set);

    // --- the per-screen TVec2f slots at 0x13068-0x131A8 ---
    // The vectors are passed by value: every caller builds one, word-copies
    // it to a second stack slot and passes that slot's address, which is
    // GC/2.6's by-value copy of a TVec2f.
    // 1 unit / 13 calls. Frees the slot whose id (field_0x13188) is id and
    // whose field_0x13148 equals pos (within epsilon).
    void fn_802D6C10(JGeometry::TVec2f pos, u32 id);
    // 1 unit / 9 calls. Fills the first slot whose field_0x13188 is -1 with
    // a, b, value and id; returns 0 or 1.
    bool fn_802D6D70(JGeometry::TVec2f a, JGeometry::TVec2f b, s32 value, u32 id);
    // 1 unit / 3 calls. With add, puts (key, pos, value) into the first free
    // slot of field_0x13068 / field_0x13088 / field_0x130c8 (shifting the
    // list when full); else removes the entry matching key and pos.
    void fn_802D6F38(u32 key, JGeometry::TVec2f pos, s32 value, bool add);

    // --- bit tables (bit = index into an array of u32, 32 per word) ---
    // 2 units. Tests / sets or clears a bit of field_0xfd68[row][col].
    bool fn_802D6AC4(s32 bit);
    void fn_802D6B24(s32 bit, bool set);
    // 1 unit / 2 units (this unused in fn_802D72BC). field_0x12fe8.
    bool fn_802D72BC(s32 bit);
    void fn_802D7310(s32 bit, bool set);
    // 1 unit each. field_0x12f68.
    bool fn_802D7384(s32 bit);
    void fn_802D73CC(s32 bit, bool set);
    // 4 units / 1 unit. Tests / sets a bit of field_0x6768[row][col].
    bool fn_802D75BC(s32 bit);
    void fn_802D7618(s32 bit);
    // 1 unit / 2 units. Tests / sets a bit of field_0x3548[row][col]; with
    // count, fn_802D7704 also increments field_0x6748[layer].
    bool fn_802D76A8(s32 bit);
    void fn_802D7704(s32 bit, bool count, s32 layer);
    // 1 unit. With flag, increments field_0x6748[layer].
    void fn_802D766C(bool flag, s32 layer);
    // 2 units (this unused). Tests / sets or clears a bit of field_0x174.
    bool fn_802D7888(s32 bit);
    void fn_802D78B4(s32 bit, bool set);
    // 2 units (this unused). The 64-bit mask at field_0x3c8 + index * 0x320
    // (index clrlwi 24): tests / sets or clears bit `bit`.
    bool fn_802D7E5C(u8 index, s32 bit);
    void fn_802D7EB0(u8 index, s32 bit, bool set);
    // 6 units / 18 calls. Tests bit `bit` of field_0x348[field_0x4][field_0x8]
    // (the word read through lbl_8053AAF8, the stage from this).
    bool fn_802D7F40(s32 bit);
    // 4 units. Sets or clears it.
    void fn_802D7F84(s32 bit, bool set);
    // 46 units / 103 calls. Tests bit `bit` of field_0x178[row][col]
    // (ActorBase_804A5770::vfunc_0x34 passes vfunc_0x3C()).
    bool fn_802D800C(s32 bit);
    // 44 units / 126 calls. With set: sets the bit, increments
    // field_0x308[bit] (saturating at 0xFF) and stores value in
    // field_0x328[bit]; else clears the bit and stores 0xFF there.
    // ActorBase_804A5770::vfunc_0x30 passes (vfunc_0x3C(), set, value).
    void fn_802D8050(s32 bit, bool set, u8 value);
    // 1 unit (this unused). field_0x308[index] (lbz). An int: ActorBase's
    // vfunc_0x38 returns it without the clrlwi a u8 return would need.
    s32 fn_802D7E04(s32 index);
    // 2 units (this unused). field_0x328[index] (lbz). An int: its callers
    // store it with stw and compare it with cmpw, no clrlwi.
    s32 fn_802D7E1C(s32 index);
    // 1 unit (this unused). Increments field_0x308[index] below 0xFF.
    void fn_802D7E34(s32 index);

    // --- bytes and halfwords of the 0x131B8 block ---
    // 1 unit / 8 units. Byte `index` (0-255) of field_0x9968[row][col]: get
    // (0 out of range) / set.
    u8 fn_802D7544(s32 index);
    void fn_802D7584(s32 index, u8 value);
    // 1 unit (this unused). field_0x131b8[index] &= bits ^ 0xFF. bits is a
    // u16 like its siblings': act_view_mode passes li 0x3000 (0x802E0538)
    // and the body xori's it without the clrlwi a u8 would need.
    void fn_802D7790(s32 index, u16 bits);
    // 2 units / 4 calls (this unused). field_0x131b8[index] |= bits.
    void fn_802D77BC(s32 index, u16 bits);
    // 1 unit (this unused). field_0x131b8[index] = value.
    void fn_802D77E4(s32 index, u16 value);
    // 3 units / 12 calls. field_0x131b8[index] & bits, as 0 or 1.
    bool fn_802D7804(s32 index, u16 bits);
    // 6 units (this unused). field_0x131d8[index] = value for index 0-3.
    void fn_802D7844(u8 value, s32 index);
    // Plain setters (this unused; the callers' li or clrlwi decide nothing).
    void fn_802D6AAC(u8 value); // 2 units: field_0x131f9
    void fn_802D6BC8(u8 value); // 2 units: field_0x131f4
    void fn_802D6BE0(u8 value); // 1 unit: field_0x131f3
    void fn_802D6BF8(u8 value); // 1 unit / 8 calls: field_0x131f2
    void fn_802D752C(u8 value); // 1 unit: field_0x131ef
    void fn_802D782C(u8 value); // 1 unit / 83 calls: field_0x131de
    void fn_802D7870(u16 value); // 2 units: field_0x131dc
    void fn_802D790C(u8 value); // 1 unit: field_0x171
    void fn_802D7920(u8 value); // 1 unit: field_0x170
    void fn_802D7934(s32 value); // 1 unit: field_0x5c
    // 23 units / 35 calls (this unused). Sets field_0x131f0 from flag, the
    // save data lbl_8050EC80 (byte 0x7C or 0x7D of entry [0x10], 0xC8 bytes
    // each) and lbl_8053AAF8's bytes 0x7E/0x7F.
    void fn_802D7440(bool flag);
    // 1 unit (this unused). field_0x5c ? field_0x5c - 1 : field_0x58.
    s32 fn_802D7948();
    // 3 units (this unused). field_0x34 = value; below 0x11 also clears
    // field_0x1328c.
    void fn_802D7970(s32 value);
    // 2 units. If field_0x1327c[index] is 0 and field_0x1324c[index] still
    // has its initial value, stores (a, b, c) in field_0x1324c/0x1325c/0x1326c
    // [index] and sets field_0x1327c[index].
    void fn_802D79A4(s32 index, f32 a, f32 b, f32 c);
    // 32 units / 54 calls. field_0xf0[index].x = max(.x, b), .y = max(.y, a)
    // (f2 goes to +0, f1 to +4); unless flag, raises all 16 to the scaled
    // pair as well.
    void fn_802D79E8(s32 index, f32 a, f32 b, bool flag);

    /* 0x00000 vtable */
    /* 0x00004 */ s32 field_0x4; // stage (see the top comment); 21 units read it
    /* 0x00008 */ s32 field_0x8; // level within the stage; 20 units read it
    /* 0x0000C */ s32 field_0xc;
    /* 0x00010 */ s32 field_0x10;
    /* 0x00014 */ s32 field_0x14; // current screen column, 0-9 (zeroed by the constructor)
    /* 0x00018 */ s32 field_0x18; // current screen row, 0-9 (zeroed by the constructor)
    /* 0x0001C */ s32 field_0x1c;
    /* 0x00020 */ s32 field_0x20;
    /* 0x00024 */ s32 field_0x24;
    /* 0x00028 */ s32 field_0x28;
    /* 0x0002C */ s32 field_0x2c; // fn_802DA630 tests > 0
    /* 0x00030 */ u8 field_0x30[0x34 - 0x30];
    /* 0x00034 */ s32 field_0x34; // fn_802D7970; 3 units read it
    /* 0x00038 */ s32 field_0x38;
    /* 0x0003C */ s32 field_0x3c;
    /* 0x00040 */ s32 field_0x40; // index into lbl_804A9660 (fn_802D9C4C)
    /* 0x00044 */ s32 field_0x44; // index into lbl_804A96D0 (fn_802D9C1C)
    /* 0x00048 */ s32 field_0x48;
    /* 0x0004C */ s32 field_0x4c;
    /* 0x00050 */ s32 field_0x50;
    /* 0x00054 */ s32 field_0x54;
    /* 0x00058 */ s32 field_0x58;
    /* 0x0005C */ s32 field_0x5c;
    /* 0x00060 */ u8 field_0x60[10][10]; // per screen (fn_802D39FC, fn_802DE764)
    /* 0x000C4 */ s32 field_0xc4; // fn_802D9FBC
    /* 0x000C8 */ u8 field_0xc8; // set by fn_802DD3D4
    /* 0x000C9 */ u8 field_0xc9; // cleared by fn_802DCDF4 and fn_802DD3D4; act_sandstorm reads it
    // The visible area in top-down coordinates (x, y - z): 58 units take its
    // address (e.g. fn_8030C928(cam, &mVisibleBox) in fn_802DCD1C) or copy
    // it word by word; ActorBase_804A5770's destructor lets an emitter
    // inside it run out instead of deleting it.
    /* 0x000CC */ JGeometry::TBox2f mVisibleBox;
    // A second box, used for actor layers 8 and up (0x801F165C); the
    // constructor sets its four floats.
    /* 0x000DC */ JGeometry::TBox2f field_0xdc;
    /* 0x000EC */ s32 field_0xec;
    /* 0x000F0 */ JGeometry::TVec2f field_0xf0[16]; // fn_802D79E8, vfunc_0x10
    /* 0x00170 */ u8 field_0x170; // fn_802D7920; 21 units read it
    /* 0x00171 */ u8 field_0x171; // fn_802D790C; 21 units read it
    /* 0x00174 */ u32 field_0x174; // bits: fn_802D7888 / fn_802D78B4
    /* 0x00178 */ u32 field_0x178[10][10]; // bits per screen: fn_802D800C / fn_802D8050
    /* 0x00308 */ u8 field_0x308[0x20]; // counters per bit: fn_802D8050, fn_802D7E04
    /* 0x00328 */ u8 field_0x328[0x20]; // values per bit: fn_802D8050, fn_802D7E1C
    /* 0x00348 */ u32 field_0x348[8][4]; // bits per (stage, level): fn_802D7F40
    // 0x3C8 + index * 0x320 holds a 64-bit mask (fn_802D7E5C); the rest of
    // this block is not mapped yet.
    /* 0x003C8 */ u8 field_0x3c8[0x1010 - 0x3C8];
    /* 0x01010 */ u64 field_0x1010[10][10][8]; // per screen and layer slot
    /* 0x02910 */ u8 field_0x2910[0x2948 - 0x2910];
    /* 0x02948 */ u16 field_0x2948[8][0x40]; // actor ids per layer slot (fn_802D4C10)
    /* 0x02D48 */ u32 field_0x2d48[4][0x80]; // bits (fn_802D46D8)
    /* 0x03548 */ u32 field_0x3548[10][10][32]; // bits per screen (fn_802D76A8)
    /* 0x06748 */ s32 field_0x6748[8]; // counters per layer slot (fn_802D766C)
    /* 0x06768 */ u32 field_0x6768[10][10][32]; // bits per screen (fn_802D75BC)
    /* 0x09968 */ u8 field_0x9968[10][10][0x100]; // bytes per screen (fn_802D7544)
    /* 0x0FD68 */ u32 field_0xfd68[10][10][32]; // bits per screen (fn_802D6AC4)
    /* 0x12F68 */ u32 field_0x12f68[0x20]; // bits (fn_802D7384)
    /* 0x12FE8 */ u32 field_0x12fe8[0x20]; // bits (fn_802D72BC)
    /* 0x13068 */ u32 field_0x13068[8]; // keys (fn_802D6F38)
    /* 0x13088 */ JGeometry::TVec2f field_0x13088[8];
    /* 0x130C8 */ s32 field_0x130c8[8]; // -1 when free
    /* 0x130E8 */ JGeometry::TVec2f field_0x130e8[8]; // fn_802D6D70's a
    /* 0x13128 */ s32 field_0x13128[8];
    /* 0x13148 */ JGeometry::TVec2f field_0x13148[8]; // fn_802D6D70's b
    /* 0x13188 */ u32 field_0x13188[8]; // ids, -1 when free
    /* 0x131A8 */ s32 field_0x131a8[4]; // fn_802D39B8; read by tu_801F6058
    /* 0x131B8 */ u16 field_0x131b8[0x10]; // bits (fn_802D77BC, fn_802D7804)
    /* 0x131D8 */ u8 field_0x131d8[4]; // fn_802D7844
    /* 0x131DC */ u16 field_0x131dc; // fn_802D7870; tu_80432638 reads it
    // fn_802D782C; lbz in act_bg_rewrite_check_switch, stb in 8 more places
    // in game/tu_802D39B8.cpp.
    /* 0x131DE */ u8 field_0x131de;
    /* 0x131DF */ u8 field_0x131df[0x131EF - 0x131DF];
    /* 0x131EF */ u8 field_0x131ef; // fn_802D752C; 2 units read it
    /* 0x131F0 */ u8 field_0x131f0; // fn_802D7440; tu_80301CD0 reads it
    /* 0x131F1 */ u8 field_0x131f1; // read by tu_8013C5C8 (fn_8013CC50) and tu_8022461C
    /* 0x131F2 */ u8 field_0x131f2; // fn_802D6BF8; 5 units read it
    /* 0x131F3 */ u8 field_0x131f3; // fn_802D6BE0
    /* 0x131F4 */ u8 field_0x131f4; // fn_802D6BC8
    /* 0x131F5 */ u8 field_0x131f5; // act_torch reads it
    /* 0x131F6 */ u8 field_0x131f6; // act_player_start_position reads it
    /* 0x131F7 */ u8 field_0x131f7; // zeroed by the constructor; tu_8044C7F0
    /* 0x131F8 */ u8 field_0x131f8; // zeroed by the constructor; tu_8044C7F0
    /* 0x131F9 */ u8 field_0x131f9; // fn_802D6AAC; 5 units read it
    /* 0x131FA */ u8 field_0x131fa[0x1321A - 0x131FA];
    /* 0x1321A */ char field_0x1321a[0x1324C - 0x1321A]; // fn_802D8128's name buffer
    /* 0x1324C */ f32 field_0x1324c[4]; // fn_802D79A4
    /* 0x1325C */ f32 field_0x1325c[4];
    /* 0x1326C */ f32 field_0x1326c[4];
    /* 0x1327C */ u8 field_0x1327c[4];
    /* 0x13280 */ JGeometry::TVec3f field_0x13280; // stored by act_shovel_old_timer
    /* 0x1328C */ s32 field_0x1328c; // fn_802D7970; act_temple_bti_display, tu_80134C04
}; // Size: 0x13290

// lbl_8053AAF8->0x4->0xC (game/tu_802D39B8.cpp; 265 units).
extern "C" GameView_804A9950* fn_802D6A9C(void);

// Free functions of game/tu_802D39B8.cpp: no view argument (callers leave r3
// unset). Each returns 0 or 1 from the stage numbers of fn_802D6A9C() (the
// view's field_0x4 / field_0x8), and returns 0 at once when
// lbl_8053AAF8->0x7F is 0, except where said.
//
// Defined in game/tu_802D39B8.cpp (C++ linkage, renamed in symbols.txt).
// 8 units. Reads lbl_80529DEC's words 0x118 and 0x11C only.
bool fn_802DE814(void);
// 19 units. Stage (2, 3) or (7, 1); with 0x7F clear, lbl_80529DEC's 0x12C +
// 0x128 * 4 is 1 or 6.
bool fn_802DE9A0(void);
// 3 units. Stage (7, 0) or (7, 2); with 0x7F clear, lbl_80529DEC's 0x12C +
// 0x128 * 4 is 4 or 9.
bool fn_802DEABC(void);

// Not written yet in game/tu_802D39B8.cpp: extern "C" keeps their dtk names
// until that unit defines them.
extern "C" {
// 2 units. (field_0x4, field_0x8) == (stage, level) and
// field_0x60[row][col] == value.
bool fn_802DE764(s32 stage, s32 level, s32 value);
// 4 units. Stage (0, 0).
bool fn_802DE7CC(void);
// 1 unit. Stage (4, 0) or (7, 2).
bool fn_802DE93C(void);
// 1 unit. Stage (6, 0), (6, 1) or (6, 3).
bool fn_802DEA34(void);
// 21 units. Stage (3, 1).
bool fn_802DEB58(void);
// 10 units. Stage (9, 3), or stage 13 and up.
bool fn_802DEBA0(void);
// 9 units. Stage (8, 3).
bool fn_802DEBF8(void);
}

#endif
