#ifndef GAME_ACT_ACT_ZORA_LOTUS_H
#define GAME_ACT_ACT_ZORA_LOTUS_H

// Code: game/act_zora_lotus.cpp. The lotus Zora (LTZR, docs/names/actors.tsv):
// a ZoraEnemy whose constructor (0x80340C60) runs the base's, stores its own
// vtable, 0x804AC3E0 (45 slots, as many as ZoraEnemy's), and clears the
// field after ZoraEnemy's fields. Named after the vtable.
#include "game/act/act_zora.h"

class cls_804AC3E0 : public ZoraEnemy_804A3D7C {
public:
    cls_804AC3E0();

    // 0x8033F9A0: vtable slot 0x8. r4 = the delete flag; > 0 frees the object
    // (__dl__FPv). ZoraEnemy's destructor (0x801BB30C) runs first.
    virtual ~cls_804AC3E0();

    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x68 */ virtual u8 vfunc_0x68();
    /* 0x90 */ virtual void vfunc_0x90();
    /* 0xA4 */ virtual void vfunc_0xA4();
    /* 0xA8 */ virtual void vfunc_0xA8();
    /* 0xAC */ virtual bool vfunc_0xAC();
    /* 0xB0 */ virtual void vfunc_0xB0();

    /* 0x2AC */ u8 field_0x2ac[4];
    /* 0x2B0 */ u8 field_0x2b0;
    /* 0x2B1 */ u8 field_0x2b1;
    /* 0x2B2 */ u16 field_0x2b2;
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ JPABaseEmitter* field_0x2b8;
};

#endif
