#ifndef GAME_MAP_H
#define GAME_MAP_H

// The stage map and its collision, from three units:
//
// * game/tu_802F5040.cpp: TileMap_804AA858, the object at gGameRoot2_8053AB10->0x20
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

#include "dolphin/gx/GXStruct.h"
#include "game/SpriteParams.h"
#include "game/Camera.h" // ScreenRect_802FF3F0

class JKRHeap;
class JUTTexture;
class J2DScreen;
class J2DAnmTextureSRTKey;
class PaneCtrl_804AF86C;
namespace JUtility {
struct TColor;
}

// game/tu_80432638.cpp (owner unit, not written yet). Named after its vtable,
// lbl_804B1668 (stored by its constructor, 0x8043395C); 0xB0 bytes
// (TileMap_804AA858's constructor allocates it). Only what TileMap_804AA858 uses.
class cls_804B1668 {
public:
    cls_804B1668();
    /* 0x08 */ virtual ~cls_804B1668();
    // 0x80432750: called by TileMap_804AA858::fn_802FF0EC with
    // (field_0x230 == -1, the field_0x21c-0x228 rectangle as floats); r4 is
    // tested clrlwi. (bool).
    void fn_80432750(bool all, f32 x, f32 y, f32 w, f32 h);
    // 0x80432638: called by fn_802FF0EC after the panels when fn_80432750 ran.
    void fn_80432638();
    // 0x80433718: follows GameView's field_0x131dc (bit 0x8000 = 'raised').
    void fn_80433718();

    /* 0x04 */ J2DScreen* field_0x4[2];
    /* 0x0C */ J2DScreen* field_0xc;
    /* 0x10 */ J2DAnmTextureSRTKey* field_0x10[2]; // the screens' animations
    /* 0x18 */ PaneCtrl_804AF86C* field_0x18[4];
    /* 0x28 */ PaneCtrl_804AF86C* field_0x28[3];
    /* 0x34 */ PaneCtrl_804AF86C* field_0x34;
    /* 0x38 */ u8 field_0x38[0x3C - 0x38];
    /* 0x3C */ PaneCtrl_804AF86C* field_0x3c;
    /* 0x40 */ JGeometry::TVec3f field_0x40[2];
    /* 0x58 */ JGeometry::TVec3f field_0x58[2];
    /* 0x70 */ f32 field_0x70[4];
    /* 0x80 */ f32 field_0x80[4];
    /* 0x90 */ s32 field_0x90;
    /* 0x94 */ s32 field_0x94;
    /* 0x98 */ s32 field_0x98;
    /* 0x9C */ u16 field_0x9c;
    /* 0x9E */ u16 field_0x9e;
    /* 0xA0 */ u16 field_0xa0;
    /* 0xA2 */ u8 field_0xa2;
    /* 0xA3 */ u8 field_0xa3;
    /* 0xA4 */ u8 field_0xa4;
    /* 0xA5 */ u8 field_0xa5;
    /* 0xA6 */ u8 field_0xa6[6];
    /* 0xAC */ u8 field_0xac;
    /* 0xAD */ u8 field_0xad[0xB0 - 0xAD];
}; // Size: 0xB0

// One 4-byte entry of cls_804AA858Sub11C (GBA OAM-like: y, 9-bit x).
struct cls_804AA858Sub11CEntry {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mY;
    /* 0x2 */ u8 mXLow;
    /* 0x3 */ u8 mScroll : 4;
    u8 mScrollBit : 1;
    u8 field_0x3_1 : 2;
    u8 mXHigh : 1;
}; // Size: 0x4

// A GBA sprite word: tu_80301CD0's per-player sprite tables (lbl_80506F88)
// and a record's field_0x18 words (fn_803066F0 / fn_803064A8 fill them,
// fn_803068D0 copies them).
union GbaSprite_80506F88 {
    u32 mWord;
    struct {
        /* 0x0 */ u8 field_0x0; // low byte of the 10-bit id (high bits: field_0x3_1)
        /* 0x1 */ u8 field_0x1; // y
        /* 0x2 */ u8 field_0x2; // x low byte; 0x80 (with field_0x3_0) hides it
        /* 0x3 */ u8 mKind : 3;
        u8 field_0x3_4 : 1;
        u8 field_0x3_3 : 1;
        u8 field_0x3_1 : 2;
        u8 field_0x3_0 : 1; // x bit 8
    } f;
}; // Size: 0x4

