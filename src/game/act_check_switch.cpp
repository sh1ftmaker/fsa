#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_check_switch.h"

CheckSwitch_804A8BD0::~CheckSwitch_804A8BD0() {}

void CheckSwitch_804A8BD0::draw() {}

// FIXME: 99.55 %. Only the register allocation of the shift: the original
// loads the table entry into r0 and field_0x90 into r4 (srw r0, r4, r0);
// ours loads field_0x90 into r0 and the entry into r3.
void CheckSwitch_804A8BD0::update() {
    static const s32 sShifts[4] = {22, 17, 12, 7};

    switch (mState) {
    case 0:
        if ((field_0x90 >> 27) == 0 || vfunc_0x34() == 1) {
            s32 bit = (field_0x90 >> sShifts[fn_801CD664(100) & 3]) & 0x1F;
            fn_802D6A9C()->fn_802D8050(bit, true, 0xFF);
            if (mMode == 0) {
                fn_801F3668();
            } else if (mMode == 1) {
                mState = 1;
            } else if (mMode == 2) {
                mAlive = false;
            }
        }
        break;
    case 1:
        if (!vfunc_0x34()) {
            mState = 0;
        }
        break;
    }
}

void CheckSwitch_804A8BD0::init() {
    mMode = field_0x90 & 0x1F;
    mState = 0;
    update();
}
