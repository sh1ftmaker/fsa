#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_on_off_switch.h"
#include "game/act/tu_801BEDBC.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Stops an emitter (it runs out) and forgets it (as act_rank_display).
static inline void releaseEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        emitter->becomeInvalidEmitter();
        emitter = NULL;
    }
}

void cls_804A8FC8::draw() {
    if (mState == 2 || mState == 1) {
        drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 70.0f);
    }
}

void cls_804A8FC8::update() {
    JGeometry::TBox2f box;
    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    box.set(mBox);
    box.addPos(p);
    s32 playerA = fn_8023AE34(&box, field_0x4, &p);
    mBox.i.x = -5.0f;
    mBox.i.y = -5.0f;
    mBox.f.x = 5.0f;
    mBox.f.y = 5.0f;
    box.set(mBox);
    box.addPos(p);
    s32 playerB = fn_8023BA68(&box, field_0x4, &p, true);
    if (playerB < 0) {
        playerB = playerA;
    }
    s32 playerC = fn_8023A6CC(&box, field_0x4);
    s32 lnbmSerial = fn_801F8108(this, ACTOR_LNBM, -1, 32.0f);
    s32 tuboSerial = fn_801F8108(this, ACTOR_TUBO, -1, 32.0f);
    s32 hbmbSerial = fn_801F2718(ACTOR_HBMB);
    field_0xd8 = 0;

    switch (mState) {
    case 0:
        if (field_0x254 == 0 && !(playerB < 0 && playerC < 0 && lnbmSerial < 0 && tuboSerial < 0 && hbmbSerial < 0)) {
            if (lnbmSerial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(lnbmSerial);
                if (actor != NULL && actor->mActorId == ACTOR_LNBM) {
                    static_cast<cls_804A4188*>(getActorManager_801F856C()->findActorBySerial_801F666C(lnbmSerial))->fn_801BEDBC();
                }
            } else if (hbmbSerial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(hbmbSerial);
                if (actor != NULL && actor->field_0x94 == 0) {
                    return;
                }
            }
            if (field_0x240 == 3 || field_0x240 == 4) {
                getGameView_802D6A9C()->fn_802D78B4(field_0x244, true);
            }
            gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xBE);
            vfunc_0x30(0xFF, true);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x42B, 1, 0, NULL, NULL, true, 1.0f);
            if (mEmitter230 == NULL) {
                mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x428, 1, 0, NULL, NULL, true, 1.0f);
            }
            if (mEmitter234 == NULL) {
                mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x429, 1, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x240 == 1 || field_0x240 == 3) {
                mState = 2;
                field_0x23c = 25;
            } else if (field_0x240 == 2) {
                mState = 1;
                field_0x23c = field_0x24c;
            } else {
                mState = 4;
            }
            startSound_801F0E34(0x30009, 0);
            field_0x254 = 1;
        } else {
            if (field_0x240 == 3 && getGameView_802D6A9C()->fn_802D7888(field_0x244) == true) {
                gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xBE);
                vfunc_0x30(0xFF, true);
                if (mEmitter230 == NULL) {
                    mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x428, 1, 0, NULL, NULL, true, 1.0f);
                }
                if (mEmitter234 == NULL) {
                    mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x429, 1, 0, NULL, NULL, true, 1.0f);
                }
                mState = 2;
                field_0x23c = 25;
                field_0x254 = 1;
            } else if (playerB < 0 && playerC < 0 && lnbmSerial < 0 && tuboSerial < 0 && hbmbSerial < 0) {
                field_0x254 = 0;
            }
        }
        break;
    case 2:
        if (field_0x23c == 0 && field_0x254 == 0) {
            if (!(playerB < 0 && playerC < 0 && lnbmSerial < 0 && tuboSerial < 0 && hbmbSerial < 0)) {
                field_0x254 = 1;
                if (lnbmSerial >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(lnbmSerial);
                    if (actor != NULL && actor->mActorId == ACTOR_LNBM) {
                        static_cast<cls_804A4188*>(getActorManager_801F856C()->findActorBySerial_801F666C(lnbmSerial))->fn_801BEDBC();
                    }
                } else if (hbmbSerial >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(hbmbSerial);
                    if (actor != NULL && actor->field_0x94 == 0) {
                        return;
                    }
                }
                if (field_0x240 == 3) {
                    getGameView_802D6A9C()->fn_802D78B4(field_0x244, false);
                }
                gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xBF);
                vfunc_0x30(0xFF, false);
                mState = 3;
                field_0x23c = 25;
                startSound_801F0E34(0x30009, 0);
                releaseEmitter(mEmitter230);
                releaseEmitter(mEmitter234);
            } else if (field_0x240 == 3 && getGameView_802D6A9C()->fn_802D7888(field_0x244) == false) {
                gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xBF);
                vfunc_0x30(0xFF, false);
                mState = 3;
                field_0x23c = 25;
                releaseEmitter(mEmitter230);
                releaseEmitter(mEmitter234);
            }
        } else {
            if (field_0x23c != 0) {
                field_0x23c--;
            }
            if (playerB < 0 && playerC < 0 && lnbmSerial < 0 && tuboSerial < 0 && hbmbSerial < 0) {
                field_0x254 = 0;
            }
        }
        break;
    case 1:
        if (field_0x23c == 0) {
            field_0x254 = 0;
            if (field_0x240 == 3) {
                getGameView_802D6A9C()->fn_802D78B4(field_0x244, false);
            }
            gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xBF);
            vfunc_0x30(0xFF, false);
            mState = 3;
            startSound_801F0E34(0x30009, 0);
            field_0x23c = 25;
            releaseEmitter(mEmitter230);
            releaseEmitter(mEmitter234);
        } else {
            field_0x23c--;
        }
        break;
    case 3:
        if (field_0x23c == 0) {
            mState = 0;
        } else {
            field_0x23c--;
        }
        break;
    case 4: // nothing to do
        break;
    }
}

