#include "game/GameAudio.h"
#include "game/act/tu_802ED538.h"

cls_804AA350::~cls_804AA350() {
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
    if (mEmitter244 != NULL) {
        fn_801EE434(mEmitter244);
    }
    if (mEmitter248 != NULL) {
        fn_801EE434(mEmitter248);
    }
}
