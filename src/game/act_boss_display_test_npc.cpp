// The boss display test NPC (actor BOTS, "Boss display test NPC" in
// docs/names/actors.tsv). The spawner (fn_801F9484) constructs it with the
// weak constructor fn_802020F0, which calls ActorBase_804A5770's and stores
// this class's vtable (0x8049C7F0). It overrides only draw, update and
// init, all empty. Unlike TutorialOldMan_804AE508's, its destructor comes
// first in .text, so it is defined here rather than generated.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Named after its vtable (0x8049C7F0).
// Size: 0x248 (the spawner's BOTS case allocates 0x248 at 0x801FCD7C).
class BossDisplayTestNpc_8049C7F0 : public ActorBase_804A5770 {
public:
    BossDisplayTestNpc_8049C7F0() {}

    /* 0x08 */ virtual ~BossDisplayTestNpc_8049C7F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ u8 field_0x230[0x18]; // not touched by this unit's code
};

BossDisplayTestNpc_8049C7F0::~BossDisplayTestNpc_8049C7F0() {}

void BossDisplayTestNpc_8049C7F0::draw() {}

void BossDisplayTestNpc_8049C7F0::update() {}

void BossDisplayTestNpc_8049C7F0::init() {}
