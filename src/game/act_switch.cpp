#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"

static const s32 lbl_8046C828[4] = { 22, 17, 12, 7 };
static const u32 lbl_8046C838[5] = { 'AMOS', 'BRBL', 'BLCK', 'BCK2', 'SMRI' };

void Switch_804A7F38::init() {
    field_0x238 = ((field_0x90 >> 22) & 0x1F) + 1;
    field_0x23c = ((field_0x90 >> 17) & 0x1F) + 1;
    field_0x240 = field_0x90 & 0x3E0;
    field_0x234 = field_0x90 & 0x1F;
    field_0x264 = (field_0x90 >> 12) & 0x1F;
    if (field_0x264 == 0) {
        field_0x264 = -1;
    }
    field_0x25c = (u32)field_0x90 >> 27;

    if (field_0x234 == 8 && field_0x240 == 0) {
        field_0x240 = 60;
    }

    field_0x24c = field_0x240;
    mPos.x += 8.0f;
    mPos.y += 8.0f;

    if (field_0x234 == 2 || field_0x234 == 3) {
        mBox.i.x = -1.0f;
        mBox.i.y = -1.0f;
        mBox.f.x = 1.0f;
        mBox.f.y = 1.0f;
    } else if (field_0x234 == 7) {
        f32 y = field_0x23c * 16.0f;
        f32 x = field_0x238 * 16.0f;
        mBox.i.x = -x;
        mBox.i.y = -y;
        mBox.f.x = x;
        mBox.f.y = y;
    } else {
        mBox.i.x = -1.0f;
        mBox.i.y = -1.0f;
        mBox.f.x = 1.0f;
        mBox.f.y = 1.0f;
    }

    field_0x248 = 0;
    if (field_0x234 == 4 || field_0x234 == 7) {
        mState = 2;
    } else {
        mState = 1;
    }

    field_0x250 = -1;
    field_0x260 = -1;
}

