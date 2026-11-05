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

// .rodata: the pane tag tables (J2DPane::search), in address order; most
// functions address them off one base (lbl_804765D0).
static const u64 lbl_804765D0[3] = {
    'Tf1lv', 'Tf2lv', 'Tf3lv',
};
static const u64 lbl_804765E8[3] = {
    'Tf1fir', 'Tf2fir', 'Tf3fir',
};
static const u64 lbl_80476600[3] = {
    'Tf1date', 'Tf2date', 'Tf3date',
};
static const u64 lbl_80476618[81] = {
    'Pabb10', 'Pabb09', 'Pabb08',
    'Pabb07', 'Pabb06', 'Pabb05',
    'Pabb04', 'Pabb03', 'Pabb02',
    'Pabb01', 'Wbat01', 'Pbatc10',
    'Pbatc09', 'Pbatc08', 'Pbatc07',
    'Pbatc06', 'Pbatc05', 'Pbatc04',
    'Pbatc03', 'Pbatc02', 'Pbatc01',
    'Pqb10', 'Pqb09', 'Pqb08',
    'Pqb07', 'Pqb06', 'Pqb05',
    'Pqb04', 'Pqb03', 'Pqb02',
    'Pqb01', 'Pbatb09', 'Pbatb08',
    'Pbatb07', 'Pbatb06', 'Pbatb05',
    'Pbatb04', 'Pbatb03', 'Pbatb02',
    'Pbatb01', 'Tbat01', 'Wadv01',
    'Padhu03', 'Padhu02', 'Padvl02',
    'Padvl01', 'Padvw72', 'Padvw71',
    'Padvw70', 'Padvw62', 'Padvw61',
    'Padvw60', 'Padvw52', 'Padvw51',
    'Padvw50', 'Padvw42', 'Padvw41',
    'Padvw40', 'Padvw32', 'Padvw31',
    'Padvw30', 'Padvw22', 'Padvw21',
    'Padvw20', 'Padvw12', 'Padvw11',
    'Padvw10', 'Padvw74', 'Padvw73',
    'Padvw64', 'Padvw63', 'Padvw54',
    'Padvw53', 'Padvw44', 'Padvw43',
    'Padvw34', 'Padvw33', 'Padvw24',
    'Padvw23', 'Padvw14', 'Padvw13',
};
static const u64 lbl_804768A0[19] = {
    'Padhu01', 'Padvb09', 'Padvb08',
    'Padvb07', 'Padvb06', 'Padvb05',
    'Padvb04', 'Padvb03', 'Padvb02',
    'Padvb01', 'Tadv01', 'Pbli01',
    'Pbli02', 'Pbli03', 'Pbli04',
    'Pbli11', 'Pbli12', 'Pbli13',
    'Pbli14',
};
// The 'Pdb08'..'Pdb01' and 'Tdb01' panes of mScreen (fn_803CE11C fades
// them out with field_0x196).
static const u64 lbl_80476938[9] = {
    'Pdb08', 'Pdb07', 'Pdb06',
    'Pdb05', 'Pdb04', 'Pdb03',
    'Pdb02', 'Pdb01', 'Tdb01',
};
static const u64 lbl_80476980[10] = {
    'Pqb01', 'Pqb02', 'Pqb03',
    'Pqb04', 'Pqb05', 'Pqb06',
    'Pqb07', 'Pqb08', 'Pqb09',
    'Pqb10',
};
static const u64 lbl_804769D0[7] = {
    'Padvw10', 'Padvw20', 'Padvw30',
    'Padvw40', 'Padvw50', 'Padvw60',
    'Padvw70',
};
static const u64 lbl_80476A08[8] = {
    'Padvw11', 'Padvw21', 'Padvw31',
    'Padvw41', 'Padvw51', 'Padvw61',
    'Padvw71', 'Padvl01',
};
static const u64 lbl_80476A48[8] = {
    'Padvw12', 'Padvw22', 'Padvw32',
    'Padvw42', 'Padvw52', 'Padvw62',
    'Padvw72', 'Padvl02',
};
static const u64 lbl_80476A88[3] = {
    'Pf1tbb07', 'Pf2tbb07', 'Pf3tbb07',
};
static const u64 lbl_80476AA0[3] = {
    'Pf1tbb06', 'Pf2tbb06', 'Pf3tbb06',
};
static const u64 lbl_80476AB8[3] = {
    'Pf1tbb05', 'Pf2tbb05', 'Pf3tbb05',
};
static const u64 lbl_80476AD0[3] = {
    'Pf1tbb04', 'Pf2tbb04', 'Pf3tbb04',
};
static const u64 lbl_80476AE8[3] = {
    'Pf1tbb03', 'Pf2tbb03', 'Pf3tbb03',
};
static const u64 lbl_80476B00[3] = {
    'Pf1tbb02', 'Pf2tbb02', 'Pf3tbb02',
};
static const u64 lbl_80476B18[3] = {
    'Pf1tbb01', 'Pf2tbb01', 'Pf3tbb01',
};
static const u64 lbl_80476B30[3] = {
    'Pf1tb05', 'Pf2tb05', 'Pf3tb05',
};
static const u64 lbl_80476B48[3] = {
    'Pf1tb04', 'Pf2tb04', 'Pf3tb04',
};
static const u64 lbl_80476B60[3] = {
    'Pf1tb03', 'Pf2tb03', 'Pf3tb03',
};
static const u64 lbl_80476B78[3] = {
    'Pf1tb02', 'Pf2tb02', 'Pf3tb02',
};
static const u64 lbl_80476B90[3] = {
    'Pf1tb01', 'Pf2tb01', 'Pf3tb01',
};
static const u64 lbl_80476BA8[3] = {
    'Pf1t01', 'Pf2t01', 'Pf3t01',
};
static const u64 lbl_80476BC0[3] = {
    'Pf1bb05', 'Pf2bb05', 'Pf3bb05',
};
static const u64 lbl_80476BD8[3] = {
    'Pf1bb04', 'Pf2bb04', 'Pf3bb04',
};
static const u64 lbl_80476BF0[3] = {
    'Pf1bb03', 'Pf2bb03', 'Pf3bb03',
};
static const u64 lbl_80476C08[3] = {
    'Pf1bb02', 'Pf2bb02', 'Pf3bb02',
};
static const u64 lbl_80476C20[3] = {
    'Pf1bb01', 'Pf2bb01', 'Pf3bb01',
};
static const u64 lbl_80476C38[3] = {
    'Pf1b05', 'Pf2b05', 'Pf3b05',
};
static const u64 lbl_80476C50[3] = {
    'Pf1b04', 'Pf2b04', 'Pf3b04',
};
static const u64 lbl_80476C68[3] = {
    'Pf1b03', 'Pf2b03', 'Pf3b03',
};
static const u64 lbl_80476C80[3] = {
    'Pf1b02', 'Pf2b02', 'Pf3b02',
};
static const u64 lbl_80476C98[3] = {
    'Pf1b01', 'Pf2b01', 'Pf3b01',
};
static const u64 lbl_80476CB0[4] = {
    'Pstrb04', 'Pcopb04', 'Pdelb04',
    'Pbacb04',
};
static const u64 lbl_80476CD0[3] = {
    'Pf1bb06', 'Pf2bb06', 'Pf3bb06',
};
static const u64 lbl_80476CE8[3] = {
    'Pf1tbb08', 'Pf2tbb08', 'Pf3tbb08',
};
// Message ids (fn_803CE4CC).
static const s32 lbl_80476D00[8] = {
    0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
};
static const u64 lbl_80476D20[3] = {
    'Tf1date', 'Tf2date', 'Tf3date',
};
static const u64 lbl_80476D38[3] = {
    'Tf1lv', 'Tf2lv', 'Tf3lv',
};
static const u64 lbl_80476D50[3] = {
    'Tf1fir', 'Tf2fir', 'Tf3fir',
};
static const u64 lbl_80476D68[4] = {
    'Pbacb01', 'Pbacb02', 'Pbacb03',
    'Pbacb04',
};
static const u64 lbl_80476D88[4] = {
    'Pdelb01', 'Pdelb02', 'Pdelb03',
    'Pdelb04',
};
static const u64 lbl_80476DA8[4] = {
    'Pcopb01', 'Pcopb02', 'Pcopb03',
    'Pcopb04',
};
static const u64 lbl_80476DC8[4] = {
    'Pstrb01', 'Pstrb02', 'Pstrb03',
    'Pstrb04',
};
// fn_803BE3D8.
static const s32 lbl_80476DE8[6] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3,
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
        fn_803832BC(((J2DTextBox*)mScreenHuuin->search('Tft01'))->getStringPtr(),
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

// FIXME: 98.73 %. Missing: the save-data setup the SaveRow_8050EC80[] view
// (tu_80379F84.h) cannot express without casts: after `field_0xff = 0` the
// original runs `for (i = 0; i < 3; i++) fn_803B950C(rows[i], blocks[i])` with
// rows at lbl_8050EC80 + 0x20 + i * 0xC8 and blocks at lbl_8050EC80 + 0x280 +
// i * 0x9C4, and after field_0x114 stores field_0x5c = lbl_8050EC80 + 0x20 and
// field_0x60 = lbl_8050EC80 + 0x280 (18 instructions fewer in total); the registers around
// them differ as a result (r7/r5 for the zero, r28/r29 in two loops, r29/r27
// for the font). The string offsets differ by 0x58 (the strings of
// fn_803BE3D8 and fn_803CE4CC, unwritten, come first).
FileSelect_804AFF60::FileSelect_804AFF60() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053AAF8->field_0xff = 0;
    lbl_80529DEC.fn_80403680();
    if (!lbl_80529DEC.field_0x242) {
        lbl_80529DEC.field_0x242 = 1;
        lbl_8053AB10->field_0x20->fn_802FF798();
        void* res = lbl_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_base_sch.szs");
        lbl_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0, 0);
        res = lbl_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_schE.szs");
        lbl_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0x7000, 0);
        res = lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        lbl_8053AB10->field_0x20->fn_802F93F4(res, 0, 0, 0, 0, false);
        res = lbl_8053AAF8->field_0xa4->getResource('SCH ', "player_sch.szs");
        lbl_8053AB10->field_0x20->fn_802F9714(res, 0, 2, 0x4000, 0);
        res = lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        lbl_8053AB10->field_0x20->fn_802F93F4(res, 0, 2, 0, 0, false);
        res = lbl_8053AAF8->field_0xa4->getResource('SCH ', "gals_sch.szs");
        lbl_8053AB10->field_0x20->fn_802F9714(res, 0, 3, 0, 0);
        res = lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        lbl_8053AB10->field_0x20->fn_802F93F4(res, 0, 3, 0, 0, false);
        lbl_8053AB10->field_0x20->field_0x2364 = 1;
    }

    lbl_8053ADB0 = this;
    field_0x80 = 0;
    field_0x7c = 0;
    field_0x84 = 0;
    field_0xa4 = lbl_80529DEC.field_0x23a[1];
    field_0xa0 = 0;
    field_0xa8 = 0;
    field_0x113 = 0;
    field_0xac = 0;
    field_0x111 = 1;
    mPrevState = 1;
    mState = 1;
    field_0x114 = 0;
    field_0x115 = 0;
    field_0xb0 = -1;
    field_0xdc = 0;
    field_0x116 = 0;
    field_0xb4 = 0;
    field_0x117 = 0;
    field_0xda = 0;
    field_0x1a4 = 0;
    field_0x1a5 = 0;
    field_0x1a6 = 0;
    for (s32 i = 0; i < 8; i++) {
        field_0xb8[i] = fn_803B8CAC(0, i);
    }

    mScreen = new J2DScreen;
    mScreen->setPriority("file_select.blo", 0x1100000, lbl_80529DEC.field_0x30);
    mScreenHuuin = new J2DScreen;
    mScreenHuuin->setPriority("file_select_huuin.blo", 0x1100000, lbl_80529DEC.field_0x30);
    mScreenHuuin->search('Paccur02')->setAlpha(0x9C);
    mScreenHuuin->search('Pfcur05')->setAlpha(0x9C);

    const ResTIMG* img = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_black.bti"));
    field_0x10 = new J2DPicture(img);
    field_0x10->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));

    field_0x14 = new cls_804AF86C;
    field_0x14->fn_80402F04(mScreen->search('Ndata04'));
    field_0x18 = new cls_804AF86C;
    field_0x18->fn_80402F04(mScreenHuuin->search('Ndata01'));
    field_0x1c = new cls_804AF86C;
    field_0x1c->fn_80402F04(mScreenHuuin->search('Ndatab01'));
    field_0x20 = new cls_804AF86C;
    field_0x20->fn_80402F04(mScreen->search('Ndata06'));
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i] = new cls_804AF86C;
        field_0x24[i]->fn_80402F04(mScreenHuuin->search(lbl_80476CB0[i]));
    }
    field_0x34 = new cls_804AF86C;
    field_0x34->fn_80402F04(mScreenHuuin->search('Paccur02'));
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i] = new cls_804AF86C;
        field_0x38[i]->fn_80402F04(mScreenHuuin->search(lbl_80476CD0[i]));
        field_0x44[i] = new cls_804AF86C;
        field_0x44[i]->fn_80402F04(mScreenHuuin->search(lbl_80476CE8[i]));
    }
    field_0x50 = new cls_804AF86C;
    field_0x50->fn_80402F04(mScreenHuuin->search('Pfcur05'));
    field_0x54 = new cls_804AF86C;
    field_0x54->fn_80402F04(mScreenHuuin->search('Pftbbk03'));
    field_0x58 = lbl_8053AED8->field_0x8;
    lbl_8053AED8->fn_80440D78(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_kettei_01.bti")));
    lbl_8053AED8->fn_80440CE4(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_modoru_01.bti")));

    if (lbl_8053AAF8->field_0x7f) {
        mScreenHuuin->search('Pbli04')->hide();
    } else {
        mScreenHuuin->search('Pbli14')->hide();
    }

    JUTResFont* font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen->search('Tdb01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen->search('Tbat01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tadv01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tbac01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tdel01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tcop01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tstr01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf3date'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf3lv'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf3fir'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf2date'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf2lv'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf2fir'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf1date'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf1lv'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf1fir'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tft01'))->setFont(font);
    // The colours of the file 1 pictures stand for all three files.
    field_0xe4 = ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->getBlack();
    field_0xe0 = ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->getWhite();
    field_0xfc[0] = ((J2DPicture*)mScreenHuuin->search('Pf1t01'))->getBlack();
    field_0xe8[0] = ((J2DPicture*)mScreenHuuin->search('Pf1t01'))->getWhite();
    field_0xfc[1] = ((J2DPicture*)mScreenHuuin->search('Pf1tb01'))->getBlack();
    field_0xe8[1] = ((J2DPicture*)mScreenHuuin->search('Pf1tb01'))->getWhite();
    field_0xfc[2] = ((J2DPicture*)mScreenHuuin->search('Pf1tbb01'))->getBlack();
    field_0xe8[2] = ((J2DPicture*)mScreenHuuin->search('Pf1tbb01'))->getWhite();
    field_0xfc[3] = ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getBlack();
    field_0xe8[3] = ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getWhite();
    field_0xfc[4] = ((J2DPicture*)mScreenHuuin->search('Pf1bb01'))->getBlack();
    field_0xe8[4] = ((J2DPicture*)mScreenHuuin->search('Pf1bb01'))->getWhite();
    // Read and dropped.
    ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getBlack();
    ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getWhite();
    ((J2DPicture*)mScreenHuuin->search('Pf1b05'))->getBlack();
    ((J2DPicture*)mScreenHuuin->search('Pf1b05'))->getWhite();
    field_0x118[0] = mScreenHuuin->search('Pf1t01')->getAlpha();
    field_0x118[1] = mScreenHuuin->search('Pf1tb01')->getAlpha();
    field_0x118[2] = mScreenHuuin->search('Pf1tb02')->getAlpha();
    field_0x118[3] = mScreenHuuin->search('Pf1tb03')->getAlpha();
    field_0x118[4] = mScreenHuuin->search('Pf1tb04')->getAlpha();
    field_0x118[5] = mScreenHuuin->search('Pf1tb05')->getAlpha();
    field_0x118[6] = mScreenHuuin->search('Pf1tbb01')->getAlpha();
    field_0x118[7] = mScreenHuuin->search('Pf1tbb02')->getAlpha();
    field_0x118[8] = mScreenHuuin->search('Pf1tbb03')->getAlpha();
    field_0x118[9] = mScreenHuuin->search('Pf1tbb04')->getAlpha();
    field_0x118[10] = mScreenHuuin->search('Pf1tbb05')->getAlpha();
    field_0x118[11] = mScreenHuuin->search('Pf1tbb06')->getAlpha();
    field_0x118[12] = mScreenHuuin->search('Pf1tbb07')->getAlpha();
    field_0x118[13] = mScreenHuuin->search('Tf1fir')->getAlpha();
    field_0x118[14] = mScreenHuuin->search('Tf1lv')->getAlpha();
    field_0x118[15] = mScreenHuuin->search('Tf1date')->getAlpha();
    field_0x118[16] = mScreenHuuin->search('Pf1b01')->getAlpha();
    field_0x118[17] = mScreenHuuin->search('Pf1b02')->getAlpha();
    field_0x118[18] = mScreenHuuin->search('Pf1b03')->getAlpha();
    field_0x118[19] = mScreenHuuin->search('Pf1b04')->getAlpha();
    field_0x118[20] = mScreenHuuin->search('Pf1b05')->getAlpha();
    field_0x118[21] = mScreenHuuin->search('Pf1bb01')->getAlpha();
    field_0x118[22] = mScreenHuuin->search('Pf1bb02')->getAlpha();
    field_0x118[23] = mScreenHuuin->search('Pf1bb03')->getAlpha();
    field_0x118[24] = mScreenHuuin->search('Pf1bb04')->getAlpha();
    field_0x118[25] = mScreenHuuin->search('Pf1bb05')->getAlpha();
    for (s32 i = 0; i < 81; i++) {
        field_0x132[i] = mScreen->search(lbl_80476618[i])->getAlpha();
    }
    for (s32 i = 0; i < 19; i++) {
        field_0x183[i] = mScreenHuuin->search(lbl_804768A0[i])->getAlpha();
    }
    for (s32 i = 0; i < 9; i++) {
        field_0x196[i] = mScreen->search(lbl_80476938[i])->getAlpha();
    }
    field_0x19f[0] = mScreenHuuin->search('Pbacb01')->getAlpha();
    field_0x19f[1] = mScreenHuuin->search('Pbacb02')->getAlpha();
    field_0x19f[2] = mScreenHuuin->search('Pbacb03')->getAlpha();
    field_0x19f[3] = mScreenHuuin->search('Pbacb04')->getAlpha();
    field_0x1a3 = 0;

    fn_803832BC(((J2DTextBox*)mScreenHuuin->search('Tadv01'))->getStringPtr(), 0xA4, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreen->search('Tbat01'))->getStringPtr(), 0xA5, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x118, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x119, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search('Tdel01'))->getStringPtr(), 0x11A, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search('Tbac01'))->getStringPtr(), 0x11B, 0, NULL, NULL);

    for (s32 i = 0; i < 3; i++) {
        field_0x70[i] = 0.0f;
        field_0x64[i] = 0.0f;
    }
    mScreen->search('Ndata06')->hide();
    mScreen->search(lbl_80476980[0])->hide();
    mScreen->search(lbl_80476980[1])->hide();
    mScreen->search('Padvw10')->hide();
    mScreen->search('Padvw11')->hide();
    mScreen->search('Padvw12')->hide();
    mScreen->search('Padvw13')->hide();
    mScreen->search('Padvw14')->hide();
    mScreen->search('Padvw20')->hide();
    mScreen->search('Padvw21')->hide();
    mScreen->search('Padvw22')->hide();
    mScreen->search('Padvw23')->hide();
    mScreen->search('Padvw24')->hide();
    mScreen->search('Padvw30')->hide();
    mScreen->search('Padvw31')->hide();
    mScreen->search('Padvw32')->hide();
    mScreen->search('Padvw33')->hide();
    mScreen->search('Padvw34')->hide();
    mScreen->search('Padvw40')->hide();
    mScreen->search('Padvw41')->hide();
    mScreen->search('Padvw42')->hide();
    mScreen->search('Padvw43')->hide();
    mScreen->search('Padvw44')->hide();
    mScreen->search('Padvw50')->hide();
    mScreen->search('Padvw51')->hide();
    mScreen->search('Padvw52')->hide();
    mScreen->search('Padvw53')->hide();
    mScreen->search('Padvw54')->hide();
    mScreen->search('Padvw60')->hide();
    mScreen->search('Padvw61')->hide();
    mScreen->search('Padvw62')->hide();
    mScreen->search('Padvw63')->hide();
    mScreen->search('Padvw64')->hide();
    mScreen->search('Padvw70')->hide();
    mScreen->search('Padvw71')->hide();
    mScreen->search('Padvw72')->hide();
    mScreen->search('Padvw73')->hide();
    mScreen->search('Padvw74')->hide();
    mScreen->search('Padvl02')->hide();
    mScreen->search('Padvl01')->hide();
    mScreenHuuin->search('Padhu01')->hide();
    mScreen->search('Padhu02')->hide();
    mScreen->search('Padhu03')->hide();
    field_0x88[0] = mScreenHuuin->search('Padhu01')->getBounds()->i.x;
    field_0x88[1] = mScreen->search('Padhu02')->getBounds()->i.x;
    field_0x88[2] = mScreen->search('Padhu03')->getBounds()->i.x;
    field_0x94[0] = mScreenHuuin->search('Padhu01')->getBounds()->i.y;
    field_0x94[1] = mScreen->search('Padhu02')->getBounds()->i.y;
    field_0x94[2] = mScreen->search('Padhu03')->getBounds()->i.y;
    mScreenHuuin->search('Padhu01')->setAlpha(0xFF);
    field_0xd8 = 0;
    field_0xda = 0;

    if (lbl_8053AF80->field_0x58 != 0x100001A) {
        lbl_8053AF80->fn_80458FF0(0x100001A, false, false);
        lbl_8053AF80->fn_80458408(0x100001A, false);
        lbl_80529DEC.field_0x248 = 0;
    } else if (lbl_80529DEC.field_0x248) {
        lbl_8053AF80->fn_80458408(0x100001A, false);
        lbl_80529DEC.field_0x248 = 0;
    }
}
