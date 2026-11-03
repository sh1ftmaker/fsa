#include "game/GameAudio.h"
#include "game/act/act_shadow_link_2.h"

void ShadowLink2_804A43E8::fn_8021CDE4(int state) {
    mLife = mLifeBackup;
    field_0x568 = lbl_8046A748[state][0];
    switch (state) {
    case 1: {
        JGeometry::TVec3f rail;
        if (fn_801CD2F8(field_0x4)) {
            field_0x58c = 2;
            JGeometry::TVec3f pos(field_0x570.x, 24.0f + field_0x570.y, 0.0f);
            fn_801CD150(&pos, &field_0x580);
        } else {
            field_0x58c = 0;
            fn_801CD150(&mPos, &field_0x580);
        }
        field_0x57c = 8;
        field_0x56c = field_0x4;
        field_0x570.set(mPos);
        field_0x358.fn_8021425C(1, (field_0x90 >> 12) & 0xF, field_0x580, field_0x57c);
        field_0x358.fn_80214254(1);
        fn_800FE294(&rail, field_0x358.fn_80214244(), 0xFF);
        field_0x32c.set(rail);
        fn_801C3B7C(0x19);
        break;
    }
    case 2:
        if (field_0x564 == 0) {
            fn_801C3B7C(0x2B);
        } else if (field_0x564 == 1) {
            field_0x58c = 1;
            field_0x57c = 0;
            fn_801CD0D0(&mPos, &field_0x580);
            field_0x56c = field_0x4;
            field_0x570.set(mPos);
            field_0x358.fn_80214254(0);
            fn_801C3B7C(0x1A);
        } else if (field_0x564 == 3) {
            field_0x568 = 1;
            if (mState != 0x2A) {
                fn_801C3B7C(0x1B);
            }
        }
        break;
    case 3:
        field_0x59e = 0x258;
        fn_801C3B7C(0x2E);
        break;
    }
    field_0x564 = state;
}

bool ShadowLink2_804A43E8::fn_8021CFF8() {
    bool result = false;
    if (field_0x564 == 3 && field_0x59e == 0) {
        result = true;
    }
    return result;
}

bool ShadowLink2_804A43E8::fn_8021D020() {
    bool result = false;
    if (mLife <= mLifeBackup - field_0x568) {
        result = true;
    }
    return result;
}
