#include "game/GameAudio.h"
#include "game/act/act_knight_of_hyrule.h"
#include "game/act/tu_802ED538.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/Unk_8045A080.h"
#include "game/cls_804AE9B0.h"

static const u32 lbl_804714A0[4] = { 0x737, 0x738, 0x739, 0x73A };
static const u32 lbl_804714B0[4] = { 0x280, 0x281, 0x282, 0x283 };
static s32 lbl_804AAB98[4] = { 2, 0, 1, 3 };

// Updates the Knight of Hyrule cutscene state machine.
void KnightOfHyrule_804AABA8::init() {
    mPos.z = 34.0f;
    field_0x3c.z = 0.0f;
    field_0x284 = 0.002f;
    mState = 0;
    field_0x234 = field_0x90 & 3;
    mTimer = 120;
    field_0x23c = field_0x90 >> 27;
    field_0x244 = -1;
    field_0x248 = -1;
    field_0x240 = 0;
    field_0x278 = -1;
    field_0x27c = 0;
    field_0x27d = 0;
    field_0x27e = 0;
    field_0x28c = 0;
    field_0x280 = 0xFF;
    field_0x282 = 0xFF;
}

// FIXME: 99.73% (regalloc-only: r29/r30 swap for mActorEnd and the mActors[i] byte offset).
void KnightOfHyrule_804AABA8::vfunc_0x10() {
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
            field_0x278 = i;
            mPos.x = actor->mPos.x;
            mPos.y = actor->mPos.y;
        }
    }
    field_0x24c.set(-64.0f, -16.0f, 64.0f, 128.0f);
    field_0x24c.i.x += mPos.x;
    field_0x24c.i.y += mPos.y;
    field_0x24c.f.x += mPos.x;
    field_0x24c.f.y += mPos.y;
    s8 dx = (field_0x90 >> 12) & 0xFF;
    if (dx == 0) {
        field_0x25c.x = mPos.x - 24.0f;
    } else {
        field_0x25c.x = mPos.x + dx;
    }
    s8 dy = (field_0x90 >> 4) & 0xFF;
    if (dy == 0) {
        field_0x25c.y = 100.0f + mPos.y + 8.0f;
    } else {
        field_0x25c.y = mPos.y + dy;
    }
    field_0x25c.z = 0.0f;
    field_0x268.x = mPos.x;
    field_0x268.y = mPos.y;
    field_0x268.z = 0.0f;
}

