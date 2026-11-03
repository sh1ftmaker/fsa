// Actor EXPL ("Explosion" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x23C bytes for it (0x801FB3B0) and calls its
// constructor fn_80203C68, which stores the vtable 0x804A7FB8.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Named after its vtable (0x804A7FB8).
class Explosion_804A7FB8 : public ActorBase_804A5770 {
public:
    Explosion_804A7FB8() {}
    virtual ~Explosion_804A7FB8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234; // 20 after init
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
}; // Size: 0x23C

void Explosion_804A7FB8::draw() {}

// FIXME: update (0x802A7D40) is not written yet: it calls fn_8013CC50 (the
// effect spawner, game/tu_8013C5C8.cpp) and fn_802D79E8 (game/tu_802D39B8.cpp),
// which no shared header declares yet. Its float literals (24, 16, 1, 4, 0)
// come first in .sdata2, so init's -48 and 48 pair only once it exists.

Explosion_804A7FB8::~Explosion_804A7FB8() {
    if (mEmitter238 != NULL) {
        fn_801EE434(mEmitter238);
    }
}

void Explosion_804A7FB8::init() {
    mBox.set(-48.0f, -48.0f, 48.0f, 48.0f);
    mEmitter238 = NULL;
    field_0x230 = 0;
    field_0x234 = 20;
}
