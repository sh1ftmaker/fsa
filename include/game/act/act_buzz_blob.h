#ifndef GAME_ACT_ACT_BUZZ_BLOB_H
#define GAME_ACT_ACT_BUZZ_BLOB_H

// Code: game/act_buzz_blob.cpp.

#include "game/ActorBase.h"

// The BZBR actor ("Buzz Blob", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203F8C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F444.
class BuzzBlob_8049F444 : public ActorBase_804A5770 {
public:
    BuzzBlob_8049F444() {}
    /* 0x08 */ virtual ~BuzzBlob_8049F444();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x34]; // not yet known
}; // Size: 0x264

#endif
