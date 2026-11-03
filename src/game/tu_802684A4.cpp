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

// Stores pos's x and y in field_0x324 when fn_80251AEC accepts it for this
// player: the arg is 0x38 on field_0xd64 == 0, else 0x13.
bool Player_804A6F38::fn_8026E5F8(const JGeometry::TVec2f* pos) {
    if (field_0xd64 == 0) {
        if (fn_80251AEC(0x38)) {
            field_0x324.x = pos->x;
            field_0x324.y = pos->y;
            return true;
        }
    } else if (fn_80251AEC(0x13)) {
        field_0x324.x = pos->x;
        field_0x324.y = pos->y;
        return true;
    }
    return false;
}

cls_804A71A8::~cls_804A71A8() {
    field_0x4 = nullptr;
}
