#ifndef GAME_ACT_ACT_WIZZROBE_H
#define GAME_ACT_ACT_WIZZROBE_H

// Code: game/act_wizzrobe.cpp.

#include "game/ActorBase.h"

void fn_80154590(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1);
f32 fn_80154378(f32 x);

class JPABaseEmitter;

// Spawned for 'WIZR' (Wizzrobe, docs/names/actors.tsv, high confidence);
// moved here from game/act/tu_801F9484.h. Named after its vtable, 0x804A0EDC.
class Wizzrobe_804A0EDC : public ActorBase_804A5770 {
public:
    Wizzrobe_804A0EDC() {}
    /* 0x08 */ virtual ~Wizzrobe_804A0EDC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();

    void killLinkedActor();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235[3];
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
    /* 0x270 */ s32 field_0x270[20];
    /* 0x2C0 */ s32 field_0x2c0;
    /* 0x2C4 */ s32 field_0x2c4;
    /* 0x2C8 */ s32 field_0x2c8;
    /* 0x2CC */ s16 field_0x2cc;
    /* 0x2CE */ u8 field_0x2ce[2];
    /* 0x2D0 */ f32 field_0x2d0;
    /* 0x2D4 */ f32 field_0x2d4;
    /* 0x2D8 */ f32 field_0x2d8;
    /* 0x2DC */ f32 field_0x2dc;
    /* 0x2E0 */ f32 field_0x2e0;
    /* 0x2E4 */ u8 field_0x2e4;
    /* 0x2E5 */ u8 field_0x2e5;
    /* 0x2E6 */ u8 field_0x2e6[2];
    /* 0x2E8 */ JGeometry::TVec3f field_0x2e8;
    /* 0x2F4 */ JGeometry::TVec3f field_0x2f4;
    /* 0x300 */ JPABaseEmitter* mEmitters[3];
}; // Size: 0x30C

// 0x80178EB8 (game/act_wizzrobe.cpp): out-of-line copy of
// JPABaseEmitter::setGlobalTranslation(const TVec3&); also called by tu_8044C7F0.
void fn_80178EB8(JPABaseEmitter* emitter, const JGeometry::TVec3f* trans);

#endif
