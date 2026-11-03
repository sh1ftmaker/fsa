#ifndef GAME_ACT_ACT_ADVANCE_NOTICE_SE_H
#define GAME_ACT_ACT_ADVANCE_NOTICE_SE_H

// Code: game/act_advance_notice_se.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9C78.
class cls_804A9C78 : public ActorBase_804A5770 {
public:
    cls_804A9C78() {}
    /* 0x08 */ virtual ~cls_804A9C78();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u32 field_0x230; // not yet known
    /* 0x234 */ u32 mSoundIndex; // field_0x90 & 0xFFF (init), indexes the u32 table in .rodata
    /* 0x238 */ u32 mVariant; // (field_0x90 >> 24) & 7, indexes the u8 table in .sdata2
    /* 0x23C */ u8 mFlag; // (field_0x90 >> 23) & 1
    /* 0x23D */ u8 field_0x23d[3]; // padding
}; // Size: 0x240

// Unknown sound call (0x804561D8), called from update().
void fn_804561D8(ActorSoundObj_804578F4* sound, const u32* id, u8 a, u8 b, u8 c);

#endif
