#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameMode.h"
#include "game/Player.h"
#include "game/cls_8049F140.h"
#include "game/RotMatrix.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTTexture.h"

// The ten file names fn_8013DDEC indexes (five claw textures with their dummy
// generators, 0x8049F118 in .data).
const char* lbl_8049F118[10] = {
    "jen_blue_tsume.bti",   "AK_dummyGenB00",  "jen_green_tsume.bti", "AK_dummyGenG00",
    "jen_red_tsume.bti",    "AK_dummyGenR00",  "jen_purple_tsume.bti", "AK_dummyGenP00",
    "jen_8_3_tsume.bti",    "AK_dummyGenS00",
};

// Vtable 0x8049F15C: emits the queued positions of EffectSystem_8049F178.
class cls_8049F15C : public JPAEmitterCallBack {
public:
    virtual ~cls_8049F15C();
    virtual void executeAfter(JPABaseEmitter* emitter);
};

// Vtable 0x8049F184: projects the emitter's matrix; lbl_8053AB18 is one.
class cls_8049F184 : public JPAEmitterCallBack {
public:
    virtual ~cls_8049F184();
    virtual void draw(JPABaseEmitter* emitter);
    virtual void drawAfter(JPABaseEmitter* emitter);
};

// FIXME: 98.7. The original tests field_0x4c == 0xC as bne/b (a branch to the end after the
// bne) where this emits one beq, and keeps the 'PEFF' arguments of case 0 in r29-r31 (here r28-r30).
void cls_8049F140::execute(JPABaseEmitter* emitter) {
    if (!emitter->checkStatus(1)) {
        GameRoot_8049EF20* root = lbl_8053AAF8;
        if (isGameModeSet() && root->field_0x4c != GAME_MODE_C && !root->field_0x94) {
            field_0xc++;
            switch (field_0x8) {
            case 0:
                if (field_0xc >= 8) {
                    field_0xc = 0;
                    fn_801F856C()->fn_801F9484(ACTOR_PEFF, fn_8023DE58(field_0x4), fn_8023E724(field_0x4),
                                               field_0x4 & 3, -1, field_0x4);
                }
                break;
            case 1:
                if (field_0xc >= 8) {
                    field_0xc = 0;
                    fn_801F856C()->fn_801F9484(ACTOR_PEFF, fn_8023DE58(field_0x4), fn_8023E724(field_0x4),
                                               (field_0x4 & 3) | 0x30, -1, field_0x4);
                }
                break;
            }
        }
    }
}

EffectSystem_8049F178* fn_8013C824() {
    return lbl_8053AB10->field_0x28;
}

void EffectSystem_8049F178::fn_8013C830(s32 player, JPABaseEmitter* emitter) {
    if (emitter != NULL && player >= 0 && player < 4) {
        emitter->setColorRGB(mColors[player][0], mColors[player][1], mColors[player][2]);
    }
}

cls_8049F15C::~cls_8049F15C() {}

void EffectSystem_8049F178::fn_8013C8CC() {
    for (s32 i = 0; i < mEmitterNum; i++) {
        if (mEmitterIds[i] == 0x349 || mEmitterIds[i] == 0x34A || mEmitterIds[i] == 0x34B) {
            mEmitters[i]->deleteAllParticle();
        }
    }
}

void EffectSystem_8049F178::fn_8013C958(JPABaseEmitter* emitter) {
    s32 count = emitter->getCurrentCreateNumber();
    u16 id;
    s32 word;
    for (s32 i = 0; i < mEmitterNum; i++) {
        if (emitter == mEmitters[i]) {
            id = mEmitterIds[i];
            word = field_0x40a8[i];
            break;
        }
    }
    if (count > 0 || word != 0 || id == 0x54D) {
        emitter->playCreateParticle();
        for (s32 i = 0; i < 0x400; i++) {
            if (mEntries[i].mId == id && mEntries[i].mCount != 0) {
                if (count > 0) {
                    emitter->setGlobalTranslation(mEntries[i].mPos);
                    for (s32 j = 0; j < count; j++) {
                        JPABaseParticle* particle = emitter->createParticle();
                        if (particle != NULL) {
                            particle->setOffsetPosition(mEntries[i].mPos);
                        }
                    }
                }
                mEntries[i].mCount--;
            }
        }
        emitter->stopCreateParticle();
    }
}

