#ifndef GAME_ACT_ACT_THIEF_H
#define GAME_ACT_ACT_THIEF_H

// Code: game/act_thief.cpp (not written yet; the members below are in
// game/tu_8020F798.cpp). Actor TZOK ("Thief" in docs/names/actors.tsv): an
// EnemyBase_804A4808 whose spawner (ctor 0x80206810) allocates 0x388 bytes.
// Only the fields tu_8020F798 uses are named; the rest of the class is
// padding until act_thief is written.

#include "game/EnemyBase.h"

class JPABaseEmitter;

// One entry of the type table lbl_8046A238 (0x14 bytes each, 4 entries,
// .rodata of act_thief; indexed by Thief_804A5A30::mType). Named after the
// class it belongs to; only the two fields tu_8020F798 reads are known.
struct ThiefTypeParams_804A5A30 {
    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ s32 mSpriteFrame; // added to the animation cel (mAnim.mCel) for the sprite frame
    /* 0x10 */ f32 mHeight;      // subtracted from mPos.z for the lit sprite
};
extern const ThiefTypeParams_804A5A30 lbl_8046A238[];

// Named after its vtable, 0x804A5A30.
class Thief_804A5A30 : public EnemyBase_804A4808 {
public:
    virtual ~Thief_804A5A30();
    /* 0x20 */ virtual void draw();

    // 0x8020F80C: draws the shadow and the sprite at mPos on the layer for
    // sprite frame `frame` (the draw() argument).
    void fn_8020F80C(s32 frame);
    // 0x8020FADC: called from act_thief's update.
    void fn_8020FADC();

    /* 0x2A8 */ u8 field_0x2a8[0x350 - 0x2A8];
    /* 0x350 */ s32 mType;            // index into lbl_8046A238
    /* 0x354 */ s32 field_0x354;
    /* 0x358 */ JPABaseEmitter* mEmitter358;
    /* 0x35C */ u8 field_0x35c[0x388 - 0x35C];
}; // Size: 0x388

#endif