// TileMap_804AA858::field_0x11c: four of these, plain new(0x68) (no constructor).
// fn_802F53F4 copies one in whole from tu_80301CD0's data (0x803051A0) and
// reads its packed bytes; fn_803068D0 reads the same bits of byte 2.
struct cls_804AA858Sub11C {
    /* 0x00 */ u32 field_0x0 : 16;
    /* 0x02 */ u32 mMode : 2;      // 2 / 3 select the bits of 0x5 / 0x4 kept; bit 0 enables the tables
    u32 field_0x2_5 : 1;
    u32 mValid : 3;                // nonzero for a valid record
    u32 mPlayer : 2;               // fn_803068D0 (clrlwi 30)
    /* 0x03 */ u32 mKind : 4;      // 5 or 14 for the records fn_802F53F4 keeps
    u32 field_0x3_0 : 4;
    /* 0x04 */ u32 field_0x4_7 : 1;
    u32 field_0x4_5 : 2;
    u32 field_0x4_0 : 5;
    /* 0x05 */ u32 field_0x5_7 : 1;
    u32 field_0x5_6 : 1;
    u32 mOffsetMode : 2;           // fn_802FB69C: cls_804AA8D8::fn_8030B1E8's mode in mode 3
    u32 field_0x5_0 : 4;
    /* 0x06 */ u32 field_0x6 : 8;  // four 2-bit values
    /* 0x07 */ u32 field_0x7_4 : 4; // four 1-bit values
    u32 field_0x7_0 : 4;           // four 1-bit values
    // Four player marks (fn_802FCE74 places a 48 x 48 view around mEntries[
    // lbl_80506DD8[player]]: x = mXLow | mXHigh << 8, y = mY); the high bits
    // of their last bytes hold the panel's 9-bit scroll (fn_802FB69C): x =
    // mEntries[0].mScroll << 5 | mEntries[0].mScrollBit << 4 | mEntries[1].mScroll,
    // y likewise from entries 2 and 3; mEntries[3].mScrollBit is the overlay flag.
    /* 0x08 */ cls_804AA858Sub11CEntry mEntries[4];
    /* 0x18 */ GbaSprite_80506F88 field_0x18[(0x68 - 0x18) / 4]; // fn_803068D0 copies words from here
}; // Size: 0x68


// game/tu_80301CD0.cpp (owner unit, not written yet): per-player tables
// filled by fn_802F53F4 (0x802F54AC) for the player `slot`, and the record
// handler it calls last (0x802F5558; reads byte 2 like fn_802F53F4).
extern cls_804AA858Sub11C lbl_80506DE8[4];
extern u8 lbl_80506DA8[4][4];
extern u8 lbl_80506DB8[4][4];
extern u8 lbl_80506DC8[4][4];
extern s32 lbl_80506DD8[4];
void fn_803068D0(cls_804AA858Sub11C* record);
// Called by TileMap_804AA858::fn_802FB69C before (fn_8030275C) and after
// (fn_803022C4) its tint boxes: the record, the player, fn_802FB69C's flag
// byte and whether field_0x134[player] is set.
void fn_803022C4(cls_804AA858Sub11C* record, s32 player, u8 flag, bool set);
void fn_8030275C(cls_804AA858Sub11C* record, s32 player, u8 flag, bool set);
// Also tu_80301CD0's, read by TileMap_804AA858::fn_802FF0EC per player:
// fn_80301D30's state (1 / 2), a byte counter (>> 1, or (0x10 - it) << 4)
// and two s32 tables compared for equality.
u8 fn_80301D30(s32 player);
extern u8 lbl_8053ACF0[4];
extern s32 lbl_80506D88[4];
extern s32 lbl_80506D98[4];

// The element classes of TileMap_804AA858's arrays (constructed with
// __construct_array in its constructor, fn_802FFDEC). Each is named after
// its out-of-line constructor, the only code of its own.

// 4 x 3 at 0x150 / 0x190 / 0x1D0 (__construct_array 0x10 x 4); empty
// constructor. fn_802FF798 fills the 0x1D0 ones with x, y, w, h words.
class cls_803004C0 {
public:
    cls_803004C0();

    /* 0x00 */ s32 field_0x0;
    /* 0x04 */ s32 field_0x4;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ s32 field_0xc;
}; // Size: 0x10

