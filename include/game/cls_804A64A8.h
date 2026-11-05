#ifndef GAME_CLS_804A64A8_H
#define GAME_CLS_804A64A8_H

#include "types.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JParticle/JPAEmitter.h"

// A particle callback, defined in game/tu_802144F0.cpp. Named after its vtable
// (__vt__12cls_804A64A8: dtor, execute, draw), the vtable of a
// JPAParticleCallBack subclass. Its destructor (__dt__12cls_804A64A8Fv) calls
// __dt__19JPAParticleCallBackFv; it is inline (cls_802150B8's and
// Player_804A6F38's destructors inline it, tu_802144F0 also emits the weak copy). The base is JSystem's own struct
// (JPAEmitter.h), which the game-side stand-in used to duplicate.
class cls_804A64A8 : public JPAParticleCallBack {
public:
    cls_804A64A8() : field_0x10(0) {}
    virtual ~cls_804A64A8() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);

    /* 0x04 */ JGeometry::TVec3f field_0x4;
    /* 0x10 */ u8 field_0x10;
}; // Size: 0x14

#endif
