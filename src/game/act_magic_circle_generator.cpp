#include "game/GameAudio.h"
#include "game/act/act_magic_circle_generator.h"

cls_804A9FB0::~cls_804A9FB0() {
    if (mEmitter230 != NULL) {
        fn_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        fn_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        fn_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        fn_801EE434(mEmitter23c);
    }
    if (mEmitter240 != NULL) {
        fn_801EE434(mEmitter240);
    }
}
