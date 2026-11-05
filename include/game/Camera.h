#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

// The 2D camera of game/tu_8030C058.cpp: one instance, allocated with
// __nw__(0x85C, heap, 0) and built by its constructor fn_8030E718 in
// game/tu_8013AAD4.cpp (0x8013C264-0x8013C280), stored at
// lbl_8053AB10->0x24. 94 game units use the unit, 77 of them through
// fn_8030C210.
//
// "Camera_804AA9E4" is a provisional name carrying its vtable address
// (docs/PLAN.md, "Placeholder names"; it was cls_8030C058, after its first
// function, before the vtable was found: the constructor stores 0x804AA9E4
// at 0). Why a camera: field_0x4 is a view matrix whose translation is
// minus the centre of the box it is built from (0x8030CB0C-0x8030CC14: an
// identity with m[0][3] = -(x0 + x1) / 2), followed by an orthographic
// projection (C_MTXOrtho into field_0x34) that the unit loads with
// GXSetProjection / GXLoadPosMtxImm; fn_8030C5F0 projects world positions
// to the screen; field_0x154 is the visible area and fn_8030C210 collects
// the points the camera has to keep in view.
//
// Conventions follow GameView.h: members for the exports other units call
// with the camera in r3 (all pass lbl_8053AB10->0x24), with their mangled
// names in symbols.txt, so a changed parameter type is a symbols.txt rename
// too. "N units" counts the other game units calling the function or
// touching the field. Vectors are JGeometry::TVec3f (which derives from
// Vec) unless a mangled name says Vec.
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function showing it.

#include "types.h"
#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"

// The screen rectangle of Camera_804AA9E4::field_0x144 (x0, y0, x1, y1 as
// unsigned ints, converted to float with the unsigned idiom: fn_8030C2A8 tests a point against x0+0xE..x1-0xE and y0..y1). No code of its own; named after the first function that builds
// one (cls_804AA858::fn_802FF3F0, game/tu_802F5040.cpp: copied whole from
// cls_804AA858::field_0x24c, or (0, 0, fbWidth, efbHeight) built on the stack).
struct ScreenRect_802FF3F0 {
    // All four convert to float unsigned (GameRoot2_8049F108::fn_8013BBE0).
    /* 0x0 */ u32 x0;
    /* 0x4 */ u32 y0;
    /* 0x8 */ u32 x1; // cmplwi against 0 (0x802FF420)
    /* 0xC */ u32 y1;
}; // Size: 0x10

class Camera_804AA9E4 {
public:
    // fn_8030E718 (tu_8013AAD4 only): stores the vtable, sets field_0xe4 and
    // field_0x114 to identity and constructs field_0x1f0 (__construct_array
    // with TVec3<f>'s constructor, 0x80 x 0xC).
    Camera_804AA9E4();
    // fn_8030E6D0; the only slot of the vtable at 0x804AA9E4.
    /* 0x08 */ virtual ~Camera_804AA9E4();

