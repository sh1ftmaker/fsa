// An actor (vtable 0x804AA6A0, 4CC not identified) that shows a message to
// one player with a PEFF ("Player effect") actor beside it. The spawner
// allocates 0x240 bytes and calls fn_801FF9CC, which stores the vtable.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Message.h"
#include "game/act/tu_802F3B74.h"

cls_804AA6A0::~cls_804AA6A0() {}

void cls_804AA6A0::draw() {}

void cls_804AA6A0::update() {
    if (fn_80388EEC(mPlayer)) {
        vfunc_0x24();
    }
}

void cls_804AA6A0::vfunc_0x24() {
    s32 serial = mEffectSerial;
    if (serial >= 0) {
        ActorBase_804A5770* effect = fn_801F856C()->findActorBySerial_801F666C(serial);
        if (effect != NULL) {
            effect->mAlive = false;
        }
        mEffectSerial = -1;
    }
    mAlive = false;
}

void cls_804AA6A0::init() {
    mMsg = field_0x90 & 0xFFF;
    field_0x234 = (field_0x90 >> 12) & 0xF;
    mPlayer = (field_0x90 >> 16) & 0xF;
    mEffectSerial = -1;
    if (field_0x234 == mPlayer) {
        mEffectSerial = fn_801F856C()->fn_801F9484(ACTOR_PEFF, field_0x4, &mPos,
                                                   (field_0x234 & 3) | 0x24, -1, -1);
        open(field_0x4, mPos, 0x21);
    } else {
        mAlive = false;
    }
}
