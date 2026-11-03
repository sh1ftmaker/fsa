#include "game/GameAudio.h"
#include "game/cls_804A71A8.h"

extern const f32 lbl_80540228;

// Runs fn_802685D4 with mPos.z held at lbl_80540228 unless the player is in
// state 5 and fn_80240808 is set; the z is kept only if fn_802685D4 left it.
void Player_804A6F38::fn_80268B3C() {
    if (fn_802486A0() == 5 && fn_80240808()) {
        fn_802685D4();
        return;
    }
    f32 saved = mPos.z;
    mPos.z = lbl_80540228;
    fn_802685D4();
    if (lbl_80540228 == mPos.z) {
        mPos.z = saved;
    }
}

cls_804A71A8::~cls_804A71A8() {
    field_0x4 = nullptr;
}