// 0x100 at 0x2398: a timed sprite effect that fn_802F6800 adds (the first
// entry with mTimer 0) from params' field_0x30 kind; fn_802FF568 counts
// mTimer down each frame.
class cls_803003AC {
public:
    cls_803003AC();

    /* 0x00 */ s32 mTimer;    // the kind's duration, counted down
    /* 0x04 */ s32 mDuration; // mTimer's start value
    /* 0x08 */ f32 mScale;    // per kind; fn_802F6800 gives mMtx an identity when it is not 1.0f
    /* 0x0C */ JGeometry::TVec3f mPos;
    // The draw arguments of the sprite it came from (cls_8030029C's).
    /* 0x18 */ void* mRes;
    /* 0x1C */ s32 mFrame;
    /* 0x20 */ void* mImage;
    /* 0x24 */ void* field_0x24;
    /* 0x28 */ SpriteParams_801F5AE0 mParams;
    /* 0x64 */ Mtx mMtx; // mParams.mMtx points here when set
    // params' field_0x10 times the kind's colour / 255, and params'
    // field_0x14 plus the kind's colour clamped to 0xFF (fn_802F6800). The
    // constructor's two -1 stores after mParams are TColor's default constructor.
    /* 0x94 */ JUtility::TColor mColor;
    /* 0x98 */ JUtility::TColor mAddColor;
}; // Size: 0x9C

// 0x10 at 0xBF9C: a slot that fn_802F601C fills (mKind 0 is free, else the
// slot with the smallest mTimer is reused).
class cls_803003A8 {
public:
    cls_803003A8();

    /* 0x00 */ s32 mKind;
    /* 0x04 */ JGeometry::TVec3f mPos;
    /* 0x10 */ s32 mTimer;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ JGeometry::TVec3f field_0x1c;
}; // Size: 0x28

// 0x20 x 2 at 0xC220 / 0xD324: a queued sprite. fn_802FF3F0 / fn_802FF2D8
// draw it with fn_802F86CC (mRes, mFrame), else fn_802F745C (mImage), else
// fn_802F6CF4 (field_0x18), passing &mParams.
class cls_8030029C {
public:
    cls_8030029C();

    /* 0x00 */ JGeometry::TVec3f mPos;
    /* 0x0C */ void* mRes;
    /* 0x10 */ s32 mFrame;
    /* 0x14 */ void* mImage;
    /* 0x18 */ void* field_0x18;
    /* 0x1C */ SpriteParams_801F5AE0 mParams;
    // fn_802F6298 / fn_802F654C: a copy of params' matrix, mParams.mMtx then
    // points here.
    /* 0x58 */ Mtx mMtx;
}; // Size: 0x88

