#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "game/tu_803BC6F0.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "dolphin/os/OSReset.h"

// Run each frame by fn_803CE9EC (index: mState).
static FileSelect_804AFF60::StateFunc lbl_804AFD70[40] = {
    &FileSelect_804AFF60::fn_803CE370, &FileSelect_804AFF60::fn_803CE11C,
    &FileSelect_804AFF60::fn_803CE05C, &FileSelect_804AFF60::fn_803CDEE8,
    &FileSelect_804AFF60::fn_803CD75C, &FileSelect_804AFF60::fn_803CD2DC,
    &FileSelect_804AFF60::fn_803CC760, &FileSelect_804AFF60::fn_803CC62C,
    &FileSelect_804AFF60::fn_803CC53C, &FileSelect_804AFF60::fn_803CC490,
    &FileSelect_804AFF60::fn_803CC3DC, &FileSelect_804AFF60::fn_803CC2BC,
    &FileSelect_804AFF60::fn_803CC17C, &FileSelect_804AFF60::fn_803CC0D8,
    &FileSelect_804AFF60::fn_803CC048, &FileSelect_804AFF60::fn_803CBF0C,
    &FileSelect_804AFF60::fn_803CBEE0, &FileSelect_804AFF60::fn_803CBE38,
    &FileSelect_804AFF60::fn_803CB3DC, &FileSelect_804AFF60::fn_803CABAC,
    &FileSelect_804AFF60::fn_803C9F80, &FileSelect_804AFF60::fn_803C9F14,
    &FileSelect_804AFF60::fn_803C9898, &FileSelect_804AFF60::fn_803C9338,
    &FileSelect_804AFF60::fn_803C92B8, &FileSelect_804AFF60::fn_803C8B80,
    &FileSelect_804AFF60::fn_803C7F7C, &FileSelect_804AFF60::fn_803C7CA0,
    &FileSelect_804AFF60::fn_803C70B8, &FileSelect_804AFF60::fn_803C6798,
    &FileSelect_804AFF60::fn_803C6690, &FileSelect_804AFF60::fn_803C61C4,
    &FileSelect_804AFF60::fn_803C5808, &FileSelect_804AFF60::fn_803C579C,
    &FileSelect_804AFF60::fn_803C5190, &FileSelect_804AFF60::fn_803C4628,
    &FileSelect_804AFF60::fn_803C4330, &FileSelect_804AFF60::fn_803C4054,
    &FileSelect_804AFF60::fn_803C3904, &FileSelect_804AFF60::fn_803C2E24,
};

void FileSelect_804AFF60::fn_803C579C() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 2) {
        field_0x84 = 0;
        field_0x80 = 0x68;
        setState(0);
    } else if (result == 1) {
        fn_803BF004();
    }
}

void FileSelect_804AFF60::fn_803C92B8() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        fn_803C0018();
        lbl_8053AF80->fn_80458880(0xB, -1);
    }
}

void FileSelect_804AFF60::fn_803C9F14() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 2) {
        field_0x84 = 0;
        field_0x80 = 0x69;
        setState(0);
    } else if (result == 1) {
        fn_803C0A34();
    }
}

void FileSelect_804AFF60::fn_803CBE38() {
    s32 result = lbl_8051C1A0.fn_803B9A94(field_0x5c, 0x258, field_0x60, 0x1D4C, 0);
    if (result != 0) {
        if (result == 1) {
            lbl_8053AAF8->field_0xff = 1;
            fn_803C24BC();
            lbl_8050EC80[0].field_0x8 = lbl_8051C1A0.field_0x1ff8;
        } else if (result == 2) {
            setState(1);
        }
    }
}

void FileSelect_804AFF60::fn_803CBEE0() {
    OSResetSystem(1, 1, 1);
}

void FileSelect_804AFF60::fn_803CC048() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        field_0x80 = 0;
        setState(1);
        lbl_8053AF80->fn_80458880(0xB, -1);
    }
}

void FileSelect_804AFF60::fn_803CC0D8() {
    s32 result = lbl_8051C1A0.fn_803B9C5C();
    if (result == 1) {
        field_0x80 = 0x55;
        setState(0xE);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0xb0 = 0x5A;
    } else if (result == 2) {
        field_0x80 = 0x56;
        setState(0);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

void FileSelect_804AFF60::fn_803CC3DC() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        field_0x80 = 0;
        lbl_8053AAF8->field_0xff = 1;
        fn_803C24BC();
        lbl_8053AF80->fn_80458880(0xB, -1);
        lbl_8050EC80[0].field_0x8 = lbl_8051C1A0.field_0x1ff8;
    }
}

void FileSelect_804AFF60::fn_803CC490() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 1) {
        field_0x80 = 0x5B;
        setState(0xA);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0xb0 = 0x5A;
    } else if (result == 2) {
        field_0x80 = 0x5A;
        fn_803CE4CC();
        setState(0);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

s32 FileSelect_804AFF60::fn_803CE4B0() {
    if (lbl_8053ADB0 != NULL) {
        return lbl_8053ADB0->field_0x1a8;
    }
    return 3;
}
