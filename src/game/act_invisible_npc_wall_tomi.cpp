#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_invisible_npc_wall_tomi.h"
#include "game/act/act_small_key.h"

void InvisibleNPCWall_804AE6D0::init() {
    field_0x230 = field_0x90 >> 27;
    field_0x234 = (field_0x90 >> 22) & 0x1F;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    f32 w = ((field_0x90 & 0xFF) + 1) * 8;
    f32 h = (((field_0x90 >> 8) & 0xFF) + 1) * 8;
    if (field_0x234 == 1) {
        h = 4.0f;
    }
    if ((field_0x90 >> 16) & 1) {
        mPos.x += w;
        mPos.y += h;
    } else {
        mPos.x = w + (s32)mPos.x / 16 * 16;
        mPos.y = h + (s32)mPos.y / 16 * 16;
    }
    field_0x80.set(-w, -h, w, h);
    if (field_0x234 == 1) {
        mBox.set(-w, -h, w, h);
    }
}

void InvisibleNPCWall_804AE6D0::update() {
    switch (field_0x234) {
    case 0:
        if (field_0x230 == 0 || vfunc_0x34()) {
            fn_8020D58C()->addBox(mSerial);
        }
        break;
    case 1: {
        s32 serial = fn_801F8108(this, ACTOR_KEY0, -1, 32.0f);
        if (serial >= 0) {
            SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (key != 0 && key->field_0x240 == 4 && key->field_0x3c.y < 0.0f) {
                key->fn_802C2BD0();
            }
        }
        break;
    }
    }
}

InvisibleNPCWall_804AE6D0::~InvisibleNPCWall_804AE6D0() {}
