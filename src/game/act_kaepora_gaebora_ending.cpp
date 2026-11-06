#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/act_kaepora_gaebora_ending.h"


// One step of the sequence: a negative value ends the sequence and restarts it.
struct KaeporaStep {
    s32 mValue;
    s32 mCount;
};

// FIXME: the original object is 0x4C bytes; its last 0x14 bytes (after the
// terminator) are not written yet, as no code here reads them.
static const KaeporaStep lbl_804742A8[] = {
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
    {-1, -1},
};

// The update() steps, indexed by mPtmfIndex; built at static init time.
void (KaeporaGaeboraEnding_804ADAC8::*lbl_804ADA50[6])() = {
    &KaeporaGaeboraEnding_804ADAC8::fn_8036003C, &KaeporaGaeboraEnding_804ADAC8::fn_8035FF70, &KaeporaGaeboraEnding_804ADAC8::fn_8035FD6C,
    &KaeporaGaeboraEnding_804ADAC8::fn_8035FCD8, &KaeporaGaeboraEnding_804ADAC8::fn_8035FB64, &KaeporaGaeboraEnding_804ADAC8::fn_8035F9C8,
};

void KaeporaGaeboraEnding_804ADAC8::update() {
    if (field_0x280 != 0) {
        field_0x280--;
    }
    if (field_0x234 != 0) {
        field_0x234--;
    }
    while (fn_8035F3C0()) {
    }
    (this->*lbl_804ADA50[mPtmfIndex])();
    if (field_0x2B0 == 0) {
        if (field_0x2B4 != 0) {
            setEmitterPos_8013CB44(field_0x2B4, &mPos, -2, -2, true);
        }
        if (field_0x2B8 != 0) {
            setEmitterPos_8013CB44(field_0x2B8, &mPos, -2, -2, true);
        }
    } else {
        if (field_0x2B4 != 0) {
            setEmitterPos_8013CB44(field_0x2B4, &mPos, mHeight, mDepth, true);
        }
        if (field_0x2B8 != 0) {
            setEmitterPos_8013CB44(field_0x2B8, &mPos, mHeight, mDepth, true);
        }
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_8035FF70() {
    if (field_0x2B0 == 1) {
        if (field_0x270 == 0) {
            mSway.fn_8021A570(true);
        } else if (field_0x234 == 0) {
            if (lbl_804742A8[mStep].mValue < 0) {
                mStep = 0;
            }
            field_0x234 = lbl_804742A8[mStep].mCount;
            field_0x23C = lbl_804742A8[mStep].mValue;
            mStep++;
        }
    }
    if (field_0x280 == 0) {
        fn_80360914();
    }
}

KaeporaGaeboraEnding_804ADAC8::~KaeporaGaeboraEnding_804ADAC8() {}
