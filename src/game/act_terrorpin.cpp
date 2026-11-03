#include "game/GameAudio.h"
#include "game/act/act_terrorpin.h"

Terrorpin_804A3B10::Terrorpin_804A3B10() {
    field_0x2a8 = 0;
}

Terrorpin_804A3B10::~Terrorpin_804A3B10() {}

// The 0.1f is lbl_8053EB34, the sixth float of the unit's .sdata2 (after the
// literals of draw, vfunc_0x24, vfunc_0x68 and vfunc_0x5C, not written yet).
extern const f32 lbl_8053EB34;

void Terrorpin_804A3B10::vfunc_0x7C() {
    if (__fabsf(field_0x3c.z) >= lbl_8053EB34) {
        if (mFlags & 0x2000) {
            fn_801F0E34(0x50014, 0);
        } else {
            fn_801F0E34(0x50015, 0);
        }
    }
}
