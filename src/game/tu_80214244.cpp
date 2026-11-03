#include "game/GameAudio.h"
#include "game/RailBase.h"

class cls_80214244 {
public:
    cls_800FE3EC* fn_80214244();
    void fn_80214254(u8);
    void fn_8021425C(u8, u8, const JGeometry::TVec3f&, int);
    void fn_802142A8();

    /* 0x000 */ cls_800FE3EC field_0x0[2];
    /* 0x190 */ u8 field_0x190;
};

cls_800FE3EC* cls_80214244::fn_80214244() {
    return &field_0x0[field_0x190];
}

void cls_80214244::fn_80214254(u8 index) {
    field_0x190 = index;
}

void cls_80214244::fn_8021425C(u8 index, u8 arg1, const JGeometry::TVec3f& pos, int arg3) {
    field_0x0[index].fn_800FE3EC(arg1, pos, arg3);
}

void cls_80214244::fn_802142A8() {
    field_0x190 = 0;
    for (int i = 0; i < 2; i++) {
        field_0x0[i].clear();
    }
}
