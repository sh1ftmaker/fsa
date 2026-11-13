#ifndef GAME_ACT_ACT_SEESAW_LIFT_H
#define GAME_ACT_ACT_SEESAW_LIFT_H

// Code: game/act_seesaw_lift.cpp.

#include "game/ActorBase.h"

// The SSLF actor ("Seesaw lift", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_8020113C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD348.
class SeesawLift_804AD348 : public ActorBase_804A5770 {
public:
    SeesawLift_804AD348() {}
    /* 0x08 */ virtual ~SeesawLift_804AD348();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    void fn_803584C0();
    void fn_80358F98();
    void fn_803597EC();
    void fn_8035A210();
    void fn_8035A860(f32 weight, s32 player);
    void fn_8035AA54();
    s32 fn_8035AC84(s32 player);
    s32 fn_8035AE9C(s32 player);

    /* 0x230 */ s16 field_0x230; // tilt angle
    /* 0x232 */ u8 field_0x232[0x234 - 0x232];
    /* 0x234 */ void* field_0x234; // 'formation_lift.bti' / 'sand5.bti' resource
    /* 0x238 */ s16 field_0x238;
    /* 0x23A */ s16 field_0x23a;
    /* 0x23C */ s16 field_0x23c;
    /* 0x23E */ s16 field_0x23e;
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ u8 field_0x244;
    /* 0x245 */ u8 field_0x245;
    /* 0x246 */ s8 field_0x246[4]; // per player: side result of fn_8035AE9C
    /* 0x24A */ s8 field_0x24a[4]; // per player: side of the contact box
    /* 0x24E */ u8 field_0x24e;
    /* 0x24F */ u8 field_0x24f;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ ActorBase_804A5770* field_0x25c[4]; // per player: the actor it carries
    /* 0x26C */ s32 field_0x26c;
    // init clears the word, fn_803584C0 uses the first half as a signed step.
    union {
        s32 field_0x270w;
        s16 field_0x270;
    };
}; // Size: 0x274 (init also stores words at 0x274 and 0x278, past the allocation)

#endif
