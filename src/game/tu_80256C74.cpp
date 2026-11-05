#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_somaria_block.h"
#include "game/Map.h"
#include "game/cls_804A71A8.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/ViewLayer.h"

// One entry per facing (0x3DC: right, left, down, up): 8 to the side and 10
// below, read by the enter of state 0x39 (fn_8025E79C).
static const Vec lbl_8046AF00[4] = {
    {-8.0f, 0.0f, -10.0f}, {8.0f, 0.0f, -10.0f}, {0.0f, -8.0f, -10.0f}, {0.0f, 0.0f, -10.0f},
};

// The recoil hop's speed per facing (0x3DC: right, left, down, up): 2 back
// against the facing and 4 up (enter of state 0x41, fn_8025D7E4).
static const Vec lbl_8046AFE0[4] = {
    {-2.0f, 0.0f, 4.0f}, {2.0f, 0.0f, 4.0f}, {0.0f, -2.0f, 4.0f}, {0.0f, 2.0f, 4.0f},
};

// The dig effect per result of fn_802A1844 (1-4) and facing (fn_80258890).
static const u16 lbl_8046B200[4][4] = {
    {0x137, 0x132, 0x12D, 0x13C},
    {0x135, 0x134, 0x133, 0x136},
    {0x13A, 0x139, 0x138, 0x13B},
    {0x130, 0x12F, 0x12E, 0x131},
};

// One step of 12.999 along each facing (0: +x, 1: -x, 2: +y, 3: -y): where item 8
// ('HBMB') is placed in front of the player (fn_8025785C).
static const Vec lbl_8046B220[4] = {
    {12.999f, 0.0f, 0.0f}, {-12.999f, 0.0f, 0.0f}, {0.0f, 12.999f, 0.0f}, {0.0f, -12.999f, 0.0f},
};

bool Player_804A6F38::fn_80256C74(const JGeometry::TBox2f* box, s32 layer) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!fn_80270A8C()) {
        return false;
    }
    if (!field_0xba4) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x5dc);
    if (overlap.intersect(*box)) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80256DCC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!field_0xb9e) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x5cc);
    if (overlap.intersect(*box)) {
        out->x = field_0x5cc.f.x + (field_0x5cc.i.x - field_0x5cc.f.x) * 0.5f;
        out->y = field_0x5cc.f.y + (field_0x5cc.i.y - field_0x5cc.f.y) * 0.5f;
        return true;
    }
    return false;
}

