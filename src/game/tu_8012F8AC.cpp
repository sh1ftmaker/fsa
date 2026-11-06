#include "game/GameAudio.h"
#include "game/act/tu_8012F8AC.h"
#include "game/tu_8012F3E4.h"

void cls_8049E9A8::init() {
    ActorBase_804A5770::init();
}

void cls_8049E9A8::update() {
    ActorBase_804A5770::update();
    switch (mState) {
    case 0:
        if (fn_8012F554()->fn_8012F3E4()) {
            vfunc_0x30(0xFF, true);
            mState = 1;
        }
        break;
    case 1:
        break;
    }
}

cls_8049E9A8::~cls_8049E9A8() {}
