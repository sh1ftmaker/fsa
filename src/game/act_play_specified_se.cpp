// Actor SEN2 ("Play specified SE" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x23C bytes for it and calls its constructor
// fn_802011F0, which stores the vtable 0x804A9F00.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// The ids init's spawn parameter selects from (one entry, 3).
static const u32 lbl_80541250[] = {3};

// Named after its vtable (0x804A9F00).
class PlaySpecifiedSe_804A9F00 : public ActorBase_804A5770 {
public:
    PlaySpecifiedSe_804A9F00() {}
    virtual ~PlaySpecifiedSe_804A9F00();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230; // bit 26 of the spawn parameter (0x90)
    /* 0x234 */ u32 field_0x234; // low 8 bits: index into lbl_80541250
    /* 0x238 */ s32 field_0x238; // bits 8-19: frames to wait
}; // Size: 0x23C

PlaySpecifiedSe_804A9F00::~PlaySpecifiedSe_804A9F00() {}

void PlaySpecifiedSe_804A9F00::draw() {}

// Counts field_0x238 down, then (unless field_0x230 is set) calls
// fn_801F0D20 with the selected id once and removes the actor.
void PlaySpecifiedSe_804A9F00::update() {
    if (field_0x238 == 0) {
        if (field_0x230 == 0) {
            fn_801F0D20(lbl_80541250[field_0x234]);
        }
        mAlive = false;
    } else {
        field_0x238--;
    }
}

void PlaySpecifiedSe_804A9F00::init() {
    field_0x230 = ((u32)field_0x90 >> 26) & 1;
    field_0x234 = field_0x90 & 0xFF;
    field_0x238 = ((u32)field_0x90 >> 8) & 0xFFF;
}
