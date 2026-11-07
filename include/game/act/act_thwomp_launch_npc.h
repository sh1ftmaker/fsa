#ifndef GAME_ACT_ACT_THWOMP_LAUNCH_NPC_H
#define GAME_ACT_ACT_THWOMP_LAUNCH_NPC_H

// Code: game/act_thwomp_launch_npc.cpp.

#include "game/ActorBase.h"
#include "game/Message.h"

// The JPEN actor ("NPC launched by Thwomp", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80201F8C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC010.
class NPCLaunchedByThwomp_804AC010 : public ActorBase_804A5770 {
public:
    NPCLaunchedByThwomp_804AC010() {}
    /* 0x08 */ virtual ~NPCLaunchedByThwomp_804AC010();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_80337A08();
    void fn_803376B0();

    // fn_8038C678 takes the position by value. The original loads mSerial,
    // field_0x4 and flags before the mPos copy, and field_0x24c and field_0x264
    // after it, so layer, serial, pos and flags are parameters here.
    bool talk(s32 layer, s32 serial, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038C678(field_0x24c, field_0x264, serial, layer, pos, flags);
    }

    /* 0x230 */ s32 field_0x230;
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
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ u32 field_0x264;
    /* 0x268 */ u8 field_0x268;
    /* 0x269 */ u8 field_0x269[3];
    /* 0x26C */ JGeometry::TVec3f field_0x26c;
    /* 0x278 */ JGeometry::TVec3f field_0x278;
}; // Size: 0x284

#endif
