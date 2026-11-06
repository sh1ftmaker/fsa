#ifndef GAME_CLS_804A71A8_H
#define GAME_CLS_804A71A8_H

#include "game/Player.h"

// A heap object of 0xC0 bytes owned by a Player (vtable 0x804A71A8, defined
// in game/tu_802684A4.cpp). Its constructor (0x8026F9E8) stores the
// Player at 0x4; the destructor clears it. The Player's
// constructor (game/tu_8023FBF0.cpp) allocates it and keeps it in
// field_0xbf0.
class cls_804A71A8 {
public:
    // 0x8026F9E8 (tu_802684A4): r4 = the owning Player.
    cls_804A71A8(Player_804A6F38* player);

    // 0x8026F998: r4 = the delete flag; > 0 frees the object (__dl__FPv).
    virtual ~cls_804A71A8();

    // tu_802684A4: copies the owner's mPos into *pos, then, while the owner
    // is in state 2, offsets x and y by the corner table entry for its
    // fn_80248778 index (y's sign follows viewLayerFlag of its layer).
    void fn_8026947C(JGeometry::TVec3f* pos);
    // 0x80268BC4: called by the owner's fn_802685D4 each frame. Probes the
    // eight directions around the owner (lbl_8046BAE8) and hands each to
    // fn_8026C350 or fn_8026B81C, then runs fn_80269574 (and fn_8026E1E0 on
    // the first pass), repeating while fn_80269574 asks (field_0x9a).
    void fn_80268BC4();
    // tu_8023FBF0's fn_8024B274 / fn_8024B298 forward to them.
    void fn_80269420();
    void fn_8026942C(s32 index, JGeometry::TVec3f* out);
    // 0x80269574: run after each probe pass (docs/PLAYER.md, cls_804A71A8).
    void fn_80269574();
    // 0x8026B81C: fn_8026C350 for the second probe, on another player's
    // layer: every tile that does anything there only blocks the direction
    // (field_0x38), the half / diagonal / corner tiles by where the point
    // lies in its 16-unit cell, 0x25 / 0x26 by the view's field_0x171.
    void fn_8026B81C(u16 tile);
    // 0x8026C350: the tile code under a probe point inside the view, per
    // direction: blocks (field_0x38), open floor (field_0x20), ledges
    // (fn_8026E68C), pushable blocks (fn_8026EE60 family) or one of the
    // flags 0x50-0x88 that fn_80269574 acts on. Nothing but field_0xb8 for
    // the ledges 0x10-0x13 while the owner's fn_8026FC20() >= 1.
    void fn_8026C350(u16 tile);
    // 0x8026E1E0: run after the first probe pass only. Counts frames of
    // pushing into a blocked side (field_0xa0, up to 20) and frames of not
    // pushing (field_0xa4, up to 5, which clears the owner's 0xD94-0xD9C).
    void fn_8026E1E0();
    // 0x8026E464: the start-of-frame reset (inlined in fn_80268BC4 and
    // fn_8026F710); the owner's fn_80261750 (tu_8025EB18) calls it.
    void fn_8026E464();
    // 0x8026E68C (fn_8026C350, tiles 0x10-0x17): a one-way ledge. When
    // the owner may take it (fn_8026EB1C, or the other cases below), scans
    // from the probe in steps of lbl_8046B9C8[tile - 0x10] for a landing
    // tile (0, 3, 0xD, 0x39, 0x3A) inside the view and hands its 16-unit
    // cell centre to the owner's fn_8026E5F8 (the jump; inlined); tile 0x11
    // then sets the owner's field_0x441. Otherwise the direction is marked
    // in field_0x38.
    void fn_8026E68C(u16 tile);
    // 0x8026EB1C (called by fn_8026E68C): whether the owner pushes
    // (fn_80250D30) with a facing that tile 0x10-0x17 accepts (0x10-0x13
    // one facing each, 0x14-0x17 two).
    u8 fn_8026EB1C(u16 tile); // extrwi 8: a u8
    // 0x8026EC88 (fn_8026C350, tiles 0x31 + player): fn_8026EE60 for the
    // block of player `player` only (the owner's fn_80240868), its id + 1
    // going into bits 4-6 of the 'BLCK' actor's field_0x90.
    void fn_8026EC88(s32 player);
    // 0x8026EE60 (fn_8026C350, a pushable tile): pushing (fn_80250D30)
    // toward the probe with fn_8024FF84() < 0 and a free tile (0, 2 or
    // 0x50) 16 units ahead spawns a 'BLCK' actor at the probe and keeps its
    // serial in the owner's field_0xd94 (unless the probe's tile is 0x50);
    // otherwise the direction is marked in field_0x38.
    void fn_8026EE60();
    // 0x8026F000 / 0x8026F1C4 / 0x8026F388 / 0x8026F54C: fn_8026EE60 for a
    // tile that only moves toward facing 1 / 0 / 2 / 3.
    void fn_8026F000();
    void fn_8026F1C4();
    void fn_8026F388();
    void fn_8026F54C();
    // 0x8026F710: clears the per-direction flags, the counters and the
    // owner's field_0xef0; called by the owner's reset fn_80254744.
    void fn_8026F710();
    // Whether any of the eight flags at 0x48 (only the first four without
    // `all`) is 1, flags 2, 4 and 5 not counting while field_0xb4 is set.
    // Inlined by the owner's update B fn_80252FC0 (tu_8023FBF0) and in
    // tu_80256C74, tu_8025EB18 and tu_8026FC20 (an 8-pass loop unrolled by
    // 4); the owner's fn_8025197C(all) is the call with the argument (the
    // `i >= 4` test stays in its unrolled loop).
    bool anyFlag48(bool all = true) const {
        for (int i = 0; i < 8; i++) {
            if (!all && i >= 4) {
                return false;
            }
            if (field_0xb4 == true && (i == 2 || i == 4 || i == 5)) {
                continue;
            }
            if (field_0x48[i] == 1) {
                return true;
            }
        }
        return false;
    }

