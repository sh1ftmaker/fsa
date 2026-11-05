#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Player.h"

bool Player_804A6F38::fn_8025EDD4(s32 a, const JGeometry::TVec3f* pos) {
    if (field_0xd68 == a) {
        field_0xd68 = -1;
        field_0xd78 = *pos;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8025EE0C(s32 a, const JGeometry::TVec3f* b) {
    if (field_0xd68 == a) {
        field_0x300.set(b->x, b->y, b->z);
    }
}

// field_0xd64 == 1 as a u8. The original's callers below expand this inline (they test
// it again with cmplwi 1) and fn_8025F684 is its out-of-line copy; a member inline in
// Player.h was measured: fn_8025F684 is then not emitted and fn_8023EFEC (inlines it) drops.
static inline u8 isField0xd64One(const Player_804A6F38& p) {
    return p.field_0xd64 == 1;
}

bool Player_804A6F38::fn_8025EE34(s32 arg) {
    return field_0xd68 == arg;
}

bool Player_804A6F38::fn_8025EF68(s32 arg) {
    if (field_0xd88 == arg) {
        return true;
    }
    if (field_0xd88 >= 0) {
        return false;
    }
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(arg);
    if (actor != 0) {
        if (actor->fn_801F36D4(field_0x4)) {
            field_0xd88 = arg;
            return true;
        }
        return false;
    }
    return false;
}

bool Player_804A6F38::fn_8025F418() {
    if (field_0xd70 >= 0 && field_0xd70 < 4 && field_0xd64 == 0) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025F448() {
    if ((field_0xd70 >= 0 && field_0xd70 < 4 || field_0xd68 >= 0) && field_0xd64 == 0) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025F484() const {
    if (isField0xd64One(*this) == 1) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != 0 && actor->field_0x1a4 > 0) {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool Player_804A6F38::fn_8025F500() {
    if (isField0xd64One(*this) == 1 && field_0xd68 >= 0) {
        return true;
    }
    return false;
}

s32 Player_804A6F38::fn_8025F534() {
    if (field_0xd64 == 1 || field_0xd70 < 0 || field_0xd70 >= 4) {
        return -1;
    }
    return field_0xd70;
}

s32 Player_804A6F38::fn_8025F55C() {
    if (isField0xd64One(*this) == 1 && field_0xd70 >= 0 && field_0xd70 < 4) {
        return field_0xd70;
    }
    return -1;
}

bool Player_804A6F38::fn_8025F590() {
    if (isField0xd64One(*this) == 1 && field_0xd70 >= 0 && field_0xd70 < 4) {
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8025F5CC() {
    if (field_0xd64 != 0) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != 0) {
                actor->mAlive = false;
                field_0xd6c = 'NULL';
                field_0xd64 = 0;
                field_0xd68 = -1;
            }
        }
    }
}

s32 Player_804A6F38::fn_8025F648() {
    if (field_0xd64 == 1) {
        return field_0xd6c;
    }
    return 'NULL';
}

s32 Player_804A6F38::fn_8025F668() {
    if (field_0xd64 == 1) {
        return field_0xd68;
    }
    return -1;
}

bool Player_804A6F38::fn_8025F684() {
    return isField0xd64One(*this);
}

bool Player_804A6F38::fn_8025FEDC() {
    if (!fn_8025F448()) {
        return false;
    }
    fn_80251AEC(0);
    field_0xd70 = -1;
    field_0xd68 = -1;
    mPos.z = 0.0f;
    return true;
}

bool Player_804A6F38::fn_8026000C() {
    fn_8025F698(false);
    return fn_80251AEC(0);
}

bool Player_804A6F38::fn_80260048(s32 arg) {
    if (!fn_80270A8C()) {
        return false;
    }
    field_0xd70 = arg;
    fn_80248FF0();
    return true;
}

void Player_804A6F38::fn_80261468() {
    if (field_0xd74 > 0) {
        field_0xd74--;
    }
}

void Player_804A6F38::fn_8026173C() {
    field_0x121c = 0;
    field_0xd84 = 20;
}

void Player_804A6F38::fn_80262234() {
    fn_80258BBC();
}

void Player_804A6F38::fn_80262B6C() {
    fn_80259660();
}

void Player_804A6F38::fn_80262B8C() {
    fn_80283D2C();
}

void Player_804A6F38::fn_802632A4() {}

void Player_804A6F38::fn_802635C8() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_802635D4() {}

void Player_804A6F38::fn_802636C4() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_802636D0() {}

void Player_804A6F38::fn_80263848() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80263854() {
    field_0x459 = 0;
}

void Player_804A6F38::fn_80264410() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80264768() {
    field_0x458 = 0;
}
