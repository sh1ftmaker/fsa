// Actor SVGT ("Side-view gate display NPC" in docs/names/actors.tsv): the
// spawner (spawnActor_801F9484) allocates 0x23C bytes for it (0x801FEA10) and calls
// its constructor fn_802001D0, which stores the vtable 0x804ACB98. init
// starts two effects (ids 0x447 and 0x448) at its position and keeps their
// emitters; the destructor releases them.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_side_view_gate_display_npc.h"
#include "game/Effect.h"

void SideViewGateDisplayNpc_804ACB98::draw() {}

void SideViewGateDisplayNpc_804ACB98::update() {}

void SideViewGateDisplayNpc_804ACB98::init() {
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    if (mEmitter234 == NULL) {
        mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x447, -1, 0, NULL, NULL, true, 1.0f);
    }
    if (mEmitter238 == NULL) {
        mEmitter238 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x448, -1, 0, NULL, NULL, true, 1.0f);
    }
}

SideViewGateDisplayNpc_804ACB98::~SideViewGateDisplayNpc_804ACB98() {
    releaseEmitter_801EE434(mEmitter234);
    releaseEmitter_801EE434(mEmitter238);
}