u8 fn_8013CAA4(JPABaseEmitter* emitter, f32 angle) {
    bool result = false;
    if (emitter != NULL) {
        Mtx m;
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        JGeometry::TVec3f axis(0.0f, 1.0f, 1.0f);
        fn_8010F668(m, &axis, angle + JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        m[0][0] *= -1.0f;
        m[0][1] *= -1.0f;
        m[0][2] *= -1.0f;
        JPASetRMtxfromMtx(m, emitter->mGlobalRot);
        result = true;
    }
    return result;
}

u8 fn_8013CB44(JPABaseEmitter* emitter, const JGeometry::TVec3f* pos, s32 height, s32 depth, bool adjust) {
    bool result = false;
    if (emitter != NULL) {
        if (adjust) {
            adjust = pos->z > 0.0f;
        }
        if (adjust && height < 1) {
            height++;
            depth--;
        }
        f32 offset = height * 1024 + depth * 128;
        f32 x = pos->x;
        f32 y = pos->y + offset;
        f32 z = pos->z + offset;
        z += lbl_8053AB10->field_0x20->fn_802F5E9C(pos) - pos->z;
        emitter->setTranslation(x, y, z);
        result = true;
    }
    return result;
}

void EffectSystem_8049F178::fn_8013D5B0() {
    mEmitterManager->forceDeleteAllEmitter();
}

void EffectSystem_8049F178::fn_8013DC1C() {
    mEmitterManager->calc(10);
}

void EffectSystem_8049F178::fn_8013DC44() {
    bool update = false;
    if (isGameModeSet() && lbl_8053AAF8->field_0x4c != GAME_MODE_C) {
        if (!fn_802D6A9C()->fn_802DCD0C()) {
            update = true;
        }
    } else {
        update = true;
    }
    if (update) {
        if (lbl_8053AAF8->field_0x4c != GAME_MODE_B) {
            mEmitterManager->calc(0);
            mEmitterManager->calc(1);
            mEmitterManager->calc(2);
            mEmitterManager->calc(4);
            mEmitterManager->calc(5);
            mEmitterManager->calc(6);
        }
        mEmitterManager->calc(7);
        mEmitterManager->calc(8);
    }
    mEmitterManager->calc(3);
    mEmitterManager->calc(9);
}

EffectSystem_8049F178::~EffectSystem_8049F178() {
    mEmitterManager->clearResourceManager(0);
    mHeap->destroy();
}

// FIXME: 99.9. Code identical; only the .rodata string and .sdata2 relocations differ until
// the strings of fn_8013CC50 / fn_8013DDEC exist.
EffectSystem_8049F178::EffectSystem_8049F178(JKRHeap* heap) {
    mHeap = JKRSolidHeap::create(-1, heap, false);
    mResourceManager = new (mHeap, 0)
        JPAResourceManager(lbl_8053AAF8->field_0xa4->getResource('????', "particle.jpc"), mHeap);
    OSReport("***** particle.jpc load *****\n");
    mEmitterManager = new (mHeap, 0) JPAEmitterManager(0x1C00, 0x200, mHeap, 0xB, 8);
    mEmitterManager->entryResourceManager(mResourceManager, 0);
    mResourceManager->swapTexture(lbl_8053AB10->field_0x34->getTexInfo(), "AK_kagerouSwap00");
    mHeap->adjustSize();
    mColors[0][0] = 0xC8;
    mColors[0][1] = 0xFA;
    mColors[0][2] = 0xAC;
    mColors[1][0] = 0xFA;
    mColors[1][1] = 0xAE;
    mColors[1][2] = 0xAC;
    mColors[2][0] = 0xB4;
    mColors[2][1] = 0xD2;
    mColors[2][2] = 0xE6;
    mColors[3][0] = 0xF0;
    mColors[3][1] = 0xB4;
    mColors[3][2] = 0xF0;
}

void cls_8049F15C::executeAfter(JPABaseEmitter* emitter) {
    lbl_8053AB10->field_0x28->fn_8013C958(emitter);
}

// FIXME: 97.7. Regalloc only: the original loads lbl_8053AB10 into r3 and the camera
// last (r4 = effect system first); here the root is in r4 and the camera comes in r5.
void cls_8049F184::drawAfter(JPABaseEmitter* emitter) {
    GameRoot2_8049F108* root = lbl_8053AB10;
    JPAEmitterManager* manager = root->field_0x28->mEmitterManager;
    PSMTXCopy(root->field_0x24->field_0xe4, manager->pWd->mPosCamMtx);
}

cls_8049F184::~cls_8049F184() {}

cls_8049F184 lbl_8053AB18;
