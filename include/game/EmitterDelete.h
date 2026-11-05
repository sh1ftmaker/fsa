#ifndef GAME_EMITTERDELETE_H
#define GAME_EMITTERDELETE_H

#include "game/Effect.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Deletes the emitter a member points to, at once, and clears the member:
// inlined in 86 functions of 44 units (lwz field; cmplwi; beq; bl
// getEffectSystem_8013C824; lwz 0xc; lwz field again; bl forceDeleteEmitter; stw 0).
// The manager is fetched into a local first, then the emitter is read again
// for the call; getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(e) in one
// expression keeps the first load of e in a saved register instead.
// Proven by act_eyeball_switch's cls_804AE678::vfunc_0x24.
inline void deleteEmitter(JPABaseEmitter** emitter) {
    if (*emitter != NULL) {
        JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(*emitter);
        *emitter = NULL;
    }
}

#endif
