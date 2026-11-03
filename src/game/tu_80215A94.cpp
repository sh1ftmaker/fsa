#include "game/GameAudio.h"
#include "game/tu_80215A94.h"

s32 cls_80215E48::fn_80215E58(s32 key) {
    s32 result = -1;
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        if (key == entry->mKey) {
            result = i;
            break;
        }
        entry++;
    }
    return result;
}
