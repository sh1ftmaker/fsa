#ifndef GAME_EMITTERDELETEKEEP_H
#define GAME_EMITTERDELETEKEEP_H

#include "game/Effect.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Deletes an emitter at once but leaves the member pointing at it: the destructors that
// delete the emitters of a whole array (act GOAL's fn_802A64E8) and update() functions about
// to free the actor use this form (lwz; bl getEffectSystem; lwz 0xc; lwz; bl forceDeleteEmitter,
// no store of 0). Include only where used. The clearing form is deleteEmitter in EmitterDelete.h.
inline void deleteEmitterKeep(JPABaseEmitter* emitter) {
    if (emitter != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(emitter);
    }
}

#endif
