#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_special_conversation_npc.h"

SpecialConversationNPC_804AAFB8::~SpecialConversationNPC_804AAFB8() {}

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
        if (!fn_802D6A9C()->fn_802D7F40(2)) {
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
