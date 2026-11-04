#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"

bool Player_804A6F38::fn_8028FBA0() {
    JGeometry::TVec3f pos;
    pos.x = field_0xcb0.x;
    pos.y = field_0xcb0.y;
    pos.z = field_0xcb0.z;
    if (field_0xcf2 == 0) {
        pos.y = field_0xcb0.y - 16.0f;
    }
    if (fn_80290070(0, &mPos, &pos, 5)) {
        field_0xbdc = 0;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_80290E90() {
    if (!fn_80240670()) {
        fn_80301D54(field_0x4);
    }
    field_0x400 = 0;
    if (lbl_8053AAF8->field_0x7e == 0 && !fn_80240670()) {
        if (field_0xce4 < 8) {
            lbl_8053AF80->fn_8045815C(false);
        } else if (field_0xce4 == 8) {
            lbl_8053AF80->fn_8045815C(true);
        }
    }
}

void Player_804A6F38::fn_802922D0() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fb = 1;
    field_0x3f9 = 0;
}

void Player_804A6F38::fn_80292454() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 3;
    field_0x3e8 = 3;
    if (field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(0);
    }
}

void Player_804A6F38::fn_802925C4() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 2;
    field_0x3e8 = 2;
}

void Player_804A6F38::fn_80292124() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 3;
    field_0x3e8 = 3;
    if (!fn_80240670() && field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(0);
    }
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
