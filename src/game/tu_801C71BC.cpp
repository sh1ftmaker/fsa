#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_shadow_link_2.h"

void ShadowLink2_804A43E8::fn_801C79A0() {
    JGeometry::TVec3f railPos;

    mFlags &= ~0x02000000;
    fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
    // Copies through set() where the original copies field by field;
    // operator= (the psq pairs) gives other code.
    field_0x32c.set(railPos);
    field_0x56c = field_0x4;
    field_0x570.set(mPos);
    field_0x4 = field_0x358.fn_80214244()->fn_800FE25C(0xFF);
    mPos = field_0x32c;
    mPrevPos = field_0x32c;
    field_0x57c = field_0x4;
    field_0x580.set(mPos);
    if (field_0x358.fn_80214244()->fn_800FD594(0xFF)) {
        mFlags |= 0x20000;
    } else {
        mFlags &= ~0x20000;
    }

    s32 dir = 1;
    if (field_0x358.fn_80214244()->field_0x1 != 0) {
        dir = -1;
    }
    field_0x358.fn_80214244()->fn_800FD818(dir);
    fn_801C8C84(0, 1);
    fn_801C8BC4(1);

    if (field_0x524 & 0x4) {
        field_0x4 = field_0x56c;
        mPos.set(field_0x570);
        if (fn_801CD2F8(field_0x56c)) {
            field_0x58c = 1;
        } else {
            field_0x58c = 0;
        }
        fn_801C3B7C(0x18);
    } else {
        fn_801C3B7C(0x17);
    }
}

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