// FIXME: 98.62% (different: getActorManager_801F856C()->mActors[slot] indexing uses addi+lwzx
// instead of add+lwz 0x8; fn_802303FC's 3rd arg is declared bool in Player.h so it passes 1
// instead of 0xFF; and f0..f5 register allocation in the camera box setup).
void KnightOfHyrule_804AABA8::update() {
    JGeometry::TVec3f pos;
    switch (mState) {
    case 0:
        if (getGameView_802D6A9C()->fn_802D800C(field_0x23c)) {
            gAudioMgr_8053AF80->fn_80458F9C(60);
            mState = 1;
        } else {
            break;
        }
    case 1: {
        if (mTimer > 0) {
            mTimer--;
            break;
        }
        bool allLayer0 = true;
        pos.set(field_0x25c);
        for (s32 i = 0; i < 4; i++) {
            if (getPlayerLayer_8023DE58(i) != 0) {
                allLayer0 = false;
            }
            if (!fn_80230188(i) && !fn_8022D75C(i)) {
                if (getPlayerLayer_8023DE58(i) != 0) {
                    fn_8022CA6C(i, 5, false);
                    fn_80230C6C(i);
                    if (!fn_8022F514(i)) {
                        fn_802317F0(i, 0, getPlayerPos_8023E724(i), &pos, 0x1F);
                    }
                } else if (fn_8023188C(i)) {
                    fn_80230C6C(i);
                    fn_80230BF4(i, 3);
                    fn_80230AD4(i);
                    fn_8022CA6C(i, 5, false);
                    field_0x240 |= (1 << i);
                }
            }
            pos.x += 16.0f;
        }
        if (field_0x27e == 0 && allLayer0 == true) {
            s32 slot = field_0x278;
            if (slot >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
                if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                    static_cast<cls_804AA350*>(actor)->field_0x276 = 1;
                }
            }
            field_0x27e = 1;
        }
        if (field_0x240 == 0xF) {
            lbl_8053AD70->fn_8037AE00();
            mState = 2;
            mTimer = 150;
        }
        break;
    }
    case 2:
        mTimer--;
        if (mTimer <= 0) {
            pos.set(field_0x25c);
            field_0x244 = lbl_804AAB98[field_0x234];
            field_0x248 = fn_8022EA0C(field_0x244);
            pos.x += 24.0f;
            pos.y -= 8.0f;
            fn_80230498(field_0x244, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            fn_80230BF4(field_0x244, 3);
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x244 && fn_8022E438(i, field_0x244)) {
                    pos.set(*getPlayerPos_8023E724(i));
                    pos.y += 32.0f;
                    fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
                }
            }
            gBgRewrite_8053AC98->fn_8029D898(0x1A);
            gAudioMgr_8053AF80->fn_8045815C(false);
            mState = 3;
            mTimer = 10;
        }
        break;
    case 3:
        mTimer--;
        if (mTimer <= 0) {
            mTimer = 72;
            mState++;
        }
        break;
    case 4: {
        mTimer--;
        pos.set(*getPlayerPos_8023E724(field_0x244));
        pos.y -= 0.5f;
        if (mTimer <= 0) {
            fn_80230498(field_0x244, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
        } else {
            fn_802303FC(field_0x244, &pos, true);
        }
        if (mTimer <= 0) {
            mState = 5;
            mTimer = 60;
            field_0x280 = field_0x282 = gGameRoot2_8053AB10->field_0x2c->field_0xc;
        }
        break;
    }
    case 6:
        if (field_0x28c < 0xBF) {
            field_0x28c++;
        } else {
            field_0x28c += 2;
        }
        if (field_0x28c >= 0xFF) {
            field_0x28c = 0xFF;
            mState = 7;
            mTimer = 60;
        }
        break;
    case 8:
        fn_8022FE80(field_0x248, field_0x4, mSerial, false, false);
        if (field_0x248 == fn_8022FD8C(mSerial)) {
            if (fn_8038AD24(field_0x248, lbl_804714A0[field_0x234], mSerial, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4102)) {
                mState = 9;
            }
        }
        break;
    case 9:
        fn_8022FE80(field_0x248, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x248, mSerial)) {
            mState = 10;
            mTimer = 40;
        }
        break;
    case 11:
        if (field_0x28c > 2) {
            field_0x28c -= 2;
        } else {
            field_0x28c = 0;
            mState = 12;
            mTimer = 30;
        }
        break;
    case 12:
        mTimer--;
        if (mTimer <= 0) {
            s32 slot = field_0x278;
            if (slot >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
                if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                    static_cast<cls_804AA350*>(actor)->field_0x277 = 1;
                }
            }
            mState = 13;
            mPos.y -= 32.0f;
            mPos.z = 0.0f;
            field_0x3c.z = 0.4f;
        }
        break;
    case 13: {
        if (mPos.z >= 40.0f) {
            mPos.z = 40.0f;
            mState = 14;
            mTimer = 90;
            field_0x3c.z = 0.0f;
        }
        s32 slot = field_0x278;
        if (slot >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
            if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                if (static_cast<cls_804AA350*>(actor)->fn_802ED7F0(mPos.z)) {
                    mPos.z += field_0x3c.z;
                    addCalc2_801F71A4(&field_0x3c.z, 0.01f, 0.008f, 0.005f);
                    field_0x27c = 1;
                }
            }
        }
        break;
    }
    case 14:
        mTimer--;
        if (mTimer <= 0) {
            s32 slot = field_0x278;
            if (slot >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
                if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                    static_cast<cls_804AA350*>(actor)->field_0x278 = 1;
                }
            }
            mState = 15;
            mTimer = 510;
        }
        break;
    case 15:
        mTimer--;
        if (mTimer <= 0) {
            mState = 16;
            mTimer = 168;
        }
        break;
    case 16: {
        mTimer--;
        pos.set(*getPlayerPos_8023E724(field_0x244));
        pos.y -= 0.5f;
        if (mTimer <= 0) {
            fn_80230498(field_0x244, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
        } else {
            fn_802303FC(field_0x244, &pos, true);
        }
        if (mTimer < 60) {
            addCalc2_801F71A4(&field_0x3c.z, -0.2f, 0.02f, 0.01f);
            mPos.z += field_0x3c.z;
            s32 slot = field_0x278;
            if (slot >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
                if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                    static_cast<cls_804AA350*>(actor)->fn_802ED7F0(mPos.z);
                }
            }
            if (mTimer <= 0) {
                mState = 17;
                mTimer = 60;
            }
        }
        break;
    }
    case 18: {
        mPos.z += field_0x3c.z;
        s32 slot = field_0x278;
        if (slot >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
            if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                static_cast<cls_804AA350*>(actor)->fn_802ED7F0(mPos.z);
            }
        }
        if (mPos.z <= 2.0f) {
            mState = 19;
            mTimer = 30;
        } else if (mPos.z <= 10.0f && field_0x27d == 0) {
            s32 slot2 = field_0x278;
            if (slot2 >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot2];
                if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                    static_cast<cls_804AA350*>(actor)->fn_802ED7D8();
                    field_0x27d = 1;
                }
            }
        }
        if (field_0x27d == 1) {
            addCalc2_801F71A4(&field_0x3c.z, -0.01f, 0.02f, 0.05f);
        } else {
            addCalc2_801F71A4(&field_0x3c.z, -0.2f, 0.02f, 0.01f);
        }
        break;
    }
    case 19:
        mTimer--;
        if (mTimer <= 0) {
            mState = 20;
            mTimer = 90;
            pos.set(*getPlayerPos_8023E724(field_0x244));
            mPos.x = pos.x;
            mPos.y = pos.y - 10.0f;
            mPos.z = pos.z;
            field_0x288 = 0;
            fn_80230498(field_0x244, getPlayerPos_8023E724(field_0x244), field_0x288 + 0xED, 0xFF, -0x80, -0x80, true);
        }
        break;
    case 20:
        switch (mTimer) {
        case 0:
            if (fn_8038BD08(field_0x248, lbl_804714B0[field_0x234], field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4102)) {
                mState = 21;
            }
            gGameRoot_8053AAF8->fn_80138B7C();
            break;
        case 80:
            fn_80230F44(field_0x244, 0x1000002);
        case 76:
        case 72:
            mPos.y -= 4.0f;
            field_0x288++;
            fn_80230498(field_0x244, getPlayerPos_8023E724(field_0x244), field_0x288 + 0xED, 0xFF, -0x80, -0x80, true);
        default:
            mTimer--;
            break;
        }
        break;
    case 21:
        if (fn_80388CAC(field_0x248, mSerial)) {
            lbl_8053AD70->fn_8037AD70(field_0x234);
            mState = 22;
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x244 && fn_8022E438(i, field_0x244)) {
                    fn_80230498(i, getPlayerPos_8023E724(field_0x244), 0x3F, 0xFF, -0x80, -0x80, true);
                }
            }
        }
        break;
    case 22:
        fn_80230534(false);
        fn_8022CC20(field_0x248);
        getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 20) & 0x1F, true, 0xFF);
        for (s32 i = 0; i < 4; i++) {
            fn_8022CA6C(i, 5, true);
        }
        fn_8022ADCC();
        fn_801F35AC();
        break;
    case 17: {
        mPos.z += field_0x3c.z;
        s32 slot = field_0x278;
        if (slot >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
            if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                static_cast<cls_804AA350*>(actor)->fn_802ED7F0(mPos.z);
            }
        }
    }
    case 5:
    case 7:
    case 10:
        mTimer--;
        if (mTimer <= 0) {
            mState++;
        }
        break;
    }

    if (mState >= 3) {
        if (mState < 13 || (mState == 13 && field_0x27c == 0)) {
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            if (cam->field_0x840 == 0) {
                cam->field_0x840 = 1;
                cam->field_0x844.i.x = cam->field_0x844.f.x = field_0x268.x;
                cam->field_0x844.i.y = cam->field_0x844.f.y = field_0x268.y;
                cam->field_0x854 = 1.0f;
                cam->field_0x1d8 = cam->field_0x1d4 = -1.0f;
            }
        } else {
            f32 x0, y0, x1, y1;
            if (gGameRoot_8053AAF8->field_0x7e != 0) {
                x0 = -30.0f;
                y0 = -38.0f;
                x1 = 30.0f;
                y1 = 22.0f;
            } else {
                x0 = -10.0f;
                y0 = -18.0f;
                x1 = 10.0f;
                y1 = 2.0f;
            }
            x0 += field_0x268.x;
            x1 += field_0x268.x;
            y0 += field_0x268.y;
            y1 += field_0x268.y;
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            if (cam->field_0x7f5 == 0) {
                cam->field_0x7f4 = 1;
                cam->field_0x7f8 = 5;
                cam->field_0x808.set(x0, y0, x1, y1);
                cam->field_0x7f6 = 0;
            }
        }
        cls_804A03A8* unk30 = gGameRoot2_8053AB10->field_0x30;
        if (unk30->field_0x284 == 0) {
            unk30->field_0x284 = 1;
            unk30->field_0x28c = 0.05f;
        }
    }

    if (mState >= 5 && mState < 12) {
        if (field_0x280 > 100) {
            field_0x280 -= 2;
        }
        if (field_0x280 < 100) {
            field_0x280 = 100;
        }
    } else if (mState >= 12) {
        if (field_0x280 < field_0x282) {
            field_0x280 += 2;
        }
        if (field_0x280 >= field_0x282) {
            field_0x280 = field_0x282;
            gGameRoot2_8053AB10->field_0x2c->field_0xc = field_0x280;
            gGameRoot2_8053AB10->field_0x2c->field_0xd[1] = field_0x280;
        }
    }

    if (field_0x280 != field_0x282) {
        gGameRoot2_8053AB10->field_0x2c->field_0xc = field_0x280;
        gGameRoot2_8053AB10->field_0x2c->field_0xd[1] = field_0x280;
    }
}

