#ifndef GAME_ACT_ACT_ZORA_LOTUS_H
#define GAME_ACT_ACT_ZORA_LOTUS_H

// Code: game/act_zora_lotus.cpp. The lotus Zora (LTZR, docs/names/actors.tsv):
// a ZoraEnemy whose constructor (0x80340C60) runs the base's, stores its own
// vtable, 0x804AC3E0 (45 slots; one more than ZoraEnemy's), and clears the
// field after ZoraEnemy's fields. Named after the vtable.
#include "game/act/act_zora.h"

class cls_804AC3E0 : public ZoraEnemy_804A3D7C {
public:
    cls_804AC3E0();

    // 0x2B4: nothing here in the constructors; ZoraEnemy's last field ends at 0x2B4.
    /* 0x2B4 */ u8 field_0x2b4[0x2B8 - 0x2B4];
    /* 0x2B8 */ s32 field_0x2b8; // the constructor stores 0
};

#endif
