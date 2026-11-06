#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_special_conversation_npc.h"


// The state table update() runs from (field_0x238). Not const, so its
// initialiser (lbl_804AAE68) is copied into it at static-init time.
static void (SpecialConversationNPC_804AAFB8::*lbl_804AAED4[9])() = {
    &SpecialConversationNPC_804AAFB8::fn_80319658, &SpecialConversationNPC_804AAFB8::fn_803190E0,
    &SpecialConversationNPC_804AAFB8::fn_80318D74, &SpecialConversationNPC_804AAFB8::fn_80318B84,
    &SpecialConversationNPC_804AAFB8::fn_803187F4, &SpecialConversationNPC_804AAFB8::fn_803185A8,
    &SpecialConversationNPC_804AAFB8::fn_8031825C, &SpecialConversationNPC_804AAFB8::fn_80317F18,
    &SpecialConversationNPC_804AAFB8::fn_803193B4,
};
// The frame counts of the cycle (field_0x25c), indexed by field_0x264.
static s32 lbl_804AAF40[6] = {2, 3, 4, 5, 4, 3};

void SpecialConversationNPC_804AAFB8::init() {
    mState = 0;
    mPlayer = -1;
    field_0x244 = (field_0x90 & 0xFFF) + 0x12b;
    field_0x258 = -1;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 2;
    field_0x268 = 0;
    switch ((field_0x90 >> 16) & 0xF) {
    case 0:
        field_0x238 = 0;
        field_0x23c = 0;
        field_0x26c = 0;
        break;
    case 1:
        field_0x238 = 1;
        field_0x23c = 2;
        field_0x26c = 0x2f;
        break;
    case 2:
        field_0x238 = 1;
        field_0x23c = 1;
        field_0x26c = 0xc3;
        break;
    case 3:
        field_0x238 = 2;
        field_0x23c = 1;
        field_0x26c = 0x12e;
        if (getGameView_802D6A9C()->fn_802D7F40(2) == true) {
            mAlive = false;
        }
        break;
    case 4:
        field_0x238 = 3;
        field_0x23c = 1;
        field_0x26c = 0x130;
        break;
    case 5:
        field_0x238 = 4;
        field_0x23c = 3;
        field_0x26c = 0x530;
        field_0x25c |= field_0x264 << 1;
        break;
    case 6:
        field_0x238 = 5;
        field_0x23c = 1;
        field_0x26c = 0x132;
        field_0x244 = 0x580;
        field_0x264 = 0;
        break;
    case 7: {
        JGeometry::TVec3f spawnPos = mPos;
        spawnPos.x += 48.0f;
        field_0x258 = getActorManager_801F856C()->spawnActor_801F9484('SNPC', field_0x4, &spawnPos, 0x80000, -1, -1);
        s32 serial = field_0x258;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            static_cast<SpecialConversationNPC_804AAFB8*>(actor)->field_0x258 = mSerial;
        }
        field_0x238 = 6;
        field_0x23c = 1;
        field_0x26c = 0x126;
        field_0x244 = 0x591;
        field_0x260 = 10;
        if (getGameView_802D6A9C()->fn_802D800C(0x1F) == true) {
            field_0x264 = 3;
        }
        break;
    }
    case 8:
        field_0x238 = 7;
        field_0x23c = 1;
        field_0x26c = 0x120;
        field_0x244 = 0x58E;
        field_0x260 = 20;
        if (getGameView_802D6A9C()->fn_802D800C(0x1E) == true) {
            field_0x264 = 3;
        }
        break;
    case 9:
        field_0x238 = 8;
        field_0x23c = 2;
        field_0x26c = 0x315;
        break;
    case 10:
        field_0x238 = 2;
        field_0x23c = 2;
        field_0x26c = 0x37E;
        field_0x260 = 60;
        if (getGameView_802D6A9C()->fn_802D7F40(2) == true) {
            mAlive = false;
        }
        break;
    }
    if (field_0x238 == 0) {
        field_0x248.set(-10.0f, -2.0f, 16.0f, 18.0f);
    } else {
        field_0x248.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox = field_0x248;
    }
    if ((field_0x90 >> 27) != 0) {
        field_0x240 = (field_0x90 >> 24) & 3;
        bool done = vfunc_0x34();
        if ((field_0x240 == 1 && done == true) || (field_0x240 == 2 && done == false)) {
            field_0x270 = 0;
        } else {
            field_0x270 = 1;
        }
    } else {
        field_0x240 = 0;
        field_0x270 = 1;
    }
}

