#ifndef GAME_ACT_ACT_PHANTOM_GANON_2_H
#define GAME_ACT_ACT_PHANTOM_GANON_2_H

// Code: game/act_phantom_ganon_2.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The FGN2 actor (Phantom Ganon 2, docs/names/actors.tsv, high confidence;
// the spawner's case for ACTOR_FGN2). Named after its vtable, 0x804A1A98.
class PhantomGanon2_804A1A98 : public ActorBase_804A5770 {
public:
    PhantomGanon2_804A1A98() {}
    /* 0x08 */ virtual ~PhantomGanon2_804A1A98();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();

    // Picks the player this actor goes for (a random starting player, the next
    // free one after it), stores it in field_0x94 and returns it (0x80189DEC).
    u16 fn_80189DEC();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ u8 field_0x238[0x274 - 0x238]; // not yet known
    /* 0x274 */ s32 field_0x274; // serial of the other Phantom Ganon (-1 for none)
    /* 0x278 */ u8 field_0x278[0x27C - 0x278]; // not yet known
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ u8 field_0x280[0x28C - 0x280]; // not yet known
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ u8 field_0x294[0x2EE - 0x294]; // not yet known
    /* 0x2EE */ s8 field_0x2ee;
    /* 0x2EF */ u8 field_0x2ef[0x2F4 - 0x2EF]; // not yet known
    /* 0x2F4 */ JPABaseEmitter* mEmitters2f4[4];
    /* 0x304 */ JPABaseEmitter* mEmitters304[4];
    /* 0x314 */ JPABaseEmitter* mEmitter314;
}; // Size: 0x318 (the spawner's __nw__)

#endif
