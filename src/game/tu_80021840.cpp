// The unit opens with a stub free function, called by tu_80134C04 (a screen
// mode query: the caller compares the result with 0, 1 and 2). The rest of
// the unit is the standard game __sinit and its nine JASMemPool destructors,
// which come from game/GameAudio.h.

#include "game/GameAudio.h"

s32 fn_80021840() {
    return 0;
}
