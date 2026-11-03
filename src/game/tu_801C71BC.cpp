#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_shadow_link_2.h"

void ShadowLink2_804A43E8::fn_801C8BC4(u8 flag) {
    if (flag != 0) {
        u8 buf[0x30];
        s32 result = field_0x358.fn_80214244()->fn_800FD894(field_0x4, buf, 0x30);
        if (result <= 3) {
            field_0x55c[1] = 1;
        } else {
            field_0x55c[1] = fn_801CD664(4) + 1;
            if (field_0x524 & 0x100) {
                field_0x55c[1]++;
            }
            u8 cap = result - 2;
            u8 cur = field_0x55c[1];
            if (cap >= cur) {
                cap = cur;
            }
            field_0x55c[1] = cap;
        }
    } else {
        field_0x55c[1] = fn_801CD664(0xC) + 1;
    }
}