    // Whether the owner was pushed along x (field_0x48 at -x or +x). No
    // out-of-line copy: tu_802684A4 and the owner's fn_80261750
    // (tu_8025EB18) inline it.
    bool pushedAlongX() const {
        if (field_0x48[1] == 1) {
            return true;
        }
        if (field_0x48[0] == 1) {
            return true;
        }
        return false;
    }

    // fn_80269574 inlines (no out-of-line copies):
    // How many directions are blocked (field_0x38) or inside an owner box
    // (field_0x28).
    s32 countClosed() const {
        s32 count = 0;
        for (s32 i = 0; i < 8; i++) {
            if (field_0x38[i] != 0 || field_0x28[i] != 0) {
                count++;
            }
        }
        return count;
    }

    // Whether every direction is blocked or inside an owner box.
    bool allClosed() const {
        for (s32 i = 0; i < 8; i++) {
            if (field_0x38[i] == 0 && field_0x28[i] == 0) {
                return false;
            }
        }
        return true;
    }

    // Whether every direction is closed (field_0x78, field_0x38 or field_0x28)
    // and at least one is marked in field_0x78.
    bool closedBy78() const {
        for (s32 i = 0; i < 8; i++) {
            if (field_0x78[i] == 0 && field_0x38[i] == 0 && field_0x28[i] == 0) {
                return false;
            }
        }
        for (s32 i = 0; i < 8; i++) {
            if (field_0x78[i] != 0) {
                return true;
            }
        }
        return false;
    }

    // Whether direction i is blocked or inside an owner box (as 1).
    // Inlined in fn_80268BC4; the owner's fn_8024B24C returns it.
    bool isField98Or99Set() const { return field_0x98 || field_0x99; }

