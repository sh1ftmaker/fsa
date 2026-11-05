#include "game/GameAudio.h"
#include "game/GameGlobals.h"

static u8 lbl_8053AD38[8];

// 0x80309A00
bool cls_804AA8C8::fn_80309A00(s32 slot) {
    if (slot == -0x3E7) {
        return false;
    }
    return field_0xb4[slot] >= 1;
}

// 0x80309A38
u8 cls_804AA8C8::fn_80309A38(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0xe6[slot];
}

// 0x80309A54
u32 cls_804AA8C8::fn_80309A54(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x94[slot] & mask;
}

// 0x80309A78
u32 cls_804AA8C8::fn_80309A78(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x84[slot] & mask;
}

// 0x80309D24
u32 cls_804AA8C8::fn_80309D24(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x4[slot] & mask;
}

// 0x80309D48
u32 cls_804AA8C8::fn_80309D48(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x34[slot] & mask;
}

// 0x80309D6C
u32 cls_804AA8C8::fn_80309D6C(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x24[slot] & mask;
}

// 0x80309F20
f32 cls_804AA8C8::fn_80309F20(s32 slot) {
    if (slot == -0x3E7) {
        return 0.0f;
    }
    f32 x = fn_80309D24(slot, 4) ? -1.0f : 0.0f;
    x += fn_80309D24(slot, 8) ? 1.0f : 0.0f;
    if (fn_80309D24(slot, 3)) {
        x *= 0.70710677f;
    }
    return x;
}

// 0x80309FCC
f32 cls_804AA8C8::fn_80309FCC(s32 slot) {
    if (slot == -0x3E7) {
        return 0.0f;
    }
    f32 y = fn_80309D24(slot, 1) ? -1.0f : 0.0f;
    y += fn_80309D24(slot, 2) ? 1.0f : 0.0f;
    if (fn_80309D24(slot, 0xC)) {
        y *= 0.70710677f;
    }
    return y;
}

// Key repeat of one direction bit: repeats after 20 frames, then every 10.
static inline void updateRepeat(const u32& held, const u32& pressed, u32& repeat, s32& counter, u32 bit) {
    if (held & bit) {
        if (pressed & bit) {
            repeat |= bit;
            counter = 0x14;
        } else {
            counter--;
            if (counter <= 0) {
                repeat |= bit;
                counter = 0xA;
            }
        }
    } else {
        counter = 0;
    }
}

// 0x8030A078
void cls_804AA8C8::fn_8030A078() {
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i] = field_0x4[i] & (field_0x4[i] ^ field_0x14[i]);
        field_0xa4[i] = field_0x14[i] & (field_0x4[i] ^ field_0x14[i]);
        field_0x34[i] = 0;
        updateRepeat(field_0x4[i], field_0x24[i], field_0x34[i], field_0x44[0][i], 1);
        updateRepeat(field_0x4[i], field_0x24[i], field_0x34[i], field_0x44[1][i], 2);
        updateRepeat(field_0x4[i], field_0x24[i], field_0x34[i], field_0x44[2][i], 4);
        updateRepeat(field_0x4[i], field_0x24[i], field_0x34[i], field_0x44[3][i], 8);
    }
}

// 0x8030A2E4
u8 cls_804AA8C8::fn_8030A2E4(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0xdc[slot] < 3;
}

// 0x8030A308
u8 cls_804AA8C8::fn_8030A308(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    if (field_0xc4[slot] != 0x40000) {
        return 0;
    }
    if (field_0xd4[slot] != 0) {
        return 0;
    }
    return field_0xd8[slot] == 1;
}

// 0x8030A364
u8 cls_804AA8C8::fn_8030A364(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0xe0[slot] < 3;
}

// 0x8030A388
u8 cls_804AA8C8::fn_8030A388(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return lbl_8053AD38[slot] != 0;
}

// FIXME: 87.14. The original's inlined fn_8030A308 ends in `if (byte == 1) result = 1 else 0`
// (cmplwi 1 and branches); ours inlines the call's `== 1` as subfic/cntlzw/extrwi and
// converts the byte result with neg/or/srwi.
// 0x8030A3B0
bool cls_804AA8C8::fn_8030A3B0(s32 slot) {
    if (slot == -0x3E7) {
        return false;
    }
    return fn_8030A308(slot) || lbl_8053AD38[slot] != 0;
}

// 0x8030AAF4
s32 fn_8030AAF4(s32 player) {
    if (player == 0) {
        return 0;
    }
    if (player == 1) {
        return 1;
    }
    if (player == 2) {
        return 2;
    }
    if (player == 3) {
        return 3;
    }
    return -0x3E7;
}

// 0x8030AB38
void cls_804AA8C8::fn_8030AB38() {
    for (s32 i = 0; i < 4; i++) {
        field_0x4[i] = 0;
        field_0x14[i] = 0;
        field_0x24[i] = 0;
        field_0x34[i] = 0;
        field_0x84[i] = 0;
        field_0x94[i] = 0;
        field_0xa4[i] = 0;
        lbl_8053AD38[i] = 0;
        lbl_8053AD34[i] = 0;
        field_0xc4[i] = 0;
        field_0xd4[i] = 3;
        field_0xd8[i] = 0;
        field_0xdc[i] = 0;
        field_0xe0[i] = 0;
        field_0xb4[i] = 0;
    }
    field_0xe4[0] = 0;
    field_0xe4[1] = 0;
}

// 0x8030ABFC
cls_804AA8C8::~cls_804AA8C8() {
    lbl_8053AD30 = NULL;
}

// 0x8030AC4C
cls_804AA8C8::cls_804AA8C8() {
    lbl_8053AD30 = this;
}

// FIXME: __sinit_tu_803099FC_cpp (compiler-generated from the GameAudio.h include, no source
// line of its own; precedent act_phantom_ganon_2.cpp) is 99.61 %: the JAudio pool records are
// 0x10 lower in our .bss than in the original, whose first 0x10 bytes (0x805076E8) belong to a
// static of the unwritten fn_8030A43C.
//
// Not written yet: fn_80309A9C / fn_80309B74 / fn_80309C4C (each inlines fn_8030AAF4 and a
// later-defined accessor; as calls they come out at 44 %, they need -inline deferred),
// fn_8030A258 (reads lbl_804DB798 / lbl_8053ACE4 of other units), fn_8030A43C (0x6B8 pad poll),
// fn_80309D90 (inlines fn_80309F20 and fn_80309FCC, defined after it).
