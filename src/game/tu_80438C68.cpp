#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/Message.h"
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

// FIXME(shotgun): 99.9 %, operands (string literal pooling)
// 0x80439C78
cls_804B17A0::cls_804B17A0() {
    lbl_8053AEC8 = this;
    field_0x4 = new J2DScreen;
    field_0x4->setPriority("gameover.blo", 0x1100000, lbl_80529DEC.field_0x4c);
    OSInitFastCast();
    field_0x8 = J2DAnmLoaderDataBase::load(fn_80403084('SCRN', "gameover.bck"));
    field_0x4->search('Ndata01')->setAnimation(static_cast<J2DAnmTransform*>(field_0x8));
    field_0x3c = 0;
    field_0xc[0] = NULL;
    field_0xc[1] = NULL;
    field_0xc[2] = NULL;
    field_0xc[3] = NULL;
    field_0xc[4] = NULL;
    field_0xc[5] = NULL;
    field_0xc[6] = NULL;
    field_0xc[7] = NULL;
    field_0xc[8] = NULL;
    field_0xc[9] = NULL;
    const ResTIMG* black = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_black.bti"));
    field_0x34 = new J2DPicture(black);
    field_0x34->setBlackWhite(JUtility::TColor(0xFF), JUtility::TColor(0));
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

// One of the eight spark emitters of the game-over screen: spawned once at (x, 255, 0), kept
// immortal, with a sound each time it is requested.
static inline void sparkEmitter(JPABaseEmitter*& slot, JGeometry::TVec3f& pos, f32 x) {
    pos.set(x, 255.0f, 0.0f);
    if (slot == NULL) {
        slot = spawnEmitter_8013CC50(0, &pos, 0x567, 0, 0, NULL, NULL, true, 1.0f);
    }
    if (slot != NULL) {
        slot->setStatus(JPAEMIT_Immortal);
    }
    gAudioMgr_8053AF80->fn_80458880(0xD, -1);
}

// FIXME(shotgun): 97.4 %, missing/extra (we keep one more saved register: the target reuses r28-r30 for the cleanup loops)
// 0x80438E94
u8 cls_804B17A0::fn_80438E94() {
    s32 serial = field_0x44;
    if (serial > 0 && getActorManager_801F856C()->findActorBySerial_801F666C(serial) == NULL) {
        field_0x44 = -2;
    }
    switch (field_0x40) {
    case 0:
        if (field_0x5a == 0xFF) {
            if (gGameRoot_8053AAF8->field_0x7f == 1) {
                if (lbl_80529DEC.field_0x24f != 0) {
                    field_0x48 = 0;
                    field_0x5a = 1;
                } else {
                    field_0x5a = 0;
                }
            } else {
                field_0x5a = 0;
            }
            field_0x5b = gGameRoot2_8053AB10->field_0x2c->field_0xc;
        }
        if (field_0x5a != 0) {
            fn_80438C68();
        } else {
            field_0x44 = -2;
            field_0x58 = 2;
        }
        if (field_0x58 == 2) {
            u8 alpha = field_0x41;
            if (field_0x42 < alpha - 2) {
                field_0x42 += 2;
                field_0x4->search('Pblk')->setAlpha(field_0x42);
            } else {
                field_0x4->search('Pblk')->setAlpha(alpha);
                field_0x40 = 1;
                field_0x56 = 1;
                gAudioMgr_8053AF80->fn_80458F9C(0x14);
            }
        }
        break;
    case 1: {
        field_0x3c++;
        if (field_0x3c >= 0x3C) {
            field_0x40 = 3;
        }
        JGeometry::TVec3f sparkPos;
        switch (field_0x3c) {
        case 0x1E:
            sparkEmitter(field_0xc[0], sparkPos, 126.0f);
            break;
        case 0x11:
            sparkEmitter(field_0xc[1], sparkPos, 174.0f);
            break;
        case 0x15:
            sparkEmitter(field_0xc[2], sparkPos, 224.0f);
            break;
        case 0x1B:
            sparkEmitter(field_0xc[3], sparkPos, 272.0f);
            break;
        case 0x13:
            sparkEmitter(field_0xc[4], sparkPos, 340.0f);
            break;
        case 0x19:
            sparkEmitter(field_0xc[5], sparkPos, 386.0f);
            break;
        case 0x21:
            sparkEmitter(field_0xc[6], sparkPos, 433.0f);
            break;
        case 0x17:
            sparkEmitter(field_0xc[7], sparkPos, 475.0f);
            break;
        }
        break;
    }
    case 3:
        if (field_0x44 == -2 && lbl_8053AD30->fn_80309C4C(0x1100, -1) != 0) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            if (field_0xc[8] == NULL) {
                field_0xc[8] = spawnEmitter_8013CC50(0, &pos, 0x568, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0xc[8] != NULL) {
                field_0xc[8]->setStatus(JPAEMIT_Immortal);
            }
            if (field_0xc[9] == NULL) {
                field_0xc[9] = spawnEmitter_8013CC50(0, &pos, 0x566, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0xc[9] != NULL) {
                field_0xc[9]->setStatus(JPAEMIT_Immortal);
            }
            field_0x40 = 4;
            field_0x42 = 0;
            gAudioMgr_8053AF80->fn_80458880(0x48, -1);
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        break;
    case 4:
        if (field_0x42 < lbl_8050DB0C.field_0x4c[8]) {
            field_0x42++;
        } else {
            field_0x3c++;
            if (field_0x3c >= field_0x8->mTotalFrameCount - 1) {
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
                field_0x3c = 0;
                field_0x42 = 0;
                field_0x40 = 6;
            }
        }
        break;
    case 6:
        if (field_0x42 < lbl_8050DB0C.field_0x4c[9]) {
            field_0x42++;
        } else {
            lbl_80529DEC.field_0x237 = 7;
            field_0x40 = 7;
        }
        break;
    }
    field_0x8->mCurrentFrame = field_0x3c;
    field_0x4->search('Ndata01')->animationTransform();
    for (s32 i = 0; i < 8; i++) {
        if (field_0xc[i] != NULL && field_0xc[i]->isEnableDeleteEmitter()) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0xc[i]);
            field_0xc[i] = NULL;
        }
    }
    if (field_0xc[8] != NULL && field_0xc[8]->isEnableDeleteEmitter()) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0xc[8]);
        field_0xc[8] = NULL;
    }
    if (field_0xc[9] != NULL && field_0xc[9]->isEnableDeleteEmitter()) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0xc[9]);
        field_0xc[9] = NULL;
    }
    if (field_0x54 < lbl_8050DB0C.field_0x4c[40]) {
        field_0x54++;
    } else if (field_0x4c > 0.0f) {
        field_0x4c -= 1.0f / lbl_8050DB0C.field_0x4c[41];
        if (field_0x4c <= 0.0f) {
            field_0x5d = 0;
            field_0x4c = 0.0f;
        }
    }
    if (field_0x40 == 5) {
        gGameRoot2_8053AB10->field_0x2c->field_0xc = field_0x5b;
        if (field_0x5f == 0) {
            fn_8022ADCC();
        }
        if (field_0x5e == 0) {
            field_0x5e = 1;
        } else {
            field_0x5e = 0;
        }
        field_0x5c = 0;
    } else {
        s32 level = (s32)field_0x4c;
        if ((u8)level < field_0x5b) {
            gGameRoot2_8053AB10->field_0x2c->field_0xc = level;
        }
        field_0x5c = (s32)(255.0f - field_0x4c);
    }
    if (field_0x56 > 0) {
        if (field_0x56 < lbl_8050DB0C.field_0x4c[38]) {
            field_0x56++;
        } else if (field_0x50 < 255.0f) {
            field_0x50 += 255.0f / lbl_8050DB0C.field_0x4c[39];
            if (field_0x50 > 255.0f) {
                field_0x50 = 255.0f;
            }
        }
    }
    return field_0x40;
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
