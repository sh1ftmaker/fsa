#include "game/GameAudio.h"
#include "game/Player.h"

void Player_804A6F38::fn_802922D0() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fb = 1;
    field_0x3f9 = 0;
}

void Player_804A6F38::fn_802925C4() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 2;
    field_0x3e8 = 2;
}

void Player_804A6F38::fn_80292E90() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fd = 1;
    fn_8024F0E8(0x2000C);
}

void Player_804A6F38::fn_80293760() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_802938B8() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fd = 1;
    if (field_0xcf4 != 0x1D) {
        fn_8024F0E8(0x2000C);
    }
}

void Player_804A6F38::fn_80293B1C() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_80295240() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = -0.5f;
    field_0x344.z = 0.0f;
}

void Player_804A6F38::fn_802954B8() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
}

void Player_804A6F38::fn_80295584() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fd = 1;
    field_0x3dc = 2;
    field_0x3e8 = 2;
}
