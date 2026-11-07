#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GbaClient.h"
#include "JSystem/JUtility/JUTGamePad.h"

static u8 lbl_8053AD38[8];

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

// FIXME: __sinit_tu_803099FC_cpp (compiler-generated from the GameAudio.h include, no source
// line of its own; precedent act_phantom_ganon_2.cpp) is 99.61 %: the JAudio pool records are
// 0x10 lower in our .bss than in the original, whose first 0x10 bytes (0x805076E8) belong to an
// object not identified yet (fn_8030A43C, now written, has no static).

// 0x8030AC4C
cls_804AA8C8::cls_804AA8C8() {
    lbl_8053AD30 = this;
}

// 0x8030ABFC
cls_804AA8C8::~cls_804AA8C8() {
    lbl_8053AD30 = NULL;
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

// 0x8030AAF4
s32 getPlayerInputSlot_8030AAF4(s32 player) {
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

// FIXME(shotgun): 14.5 %, missing/extra. The logic follows the asm; the low % is the register
// numbering, one higher in the target throughout (this in r22, the key word in r31; ours r21/r22),
// plus two local shapes: the target materialises mState >= 4 as a value (li 4; subi; orc; srwi;
// subf; srwi.) as main in tu_801344C0 does, where we emit cmplwi 4, and it builds the checksum
// argument as rlwinm 0xFF00 + rlwimi where we emit extrwi + srwi + rlwimi.
// 0x8030A43C
void cls_804AA8C8::fn_8030A43C() {
    u32 resetMask = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x14[i] = field_0x4[i];
        field_0x94[i] = field_0x84[i];
        field_0x84[i] = field_0x24[i];
    }
    for (s32 i = 0; i < 4; i++) {
        GbaClient_803007D0& client = lbl_804DB798[i];
        lbl_8053AD34[i] = 0;
        lbl_8053AD38[i] = 0;
        if (field_0xdc[i] < 3) {
            field_0xdc[i]++;
        }
        if (field_0xe0[i] < 3) {
            field_0xe0[i]++;
        }
        if (client.field_0xd1 != 0) {
            continue;
        }
        field_0xc4[i] = client.mState;
        field_0xd4[i] = client.field_0xd3;
        field_0xd8[i] = client.field_0xd8;
        if (field_0xc4[i] == 0x40000) {
            switch (field_0xd4[i]) {
            case 0:
                if (field_0xd8[i] == 0) {
                    OSSendMessage(&client.mQueue, (OSMessage)7, 1);
                    continue;
                }
                if (lbl_8053AAF0->mState >= 4 || lbl_8053AAF0->field_0x60 != 0) {
                    lbl_8053AD34[i] = client.field_0xd6;
                    u32 keys = client.mKeys;
                    memcpy(client.mTransfer, &lbl_80506DE8[i], lbl_80541618[lbl_8053AD08] * 4);
                    OSSendMessage(&client.mQueue, (OSMessage)10, 1);
                    if (lbl_8053AD34[i] != 0 && (u8)keys == fn_803006E8((keys & 0xFF00) | ((keys >> 16) & 0xFF))) {
                        field_0x4[i] = (((keys >> 20) & 1) ? 2 : 0) | (((keys >> 22) & 1) ? 8 : 0) | ((keys >> 21) & 1) |
                                       (((keys >> 9) & 1) ? 0x40 : 0) | (((keys >> 23) & 1) ? 4 : 0) |
                                       (((keys >> 8) & 1) ? 0x20 : 0) | (((keys >> 19) & 1) ? 0x1000 : 0) |
                                       (((keys >> 18) & 1) ? 0x400 : 0) | (((keys >> 16) & 1) ? 0x100 : 0) |
                                       (((keys >> 17) & 1) ? 0x200 : 0);
                        field_0xe6[i] = (keys >> 15) & 1;
                    }
                }
                field_0xdc[i] = 0;
                break;
            case 3:
                client.mState = 0;
                if ((u32)client.mMessage[0] == 7 && field_0xb4[i] < 0xFFFFFFFF) {
                    field_0xb4[i]++;
                }
                break;
            default:
                client.mState = 0;
                break;
            }
        } else if (field_0xc4[i] == 0) {
            field_0x4[i] = 0;
            if ((u32)client.mMessage[0] != 7) {
                field_0xb4[i] = 0;
            }
            OSSendMessage(&client.mQueue, (OSMessage)6, 1);
        } else {
            field_0xb4[i] = 0;
            if (client.field_0xde == 1) {
                client.field_0xdd = 1;
            }
            field_0x4[i] = 0;
            s32 slot = getPlayerInputSlot_8030AAF4(i);
            JUTGamePad* pad = JUTGamePad::getGamePad(slot);
            if (pad == NULL) {
                continue;
            }
            if (JUTGamePad::mPadStatus[slot].err != 0) {
                client.mState = 0;
                resetMask |= 0x80000000 >> i;
                continue;
            }
            lbl_8053AD38[i] = 1;
            if (((pad->getButton() & 1) && gGameRoot_8053AAF8->fn_80136320()) ||
                pad->getMainStickX() < -0.6f) {
                field_0x4[i] |= 1;
            }
            if (((pad->getButton() & 2) && gGameRoot_8053AAF8->fn_80136320()) ||
                pad->getMainStickX() > 0.6f) {
                field_0x4[i] |= 2;
            }
            if (((pad->getButton() & 4) && gGameRoot_8053AAF8->fn_80136320()) ||
                pad->getMainStickY() < -0.6f) {
                field_0x4[i] |= 4;
            }
            if (((pad->getButton() & 8) && gGameRoot_8053AAF8->fn_80136320()) ||
                pad->getMainStickY() > 0.6f) {
                field_0x4[i] |= 8;
            }
            if ((pad->getButton() & 0x400) || ((pad->getButton() & 1) && !gGameRoot_8053AAF8->fn_80136320()) ||
                ((pad->getButton() & 2) && !gGameRoot_8053AAF8->fn_80136320()) ||
                ((pad->getButton() & 4) && !gGameRoot_8053AAF8->fn_80136320()) ||
                ((pad->getButton() & 8) && !gGameRoot_8053AAF8->fn_80136320())) {
                field_0x4[i] |= 0x400;
            }
            if (pad->getButton() & 0x800) {
                field_0x4[i] |= 0x40;
            }
            if (field_0xe4[1] != 0) {
                field_0xe4[1] = pad->getAnalogRf() > 0.6f;
            } else {
                field_0xe4[1] = pad->getAnalogRf() > 0.7f;
            }
            if (field_0xe4[1] != 0) {
                field_0x4[i] |= 0x20;
            }
            if (field_0xe4[0] != 0) {
                field_0xe4[0] = pad->getAnalogLf() > 0.6f;
            } else {
                field_0xe4[0] = pad->getAnalogLf() > 0.7f;
            }
            if (field_0xe4[0] != 0) {
                field_0x4[i] |= 0x40;
            }
            if (pad->getButton() & 0x100) {
                field_0x4[i] |= 0x100;
            }
            if (pad->getButton() & 0x200) {
                field_0x4[i] |= 0x200;
            }
            if (pad->getButton() & 0x1000) {
                field_0x4[i] |= 0x1000;
            }
            field_0xe0[i] = 0;
        }
    }
    if (resetMask != 0) {
        PADReset(resetMask);
    }
    fn_8030A078();
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

// 0x8030A388
u8 cls_804AA8C8::fn_8030A388(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return lbl_8053AD38[slot] != 0;
}

// 0x8030A364
u8 cls_804AA8C8::fn_8030A364(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0xe0[slot] < 3;
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

// 0x8030A2E4
u8 cls_804AA8C8::fn_8030A2E4(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0xdc[slot] < 3;
}

// 0x8030A258
u8 cls_804AA8C8::fn_8030A258(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    if (field_0xc4[slot] != 0x40000) {
        return 0;
    }
    if (field_0xd4[slot] != 0) {
        return 0;
    }
    if (field_0xd8[slot] == 0) {
        return 0;
    }
    if (lbl_804DB798[slot].field_0xdc != 0) {
        return 0;
    }
    return lbl_8053ACE4[slot];
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

// 0x80309D90
f32 cls_804AA8C8::fn_80309D90(s32 slot) {
    f32 x = fn_80309F20(slot);
    f32 y = fn_80309FCC(slot);
    return JGeometry::TUtil<f32>::sqrt(y * y + x * x);
}

// 0x80309D6C
u32 cls_804AA8C8::fn_80309D6C(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x24[slot] & mask;
}

// 0x80309D48
u32 cls_804AA8C8::fn_80309D48(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x34[slot] & mask;
}

// 0x80309D24
u32 cls_804AA8C8::fn_80309D24(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x4[slot] & mask;
}

// FIXME(shotgun): 97.8 %, missing/extra
// 0x80309C4C
u32 cls_804AA8C8::fn_80309C4C(u32 mask, s32 slot) {
    s32 count = slot;
    if (count == -1) {
        count = gGameRoot_8053AAF8->fn_80138A30();
    }
    for (s32 i = 0; i < count; i++) {
        u32 pressed = fn_80309D6C(getPlayerInputSlot_8030AAF4(i), mask);
        if (pressed) {
            return pressed;
        }
    }
    return 0;
}

// FIXME(shotgun): 97.8 %, missing/extra
// 0x80309B74
u32 cls_804AA8C8::fn_80309B74(u32 mask, s32 slot) {
    s32 count = slot;
    if (count == -1) {
        count = gGameRoot_8053AAF8->fn_80138A30();
    }
    for (s32 i = 0; i < count; i++) {
        u32 pressed = fn_80309D24(getPlayerInputSlot_8030AAF4(i), mask);
        if (pressed) {
            return pressed;
        }
    }
    return 0;
}

// FIXME(shotgun): 97.8 %, missing/extra
// 0x80309A9C
u32 cls_804AA8C8::fn_80309A9C(u32 mask, s32 slot) {
    s32 count = slot;
    if (count == -1) {
        count = gGameRoot_8053AAF8->fn_80138A30();
    }
    for (s32 i = 0; i < count; i++) {
        u32 pressed = fn_80309D48(getPlayerInputSlot_8030AAF4(i), mask);
        if (pressed) {
            return pressed;
        }
    }
    return 0;
}

// 0x80309A78
u32 cls_804AA8C8::fn_80309A78(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x84[slot] & mask;
}

// 0x80309A54
u32 cls_804AA8C8::fn_80309A54(s32 slot, u32 mask) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0x94[slot] & mask;
}

// 0x80309A38
u8 cls_804AA8C8::fn_80309A38(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    return field_0xe6[slot];
}

// 0x80309A00
bool cls_804AA8C8::fn_80309A00(s32 slot) {
    if (slot == -0x3E7) {
        return false;
    }
    return field_0xb4[slot] >= 1;
}
