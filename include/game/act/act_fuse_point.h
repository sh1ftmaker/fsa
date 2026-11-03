#ifndef GAME_ACT_ACT_FUSE_POINT_H
#define GAME_ACT_ACT_FUSE_POINT_H

// Code: game/act_fuse_point.cpp. Actor FSPO ("Fuse point" in
// docs/names/actors.tsv); the spawner allocates 0x2A8 bytes for it
// (0x801FD70C), EnemyBase_804A4808's size: it adds no fields.

#include "game/EnemyBase.h"

// Named after its vtable (0x804A54C0).
class FusePoint_804A54C0 : public EnemyBase_804A4808 {
public:
    // Out of line in act_fuse_point (0x801E7DBC), which nothing there calls;
    // the spawner calls it (0x801FD720).
    FusePoint_804A54C0();
    virtual ~FusePoint_804A54C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
}; // Size: 0x2A8

#endif
