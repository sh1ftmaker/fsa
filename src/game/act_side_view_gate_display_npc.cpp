// Actor SVGT ("Side-view gate display NPC" in docs/names/actors.tsv): the
// spawner (fn_801F9484) allocates 0x23C bytes for it (0x801FEA10) and calls
// its constructor fn_802001D0, which stores the vtable 0x804ACB98. init
// starts two effects (ids 0x447 and 0x448) at its position and keeps their
// emitters; the destructor releases them.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Named after its vtable (0x804ACB98).
class SideViewGateDisplayNpc_804ACB98 : public ActorBase_804A5770 {
public:
    SideViewGateDisplayNpc_804ACB98() {}
    virtual ~SideViewGateDisplayNpc_804ACB98();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ u8 field_0x230[4];
    /* 0x234 */ JPABaseEmitter* mEmitter234; // fn_8013CC50(field_0x4, &mPos, 0x447, ...)
    /* 0x238 */ JPABaseEmitter* mEmitter238; // fn_8013CC50(field_0x4, &mPos, 0x448, ...)
}; // Size: 0x23C

// FIXME: init (0x803524D0) is not written yet: it calls fn_8013CC50, the
// effect spawner of game/tu_8013C5C8.cpp, which no shared header declares
// yet (docs/GAME_PLAN.md G1 item 4, Effect.h).

void SideViewGateDisplayNpc_804ACB98::draw() {}

void SideViewGateDisplayNpc_804ACB98::update() {}

SideViewGateDisplayNpc_804ACB98::~SideViewGateDisplayNpc_804ACB98() {
    fn_801EE434(mEmitter234);
    fn_801EE434(mEmitter238);
}
