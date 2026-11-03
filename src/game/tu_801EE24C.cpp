// The actor base class (include/game/ActorBase.h) and the code around it.

#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Lets an emitter that is still on screen finish its particles; deletes one
// that is off screen at once.
static inline void releaseEmitter(JPABaseEmitter* emitter) {
    GameView* view = fn_802D6A9C();
    f32 x = emitter->mGlobalTrs.x;
    f32 y = emitter->mGlobalTrs.y - emitter->mGlobalTrs.z;
    bool onScreen = false;
    if (x >= view->mVisibleMinX && y >= view->mVisibleMinY && x < view->mVisibleMaxX &&
        y < view->mVisibleMaxY)
    {
        onScreen = true;
    }
    if (onScreen) {
        emitter->clearStatus(JPAEMIT_Immortal);
        emitter->becomeInvalidEmitter();
    } else {
        fn_8013C824()->mEmitterManager->forceDeleteEmitter(emitter);
    }
}

void ActorBase::draw() {}

void ActorBase::savePrevPos() {
    mPrevPos = mPos;
}

void ActorBase::update() {}

void ActorBase::vfunc_0x10() {}

void ActorBase::init() {}

ActorBase::~ActorBase() {
    if (mEmitter100 != NULL) {
        releaseEmitter(mEmitter100);
    }
    if (mEmitter104 != NULL) {
        releaseEmitter(mEmitter104);
    }
    fn_801F8448(fn_801F856C(), this);
}

ActorBase::ActorBase()
    : field_0x4(0),
      field_0x8(0),
      mPos(0.0f, 0.0f, 0.0f),
      mPrevPos(0.0f, 0.0f, 0.0f),
      mSpawnPos(0.0f, 0.0f, 0.0f),
      field_0x30(0.0f, 0.0f, 0.0f),
      field_0x3c(0.0f, 0.0f, 0.0f),
      field_0x48(1.0f, 1.0f, 1.0f),
      field_0x54(1.0f, 1.0f, 1.0f),
      mBox(-8.0f, -8.0f, 8.0f, 8.0f),
      mBox2(0.0f, 0.0f, 0.0f, 0.0f),
      field_0x80(0.0f),
      field_0x84(0.0f),
      field_0x88(0.0f),
      field_0x8c(0.0f),
      field_0x90(0),
      field_0x94(0),
      field_0x98(0),
      field_0x9c(-1),
      field_0xa0(-1),
      field_0xa4(0),
      field_0xa8(0),
      field_0xac(0),
      field_0xb0(0),
      field_0xb4(0),
      field_0xb8(0),
      field_0xbc(1),
      field_0xc0(1),
      field_0xc4(0),
      field_0xc8(-1),
      field_0xcc(0),
      field_0xd0(-1),
      field_0xd4(0),
      field_0xd5(0),
      field_0xd8(0),
      field_0xdc(0x18),
      field_0xe0(0),
      field_0xe4(0),
      field_0xe8(0),
      field_0xec(0.0f, 0.0f, 0.0f),
      field_0xf8(0),
      field_0xfc(0),
      mEmitter100(NULL),
      mEmitter104(NULL),
      mLife(4),
      field_0x10c(-1),
      field_0x110(-1),
      field_0x114(0),
      field_0x118(0),
      mAlive(true),
      mDrawEnabled(true),
      field_0x11e(0),
      field_0x11f(0),
      field_0x120(),
      field_0x138(0),
      field_0x13c(0),
      field_0x13d(0),
      field_0x13e(0),
      field_0x13f(1),
      field_0x140(1),
      field_0x141(0),
      field_0x142(0),
      field_0x143(0),
      field_0x144(0),
      field_0x145(0),
      field_0x146(1),
      field_0x148(3),
      field_0x14c(1),
      field_0x14d(0),
      field_0x14e(0),
      field_0x150(-1),
      field_0x154(0),
      field_0x158(2),
      mListPrev(NULL),
      mListNext(NULL),
      field_0x164(0),
      mInList(0),
      field_0x168(0),
      field_0x16c(-1),
      field_0x174(0),
      field_0x178(0),
      field_0x17c(0),
      field_0x17d(0),
      field_0x180(20),
      field_0x184(40.0f),
      field_0x188(-1),
      field_0x18c(0),
      field_0x190(0),
      field_0x194(0),
      field_0x195(0),
      field_0x196(0),
      mSerial(-1),
      mSlot(0),
      mActorId('NULL'),
      field_0x1a4(0),
      field_0x1a8(false)
{
    for (int i = 0; i < 4; i++) {
        field_0x1ac[i].set(0.0f, 0.0f, 0.0f);
    }
}
