#ifndef GAME_ACT_ACT_QUICKSAND_H
#define GAME_ACT_ACT_QUICKSAND_H

// Code: game/act_quicksand.cpp. RUSA, the quicksand (docs/names/actors.tsv).

#include "game/EnemyBase.h"
#include "game/RailBase.h"

// Named after its vtable, 0x804A6270 (__vt__12cls_804A6270).
// Only the fields fn_80210E90 and vfunc_0x54 read are known.
class cls_804A6270 : public EnemyBase_804A4808 {
public:
    // 0x80212800 (__ct__12cls_804A6270Fv). Runs after EnemyBase's constructor.
    cls_804A6270();
    // 0x80212760 (__dt__12cls_804A6270Fv, vtable slot 0x08, over ActorBase's
    // destructor): the body removes mSerial from the list fn_80212AC0 returns;
    // EnemyBase's inline destructor follows, then ActorBase's.
    /* 0x08 */ virtual ~cls_804A6270();
    // 0x80212170 (vtable slot 0x54, over EnemyBase's empty vfunc_0x54): if
    // mFlags has bit 0x8000 and field_0x2bc is set, the rail at field_0x2c0
    // is called with field_0x2bc, mPos and field_0x4.
    /* 0x54 */ virtual void vfunc_0x54();
    // 0x80211078: zeroes field_0x3c, then calls fn_802110A8. Its only caller
    // is the vtable (slot 0x88, over EnemyBase's vfunc_0x88).
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x80211354 (vtable slot 0x9C, over EnemyBase's vfunc_0x9C): calls
    // EnemyBase's vfunc_0x9C with mapHit; then, when mState is set and
    // field_0x54.x and field_0x398 are not 0.0f, appends an entry to the
    // shared list fn_801902C0 returns; then, under the same test, plays
    // sound 0x30073 (startSound_801F0E34).
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    // 0x80211C94 (not virtual; called by fn_8021148C with this). Only while
    // mState is 2 to 5: when field_0x2ac is 3 and field_0x2b0 is set but the
    // view does not report that bit (fn_802D800C), or when it is 4 and
    // fn_801D05F8 finds no player within field_0x3a0, the state is left
    // (see leaveSand in the .cpp).
    void fn_80211C94();
    // 0x80210D90 (not virtual): sets mFlags bit 0x20000, calls ActorBase's
    // fn_801F3604, then acts on mState: state 1 calls fn_80212ECC on the
    // actor serial field_0x388 and passes field_0x2b8 to GameView with bit
    // set; state 6 passes it with bit clear. Then the actor in field_0x388
    // gets mFlags bit 0x4000, and the state becomes 6 (mStateTimer 0).
    void fn_80210D90();
    // 0x802110A8, direct call from vfunc_0x88 (not virtual).
    void fn_802110A8();
    // 0x802121C8 / 0x80211EB0 (plain symbols): the quicksand's warp-partner search and the push on the players.
    void fn_802121C8();
    void fn_80211EB0();
    // Called by the quicksand list (cls_80212AC0::fn_80212AF0, tu_80212AC0)
    // on each listed actor: whether *pos lies in this quicksand's area, and
    // the push-out vector for it.
    bool fn_80211000(const JGeometry::TVec3f* pos) const;
    void fn_80210E90(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) const;

    /* 0x2A8 */ s32 field_0x2a8; // not yet known; the constructor zeroes it
    /* 0x2AC */ s32 field_0x2ac; // not yet known; the constructor zeroes it
    /* 0x2B0 */ s32 field_0x2b0; // not yet known; the constructor zeroes it
    /* 0x2B4 */ s32 field_0x2b4; // not yet known; the constructor zeroes it
    /* 0x2B8 */ s32 field_0x2b8; // not yet known; the constructor zeroes it
    /* 0x2BC */ u8 field_0x2bc; // vfunc_0x54's first argument, and its test
    /* 0x2BD */ u8 field_0x2bd[0x2C0 - 0x2BD]; // not yet known
    /* 0x2C0 */ cls_800FE3EC field_0x2c0; // the rail vfunc_0x54 calls (0xC8 bytes)
    /* 0x388 */ s32 field_0x388; // an actor's serial (-1 for none); not yet known
    /* 0x38C */ s32 field_0x38c; // another actor's serial (-1 for none)
    /* 0x390 */ f32 field_0x390; // not yet known; the constructor zeroes it
    /* 0x394 */ f32 field_0x394; // not yet known; the constructor zeroes it
    /* 0x398 */ f32 field_0x398; // not yet known; the constructor zeroes it
    /* 0x39C */ f32 field_0x39c; // not yet known; the constructor zeroes it
    /* 0x3A0 */ f32 field_0x3a0; // not yet known; the constructor zeroes it
    /* 0x3A4 */ f32 field_0x3a4; // the most fn_80210E90 pushes a position out by
    /* 0x3A8 */ f32 field_0x3a8; // not yet known; the constructor sets it to 1.0f
    /* 0x3AC */ s32 field_0x3ac; // not yet known; the constructor sets it to -1
    /* 0x3B0 */ u8 field_0x3b0; // not yet known; the constructor clears it
}; // Size: not yet known

#endif
