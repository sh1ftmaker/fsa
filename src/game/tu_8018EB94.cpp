#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_8018EB94.h"

// Destructor at 0x8018FB24.
cls_804A1FA0::~cls_804A1FA0() {
    for (int i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// 0x8018FAB4.
// FIXME(shotgun): 67.61 %, missing/extra
void cls_804A1FA0::vfunc_0x44() {
    if (mEmitters[0] != NULL) {
        mEmitters[0]->becomeInvalidEmitter();
        mEmitters[0] = NULL;
    }
    if (mEmitters[1] != NULL) {
        mEmitters[1]->becomeInvalidEmitter();
        mEmitters[1] = NULL;
    }
    if (mEmitters[2] != NULL) {
        mEmitters[2]->becomeInvalidEmitter();
        mEmitters[2] = NULL;
    }
}
