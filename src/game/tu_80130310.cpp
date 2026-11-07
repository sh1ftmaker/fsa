#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/cls_8049E350.h"
#include "game/act/tu_80130310.h"

// FIXME(shotgun): 99.7 %, reloc-name
void cls_8049EA88::init() {
    ActorBase_804A5770::init();
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.i.x = mBox.i.x;
    field_0x80.i.y = mBox.i.y;
    field_0x80.f.x = mBox.f.x;
    field_0x80.f.y = mBox.f.y;
    field_0x23c = (field_0x90 >> 22) & 0x1F;
    field_0x240 = (field_0x90 >> 17) & 0x1F;
}

// FIXME(shotgun): 99.9 %, reloc-name
void cls_8049EA88::vfunc_0x10() {
    s32 serial = getActorManager_801F856C()->fn_801F7884('OPGA', field_0x4);
    while (serial != -1) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            continue;
        }
        if (static_cast<cls_8049E350*>(actor)->field_0x238 == 1) {
            field_0x238 = actor->mSerial;
        } else {
            field_0x234 = actor->mSerial;
        }
        serial = getActorManager_801F856C()->fn_801F78F4('OPGA', serial);
    }
    ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
    mPos = target->mPos;
    mPos.y += 24.0f;
}

// FIXME(shotgun): 94.5 %, scheduling
s32 cls_8049EA88::fn_8013056C(u16 msg, s32 mode, s32 value) {
    s32 serial = field_0x238;
    if (mode == 1) {
        serial = field_0x234;
    }
    fn_8022FE80(0, field_0x4, serial, true, false);
    if (fn_8038BD08(0, msg, field_0x4, mPos, 0x22)) {
        switch (mode) {
        case 0: {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234);
            if (a != NULL) {
                a->field_0x98 = value;
                static_cast<cls_8049E350*>(a)->fn_80125C58(true);
            }
            break;
        }
        case 1: {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
            if (a != NULL) {
                a->field_0x98 = value;
                static_cast<cls_8049E350*>(a)->fn_80125C58(true);
            }
            break;
        }
        case 2: {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
            if (a != NULL) {
                a->field_0x98 = value;
                static_cast<cls_8049E350*>(a)->fn_80125C58(true);
                ActorBase_804A5770* b = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234);
                if (b != NULL) {
                    b->field_0x98 = value;
                }
            }
            break;
        }
        }
        return 0;
    }
    return -1;
}

// FIXME(shotgun): not written (size 0x1FC): fn_80130370
// FIXME(shotgun): not written (size 0x55C): update


cls_8049EA88::~cls_8049EA88() {}
