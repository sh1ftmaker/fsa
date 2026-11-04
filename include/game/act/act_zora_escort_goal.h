#ifndef GAME_ACT_ACT_ZORA_ESCORT_GOAL_H
#define GAME_ACT_ACT_ZORA_ESCORT_GOAL_H

// Code: game/act_zora_escort_goal.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The escort's arrival point, a Zora ('FZRA', docs/names/actors.tsv, med
// confidence). Named after its vtable, 0x8049E424, as a provisional
// descriptive name; its constructor is fn_80127B48 (game_units.tsv).
class ZoraEscortGoal_8049E424 : public ActorBase_804A5770 {
public:
    ZoraEscortGoal_8049E424();
    /* 0x08 */ virtual ~ZoraEscortGoal_8049E424();
    // Vtable slot 0x20, fn_80126F1C: the base draw, then the escort's sprite
    // ("enemy_both2_sob.bin") with frame mAnim27c.mCel + 0x242.
    /* 0x20 */ virtual void draw();

    // Returns 1 when the state word at 0x230 is 1 or 0xC, else 0. Only reads
    // the word, so it is const.
    s32 fn_80126EF8() const;

    /* 0x230 */ s32 field_0x230; // state word, read by fn_80126EF8; 0 after construction
    /* 0x234 */ s32 field_0x234; // set to 0 by the constructor, not yet known
    /* 0x238 */ s32 field_0x238; // set to 0 by the constructor, not yet known
    /* 0x23C */ s32 field_0x23c; // set to -1 by the constructor, not yet known
    /* 0x240 */ s32 field_0x240; // set to 0 by the constructor, not yet known
    /* 0x244 */ s32 field_0x244; // set to 0 by the constructor, not yet known
    /* 0x248 */ s32 field_0x248; // set to 0 by the constructor, not yet known
    /* 0x24C */ s32 field_0x24c; // set to 0 by the constructor, not yet known
    /* 0x250 */ s32 field_0x250; // set to 0 by the constructor, not yet known
    /* 0x254 */ s32 field_0x254; // set to 0xFF by the constructor, not yet known
    /* 0x258 */ u8 field_0x258; // set to 0 by the constructor, not yet known
    /* 0x259 */ u8 field_0x259; // set to 0 by the constructor, not yet known
    /* 0x25C */ f32 field_0x25c; // set to 0.0f by the constructor, not yet known
    /* 0x260 */ f32 field_0x260; // set to 1.0f by the constructor, not yet known
    /* 0x264 */ f32 field_0x264; // set to 0.0f by the constructor, not yet known
    /* 0x268 */ f32 field_0x268; // set to 0.0f by the constructor, not yet known
    /* 0x26C */ f32 field_0x26c; // set to 0.0f by the constructor, not yet known
    /* 0x270 */ f32 field_0x270; // set to 0.0f by the constructor, not yet known
    /* 0x274 */ f32 field_0x274; // set to 0.0f by the constructor, not yet known
    /* 0x278 */ f32 field_0x278; // set to 0.0f by the constructor, not yet known
    /* 0x27C */ SpriteAnim_801D267C mAnim27c; // its J3DFrameCtrl at 0x2A0
    /* 0x2B4 */ u8 field_0x2b4[0x37C - 0x2B4]; // not yet known
    /* 0x37C */ JPABaseEmitter* field_0x37c; // released by the destructor; 0 after construction
    /* 0x380 */ JPABaseEmitter* field_0x380; // released by the destructor; 0 after construction
}; // Size: at least 0x384

#endif
