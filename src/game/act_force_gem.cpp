#include "game/GameAudio.h"
#include "game/act/act_force_gem.h"

// Per-kind value table (fn_8032D564). The two SOB file names that follow it
// (lbl_80471A28, lbl_80471A3C) are string literals of unwritten functions
// that address them off the same base.
static const s32 lbl_804719D8[20] = {
    1, 5, 20, 50, 100, 150, 200, 300, 1000, 1500, 2000, 3000, 404, 403, 402, 401, 439, 440, 441, 442,
};

ForceGem_804AB6D8::~ForceGem_804AB6D8() {}

void ForceGem_804AB6D8::vfunc_0x10() {
    if (field_0x94 == 0x19 || field_0x94 == 0x1a) {
        if (vfunc_0x34()) {
            field_0x280 = 0;
        }
    }
}

s32 ActorBase_804A5770::fn_8032D564(s32 index) { return lbl_804719D8[index]; }