cls_804A8FC8::~cls_804A8FC8() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
}

static const s32 lbl_8046D8F8[4] = {0x16, 0x11, 0xC, 7};

// FIXME: 99.19 %. Regalloc only: the original keeps the actor count in r31 and
// the byte offset of the array index in r30; we have them the other way round
// (declaring the count in the for-init made it worse).
void cls_804A8FC8::vfunc_0x10() {
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && (actor->mActorId == ACTOR_SWT4 || actor->mActorId == ACTOR_LOSW) &&
            field_0x170 == actor->field_0x170)
        {
            u32 params = actor->field_0x90;
            for (s32 j = 0; j < 4; j++) {
                if (field_0x244 == ((params >> lbl_8046D8F8[j]) & 0x1F)) {
                    field_0x258 = params >> 27;
                    if (getGameView_802D6A9C()->fn_802D800C(field_0x258)) {
                        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xBE);
                        if (mEmitter230 == NULL) {
                            mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x428, 1, 0, NULL, NULL, true, 1.0f);
                        }
                        if (mEmitter234 == NULL) {
                            mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x429, 1, 0, NULL, NULL, true, 1.0f);
                        }
                        mState = 4;
                        break;
                    }
                }
            }
        }
    }
}

void cls_804A8FC8::init() {
    mPos.x += 8.0f;
    mPos.y += 8.0f;
    field_0x244 = field_0x90 >> 27;
    field_0x24c = field_0x90 & 0xFFF;
    field_0x240 = (field_0x90 >> 12) & 7;
    field_0x248 = (field_0x90 >> 25) & 3;
    field_0x23c = 0;
    field_0x254 = 0;
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    u8 players = lbl_8053AAF8->fn_80138A30();
    s32 active = 0;
    if (field_0x248 == 0) {
        active = 1;
    } else if (field_0x248 == 1) {
        if (players != 2) {
            active = 1;
        }
    } else if (field_0x248 == 2) {
        if (!(players == 2 || players == 3)) {
            active = 1;
        }
    } else if (field_0x248 == 3) {
        active = 1;
    }
    if (active == 1) {
        if (field_0x240 == 3 || field_0x240 == 4) {
            if (getGameView_802D6A9C()->fn_802D7888(field_0x244) == true) {
                gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xBE);
                if (mEmitter230 == NULL) {
                    mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x428, 1, 0, NULL, NULL, true, 1.0f);
                }
                if (mEmitter234 == NULL) {
                    mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x429, 1, 0, NULL, NULL, true, 1.0f);
                }
                vfunc_0x30(0xFF, true);
                if (field_0x240 == 3) {
                    mState = 2;
                } else {
                    mState = 4;
                }
            } else {
                gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xBF);
                vfunc_0x30(0xFF, false);
                mState = 0;
            }
        } else {
            gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xBF);
            vfunc_0x30(0xFF, false);
            mState = 0;
        }
    } else {
        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0xBE);
        if (mEmitter230 == NULL) {
            mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x428, 1, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitter234 == NULL) {
            mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x429, 1, 0, NULL, NULL, true, 1.0f);
        }
        vfunc_0x30(0xFF, true);
        mState = 4;
    }
    mBox.i.x = -9.0f;
    mBox.i.y = -9.0f;
    mBox.f.x = 9.0f;
    mBox.f.y = 9.0f;
}
