#ifndef GAME_ACT_TU_80147B50_H
#define GAME_ACT_TU_80147B50_H

// Code: game/tu_80147B50.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// Spawned for 'DGTL' (Digdogger: its unit loads the boss_deg_*.bti textures). Named after
// its vtable, 0x8049F6C8. The head is drawn at field_0x2c0[7] with a trail of segments
// taken from the position history in field_0x3d8 (with the headings in field_0x338).
class cls_8049F6C8 : public ActorBase_804A5770 {
public:
    cls_8049F6C8() {}
    /* 0x08 */ virtual ~cls_8049F6C8();
    /* 0x0C */ virtual void init();   // fn_8014A5F4
    /* 0x14 */ virtual void update(); // fn_8014821C, not written yet
    /* 0x20 */ virtual void draw();   // fn_80147B50

    // 0x80149AEC: damage check for the players overlapping the head; true when it hit.
    bool fn_80149AEC();
    // 0x80149FDC: counts the hit timer down and picks the next direction.
    bool fn_80149FDC();
    // 0x8014A2CC: pushes the position (and heading) history and places the segments.
    void fn_8014A2CC(s32 flag);

    // SHARED-EDIT: carved from field_0x230[0x90] / field_0x338[0xA0] / field_0x798[0x30]
    // (game/tu_80147B50.cpp).
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ u16 field_0x284;
    /* 0x286 */ u16 field_0x286;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0;
    /* 0x2AC */ u16 field_0x2ac[10];
    /* 0x2C0 */ JGeometry::TVec3f field_0x2c0[10];
    /* 0x338 */ u16 field_0x338[80];
    /* 0x3D8 */ JGeometry::TVec3f field_0x3d8[80];
    /* 0x798 */ JPABaseEmitter* mEmitters798[10];
    /* 0x7C0 */ JPABaseEmitter* mEmitters7c0[2];
}; // Size: 0x7C8

#endif
