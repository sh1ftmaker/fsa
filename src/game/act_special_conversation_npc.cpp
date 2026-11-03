#include "game/GameAudio.h"
#include "game/act/act_special_conversation_npc.h"

SpecialConversationNPC_804AAFB8::~SpecialConversationNPC_804AAFB8() {}

static void (SpecialConversationNPC_804AAFB8::*lbl_804AAED4[9])() = {};

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
