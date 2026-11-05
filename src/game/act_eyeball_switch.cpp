#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_eyeball_switch.h"

static const s32 lbl_80475138[4] = {0x16, 0x11, 0xC, 7};

void cls_804AE678::vfunc_0x24() {
    deleteEmitter(&mEmitter);
    mAlive = false;
}

// FIXME: 99.1 %. The target loads the bit argument of fn_802D78B4
// (field_0x248) after the field_0x244 byte, we load it before; and the
// .sdata2 literals come out as 32.0f, 1.0f, the target has 1.0f, 32.0f
// (relocation names only).
void cls_804AE678::update() {
    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
    JGeometry::TBox2f box(mBox);
    box.addPos(p);
    fn_80239914(&box, field_0x4, mSerial);
    fn_8020D58C()->addBox(mSerial);
    s32 arrow = field_0x250;
    if (arrow != -1) {
        ActorBase_804A5770* hit = getActorManager_801F856C()->findActorBySerial_801F666C(arrow);
        if (hit != NULL && hit->mActorId == ACTOR_ARRW) {
            return;
        }
        field_0x250 = -1;
    }
    switch (field_0x234) {
    case 0:
        if (field_0x238 != 0) {
            field_0x238--;
        } else {
            fn_8023BA68(&box, field_0x4, &p, true);
            s32 serial = fn_801F8108(this, ACTOR_ARRW, -1, 32.0f);
            if (serial >= 0) {
                ActorBase_804A5770* arrowActor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (arrowActor != NULL && arrowActor->mActorId == ACTOR_ARRW) {
                    field_0x244 ^= 1;
                    field_0x250 = serial;
                    if (field_0x23c >= 3) {
                        getGameView_802D6A9C()->fn_802D78B4(field_0x248, field_0x244);
                    }
                    if (field_0x23c == 0 || field_0x23c == 4) {
                        vfunc_0x30(0xFF, true);
                        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x35);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x181, 0, 0, NULL, NULL, true, 1.0f);
                        startSound_801F0E34(0x3008F, 0);
                        field_0x234 = 2;
                    } else {
                        if (field_0x23c == 2) {
                            field_0x234 = 1;
                            field_0x238 = field_0x24c;
                        } else {
                            field_0x238 = 25;
                        }
                        if (!field_0x244) {
                            vfunc_0x30(0xFF, false);
                            gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xAC);
                        } else {
                            vfunc_0x30(0xFF, true);
                            gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x35);
                            spawnEmitter_8013CC50(field_0x4, &mPos, 0x181, 0, 0, NULL, NULL, true, 1.0f);
                            startSound_801F0E34(0x3008F, 0);
                        }
                    }
                }
            }
        }
        break;
    case 1:
        if (field_0x238 != 0) {
            field_0x238--;
            if (field_0x238 == 0) {
                field_0x234 = 0;
                vfunc_0x30(0xFF, false);
                gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xAC);
                field_0x244 = false;
            }
        }
        break;
    case 2:
        field_0x244 = true;
        break;
    }
    if (field_0x244 == true) {
        if (mEmitter == NULL) {
            mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x182, 0, 0, NULL, NULL, true, 1.0f);
        }
    } else {
        deleteEmitter(&mEmitter);
    }
}

cls_804AE678::~cls_804AE678() {
    if (mEmitter != NULL) {
        releaseEmitter_801EE434(mEmitter);
        mEmitter = NULL;
    }
}

// FIXME: 98.5 %. Regalloc only: the target keeps this/end in r29/r28 (we swap
// them) and params/table/params >> 27 in r27/r26/r25 (we have r25/r27/r26).
void cls_804AE678::vfunc_0x10() {
    s32 i;
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (i = 0; i < end; i++) {
        ActorBase_804A5770* monitor = getActorManager_801F856C()->mActors[i];
        if (monitor != NULL && (monitor->mActorId == ACTOR_SWT4 || monitor->mActorId == ACTOR_LOSW) &&
            field_0x170 == monitor->field_0x170)
        {
            u32 params = monitor->field_0x90;
            for (s32 j = 0; j < 4; j++) {
                if (field_0x248 == ((params >> lbl_80475138[j]) & 0x1F)) {
                    field_0x254 = params >> 27;
                    if (getGameView_802D6A9C()->fn_802D800C(field_0x254)) {
                        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x35);
                        field_0x234 = 2;
                        break;
                    }
                }
            }
        }
    }
}

void cls_804AE678::init() {
    mPos.x += 8.0f;
    mPos.y += 8.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mBox.set(-8.5f, -8.5f, 8.5f, 8.5f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x250 = -1;
    field_0x24c = field_0x90 & 0xFFF;
    field_0x23c = (field_0x90 >> 12) & 7;
    field_0x248 = field_0x90 >> 27;
    field_0x240 = (field_0x90 >> 25) & 3;
    field_0x238 = 0;
    vfunc_0x30(0xFF, false);
    field_0x244 = false;
    field_0x234 = 0;
    mEmitter = NULL;
    u8 players = lbl_8053AAF8->fn_80138A30();
    s32 active = 0;
    if (field_0x240 == 0) {
        active = 1;
    } else if (field_0x240 == 1) {
        if (players != 2) {
            active = 1;
        }
    } else if (field_0x240 == 2) {
        if (!(players == 2 || players == 3)) {
            active = 1;
        }
    } else if (field_0x240 == 3) {
        active = 1;
    }
    if (active == 1) {
        if (field_0x23c >= 3 && getGameView_802D6A9C()->fn_802D7888(field_0x248) == true) {
            gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x35);
            vfunc_0x30(0xFF, true);
            if (field_0x23c == 4) {
                field_0x234 = 2;
            } else {
                field_0x244 = true;
            }
        } else {
            gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0xAC);
        }
    } else {
        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x35);
        vfunc_0x30(0xFF, true);
        field_0x234 = 2;
        field_0x244 = true;
    }
}