u8 Player_804A6F38::fn_80256F44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (!field_0xb99) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x5b0);
    if (overlap.intersect(*box)) {
        out->x = field_0x5b0.f.x + (field_0x5b0.i.x - field_0x5b0.f.x) * 0.5f;
        out->y = field_0x5b0.f.y + (field_0x5b0.i.y - field_0x5b0.f.y) * 0.5f;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802573E0(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (field_0xb38 < 0) {
        return false;
    }
    if (field_0xb6c != layer) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x590);
    if (overlap.intersect(*box)) {
        out->x = field_0x590.f.x + (field_0x590.i.x - field_0x590.f.x) * 0.5f;
        out->y = field_0x590.f.y + (field_0x590.i.y - field_0x590.f.y) * 0.5f;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802574CC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (field_0xb18 < 0) {
        return false;
    }
    if (field_0xb58 != layer) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x540);
    if (overlap.intersect(*box)) {
        out->x = field_0x540.f.x + (field_0x540.i.x - field_0x540.f.x) * 0.5f;
        out->y = field_0x540.f.y + (field_0x540.i.y - field_0x540.f.y) * 0.5f;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802575B8(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    for (s32 i = 0; i < 3; i++) {
        if (field_0xb0c[i] >= 0 && field_0xb4c[i] == layer) {
            JGeometry::TBox2f overlap(field_0x510[i]);
            if (overlap.intersect(*box)) {
                out->x = field_0x510[i].f.x + (field_0x510[i].i.x - field_0x510[i].f.x) * 0.5f;
                out->y = field_0x510[i].f.y + (field_0x510[i].i.y - field_0x510[i].f.y) * 0.5f;
                field_0xb80 = i;
                return true;
            }
        }
    }
    field_0xb80 = -1;
    return false;
}

bool Player_804A6F38::fn_802576CC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    for (s32 i = 0; i < 3; i++) {
        if (field_0xb00[i] >= 0 && field_0xb40[i] == layer) {
            JGeometry::TBox2f overlap(field_0x4e0[i]);
            if (overlap.intersect(*box)) {
                out->x = field_0x4e0[i].f.x + (field_0x4e0[i].i.x - field_0x4e0[i].f.x) * 0.5f;
                out->y = field_0x4e0[i].f.y + (field_0x4e0[i].i.y - field_0x4e0[i].f.y) * 0.5f;
                field_0xb7c = i;
                return true;
            }
        }
    }
    field_0xb7c = -1;
    return false;
}

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

// FIXME: 90.40. Structurally close: the original leaves item 7's checks (living held
// actor, a player holding 'RCK2', the floor probe / height) and item 6's actor count by
// branching straight to the final store, which `blocked` / `count` stand in for here
// (9 extra instructions, and this / the item index swap r30 / r31).
// The per-frame item check: while fn_8022B334 and fn_8022C310 hold, the selected
// item fn_802416C4 (1-10) requests its state. Items 3 and 6 allow three of their
// actors (field_0xb00 / field_0xb1c) at a time, item 8 ('HBMB') two (field_0xb30).
void Player_804A6F38::fn_8025785C() {
    if (field_0xb74 > 0) {
        field_0xb74--;
    }
    if (fn_8022B334(field_0x4)) {
        if (fn_80240808()) {
            s32 item = fn_802416C4();
            if (item != fn_802373EC(fn_80240644())) {
                fn_80241618(fn_802373EC(fn_80240644()), 1);
            }
        }
        if (fn_8022C310(field_0x4)) {
            if (fn_802416C4() == 1) {
                fn_80251AEC(0x39);
            } else if (fn_802416C4() == 2) {
                fn_80251AEC(0x48);
            } else if (fn_802416C4() == 3) {
                if (field_0xb18 < 0) {
                    s32 count = 0;
                    for (s32 i = 0; i < 3; i++) {
                        if (field_0xb00[i] >= 0) {
                            count++;
                        }
                    }
                    if (count < 3) {
                        fn_80251AEC(0x3d);
                    }
                }
            } else if (fn_802416C4() == 4) {
                if (field_0xb18 < 0 && field_0xb74 <= 0 &&
                    (!fn_8022B160() || ((!fn_80240808() || fn_802486A0() == 4) && !lbl_8053ABB8[field_0x4]))) {
                    fn_80251AEC(0x3a);
                }
            } else if (fn_802416C4() == 5) {
                fn_80251AEC(0x3c);
            } else if (fn_802416C4() == 6) {
                if (fn_802415CC() >= 2) {
                    fn_80251AEC(0x45);
                } else {
                    s32 count = 0;
                    if (fn_802415CC() < 2) {
                        for (s32 i = 0; i < 3; i++) {
                            if (field_0xb1c[i] >= 0) {
                                count++;
                            }
                        }
                    }
                    if (count < 3) {
                        fn_80251AEC(0x44);
                    }
                }
            } else if (fn_802416C4() == 7) {
                bool blocked = false;
                if (fn_8025F500()) {
                    ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
                    if (held != NULL && held->field_0x1a4 != 0) {
                        blocked = true;
                    }
                }
                for (s32 i = 0; !blocked && i < 4; i++) {
                    if (fn_80243194(i) && fn_8023EEB4(i) == 'RCK2') {
                        blocked = true;
                    }
                }
                if (!blocked) {
                    if (!viewLayerFlag(field_0x3b4)) {
                        if (mPos.z > 0.0f) {
                            blocked = true;
                        }
                    } else if (!fn_802518AC(true)) {
                        JGeometry::TVec3f pos;
                        fn_8024B298(2, &pos);
                        pos.y += 2.0f;
                        if (fn_8022461C(&pos, field_0x3b4) == 2) {
                            blocked = true;
                        }
                    }
                }
                if (!blocked) {
                    if (fn_8025F684()) {
                        fn_80251AEC(0x14);
                    } else {
                        fn_80251AEC(0x46);
                    }
                }
            } else if (fn_802416C4() == 8) {
                for (s32 i = 0; i < 2; i++) {
                    if (field_0xb30[i] < 0) {
                        JGeometry::TVec3f pos(lbl_8046B220[field_0x3dc]);
                        pos += mPos;
                        pos.z = 0.0f;
                        if (fn_8022461C(&pos, field_0x3b4) == 0xe) {
                            break;
                        }
                        s32 serial;
                        if (fn_802415CC() >= 2) {
                            serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HBMB, field_0x3b4, &pos, 8, -1, field_0x4);
                        } else {
                            serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HBMB, field_0x3b4, &pos, 0, -1, field_0x4);
                        }
                        if (getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
                            field_0xb30[i] = serial;
                            fn_8024F0E8(0x20006);
                            break;
                        }
                    }
                }
            } else if (fn_802416C4() == 9) {
                fn_80251AEC(0x47);
            } else if (fn_802416C4() == 10) {
                if (field_0xb18 < 0) {
                    fn_80251AEC(0x3b);
                }
            } else if (fn_80246228()) {
                field_0x1200 = true;
            }
        }
    }
    field_0x1174 = 0;
}

