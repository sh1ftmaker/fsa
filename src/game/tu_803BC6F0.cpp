#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"
#include "game/Message.h"
#include "game/act/tu_80440BF4.h"
#include "game/tu_803BC6F0.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "dolphin/os/OSReset.h"

// The 'Pdb08'..'Pdb01' and 'Tdb01' panes of mScreen (fn_803CE11C fades
// them out with field_0x196).
static const u64 lbl_80476938[9] = {
    0x5064623038ull, 0x5064623037ull, 0x5064623036ull, 0x5064623035ull, 0x5064623034ull,
    0x5064623033ull, 0x5064623032ull, 0x5064623031ull, 0x5464623031ull,
};

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

// FIXME: 99.85 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_: the literals of the unwritten earlier functions come first).
void FileSelect_804AFF60::fn_803C6690() {
    s16 delay = lbl_8050DB0C.field_0x4c[0xC0];
    s16 frames = lbl_8050DB0C.field_0x4c[0xC1];
    s32 end = delay + frames;
    field_0xd8++;
    if (field_0xd8 > delay) {
        for (s32 i = 0; i < 3; i++) {
            field_0x38[i]->fn_80402940(frames, 0, field_0x70[i], 0.0f);
            field_0x44[i]->fn_80402940(frames, 0, field_0x64[i], 0.0f);
        }
        fn_803BC6F0(field_0xa4, 1.0f - (f32)(field_0xd8 - delay) / (f32)frames);
    }
    if (field_0xd8 > end) {
        setState(5);
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

void FileSelect_804AFF60::fn_803CBF0C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0;
            setState(0x10);
            lbl_8053AF80->fn_80458880(0xB, -1);
        } else {
            if (lbl_8053AAF8->field_0x7f != 0) {
                field_0x80 = 0x5C;
            } else {
                field_0x80 = 0x256;
            }
            setState(3);
            lbl_8053AF80->fn_80458880(0xC, -1);
        }
    }
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

void FileSelect_804AFF60::fn_803CC17C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            lbl_8051C1A0.fn_803B9DDC();
            field_0x80 = 0x54;
            setState(0xD);
            field_0x113 = 1;
            lbl_80529DEC.field_0x24c = 1;
            lbl_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0x52;
            setState(2);
            lbl_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

void FileSelect_804AFF60::fn_803CC2BC() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0x53;
            setState(0xC);
            lbl_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0x52;
            setState(2);
            lbl_8053AF80->fn_80458880(0xC, -1);
        }
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

void FileSelect_804AFF60::fn_803CC62C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            lbl_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0x59;
            setState(8);
            field_0x113 = 1;
            lbl_80529DEC.field_0x24c = 1;
            lbl_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0x58;
            setState(2);
            lbl_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

void FileSelect_804AFF60::fn_803CE05C() {
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0) {
        if (field_0x7c == 0x51) {
            field_0x80 = 0x83;
        } else if (field_0x7c == 0x83) {
            field_0x80 = 0x84;
            setState(0xF);
        } else {
            if (lbl_8053AAF8->field_0x7f != 0) {
                field_0x80 = 0x5C;
            } else {
                field_0x80 = 0x256;
            }
            setState(3);
        }
        lbl_8053AF80->fn_80458880(0xB, -1);
    }
}

// FIXME: 99.50 %. As in fn_803CE370, the bounds x and field_0x8 load into
// f1 / f0 (original f0 / f1), and the loop's counter and table pointer swap
// r29 / r30; 600.0f is still @NNN.
void FileSelect_804AFF60::fn_803CE11C() {
    u32 cardState = lbl_8051C1A0.fn_803B9940(0);
    if (field_0x20->mPane->getBounds()->i.x - field_0x20->field_0x8 < 600.0f) {
        field_0x20->fn_80402B10(lbl_8050DB0C.field_0x4c[0xC3], 0, 0.0f, 600.0f);
        field_0x20->fn_80402628(lbl_8050DB0C.field_0x4c[0xC3], field_0x20->field_0x28, 0);
        for (s32 i = 0; i < 9; i++) {
            field_0x20->fn_804023D0(mScreen->search(lbl_80476938[i]), field_0x196[i]);
        }
        field_0x117 = 0;
        return;
    }
    switch (cardState) {
    case 0:
        field_0x80 = 0x4C;
        mPrevState = mState;
        field_0x1a4 = 1;
        fn_803C24BC();
        mState = 2;
        lbl_8050EC80[0].field_0x8 = 0;
        break;
    case 8:
        field_0x80 = 0x4D;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 9:
        field_0x80 = 0x4E;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 10:
        field_0x80 = 0x4F;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 6:
    case 7:
        field_0x80 = 0x50;
        setState(0xB);
        break;
    case 11:
    case 12:
        field_0x80 = 0x51;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 2:
        lbl_8051C1A0.fn_803B9B8C();
        field_0x80 = 0;
        setState(0x11);
        break;
    case 1:
        field_0x80 = 0x57;
        setState(7);
        break;
    case 3:
    case 4:
    case 5:
    default:
        field_0x80 = 0;
        break;
    }
}

// FIXME: 99.69. The bounds x and field_0x8 load into f2 / f1 (original f1 / f2)
// and the fsubs operands swap; 600.0f is still @NNN until fn_803BC6F0 is written.
void FileSelect_804AFF60::fn_803CE370() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        field_0x80 = 0;
        u32 msg = 0x117;
        if (lbl_8053AAF8->field_0x7f != 0) {
            msg = 0x116;
        }
        field_0x84 = msg;
        fn_803832BC(((J2DTextBox*)mScreenHuuin->search(0x5466743031ull /* "Tft01" */))->getStringPtr(),
                    field_0x84, 0, NULL, NULL);
        if (field_0x84 == 0x60 || field_0x84 == 0x64) {
            field_0xb0 = 0x5A;
        }
        if (field_0x20->mPane->getBounds()->i.x - field_0x20->field_0x8 < 600.0f) {
            field_0x20->field_0x20 = 0;
            field_0x20->field_0x26 = 0;
            field_0x117 = 1;
        }
        setState(1);
        lbl_8053AF80->fn_80458880(0xB, -1);
    }
}

