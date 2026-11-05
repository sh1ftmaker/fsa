// Actor FBGE ("Fireball generator" in docs/names/actors.tsv): an
// EnemyBase_804A4808 that fires at the nearest player every 0x78 frames.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_fireball_generator.h"

FireballGenerator_804A6660::~FireballGenerator_804A6660() {}

void FireballGenerator_804A6660::vfunc_0x90(u8 mapHit) {
    if (vfunc_0x34()) {
        fn_801F3668();
    } else {
        switch (mState) {
        case 0:
            if (mCooldown > 1) {
                mCooldown--;
            } else if (fn_801CECF4('FBAL', 1.5f, 112.0f)) {
                startSound_801F0E34(0x50030, 0);
                mCooldown = 0x78;
            }
            break;
        }
    }
}

void FireballGenerator_804A6660::init() {
    EnemyBase_804A4808::init();
    if (gGameRoot_8053AAF8->field_0x7e != 0) {
        if ((field_0x90 >> 1) & 1) {
            mAlive = false;
            return;
        }
    } else if (field_0x90 & 1) {
        mAlive = false;
        return;
    }
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    field_0x250 = -1;
    mStateTimer = 0;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    if (vfunc_0x34()) {
        fn_801F3668();
    }
}

FireballGenerator_804A6660::FireballGenerator_804A6660() {
    mCooldown = 0x78;
}
