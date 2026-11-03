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

// Maps the argument of fn_8021BD18 to a table index. An inline helper: the
// original's dead `b` after case 0 and the shared zero (the one stored to
// field_0x8 doubles as this function's result for case 1 and the default)
// are what MWCC leaves after inlining it.
static inline u32 argToIndex(int arg) {
    u32 index = 0;
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
    return index;
}

void cls_8021BCBC::fn_8021BD18(int arg) {
    field_0x8 = 0;
    field_0x4 = argToIndex(arg);
    field_0x0 = lbl_8046A678[field_0x4] + 0x3B5;
}
