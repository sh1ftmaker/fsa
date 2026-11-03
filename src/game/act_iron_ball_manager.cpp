#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/act/act_iron_ball_manager.h"

IronBallManager_804ABA38::~IronBallManager_804ABA38() {}

// FIXME: 98.9 %. Only the clear of bit 0 in field_0x90 differs: the original
// loads -2 into r0 early (li r0, -2) and does `and r0, r3, r0` with the
// decremented counter in r3; ours emits `clrrwi r0, r0, 1` (one instruction
// fewer) and keeps the counter in r0.
void IronBallManager_804ABA38::update() {
    JGeometry::TVec3f pos = field_0x230;
    if (!vfunc_0x34()) {
        if (field_0x240 == 0) {
            if (field_0x247) {
                if (field_0x246 == 0) {
                    field_0x246 = 5;
                    field_0x90 |= 1;
                } else {
                    field_0x246--;
                    field_0x90 &= ~1;
                    if (field_0x248 < 2) {
                        if (lbl_8053AAF8->mRandom.get_ufloat_1() <= field_0x23c) {
                            pos.x += 8.0;
                            field_0x23c = 0.2f;
                        } else {
                            pos.x -= 8.0;
                            field_0x23c = 0.8f;
                        }
                    } else {
                        if (lbl_8053AAF8->mRandom.get_ufloat_1() <= field_0x23c) {
                            pos.y += 8.0f;
                            field_0x23c = 0.2f;
                        } else {
                            pos.y -= 8.0f;
                            field_0x23c = 0.8f;
                        }
                    }
                }
            }
            fn_801F856C()->fn_801F9484('BALL', field_0x4, &pos, field_0x90 & 0xF80000FF, -1, -1);
        }
        if (field_0x240 <= 0) {
            field_0x240 = field_0x244 + (s32)(field_0x245 * lbl_8053AAF8->mRandom.get_ufloat_1());
        } else {
            field_0x240--;
        }
    }
}

void IronBallManager_804ABA38::init() {
    field_0xb0 = 0x2000;
    field_0x230 = mPos;
    field_0x23c = 0.5f;
    field_0x244 = (field_0x90 >> 8) & 0xFF;
    field_0x245 = (field_0x90 >> 16) & 0xFF;
    field_0x240 = (s32)(field_0x245 * lbl_8053AAF8->mRandom.get_ufloat_1());
    field_0x246 = 5;
    field_0x247 = field_0x90 & 0xF;
    field_0x248 = (field_0x90 >> 4) & 0xF;
}
