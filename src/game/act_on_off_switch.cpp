#include "game/GameAudio.h"
#include "game/act/act_on_off_switch.h"

cls_804A8FC8::~cls_804A8FC8() {
    if (mEmitter230 != NULL) {
        fn_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        fn_801EE434(mEmitter234);
    }
}