// gGameRoot2_8053AB10->0x20. Named after its vtable (0x804AA858, one slot);
// allocated with __nw__(0xE464, heap, 0) in game/tu_8013AAD4.cpp
// (0x8013C230-0x8013C25C).
class TileMap_804AA858 {
public:
    // fn_802FFDEC; tu_8013AAD4 passes the heap it allocated the object from
    // (r30, also __nw__'s heap). Proven: fn_802FFDEC calls getTotalFreeSize__7JKRHeapFv on it.
    TileMap_804AA858(JKRHeap* heap);
    /* 0x08 */ virtual ~TileMap_804AA858(); // fn_802FFBF8

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
    // 0x802F6298 / 0x802F654C: queue a sprite (pos, the "SOB " resource and
    // frame, or the image of fn_802F745C, or the fn_802F6CF4 resource) with a
    // copy of params (field_0x22 / field_0x21 cleared so the queued draw does
    // not queue again; its matrix copied into the entry) on field_0xd324 /
    // field_0xc220; false when the queue's 0x20 entries are taken. The draws
    // call them when params' byte 0x22 / 0x21 is set.
    u8 fn_802F6298(const JGeometry::TVec3f* pos, void* res, s32 frame, void* image, void* res3,
                     SpriteParams_801F5AE0* params);
    u8 fn_802F654C(const JGeometry::TVec3f* pos, void* res, s32 frame, void* image, void* res3,
                     SpriteParams_801F5AE0* params);
    // 0x802F6800: called by the three draws (same arguments) when params'
    // field_0x30 is not -1; adds a field_0x2398 effect of that kind (tables
    // indexed by field_0x30) on frames where the sprite's resource address
    // (res + image + res3) matches the frame counter modulo its period.
    u8 fn_802F6800(const JGeometry::TVec3f* pos, void* res, s32 frame, void* image, void* res3,
                     SpriteParams_801F5AE0* params);
    // 5 units (act_shutter, act_red_blue_switch, act_spike_floor_check,
    // tu_802D39B8, tu_8029D898) plus BgRewrite's tile animation. Decodes
    // the 'SCH ' resource `res` (JKRDecomp::checkCompressed / decode, size
    // from getResSize when `size` is 0) and rewrites the 4-bit tile data
    // at this + 4 + slot * 4 + dstOffset from res + srcOffset (0x802FA160).
    // Callers pass (res, 0 / 0xC0 / 0x180, 0, 0x40, 0xC0).
    void fn_802FA160(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size);
    // 0x802F99DC: loads the u16 tile grid of `layer` (little-endian in res)
    // at byte dstOffset; tu_802D39B8 passes (res, 0, layer, 0, 0).
    void fn_802F99DC(void* res, u32 srcOffset, s32 layer, u32 dstOffset, u32 size);
    // 0x802F9BE8: loads the u16 grid field_0x24 the same way, from byte
    // `start` on (tu_802D39B8).
    void fn_802F9BE8(void* res, u32 srcOffset, u32 start, u32 size);
    // 0x802F9DA8: fn_802F93F4 for the sprite palettes field_0xc / field_0x18
    // of slot 0-2 (tu_802D39B8).
    void fn_802F9DA8(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size, bool flag);
    // 0x802F53F4. tu_80301CD0 (0x803051A0, 0x80305DEC). Keeps the record of
    // player `slot` in field_0x11c[slot] and hands it to fn_803068D0.
    void fn_802F53F4(s32 slot, const cls_804AA858Sub11C* record);
    // 0x802F5570: the position (out) and parameters (*outParams: params, or
    // tmp with mtx) a sprite at pos is drawn with (fn_802F86CC 0x802F8944).
    void fn_802F5570(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params,
                     JGeometry::TVec3f* out, SpriteParams_801F5AE0** outParams, Mtx mtx,
                     SpriteParams_801F5AE0* tmp);
    // 0x802F58A8: when pos is on the height grid, mtx = the ground's tilt
    // there (cross products of the grid's slopes); returns whether it did.
    bool fn_802F58A8(const JGeometry::TVec3f* pos, Mtx mtx);
    // 0x802F601C: starts an effect of `kind` (1-6; 4 becomes 5 with scaleX
    // negated) at pos in a field_0xbf9c slot; kind 3 keeps dir normalized
    // (r6 is read as a TVec3f only there). act_warp_cannon passes (1, NULL,
    // 1.0f, -2.0f).
    void fn_802F601C(const JGeometry::TVec3f* pos, s32 kind, const JGeometry::TVec3f* dir, f32 scaleX,
                     f32 scaleY);
    // 0x802F9714 (game/tu_802F5040.cpp): the same arguments as fn_802FA160
    // (checkCompressed / getResSize for size 0); writes the nibble-swapped
    // bytes to field_0x88[slot] + dstOffset and flushes them; tu_8039003C's
    // constructor passes the 'SCH ' resources (slots 0, 0, 2, 3), as does
    // tu_803A85E4's (slots 0, 0, 2, 3; dstOffset 0 / 0x7000 / 0x4000 / 0).
    void fn_802F9714(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size);
    // 0x802F93F4 (game/tu_802F5040.cpp, not written yet): the 'SCL ' form of
    // fn_802F9714 with a trailing byte flag (clrlwi. 24 at 0x802F9568); the
    // constructor of tu_8039003C and tu_803A85E4's pass false.
    void fn_802F93F4(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size, bool flag);
    // 0x802FF548 (game/tu_802F5040.cpp): r3 only, passed on unchanged to
    // fn_802FF798. Called by tu_8013AAD4's fn_8013B4C0 with field_0x20.
    void fn_802FF548();
    // 0x802FF798 (game/tu_802F5040.cpp): resets the map and sets up its
    // GX texture and palette objects; no arguments;
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
    // 0x802F6BBC: copies the 0x1000 tiles of layer src to layer dst (the
    // same layer branches); tu_802D39B8 passes (0, 4) and (1, 5).
    void fn_802F6BBC(s32 src, s32 dst);
    // 0x802FF0EC: draws the four players' panels (fn_802FB69C, for each
    // field_0x148[player] set outside screen mode 12), with field_0xfc's
    // window around field_0x21c-0x228 first when field_0x214 is set and
    // field_0x230 is that player or -1 (all); clears field_0xbf98. Called by
    // GameRoot2_8049F108::fn_8013B538 / fn_8013B618 with field_0x20.
    void fn_802FF0EC();
    // 0x802FAC10 (not written yet; 99.2 attempt differed in opcodes): draws the panel tile grid field_0x48[layer] (2 x 2 blocks of
    // 16 x 16, field_0x4[1]'s colour-index texture, palette set field_0x18[1]
    // for layers 0 / 8 or a nonzero flag, else field_0x18[2]) scrolled by (x, y),
    // tinted grey / alpha unless both are 0xFF; offsets (or NULL): a shift per
    // screen line (0xA0), averaged over each 8-line row. Called only by fn_802FB69C.
    void fn_802FAC10(s32 layer, s32 x, s32 y, s32 flag, u8 grey, u8 alpha, const s32* offsets);
    // 0x802FB69C: one player's panel, from fn_802FF0EC:
    // the player, its field_0x11c record, field_0x150 / field_0x190 rects,
    // field_0x210 byte (tested clrlwi.), a count (cmpwi 0, + 1) and an
    // alpha (0xFF - alpha).
    void fn_802FB69C(s32 player, cls_804AA858Sub11C* record, cls_803004C0* rect, cls_803004C0* rect2,
                     u8 flag, s32 count, u8 alpha);
    // 0x802FF3B0 (game/tu_802F5040.cpp): no arguments. Calls fn_802FA3FC
    // when byte 0xE451 is set and gGameRoot_8053AAF8->field_0x4 is not NULL.
    void fn_802FF3B0();
    // 0x802FA3FC (game/tu_802F5040.cpp, not written yet): no arguments; reads
    // this's 0x2368 / 0x2374 / 0x235C / 0x2380 fields (r15 = r3 on entry).
    // Called by fn_802FF3B0 (the only caller seen).
    void fn_802FA3FC();