    /* 0x4 */ Player_804A6F38* field_0x4;
    // The probe point: fn_80268BC4 sets it to the owner's mPos plus the
    // offset lbl_8046BAE8[field_0x1c] and tests the tile and boxes there.
    /* 0x8 */ JGeometry::TVec3f field_0x8;
    // Zeroed by the owner's update fn_80253E6C each frame; fn_80268BC4 sets
    // it to how far the owner's mPos x / y moved during it. A TVec2f: the
    // owner's fn_802422C4 (tu_8023FBF0) returns its address as the
    // `const TVec2f*` of fn_8022DFD4.
    /* 0x14 */ JGeometry::TVec2f field_0x14;
    // The direction fn_80268BC4 is probing (0-7), the index into the
    // per-direction byte arrays below.
    /* 0x1C */ s32 field_0x1c;
    // Per-direction flags (8 probe directions, lbl_8046BAE8). fn_80268BC4
    // clears all of them except 0x30, 0x40 and 0x48 before each pass; 0x28
    // is set for a direction inside one of the owner's 0x5FC boxes, 0x38 for
    // one outside the view's box.
    /* 0x20 */ u8 field_0x20[8];
    /* 0x28 */ u8 field_0x28[8];
    /* 0x30 */ u8 field_0x30[8];
    /* 0x38 */ u8 field_0x38[8];
    // Last frame's field_0x48 (fn_80268BC4 copies it before clearing 0x48
    // and 0x30).
    /* 0x40 */ u8 field_0x40[8];
    // Bytes tested for 1 by the owner's fn_802517F8, fn_802518AC and
    // fn_8025190C (tu_8023FBF0), and in a loop by anyFlag48 (the owner's
    // update B, lbzx 0x48 + i, i < 8).
    /* 0x48 */ u8 field_0x48[8];
    /* 0x50 */ u8 field_0x50[8];
    /* 0x58 */ u8 field_0x58[8];
    /* 0x60 */ u8 field_0x60[8];
    /* 0x68 */ u8 field_0x68[8];
    /* 0x70 */ u8 field_0x70[8];
    /* 0x78 */ u8 field_0x78[8];
    /* 0x80 */ u8 field_0x80[8];
    /* 0x88 */ u8 field_0x88[8];
    /* 0x90 */ u8 field_0x90[0x98 - 0x90]; // not yet known
    // The owner's fn_8024B24C is true when either is set; fn_80269420 sets
    // 0x98. fn_80268BC4 moves 0x98 into 0x99 (unless gGameRoot_8053AAF8->0x96).
    /* 0x98 */ u8 field_0x98;
    /* 0x99 */ u8 field_0x99;
    // fn_80268BC4 repeats its probe pass while this is 1 (at most 100 passes).
    /* 0x9A */ u8 field_0x9a;
    /* 0x9B */ u8 field_0x9b[0xA0 - 0x9B]; // not yet known
    // Tested for 0 by the owner's fn_8027EEB4 (tu_8027E5BC) before it resets
    // the walk-animation counter.
    /* 0xA0 */ s32 field_0xa0;
    /* 0xA4 */ s32 field_0xa4;
    /* 0xA8 */ s32 field_0xa8;
    /* 0xAC */ s32 field_0xac;
    // fn_80268BC4's pass counter.
    /* 0xB0 */ s32 field_0xb0;
    // Returned by the owner's fn_8024F630 (lbz; blr). fn_80268BC4 moves it
    // into 0xB5 at the start of each frame.
    /* 0xB4 */ bool field_0xb4;
    // Tested for 1 by the owner's fn_80285F44 (tu_80285E58), with field_0xb4 clear.
    /* 0xB5 */ u8 field_0xb5;
    /* 0xB6 */ u8 field_0xb6;
    // Byte the owner's fn_8027EEB4 (tu_8027E5BC) tests for 0.
    /* 0xB7 */ u8 field_0xb7;
    /* 0xB8 */ u8 field_0xb8;
    /* 0xB9 */ u8 field_0xb9;
    /* 0xBA */ u8 field_0xba;
    /* 0xBC */ s32 field_0xbc;
}; // Size: 0xC0

#endif
