// Actor SZSC ("Zelda scroll-lock NPC", JP ゼルダスクロール禁止NPC, in
// docs/names/actors.tsv): the spawner (fn_801F9484) allocates 0x234 bytes
// for it and calls its constructor fn_802013D8, which stores the vtable
// 0x804AC708.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_zelda_scroll_lock.h"

ZeldaScrollLock_804AC708::~ZeldaScrollLock_804AC708() {}

void ZeldaScrollLock_804AC708::draw() {}

void ZeldaScrollLock_804AC708::update() {
    switch (mState) {
    case 0:
        if (!fn_802DCD0C(fn_802D6A9C())) {
            fn_8022ADE4();
            mState = 1;
        }
        break;
    case 1:
        fn_8022ADE4();
        if (vfunc_0x34() == true) {
            fn_8022ADCC();
            fn_801F3668();
        }
        break;
    }
}

void ZeldaScrollLock_804AC708::init() {
    mState = 0;
}