    // 0x802F5E9C. The height under pos: pos->z plus the bilinear blend of
    // the z of the four field_0x100[field_0x2388] grid points (8-unit cells,
    // 65 per row) around pos, when field_0xc21c is positive and pos lies in
    // the field_0x2380 x field_0x2384 cells from field_0x2368.
    f32 fn_802F5E9C(const JGeometry::TVec3f* pos);
    // 0x802F6CF4: the third sprite kind of the queues (cls_8030029C::field_0x18),
    // called like fn_802F745C (pos, the resource, &params).
    void fn_802F6CF4(const JGeometry::TVec3f* pos, void* res, SpriteParams_801F5AE0* params);
    // 0x802F91AC: loads the view matrix of a sprite at pos (fn_802F86CC
    // passes its frame offset); *loaded is set when it is the sprite's own.
    void fn_802F91AC(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, bool* loaded, f32 offset);
    // 0x802F8F0C: the GX state of a sprite quad drawn with params (or the
    // defaults for NULL); `this` is unused.
    void fn_802F8F0C(SpriteParams_801F5AE0* params);
    void fn_802FC80C(); // 0x802FC80C, called by fn_802FF3F0
    // 0x802FC268: draws the tile grid `grid` of `layer` (converted to a float
    // offset) at origin (PSMTXTrans, then the 16 palettes of field_0x18).
    void fn_802FC268(const JGeometry::TVec3f* origin, s32 layer, u16* grid);
    void fn_802FCE74(); // 0x802FCE74, called by fn_802FF568 in the game modes of isGameModeSet()
    void fn_802FDDD4(); // 0x802FDDD4, called by fn_802FF568 and fn_802FF798
    void fn_802FEE68(); // 0x802FEE68, called by fn_802FF2D8
    // 0x802FF2D8: draws the field_0xd324 queue, empties it, then fn_802FEE68.
    void fn_802FF2D8();
    // 0x802FF3F0: sets the camera's screen rectangle, fn_802FC80C, then draws
    // the field_0xc220 queue and empties it.
    void fn_802FF3F0();
    // 0x802FF568: the per-frame update (fn_802FDDD4, timers of field_0x2398).
    void fn_802FF568();

