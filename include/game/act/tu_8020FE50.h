#ifndef GAME_ACT_TU_8020FE50_H
#define GAME_ACT_TU_8020FE50_H

// Code: game/tu_8020FE50.cpp. The IBBM actor (spawner spawnActor_801F9484, constructor
// fn_802103C0) is not named: docs/names/actors.tsv has no plain-word name for
// it. Named after its vtable, 0x804A6100.

#include "game/act/tu_80210630.h"

class JPABaseEmitter;

class cls_804A6100 : public cls_804A61C8 {
public:
    // Out of line in tu_8020FE50 (0x802103C0); the spawner calls it.
    cls_804A6100();
    virtual ~cls_804A6100();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0xA0 */ virtual void vfunc_0xA0();

    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ s32 field_0x2b0;
    /* 0x2B4 */ JPABaseEmitter* mEmitter;
}; // Size: 0x2B8

#endif