bool Player_804A6F38::fn_80257DB4() {
    if (field_0x1174) {
        return false;
    }
    if (fn_80240808() && fn_80230188(fn_80240644())) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (fn_80270A8C()) {
        return false;
    }
    if (fn_8025F684() && fn_802416C4() != 7) {
        return false;
    }
    if (fn_8025F448()) {
        return false;
    }
    if (fn_802412A4()) {
        return false;
    }
    if (fn_8024C448()) {
        return false;
    }
    if (fn_80241230()) {
        return false;
    }
    if (fn_80250D30()) {
        return false;
    }
    if (fn_802411C4()) {
        return false;
    }
    if (fn_80240B70()) {
        return false;
    }
    if (fn_80240B5C()) {
        return false;
    }
    if (fn_80240B84()) {
        return false;
    }
    if (fn_8024BBB0()) {
        return false;
    }
    if (fn_80240D64()) {
        return false;
    }
    if (fn_80240DAC()) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (fn_8026FCB0()) {
        return false;
    }
    if (fn_802412C4()) {
        return false;
    }
    if (field_0xd74 != 0) {
        return false;
    }
    return field_0xd3c < 0;
}

void Player_804A6F38::fn_80257FF4() {
    s32 serial; // declared outside the loop: the original keeps the counter in the lower saved register
    for (s32 i = 0; i < 2; i++) {
        serial = field_0xb30[i];
        if (serial >= 0 && !getActorManager_801F856C()->findActorBySerial_801F666C(serial)) {
            field_0xb30[i] = -1;
        }
    }
}

void Player_804A6F38::fn_80258070() {
    s32 serial = field_0xb2c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0xb2c = -1;
        } else if (!static_cast<CaneOfSomariaBlock_804AA9F0*>(actor)->fn_8030EA0C()) {
            field_0xb2c = -1;
        }
    }
}

void Player_804A6F38::fn_802580E0() {
    f32 target;
    if (fn_80270A8C()) {
        target = 0.0f;
    } else if (lbl_8053AAF8->field_0x7f) {
        if (fn_802416C4() == 2) {
            target = 100.0f;
        } else {
            target = 0.0f;
        }
    } else {
        target = 85.0f;
    }
    if (field_0xb90 > target) {
        field_0xb90 -= 1.0f;
        if (field_0xb90 < target) {
            field_0xb90 = target;
        }
    } else if (field_0xb90 < target) {
        field_0xb90 += 1.0f;
        if (field_0xb90 > target) {
            field_0xb90 = target;
        }
    }
}

