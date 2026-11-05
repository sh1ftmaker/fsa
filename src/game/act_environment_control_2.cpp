// Actor SEV2 ("Environment control 2"): decodes a set of flags from its
// spawn parameter and applies them every frame to the shared objects
// returned by fn_801902C0 and fn_8016A3AC.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameRootSub20.h"
#include "game/act/act_environment_control_2.h"

void EnvironmentControl2_804A9CD0::fn_802E43E0() {
    field_0x280 = 0;
}

void EnvironmentControl2_804A9CD0::draw() {}

void EnvironmentControl2_804A9CD0::update() {
    JGeometry::TVec3f pos = mPos;
    if (field_0x90 & 8) {
        pos.x += 512.0f;
    }
    if (field_0x90 & 4) {
        pos.x -= 512.0f;
    }
    if (field_0x90 & 2) {
        pos.y += 384.0f;
    }
    if (field_0x90 & 1) {
        pos.y -= 384.0f;
    }

    if (field_0x234 != 0) {
        GameRootSub20_801902C0::SmallRequest& req = fn_801902C0()->mSmallRequests[0];
        if (req.field_0xc != NULL && req.mActive == 0) {
            req.mActive = 1;
            req.mPos.x = pos.x;
            req.mPos.y = pos.y;
            req.mPos.z = pos.z;
        }
    }
    if (field_0x238 != 0) {
        GameRootSub20_801902C0::SmallRequest& req = fn_801902C0()->mSmallRequests[1];
        if (req.field_0xc != NULL && req.mActive == 0) {
            req.mActive = 1;
            req.mPos.x = pos.x;
            req.mPos.y = pos.y;
            req.mPos.z = pos.z;
        }
    }
    if (field_0x23c != 0) {
        GameRootSub20_801902C0* shared = fn_801902C0();
        if (shared->mQueuedPosCount < 2) {
            GameRootSub20_801902C0::QueuedPos& slot = shared->mQueuedPos[shared->mQueuedPosCount];
            slot.mPos.x = pos.x;
            slot.mPos.y = pos.y;
            slot.mPos.z = pos.z;
            shared->mQueuedPosCount++;
        }
    }

    if (field_0x240 == 0) {
        if (field_0x284 != 0) {
            field_0x284--;
        }
    } else if (field_0x284 < field_0x244) {
        field_0x284++;
    }
    fn_8016A3AC()->field_0x1f2 = field_0x284;

    if (field_0x248 != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1d1 = 1;
    }
    if (field_0x24c != 0) {
        if (lbl_8053AAF8->field_0x4c != 11) {
            mSound->startSound(0x50078);
        }
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1d2 = 1;
    }
    if (field_0x250 != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1d3 = 1;
    }

    if (field_0x254 == 0) {
        if (field_0x285 != 0) {
            field_0x285--;
        }
    } else if (field_0x285 < field_0x258) {
        field_0x285++;
    }
    fn_8016A3AC()->field_0x1f3 = field_0x285;

    if (field_0x25c != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        if (shared->field_0x1d8 < 0) {
            shared->field_0x1d8 = 0x100;
        }
    }
    if (field_0x260 != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1dc = 1;
    }
    if (field_0x264 != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1ca = 1;
    }

    if (field_0x268 == 0) {
        field_0x286 = 0;
    } else if (field_0x286 < field_0x26c) {
        field_0x286++;
    }
    fn_8016A3AC()->field_0x1f4 = field_0x286;

    if (field_0x270 == 0) {
        if (field_0x287 != 0) {
            field_0x287--;
        }
    } else if (field_0x287 < field_0x274) {
        field_0x287++;
    }
    fn_8016A3AC()->field_0x1f5 = field_0x287;

    if (field_0x278 != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1cf = 1;
        shared->field_0x1d0 = 0;
    }
    if (field_0x27c != 0) {
        GameRootSub24_8016A3AC* shared = fn_8016A3AC();
        shared->field_0x1dd = 1;
    }

    if (field_0x280 == 0) {
        if (field_0x288 != 0) {
            field_0x288--;
        }
    }
    fn_8016A3AC()->field_0x1ef = field_0x288;
}

EnvironmentControl2_804A9CD0::~EnvironmentControl2_804A9CD0() {
    if (mSound != NULL) {
        delete mSound;
    }
}

void EnvironmentControl2_804A9CD0::init() {
    field_0x234 = (field_0x90 >> 26) & 1;
    field_0x238 = (field_0x90 >> 25) & 1;
    field_0x23c = (field_0x90 >> 24) & 1;
    field_0x240 = (field_0x90 >> 23) & 1;
    field_0x248 = (field_0x90 >> 22) & 1;
    field_0x24c = (field_0x90 >> 21) & 1;
    field_0x250 = (field_0x90 >> 20) & 1;
    field_0x254 = (field_0x90 >> 19) & 1;
    field_0x25c = (field_0x90 >> 18) & 1;
    field_0x260 = (field_0x90 >> 17) & 1;
    field_0x264 = (field_0x90 >> 16) & 1;
    field_0x268 = (field_0x90 >> 15) & 1;
    field_0x270 = (field_0x90 >> 14) & 1;
    field_0x278 = (field_0x90 >> 13) & 1;
    field_0x27c = (field_0x90 >> 12) & 1;
    field_0x280 = (field_0x90 >> 11) & 1;

    field_0x284 = fn_8016A3AC()->field_0x1f2;
    if (field_0x240 != 0) {
        field_0x244 = 0xFF;
    }
    fn_8016A3AC()->field_0x1f2 = field_0x284;

    field_0x285 = fn_8016A3AC()->field_0x1f3;
    if (field_0x254 != 0) {
        field_0x258 = 0xFF;
    }
    fn_8016A3AC()->field_0x1f3 = field_0x285;

    field_0x286 = fn_8016A3AC()->field_0x1f4;
    if (field_0x268 != 0) {
        field_0x26c = 0xFF;
    }
    fn_8016A3AC()->field_0x1f4 = field_0x286;

    field_0x287 = fn_8016A3AC()->field_0x1f5;
    if (field_0x270 != 0) {
        field_0x274 = 0xFF;
    }
    fn_8016A3AC()->field_0x1f5 = field_0x287;

    field_0x288 = fn_8016A3AC()->field_0x1ef;
    if (field_0x280 != 0) {
        field_0x288 = 0xFF;
    }
    fn_8016A3AC()->field_0x1ef = field_0x288;

    mSound = new ActorSoundObj_804578F4(NULL, NULL, 8);
    update();
}