    // 77 units / 122 calls. If fewer than 0x80 points are queued and pos
    // (x, y) lies in field_0x154, appends pos to field_0x1f0 (count
    // field_0x7f0); returns 0 or 1. Actors pass &mPos, mostly for layers
    // below 8 (act_arrow 0x80165FD8).
    u8 fn_8030C210(const JGeometry::TVec3f* pos);
    // 3 units. The same for both corners of box (reads 0x0-0xC); it queues
    // them only when both lie in field_0x154. TBox2f is a guess: two
    // TVec2f would read the same.
    bool fn_8030C058(const JGeometry::TBox2f* box);
    // 9 units / 24 calls. Projects pos with fn_8030C5F0(pos, true, false,
    // &tmp) and tests tmp (x, y) against the screen rectangle field_0x144
    // shrunk by 0xE on each side; returns 0 or 1. pos goes straight on to
    // fn_8030C5F0, so const Vec* would compile too: TVec3f (callers pass
    // &mPos, act_door 0x802C73AC) is a guess.
    bool fn_8030C2A8(const JGeometry::TVec3f* pos);
    // Multiplies pos by the Mtx at +0x74 (PSMTXMultVec) and writes x, y and a
    // constant z to out: a world-to-screen projection. With the 2nd argument
    // false it scales by JUTVideo's frame size (progress/game__tu_8045BA84.md).
    void fn_8030C5F0(const Vec* pos, bool, bool, Vec* out);
    // 2 units / 10 calls (this unused: the body reads the camera through
    // lbl_8053AB10 again). Sets field_0x180 to the box of half-size
    // size * k around center, with y scaled by the video frame's aspect
    // (JUTVideo's height / width), and field_0x17e to 1. Reads center 0x0
    // and 0x4 only (tu_8011CEC8 passes a stack pair, 0x8011D008).
    void fn_8030C7C4(const JGeometry::TVec2f* center, f32 size);
    // 1 unit (GameView fn_802D9CB0). Clears field_0x1e8, zeroes each of the
    // map's bytes 0x2360-0x2363 whose byte 0x235C-0x235F is set, clears the
    // view (fn_802DCDF4) and copies field_0x190 to field_0x154.
    void fn_8030C85C();
    // 1 unit (GameView fn_802DCD1C passes &mVisibleBox). Stores box in
    // field_0x190 and updates the map object's scroll state (its 0x2360 and
    // 0x2368 / 0x2374, see Map.h).
    void fn_8030C928(const JGeometry::TBox2f* box);
    // 1 unit (GameView fn_802DD3D4 passes &mVisibleBox). Starts a screen
    // change towards box: field_0x1e8 = 1, 0x1EC = 30, field_0x7f5 = 0
    // (0x8030CA14-0x8030CA2C).
    void fn_8030C9E4(const JGeometry::TBox2f* box);
    // Called once per frame from game/tu_8013AAD4.cpp only (no arguments
    // read; the caller ignores r3).
    void fn_8030DD68();
    // 2 units: tu_8013AAD4 at 0x8013AE74 with a stack matrix, and
    // fn_8013B3CC, which passes its own r4 on. Concatenates field_0x74 with m into a temporary
    // (PSMTXConcat), makes GX_PNMTX9 the current matrix and loads the result
    // as its position and normal matrix.
    void fn_8030DD20(const Mtx m);
    // 0x8030DD8C (game/tu_8030C058.cpp, not written yet). Sets the screen
    // box (reads 0x0-0xC of box) as the ortho projection; when `light`
    // is set also builds the light matrices. Called by
    // GameRoot2_8049F108::fn_8013B414, which passes its arguments on.
    void fn_8030DD8C(const JGeometry::TBox2f* box, bool light);
    // 0x8030DFE8 (game/tu_8030C058.cpp, not written yet). Loads the
    // projection, position matrix 0x74 and viewport / scissor from field_0x144;
    // `copy` first copies the matrices from 0x4 / 0x34 into 0x74 / 0xA4.
    void fn_8030DFE8(bool copy);
    void fn_8030E150(); // empty
    void fn_8030E154();
    void fn_8030E5BC();

