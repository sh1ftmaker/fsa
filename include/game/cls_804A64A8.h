#ifndef GAME_CLS_804A64A8_H
#define GAME_CLS_804A64A8_H

#include "types.h"
#include "JSystem/JGeometry.h"

class JPABaseEmitter;
class JPABaseParticle;

// JSystem's JPAParticleCallBack (JPAParticle.h), declared here because that
// header needs <cmath> (c_xyz.h), which game units can't include. Same
// virtuals, so the same vtable (__vt__19JPAParticleCallBack) and destructor.
// Only this unit uses it.
class JPAParticleCallBack {
public:
    JPAParticleCallBack() {}
    virtual ~JPAParticleCallBack();
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

// A particle callback, defined in game/tu_802144F0.cpp. Named after its vtable
// (__vt__12cls_804A64A8: dtor, execute, draw), the vtable of a
// JPAParticleCallBack subclass. Its destructor (dtor_802150FC) calls
// __dt__19JPAParticleCallBackFv.
class cls_804A64A8 : public JPAParticleCallBack {
public:
    cls_804A64A8() : field_0x10(0) {}
    virtual ~cls_804A64A8();
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);

    /* 0x04 */ JGeometry::TVec3f field_0x4;
    /* 0x10 */ u8 field_0x10;
}; // Size: 0x14

#endif
