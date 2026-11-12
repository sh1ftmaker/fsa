#ifndef GAME_ACT_TU_801DB18C_H
#define GAME_ACT_TU_801DB18C_H

#include "types.h"
#include "game/EnemyBase.h"

// Code: game/tu_801DB18C.cpp. An EnemyBase_804A4808 named after its vtable,
// 0x804A4EE8 (config/G4SE01/tu_map.tsv: enemy_both_sob.bin unit).
// Its vfunc_0x9C runs EnemyBase's, then moves the emitter at 0x2E4 to mPos
// with setEmitterPos_8013CB44 (height 1, depth 0, adjusted).
class cls_804A4EE8 : public EnemyBase_804A4808 {
public:
    // 0x801DBEF4, the deleting destructor (vtable slot 0x08): releases the
    // emitter at 0x2E4 and the serial at 0x198 (the singleton list), then
    // the EnemyBase and ActorBase destructors.
    /* 0x08 */ virtual ~cls_804A4EE8();
    // 0x801DB2C8 (EnemyBase's player-hit hook): hits every player whose box
    // overlaps mBox (fn_801F2740(-1)), at the centre of mBox (fn_801F3128)
    // or, for a player for whom fn_801F329C holds, that centre converted by
    // fn_801CD0D0, with fn_80238548(player, field_0x114, &pos, 1). Returns
    // the bitmask of the players overlapping mBox (0 when mBox is empty).
    /* 0x68 */ virtual u8 vfunc_0x68();
    /* 0x94 */ virtual void vfunc_0x94();
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);

    // Shotgun pass: the constructor and overrides below are written from the asm only.
    cls_804A4EE8();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    void fn_801DB6C8();

    // 0x801DB950: the state setter. State 1 starts a search: the frame rate
    // is set, field_0x2ac cleared, fn_801DB9CC runs, then mTargetPlayer =
    // fn_801D05F8(&mSearchPos, mSearchRange). Every state resets mStateTimer.
    void fn_801DB950(s32 state);
    // 0x801DB9CC, called by the state setter with only `this`.
    void fn_801DB9CC();

    /* 0x2A8 */ s32 mTargetPlayer;  // fn_801D05F8's result (-1: none)
    /* 0x2AC */ u32 field_0x2ac;    // cleared when state 1 starts; an unsigned counter
    /* 0x2B0 */ f32 mSearchRange;   // the range of fn_801D05F8
    /* 0x2B4 */ f32 field_0x2b4; // the drift velocity (damped)
    /* 0x2B8 */ f32 field_0x2b8;
    /* 0x2BC */ JGeometry::TVec3f mSearchPos; // the origin of fn_801D05F8
    /* 0x2C8 */ f32 field_0x2c8;
    /* 0x2CC */ f32 field_0x2cc;
    /* 0x2D0 */ f32 field_0x2d0;
    /* 0x2D4 */ f32 mRadius;       // of the circle it flies around mSearchPos
    /* 0x2D8 */ f32 mTargetRadius;
    /* 0x2DC */ s16 mAngle;
    /* 0x2DE */ s16 mAngleSpeed;
    /* 0x2E0 */ f32 field_0x2e0;
    /* 0x2E4 */ JPABaseEmitter* field_0x2e4; // emitter moved with mPos
}; // Size: at least 0x2E8

#endif
