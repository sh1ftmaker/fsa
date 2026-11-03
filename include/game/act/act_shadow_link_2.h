#ifndef GAME_ACT_ACT_SHADOW_LINK_2_H
#define GAME_ACT_ACT_SHADOW_LINK_2_H

// Code: game/act_shadow_link_2.cpp (not written yet). Only what
// game/tu_802188B0.cpp's helper reads is declared.

#include "game/EnemyBase.h"
#include "game/cls_80218A8C.h"

// The SLK2 actor ("Shadow Link 2", docs/names/actors.tsv, high confidence):
// the spawner constructs it with fn_801C573C, which stores this vtable
// (0x804A43E8). Named after its vtable. A child of EnemyBase_804A4808.
class ShadowLink2_804A43E8 : public EnemyBase_804A4808 {
public:
    ShadowLink2_804A43E8() {}
    /* 0x2A8 */ u8 field_0x2a8[0x540 - 0x2A8]; // not yet known
    /* 0x540 */ cls_80218A8C mHelper540;       // fn_801C573C calls its constructor
    /* 0x548 */ u8 field_0x548[0x564 - 0x548]; // not yet known
    /* 0x564 */ s32 field_0x564;               // 3: the helper does nothing
}; // Size: at least 0x568

#endif
