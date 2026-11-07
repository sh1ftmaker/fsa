#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/tu_802EF76C.h"
#include "game/act/tu_801F9484.h"

// FIXME(shotgun): 100 %, matches
void PlayerSwordLevelCheckNPC_804AA590::init() {
    field_0x258 = fn_801B7550();
    field_0x25c = field_0x258;
    if (fn_80237060(0) == 2) {
        mAlive = false;
    } else {
        field_0x244 = 0;
    }
    mEmitters[0] = NULL;
    field_0x264[0] = 2;
    mEmitters[1] = NULL;
    field_0x264[1] = 2;
    mEmitters[2] = NULL;
    field_0x264[2] = 2;
    mEmitters[3] = NULL;
    field_0x264[3] = 2;
    mSoundObj = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

// FIXME(shotgun): 97.92 %, missing/extra
PlayerSwordLevelCheckNPC_804AA590::~PlayerSwordLevelCheckNPC_804AA590() {
    for (s32 i = 0; i < 4; i++) {
        if (mEmitters[i] != NULL) {
            releaseEmitter_801EE434(mEmitters[i]);
        }
    }
    delete mSoundObj;
}

// FIXME(shotgun): 100 %, matches
void PlayerSwordLevelCheckNPC_804AA590::draw() {}
