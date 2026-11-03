// The GFBS actor ("Vaati Buster", docs/names/actors.tsv, medium confidence).
// Spawned by fn_801F9484 (inline constructor fn_80201630, __nw__ 0x230): no
// members of its own; it overrides only the destructor and init().

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_vaati_buster.h"

void VaatiBuster_8049E3A8::init() {
    ActorBase_804A5770::init();
    fn_802D6A9C()->fn_802D7440(false);
}

VaatiBuster_8049E3A8::~VaatiBuster_8049E3A8() {}