s32 FileSelect_804AFF60::fn_803CE4B0() {
    if (lbl_8053ADB0 != NULL) {
        return lbl_8053ADB0->field_0x1a8;
    }
    return 3;
}

// FIXME: 99.86 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_: fn_803BC6F0, unwritten, uses 0, -200, 200 and the
// conversion doubles first).
void FileSelect_804AFF60::fn_803CE9EC() {
    field_0x1a8 = 3;
    if (field_0x116 == 1) {
        lbl_8053AEC0->fn_80431174(field_0x114);
    }
    cls_803B9940* card = &lbl_8051C1A0;
    if ((card->field_0x1ff5 == 0 || card->field_0x1ff5 == 1) && !lbl_8053ACA8->fn_802B1DCC() &&
        field_0x113 == 0 && field_0x117 == 0) {
        setState(1);
    }
    if (field_0x116 < 2) {
        (lbl_8053ADB0->*lbl_804AFD70[mState])();
    }
    if (mState != field_0x111) {
        field_0x111 = mState;
        switch (mState) {
        case 0:
        case 2:
        case 5:
        case 10:
        case 14:
        case 19:
        case 24:
        case 25:
        case 36:
            field_0x1a5 = 0;
            break;
        case 3:
        case 7:
        case 11:
        case 12:
        case 15:
        case 27:
        case 37:
            field_0x1a5 = 1;
            break;
        }
    }
    if (field_0x1a5 != field_0x1a6) {
        field_0x1a4 = 2;
    } else if (field_0x1a4 == 2) {
        field_0x1a4 = 1;
    }
    switch (field_0x1a4) {
    case 1: {
        s16 frames = lbl_8050DB0C.field_0x4c[0x8A];
        field_0xde++;
        if (field_0xde < frames) {
            f32 t = (f32)(field_0xde * field_0xde) / (f32)(frames * frames);
            field_0x58->fn_80402E08(0.0f, 200.0f * t + -200.0f);
            field_0x58->mPane->setAlpha(t * field_0x58->field_0x28);
        } else {
            field_0xde = frames;
            field_0x58->fn_80402E08(0.0f, 0.0f);
            field_0x58->mPane->setAlpha(field_0x58->field_0x28);
            field_0x1a4 = 0;
        }
        break;
    }
    case 2: {
        s16 frames = lbl_8050DB0C.field_0x4c[0x8A];
        field_0xde--;
        if (field_0xde > 0) {
            f32 t = (f32)(field_0xde * field_0xde) / (f32)(frames * frames);
            field_0x58->fn_80402E08(0.0f, 200.0f * t + -200.0f);
            field_0x58->mPane->setAlpha(t * field_0x58->field_0x28);
        } else {
            field_0xde = 0;
            field_0x58->fn_80402E08(0.0f, -200.0f);
            field_0x58->mPane->setAlpha(0);
            field_0x1a6 = field_0x1a5;
            if (field_0x1a5 == 0) {
                lbl_8053AED8->fn_80440BF4(true);
            } else {
                lbl_8053AED8->fn_80440BF4(false);
            }
            field_0x1a4 = 1;
        }
        break;
    }
    }
    if (field_0x7c != field_0x80) {
        if (field_0x116 == 0) {
            field_0x7c = field_0x80;
            if (field_0x7c != 0) {
                if (field_0x7c == 0x50 || field_0x7c == 0x53 || field_0x7c == 0x57 || field_0x7c == 0x5C ||
                    field_0x7c == 0x84 || field_0x7c == 0x256) {
                    lbl_8053AEC0->fn_80431A9C(field_0x7c, 0, &field_0x114);
                    field_0x1a3 = 0;
                } else {
                    lbl_8053AEC0->fn_80431A9C(field_0x7c, 0, NULL);
                    field_0x1a3 = 0;
                }
                field_0x116 = 2;
            }
        } else if (field_0x116 == 1) {
            field_0x116 = 3;
        }
    }
    if (field_0x116 == 2) {
        if (field_0x1a3 > 0x48) {
            field_0x1a3 = 0x50;
        } else {
            field_0x1a3 += 8;
        }
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x1a3 = 0x50;
            field_0x116 = 1;
        }
    } else if (field_0x116 == 3) {
        if (field_0x1a3 < 8) {
            field_0x1a3 = 0;
        } else {
            field_0x1a3 -= 8;
        }
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x1a3 = 0;
            field_0x116 = 0;
        }
    }
    card->field_0x1ff5 = 2;
}

FileSelect_804AFF60::~FileSelect_804AFF60() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete mScreen;
    delete mScreenHuuin;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x24[i];
        field_0x24[i] = NULL;
    }
    delete field_0x34;
    field_0x34 = NULL;
    for (s32 i = 0; i < 3; i++) {
        delete field_0x38[i];
        field_0x38[i] = NULL;
        delete field_0x44[i];
        field_0x44[i] = NULL;
    }
    delete field_0x50;
    field_0x50 = NULL;
    delete field_0x54;
    field_0x54 = NULL;
    lbl_80529DEC.field_0x30->unmount();
    lbl_8053ADB0 = NULL;
}
