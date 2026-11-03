#ifndef GAME_ACT_ACT_FORCE_GEM_SHADOW_H
#define GAME_ACT_ACT_FORCE_GEM_SHADOW_H

// Code: game/act_force_gem_shadow.cpp.

#include "game/ActorBase.h"

// fn_803075AC (tu_803075AC, not yet decompiled), called here as
// (mSerial, field_0x4, &mPos, 0x3CD, 0, 2, -1, 0, 0, 0); r3..r10 plus two
// stack words, all integers or pointers in the asm. Meaning unknown.
extern "C" void fn_803075AC(s32 a, s32 b, const JGeometry::TVec3f* pos, s32 c, s32 d, s32 e, s32 f,
                            s32 g, s32 h, s32 i);

// The RSNP actor ("Force Gem shadow NPC", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FFF24, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804ACD18.
class ForceGemShadowNPC_804ACD18 : public ActorBase_804A5770 {
public:
    ForceGemShadowNPC_804ACD18() {}
    /* 0x08 */ virtual ~ForceGemShadowNPC_804ACD18();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // frame counter, 0..0x32
}; // Size: 0x234

#endif