void Player_804A6F38::fn_8025819C() {
    s32 serial = field_0xb3c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
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

void Player_804A6F38::fn_8025825C() {
    s32 serial = field_0xb38;
    if (serial < 0) {
        field_0xb68 = 0;
        return;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        field_0xb68 = 0;
        field_0xb38 = -1;
        field_0xb6c = -1;
        return;
    }
    if (actor->field_0x3c.isZero()) {
        field_0xb68 = 0;
    } else {
        field_0xb68 = 1;
    }
    JGeometry::TVec2<f32> pos;
    pos.x = actor->mPos.x;
    pos.y = actor->mPos.y;
    field_0xb6c = actor->field_0x4;
    field_0x590.set(actor->mBox);
    field_0x590.addPos(pos);
}

void Player_804A6F38::fn_80258384() {
    s32 serial; // declared outside the loop: the original keeps the counter in the lower saved register
    for (s32 i = 0; i < 3; i++) {
        serial = field_0xb1c[i];
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor == NULL) {
                field_0xb1c[i] = -1;
                field_0xb5c[i] = -1;
            } else {
                JGeometry::TVec2<f32> pos;
                pos.x = actor->mPos.x;
                pos.y = actor->mPos.y;
                field_0xb5c[i] = actor->field_0x4;
                field_0x550[i].set(actor->mBox);
                field_0x550[i].addPos(pos);
            }
        }
    }
}

void Player_804A6F38::fn_80258470() {
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb18);
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

void Player_804A6F38::fn_8025851C() {
    for (s32 i = 0; i < 3; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb0c[i]);
        if (actor == NULL) {
            field_0xb0c[i] = -1;
            field_0xb4c[i] = -1;
        } else {
            JGeometry::TVec2<f32> pos;
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            field_0xb4c[i] = actor->field_0x4;
            field_0x510[i].set(actor->mBox);
            field_0x510[i].addPos(pos);
        }
    }
}

void Player_804A6F38::fn_802585F4() {
    for (s32 i = 0; i < 3; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb00[i]);
        if (actor == NULL) {
            field_0xb00[i] = -1;
            field_0xb40[i] = -1;
        } else {
            JGeometry::TVec2<f32> pos;
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            field_0xb40[i] = actor->field_0x4;
            field_0x4e0[i].set(actor->mBox);
            field_0x4e0[i].addPos(pos);
        }
    }
}

void Player_804A6F38::fn_802586CC() {}

// Update of state 0x48 (item 2, the Fire Rod: it spawns ACTOR_FIRE, "Fire Rod fire"):
// at frame 2, when fn_80247594(0) allows it and the player's last fire (field_0xb3c)
// is gone, one is spawned by the player, with the item level fn_802415CC in bits 12+
// of its argument; the state ends after 8 frames.
void Player_804A6F38::fn_802586D0() {
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 6) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 8) {
        field_0x3fb = 0;
    } else {
        field_0x3fb = 0;
        fn_80251AEC(0);
    }
    if (field_0x3fa == 2 && fn_80247594(0) && field_0xb3c < 0) {
        JGeometry::TVec3f pos = mPos;
        switch (field_0x3dc) {
        case 0:
            pos.y -= 2.0f;
            break;
        case 1:
            pos.y -= 2.0f;
            break;
        case 2:
            pos.x += 2.0f;
            break;
        case 3:
            pos.x -= 2.0f;
            break;
        }
        s32 serial; // declared before index: the original gives serial the higher saved register
        s32 index = field_0x4;
        serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FIRE, field_0x3b4, &pos,
                                                (fn_802415CC() << 12) | ((field_0x3dc & 3) << 4) | (index & 3) | 0x140,
                                                -1, index);
        if (getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
            field_0xb3c = serial;
        }
    }
    field_0x3fa++;
    fn_80268B3C();
}

