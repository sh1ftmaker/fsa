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
    0x5466316C76ull /* "Tf1lv" */, 0x5466326C76ull /* "Tf2lv" */, 0x5466336C76ull /* "Tf3lv" */,
};
static const u64 lbl_804765E8[3] = {
    0x546631666972ull /* "Tf1fir" */, 0x546632666972ull /* "Tf2fir" */, 0x546633666972ull /* "Tf3fir" */,
};
static const u64 lbl_80476600[3] = {
    0x54663164617465ull /* "Tf1date" */, 0x54663264617465ull /* "Tf2date" */, 0x54663364617465ull /* "Tf3date" */,
};
static const u64 lbl_80476618[81] = {
    0x506162623130ull /* "Pabb10" */, 0x506162623039ull /* "Pabb09" */, 0x506162623038ull /* "Pabb08" */,
    0x506162623037ull /* "Pabb07" */, 0x506162623036ull /* "Pabb06" */, 0x506162623035ull /* "Pabb05" */,
    0x506162623034ull /* "Pabb04" */, 0x506162623033ull /* "Pabb03" */, 0x506162623032ull /* "Pabb02" */,
    0x506162623031ull /* "Pabb01" */, 0x576261743031ull /* "Wbat01" */, 0x50626174633130ull /* "Pbatc10" */,
    0x50626174633039ull /* "Pbatc09" */, 0x50626174633038ull /* "Pbatc08" */, 0x50626174633037ull /* "Pbatc07" */,
    0x50626174633036ull /* "Pbatc06" */, 0x50626174633035ull /* "Pbatc05" */, 0x50626174633034ull /* "Pbatc04" */,
    0x50626174633033ull /* "Pbatc03" */, 0x50626174633032ull /* "Pbatc02" */, 0x50626174633031ull /* "Pbatc01" */,
    0x5071623130ull /* "Pqb10" */, 0x5071623039ull /* "Pqb09" */, 0x5071623038ull /* "Pqb08" */,
    0x5071623037ull /* "Pqb07" */, 0x5071623036ull /* "Pqb06" */, 0x5071623035ull /* "Pqb05" */,
    0x5071623034ull /* "Pqb04" */, 0x5071623033ull /* "Pqb03" */, 0x5071623032ull /* "Pqb02" */,
    0x5071623031ull /* "Pqb01" */, 0x50626174623039ull /* "Pbatb09" */, 0x50626174623038ull /* "Pbatb08" */,
    0x50626174623037ull /* "Pbatb07" */, 0x50626174623036ull /* "Pbatb06" */, 0x50626174623035ull /* "Pbatb05" */,
    0x50626174623034ull /* "Pbatb04" */, 0x50626174623033ull /* "Pbatb03" */, 0x50626174623032ull /* "Pbatb02" */,
    0x50626174623031ull /* "Pbatb01" */, 0x546261743031ull /* "Tbat01" */, 0x576164763031ull /* "Wadv01" */,
    0x50616468753033ull /* "Padhu03" */, 0x50616468753032ull /* "Padhu02" */, 0x506164766C3032ull /* "Padvl02" */,
    0x506164766C3031ull /* "Padvl01" */, 0x50616476773732ull /* "Padvw72" */, 0x50616476773731ull /* "Padvw71" */,
    0x50616476773730ull /* "Padvw70" */, 0x50616476773632ull /* "Padvw62" */, 0x50616476773631ull /* "Padvw61" */,
    0x50616476773630ull /* "Padvw60" */, 0x50616476773532ull /* "Padvw52" */, 0x50616476773531ull /* "Padvw51" */,
    0x50616476773530ull /* "Padvw50" */, 0x50616476773432ull /* "Padvw42" */, 0x50616476773431ull /* "Padvw41" */,
    0x50616476773430ull /* "Padvw40" */, 0x50616476773332ull /* "Padvw32" */, 0x50616476773331ull /* "Padvw31" */,
    0x50616476773330ull /* "Padvw30" */, 0x50616476773232ull /* "Padvw22" */, 0x50616476773231ull /* "Padvw21" */,
    0x50616476773230ull /* "Padvw20" */, 0x50616476773132ull /* "Padvw12" */, 0x50616476773131ull /* "Padvw11" */,
    0x50616476773130ull /* "Padvw10" */, 0x50616476773734ull /* "Padvw74" */, 0x50616476773733ull /* "Padvw73" */,
    0x50616476773634ull /* "Padvw64" */, 0x50616476773633ull /* "Padvw63" */, 0x50616476773534ull /* "Padvw54" */,
    0x50616476773533ull /* "Padvw53" */, 0x50616476773434ull /* "Padvw44" */, 0x50616476773433ull /* "Padvw43" */,
    0x50616476773334ull /* "Padvw34" */, 0x50616476773333ull /* "Padvw33" */, 0x50616476773234ull /* "Padvw24" */,
    0x50616476773233ull /* "Padvw23" */, 0x50616476773134ull /* "Padvw14" */, 0x50616476773133ull /* "Padvw13" */,
};
static const u64 lbl_804768A0[19] = {
    0x50616468753031ull /* "Padhu01" */, 0x50616476623039ull /* "Padvb09" */, 0x50616476623038ull /* "Padvb08" */,
    0x50616476623037ull /* "Padvb07" */, 0x50616476623036ull /* "Padvb06" */, 0x50616476623035ull /* "Padvb05" */,
    0x50616476623034ull /* "Padvb04" */, 0x50616476623033ull /* "Padvb03" */, 0x50616476623032ull /* "Padvb02" */,
    0x50616476623031ull /* "Padvb01" */, 0x546164763031ull /* "Tadv01" */, 0x50626C693031ull /* "Pbli01" */,
    0x50626C693032ull /* "Pbli02" */, 0x50626C693033ull /* "Pbli03" */, 0x50626C693034ull /* "Pbli04" */,
    0x50626C693131ull /* "Pbli11" */, 0x50626C693132ull /* "Pbli12" */, 0x50626C693133ull /* "Pbli13" */,
    0x50626C693134ull /* "Pbli14" */,
};
// The 'Pdb08'..'Pdb01' and 'Tdb01' panes of mScreen (fn_803CE11C fades
// them out with field_0x196).
static const u64 lbl_80476938[9] = {
    0x5064623038ull /* "Pdb08" */, 0x5064623037ull /* "Pdb07" */, 0x5064623036ull /* "Pdb06" */,
    0x5064623035ull /* "Pdb05" */, 0x5064623034ull /* "Pdb04" */, 0x5064623033ull /* "Pdb03" */,
    0x5064623032ull /* "Pdb02" */, 0x5064623031ull /* "Pdb01" */, 0x5464623031ull /* "Tdb01" */,
};
static const u64 lbl_80476980[10] = {
    0x5071623031ull /* "Pqb01" */, 0x5071623032ull /* "Pqb02" */, 0x5071623033ull /* "Pqb03" */,
    0x5071623034ull /* "Pqb04" */, 0x5071623035ull /* "Pqb05" */, 0x5071623036ull /* "Pqb06" */,
    0x5071623037ull /* "Pqb07" */, 0x5071623038ull /* "Pqb08" */, 0x5071623039ull /* "Pqb09" */,
    0x5071623130ull /* "Pqb10" */,
};
static const u64 lbl_804769D0[7] = {
    0x50616476773130ull /* "Padvw10" */, 0x50616476773230ull /* "Padvw20" */, 0x50616476773330ull /* "Padvw30" */,
    0x50616476773430ull /* "Padvw40" */, 0x50616476773530ull /* "Padvw50" */, 0x50616476773630ull /* "Padvw60" */,
    0x50616476773730ull /* "Padvw70" */,
};
static const u64 lbl_80476A08[8] = {
    0x50616476773131ull /* "Padvw11" */, 0x50616476773231ull /* "Padvw21" */, 0x50616476773331ull /* "Padvw31" */,
    0x50616476773431ull /* "Padvw41" */, 0x50616476773531ull /* "Padvw51" */, 0x50616476773631ull /* "Padvw61" */,
    0x50616476773731ull /* "Padvw71" */, 0x506164766C3031ull /* "Padvl01" */,
};
static const u64 lbl_80476A48[8] = {
    0x50616476773132ull /* "Padvw12" */, 0x50616476773232ull /* "Padvw22" */, 0x50616476773332ull /* "Padvw32" */,
    0x50616476773432ull /* "Padvw42" */, 0x50616476773532ull /* "Padvw52" */, 0x50616476773632ull /* "Padvw62" */,
    0x50616476773732ull /* "Padvw72" */, 0x506164766C3032ull /* "Padvl02" */,
};
static const u64 lbl_80476A88[3] = {
    0x5066317462623037ull /* "Pf1tbb07" */, 0x5066327462623037ull /* "Pf2tbb07" */, 0x5066337462623037ull /* "Pf3tbb07" */,
};
static const u64 lbl_80476AA0[3] = {
    0x5066317462623036ull /* "Pf1tbb06" */, 0x5066327462623036ull /* "Pf2tbb06" */, 0x5066337462623036ull /* "Pf3tbb06" */,
};
static const u64 lbl_80476AB8[3] = {
    0x5066317462623035ull /* "Pf1tbb05" */, 0x5066327462623035ull /* "Pf2tbb05" */, 0x5066337462623035ull /* "Pf3tbb05" */,
};
static const u64 lbl_80476AD0[3] = {
    0x5066317462623034ull /* "Pf1tbb04" */, 0x5066327462623034ull /* "Pf2tbb04" */, 0x5066337462623034ull /* "Pf3tbb04" */,
};
static const u64 lbl_80476AE8[3] = {
    0x5066317462623033ull /* "Pf1tbb03" */, 0x5066327462623033ull /* "Pf2tbb03" */, 0x5066337462623033ull /* "Pf3tbb03" */,
};
static const u64 lbl_80476B00[3] = {
    0x5066317462623032ull /* "Pf1tbb02" */, 0x5066327462623032ull /* "Pf2tbb02" */, 0x5066337462623032ull /* "Pf3tbb02" */,
};
static const u64 lbl_80476B18[3] = {
    0x5066317462623031ull /* "Pf1tbb01" */, 0x5066327462623031ull /* "Pf2tbb01" */, 0x5066337462623031ull /* "Pf3tbb01" */,
};
static const u64 lbl_80476B30[3] = {
    0x50663174623035ull /* "Pf1tb05" */, 0x50663274623035ull /* "Pf2tb05" */, 0x50663374623035ull /* "Pf3tb05" */,
};
static const u64 lbl_80476B48[3] = {
    0x50663174623034ull /* "Pf1tb04" */, 0x50663274623034ull /* "Pf2tb04" */, 0x50663374623034ull /* "Pf3tb04" */,
};
static const u64 lbl_80476B60[3] = {
    0x50663174623033ull /* "Pf1tb03" */, 0x50663274623033ull /* "Pf2tb03" */, 0x50663374623033ull /* "Pf3tb03" */,
};
static const u64 lbl_80476B78[3] = {
    0x50663174623032ull /* "Pf1tb02" */, 0x50663274623032ull /* "Pf2tb02" */, 0x50663374623032ull /* "Pf3tb02" */,
};
static const u64 lbl_80476B90[3] = {
    0x50663174623031ull /* "Pf1tb01" */, 0x50663274623031ull /* "Pf2tb01" */, 0x50663374623031ull /* "Pf3tb01" */,
};
static const u64 lbl_80476BA8[3] = {
    0x506631743031ull /* "Pf1t01" */, 0x506632743031ull /* "Pf2t01" */, 0x506633743031ull /* "Pf3t01" */,
};
static const u64 lbl_80476BC0[3] = {
    0x50663162623035ull /* "Pf1bb05" */, 0x50663262623035ull /* "Pf2bb05" */, 0x50663362623035ull /* "Pf3bb05" */,
};
static const u64 lbl_80476BD8[3] = {
    0x50663162623034ull /* "Pf1bb04" */, 0x50663262623034ull /* "Pf2bb04" */, 0x50663362623034ull /* "Pf3bb04" */,
};
static const u64 lbl_80476BF0[3] = {
    0x50663162623033ull /* "Pf1bb03" */, 0x50663262623033ull /* "Pf2bb03" */, 0x50663362623033ull /* "Pf3bb03" */,
};
static const u64 lbl_80476C08[3] = {
    0x50663162623032ull /* "Pf1bb02" */, 0x50663262623032ull /* "Pf2bb02" */, 0x50663362623032ull /* "Pf3bb02" */,
};
static const u64 lbl_80476C20[3] = {
    0x50663162623031ull /* "Pf1bb01" */, 0x50663262623031ull /* "Pf2bb01" */, 0x50663362623031ull /* "Pf3bb01" */,
};
static const u64 lbl_80476C38[3] = {
    0x506631623035ull /* "Pf1b05" */, 0x506632623035ull /* "Pf2b05" */, 0x506633623035ull /* "Pf3b05" */,
};
static const u64 lbl_80476C50[3] = {
    0x506631623034ull /* "Pf1b04" */, 0x506632623034ull /* "Pf2b04" */, 0x506633623034ull /* "Pf3b04" */,
};
static const u64 lbl_80476C68[3] = {
    0x506631623033ull /* "Pf1b03" */, 0x506632623033ull /* "Pf2b03" */, 0x506633623033ull /* "Pf3b03" */,
};
static const u64 lbl_80476C80[3] = {
    0x506631623032ull /* "Pf1b02" */, 0x506632623032ull /* "Pf2b02" */, 0x506633623032ull /* "Pf3b02" */,
};
static const u64 lbl_80476C98[3] = {
    0x506631623031ull /* "Pf1b01" */, 0x506632623031ull /* "Pf2b01" */, 0x506633623031ull /* "Pf3b01" */,
};
static const u64 lbl_80476CB0[4] = {
    0x50737472623034ull /* "Pstrb04" */, 0x50636F70623034ull /* "Pcopb04" */, 0x5064656C623034ull /* "Pdelb04" */,
    0x50626163623034ull /* "Pbacb04" */,
};
static const u64 lbl_80476CD0[3] = {
    0x50663162623036ull /* "Pf1bb06" */, 0x50663262623036ull /* "Pf2bb06" */, 0x50663362623036ull /* "Pf3bb06" */,
};
static const u64 lbl_80476CE8[3] = {
    0x5066317462623038ull /* "Pf1tbb08" */, 0x5066327462623038ull /* "Pf2tbb08" */, 0x5066337462623038ull /* "Pf3tbb08" */,
};
// Message ids (fn_803CE4CC).
static const s32 lbl_80476D00[8] = {
    0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
};
static const u64 lbl_80476D20[3] = {
    0x54663164617465ull /* "Tf1date" */, 0x54663264617465ull /* "Tf2date" */, 0x54663364617465ull /* "Tf3date" */,
};
static const u64 lbl_80476D38[3] = {
    0x5466316C76ull /* "Tf1lv" */, 0x5466326C76ull /* "Tf2lv" */, 0x5466336C76ull /* "Tf3lv" */,
};
static const u64 lbl_80476D50[3] = {
    0x546631666972ull /* "Tf1fir" */, 0x546632666972ull /* "Tf2fir" */, 0x546633666972ull /* "Tf3fir" */,
};
static const u64 lbl_80476D68[4] = {
    0x50626163623031ull /* "Pbacb01" */, 0x50626163623032ull /* "Pbacb02" */, 0x50626163623033ull /* "Pbacb03" */,
    0x50626163623034ull /* "Pbacb04" */,
};
static const u64 lbl_80476D88[4] = {
    0x5064656C623031ull /* "Pdelb01" */, 0x5064656C623032ull /* "Pdelb02" */, 0x5064656C623033ull /* "Pdelb03" */,
    0x5064656C623034ull /* "Pdelb04" */,
};
static const u64 lbl_80476DA8[4] = {
    0x50636F70623031ull /* "Pcopb01" */, 0x50636F70623032ull /* "Pcopb02" */, 0x50636F70623033ull /* "Pcopb03" */,
    0x50636F70623034ull /* "Pcopb04" */,
};
static const u64 lbl_80476DC8[4] = {
    0x50737472623031ull /* "Pstrb01" */, 0x50737472623032ull /* "Pstrb02" */, 0x50737472623033ull /* "Pstrb03" */,
    0x50737472623034ull /* "Pstrb04" */,
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

// FIXME: 98.73 %. Missing: the save-data setup the SaveRow_8050EC80[] view
// (tu_80379F84.h) cannot express without casts: after `field_0xff = 0` the
// original runs `for (i = 0; i < 3; i++) fn_803B950C(rows[i], blocks[i])` with
// rows at lbl_8050EC80 + 0x20 + i * 0xC8 and blocks at lbl_8050EC80 + 0x280 +
// i * 0x9C4, and after field_0x114 stores field_0x5c = lbl_8050EC80 + 0x20 and
// field_0x60 = lbl_8050EC80 + 0x280 (16 instructions); the registers around
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
    mScreenHuuin->search(0x5061636375723032ull /* "Paccur02" */)->setAlpha(0x9C);
    mScreenHuuin->search(0x50666375723035ull /* "Pfcur05" */)->setAlpha(0x9C);

    const ResTIMG* img = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_black.bti"));
    field_0x10 = new J2DPicture(img);
    field_0x10->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));

    field_0x14 = new cls_804AF86C;
    field_0x14->fn_80402F04(mScreen->search(0x4E646174613034ull /* "Ndata04" */));
    field_0x18 = new cls_804AF86C;
    field_0x18->fn_80402F04(mScreenHuuin->search(0x4E646174613031ull /* "Ndata01" */));
    field_0x1c = new cls_804AF86C;
    field_0x1c->fn_80402F04(mScreenHuuin->search(0x4E64617461623031ull /* "Ndatab01" */));
    field_0x20 = new cls_804AF86C;
    field_0x20->fn_80402F04(mScreen->search(0x4E646174613036ull /* "Ndata06" */));
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i] = new cls_804AF86C;
        field_0x24[i]->fn_80402F04(mScreenHuuin->search(lbl_80476CB0[i]));
    }
    field_0x34 = new cls_804AF86C;
    field_0x34->fn_80402F04(mScreenHuuin->search(0x5061636375723032ull /* "Paccur02" */));
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i] = new cls_804AF86C;
        field_0x38[i]->fn_80402F04(mScreenHuuin->search(lbl_80476CD0[i]));
        field_0x44[i] = new cls_804AF86C;
        field_0x44[i]->fn_80402F04(mScreenHuuin->search(lbl_80476CE8[i]));
    }
    field_0x50 = new cls_804AF86C;
    field_0x50->fn_80402F04(mScreenHuuin->search(0x50666375723035ull /* "Pfcur05" */));
    field_0x54 = new cls_804AF86C;
    field_0x54->fn_80402F04(mScreenHuuin->search(0x50667462626B3033ull /* "Pftbbk03" */));
    field_0x58 = lbl_8053AED8->field_0x8;
    lbl_8053AED8->fn_80440D78(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_kettei_01.bti")));
    lbl_8053AED8->fn_80440CE4(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_modoru_01.bti")));

    if (lbl_8053AAF8->field_0x7f) {
        mScreenHuuin->search(0x50626C693034ull /* "Pbli04" */)->hide();
    } else {
        mScreenHuuin->search(0x50626C693134ull /* "Pbli14" */)->hide();
    }

    JUTResFont* font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen->search(0x5464623031ull /* "Tdb01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen->search(0x546261743031ull /* "Tbat01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x546164763031ull /* "Tadv01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x546261633031ull /* "Tbac01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x5464656C3031ull /* "Tdel01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x54636F703031ull /* "Tcop01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x547374723031ull /* "Tstr01" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x54663364617465ull /* "Tf3date" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x5466336C76ull /* "Tf3lv" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x546633666972ull /* "Tf3fir" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x54663264617465ull /* "Tf2date" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x5466326C76ull /* "Tf2lv" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x546632666972ull /* "Tf2fir" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x54663164617465ull /* "Tf1date" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x5466316C76ull /* "Tf1lv" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x546631666972ull /* "Tf1fir" */))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search(0x5466743031ull /* "Tft01" */))->setFont(font);
    // The colours of the file 1 pictures stand for all three files.
    field_0xe4 = ((J2DPicture*)mScreenHuuin->search(0x50636F70623031ull /* "Pcopb01" */))->getBlack();
    field_0xe0 = ((J2DPicture*)mScreenHuuin->search(0x50636F70623031ull /* "Pcopb01" */))->getWhite();
    field_0xfc[0] = ((J2DPicture*)mScreenHuuin->search(0x506631743031ull /* "Pf1t01" */))->getBlack();
    field_0xe8[0] = ((J2DPicture*)mScreenHuuin->search(0x506631743031ull /* "Pf1t01" */))->getWhite();
    field_0xfc[1] = ((J2DPicture*)mScreenHuuin->search(0x50663174623031ull /* "Pf1tb01" */))->getBlack();
    field_0xe8[1] = ((J2DPicture*)mScreenHuuin->search(0x50663174623031ull /* "Pf1tb01" */))->getWhite();
    field_0xfc[2] = ((J2DPicture*)mScreenHuuin->search(0x5066317462623031ull /* "Pf1tbb01" */))->getBlack();
    field_0xe8[2] = ((J2DPicture*)mScreenHuuin->search(0x5066317462623031ull /* "Pf1tbb01" */))->getWhite();
    field_0xfc[3] = ((J2DPicture*)mScreenHuuin->search(0x506631623031ull /* "Pf1b01" */))->getBlack();
    field_0xe8[3] = ((J2DPicture*)mScreenHuuin->search(0x506631623031ull /* "Pf1b01" */))->getWhite();
    field_0xfc[4] = ((J2DPicture*)mScreenHuuin->search(0x50663162623031ull /* "Pf1bb01" */))->getBlack();
    field_0xe8[4] = ((J2DPicture*)mScreenHuuin->search(0x50663162623031ull /* "Pf1bb01" */))->getWhite();
    // Read and dropped.
    ((J2DPicture*)mScreenHuuin->search(0x506631623031ull /* "Pf1b01" */))->getBlack();
    ((J2DPicture*)mScreenHuuin->search(0x506631623031ull /* "Pf1b01" */))->getWhite();
    ((J2DPicture*)mScreenHuuin->search(0x506631623035ull /* "Pf1b05" */))->getBlack();
    ((J2DPicture*)mScreenHuuin->search(0x506631623035ull /* "Pf1b05" */))->getWhite();
    field_0x118[0] = mScreenHuuin->search(0x506631743031ull /* "Pf1t01" */)->getAlpha();
    field_0x118[1] = mScreenHuuin->search(0x50663174623031ull /* "Pf1tb01" */)->getAlpha();
    field_0x118[2] = mScreenHuuin->search(0x50663174623032ull /* "Pf1tb02" */)->getAlpha();
    field_0x118[3] = mScreenHuuin->search(0x50663174623033ull /* "Pf1tb03" */)->getAlpha();
    field_0x118[4] = mScreenHuuin->search(0x50663174623034ull /* "Pf1tb04" */)->getAlpha();
    field_0x118[5] = mScreenHuuin->search(0x50663174623035ull /* "Pf1tb05" */)->getAlpha();
    field_0x118[6] = mScreenHuuin->search(0x5066317462623031ull /* "Pf1tbb01" */)->getAlpha();
    field_0x118[7] = mScreenHuuin->search(0x5066317462623032ull /* "Pf1tbb02" */)->getAlpha();
    field_0x118[8] = mScreenHuuin->search(0x5066317462623033ull /* "Pf1tbb03" */)->getAlpha();
    field_0x118[9] = mScreenHuuin->search(0x5066317462623034ull /* "Pf1tbb04" */)->getAlpha();
    field_0x118[10] = mScreenHuuin->search(0x5066317462623035ull /* "Pf1tbb05" */)->getAlpha();
    field_0x118[11] = mScreenHuuin->search(0x5066317462623036ull /* "Pf1tbb06" */)->getAlpha();
    field_0x118[12] = mScreenHuuin->search(0x5066317462623037ull /* "Pf1tbb07" */)->getAlpha();
    field_0x118[13] = mScreenHuuin->search(0x546631666972ull /* "Tf1fir" */)->getAlpha();
    field_0x118[14] = mScreenHuuin->search(0x5466316C76ull /* "Tf1lv" */)->getAlpha();
    field_0x118[15] = mScreenHuuin->search(0x54663164617465ull /* "Tf1date" */)->getAlpha();
    field_0x118[16] = mScreenHuuin->search(0x506631623031ull /* "Pf1b01" */)->getAlpha();
    field_0x118[17] = mScreenHuuin->search(0x506631623032ull /* "Pf1b02" */)->getAlpha();
    field_0x118[18] = mScreenHuuin->search(0x506631623033ull /* "Pf1b03" */)->getAlpha();
    field_0x118[19] = mScreenHuuin->search(0x506631623034ull /* "Pf1b04" */)->getAlpha();
    field_0x118[20] = mScreenHuuin->search(0x506631623035ull /* "Pf1b05" */)->getAlpha();
    field_0x118[21] = mScreenHuuin->search(0x50663162623031ull /* "Pf1bb01" */)->getAlpha();
    field_0x118[22] = mScreenHuuin->search(0x50663162623032ull /* "Pf1bb02" */)->getAlpha();
    field_0x118[23] = mScreenHuuin->search(0x50663162623033ull /* "Pf1bb03" */)->getAlpha();
    field_0x118[24] = mScreenHuuin->search(0x50663162623034ull /* "Pf1bb04" */)->getAlpha();
    field_0x118[25] = mScreenHuuin->search(0x50663162623035ull /* "Pf1bb05" */)->getAlpha();
    for (s32 i = 0; i < 81; i++) {
        field_0x132[i] = mScreen->search(lbl_80476618[i])->getAlpha();
    }
    for (s32 i = 0; i < 19; i++) {
        field_0x183[i] = mScreenHuuin->search(lbl_804768A0[i])->getAlpha();
    }
    for (s32 i = 0; i < 9; i++) {
        field_0x196[i] = mScreen->search(lbl_80476938[i])->getAlpha();
    }
    field_0x19f[0] = mScreenHuuin->search(0x50626163623031ull /* "Pbacb01" */)->getAlpha();
    field_0x19f[1] = mScreenHuuin->search(0x50626163623032ull /* "Pbacb02" */)->getAlpha();
    field_0x19f[2] = mScreenHuuin->search(0x50626163623033ull /* "Pbacb03" */)->getAlpha();
    field_0x19f[3] = mScreenHuuin->search(0x50626163623034ull /* "Pbacb04" */)->getAlpha();
    field_0x1a3 = 0;

    fn_803832BC(((J2DTextBox*)mScreenHuuin->search(0x546164763031ull /* "Tadv01" */))->getStringPtr(), 0xA4, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreen->search(0x546261743031ull /* "Tbat01" */))->getStringPtr(), 0xA5, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search(0x547374723031ull /* "Tstr01" */))->getStringPtr(), 0x118, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search(0x54636F703031ull /* "Tcop01" */))->getStringPtr(), 0x119, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search(0x5464656C3031ull /* "Tdel01" */))->getStringPtr(), 0x11A, 0, NULL, NULL);
    fn_803832BC(((J2DTextBox*)mScreenHuuin->search(0x546261633031ull /* "Tbac01" */))->getStringPtr(), 0x11B, 0, NULL, NULL);

    for (s32 i = 0; i < 3; i++) {
        field_0x70[i] = 0.0f;
        field_0x64[i] = 0.0f;
    }
    mScreen->search(0x4E646174613036ull /* "Ndata06" */)->hide();
    mScreen->search(lbl_80476980[0])->hide();
    mScreen->search(lbl_80476980[1])->hide();
    mScreen->search(0x50616476773130ull /* "Padvw10" */)->hide();
    mScreen->search(0x50616476773131ull /* "Padvw11" */)->hide();
    mScreen->search(0x50616476773132ull /* "Padvw12" */)->hide();
    mScreen->search(0x50616476773133ull /* "Padvw13" */)->hide();
    mScreen->search(0x50616476773134ull /* "Padvw14" */)->hide();
    mScreen->search(0x50616476773230ull /* "Padvw20" */)->hide();
    mScreen->search(0x50616476773231ull /* "Padvw21" */)->hide();
    mScreen->search(0x50616476773232ull /* "Padvw22" */)->hide();
    mScreen->search(0x50616476773233ull /* "Padvw23" */)->hide();
    mScreen->search(0x50616476773234ull /* "Padvw24" */)->hide();
    mScreen->search(0x50616476773330ull /* "Padvw30" */)->hide();
    mScreen->search(0x50616476773331ull /* "Padvw31" */)->hide();
    mScreen->search(0x50616476773332ull /* "Padvw32" */)->hide();
    mScreen->search(0x50616476773333ull /* "Padvw33" */)->hide();
    mScreen->search(0x50616476773334ull /* "Padvw34" */)->hide();
    mScreen->search(0x50616476773430ull /* "Padvw40" */)->hide();
    mScreen->search(0x50616476773431ull /* "Padvw41" */)->hide();
    mScreen->search(0x50616476773432ull /* "Padvw42" */)->hide();
    mScreen->search(0x50616476773433ull /* "Padvw43" */)->hide();
    mScreen->search(0x50616476773434ull /* "Padvw44" */)->hide();
    mScreen->search(0x50616476773530ull /* "Padvw50" */)->hide();
    mScreen->search(0x50616476773531ull /* "Padvw51" */)->hide();
    mScreen->search(0x50616476773532ull /* "Padvw52" */)->hide();
    mScreen->search(0x50616476773533ull /* "Padvw53" */)->hide();
    mScreen->search(0x50616476773534ull /* "Padvw54" */)->hide();
    mScreen->search(0x50616476773630ull /* "Padvw60" */)->hide();
    mScreen->search(0x50616476773631ull /* "Padvw61" */)->hide();
    mScreen->search(0x50616476773632ull /* "Padvw62" */)->hide();
    mScreen->search(0x50616476773633ull /* "Padvw63" */)->hide();
    mScreen->search(0x50616476773634ull /* "Padvw64" */)->hide();
    mScreen->search(0x50616476773730ull /* "Padvw70" */)->hide();
    mScreen->search(0x50616476773731ull /* "Padvw71" */)->hide();
    mScreen->search(0x50616476773732ull /* "Padvw72" */)->hide();
    mScreen->search(0x50616476773733ull /* "Padvw73" */)->hide();
    mScreen->search(0x50616476773734ull /* "Padvw74" */)->hide();
    mScreen->search(0x506164766C3032ull /* "Padvl02" */)->hide();
    mScreen->search(0x506164766C3031ull /* "Padvl01" */)->hide();
    mScreenHuuin->search(0x50616468753031ull /* "Padhu01" */)->hide();
    mScreen->search(0x50616468753032ull /* "Padhu02" */)->hide();
    mScreen->search(0x50616468753033ull /* "Padhu03" */)->hide();
    field_0x88[0] = mScreenHuuin->search(0x50616468753031ull /* "Padhu01" */)->getBounds()->i.x;
    field_0x88[1] = mScreen->search(0x50616468753032ull /* "Padhu02" */)->getBounds()->i.x;
    field_0x88[2] = mScreen->search(0x50616468753033ull /* "Padhu03" */)->getBounds()->i.x;
    field_0x94[0] = mScreenHuuin->search(0x50616468753031ull /* "Padhu01" */)->getBounds()->i.y;
    field_0x94[1] = mScreen->search(0x50616468753032ull /* "Padhu02" */)->getBounds()->i.y;
    field_0x94[2] = mScreen->search(0x50616468753033ull /* "Padhu03" */)->getBounds()->i.y;
    mScreenHuuin->search(0x50616468753031ull /* "Padhu01" */)->setAlpha(0xFF);
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
