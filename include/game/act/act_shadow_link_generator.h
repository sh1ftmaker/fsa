#ifndef GAME_ACT_ACT_SHADOW_LINK_GENERATOR_H
#define GAME_ACT_ACT_SHADOW_LINK_GENERATOR_H

// Code: game/act_shadow_link_generator.cpp. Actor SLGE ("Shadow Link generator"
// in docs/names/actors.tsv): an EnemyBase_804A4808 that stages the Shadow Link
// fight's intro and spawns the 'SLK2' actors (ShadowLink2_804A43E8) from its
// own layer. Named after its vtable (0x804A4310).

#include "game/EnemyBase.h"
#include "game/cls_8021C2B4.h"
#include "game/tu_8021F4B0.h"

class JPABaseEmitter;

// Named after its vtable, __vt__28ShadowLinkGenerator_804A4310 at 0x804A4310
// (0xA4 bytes). The destructor and init, draw and update are its overrides.
class ShadowLinkGenerator_804A4310 : public EnemyBase_804A4808 {
public:
    ShadowLinkGenerator_804A4310();
    virtual ~ShadowLinkGenerator_804A4310();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // game/act_shadow_link_generator.cpp: starts the intro effects (the 5 ids
    // of lbl_80469424 into mEmitters2, the 4 of lbl_8053EDB8 kept by nothing)
    // and a sound, and sets state 4.
    void fn_801C1768();
    // game/act_shadow_link_generator.cpp: spawns four 'SLK2' actors, one per
    // step, each on a random slot from mSlotFirst to mSlotLast, and switches
    // each one that is found to its state 0xD.
    void fn_801C186C() const;

    // The slot range comes from the spawn argument field_0x90 (init: bits
    // 8-15 and 16-23); fn_801C5660 keeps the low nibble of a slot.
    /* 0x2A8 */ u8 mSlotFirst;
    /* 0x2A9 */ u8 mSlotLast;
    // Set to 180 + rand(61) in state 1; counted down in state 2, which spawns
    // when it reaches 0.
    /* 0x2AA */ u16 mWaitTimer;
    /* 0x2AC */ u16 field_0x2ac;
    /* 0x2B0 */ JPABaseEmitter* mEmitter; // effect 0x4F at mPos (init)
    /* 0x2B4 */ JPABaseEmitter* mEmitters[3]; // effects 0x4B-0x4D (init)
    /* 0x2C0 */ JPABaseEmitter* mEmitters2[5]; // effects 0x3FC-0x400 (fn_801C1768)
    /* 0x2D4 */ void* mTexture; // fn_804032E8('TIMG', "jen_8_3.bti") (init)
    /* 0x2D8 */ cls_8021C2B4 mPieces; // four thrown pieces (fn_8021C2B4 per frame)
    /* 0x3F8 */ cls_8021FB44 mSequence; // the three-emitter sequence
}; // Size: 0x438

#endif