// FIXME: 99.69 %. Regalloc only: target->ours r30->r31, r4->r5, r5->r4, r31->r30.
void Switch_804A7F38::vfunc_0x10() {
    if (mState == 1) {
        if (vfunc_0x34()) {
            field_0x244 = fn_80226AE0(&mPos, field_0x4);
            field_0x258 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, field_0x244) & 0x3FF;
            vfunc_0x54(-1);
            mAlive = false;
        } else {
            field_0x244 = fn_80226AE0(&mPos, field_0x4);
            field_0x258 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, field_0x244) & 0x3FF;
            if (field_0x258 == 0x12 || field_0x258 == 0x50 || field_0x258 == 0x52 || field_0x258 == 0x41 || field_0x258 == 0x43 ||
                field_0x258 == 0x68 || field_0x258 == 0x6a || field_0x258 == 0x15a || (field_0x258 == 0x13 || field_0x258 == 0x14) || field_0x258 == 0x51) {
                mState = 2;
                if (field_0x258 == 0x41) {
                    field_0x258 = 0x12;
                }
                s32 switchBit = field_0x264;
                if (switchBit < 0) {
                    s32 end = getActorManager_801F856C()->mActorEnd;
                    for (int i = 0; i < end; i++) {
                        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                        if (!actor) continue;
                        if (actor->mActorId != 'SWT4' && actor->mActorId != 'LOSW') continue;
                        if (field_0x170 != actor->field_0x170) continue;
                        u32 param = actor->field_0x90;
                        if ((param & 0x1F) == 1 || (param & 0x1F) == 2) {
                            for (int j = 0; j < 4; j++) {
                                if (field_0x25c == ((param >> lbl_8046C828[j]) & 0x1F)) {
                                    field_0x260 = param >> 27;
                                    if (getGameView_802D6A9C()->fn_802D800C(field_0x260)) {
                                        vfunc_0x54(-1);
                                        mAlive = false;
                                    } else {
                                        if (field_0x258 == 0x13 || field_0x258 == 0x51) {
                                            if (field_0x258 == 0x13) {
                                                field_0x258 = 0x12;
                                            } else {
                                                field_0x258 = 0x51;
                                            }
                                            vfunc_0x30(0xFF, false);
                                            gBgRewrite_8053AC98->fn_802A3884(&mPos, -1, field_0x4, field_0x258);
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (field_0x258 == 0x43 || field_0x258 == 0x68 || field_0x258 == 0x6a || field_0x258 == 0x41) {
                        if (vfunc_0x34()) {
                            vfunc_0x54(-1);
                            mAlive = false;
                        } else {
                            mState = 1;
                        }
                    }
                } else {
                    if (getGameView_802D6A9C()->fn_802D800C(switchBit)) {
                        vfunc_0x54(-1);
                        mAlive = false;
                    } else if (field_0x258 == 0x43 || field_0x258 == 0x68 || field_0x258 == 0x6a) {
                        mState = 1;
                    }
                }
            }
        }
    }
}

// FIXME: 99.71 %. Operands: regalloc (r27/r28), and fn_802302C4 3rd argument (target passes 0xFF, but shared Player.h declares bool b).
void Switch_804A7F38::update() {
    JGeometry::TVec3f playerPos;
    f32 y = mPos.y;
    f32 x = mPos.x;
    JGeometry::TBox2f box(mBox);
    box.i.x += x;
    box.i.y += y;
    box.f.x += x;
    box.f.y += y;

    if (fn_801F8108(this, 'TBO2', -1, 32.0f) >= 0) {
        return;
    }

    s32 player = fn_8023AF14(&box, field_0x4, true, 0.0f);
    s32 r30 = 0;
    s32 r27 = -1;

    if (field_0x234 != 7) {
        r27 = fn_801F8108(this, 'AMOS', -1, 32.0f);
        if (r27 < 0) {
            r27 = fn_801F8108(this, 'BRBL', -1, 32.0f);
            r30 = 1;
            if (r27 < 0) {
                r27 = fn_801F8108(this, 'BLCK', -1, 32.0f);
                r30 = 2;
                if (r27 < 0) {
                    r27 = fn_801F8108(this, 'BCK2', -1, 32.0f);
                    r30 = 3;
                    if (r27 < 0) {
                        r27 = fn_801F8108(this, 'SMRI', -1, 0.1f);
                        r30 = 4;
                    }
                }
            }
        }
    }

    if (player >= 0 && (field_0x234 == 2 || field_0x234 == 3)) {
        playerPos = *getPlayerPos_8023E724(player);
        f32 diff = mPos.x - playerPos.x;
        if (!(diff > -7.0f) || !(diff < 7.0f)) {
            player = -1;
        }
    }

    switch (mState) {
    case 0:
    case 6:
    default:
        break;

    case 1:
        if (vfunc_0x34()) {
            field_0x244 = fn_80226AE0(&mPos, field_0x4);
            field_0x258 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, field_0x244) & 0x3FF;
            vfunc_0x54(player);
            mAlive = false;
        } else {
            field_0x244 = fn_80226AE0(&mPos, field_0x4);
            field_0x258 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, field_0x244) & 0x3FF;
            if (field_0x258 == 0x12 || field_0x258 == 0x50 || field_0x258 == 0x14 ||
                field_0x258 == 0x52 || field_0x258 == 0x15a || field_0x258 == 0x41) {
                mState = 2;
            }
        }
        break;

    case 2:
        if (player >= 0) {
            if (field_0x234 != 4 && field_0x234 != 7) {
                if (fn_8023513C(player) == true || fn_802353AC(player) == true ||
                    fn_80233DA4(player) == true || fn_80235004(player) == true) {
                    break;
                }
                r30 = vfunc_0x3C();
                if (getGameView_802D6A9C()->fn_802D800C(r30) == false) {
                    if (gGameRoot_8053AAF8->field_0x7e == 1) {
                        bool found = false;
                        r30 = getActorManager_801F856C()->mActorEnd;
                        for (int i = 0; i < r30; i++) {
                            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                            if (actor && actor->mActorId == 'SWTH' && actor->mSerial != mSerial) {
                                u32 param = actor->field_0x90;
                                if (player == getGameView_802D6A9C()->fn_802D7E1C(param >> 27)) {
                                    u32 kind = param & 0x3F;
                                    if (kind == 0 || kind == 5) {
                                        found = true;
                                    }
                                }
                            }
                        }
                        if (found) {
                            break;
                        }
                        if (fn_8022E39C(player, 10) != true) {
                            break;
                        }
                        vfunc_0x30(player, true);
                        startSound_801F0E34(0x30009, 0);
                        vfunc_0x54(player);
                        mState = 3;
                        field_0x248 = 5;
                        field_0x250 = player;
                    } else {
                        if (player == 0) {
                            if (fn_8022E39C(player, 5) == true) {
                                vfunc_0x30(player, true);
                                startSound_801F0E34(0x30009, 0);
                                vfunc_0x54(player);
                                field_0x248 = 5;
                            }
                        } else {
                            vfunc_0x30(player, true);
                            startSound_801F0E34(0x30009, 0);
                            vfunc_0x54(player);
                            field_0x248 = 0;
                        }
                        mState = 3;
                        field_0x250 = player;
                    }
                } else {
                    vfunc_0x54(player);
                    field_0x250 = player;
                    mState = 4;
                }
            } else {
                vfunc_0x30(player, true);
                mState = 6;
            }
        } else if (r27 >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(r27);
            if (actor && actor->mActorId == lbl_8046C838[r30]) {
                field_0x250 = -1;
                field_0x254 = r30;
                vfunc_0x30(0xFF, true);
                startSound_801F0E34(0x30009, 0);
                vfunc_0x54(field_0x250);
                mState = 7;
                field_0x248 = 10;
            }
        }
        break;

    case 3:
        fn_802302C4(field_0x250, 0xE8, 0xFF);
        if (field_0x248 == 0) {
            mState = 4;
        } else {
            field_0x248--;
        }
        if (field_0x248 == 2 && (field_0x234 == 2 || field_0x234 == 3 || field_0x234 == 6)) {
            startSound_801F0E34(0x30009, 0);
        }
        break;

    case 4:
        if (player < 0) {
            if (fn_80230188(field_0x250)) {
                break;
            }
            if (vfunc_0x34() && (field_0x234 == 1 || field_0x234 == 3)) {
                mAlive = false;
                break;
            }
            if (field_0x234 == 8) {
                mState = 5;
                field_0x24c = field_0x240;
                break;
            }
            if ((u32)field_0x234 <= 1 || field_0x234 == 5) {
                r27 = field_0x264;
                if (r27 >= 0) {
                    if (getGameView_802D6A9C()->fn_802D800C(r27)) {
                        mAlive = false;
                        break;
                    }
                } else {
                    r27 = field_0x260;
                    if (r27 >= 0) {
                        if (getGameView_802D6A9C()->fn_802D800C(r27)) {
                            mAlive = false;
                            break;
                        }
                    }
                }
                if (field_0x234 == 0 || field_0x234 == 5) {
                    vfunc_0x30(0xFF, false);
                    gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, field_0x258);
                } else {
                    gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, field_0x258);
                }
            } else {
                if (field_0x234 == 2 || field_0x234 == 6) {
                    vfunc_0x30(0xFF, false);
                    gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, field_0x258);
                } else {
                    gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, field_0x258);
                }
            }
            mState = 2;
        } else {
            fn_802302C4(player, 0xE8, 0xFF);
            if (player == field_0x250) {
                if (field_0x234 == 5 || field_0x234 == 6) {
                    vfunc_0x30(field_0x250, true);
                }
            } else {
                field_0x250 = player;
                vfunc_0x30(field_0x250, true);
            }
        }
        break;

    case 5:
        if (field_0x24c == 0) {
            vfunc_0x30(0xFF, false);
            gBgRewrite_8053AC98->fn_802A3884(&mPos, player, field_0x4, field_0x258);
            mState = 2;
        } else {
            field_0x24c--;
        }
        break;

    case 7:
        if (field_0x248 == 0) {
            mState = 8;
        } else {
            field_0x248--;
        }
        break;

    case 8:
        if (r27 >= 0 && (r27 < 0 || field_0x254 == r30)) {
            break;
        }
        if (field_0x234 == 0 || field_0x234 == 1 || field_0x234 == 5 || field_0x234 == 8) {
            if (field_0x234 == 0 || field_0x234 == 5) {
                vfunc_0x30(0xFF, false);
                gBgRewrite_8053AC98->fn_802A3884(&mPos, field_0x250, field_0x4, field_0x258);
            } else {
                gBgRewrite_8053AC98->fn_802A3884(&mPos, field_0x250, field_0x4, field_0x258);
            }
        } else {
            if (field_0x234 == 2 || field_0x234 == 6) {
                vfunc_0x30(0xFF, false);
            }
            gBgRewrite_8053AC98->fn_802A3884(&mPos, field_0x250, field_0x4, field_0x258);
        }
        mState = 2;
        break;
    }
}

void Switch_804A7F38::draw() {
}

void Switch_804A7F38::vfunc_0x54(s32 arg) {
    switch (field_0x258) {
    case 0x12:
    case 0x41:
    case 0x68:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x13);
        break;
    case 0x43:
    case 0x50:
    case 0x6a:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x51);
        break;
    case 0x14:
    case 0x52:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x15);
        break;
    case 0x15a:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x360);
        break;
    }
}

Switch_804A7F38::~Switch_804A7F38() {
}
