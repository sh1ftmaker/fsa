#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_H

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"
#include "game/RailBase.h"

// The ZLDA actor ("Princess Zelda", docs/names/actors.tsv, high confidence):
// spawned by spawnActor_801F9484 (0x8010AFC4). Named after its vtable, 0x8049CEE0.
class cls_8049CEE0 : public ActorBase_804A5770 {
public:
    cls_8049CEE0();
    /* 0x08 */ virtual ~cls_8049CEE0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_80109534();
    bool fn_801096C8(s32 state);
    bool fn_80109AE0(JGeometry::TVec3f* outPos);
    bool stepRail();

    /* 0x230 */ cls_800FE3EC mRail;
    /* 0x2F8 */ u8 field_0x2f8;
    /* 0x2F9 */ u8 field_0x2f9[3];
    /* 0x2FC */ s32 field_0x2fc;
    /* 0x300 */ s32 field_0x300;
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ s32 field_0x308;
    /* 0x30C */ s32 field_0x30c;
    /* 0x310 */ s32 field_0x310;
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320;
    /* 0x324 */ s32 field_0x324;
    /* 0x328 */ s32 field_0x328;
    /* 0x32C */ s32 field_0x32c;
    /* 0x330 */ f32 field_0x330;
    /* 0x334 */ f32 field_0x334;
    /* 0x338 */ u8 field_0x338;
    /* 0x339 */ u8 field_0x339;
    /* 0x33A */ u8 field_0x33a[2];
    /* 0x33C */ SpriteAnim_801D267C mSpriteAnim;
}; // Size: 0x374

#endif
