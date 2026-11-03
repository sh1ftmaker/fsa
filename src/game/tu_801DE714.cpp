#include "game/GameAudio.h"
#include "game/act/tu_801DE714.h"

// tu_801DE714 (0x801E1D9C)
bool cls_804A51E4::fn_801E1D9C() const {
    bool result = false;
    if (mAnim.mAnim == 2) {
        result = true;
    }
    return result;
}

// tu_801DE714 (0x801E1D7C): whether mState is 0xB or 0xC.
// The upper bound is written first: the original branches on mState >= 0xD
// before mState < 0xB, and this order is what matches it.
bool cls_804A51E4::fn_801E1D7C() {
    bool result = false;
    if (mState < 0xD && mState >= 0xB) {
        result = true;
    }
    return result;
}
