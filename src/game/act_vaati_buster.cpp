// The GFBS actor ("Vaati Buster", docs/names/actors.tsv, medium confidence).
// Spawned by fn_801F9484 (inline constructor fn_80201630, __nw__ 0x230): no
// members of its own; it overrides only the destructor and init().

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

// Named after its vtable, 0x8049E3A8.
class VaatiBuster_8049E3A8 : public ActorBase_804A5770 {
public:
    VaatiBuster_8049E3A8() {}
    virtual ~VaatiBuster_8049E3A8();
    virtual void init();
};

void VaatiBuster_8049E3A8::init() {
    ActorBase_804A5770::init();
    fn_802D7440(fn_802D6A9C(), false);
}

VaatiBuster_8049E3A8::~VaatiBuster_8049E3A8() {}
