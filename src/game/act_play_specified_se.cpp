// Actor SEN2 ("Play specified SE" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x23C bytes for it and calls its constructor
// fn_802011F0, which stores the vtable 0x804A9F00.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_play_specified_se.h"

// The ids init's spawn parameter selects from (one entry, 3).
static const u32 lbl_80541250[] = {3};

PlaySpecifiedSe_804A9F00::~PlaySpecifiedSe_804A9F00() {}

void PlaySpecifiedSe_804A9F00::draw() {}

// Counts field_0x238 down, then (unless field_0x230 is set) calls
// fn_801F0D20 with the selected id once and removes the actor.
void PlaySpecifiedSe_804A9F00::update() {
    if (field_0x238 == 0) {
        if (field_0x230 == 0) {
            fn_801F0D20(lbl_80541250[field_0x234]);
        }
        mAlive = false;
    } else {
        field_0x238--;
    }
}

void PlaySpecifiedSe_804A9F00::init() {
    field_0x230 = ((u32)field_0x90 >> 26) & 1;
    field_0x234 = field_0x90 & 0xFF;
    field_0x238 = ((u32)field_0x90 >> 8) & 0xFFF;
}
