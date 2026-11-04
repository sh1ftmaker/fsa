#ifndef GAME_ACT_ACT_ICE_MELTED_BY_FOUR_PLAYERS_H
#define GAME_ACT_ACT_ICE_MELTED_BY_FOUR_PLAYERS_H

// Code: game/act_ice_melted_by_four_players.cpp.

#include "game/ActorBase.h"

// The ICEB actor ("Ice melted by four players", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_801FF990, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AE888.
class IceMeltedByFourPlayers_804AE888 : public ActorBase_804A5770 {
public:
    IceMeltedByFourPlayers_804AE888() {}
    /* 0x08 */ virtual ~IceMeltedByFourPlayers_804AE888();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // Advances the melting timeline (field_0x248 < 0: finished, returns 0).
    // States 0-3 wait for fn_8023077C(); from state 4 on, field_0x244 is a
    // countdown and field_0x24c walks the stages through lbl_804751A8. Returns 1
    // while the ice is still melting.
    u8 fn_8037845C();
    // Collects the player bits of the 'FIRE' actors (Fire Rod fire) on this
    // layer whose box overlaps this one; returns true when all four are set.
    u8 fn_80378880();
    // Scans the 'HBMB' actors on this layer whose box overlaps this one: one
    // with a zero field_0x3c.xy gets its y moved 16 above or below this one,
    // any other gets mLife 0. Always returns false.
    bool fn_80378680();

    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor, then NULL
    /* 0x234 */ s32 field_0x234; // update() switch: 0 (melt starts), 1 (melting)
    /* 0x238 */ f32 field_0x238; // scale, 1.0f at init
    /* 0x23C */ f32 field_0x23c; // scale, 1.0f at init
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244; // countdown in fn_8037845C
    /* 0x248 */ s32 field_0x248; // -1 after init; the state of fn_8037845C
    /* 0x24C */ s32 field_0x24c; // stage counter in fn_8037845C
    /* 0x250 */ u8 field_0x250;
    /* 0x251 */ u8 field_0x251; // draw() and update() branch on it (1 = done)
}; // Size: at least 0x252 (the spawner's allocation is not checked)

#endif
