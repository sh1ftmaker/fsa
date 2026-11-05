#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_blizzard_battle.h"
#include "game/cls_804A8638.h"
#include "JSystem/JParticle/JPAEmitter.h"

// The visible box is passed by value: that is what makes the original copy it to
// the stack (lwz/stw) before reading it with lfs.
static inline void setCenter(JGeometry::TVec3f& pos, JGeometry::TBox2f box) {
    pos.x = box.f.x + 0.5f * (box.i.x - box.f.x);
    pos.y = box.f.y + 0.5f * (box.i.y - box.f.y);
    pos.z = 0.0f;
}

inline void cls_8049E7F0::stopEmitter() {
    if (mEmitter != NULL) {
        mEmitter->stopCreateParticle();
    }
}

inline void cls_8049E7F0::playEmitter() {
    if (mEmitter != NULL) {
        mEmitter->playCreateParticle();
    }
}

// The SWEV actor (ACTOR_SWEV, the environment switch control) gets fn_802B39E8.
inline void cls_8049E7F0::notifySwev() {
    s32 serial = fn_801F856C()->fn_801F7A08(ACTOR_SWEV);
    ActorBase_804A5770* swev = fn_801F856C()->findActorBySerial_801F666C(serial);
    if (swev != NULL) {
        static_cast<cls_804A8638*>(swev)->fn_802B39E8();
    }
}

// A sound and a particle burst at the view's centre, from the camera's translation.
inline void cls_8049E7F0::spawnPuff() {
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    pos.x = -camera->field_0x4[0][3];
    pos.y = -camera->field_0x4[1][3];
    startSound_801F0E34(0x30036, 0);
    spawnEmitter_8013CC50(field_0x4, &pos, 0x1bc, 2, 2, NULL, NULL, true, 1.0f);
    field_0x238 = 0;
}

// The phase dispatch: 0 stops the emitter, 1 starts it and notifies SWEV, 2 spawns the puff.
inline void cls_8049E7F0::runPhase() {
    switch (field_0x234) {
    case 0:
        stopEmitter();
        break;
    case 1:
        playEmitter();
        field_0x238 = 0;
        notifySwev();
        break;
    case 2:
        spawnPuff();
        break;
    }
}

// FIXME: 97.18 %. spawnPuff's camera position: the original stores z = 0 a second
// time between x and y (stfs f1, 0x24 after the x store), we store it once in the
// zeroed TVec3f. Its .sdata2 literals (1.0f, 0.5f) also come out in another order.
void cls_8049E7F0::update() {
    ActorBase_804A5770::update();
    switch (field_0x234) {
    case 0:
        if (vfunc_0x3C() == 0) {
            field_0x234 = 1;
            runPhase();
        } else if (vfunc_0x34()) {
            field_0x234 = 1;
            runPhase();
            if (field_0x231 != 0) {
                fn_801F0D20(6);
            }
        }
        break;
    case 1:
        if (vfunc_0x3C() > 0 && !vfunc_0x34()) {
            field_0x234 = 0;
            runPhase();
        }
        startSound_801F0E34(0x30032, 0);
        field_0x238++;
        if (field_0x238 >= (field_0x230 != 0 ? 0x3E8 : 0x12C)) {
            field_0x234 = 2;
            runPhase();
        }
        break;
    case 2:
        startSound_801F0E34(0x30032, 0);
        for (s32 i = 0; i < 4; i++) {
            if (field_0x4 != fn_8023DE58(i) || !fn_8023F264(i) || fn_80230188(i) || fn_8022F478(i) ||
                fn_80236D14(i))
            {
                continue;
            }
            JGeometry::TVec3f* player = fn_8023E724(i);
            JGeometry::TVec2<f32> p(player->x, player->y);
            p.x += 0.1f;
            p.y -= 0.1f;
            fn_80238548(i, 1, &p, 1);
            lbl_80529DEC.field_0x239 |= (u8)(1 << i);
        }
        field_0x238++;
        if (field_0x238 >= 0xAA) {
            field_0x234 = 1;
            runPhase();
        }
        break;
    }
}

// FIXME: 99.41 %. Regalloc only: the centre's x and y go to f0/f2 in the original
// (we use f3/f0), and the 0.5f literal's relocation is named differently.
void cls_8049E7F0::init() {
    ActorBase_804A5770::init();
    field_0x230 = field_0x90 & 1;
    field_0x231 = (field_0x90 >> 1) & 1;
    if (mEmitter == NULL) {
        JGeometry::TVec3f pos;
        setCenter(pos, getGameView_802D6A9C()->mVisibleBox);
        mEmitter = spawnEmitter_8013CC50(field_0x4, &pos, 0x58, 1, 0, NULL, NULL, true, 1.0f);
    }
    if (mEmitter != NULL) {
        mEmitter->stopCreateParticle();
    }
}

cls_8049E7F0::~cls_8049E7F0() {
    fn_801EE434(mEmitter);
    mEmitter = NULL;
}

cls_8049E7F0::cls_8049E7F0() {
    field_0x230 = 0;
    field_0x231 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    mEmitter = NULL;
}
