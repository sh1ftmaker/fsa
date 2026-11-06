#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/Effect.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/act/act_navi_maiden_crystal.h"

using JGeometry::TVec3f;

// FIXME: 99.64 %, regalloc-only: loop registers rotated (target r30/r31 vs ours r31/r30).
void Navi54MaidenCrystalCutscene_804ADFE8::init() {
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_DCR2) {
            field_0x250 = i;
            field_0x254 = static_cast<cls_804AE088*>(actor);
            field_0x258 = actor->mPos;
            break;
        }
    }

    mState = 0;
    field_0x23c = 0.0f;
    field_0x240 = 0;
    field_0x264 = 0x1ee;
    mEmitter270 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32c, 0, -1, NULL, NULL, true, 1.0f);
    mEmitter274 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, -1, NULL, NULL, true, 1.0f);
    if (mEmitter270 != NULL) {
        mEmitter270->setGlobalEnvColor(0x1e, 0xa0, 0xbe);
    }
    field_0x26c = 0;
    field_0x268 = 1;
    field_0x230 = 0;
}

// FIXME: 98.55 %, scheduling+regalloc: 5 opcodes differ in argument load order before fn_8038BD08 and register order in case 6.
void Navi54MaidenCrystalCutscene_804ADFE8::update() {
    TVec3f pos;
    TVec3f offset;

    if (mEmitter270 != NULL) {
        setEmitterPos_8013CB44(mEmitter270, &mPos, 0, -1, true);
    }
    if (mEmitter274 != NULL) {
        setEmitterPos_8013CB44(mEmitter274, &mPos, 0, -1, true);
    }

    switch (mState) {
    case 0:
        if (mPos.y > field_0x258.y + 72.0f) {
            mPos.y -= 2.0f;
        } else {
            mPos.y = field_0x258.y + 72.0f;
            field_0x244.x = mPos.x;
            field_0x244.y = mPos.y;
            field_0x244.z = mPos.z;
            mState++;
            field_0x240 = 0;
        }
        break;

    case 1:
        if (field_0x230 == 0) {
            if (field_0x240 >= 0) {
                mPos.y -= 1.4f;
                if (mPos.y <= field_0x244.y - 10.0f) {
                    field_0x240 = -0x8000;
                }
            } else {
                mPos.y += 1.4f;
                if (mPos.y >= field_0x244.y + 10.0f) {
                    field_0x240 = 0;
                }
            }
        } else {
            if (mPos.y > field_0x244.y) {
                mPos.y -= 1.4f;
            } else if (mPos.y < field_0x244.y) {
                mPos.y += 1.4f;
            }
        }
        break;

    case 2: {
        field_0x240 += 0x200;
        mPos = field_0x244 + TVec3f(40.0f * JMath::sincosTable_.sinShort(field_0x240),
                                    12.0f * JMath::sincosTable_.sinShort(field_0x240 * 2),
                                    0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            mState++;
        }
        break;
    }

    case 3:
        // The asm compares the halfword unsigned (clrlwi 16, cmplwi 0x8000) after the extsh. zero test.
        if (field_0x240 != 0 && (u16)field_0x240 != 0x8000) {
            field_0x240 += 0x200;
            mPos = field_0x244 + TVec3f(40.0f * JMath::sincosTable_.sinShort(field_0x240),
                                        12.0f * JMath::sincosTable_.sinShort(field_0x240 * 2),
                                        0.0f);
        } else {
            GXRenderModeObj* renderMode = JUTVideo::getManager()->getRenderMode();
            pos.z = 0.0f;
            pos.y = (s32)renderMode->efbHeight * 0.5f;
            pos.x = (s32)renderMode->fbWidth * 0.5f;
            pos.y -= 16.0f;
            fn_8022FE80(0, field_0x4, mSerial, true, false);
            fn_8038BD08(0, 0x83a, field_0x4, pos, 0x62);
            mState++;
        }
        break;

    case 4:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            field_0x254->vfunc_0x60(10);
            mState++;
        }
        break;

    case 5:
        break;

    case 6: {
        field_0x23c += 0.5f;
        field_0x240 += 0x800;
        if (field_0x23c < 15.0f) {
            offset.set(field_0x23c * JMath::sincosTable_.cosShort(field_0x240),
                       field_0x23c * JMath::sincosTable_.sinShort(field_0x240),
                       0.0f);
        } else {
            mPos.z -= 0.8f;
            f32 radius = __fabsf(30.0f - field_0x23c);
            offset.set(JMath::sincosTable_.cosShort(field_0x240) * radius,
                       JMath::sincosTable_.sinShort(field_0x240) * radius,
                       0.0f);
            if (field_0x23c >= 30.0f) {
                cls_804AE088* dcr2 = field_0x254;
                dcr2->vfunc_0x60(12);
                fn_803676CC();
                dcr2->mNaviActor = NULL;
                fn_801F3668();
            }
        }
        mPos = field_0x244 + offset;
        break;
    }
    }
}

void Navi54MaidenCrystalCutscene_804ADFE8::draw() {
    SpriteParams_801F5AE0 params;
    if (field_0x268 != 0) {
        field_0x268--;
    }
    if (field_0x268 == 0) {
        field_0x268 = 1;
        field_0x26c++;
        if (field_0x26c >= 6) {
            field_0x26c = 0;
        }
    }
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + field_0x26c, &params, 7, 1.0f, 1.0f);
}

void Navi54MaidenCrystalCutscene_804ADFE8::fn_803676CC() {
    if (mEmitter270 != NULL) {
        mEmitter270->setStatus(JPAEMIT_ForceDelete);
        mEmitter270 = NULL;
    }
    if (mEmitter274 != NULL) {
        mEmitter274->setStatus(JPAEMIT_ForceDelete);
        mEmitter274 = NULL;
    }
}

Navi54MaidenCrystalCutscene_804ADFE8::~Navi54MaidenCrystalCutscene_804ADFE8() {}
