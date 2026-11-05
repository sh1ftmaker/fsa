#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_somaria_block.h"

bool Player_804A6F38::fn_802577E0() {
    if (fn_80270C88()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (field_0x1240 == 0x3f || field_0x1240 == 0xc) {
        return false;
    }
    return fn_80251AEC(0x3f);
}

// FIXME: 92.6. Same instructions; the loop counter and the serial swap r28/r29 (original loads the counter later, after the prologue saves).
void Player_804A6F38::fn_80257FF4() {
    for (s32 i = 0; i < 2; i++) {
        s32 serial = field_0xb30[i];
        if (serial >= 0 && !fn_801F856C()->fn_801F666C(serial)) {
            field_0xb30[i] = -1;
        }
    }
}

void Player_804A6F38::fn_80258070() {
    s32 serial = field_0xb2c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
        if (actor == NULL) {
            field_0xb2c = -1;
        } else if (!static_cast<CaneOfSomariaBlock_804AA9F0*>(actor)->fn_8030EA0C()) {
            field_0xb2c = -1;
        }
    }
}

void Player_804A6F38::fn_8025819C() {
    s32 serial = field_0xb3c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
        if (actor == NULL) {
            field_0xb3c = -1;
            field_0xb70 = -1;
        } else {
            JGeometry::TVec2<f32> pos;
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            field_0xb70 = actor->field_0x4;
            field_0x5a0.set(actor->mBox);
            field_0x5a0.addPos(pos);
        }
    }
}

void Player_804A6F38::fn_80258470() {
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(field_0xb18);
    if (actor == NULL) {
        field_0xb18 = -1;
        field_0xb58 = -1;
    } else {
        JGeometry::TVec2<f32> pos;
        pos.x = actor->mPos.x;
        pos.y = actor->mPos.y;
        field_0xb58 = actor->field_0x4;
        field_0x540.set(actor->mBox);
        field_0x540.addPos(pos);
    }
}

void Player_804A6F38::fn_802586CC() {}

void Player_804A6F38::fn_80258880() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025888C() {}

void Player_804A6F38::fn_80258BA8() {
    field_0x458 = 0;
    field_0xb9f = false;
    field_0xba0 = 0;
}

void Player_804A6F38::fn_80258BBC() {
    if (field_0x1244 != 5 && field_0x1244 != 7) {
        mPos.z = 0.0f;
    }
    field_0xb79 = 0;
    field_0x1204 = 0;
}

void Player_804A6F38::fn_80259778() {
    field_0x344.z = 5.4f;
}

void Player_804A6F38::fn_8025A04C() {
    field_0x458 = 0;
    if (fn_80241794() <= 0) {
        field_0xb84 = false;
    } else {
        field_0xb84 = true;
    }
    field_0xb88 = 0;
    field_0xb28 = -1;
    field_0xb8c = 0;
}

bool Player_804A6F38::fn_8025AA24(s32 arg) {
    if (field_0x11f8 >= 0) {
        return false;
    }
    if (fn_80251AEC(0x43)) {
        field_0x11f8 = arg;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025AA8C(s32 arg) {
    if (field_0x11f8 >= 0) {
        return false;
    }
    if (fn_80251AEC(0x42)) {
        field_0x11f8 = arg;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8025AAF4() {
    field_0x11f8 = -1;
}

void Player_804A6F38::fn_8025AEB0() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025AEC4() {
    field_0x11f8 = -1;
}

void Player_804A6F38::fn_8025B218() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025B35C() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025B368() {
    field_0xb99 = false;
    field_0xb9a = false;
    field_0xb9b = false;
    if (field_0x1274 != NULL) {
        field_0x1274->clearStatus(JPAEMIT_Immortal);
        field_0x1274->becomeInvalidEmitter();
        field_0x1274 = NULL;
    }
    if (field_0x128c != NULL) {
        field_0x128c->clearStatus(JPAEMIT_Immortal);
        field_0x128c->becomeInvalidEmitter();
        field_0x128c = NULL;
    }
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025BDC8() {
    field_0x458 = 0;
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025C79C() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025D090() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025D6A0() {
    field_0xb9c = false;
}
