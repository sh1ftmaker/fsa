#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JAudio/JAUAudioMgr.h"
#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/System/JASAudioReseter.h"
#include "JSystem/JAudio/System/JASWaveArcLoader.h"
#include "JSystem/JAudio/System/JASDriver.h"

// 0x80457DFC
void Unk_8045A080::fn_80457DFC(u8 value) {
    field_0x6 = value;
}

// 0x80457E04
void Unk_8045A080::fn_80457E04(bool flag) {
    if (field_0x8c == 0 && field_0x8b == 0) {
        if (flag) {
            fn_80458744(3);
        } else {
            fn_80458744(0);
        }
    }
}

// 0x80457F60
void Unk_8045A080::fn_80457F60() {
    field_0x74 = 1;
}

// 0x80457F6C
void Unk_8045A080::fn_80457F6C() {
    field_0x74 = 0;
    field_0x6d = 0xB4;
}

// 0x80458090
void Unk_8045A080::fn_80458090() {
    field_0x44->loadWaveArc(2);
    field_0x44->loadWaveArc(3);
    field_0x44->loadWaveArc(4);
}

// 0x804580DC
bool Unk_8045A080::fn_804580DC(u8 index) {
    if (field_0x44 != NULL) {
        JASWaveBank* bank = field_0x44->getWaveBankTable().getWaveBank(index);
        if (bank != NULL && bank->getWaveArc(0)->_4c == 2) {
            return true;
        }
    }
    return false;
}

// 0x8045873C
void Unk_8045A080::fn_8045873C(u8 value) {
    field_0x69 = value;
}

// 0x80458F9C
void Unk_8045A080::fn_80458F9C(s32 frames) {
    if (field_0x80 != NULL) {
        field_0x80->stop(frames);
    }
    field_0x68 = 0;
    field_0x64 = -1;
    field_0x58 = 0;
    field_0x5c = 0;
    field_0x60 = 0;
}

// 0x804594E4
void Unk_8045A080::fn_804594E4(u32 mode) {
    JASDriver::setOutputMode(mode);
}

// 0x8045959C
int Unk_8045A080::fn_8045959C() {
    if (field_0x7c == 0) {
        return 0;
    }
    return (field_0x2c->checkDone() != 0) + 1;
}

// 0x804595E4
void Unk_8045A080::fn_804595E4(bool flag) {
    field_0x2c->start(5, flag);
    field_0x7c = 1;
}

// 0x80459624
// FIXME: 79.35. The original loads and stores the three coordinates one at a time
// (lfs/stfs through f0, the mtx pointer moved in between); ours loads x and z
// (f0, f1) before the first store and the register assignment follows.
void Unk_8045A080::fn_80459624(const JGeometry::TVec3f* pos, Mtx mtx, s32 index) {
    field_0x8.set(pos->x, pos->z, pos->y);
    field_0x3c->setAudienceMtx(mtx, index, false);
}

// 0x8045A080
Unk_8045A080::Unk_8045A080() {
    field_0x80 = NULL;
    field_0x84 = NULL;
    lbl_8053AF80 = this;
    field_0x30 = NULL;
    field_0x34 = NULL;
    field_0x38 = NULL;
    field_0x0 = 0;
    field_0x6b = 0;
    field_0x70 = 0;
    field_0x6c = 0;
    field_0x6d = 0;
    field_0x74 = 0;
    field_0x4 = 0;
    field_0x5 = 1;
    field_0x6 = 0;
    field_0x8.set(0.0f, 0.0f, 0.0f);
}

// 0x8045A0D8
u8 fn_8045A0D8() {
    return lbl_8053AAF8->field_0x96;
}