void Player_804A6F38::fn_80258880() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025888C() {}

// FIXME: 99.97. Instructions identical; only the lhz of the effect table lbl_8046B200 is marked
// (the table sits in this unit's still-unwritten .rodata, so its placement differs).
// Update of state 0x47 (item 9): a dig 16 in front of the player at floor level.
// Frame 11 sets the 8x8 box field_0x5cc there (field_0xb9e), frame 12 asks the
// background (fn_802A1844) and plays a sound by the result; results 1-4 give an
// effect by facing at frame 28. 0x3FD selects the short animation (frames 4-6).
void Player_804A6F38::fn_80258890() {
    JGeometry::TVec3f pos = mPos;
    pos.z = 0.0f;
    switch (field_0x3dc) {
    case 0:
        pos.x += 16.0f;
        break;
    case 1:
        pos.x -= 16.0f;
        break;
    case 2:
        pos.y += 16.0f;
        break;
    case 3:
        pos.y -= 16.0f;
        break;
    }
    if (field_0x3fd == 1) {
        if (field_0x3fa < 16) {
            field_0x3fb = 4;
        } else if (field_0x3fa < 19) {
            field_0x3fb = 5;
        } else if (field_0x3fa < 22) {
            field_0x3fb = 6;
        } else {
            field_0x3fb = 6;
            fn_80251AEC(0);
        }
    } else {
        if (field_0x3fa < 12) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 28) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 32) {
            field_0x3fb = 2;
        } else if (field_0x3fa < 40) {
            field_0x3fb = 3;
        } else {
            field_0x3fb = 3;
            fn_80251AEC(0);
        }
        if (field_0x3fa == 11) {
            field_0xb9e = true;
            field_0x5cc.set(pos.x - 4.0f, pos.y - 4.0f, pos.x + 4.0f, pos.y + 4.0f);
        }
        if (field_0x3fa == 12) {
            field_0xba0 = gBgRewrite_8053AC98->fn_802A1844(&pos, field_0x4, field_0x3b4);
            if ((field_0xba0 > 0 && field_0xba0 <= 4) || field_0xb9f) {
                fn_8024F0E8(0x20044);
            } else {
                field_0x3fb = 0;
                field_0x3fd = 1;
                if (fn_8022461C(&pos, field_0x3b4) == 0x39) {
                    fn_8024F0E8(0x20040);
                } else if (field_0xba0 == 8) {
                    fn_8024F0E8(0x2004F);
                } else {
                    fn_8024F0E8(0x20066);
                }
            }
            field_0xb9e = false;
            field_0xb9f = false;
        }
        if (field_0x3fa == 28 && field_0x3fd == 0 && field_0xba0 > 0 && field_0xba0 <= 4) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, lbl_8046B200[field_0xba0 - 1][field_0x3dc], 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    field_0x3fa++;
    fn_80268B3C();
}

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

void Player_804A6F38::fn_80259784() {
    if (field_0x12c4 != NULL) {
        field_0x12c4->becomeInvalidEmitter();
        field_0x12c4 = NULL;
    }
    if (field_0x12c8 != NULL) {
        field_0x12c8->becomeInvalidEmitter();
        field_0x12c8 = NULL;
    }
    s32 serial = field_0xb28;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802BAA00();
        }
        field_0xb28 = -1;
    }
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

void Player_804A6F38::fn_8025A0B0() {
    if (field_0x12c4 != NULL) {
        field_0x12c4->becomeInvalidEmitter();
        field_0x12c4 = NULL;
    }
    if (field_0x12c8 != NULL) {
        field_0x12c8->becomeInvalidEmitter();
        field_0x12c8 = NULL;
    }
    s32 serial = field_0xb28;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802BAA00();
        }
        field_0xb28 = -1;
    }
}

