#include "game/GameAudio.h"
#include "game/Player.h"

void Player_804A6F38::fn_8027EE84() {
    field_0x3f8++;
    if (field_0x3f8 >= 16) {
        field_0x3f8 = 0;
    }
    field_0x3f9 = (field_0x3f8 >> 1) & 0xFF; // mask only to get the original rlwinm (extrwi) instead of srawi
}

void Player_804A6F38::fn_8027F16C() {
    if (!fn_80240808() && !fn_80245FEC() && field_0x1201 != 1) {
        fn_802416FC(0x180);
    }
}

void Player_804A6F38::fn_80281E84() {
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
}

void Player_804A6F38::fn_8028211C() {
    if (field_0xce8 == true && (field_0xcf4 == 0x13 || field_0xcf4 == 0x16)) {
        field_0xce8 = false;
    }
    field_0xcea = 0;
    if (field_0x3b4 < 8) {
        fn_8024BAEC();
    }
}

void Player_804A6F38::fn_8028286C() {
    field_0x344.x = field_0x374.x;
    field_0x344.y = field_0x374.y;
    field_0x344.z = field_0x374.z;
    field_0x380.x = 0.0f;
    field_0x380.y = 0.0f;
    field_0x380.z = 0.0f;
    field_0x458 = 0;
    field_0x394 = 0;
    field_0xcea = 0;
}

void Player_804A6F38::fn_80282AA8() {
    if (field_0x1244 == 0xf) {
        return;
    }
    if (field_0x1244 == 0x10) {
        return;
    }
    field_0xbe4 = 0;
    field_0xbe8 = 0;
}

void Player_804A6F38::fn_80282FC8() {
    field_0x42c = false;
    field_0x430 = 0;
    field_0x42d = 0;
    field_0x400 = 0;
}

void Player_804A6F38::fn_80283944() {}

void Player_804A6F38::fn_80283CEC() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_80283D00() {
    fn_8027EB04();
}

void Player_804A6F38::fn_80283D20() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80284290() {}

void Player_804A6F38::fn_80284328() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
}

void Player_804A6F38::fn_802848F0() {
    field_0x124d = 1;
    field_0xce8 = false;
    field_0x458 = 0;
    if (!field_0x43f) {
        mPos.z = 0.0f;
    }
    if (field_0x1248 == 0x4a) {
        field_0x3fd = 1;
    } else if (field_0x1248 == 0x4b) {
        field_0x3fd = 2;
    } else if (field_0x1248 == 0xb) {
        field_0x3fd = 3;
    } else {
        field_0x3fd = 0;
    }
    field_0x3fe = 0;
}

void Player_804A6F38::fn_8028496C() {
    field_0xda4 = -1;
}

void Player_804A6F38::fn_80284DD0() {
    field_0x458 = 0;
    field_0xda4 = -1;
}

void Player_804A6F38::fn_80284DE4() {
    field_0xd94 = -1;
    field_0xd9c = -1;
    field_0xd98 = 0;
    field_0x1170 = 60;
}

void Player_804A6F38::fn_80285068() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285074() {
    if (field_0x1244 == 0xe) {
        return;
    }
    field_0xd94 = -1;
    field_0xd9c = -1;
    field_0xd98 = 0;
}

void Player_804A6F38::fn_8028537C() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285388() {}

void Player_804A6F38::fn_80285668() {
    if (field_0x1248 == 2) {
        field_0x3f8 = field_0x437;
        field_0x3f9 = field_0x438;
    }
    field_0x458 = 0;
}

void Player_804A6F38::fn_802857C8() {
    if (field_0x1248 == 3) {
        field_0x3f8 = field_0x437;
        field_0x3f9 = field_0x438;
    }
    field_0x458 = 0;
}

void Player_804A6F38::fn_802859B8() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285C28() {
    field_0x458 = 0;
}
