#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H

// lbl_80529DEC: the game's static state object (93 game units touch it).
// GAME_PLAN.md and the structure research called it "the UI struct"; it is
// not UI-specific. It holds the boot heaps, the resource archives every
// screen loads 2D data from, the selected stage and per-player tables, and
// a block of byte flags. game/tu_804023D0.cpp defines it: its __sinit
// (fn_80403B48) constructs it with fn_8040384C and registers fn_80403804 as
// its destructor; those two store the vtable 0x804B0FE0 (one slot, the
// destructor).
//
// "GameState_804B0FE0" is a provisional descriptive name with its vtable
// address (docs/PLAN.md, "Placeholder names"). The variable keeps dtk's link
// name. dtk sizes lbl_80529DEC as 0x354, but the same .bss block also holds
// the unit's nine __register_global_object chain nodes from 0x8052A044
// (lbl_80529DE0 + 0x264, fn_80403B48), so the object is 0x258 bytes at most;
// its last field seen is the byte at 0x254.
//
// Also here: cls_804AF86C, a wrapper around one J2DPane that 28 units
// allocate (tu_804023D0 holds its methods).
//
// Fields are named only where the code that writes them shows what they
// are; other offsets keep field_0xNN with the function that shows them.
// Archive users call vtable slot 0x18 (getResource(u32 type, const char*
// name), see GameGlobals.h on GameRoot_8049EF20::field_0xa4) on them.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function showing it.

#include "types.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRExpHeap.h"

class JKRArchive;
class JKRSolidHeap;
class J2DPane;
struct Unk_8045A080; // GameGlobals.h

class GameState_804B0FE0 {
public:
    // The 0x20 bytes at 0x80700000 (OSSetSaveRegion(0x80700000, 0x8070001F)),
    // as main (tu_801344C0) sees them inside GameState_804B0FE0 at 0x216. Only
    // the bytes main and its neighbours touch are named; offsets are within the
    // region.
    union SaveRegion {
        u8 bytes[0x20];
        struct Fields {
            /* 0x00 */ u16 magic0; // 0xA759 when valid
            /* 0x02 */ u16 magic1; // 0xDC96 when valid
            /* 0x04 */ u8 field_0x4[0x09 - 0x04];
            /* 0x09 */ u8 field_0x9; // fn_802B1DCC tests it
            /* 0x0A */ u8 field_0xa;
            /* 0x0B */ u8 field_0xb; // fn_802B1D7C tests it; fn_802B1D98 clears it
            /* 0x0C */ u8 field_0xc; // 0xFF preset; fn_802B1D60 returns it if <= 3
            /* 0x0D */ u8 field_0xd; // tu_803FF2C4's loader constructor tests it
            /* 0x0E */ u8 field_0xe;
            /* 0x0F */ u8 field_0xf;
            /* 0x10 */ u8 field_0x10; // 1: progressive scan wanted
            /* 0x11 */ u8 field_0x11[0x20 - 0x11];
        } f;
    }; // Size: 0x20

    // fn_8040384C: stores the vtable, constructs field_0x1c4 (TVec3f's
    // empty constructor through __construct_array) and zeroes or presets
    // most fields (it repeats fn_80403680's resets).
    GameState_804B0FE0();
    /* 0x08 */ virtual ~GameState_804B0FE0(); // fn_80403804

    // Out-of-line copy at 0x8042E780 in game/tu_80427D2C.cpp (the movie
    // player), built there with -inline off.
    JKRHeap* getHeap_8042E780() { return mHeap; }

    // 3 units (tu_803BC6F0, tu_803E1668, tu_803F0A54; all pass
    // &lbl_80529DEC). Presets the per-player tables (field_0x68[i] = 0,
    // field_0x78[i] = 1, field_0x88[i] = 2, ...), clears 0x118-0x12C, sets
    // field_0x64 and field_0x1a8 to -1 and resets most byte flags.
    void fn_80403680();
    // 1 unit (tu_803BC6F0). Clears field_0x118/field_0x11c, then picks them
    // from the table lbl_8050EC80 (0xC8-byte rows).
    void fn_80403514();

