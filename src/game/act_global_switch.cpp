#include "game/GameAudio.h"
#include "game/act/act_global_switch.h"
#include "game/GameView.h"

GlobalSwitch_804AA060::~GlobalSwitch_804AA060() {}

void GlobalSwitch_804AA060::init() {
    update();
}

void GlobalSwitch_804AA060::update() {
    if (fn_802D6A9C()->fn_802D800C((field_0x90 >> 22) & 0x1F)) {
        fn_802D6A9C()->fn_802D7F84(vfunc_0x3C(), true);
    }
}

void GlobalSwitch_804AA060::draw() {}
