#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_vaati_boss.h"

// Shared constants (declared in GameGlobals.h): game/tu_801EDBA4.cpp passes the first to
// fn_802F5FF4 and multiplies -12.0f by the second at run time. No function of this unit
// reads them. They lead this unit's .sdata2 (0x8053F0B8, before 0x8053F0C0 / 0x8053F0C4
// and the function literals): under deferred codegen file-scope constants are written
// before every literal of the file, so they cannot end game/tu_801D3CBC.cpp's pool.
const f32 lbl_8053F0B8 = 90.0f;
const f32 lbl_8053F0BC = 1.7f;
const s32 lbl_8053F0C0 = 12;
const u8 lbl_8053F0C4 = 0x1E;

cls_801D6A88::cls_801D6A88() {
    field_0xc.fn_8021BD18(2);
    fn_801D789C();
}

cls_801D6A58::cls_801D6A58() {
    fn_801D7874();
}

cls_801E6AB8::~cls_801E6AB8() {}

VaatiBoss_804A4958::VaatiBoss_804A4958() {
    field_0x5b0 = -1;
    field_0x5b4 = 0;
    field_0x5b8 = 0;
    field_0x5bc = 0.0f;
    field_0x5c0 = 0.0f;
    field_0x5c4 = 0.0f;
    field_0x5c8 = 0.0f;
    field_0x5cc = 0;
    field_0x5cd = 0;
    field_0x5ce = 0;
    field_0x5cf = 0;
    field_0x5d0 = 0;
    field_0x5d1 = 0;
    field_0x5d2 = 0;
    field_0x5d4 = 0;
    field_0x5d8 = 0;
    field_0x5dc = 0;
    field_0x5e0 = 0.0f;
    field_0x5e4 = 0.0f;
    field_0x5e8 = 0;
    field_0x5e9 = 0;
    field_0x5ec = 0.0f;
    field_0x5f0 = 0.0f;
    field_0x5f4 = 0.0f;
    field_0x5f8 = 0.0f;
    field_0x5fc = 0;
    field_0x600.set(0.0f, 0.0f, 0.0f);
    field_0x60c.set(0.0f, 0.0f, 0.0f);

    int i = 0;
    for (; i < 4; i++) {
        field_0x2a8[i].fn_801D789C();
    }

    i = 0;
    for (; i < 16; i++) {
        field_0x308[i].fn_801D7874();
    }

    field_0x448.mOwner = this;
}

void VaatiBoss_804A4958::vfunc_0x54() {
    gAudioMgr_8053AF80->fn_80458F9C(60);
}

void VaatiBoss_804A4958::vfunc_0x94() {
    switch (mState) {
    case 6:
    case 7:
    case 0xB:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        break;
    default:
        mAnim.fn_801D1C18();
        break;
    }
}

void VaatiBoss_804A4958::vfunc_0x24() {}

VaatiBoss_804A4958::~VaatiBoss_804A4958() {}
