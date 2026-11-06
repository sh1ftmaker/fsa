// The boss display test NPC (actor BOTS, "Boss display test NPC" in
// docs/names/actors.tsv). The spawner (spawnActor_801F9484) constructs it with the
// weak constructor fn_802020F0, which calls ActorBase_804A5770's and stores
// this class's vtable (0x8049C7F0). It overrides only draw, update and
// init, all empty. Unlike TutorialOldMan_804AE508's, its destructor comes
// first in .text, so it is defined here rather than generated.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_boss_display_test_npc.h"

void BossDisplayTestNpc_8049C7F0::init() {}

void BossDisplayTestNpc_8049C7F0::update() {}

void BossDisplayTestNpc_8049C7F0::draw() {}

BossDisplayTestNpc_8049C7F0::~BossDisplayTestNpc_8049C7F0() {}
