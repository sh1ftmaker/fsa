// Actor LCLS ("Local switch" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x230 bytes for it (no members of its own) and
// calls its constructor fn_80200E1C, which stores the vtable 0x804AA008.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_local_switch.h"

LocalSwitch_804AA008::~LocalSwitch_804AA008() {}

void LocalSwitch_804AA008::draw() {}

// When the flag selected by bits 22-26 of the spawn parameter (0x90) is set,
// calls slot 0x30 with (0xFF, true).
void LocalSwitch_804AA008::update() {
    if (fn_802D7F40(fn_802D6A9C(), ((u32)field_0x90 >> 22) & 0x1F)) {
        vfunc_0x30(0xFF, true);
    }
}

void LocalSwitch_804AA008::init() {
    update();
}
