// Actor SVGT ("Side-view gate display NPC" in docs/names/actors.tsv): the
// spawner (fn_801F9484) allocates 0x23C bytes for it (0x801FEA10) and calls
// its constructor fn_802001D0, which stores the vtable 0x804ACB98. init
// starts two effects (ids 0x447 and 0x448) at its position and keeps their
// emitters; the destructor releases them.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_side_view_gate_display_npc.h"

// FIXME: init (0x803524D0) is not written yet: it calls fn_8013CC50, the
// effect spawner of game/tu_8013C5C8.cpp, which no shared header declares
// yet (docs/GAME_PLAN.md G1 item 4, Effect.h).

void SideViewGateDisplayNpc_804ACB98::draw() {}

void SideViewGateDisplayNpc_804ACB98::update() {}

SideViewGateDisplayNpc_804ACB98::~SideViewGateDisplayNpc_804ACB98() {
    fn_801EE434(mEmitter234);
    fn_801EE434(mEmitter238);
}
