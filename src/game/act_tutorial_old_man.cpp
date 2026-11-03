// The tutorial old man (actor TJIJ, "Tutorial old man" in
// docs/names/actors.tsv). The spawner (fn_801F9484) constructs it with the
// weak constructor fn_801FFC48, which calls ActorBase_804A5770's and stores
// this class's vtable (0x804AE508). It overrides only draw, update and
// init, all empty; the destructor is the compiler's.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Named after its vtable (0x804AE508).
class TutorialOldMan_804AE508 : public ActorBase_804A5770 {
public:
    TutorialOldMan_804AE508() {}

    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
};

void TutorialOldMan_804AE508::draw() {}

void TutorialOldMan_804AE508::update() {}

void TutorialOldMan_804AE508::init() {}
