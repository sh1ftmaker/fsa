#ifndef GAME_CLS_804A64A8_H
#define GAME_CLS_804A64A8_H

#include "types.h"
#include "JSystem/JGeometry.h"

class JPABaseEmitter;
class JPABaseParticle;

// A particle callback, defined in game/tu_802144F0.cpp. Named after its vtable
// (lbl_804A64A8: dtor, execute, draw), the vtable of a JPAParticleCallBack
// subclass. It is not derived from JPAParticleCallBack: JPAParticle.h needs
// <cmath> (c_xyz.h), which game units can't include, so the class is laid out
// the same way (vptr, then 0x14 bytes in all).
class cls_804A64A8 {
public:
    virtual ~cls_804A64A8();
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);

    /* 0x04 */ JGeometry::TVec3f field_0x4;
    /* 0x10 */ u8 field_0x10;
}; // Size: 0x14

#endif