    /* 0x0000 vtable */
    // The buffers the constructor allocates on `heap` (__nwa, alignment 0x20;
    // sizes in the comments), freed by the destructor in reverse order.
    /* 0x0004 */ u8* field_0x4[2];   // 0x8000
    /* 0x000C */ u8* field_0xc[3];   // 0x200
    /* 0x0018 */ u8* field_0x18[3];  // 0x200
    /* 0x0024 */ u16* field_0x24;    // 0x1000 (fn_802F9BE8 stores u16s)
    // The u16 tile grids of layers 0-3 and 4-7 (fn_802F9908 / fn_802F9974
    // test the two ranges separately; 32 x 32 cells). 0x2000 each.
    /* 0x0028 */ u16* mLayerGrid[4];
    /* 0x0038 */ u16* mLayerGrid2[4];
    // u16 tile grids of 2 x 2 blocks of 16 x 16 (0x2000 each; fn_802FAC10 draws one).
    /* 0x0048 */ u16* field_0x48[16];
    // 4-bit tile images (0x8000 each): fn_802F9714 / fn_802F93F4 write them,
    // fn_802FF798 makes them the GXInitTexObjCI images of field_0xb5c.
    /* 0x0088 */ u8* field_0x88[4];
    /* 0x0098 */ u8* field_0x98[12]; // 0x200
    // Palettes (0x200 each, 16 of 0x20): the GXInitTlutObj data of field_0x25c.
    /* 0x00C8 */ u8* field_0xc8[12];
    // 0x8000: where fn_802F9714 decodes compressed resources (JKRDecomp::decode).
    /* 0x00F8 */ u8* field_0xf8;
    /* 0x00FC */ cls_804B1668* field_0xfc; // new(0xB0); deleted through its vtable
    // 65 x 65 grid points (0x1081 TVec3f / TColor each, __construct_new_array).
    /* 0x0100 */ JGeometry::TVec3f (*field_0x100[2])[65];
    /* 0x0108 */ JUtility::TColor* field_0x108[2];
    /* 0x0110 */ JGeometry::TVec2f* field_0x110; // 0x1000 (4 x 64 x 16, filled by fn_802FF798)
    /* 0x0114 */ u16 field_0x114[4]; // fn_802F9DA8: the first colour of each sprite palette set
    /* 0x011C */ cls_804AA858Sub11C* field_0x11c[4];
    /* 0x012C */ u8 field_0x12c[4]; // set by fn_802F53F4 for the slot it fills
    /* 0x0130 */ u8 field_0x130[4]; // fn_802F53F4: record bits 0x4 >> 5 (mode 3)
    /* 0x0134 */ u8 field_0x134[4]; // fn_802F53F4: record bit 0x5 >> 7 (mode 2)
    /* 0x0138 */ s32 field_0x138[4];
    /* 0x0148 */ u8 field_0x148[4];
    /* 0x014C */ u8 field_0x14c[4];
    /* 0x0150 */ cls_803004C0 field_0x150[4];
    /* 0x0190 */ cls_803004C0 field_0x190[4];
    /* 0x01D0 */ cls_803004C0 field_0x1d0[4];
    /* 0x0210 */ u8 field_0x210[4];
    /* 0x0214 */ u8 field_0x214; // 2 units compare it with 1 (act_player_sword_level_check_npc 0x802F1350)
    /* 0x0215 */ u8 field_0x215;
    /* 0x0216 */ u8 field_0x216; // 9 units set it to 1 (act_four_switch_monitor 0x802A9E78)
    /* 0x0217 */ u8 field_0x217[4]; // tu_80418B64 stores them (0x8041AEBC, 0x8041F43C-0x8041F48C)
    /* 0x021B */ u8 field_0x21b;
    // A rectangle (x, y, w, h: fn_802FCE74 stores it, fn_802FF0EC passes it
    // as floats to field_0xfc's fn_80432750).
    /* 0x021C */ s32 field_0x21c;
    /* 0x0220 */ s32 field_0x220;
    /* 0x0224 */ s32 field_0x224;
    /* 0x0228 */ s32 field_0x228;
    // 0x22C-0x244: the window of one player (tu_80441318's setWindow, from
    // fn_80441318 / update): field_0x22c is the window index, field_0x234 a
    // flag (0, or MessageManager's byte 0xEE), field_0x238 / field_0x23c the
    // top-left corner (centred in the screen) and field_0x240 / field_0x244
    // the size (365 x 261).
    /* 0x022C */ s32 field_0x22c;
    /* 0x0230 */ s32 field_0x230; // 2 units read it (tu_80432638 0x80432BE4)
    /* 0x0234 */ s32 field_0x234;
    /* 0x0238 */ s32 field_0x238;
    /* 0x023C */ s32 field_0x23c;
    /* 0x0240 */ s32 field_0x240;
    /* 0x0244 */ s32 field_0x244;
    // When set, fn_802FF3F0 gives the camera's screen rectangle
    // (Camera_804AA9E4::field_0x144) field_0x24c instead of the full screen.
    /* 0x0248 */ u8 field_0x248;
    /* 0x024C */ ScreenRect_802FF3F0 field_0x24c;
    /* 0x025C */ GXTlutObj field_0x25c[3][4][16];
    /* 0x0B5C */ GXTexObj field_0xb5c[3][4][16];
    // Per-player bytes: tu_80139BE8 (0x80139C24), tu_802D39B8, tu_803A8DFC
    // and tu_803B7DCC store them; Camera_804AA9E4::fn_8030C85C /
    // fn_8030C928 copy them to field_0x2360.
    /* 0x235C */ u8 field_0x235c[4];
    /* 0x2360 */ u8 field_0x2360[4];
    // fn_802F86CC draws nothing while it is 0; 6 units set it to 1
    // (tu_802D39B8 0x802D9BFC).
    /* 0x2364 */ u8 field_0x2364;
    /* 0x2365 */ u8 field_0x2365;
    // Camera_804AA9E4::fn_8030C928 moves field_0x2368 to field_0x2374 and
    // stores the new box's corner (x0, y0, constant) in field_0x2368;
    // tu_8016A2B4 reads it (0x8016E014).
    /* 0x2368 */ JGeometry::TVec3f field_0x2368;
    /* 0x2374 */ JGeometry::TVec3f field_0x2374;
    // Written by tu_802D39B8 (0x802D92D4), read as ints by tu_8016A2B4
    // (0x8016AE44). The cell counts of fn_802F5E9C's grid (64 after fn_802FF798).
    /* 0x2380 */ s32 field_0x2380;
    /* 0x2384 */ s32 field_0x2384;
    /* 0x2388 */ s32 field_0x2388; // which field_0x100 grid fn_802F5E9C reads
    /* 0x238C */ u8* field_0x238c; // __nwa(0x100)
    /* 0x2390 */ s32 field_0x2390;
    /* 0x2394 */ s32 field_0x2394;
    /* 0x2398 */ cls_803003AC field_0x2398[0x100];
    /* 0xBF98 */ u8 field_0xbf98;
    /* 0xBF9C */ cls_803003A8 field_0xbf9c[16];
    /* 0xC21C */ s32 field_0xc21c; // fn_802F5E9C reads the grid only while it is positive
    // Two sprite queues: fn_802FF3F0 / fn_802FF2D8 draw the field_0xd320 /
    // field_0xe424 entries and empty them.
    /* 0xC220 */ cls_8030029C field_0xc220[0x20];
    /* 0xD320 */ s32 field_0xd320;
    /* 0xD324 */ cls_8030029C field_0xd324[0x20];
    /* 0xE424 */ s32 field_0xe424;
    /* 0xE428 */ JUTTexture* field_0xe428[10];
    /* 0xE450 */ u8 field_0xe450;
    // fn_802FF3B0 tests it before calling fn_802FA3FC (the only reader seen).
    /* 0xE451 */ u8 field_0xe451;
    /* 0xE452 */ u8 field_0xe452[0xE454 - 0xE452];
    /* 0xE454 */ JUTTexture* field_0xe454;
    /* 0xE458 */ f32 field_0xe458;
    /* 0xE45C */ f32 field_0xe45c;
    /* 0xE460 */ u8 field_0xe460;
    /* 0xE461 */ u8 field_0xe461;
    /* 0xE462 */ u8 field_0xe462[0xE464 - 0xE462];
}; // Size: 0xE464

