#ifndef GAME_TU_801E9AFC_H
#define GAME_TU_801E9AFC_H

// Code: game/tu_801E9AFC.cpp. The fire-sprite slots: one static object in
// .bss at 0x804CA358 (guard lbl_8053AC50), built on first use by
// fn_801E9AFC(). cls_801E9B54 is one of its two elements (named after its
// empty constructor, 0x801E9B54).

#include "game/act/tu_801E801C.h"
#include "dolphin/gx.h"
#include "JSystem/JGeometry.h"

// One tracked actor of an element: the FSOB actor spawned by fn_801EA75C
// (0x0), the byte it is looked up by (0x4) and the word fn_801EA6D8 stores
// (0x8, -1 when free).
struct Slot_801E9B54 {
    /* 0x0 */ cls_804A5568* actor;
    /* 0x4 */ u8 id;
    /* 0x8 */ s32 value;
}; // Size: 0xC

struct cls_801E9B54 {
    cls_801E9B54();
    /* 0x00 */ Slot_801E9B54 field_0x0[8];
    /* 0x60 */ s32 field_0x60; // used entries of field_0x0
    /* 0x64 */ s32 field_0x64[4]; // serials appended by fn_801EA6D8 (0 as id)
    /* 0x74 */ s32 field_0x74; // used entries of field_0x64
    // Frees every slot and empties the serial list.
    inline void clear() {
        for (s32 i = 0; i < 8; i++) {
            field_0x0[i].actor = NULL;
            field_0x0[i].id = 0;
            field_0x0[i].value = -1;
        }
        field_0x60 = 0;
        for (s32 i = 0; i < 4; i++) {
            field_0x64[i] = -1;
        }
        field_0x74 = 0;
    }
    // Whether any slot / any serial is used.
    bool hasSlots() const { return field_0x60 > 0; }
    bool hasSerials() const { return field_0x74 > 0; }
    // Draws the outlines (fn_801E8080) of the slots' actors.
    inline void drawOutlines();
    // Draws the slots' sprites (fn_801E801C) and then those of the actors the
    // serials name (a cls_804A5418's fn_801E6DE8).
    inline void drawSprites();
    // 0x801EA594: drops the slots whose value is -1 (releasing their actor's
    // bytes 0x11C / 0x11D), sorts the rest with fn_801EA8C0 and hands each
    // its value (fn_801E8C74).
    void fn_801EA594();
    // 0x801EA6D8: with arg0 == 0, appends arg1 to the list at 0x64 and returns
    // -1. Otherwise finds the entry whose byte at 0x4 is arg0, stores arg1 in
    // its word at 0x8, and returns the serial (0x198) of the actor its pointer
    // at 0x0 names, or -1 when there is none.
    s32 fn_801EA6D8(u8 arg0, s32 arg1);
    // 0x801EA75C: finds the entry for `id` (or adds one, spawning an FSOB
    // actor with `id` as its field_0x90 at pos on `layer`), stores a non-zero
    // `flag` in the actor's field_0x94 and sets it up (fn_801E8EB4(index, layer, pos)).
    void fn_801EA75C(u8 id, u8 flag, u8 index, s32 layer, const JGeometry::TVec2f* pos);
}; // Size: 0x78

// The object at 0x804CA358: two elements, the byte at 0xF0 that selects one
// of them, and the three textures with their palettes and aspect ratios.
struct cls_801E9AFC {
    /* 0x000 */ cls_801E9B54 field_0x0[2];
    /* 0x0F0 */ u8 field_0xf0;
    /* 0x0F4 */ GXTexObj field_0xf4[3];
    /* 0x154 */ GXTlutObj field_0x154[3];
    /* 0x178 */ f32 field_0x178[3]; // height / width of each texture
    // 0x801E9C74: draws the outlines and then the sprites of every element
    // (both when the view's flag is set, else the selected one).
    void fn_801E9C74();
    // 0x801EA108 (the original reloads the sin/cos table for every
    // vertex): draws the sprite quad at pos (x, y only) rotated by angle (an
    // index into the sin/cos table); `z` is added to the quad's y and used as
    // its z. r3 is unused.
    void fn_801EA108(const JGeometry::TVec2f* pos, s16 angle, f32 z);
    // 0x801EA304 / 0x801EA344: load the palette and texture of sprite 1 / 0.
    void fn_801EA304();
    void fn_801EA344();
    // 0x801EA384: with flag, loads the three textures and clears both elements
    // and the selector; else flips the selector and clears that element.
    void fn_801EA384(bool flag);
}; // Size: at least 0x184

// Builds the object on first use.
cls_801E9AFC* fn_801E9AFC();

// 0x801E9C00: sets *vec to (0, 4) rotated by angle plus or minus a quarter turn
// (0x2000, by the angle's sign).
void fn_801E9C00(s16 angle, JGeometry::TVec2f* vec);

#endif
