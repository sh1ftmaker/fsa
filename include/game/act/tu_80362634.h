#ifndef GAME_ACT_TU_80362634_H
#define GAME_ACT_TU_80362634_H

// Code: game/tu_80362634.cpp.

#include "game/ActorBase.h"

// Spawned for 'EETC' (the ending's other characters, docs/names/actors.tsv; its
// "fune.bti" image is a boat). Named after its vtable, 0x804ADF28. A state
// machine: field_0x234 is the state, indexing tables of member function pointers.
// The 'EOWL' actor (act_kaepora_gaebora_ending, not written yet); only the byte
// fn_80362C44 sets at 0x294 is known.
class EndingOwl_804ADF28 : public ActorBase_804A5770 {
public:
    /* 0x230 */ u8 pad_0x230[0x278 - 0x230];
    /* 0x278 */ s32 field_0x278; // tested by cls_804ADF28::fn_80363880 (0 to be a landing spot)
    /* 0x27C */ u8 pad_0x27c[0x294 - 0x27C];
    /* 0x294 */ u8 field_0x294;
};

class cls_804ADF28 : public ActorBase_804A5770 {
public:
    // state step: wanders near field_0x27c[0].
    void fn_80363360();
    // state step: walks toward a 'RUPY' and picks it up.
    void fn_803645B4();
    // state step: orbit around a fixed point.
    void fn_80363FFC();
    // every so often spawns a 'RUPY' near itself.
    void fn_80364340();
    // state step: owl-ish animation timer.
    void fn_8036509C();
    // turns toward the 'EOWL' actor.
    void fn_80364D0C();
    // random walk step.
    void fn_80363664();
    // state step with the 0x270/0x274 tables.
    void fn_80364F60();
    // state step: waves.
    void fn_80365274();
    // state step: hop.
    void fn_80364900();
    cls_804ADF28() {}
    /* 0x08 */ virtual ~cls_804ADF28();

    // 0x80362680: draws the boat sprite.
    void fn_80362680();
    // 0x803628D4: draws the boat sprite with the mirrored matrix.
    void fn_803628D4();
    // 0x80362B74: draws the boat sprite and its three companions.
    void fn_80362B74();
    // 0x80362C44: state machine of the departure scene (field_0x238).
    void fn_80362C44();
    // 0x80365AB8: calls the member function of table lbl_804ADDE8[field_0x234].
    void fn_80365AB8();
    // 0x80364A88: walks along field_0x3c, turning when a leg length is reached.
    void fn_80364A88();
    // 0x80363880: follows the 'EOWL' owl, else wanders near field_0x27c[0].
    void fn_80363880();
    // 0x803653EC: the departure formation: moves the three companions to their marks, one formation per step.
    void fn_803653EC();
    // 0x80363880 / 0x803653EC / 0x80365B5C / 0x80366E60: see the definitions.
    // 0x80365AF0: counts timers down, calls member function of table lbl_804ADC38[field_0x234].
    void fn_80365AF0();
    // 0x80362634: deletes the emitter.
    void fn_80362634();
    // 0x80362B14: draws the "fune.bti" image at mPos.
    void fn_80362B14();
    // 0x8036335C: empty.
    void fn_8036335C();
    // 0x80363F7C: steps the pattern index when its counter ran out, then moves by field_0x3c.
    void fn_80363F7C();
    // 0x80364F00: like fn_80363F7C with the pattern read from the byte tables at 0x270 / 0x274.
    void fn_80364F00();
    // 0x80365220: bobs mPos.y up and down every 0x40 frames.
    void fn_80365220();

    /* 0x230 */ s8 field_0x230;
    /* 0x231 */ u8 pad_0x231[3];
    /* 0x234 */ s32 field_0x234; // state, the index of the member function tables
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s16 field_0x240; // angle to the owl
    /* 0x242 */ u8 field_0x242[2]; // not yet known
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // slot of the owl in the actor manager, -1 when gone
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ const s8* field_0x270;
    /* 0x274 */ const s8* field_0x274;
    /* 0x278 */ u8 field_0x278; // 1: draw mirrored
    /* 0x279 */ u8 field_0x279; // not yet known
    /* 0x27A */ u16 field_0x27a; // tile code under the actor
    /* 0x27C */ JGeometry::TVec3f field_0x27c[3];
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0[3];
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[3];
    /* 0x2E8 */ s32 field_0x2e8[3];
    /* 0x2F4 */ f32* field_0x2f4; // the walk's leg lengths (field_0x2f8 counts the distance)
    /* 0x2F8 */ f32 field_0x2f8;
    /* 0x2FC */ EndingOwl_804ADF28* field_0x2fc; // the 'EOWL' actor
    /* 0x300 */ JPABaseEmitter* field_0x300;
    /* 0x304 */ JPABaseEmitter* field_0x304;
    /* 0x308 */ JPABaseEmitter* field_0x308;
    /* 0x30C */ JPABaseEmitter* field_0x30c[4];
}; // Size: 0x31C

#endif
