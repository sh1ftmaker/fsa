#ifndef GAME_ACT_ACT_KAEPORA_GAEBORA_ENDING_H
#define GAME_ACT_ACT_KAEPORA_GAEBORA_ENDING_H

// Code: game/act_kaepora_gaebora_ending.cpp.

#include "game/ActorBase.h"
#include "game/tu_8021A570.h"

class JPABaseEmitter;
class UnknownENDM_804AA0FC;

// One waypoint of the path field_0x298 walks (fn_8035F3C0); a kind of -1000
// ends the path.
struct KaeporaWaypoint {
    /* 0x00 */ s32 mKind;   // -1: ENDM camera call; < 0: field_0x230 = kind + 5; else an EETC id
    /* 0x04 */ f32 mRadius; // > 0: distance; -1/-2: x below/above; -3/-4: y below/above
    /* 0x08 */ f32 mX;
    /* 0x0C */ f32 mY;
    /* 0x10 */ f32 mZ;
    /* 0x14 */ u8 mNext;    // copied to field_0x294
    /* 0x15 */ u8 pad_0x15[3];
}; // Size: 0x18

// Named after its vtable, 0x804ADAC8.
class KaeporaGaeboraEnding_804ADAC8 : public ActorBase_804A5770 {
public:
    KaeporaGaeboraEnding_804ADAC8() {}
    /* 0x08 */ virtual ~KaeporaGaeboraEnding_804ADAC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Advances the swinging head (mSway) while field_0x2B0 is 1, and steps
    // through the table of sway targets (lbl_804742A8) once the head is idle.
    // The update() steps, called through lbl_804ADA50 (mPtmfIndex).
    void fn_8035F9C8();
    void fn_8035FB64();
    void fn_8035FCD8();
    void fn_8035FD6C();
    bool advanceStep();
    void fn_8035FF70();
    void fn_8036003C();
    void fn_80360914();
    void updateFacing();
    void pickStepTable();
    // Owned by this unit (0x8035F3C0); takes this and returns a byte, looped
    // on in update().
    u8 fn_8035F3C0();
    bool fireWaypoint();
    // Releases the two emitters (0x2B4, 0x2B8).
    void fn_803613D0();
    // draw() helpers for mPtmfIndex 0 and 1.
    void fn_80361424();
    void fn_803616FC();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;     // countdown, loaded from the table
    /* 0x238 */ s32 field_0x238;     // sprite frame base (draw)
    /* 0x23C */ s32 field_0x23C;     // value loaded from the table
    /* 0x240 */ s32 mStep;           // index into lbl_804742A8
    /* 0x244 */ s32 field_0x244;     // index into the step tables lbl_804ADA98
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s8 mHeight;          // height argument of the effect calls (update)
    /* 0x24D */ s8 mDepth;           // depth argument of the effect calls (update)
    /* 0x24E */ u8 field_0x24E[0x2]; // not yet known
    /* 0x250 */ s32 field_0x250;     // 0, 1 or 0x10: picks the sprite layers (fn_80361424)
    /* 0x254 */ s16 field_0x254;
    /* 0x256 */ u8 field_0x256[0x2]; // not yet known
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25C;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26C;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 mPtmfIndex;      // index into the member-function table (update)
    /* 0x27C */ s32 field_0x27C;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ const f32* field_0x284; // the command script fn_80360914 reads
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28C;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ u8 field_0x294;
    /* 0x295 */ u8 field_0x295[0x3]; // not yet known
    /* 0x298 */ const KaeporaWaypoint* field_0x298; // the path (init, fn_8035F3C0)
    /* 0x29C */ UnknownENDM_804AA0FC* field_0x29C; // the live ENDM actor (init)
    /* 0x2A0 */ cls_8021ACE0 mSway;
    /* 0x2B0 */ u8 field_0x2B0;      // gates the sway logic (1 = active)
    /* 0x2B1 */ u8 field_0x2B1[0x3]; // not yet known
    /* 0x2B4 */ JPABaseEmitter* field_0x2B4; // effect emitter, drawn at mPos (update)
    /* 0x2B8 */ JPABaseEmitter* field_0x2B8; // effect emitter, drawn at mPos (update)
}; // Size: 0x2BC

#endif