// --- tile queries (game/tu_8022461C.cpp) ---
extern "C" {
// Owner tu_8022461C (0x8022486C, not written yet). A tile test at pos (byte result, callers
// clrlwi.): Player fn_8027BE74 picks the sound 0x20062 / 0x20061 by it.
bool fn_8022486C(const JGeometry::TVec3f* pos, s32 layer);
// 73 units. The tile's collision code: the tile code (as in fn_80226850)
// mapped through the u16 table lbl_8046CB88, adjusted when the layer's
// GameView byte 0x131DF + layer is set (0 -> 2, 0x61 -> 0) and on stage
// (7, 2) / (7, 3) (0xE -> 0x51); 0xF outside the box. Callers clrlwi 16
// the result.
u16 fn_8022461C(const JGeometry::TVec3f* pos, s32 layer);
// The tile code -> collision code table (0x800 bytes; .rodata of
// game/act_fire_rod_fire.cpp). Also read directly by tu_8029D898 (fn_802A1490, fn_802A1554).
extern const u16 lbl_8046CB88[0x400];
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
bool fn_80225000(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80224D0C(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80224A04(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80224B88(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80224E88(const JGeometry::TVec3f* pos, s32 layer);
// 1 unit (Player_804A6F38::fn_80285F44 picks the splash effects by it).
bool fn_80225BE4(const JGeometry::TVec3f* pos, s32 layer);
// 1 unit (tu_80221B44 picks effect 0x3d0 or 0x3cf by them). Position in r3,
// layer in r4; return a byte.
bool fn_802258D4(const JGeometry::TVec3f* pos, s32 layer);
bool fn_80225614(const JGeometry::TVec3f* pos, s32 layer);
// tu_8022461C (not written yet): tests the x/y of pos against the box of
// `layer` (fn_801CCBD0's family); fn_802255F4 forwards to it.
bool fn_802252F4(const JGeometry::TVec3f* pos, s32 layer);
}

// tu_8022461C (not written yet): whether the raw tile at the x/y of pos (0xF
// outside the box of `layer`) is one of the codes listed in lbl_8046AAB0.
// Player_804A6F38::fn_802527B0 calls it with layer 0.
bool fn_80225EF4(const JGeometry::TVec3f* pos, s32 layer);

// Forwards to fn_802252F4 (tu_8022461C); a C++ name in symbols.txt. Player_804A6F38::fn_802870C8
// tests it for the 0xD7 trail emitter.
bool fn_802255F4(const JGeometry::TVec3f* pos, s32 layer);

// --- actor collision boxes (game/tu_8020D58C.cpp) ---

// One registered box. Its empty out-of-line constructor is
// __ct__21CollisionBox_8020D65C (the list constructor's __construct_array,
// 0x50 x 0x18), hence the name.
struct CollisionBox_8020D65C {
    CollisionBox_8020D65C(); // out-of-line, empty (game/tu_8020D58C.cpp)

    /* 0x00 */ JGeometry::TBox2f mBox;
    /* 0x10 */ s32 mLayer;  // the actor's field_0x4 (fn_8020D7D0)
    /* 0x14 */ s32 mSerial; // the actor's mSerial; fn_8020D6AC skips its own
}; // Size: 0x18

// Named after its constructor __ct__25CollisionBoxList_8020D60C (count = 0,
// __construct_array of the boxes).
class CollisionBoxList_8020D60C {
public:
    CollisionBoxList_8020D60C(); // out-of-line (game/tu_8020D58C.cpp)

    // 20 units / 22 calls. Whether pos (x, y) lies in a box on `layer`
    // registered by an actor other than `serial`; with ignoreIds (a list
    // ending in 'NULL', cmplw against ActorBase_804A5770::mActorId of the
    // box's actor, findActorBySerial_801F666C), boxes of those actors don't count.
    // Returns 0 or 1. Callers use the read list (field_0x1).
    bool fn_8020D6AC(s32 layer, const JGeometry::TVec3f* pos, s32 serial, const u32* ignoreIds) const;
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
    // The function-local static's constructor, inlined by fn_8020D58C: the
    // lists are constructed first (__construct_array), then reset.
    CollisionBoxes_8020D58C() {
        field_0x0 = 0;
        field_0x1 = 1;
        mLists[0].mCount = 0;
        mLists[1].mCount = 0;
    }

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
CollisionBoxes_8020D58C* fn_8020D58C();

// game/tu_802F5040.cpp. The GX setup of a sprite at pos with params (or NULL);
// writes the draw offset to out. Called by the sprite draws of this unit and
// by tu_801618D0 (0x801619F4).
void fn_802F5040(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, JGeometry::TVec3f* out);

#endif
