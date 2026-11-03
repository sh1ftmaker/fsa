#include "game/GameAudio.h"
#include "game/Player.h"

// The 0.0f in .sdata2 is named (lbl_805405B4) in the original, which needs an
// array here: a scalar const is folded into an anonymous literal.
static const f32 lbl_805405B4[1] = {0.0f};

void Player_804A6F38::fn_80293760() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_80293B1C() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_802954B8() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = lbl_805405B4[0];
    field_0x344.y = lbl_805405B4[0];
    field_0x344.z = lbl_805405B4[0];
}

void Player_804A6F38::fn_802925C4() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 2;
    field_0x3e8 = 2;
}

void Player_804A6F38::fn_802922D0() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fb = 1;
    field_0x3f9 = 0;
}
