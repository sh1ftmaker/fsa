// The TMI2 actor ("Block for 2-3 players", docs/names/actors.tsv, medium
// confidence). Spawned by spawnActor_801F9484: __nw__ 0x238, inline constructor
// fn_8020020C (base constructor and vtable only). Each frame it checks the
// value fn_80226850 returns for its position; at 0x12 it spawns a BCK2
// ("Colour block (1-player)") in its place and removes itself. Overrides
// the destructor, init() and update().

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/act_block_for_2_3_players.h"

BlockFor2Or3Players_804AE728::~BlockFor2Or3Players_804AE728() {}

void BlockFor2Or3Players_804AE728::update() {
    if (fn_80226850(&mPos, field_0x4) == 0x12) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BCK2, field_0x4, &mPos, 0x100, -1, -1);
        mAlive = false;
    }
}

// Clears mAlive (0x11C) right after the spawn, which removes the actor at
// the next update pass; whether 0x11C means "alive" here is unconfirmed.
void BlockFor2Or3Players_804AE728::init() {
    mAlive = false;
}
