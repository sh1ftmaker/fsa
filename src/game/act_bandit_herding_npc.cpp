#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/act/act_bandit_herding_npc.h"

BanditHerdingNPC_804ACB40::~BanditHerdingNPC_804ACB40() {}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    bool result = false;
    if (p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y) {
        result = true;
    }
    return result;
}

void BanditHerdingNPC_804ACB40::draw() {}

void BanditHerdingNPC_804ACB40::update() {
    field_0x238 = 0;
    s32 serial = -1;
    for (;;) {
        serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_TZOK, serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* thief = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (thief == NULL) {
            continue;
        }
        if (field_0x4 == thief->field_0x4 && (thief->mPos.z < 8.0f || thief->field_0xd8 > 0) &&
            thief->field_0xc4 == 0) {
            if (contains(field_0x240, thief->mPos) == true && !getGameView_802D6A9C()->fn_802D800C(field_0x23c)) {
                thief->field_0x94 = 1;
                field_0x238++;
                continue;
            }
        }
        thief->field_0x94 = 0;
    }
    lbl_8053AD78->field_0x10c = field_0x234 - field_0x238;
    if (field_0x238 > 0) {
        gGameRoot2_8053AB10->field_0x24->fn_8030C058(&field_0x240);
    }
    // FIXME: 94%. The original materialises (field_0x238 >= field_0x234) as a bool
    // (srawi/srwi/subfc/adde, then cmplwi 1) and keeps the 1 stored to the flag in r0
    // rather than r30; the box test also loads x into f2 and y into f1 (mine: f1, f2).
    if (field_0x238 >= field_0x234 && !vfunc_0x34()) {
        vfunc_0x30(0xFF, true);
        fn_80204638()->field_0x0 = 1;
    }
    s32 bit = field_0x23c;
    if (bit > 0 && getGameView_802D6A9C()->fn_802D800C(bit) == true && vfunc_0x34() == true) {
        vfunc_0x30(0xFF, false);
    }
}

void BanditHerdingNPC_804ACB40::init() {
    field_0x234 = field_0x90 & 0xFF;
    field_0x23c = (field_0x90 >> 22) & 0x1F;
    field_0x238 = 0;
    mPos.x = 16.0f * ((s32)mPos.x / 16);
    mPos.y = 16.0f * ((s32)mPos.y / 16);
    mBox.set(0.0f, 0.0f, ((field_0x90 >> 8) & 0x3F) * 8, ((field_0x90 >> 16) & 0x3F) * 8);
    field_0x240 = mBox;
    field_0x240.i.x += mPos.x;
    field_0x240.i.y += mPos.y;
    field_0x240.f.x += mPos.x;
    field_0x240.f.y += mPos.y;
}
