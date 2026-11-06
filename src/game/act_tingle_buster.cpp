// Actor TINB ("Tingle Buster" in docs/names/actors.tsv): an EnemyBase_804A4808
// with no fields of its own.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/act/act_tingle_buster.h"

TingleBuster_804A5370::TingleBuster_804A5370() {}

void TingleBuster_804A5370::init() {
    EnemyBase_804A4808::init();
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
}

void TingleBuster_804A5370::update() {}

bool fn_801E3250(s32 layer, bool highNibble) {
    s32 serial;
    bool result = false;
    serial = getActorManager_801F856C()->fn_801F7884('TINB', layer);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            u32 kind;
            if (highNibble) {
                kind = (actor->field_0x90 >> 4) & 0xF;
            } else {
                kind = actor->field_0x90 & 0xF;
            }
            if (kind == 0) {
                result = true;
            } else if (gGameRoot_8053AAF8->field_0x7e == 0) {
                if (kind == 2) {
                    result = true;
                }
            } else if (kind == 1) {
                result = true;
            }
        }
    }
    return result;
}

TingleBuster_804A5370::~TingleBuster_804A5370() {}
