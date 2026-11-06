// Actor FSPO ("Fuse point" in docs/names/actors.tsv): an EnemyBase_804A4808
// with no fields of its own.
//
// Not written yet: init (0x801E7C88; it calls fn_80210B34, fn_80210AF8,
// fn_801E9AFC and fn_801EA75C, which have no declarations yet).

#include "game/GameAudio.h"
#include "game/act/act_fuse_point.h"

FusePoint_804A54C0::FusePoint_804A54C0() {}

// Does nothing: unlike EnemyBase_804A4808's, no hit checks or movement.
void FusePoint_804A54C0::update() {}

FusePoint_804A54C0::~FusePoint_804A54C0() {}