// FIXME: 97.79%. scheduling+regalloc: same instructions modulo registers, different order.
void SpecialConversationNPC_804AAFB8::vfunc_0x10() {
    if (field_0x238 == 0) {
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (int i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL) {
                if (actor->mActorId == 'JIJI') {
                    if (actor->field_0x4 == field_0x4) {
                        if (((actor->field_0x90 >> 16) & 0xFF) == 0x2A) {
                            field_0x258 = getActorManager_801F856C()->spawnActor_801F9484(
                                'SNPC', field_0x4, &actor->mPos, 0x90000 | (actor->field_0x90 & 0xFFF), -1, -1);
                            s32 serial = field_0x258;
                            if (serial >= 0) {
                                ActorBase_804A5770* spawned = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                                static_cast<SpecialConversationNPC_804AAFB8*>(spawned)->field_0x258 = mSerial;
                                actor->mAlive = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void SpecialConversationNPC_804AAFB8::update() {
    if (field_0x240 != 0) {
        bool done = vfunc_0x34();
        if ((field_0x240 == 1 && done == true) || (field_0x240 == 2 && done == false)) {
            field_0x270 = 0;
        } else {
            field_0x270 = 1;
        }
    }
    if (field_0x270 == 1) {
        (this->*lbl_804AAED4[field_0x238])();
    }
}

void SpecialConversationNPC_804AAFB8::draw() {
    if (field_0x270 != 0) {
        switch (field_0x23c & 3) {
        case 1:
            drawSprite_801F06F0("enemy_both_sob.bin", field_0x26c + field_0x25c, NULL, 7, 1.0f, 1.0f);
            fn_80307AC8(mSerial, field_0x4, &mPos, field_0x26c + field_0x25c, 0, 0, -1);
            break;
        case 2:
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x26c + field_0x25c, NULL, 7, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, field_0x26c + field_0x25c, 0, 2, -1, false, 0, 0);
            break;
        case 3:
            drawSprite_801F06F0("enemy_gc_sob.bin", field_0x26c + field_0x25c, NULL, 7, 1.0f, 1.0f);
            break;
        }
    }
}

// FIXME: 99.89%. regalloc-only: target->ours r31->r30.
void SpecialConversationNPC_804AAFB8::fn_80319658() {
    if (field_0x264 == 2) {
        JGeometry::TBox2f box = field_0x248;
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        if (mState != 1) {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 player = fn_8022FD8C(mSerial);
            if (player >= 0) {
                if (fn_802300EC(player)) {
                    if (fn_802373EC(player) == 6) {
                        u32 msg = field_0x244;
                        s32 serial = mSerial;
                        if (talkC678(player, msg, serial, field_0x4, mPos, 0x10) ? (mPlayer = player, mState = 1, true) : false) {
                            s32 other = field_0x258;
                            if (other >= 0) {
                                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(other);
                                if (actor != NULL && actor->mActorId == 'SNPC') {
                                    SpecialConversationNPC_804AAFB8* snpc = static_cast<SpecialConversationNPC_804AAFB8*>(actor);
                                    if (snpc->field_0x238 == 6 || snpc->field_0x238 == 7 || snpc->field_0x238 == 0 || snpc->field_0x238 == 8) {
                                        snpc->field_0x264 = 3;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
            fn_8022FF8C(mPlayer, &box, field_0x4, mSerial, true, false);
            if (fn_80388CAC(mPlayer, mSerial)) {
                s32 other = field_0x258;
                if (other >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(other);
                    if (actor != NULL && actor->mActorId == 'SNPC') {
                        SpecialConversationNPC_804AAFB8* snpc = static_cast<SpecialConversationNPC_804AAFB8*>(actor);
                        if (snpc->field_0x238 == 6 || snpc->field_0x238 == 7 || snpc->field_0x238 == 0 || snpc->field_0x238 == 8) {
                            snpc->field_0x264 = 2;
                        }
                    }
                }
                fn_801F35AC();
            }
        }
    }
}

void SpecialConversationNPC_804AAFB8::fn_803193B4() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    if (field_0x264 == 2) {
        box = field_0x248;
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        if (mState != 1) {
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            s32 player = fn_8022FD8C(mSerial);
            if (player >= 0) {
                if (fn_802300EC(player)) {
                    u32 msg = field_0x244;
                    s32 serial = mSerial;
                    if (talkC678(player, msg, serial, field_0x4, mPos, 0x10) ? (mPlayer = player, mState = 1, true) : false) {
                        s32 other = field_0x258;
                        if (other >= 0) {
                            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(other);
                            if (actor != NULL && actor->mActorId == 'SNPC') {
                                SpecialConversationNPC_804AAFB8* snpc = static_cast<SpecialConversationNPC_804AAFB8*>(actor);
                                if (snpc->field_0x238 == 6 || snpc->field_0x238 == 7 || snpc->field_0x238 == 0 || snpc->field_0x238 == 8) {
                                    snpc->field_0x264 = 3;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            fn_8022FF8C(mPlayer, &box, field_0x4, mSerial, true, false);
            if (fn_80388CAC(mPlayer, mSerial)) {
                mPlayer = -1;
                mState = 0;
                s32 other = field_0x258;
                if (other >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(other);
                    if (actor != NULL && actor->mActorId == 'SNPC') {
                        SpecialConversationNPC_804AAFB8* snpc = static_cast<SpecialConversationNPC_804AAFB8*>(actor);
                        if (snpc->field_0x238 == 6 || snpc->field_0x238 == 7 || snpc->field_0x238 == 0 || snpc->field_0x238 == 8) {
                            snpc->field_0x264 = 2;
                        }
                    }
                }
            }
        }
    }
    field_0x260++;
    if (field_0x260 >= 0x28) {
        field_0x260 = 0;
        field_0x25c ^= 1;
    }
}

void SpecialConversationNPC_804AAFB8::fn_80318B84() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    box = field_0x248;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (mState != 1) {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player >= 0) {
            if (fn_802300EC(player)) {
                u32 msg = field_0x244;
                s32 serial = mSerial;
                if (talkC678(player, msg, serial, field_0x4, mPos, 0x10)) {
                    mPlayer = player;
                    mState = 1;
                }
            }
        }
    } else {
        fn_8022FF8C(mPlayer, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(mPlayer, mSerial)) {
            mPlayer = -1;
            mState = 0;
        }
    }
    field_0x260++;
    if ((field_0x25c == 0 && field_0x260 >= 0x10) || (field_0x25c == 1 && field_0x260 >= 0x32)) {
        field_0x260 = 0;
        field_0x25c ^= 1;
    }
}

void SpecialConversationNPC_804AAFB8::fn_803185A8() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    box = field_0x248;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (mState != 1) {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player >= 0) {
            if (fn_802300EC(player)) {
                u32 msg = field_0x244;
                s32 serial = mSerial;
                if (talk(player, msg, serial, field_0x4, mPos, 0x10)) {
                    mPlayer = player;
                    mState = 1;
                }
            }
        }
    } else {
        fn_8022FF8C(mPlayer, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(mPlayer, mSerial)) {
            mPlayer = -1;
            mState = 0;
        }
    }
    field_0x260++;
    if ((field_0x25c < 2 && field_0x260 >= 0x10) || (field_0x25c >= 2 && field_0x260 >= 0xA)) {
        field_0x260 = 0;
        if (!getGameView_802D6A9C()->fn_802D7F40(2)) {
            field_0x264++;
            if (field_0x264 >= 6) {
                field_0x264 = 0;
            }
            field_0x25c = lbl_804AAF40[field_0x264];
        } else {
            field_0x25c++;
            field_0x25c &= 1;
        }
    }
}

SpecialConversationNPC_804AAFB8::~SpecialConversationNPC_804AAFB8() {}
