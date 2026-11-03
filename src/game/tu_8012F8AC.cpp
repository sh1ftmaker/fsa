#include "game/GameAudio.h"
#include "game/act/tu_8012F8AC.h"

// Two functions of game/tu_8012F3E4.cpp (not written yet). fn_8012F554 is
// the guarded-static getter of the 0x804BDDFC object (an s32 count and
// 32 s32 slots, all -1 after construction); fn_8012F3E4 takes it and
// returns true once every one of its slots names a live actor of the
// same kind. Propose both for the header of that unit's area.
struct Unk_804BDDFC;
extern "C" Unk_804BDDFC* fn_8012F554();
extern "C" bool fn_8012F3E4(Unk_804BDDFC*);

cls_8049E9A8::~cls_8049E9A8() {}

void cls_8049E9A8::update() {
    ActorBase_804A5770::update();
    switch (mState) {
    case 0:
        if (fn_8012F3E4(fn_8012F554())) {
            vfunc_0x30(0xFF, true);
            mState = 1;
        }
        break;
    case 1:
        break;
    }
}

void cls_8049E9A8::init() {
    ActorBase_804A5770::init();
}
