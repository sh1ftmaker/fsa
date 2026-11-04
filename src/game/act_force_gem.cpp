#include "game/GameAudio.h"
#include "game/act/act_force_gem.h"

// Per-kind value table, then the SOB file names of the two enemy resources
// spelled as four-character words.
static const s32 lbl_804719D8[30] = {
    1,    5,     20,    50,    100,   150,   200,   300,   1000,  1500,
    2000, 3000,  404,   403,   402,   401,   439,   440,   441,   442,
    'enem', 'y_bo', 'th2_', 'sob.', 'bin\0',
    'enem', 'y_gc', '_sob', '.bin', 0,
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
