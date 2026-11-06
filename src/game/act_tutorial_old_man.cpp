// The tutorial old man (actor TJIJ, "Tutorial old man" in
// docs/names/actors.tsv). The spawner (spawnActor_801F9484) constructs it with the
// weak constructor fn_801FFC48, which calls ActorBase_804A5770's and stores
// this class's vtable (0x804AE508). It overrides only draw, update and
// init, all empty; the destructor is the compiler's.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_tutorial_old_man.h"

void TutorialOldMan_804AE508::init() {}

void TutorialOldMan_804AE508::update() {}

void TutorialOldMan_804AE508::draw() {}
