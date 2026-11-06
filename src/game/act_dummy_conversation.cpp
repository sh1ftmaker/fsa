#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_dummy_conversation.h"

void DummyConversation_804B1A48::init() {
    if (!fn_802308E0(field_0x90 >> 28)) {
        mAlive = false;
    }
    mState = 0;
    mMessage = 1;
}

void DummyConversation_804B1A48::update() {
    s32 player = field_0x90 >> 28;
    switch (mState) {
    case 0:
        if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x400) &&
            lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 0x100)) {
            lbl_8053AED0->field_0x1f0[player] = 2;
            fn_8023061C(player, 0);
            mAlive = false;
            break;
        }
        // pick of the message: 1, 10 or 100 per press by held buttons
        if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x100)) {
            if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x200)) {
                if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 2)) {
                    mMessage += 100;
                } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 1)) {
                    if (mMessage > 100) {
                        mMessage -= 100;
                    } else {
                        mMessage = 1;
                    }
                }
            } else {
                if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 2)) {
                    mMessage += 10;
                } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 1)) {
                    if (mMessage > 10) {
                        mMessage -= 10;
                    } else {
                        mMessage = 1;
                    }
                }
            }
        } else {
            if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 2)) {
                mMessage += 1;
            } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 1)) {
                if (mMessage > 1) {
                    mMessage -= 1;
                } else {
                    mMessage = 1;
                }
            }
        }
        if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x20) &&
            lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(player), 0x20) &&
            lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), 0x100)) {
            mState = 1;
            fn_8023061C(player, 0);
        }
        break;
    case 1:
        fn_8022FE80(player, field_0x4, mSerial, 0, 0);
        s32 owner = fn_8022FD8C(mSerial);
        if (owner == player) {
            if (open(player, field_0x4, mSerial, mPos, 0x10)) {
                mState = 2;
            } else {
                lbl_8053AED0->field_0x1f0[player] = 2;
                mAlive = false;
            }
        }
        break;
    case 2:
        fn_8022FE80(player, field_0x4, mSerial, 0, 0);
        if (fn_80388CAC(player, mSerial)) {
            lbl_8053AED0->field_0x1f0[player] = 2;
            mAlive = false;
        }
        break;
    }
}

DummyConversation_804B1A48::~DummyConversation_804B1A48() {}