    /* 0x000 */ // vtable
    // main (tu_801344C0, 0x80134778-0x8013478C): the 0x180000-byte
    // JKRSolidHeap and the JKRExpHeap made from the root heap (the game's
    // main heap: 54 reads in 41 units pass it to __nw__ and the loaders),
    // and the 0x90-byte object fn_8045A080 builds in that heap.
    /* 0x004 */ JKRSolidHeap* field_0x4;
    /* 0x008 */ JKRExpHeap* mHeap;
    /* 0x00C */ Unk_8045A080* field_0xc; // the object lbl_8053AF80 also holds (calls fn_80459AE4)
    // Resource archives (the loader object's 0x14, an archive fn_80401AC8 /
    // fn_80401D20 create as JKRAramArchive or JKRMemArchive). The ones set
    // in fn_803FFAA8 (tu_803FF2C4) come from these paths (0x80400334-
    // 0x80400364 and 0x803FFBF0):
    /* 0x010 */ JKRArchive* field_0x10; // fn_80135494 (mode 7); fn_804032E8
    /* 0x014 */ JKRArchive* field_0x14; // "/GC4Sword_usa/data.arc"; copied to GameRoot +0xA4
    /* 0x018 */ JKRArchive* field_0x18; // "/GC4Sword_usa/logo.arc"; fn_80403244
    /* 0x01C */ JKRArchive* field_0x1c; // "/GC4Sword_usa/msgus.arc"
    /* 0x020 */ JKRArchive* field_0x20; // "/GC4Sword_usa/main2D.arc"; fn_804030C4 (36 units)
    /* 0x024 */ JKRArchive* field_0x24; // "/GC4Sword_usa/title.arc"; fn_80403204
    /* 0x028 */ JKRArchive* field_0x28; // fn_80135494 (mode 4); fn_804031C4
    /* 0x02C */ JKRArchive* field_0x2c; // fn_80135494 (modes 2, 5)
    /* 0x030 */ JKRArchive* field_0x30; // fn_80135494 (mode 3)
    /* 0x034 */ JKRArchive* field_0x34; // fn_80135494 (mode 6); fn_80403184
    /* 0x038 */ JKRArchive* field_0x38; // "/GC4Sword_usa/result.arc"; fn_80403144
    /* 0x03C */ JKRArchive* field_0x3c; // fn_8037A8D4 (tu_80379F84); fn_80403104
    /* 0x040 */ JKRArchive* field_0x40; // "/GC4Sword_usa/result1.arc"
    /* 0x044 */ JKRArchive* field_0x44; // "/GC4Sword_usa/result2.arc"
    /* 0x048 */ u8 field_0x48[0x4C - 0x48];
    /* 0x04C */ JKRArchive* field_0x4c; // fn_80135494 (mode 7, second archive); fn_80403084
    /* 0x050 */ u8 field_0x50[0x58 - 0x50];
    // fn_803FFAA8: the archive of the loader at its +0x44, the 0xEC-byte
    // object of fn_8030AC4C and the 4-byte object of fn_8030905C.
    /* 0x058 */ JKRArchive* field_0x58;
    /* 0x05C */ void* field_0x5c;
    /* 0x060 */ void* field_0x60;
    /* 0x064 */ s32 field_0x64; // -1 after fn_80403680; 15 units
    // Per-player word tables, preset by fn_80403680. fn_80403414 /
    // fn_804033D4 / fn_8040336C read 0x68 / 0x78 / 0x88 when
    // lbl_8053AAF8->field_0x7f is set and 0x98 / 0xA8 / 0xB8 otherwise.
    /* 0x068 */ s32 field_0x68[4];
    /* 0x078 */ s32 field_0x78[4];
    /* 0x088 */ s32 field_0x88[4];
    /* 0x098 */ s32 field_0x98[4];
    /* 0x0A8 */ s32 field_0xa8[4];
    /* 0x0B8 */ s32 field_0xb8[4];
    /* 0x0C8 */ s32 field_0xc8[4];
    /* 0x0D8 */ s32 field_0xd8[4];
    /* 0x0E8 */ s32 field_0xe8[4];
    /* 0x0F8 */ s32 field_0xf8[4];
    /* 0x108 */ s32 field_0x108[4];
    // The selected stage: fn_80135020 (tu_80134C04) copies 0x118 / 0x11C
    // (lbl_8053AAF8->field_0x7f set) or 0x128 / 0x12C (clear) into
    // GameView_804A9950's field_0x4 / field_0x8, the pair that picks the
    // stage's map files. 96 and 94 units read 0x118 / 0x11C.
    /* 0x118 */ s32 field_0x118;
    /* 0x11C */ s32 field_0x11c;
    /* 0x120 */ s32 field_0x120;
    /* 0x124 */ s32 field_0x124;
    /* 0x128 */ s32 field_0x128;
    /* 0x12C */ s32 field_0x12c;
    /* 0x130 */ u8 field_0x130[0x1A8 - 0x130]; // per-player tables, 0x130-0x1A4
    // -1 after the reset; tu_80441318's update picks the window from it
    // (0-3 a player, 4 all players).
    /* 0x1A8 */ s32 field_0x1a8;
    /* 0x1AC */ u8 field_0x1ac[0x1C4 - 0x1AC];
    // Per player (fn_80402FBC indexes it with fn_8022F340's player index).
    /* 0x1C4 */ JGeometry::TVec3f field_0x1c4[4];
    /* 0x1F4 */ s16 field_0x1f4[4]; // per player; fn_802702EC clears one
    /* 0x1FC */ u8 field_0x1fc[0x216 - 0x1FC];
    // main (tu_801344C0, 0x801344F8-0x80134624) copies 0x20 bytes here from
    // 0x80700000, the region it registers with OSSetSaveRegion (it survives a
    // reset), and presets it unless the first two halfwords are the magic.
    /* 0x216 */ SaveRegion mSaveRegion;
    /* 0x236 */ u8 field_0x236; // bit per player; fn_80402FBC
    /* 0x237 */ u8 field_0x237; // 24 units
    /* 0x238 */ u8 field_0x238; // 0xFF after fn_8040384C; fn_803FFAA8
    /* 0x239 */ u8 field_0x239; // bit per player; cls_804AC388::update ORs it
    /* 0x23A */ u8 field_0x23a[0x242 - 0x23A];
    /* 0x242 */ u8 field_0x242; // 19 units
    /* 0x243 */ u8 field_0x243[0x245 - 0x243];
    /* 0x245 */ u8 field_0x245; // 0 if OSGetSoundMode() is 0 else 1; tu_80442810's constructor
    /* 0x246 */ u8 field_0x246[0x248 - 0x246];
    /* 0x248 */ u8 field_0x248; // tu_8039003C's and tu_803A85E4's constructors set it to 1 after fn_80458408
    /* 0x249 */ u8 field_0x249[0x24D - 0x249];
    /* 0x24D */ u8 field_0x24d; // set by fn_8019CEE8 (tu_80197070)
    /* 0x24E */ u8 field_0x24e;
    /* 0x24F */ u8 field_0x24f; // 20 units
    /* 0x250 */ u8 field_0x250;
    /* 0x251 */ u8 field_0x251; // main counts it up from 1 to 0x5B, one per frame
    /* 0x252 */ u8 field_0x252[0x258 - 0x252];
}; // Size: 0x258 (at most; see above)

