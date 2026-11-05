#include "game/GameAudio.h"
#include "JSystem/JMath/JMath.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/act_popo.h"

Popo_804A0868::~Popo_804A0868() {}

void Popo_804A0868::draw() {
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x24c + 0x99, NULL, 0, 1.0f, 1.0f);
}

// FIXME: 98.7%. regalloc only: the three TRandom_fast_ draws in the drift
// block keep the multiplier 0x19660D in r5 and the working value in r0 in
// the original; here the multiplier is r0 and the value r3 (and the sincos
// table base is r5 instead of r4).
void Popo_804A0868::update() {
    if (field_0x90 == 1) {
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
            if (field_0x3c.z < -10.0f) {
                field_0x3c.z = -10.0f;
            }
            mPos += field_0x3c;
            return;
        }
        mPos.z = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        field_0xb0 = 0x11101;
        field_0x90 = 0;
    }
    if (fn_801F3D94(1) != 0 || fn_801F305C()) {
        fn_801F2618(NULL, false);
        return;
    }
    addCalc2_801F71A4(&field_0x3c.x, field_0x250, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x254, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);
    fn_801F2618(NULL, false);
    switch (field_0x230) {
    case 0:
        if (mTimers[1] == 0) {
            mTimers[1] = (s32)(10.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 10;
            field_0x240++;
            field_0x240 &= 1;
            if (field_0x240 == 0) {
                field_0x250 = 0.5f * JMASinShort((s32)(65536.0f * lbl_8053AAF8->mRandom.get_ufloat_1()));
                field_0x254 = 0.5f * JMACosShort((s32)(65536.0f * lbl_8053AAF8->mRandom.get_ufloat_1()));
            } else {
                field_0x250 = 0.0f;
                field_0x254 = 0.0f;
                field_0x258 = 0.0f;
            }
        }
        break;
    }
    if (mTimers[0] == 0) {
        field_0x24c++;
        field_0x24c &= 3;
        mTimers[0] = 8;
    }
    for (s32 i = 0; i < 3; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

// FIXME: 94.7%. scheduling+regalloc: the original starts with li r6, 0 (the
// zero for the counters) and keeps lis 0x19 / 0x660D for the RNG multiplier
// in r4/r5 after the 0x10000 constant; here the RNG multiplier is built
// first (r4 -> r6) and the zero lands in r7.
void Popo_804A0868::init() {
    field_0x90 = field_0x90 & 0xFF;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    field_0x24c = 0;
    field_0x250 = 0.0f;
    field_0x254 = 0.0f;
    field_0x258 = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0xb0 = 0x11101;
    field_0x240 = (s32)(1.99f * lbl_8053AAF8->mRandom.get_ufloat_1());
    mTimers[1] = (s32)(10.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 10;
    if (field_0x90 == 1) {
        field_0xb0 = 0;
    }
    field_0x230 = 0;
}