void Player_804A6F38::fn_8025A9C0() {
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

// FIXME: 99.81. regalloc-only: the original keeps the speed 6.0f in f30 and dir.z of
// the isZero test in f31 (ours swaps them), as fn_8025AB00 (state 0x43) does with its
// variable speed.
// Update of state 0x42 (requested by fn_8025AA8C with the actor serial field_0x11f8):
// the player spins (the facing turns every two frames) and is drawn 6 a frame across
// the floor toward that actor; it ends (state 0) when the actor is gone or on another
// layer, field_0xafc is 1, or a side flag of field_0xbf0 is set (anyFlag48).
void Player_804A6F38::fn_8025AED0() {
    field_0x3fc++;
    s32 turn = (field_0x3fc >> 1) & 3;
    if (turn == 0) {
        field_0x3dc = 0;
        field_0x3e8 = 0;
    } else if (turn == 1) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    } else if (turn == 2) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else {
        field_0x3dc = 3;
        field_0x3e8 = 3;
    }
    fn_80268B3C();
    if (field_0xafc != 1 && !field_0xbf0->anyFlag48()) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x11f8);
        if (actor != NULL && actor->field_0x4 == field_0x3b4) {
            JGeometry::TVec3f dir = actor->mPos - mPos;
            dir.z = 0.0f;
            dir.normalize();
            if (!dir.isZero()) {
                dir.scale(6.0f);
                mPos += dir;
            }
            mPos.z = 0.0f;
            return;
        }
    }
    field_0x124d = 0;
    fn_80251AEC(0);
}

void Player_804A6F38::fn_8025B218() {
    field_0x124d = 1;
    field_0x458 = 0;
}

// FIXME: 95.59. scheduling+regalloc: the original keeps this in r31 and field_0x4 in r30 (ours
// the other way round) and builds the spawn argument from field_0x4 & 3 first.
void Player_804A6F38::fn_8025B22C() {
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 4) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 6) {
        field_0x3fb = 2;
    } else {
        field_0x3fb = 3;
    }
    if (field_0x3fa == 6) {
        if (fn_802415CC() >= 2) {
            s32 index = field_0x4;
            field_0xb18 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BOME, field_0x3b4, &mPos,
                                                     ((field_0x3e8 & 7) << 4) | (index & 3) | 0x100, -1, index);
        } else {
            s32 index = field_0x4;
            field_0xb18 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BOME, field_0x3b4, &mPos,
                                                     ((field_0x3e8 & 7) << 4) | (index & 3), -1, index);
        }
    } else if (field_0x3fa == 16) {
        fn_80251AEC(0);
    }
    field_0x3fa++;
    fn_80268B3C();
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

void Player_804A6F38::fn_8025BDD8() {
    if (field_0x12cc != NULL) {
        field_0x12cc->clearStatus(JPAEMIT_Immortal);
        field_0x12cc->becomeInvalidEmitter();
        field_0x12cc = NULL;
    }
    if (field_0x12e4 != NULL) {
        field_0x12e4->clearStatus(JPAEMIT_Immortal);
        field_0x12e4->becomeInvalidEmitter();
        field_0x12e4 = NULL;
    }
    s32 serial = field_0x12c0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x12c0 = -1;
    }
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025C79C() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025C7BC() {
    if (field_0x12cc != NULL) {
        field_0x12cc->clearStatus(JPAEMIT_Immortal);
        field_0x12cc->becomeInvalidEmitter();
        field_0x12cc = NULL;
    }
    if (field_0x12e4 != NULL) {
        field_0x12e4->clearStatus(JPAEMIT_Immortal);
        field_0x12e4->becomeInvalidEmitter();
        field_0x12e4 = NULL;
    }
    s32 serial = field_0x12c0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x12c0 = -1;
    }
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

