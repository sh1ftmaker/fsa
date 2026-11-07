#include "game/GameAudio.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
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

void KaeporaGaeboraEnding_804ADAC8::fn_803613D0() {
    if (field_0x2B4 != NULL) {
        field_0x2B4->becomeInvalidEmitter();
        field_0x2B4 = NULL;
    }
    if (field_0x2B8 != NULL) {
        field_0x2B8->becomeInvalidEmitter();
        field_0x2B8 = NULL;
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

// The step tables (lbl_804ADA98, 8 of them) are in this unit's data.
extern const KaeporaStep* lbl_804ADA98[8];

// Counts the current step down and loads the next one from the table
// lbl_804ADA98[field_0x244]; true at the table's terminator.
inline bool KaeporaGaeboraEnding_804ADAC8::advanceStep() {
    s32 idx = field_0x244;
    if (field_0x234 == 0) {
        const KaeporaStep* table = lbl_804ADA98[idx];
        if (table[mStep].mValue < 0) {
            return true;
        }
        field_0x234 = table[mStep].mCount;
        field_0x23C = table[mStep].mValue;
        mStep++;
    }
    return false;
}

// The head directions (lbl_80474020, 4 of them) are in this unit's data.
extern const s32 lbl_80474020[4];

// FIXME(shotgun): 98.3 %, scheduling+regalloc
void KaeporaGaeboraEnding_804ADAC8::fn_8035FD6C() {
    if (advanceStep() == true) {
        mPtmfIndex = 0;
        field_0x27C = 0;
        field_0x254 = (s32)(JMath::atanTable_.atan2_(field_0x28C - mPos.y, field_0x288 - mPos.x) * 10430.378f);
        field_0x3c.x = field_0x268 * JMath::sincosTable_.cosShort(field_0x254);
        field_0x3c.y = field_0x268 * JMath::sincosTable_.sinShort(field_0x254);
        u16 a = field_0x254;
        if (a >= 0x2000 && a < 0x6000) {
            field_0x274 = 3;
        } else if (a >= 0x6000 && a < 0xa000) {
            field_0x274 = 1;
        } else if (a >= 0xa000 && a < 0xe000) {
            field_0x274 = 2;
        } else {
            field_0x274 = 0;
        }
        field_0x244 = lbl_80474020[field_0x274];
        field_0x234 = 0;
        mStep = 0;
        advanceStep();
    }
}

void KaeporaGaeboraEnding_804ADAC8::fn_8035FCD8() {
    if (advanceStep() == true) {
        fn_80360914();
    }
}

// FIXME(shotgun): 97.9 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_8035FB64() {
    if (field_0x234 == 0) {
        if (lbl_804742A8[mStep].mValue < 0) {
            mStep = 0;
        }
        field_0x234 = lbl_804742A8[mStep].mCount;
        field_0x23C = lbl_804742A8[mStep].mValue;
        mStep++;
    }
    if (field_0x26C < 16.0f) {
        field_0x26C += 0.2f;
    }
    s16 prev = field_0x254;
    s32 step = (s32)(65536.0f * (1.75f / (6.283184f * field_0x26C)));
    field_0x254 = prev - step;
    field_0x3c.x = 1.75f * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = 1.75f * JMath::sincosTable_.sinShort(field_0x254);
    field_0x3c.z = 0.2f;
    mPos += field_0x3c;
    field_0x264 += (s16)step;
    if (field_0x26C >= 16.0f) {
        fn_80360914();
        field_0x3c.z = 0.0f;
    }
}

// FIXME(shotgun): 98.1 %, missing/extra
void KaeporaGaeboraEnding_804ADAC8::fn_8035F9C8() {
    if (field_0x234 == 0) {
        if (lbl_804742A8[mStep].mValue < 0) {
            mStep = 0;
        }
        field_0x234 = lbl_804742A8[mStep].mCount;
        field_0x23C = lbl_804742A8[mStep].mValue;
        mStep++;
    }
    if (field_0x26C > 3.0f) {
        field_0x26C -= 0.2f;
    }
    s16 prev = field_0x254;
    s32 step = (s32)(65536.0f * (1.3f / (6.283184f * field_0x26C)));
    field_0x254 = prev - step;
    field_0x3c.x = 1.3f * JMath::sincosTable_.cosShort(field_0x254);
    field_0x3c.y = 1.3f * JMath::sincosTable_.sinShort(field_0x254);
    field_0x3c.z = -0.2f;
    mPos += field_0x3c;
    field_0x264 += (s16)step;
    if (field_0x26C < 3.0f) {
        fn_803613D0();
        mAlive = false;
    }
}

KaeporaGaeboraEnding_804ADAC8::~KaeporaGaeboraEnding_804ADAC8() {}
