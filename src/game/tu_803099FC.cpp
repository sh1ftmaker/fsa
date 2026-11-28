#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GbaClient.h"
#include "JSystem/JUtility/JUTGamePad.h"

static u8 lbl_8053AD38[8];
// 0x805076E8: 0x10 bytes at the start of this unit's .bss, not referenced by any code.
static u8 lbl_805076E8[0x10];

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

// __sinit_tu_803099FC_cpp is compiler-generated from the GameAudio.h include (no source
// line of its own; precedent act_phantom_ganon_2.cpp).

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
    field_0xe4 = 0;
    field_0xe5 = 0;
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

// FIXME(shotgun): 99.93 %, regalloc-only; behaviour clean.
// What still differs: register allocation in the key-bit expression only.
// 0x8030A43C
void cls_804AA8C8::pollInputs_8030A43C() {
    s32 i;
    for (i = 0; i < 4; i++) {
        field_0x14[i] = field_0x4[i];
        field_0x94[i] = field_0x84[i];
        field_0x84[i] = field_0x24[i];
    }
    u32 resetMask = 0;
    for (i = 0; i < 4; i++) {
        lbl_8053AD34[i] = 0;
        lbl_8053AD38[i] = 0;
        if (field_0xdc[i] < 3) {
            field_0xdc[i]++;
        }
        if (field_0xe0[i] < 3) {
            field_0xe0[i]++;
        }
        GbaClient_803007D0& client = lbl_804DB798[i];
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
                if (lbl_8053AAF0->isLoaded() || lbl_8053AAF0->field_0x60 != 0) {
                    lbl_8053AD34[i] = client.field_0xd6;
                    u32 keys = client.mKeys;
                    memcpy(client.mTransfer, &lbl_80506DE8[i], lbl_80541618[lbl_8053AD08] * 4);
                    OSSendMessage(&client.mQueue, (OSMessage)10, 1);
                    if (lbl_8053AD34[i] != 0 && (keys & 0xFF) == fn_803006E8(((keys & 0xFF0000) >> 16) | (keys & 0xFF00))) {
                        field_0x4[i] = (((keys >> 22) & 1) ? 8 : 0) | ((((keys >> 20) & 1) ? 2 : 0) | ((keys >> 21) & 1)) |
                                       (((keys >> 23) & 1) ? 4 : 0) | (((keys >> 9) & 1) ? 0x40 : 0) |
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
            switch (JUTGamePad::mPadStatus[slot].err) {
            case 0:
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
                if (field_0xe5 != 0) {
                    if (pad->getAnalogRf() > 0.6f) {
                        field_0xe5 = 1;
                    } else {
                        field_0xe5 = 0;
                    }
                } else {
                    if (pad->getAnalogRf() > 0.7f) {
                        field_0xe5 = 1;
                    } else {
                        field_0xe5 = 0;
                    }
                }
                if (field_0xe5 != 0) {
                    field_0x4[i] |= 0x20;
                }
                if (field_0xe4 != 0) {
                    if (pad->getAnalogLf() > 0.6f) {
                        field_0xe4 = 1;
                    } else {
                        field_0xe4 = 0;
                    }
                } else {
                    if (pad->getAnalogLf() > 0.7f) {
                        field_0xe4 = 1;
                    } else {
                        field_0xe4 = 0;
                    }
                }
                if (field_0xe4 != 0) {
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
                break;
            case -1:
            default:
                client.mState = 0;
                resetMask |= 0x80000000 >> i;
                break;
            }
        }
    }
    if (resetMask != 0) {
        PADReset(resetMask);
    }
    fn_8030A078();
}

u8 cls_804AA8C8::fn_8030A3B0(s32 slot) {
    if (slot == -0x3E7) {
        return 0;
    }
    u8 result = fn_8030A308(slot);
    if (result) {
        return result;
    }
    return lbl_8053AD38[slot] != 0;
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
    if (field_0xd8[slot] == 1) {
        return 1;
    }
    return 0;
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

u32 cls_804AA8C8::fn_80309C4C(u32 mask, s32 slot) {
    s32 count;
    if (slot == -1) {
        count = gGameRoot_8053AAF8->getPlayerCount_80138A30();
    } else {
        count = slot;
    }
    for (s32 i = 0; i < count; i++) {
        u32 pressed = fn_80309D6C(getPlayerInputSlot_8030AAF4(i), mask);
        if (pressed) {
            return pressed;
        }
    }
    return 0;
}

u32 cls_804AA8C8::fn_80309B74(u32 mask, s32 slot) {
    s32 count;
    if (slot == -1) {
        count = gGameRoot_8053AAF8->getPlayerCount_80138A30();
    } else {
        count = slot;
    }
    for (s32 i = 0; i < count; i++) {
        u32 pressed = fn_80309D24(getPlayerInputSlot_8030AAF4(i), mask);
        if (pressed) {
            return pressed;
        }
    }
    return 0;
}

u32 cls_804AA8C8::fn_80309A9C(u32 mask, s32 slot) {
    s32 count;
    if (slot == -1) {
        count = gGameRoot_8053AAF8->getPlayerCount_80138A30();
    } else {
        count = slot;
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