extern GameState_804B0FE0 lbl_80529DEC;

// game/tu_804023D0.cpp: getResource(type, name) on one of the archives
// above (vtable slot 0x18, the arguments passed through).
extern "C" void* fn_804032E8(u32 type, const char* name); // field_0x10; 29 units
extern "C" void* fn_80403244(u32 type, const char* name); // field_0x18
extern "C" void* fn_804030C4(u32 type, const char* name); // field_0x20; 36 units
extern "C" void* fn_80403204(u32 type, const char* name); // field_0x24
extern "C" void* fn_804031C4(u32 type, const char* name); // field_0x28
extern "C" void* fn_80403184(u32 type, const char* name); // field_0x34
extern "C" void* fn_80403144(u32 type, const char* name); // field_0x38
extern "C" void* fn_80403104(u32 type, const char* name); // field_0x3c
extern "C" void* fn_80403084(u32 type, const char* name); // field_0x4c

// A J2DPane and its box. Vtable 0x804AF86C: only the destructor, a weak
// inline whose kept copy is game/tu_803A60E4.cpp's 0x803A8378 (after that
// unit's functions, before its __sinit). 28 units allocate it with
// __nw__(0x2C) and the inline constructor (which only stores the vtable),
// then call fn_80402F04 with a pane from J2DScreen::search (e.g.
// tu_80403D84, 0x80412F18).
class cls_804AF86C {
public:
    cls_804AF86C() {}
    virtual ~cls_804AF86C() {}

    // 28 units. Stores pane, takes its position from getBounds() and its
    // size from its box, the centre (position + size / 2), field_0x28 from
    // the pane's byte at 0xB2, and clears the four counters.
    void fn_80402F04(J2DPane* pane);
    // 27 units. Moves the pane (vtable slot 0x10, J2DPane::move) so that its
    // centre is field_0x10 + x, field_0x14 + y.
    void fn_80402E08(f32 x, f32 y);
    // 1 unit. fn_80402F04 again for the stored pane (no counters, no 0x28).
    void fn_80402E70();
    // 2 units. Calls pane's vtable slot 0x24 (setAlpha) with
    // alpha * mPane's 0xB2 byte / field_0x28.
    void fn_804023D0(J2DPane* pane, u8 alpha);
    // fn_80402940 (16 units) and fn_80402B10 (5) step field_0x22 / field_0x20
    // up to frames (r4) with a mode byte in r5; the two floats are the
    // targets' coordinates (the f1 one is 0.0 in game/tu_804432DC.cpp).
    void fn_80402940(s16 frames, u8 mode, f32 x, f32 y);
    void fn_80402B10(s16 frames, u8 mode, f32 x, f32 y);
    // 3 units. Steps field_0x26 / field_0x24 up to frames, toward (x, y).
    void fn_8040271C(s16 frames, f32 x, f32 y);
    // 5 units. Resizes by (x, y).
    void fn_80402CE0(f32 x, f32 y);
    // Not declared yet: fn_80402628 (7 units); fn_8040245C (2).

    /* 0x00 */ // vtable
    /* 0x04 */ J2DPane* mPane;
    /* 0x08 */ f32 field_0x8;  // getBounds()->x
    /* 0x0C */ f32 field_0xc;  // getBounds()->y
    /* 0x10 */ f32 field_0x10; // centre x
    /* 0x14 */ f32 field_0x14; // centre y
    /* 0x18 */ f32 field_0x18; // width (pane 0x28 - 0x20)
    /* 0x1C */ f32 field_0x1c; // height (pane 0x2C - 0x24)
    /* 0x20 */ s16 field_0x20;
    /* 0x22 */ s16 field_0x22;
    /* 0x24 */ s16 field_0x24;
    /* 0x26 */ s16 field_0x26;
    /* 0x28 */ u8 field_0x28;
}; // Size: 0x2C

#endif
