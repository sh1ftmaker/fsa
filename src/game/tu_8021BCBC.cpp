#include "game/GameAudio.h"

static const int lbl_8046A678[4] = {0, 1, 2, 3};

class cls_8021BCBC {
public:
    void fn_8021BCBC();
    void fn_8021BD18(int);

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

void cls_8021BCBC::fn_8021BCBC() {
    field_0x8++;
    if (field_0x8 >= 4) {
        field_0x8 = 0;
        field_0x4++;
        if (field_0x4 >= 4) {
            field_0x4 = 0;
        }
    }
    field_0x0 = lbl_8046A678[field_0x4] + 0x3B5;
}

// FIXME: one instruction off. The original keeps the 0 that clears field_0x8
// in r0 for case 1 and the default, so they branch straight to the store,
// with no second `li r0, 0` and no `li r0, 0` blocks of their own. No
// natural form tried reproduces that (see the checkpoint).
void cls_8021BCBC::fn_8021BD18(int arg) {
    u32 index = 0;
    field_0x8 = 0;
    switch (arg) {
    case 0:
        index = 2;
        break;
    case 1:
        index = 0;
        break;
    case 2:
        index = 1;
        break;
    case 3:
        index = 3;
        break;
    }
    field_0x4 = index;
    field_0x0 = lbl_8046A678[field_0x4] + 0x3B5;
}