// FIXME: 99.87. Instructions identical; only the .sdata2 literals differ (@NNN vs lbl_80540104/8: the pool
// follows first use and this unit's unwritten functions use the earlier literals).
void Player_804A6F38::fn_8025D6AC() {
    if (field_0x3fc < 4) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 8) {
        field_0x3fd = 1;
    } else if (field_0x3fc < 12) {
        field_0x3fd = 2;
    } else {
        field_0x3fd = 3;
    }
    field_0x3fc++;
    if (field_0x3fc == 2) {
        field_0xb9c = false;
    }
    mPos += field_0x344;
    field_0x344.z -= 0.25f;
    if (field_0x344.z < -4.0f) {
        field_0x344.z = -4.0f;
    }
    if (field_0x344.z <= 0.0f && mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        fn_80251AEC(0);
    }
    fn_80268B3C();
    if (fn_8025197C(true)) {
        field_0x344.x = 0.0f;
        field_0x344.y = 0.0f;
    }
}

// FIXME: 96.34. Structurally close: the original reads the table entry through one
// pointer (add, then lfs 0 / 4 / 8); ours indexes x with lfsx (2 more instructions).
// Enter of state 0x41, the recoil hop: speed from lbl_8046AFE0 (back against the facing,
// 4 up), sound 0x20010, a view shake (fn_802D79E8, along y when the hop has a y speed),
// and on the TV the effect of kind 3 aimed back along the hop (fn_802F601C).
void Player_804A6F38::fn_8025D7E4() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.set(lbl_8046AFE0[field_0x3dc].x, lbl_8046AFE0[field_0x3dc].y, lbl_8046AFE0[field_0x3dc].z);
    fn_8024F0E8(0x20010);
    if (field_0x344.y != 0.0f) {
        getGameView_802D6A9C()->fn_802D79E8(field_0x3b4, 4.0f, 0.0f, false);
    } else {
        getGameView_802D6A9C()->fn_802D79E8(field_0x3b4, 0.0f, 4.0f, false);
    }
    lbl_8053AAF8->fn_80134DE4(1, 0);
    JGeometry::TVec3f offset;
    if (field_0x3dc == 0 || field_0x3dc == 1) {
        offset.set(0.0f, -8.0f, 0.0f);
    } else {
        offset.zero();
    }
    JGeometry::TVec3f dir(-field_0x344.x, -field_0x344.y, 0.0f);
    dir.normalize();
    JGeometry::TVec3f pos = mPos + offset;
    if (field_0x3b4 < 8) {
        lbl_8053AB10->field_0x20->fn_802F601C(&pos, 3, &dir, 1.0f, 1.0f);
    }
    field_0xb9c = true;
}

void Player_804A6F38::fn_8025D9FC() {
    if (field_0x1360 != NULL) {
        field_0x1360->becomeInvalidEmitter();
        field_0x1360 = NULL;
    }
    if (field_0x1368 != NULL) {
        field_0x1368->becomeInvalidEmitter();
        field_0x1368 = NULL;
    }
    if (field_0x1364 != NULL) {
        field_0x1364->becomeInvalidEmitter();
        field_0x1364 = NULL;
    }
    if (field_0xbac > 0 || fn_8026FC20() > 0) {
        field_0xce8 = false;
    }
    field_0xbac = 0;
    field_0xba8 = 0;
}

// FIXME: 89.92. Only the unused offset position differs: the original copies the table
// entry to the stack with three lfs / stfs and reloads it with psq_l; ours loads it with
// psq_l straight from the table (3 instructions fewer).
void Player_804A6F38::fn_8025E79C() {
    fn_8027221C(6);
    field_0x458 = 1;
    if (fn_80240808() && fn_802486A0() != 4) {
        s32 facing = fn_80230CFC(fn_80240644());
        field_0x3dc = facing;
        field_0x3e8 = facing;
    } else if (fn_80246B10(2)) {
        field_0x3dc = 0;
        field_0x3e8 = 0;
    } else if (fn_80246B10(1)) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else if (fn_80246B10(8)) {
        field_0x3dc = 3;
        field_0x3e8 = 3;
    } else if (fn_80246B10(4)) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    }
    // The result is unused, as in the original (it computes it and drops it).
    JGeometry::TVec3f pos(lbl_8046AF00[field_0x3dc]);
    pos += mPos;
    field_0xba8 = 0;
    field_0x3fa = 0;
    field_0xbac = 0;
}
