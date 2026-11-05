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

EffectSystem_8049F178* getEffectSystem_8013C824() {
    return gGameRoot2_8053AB10->field_0x28;
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

// The depth-biased position setEmitterPos_8013CB44 and spawnEmitter_8013CC50 both compute (inlined into each).
static inline void depthPos(JGeometry::TVec3f& p, const JGeometry::TVec3f* pos, s32 height, s32 depth, bool adjust) {
    if (adjust) {
        adjust = pos->z > 0.0f;
    }
    if (adjust && height < 1) {
        height++;
        depth--;
    }
    f32 offset = height * 1024 + depth * 128;
    p.x = pos->x;
    p.y = pos->y + offset;
    p.z = pos->z + offset;
    p.z += gGameRoot2_8053AB10->field_0x20->fn_802F5E9C(pos) - pos->z;
}

// FIXME: 99.93. Code identical; only the int-to-float double's .sdata2 relocation differs: the
// original pool has 0.5f (first used by the unwritten draw passes) before it.
u8 setEmitterPos_8013CB44(JPABaseEmitter* emitter, const JGeometry::TVec3f* pos, s32 height, s32 depth, bool adjust) {
    bool result = false;
    if (emitter != NULL) {
        JGeometry::TVec3f p;
        depthPos(p, pos, height, depth, adjust);
        emitter->setTranslation(p.x, p.y, p.z);
        result = true;
    }
    return result;
}

// The unit's static emitter callback (defined at the end of the file).
extern cls_8049F184 lbl_8053AB18;

// FIXME: 99.86. Code identical; only the .sdata2 relocations of the int-to-float double and
// 0.70710677f differ: the original pool has 0.5f (first used by the unwritten draw passes
// fn_8013D5D4 / D6E4 / DB2C) before the double, as setEmitterPos_8013CB44 shows too.
JPABaseEmitter* spawnEmitter_8013CC50(s32 layer, const JGeometry::TVec3f* pos, u16 id, s32 height, s32 depth,
                            JPAEmitterCallBack* emitterCB, JPAParticleCallBack* particleCB, bool adjust,
                            f32 scale) {
    JPABaseEmitter* emitter = NULL;
    if (scale > 0.0f) {
        if (layer == 0) {
            // The original copies emitterCB into its own register here (mr r28, r8), a separate
            // variable from the parameter the other layers pass on. group is declared before it:
            // declared after, the two swap registers with layer / pos.
            u8 group;
            JPAEmitterCallBack* callback = emitterCB;
            JGeometry::TVec3f p;
            depthPos(p, pos, height, depth, adjust);

            // The draw group (JPAEmitterManager group); 0xFF queues the
            // position for the executeAfter callback instead.
            switch (id) {
            case 0x48E: case 0x48F: case 0x490: case 0x491: case 0x492: case 0x493: case 0x494:
            case 0x495: case 0x558: case 0x559: case 0x55A: case 0x55B: case 0x55C: case 0x55D:
            case 0x55E: case 0x569: case 0x56A: case 0x56B: case 0x56C: case 0x56D: case 0x56E:
            case 0x56F: case 0x570: case 0x571:
                group = 2;
                break;
            case 0x127: case 0x128: case 0x129: case 0x12A: case 0x12B: case 0x188: case 0x189:
            case 0x18A: case 0x18B: case 0x18C: case 0x18D: case 0x2A8: case 0x4E0: case 0x4E1:
                group = 3;
                break;
            case 0x241: case 0x3DD: case 0x4CF: case 0x4D0: case 0x4D1:
                group = 4;
                break;
            case 0x543: case 0x544: case 0x54B:
                group = 5;
                break;
            case 0x26A: case 0x277: case 0x278: case 0x279: case 0x27A: case 0x27B: case 0x27C:
            case 0x27D: case 0x3A7: case 0x3A8: case 0x3A9: case 0x3AA: case 0x3AB: case 0x3AC:
            case 0x3AD: case 0x3AE: case 0x3AF: case 0x3B0: case 0x3B1: case 0x3B2: case 0x3B3:
                group = 7;
                break;
            case 0x566: case 0x567: case 0x568:
                group = 8;
                break;
            case 0x385:
                group = 9;
                break;
            case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
            case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7:
            case 0xE8: case 0xE9: case 0x1DC: case 0x1DD: case 0x1DE: case 0x1DF: case 0x1E0:
            case 0x1E1: case 0x1E2: case 0x1E3: case 0x1E4: case 0x1E5: case 0x1E6: case 0x1E7:
            case 0x1E8: case 0x1E9: case 0x1EA: case 0x1EB: case 0x1EC: case 0x1ED: case 0x1EE:
            case 0x1EF: case 0x1F0: case 0x1F1: case 0x1F2: case 0x1F3: case 0x1F4: case 0x1F5:
            case 0x1F6: case 0x1F7: case 0x1F8: case 0x1F9: case 0x1FA: case 0x1FB: case 0x1FC:
            case 0x3EC: case 0x3ED: case 0x3EE: case 0x3EF: case 0x3F0: case 0x3F1: case 0x3F2:
            case 0x3F3:
                group = 10;
                break;
            case 0x345: case 0x346: case 0x347: case 0x348: case 0x349: case 0x34A: case 0x34B:
            case 0x34C: case 0x34D: case 0x34E: case 0x34F: case 0x350: case 0x351: case 0x352:
            case 0x353: case 0x354: case 0x355: case 0x356: case 0x357: case 0x358: case 0x359:
            case 0x35A: case 0x35B: case 0x35C: case 0x54D:
                group = 0xFF;
                break;
            default:
                group = 0;
                break;
            }

            if (isGameModeSet() && lbl_8053AAF8->field_0x4c != GAME_MODE_C) {
                if (getGameView_802D6A9C()->field_0x131f1) {
                    switch (id) {
                    case 0x3CF:
                        id = 0x454;
                        break;
                    case 0x3D3:
                        id = 0x455;
                        break;
                    case 0x116:
                        id = 0x44E;
                        break;
                    case 0x115:
                        id = 0x44D;
                        break;
                    }
                } else if (getGameView_802D6A9C()->field_0x131f2) {
                    switch (id) {
                    case 0x3D3:
                        id = 0x3A4;
                        break;
                    case 0x3CF:
                        id = 0x3A3;
                        break;
                    case 0x24E:
                        id = 0x24C;
                        break;
                    case 0x24F:
                        id = 0x24D;
                        break;
                    }
                } else if (fn_802DEABC()) {
                    switch (id) {
                    case 0x3D3:
                        id = 0xD6;
                        break;
                    case 0x3CF:
                        id = 0xD5;
                        break;
                    }
                } else if (fn_802DEB58()) {
                    switch (id) {
                    case 0x2A9:
                        id = 0x45E;
                        break;
                    case 0x2AA:
                        id = 0x45F;
                        break;
                    case 0x4F3:
                    case 0x4F7:
                        id = 0x4FB;
                        break;
                    case 0x4F4:
                    case 0x4F8:
                        id = 0x4FC;
                        break;
                    case 0x4F5:
                    case 0x4F9:
                        id = 0x4FD;
                        break;
                    case 0x4F6:
                    case 0x4FA:
                        id = 0x4FE;
                        break;
                    case 0x15B:
                    case 0x15C:
                    case 0x3D0:
                        return NULL;
                    }
                }
                if (!fn_802DE9A0()) {
                    switch (id) {
                    case 0x2A9:
                        return NULL;
                    case 0x2AA:
                        return NULL;
                    case 0x4F7:
                        return NULL;
                    case 0x4F8:
                        return NULL;
                    case 0x4F9:
                        return NULL;
                    case 0x4FA:
                        return NULL;
                    }
                }
            }

            if (group == 0xFF) {
                EffectSystem_8049F178* system = gGameRoot2_8053AB10->field_0x28;
                for (s32 i = 0; i < 0x400; i++) {
                    EffectSystem_8049F178::Entry& entry = system->mEntries[i];
                    if (entry.mCount == 0) {
                        entry.mPos.set(p);
                        entry.mId = id;
                        entry.mCount = 1;
                        break;
                    }
                }
            } else {
                if (gGameRoot2_8053AB10->field_0x28->mResourceManager->getResUserWork(id) & 1) {
                    callback = &lbl_8053AB18;
                }
                if ((gGameRoot2_8053AB10->field_0x28->mResourceManager->getResUserWork(id) & 2) && group != 5) {
                    group = 1;
                }
                emitter = gGameRoot2_8053AB10->field_0x28->mEmitterManager->createSimpleEmitterID(
                    p, id, group, 0, callback, particleCB);
            }

            if (emitter != NULL) {
                f32 yz = 0.70710677f * scale;
                if (group != 2 && group != 4 && group != 5 && group != 8 && group != 10) {
                    Mtx m;
                    m[0][0] = scale;
                    m[0][1] = 0.0f;
                    m[0][2] = 0.0f;
                    m[0][3] = p.x;
                    m[1][0] = 0.0f;
                    m[1][1] = 0.0f;
                    m[1][2] = yz;
                    m[1][3] = p.y;
                    m[2][0] = 0.0f;
                    m[2][1] = yz;
                    m[2][2] = 0.0f;
                    m[2][3] = p.z;
                    emitter->setGlobalSRTMatrix(m);
                    emitter->setGlobalParticleScale(scale, scale);
                } else {
                    emitter->setScale(scale);
                }
            }
        } else {
            // Other layers: helper actors for a few ids.
            switch (id) {
            case 0x172:
                fn_801F856C()->fn_801F9484(ACTOR_KMRI, layer, pos, 0, -1, -1);
                break;
            case 0x33A:
            case 0x33B:
                fn_801F856C()->fn_801F9484(ACTOR_KMRI, layer, pos, 2, -1, -1);
                break;
            case 0x19D:
            case 0x24A:
                fn_801F856C()->fn_801F9484(ACTOR_DMGE, layer, pos, 0, -1, -1);
                break;
            case 0x174: case 0x175: case 0x176: case 0x177: case 0x178: case 0x179: case 0x17C:
            case 0x473:
                fn_801F856C()->fn_801F9484(ACTOR_DMGE, layer, pos, 1, -1, -1);
                break;
            case 0xFF:
                fn_801F856C()->fn_801F9484(ACTOR_DMGE, layer, pos, 0x80, -1, -1);
                break;
            case 0x116:
            case 0x24E: case 0x24F: case 0x250: case 0x251:
            case 0x3D3: case 0x3D4:
                emitter = gGameRoot2_8053AB10->field_0x28->mEmitterManager->createSimpleEmitterID(
                    *pos, id, 6, 0, emitterCB, particleCB);
                break;
            case 0x15B:
            case 0x3D0:
                fn_801F856C()->fn_801F9484(ACTOR_PEFF, layer, pos, 0x2C, -1, 0);
                break;
            case 0x3CF:
                fn_801F856C()->fn_801F9484(ACTOR_PEFF, layer, pos, 0x34, -1, 0);
                break;
            }
        }
    }
    return emitter;
}

void EffectSystem_8049F178::fn_8013D5B0() {
    mEmitterManager->forceDeleteAllEmitter();
}

void EffectSystem_8049F178::fn_8013D908() {
    JPADrawInfo info(gGameRoot2_8053AB10->field_0x24->field_0xe4);
    gGameRoot2_8053AB10->fn_8013B438();
    mEmitterManager->draw(&info, 7);
}

void EffectSystem_8049F178::fn_8013DA10() {
    if (!lbl_8053AAF8->fn_80134CC8()) {
        JPADrawInfo info(gGameRoot2_8053AB10->field_0x24->field_0xe4);
        gGameRoot2_8053AB10->fn_8013B438();
        GXSetAlphaUpdate(GX_FALSE);
        mEmitterManager->draw(&info, 9);
    }
}

void EffectSystem_8049F178::fn_8013DC1C() {
    mEmitterManager->calc(10);
}

void EffectSystem_8049F178::fn_8013DC44() {
    bool update = false;
    if (isGameModeSet() && lbl_8053AAF8->field_0x4c != GAME_MODE_C) {
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
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
// the strings of spawnEmitter_8013CC50 / fn_8013DDEC exist.
EffectSystem_8049F178::EffectSystem_8049F178(JKRHeap* heap) {
    mHeap = JKRSolidHeap::create(-1, heap, false);
    mResourceManager = new (mHeap, 0)
        JPAResourceManager(lbl_8053AAF8->field_0xa4->getResource('????', "particle.jpc"), mHeap);
    OSReport("***** particle.jpc load *****\n");
    mEmitterManager = new (mHeap, 0) JPAEmitterManager(0x1C00, 0x200, mHeap, 0xB, 8);
    mEmitterManager->entryResourceManager(mResourceManager, 0);
    mResourceManager->swapTexture(gGameRoot2_8053AB10->field_0x34->getTexInfo(), "AK_kagerouSwap00");
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
    gGameRoot2_8053AB10->field_0x28->fn_8013C958(emitter);
}

// FIXME: 99.85. Code identical; only the .sdata2 relocations of 0.70710677f and 0.41421357f
// differ (0.5f of the unwritten draw passes is missing before the int-to-float double).
// Projects the particles: scales the manager's camera matrix by 1/sqrt 2 in y and z and moves
// it by the emitter's translation times tan(22.5 deg) in y and z.
void cls_8049F184::draw(JPABaseEmitter* emitter) {
    JPAEmitterWorkData* work = gGameRoot2_8053AB10->field_0x28->mEmitterManager->pWd;
    JGeometry::TSMtxf camera;
    camera.set(work->mPosCamMtx);
    Mtx m;
    PSMTXScale(m, 1.0f, 0.70710677f, 0.70710677f);
    PSMTXConcat(camera, m, camera);
    JGeometry::TVec3f trans;
    trans.set(emitter->mGlobalTrs);
    trans.scale(0.41421357f);
    trans.x = 0.0f;
    PSMTXTrans(m, trans.x, trans.y, trans.z);
    PSMTXConcat(camera, m, camera);
    PSMTXCopy(camera, work->mPosCamMtx);
}

// FIXME: 97.7. Regalloc only: the original keeps gGameRoot2_8053AB10 in r3 and loads the effect
// system into r4 first; here the root is in r4 and the camera comes in r5.
void cls_8049F184::drawAfter(JPABaseEmitter* emitter) {
    GameRoot2_8049F108* root = gGameRoot2_8053AB10;
    root->field_0x24->field_0xe4.getCamMtx(root->field_0x28->mEmitterManager->pWd->mPosCamMtx);
}

cls_8049F184::~cls_8049F184() {}

cls_8049F184 lbl_8053AB18;