// FIXME: 99.32% (different: getActorManager_801F856C()->mActors[slot] indexing uses addi+lwzx
// instead of add+lwz 0x8).
void KnightOfHyrule_804AABA8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1f = 0;
    if (mState >= 6 && mState < 13) {
        mPos.z += field_0x3c.z;
        field_0x3c.z += field_0x284;
        if (field_0x3c.z <= -0.05f || field_0x3c.z >= 0.05f) {
            field_0x284 = -field_0x284;
        }
        params.field_0x10.a = field_0x28c;
        params.field_0x1c = 1;
        params.field_0x1d = 1;
        params.field_0x20 = 0;
        params.field_0x2d = 1;
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x234 + 0x27B, &params, -1, 1.0f, 1.0f);
    } else if (mState >= 13 && field_0x27c == 1) {
        if ((mState >= 14 && mState < 16) || (mState == 16 && mTimer > 60)) {
            mPos.z += field_0x3c.z;
            field_0x3c.z += field_0x284;
            if (field_0x3c.z <= -0.04f || field_0x3c.z >= 0.04f) {
                field_0x284 = -field_0x284;
            }
            s32 slot = field_0x278;
            if (slot >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
                if (actor != NULL && actor->mActorId == ACTOR_GNEF) {
                    static_cast<cls_804AA350*>(actor)->fn_802ED7F0(mPos.z);
                }
            }
        }
        s32 layer;
        if (mState < 20) {
            params.field_0x20 = 0;
            layer = 7;
        } else {
            params.field_0x1d = 1;
            params.field_0x1c = 1;
            layer = -1;
        }
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x234 + 0x27F, &params, layer, 1.0f, 1.0f);
    }
}

KnightOfHyrule_804AABA8::~KnightOfHyrule_804AABA8() {}
