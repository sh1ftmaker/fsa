#include "game/GameAudio.h"
#include "game/act/tu_80354D64.h"

// FIXME(shotgun): not written (size 0x9C4): fn_803558A4
// FIXME(shotgun): not written: fn_80354D64, fn_80354FE8, fn_8035533C, fn_803554B8, fn_80355610, fn_80356268
//   (the class declares no virtuals yet)

Log_804ACD70::~Log_804ACD70() {
    if (mEmitter2e0 != NULL) {
        releaseEmitter_801EE434(mEmitter2e0);
    }
}
