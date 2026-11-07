#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/GameState.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/ActorBase.h"
#include "game/Player.h"
#include "game/act/tu_803DA6B4.h"
#include "dolphin/os.h"
#include "JSystem/J2D/J2DAnmLoader.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/JParticle/JPAEmitter.h"

void fn_802B7CD8(ActorBase_804A5770* actor);

// FIXME(shotgun): 60.8 %, missing/extra + reloc-name
// 0x80439C78
cls_804B17A0::cls_804B17A0() {
    lbl_8053AEC8 = this;
    field_0x4 = new J2DScreen;
    field_0x4->setPriority("gameover.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    OSInitFastCast();
    field_0x8 = J2DAnmLoaderDataBase::load(fn_80403084('SCRN', "gameover.bck"));
    field_0x4->search('Ndata01')->setAnimation(static_cast<J2DAnmTransform*>(field_0x8));
    field_0x3c = 0;
    for (s32 i = 0; i < 10; i++) {
        field_0xc[i] = NULL;
    }
    field_0x34 = new J2DPicture(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_black.bti")));
    field_0x34->setCornerColor(JUtility::TColor(0, 0, 0, 0xFF));
    field_0x4c = 255.0f;
    field_0x50 = 0.0f;
    field_0x54 = 0;
    field_0x56 = 0;
    field_0x40 = 0;
    field_0x42 = 0;
    field_0x4->search('Pblk')->hide();
    field_0x41 = field_0x4->search('Pblk')->getAlpha();
    field_0x5b = 0;
    field_0x44 = -1;
    field_0x48 = 0;
    field_0x58 = 0;
    field_0x59 = 0xFF;
    field_0x5a = 0xFF;
    field_0x38 = 0;
    field_0x5c = 0;
    field_0x5d = 0;
    field_0x5e = 0;
    field_0x5f = 0;
}

// FIXME(shotgun): 95.2 %, different
// 0x80439B28
cls_804B17A0::~cls_804B17A0() {
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0x34;
    field_0x34 = NULL;
    for (s32 i = 0; i < 8; i++) {
        if (field_0xc[i] != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0xc[i]);
            field_0xc[i] = NULL;
        }
    }
    if (field_0xc[8] != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0xc[8]);
        field_0xc[8] = NULL;
    }
    if (field_0xc[9] != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0xc[9]);
        field_0xc[9] = NULL;
    }
    lbl_8053AEC8 = NULL;
}

// FIXME(shotgun): 86.1 %, missing/extra
// 0x80439AB0
void cls_804B17A0::fn_80439AB0() {
    field_0x40 = 0;
    field_0x42 = 0;
    field_0x5d = 0;
    field_0x44 = -1;
    field_0x48 = 0;
    field_0x58 = 0;
    field_0x59 = 0xFF;
    field_0x5a = 0xFF;
    field_0x4c = 255.0f;
    field_0x50 = 0.0f;
    field_0x54 = 0;
    field_0x56 = 0;
    field_0x3c = 0;
    for (s32 i = 0; i < 10; i++) {
        field_0xc[i] = NULL;
    }
    field_0x5f = 0;
}

// FIXME(shotgun): 99.7 %, reloc-name
// 0x804399A0
void cls_804B17A0::fn_804399A0() {
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x34->setAlpha((u8)field_0x50);
    field_0x34->draw(0.0f, 0.0f, 640.0f, 480.0f, false, false, false);
    field_0x8->mCurrentFrame = field_0x3c;
    field_0x4->search('Ndata01')->animationTransform();
    field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x40 == 3) {
        lbl_8053ADC8->field_0x20 = 1;
        lbl_8053ADC8->field_0x21 = 0;
    }
}

// FIXME(shotgun): 89.3 %, missing/extra
// 0x80438C68
void cls_804B17A0::fn_80438C68() {
    switch (field_0x58) {
    case 0:
        if (field_0x44 == -1) {
            s32 arg = field_0x48;
            fn_8022EA0C(0);
            JGeometry::TVec3f* pos = getPlayerPos_8023E724(0);
            s32 layer = getPlayerLayer_8023DE58(0);
            field_0x44 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TNGL, layer, pos, arg, -1, -1);
            field_0x5f = fn_8022ADB0();
            if (!field_0x5f) {
                fn_8022ADE4();
            }
        }
        if (field_0x44 > 0) {
            field_0x58 = 1;
            field_0x59 = 0xFF;
        }
        break;
    case 1:
        if (field_0x59 == 0) {
            field_0x58 = 2;
        } else if (field_0x59 >= 0 && field_0x59 < 3) {
            lbl_8053AED0->fn_8043D67C(0x33A, 0, lbl_80529DEC.field_0x24f);
            field_0x38++;
            field_0x58 = 3;
            field_0x40 = 5;
            gAudioMgr_8053AF80->fn_80458F9C(0x14);
            s32 end = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < end; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mActorId == ACTOR_DETH) {
                    fn_802B7CD8(actor);
                }
            }
        }
        break;
    }
}

// FIXME(shotgun): 90.8 %, missing/extra
// 0x80438E10
void cls_804B17A0::fn_80438E10(u8 flag) {
    field_0x40 = flag;
    if (flag) {
        return;
    }
    fn_80439AB0();
}

// fn_80438E94 (0xB0C, the per-state emitter / fade update) is not written yet.