    /* 0x000 vtable */
    // The view matrix: identity with the translation (-centre x, -centre y,
    // z); 12 units read -m[0][3] / -m[1][3] as the camera position
    // (act_all_enemies_defeated 0x8019D2D4 / 0x8019D2E0).
    /* 0x004 */ Mtx field_0x4;
    /* 0x034 */ Mtx44 field_0x34; // C_MTXOrtho (0x8030CC18)
    /* 0x074 */ Mtx field_0x74;   // fn_8030C5F0's PSMTXMultVec; GXLoadPosMtxImm
    /* 0x0A4 */ Mtx44 field_0xa4; // GXSetProjection (0x8030DEFC)
    // Copies of the view (PSMTXCopy at 0x8030CC24) and a C_MTXLightOrtho
    // (0x8030CC4C); identity after the constructor. tu_8013C5C8 copies
    // both word by word (0x8013D718-0x8013D7CC).
    /* 0x0E4 */ Mtx field_0xe4;
    /* 0x114 */ Mtx field_0x114;
    // The screen rectangle (x0, y0, x1, y1) as unsigned ints: written by tu_802F5040
    // (0x802FF438), read by fn_8030C2A8.
    /* 0x144 */ ScreenRect_802FF3F0 field_0x144;
    // The visible area: fn_8030C210 / fn_8030C058 test against it,
    // tu_802D39B8 writes it (0x802D9F64), fn_8030C85C resets it from
    // field_0x190.
    /* 0x154 */ JGeometry::TBox2f field_0x154;
    // 3 units copy it word by word to the stack (tu_8029C584 0x8029C5CC);
    // tu_8023FBF0 reads its floats.
    /* 0x164 */ JGeometry::TBox2f field_0x164;
    /* 0x174 */ u8 field_0x174[0x178 - 0x174];
    // Request flags, each set to 1 by actors (stb) and polled by the unit:
    // 0x178 6 units, 0x179 9 units (also read: act_vaati_boss sets 0x178
    // only while 0x179 is 0, 0x801D4A58), 0x17A 3, 0x17B 1, 0x17C 2,
    // 0x17D 5 (act_great_fairy_spfr 0x80132230).
    /* 0x178 */ u8 field_0x178;
    /* 0x179 */ u8 field_0x179;
    /* 0x17A */ u8 field_0x17a;
    /* 0x17B */ u8 field_0x17b;
    /* 0x17C */ u8 field_0x17c;
    /* 0x17D */ u8 field_0x17d;
    /* 0x17E */ u8 field_0x17e; // set by fn_8030C7C4
    /* 0x180 */ JGeometry::TBox2f field_0x180; // fn_8030C7C4
    /* 0x190 */ JGeometry::TBox2f field_0x190; // fn_8030C928; fn_8030C85C
    /* 0x1A0 */ u8 field_0x1a0[0x1B4 - 0x1A0];
    // fn_8030C85C sets all four floats to one constant.
    /* 0x1B4 */ JGeometry::TBox2f field_0x1b4;
    /* 0x1C4 */ u8 field_0x1c4[0x1D4 - 0x1C4];
    // 19 units store both (act_escort_woman 0x80125048), next to
    // field_0x840.
    /* 0x1D4 */ f32 field_0x1d4;
    /* 0x1D8 */ f32 field_0x1d8;
    // GameView_804A9950::vfunc_0x10 stores x, y and a constant z
    // (0x802DD594-0x802DD59C).
    /* 0x1DC */ JGeometry::TVec3f field_0x1dc;
    // Read by GameView fn_802DCD0C / fn_802DA000; cleared by fn_8030C85C.
    /* 0x1E8 */ u8 field_0x1e8;
    /* 0x1E9 */ u8 field_0x1e9[0x1F0 - 0x1E9];
    /* 0x1F0 */ JGeometry::TVec3f field_0x1f0[0x80]; // fn_8030C210's points
    /* 0x7F0 */ s32 field_0x7f0;                     // their count
    // A scripted camera move, set by 37 units when field_0x7f5 is 0: 0x7F4 = 1,
    // a mode in 0x7F8 (4 with a target position in 0x7FC, 24 units,
    // act_crystal_demo_2 0x80369128; 5 with a box in 0x808, 12 units,
    // act_all_enemies_defeated 0x8019D19C), 0x7F6 = 0.
    /* 0x7F4 */ u8 field_0x7f4;
    /* 0x7F5 */ u8 field_0x7f5;
    /* 0x7F6 */ u8 field_0x7f6;
    /* 0x7F8 */ s32 field_0x7f8;
    /* 0x7FC */ JGeometry::TVec3f field_0x7fc;
    /* 0x808 */ JGeometry::TBox2f field_0x808;
    /* 0x818 */ u8 field_0x818[0x840 - 0x818];
    // Set by 19 units when 0 (act_escort_woman 0x80125010-0x80125044),
    // with a position in both corners of field_0x844 (TBox2f is a guess:
    // the stores are four stfs), a float in field_0x854 and field_0x1d4 /
    // field_0x1d8.
    /* 0x840 */ u8 field_0x840;
    /* 0x844 */ JGeometry::TBox2f field_0x844;
    /* 0x854 */ f32 field_0x854;
    /* 0x858 */ f32 field_0x858; // 4 units divide by it (tu_8016A2B4 0x8016AE94)
}; // Size: 0x85C

#endif
