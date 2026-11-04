#include "game/GameAudio.h"
#include "game/cls_804B13D4.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "JSystem/J2D/J2DPicture.h"

// J2DPane::search tags, as u64 (the names are padded with leading zeros).
static const u64 lbl_8048DDD0[6] = {
    0x00000050746B3031ull, /* "Ptk01" */
    0x0000000050743031ull, /* "Pt01" */
    0x00000050746B3032ull, /* "Ptk02" */
    0x0000000050743032ull, /* "Pt02" */
    0x00005074696D3031ull, /* "Ptim01" */
    0x00005074696D3032ull, /* "Ptim02" */
};

// The digit textures fn_8041918C passes to fn_804030C4 ('TIMG').
const char* lbl_804B139C[10] = {
    "kk_rupyNum_00.bti",
    "kk_rupyNum_01.bti",
    "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti",
    "kk_rupyNum_04.bti",
    "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti",
    "kk_rupyNum_07.bti",
    "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

void cls_804B13D4::fn_8041918C(s32 player) {
    if (fn_802366CC(fn_8022EA0C(player)) > 0 && fn_802366CC(fn_8022EA0C(player)) < 300) {
        s32 level = fn_802366CC(fn_8022EA0C(player)) / 60 + 1;
        if (level != field_0x294[player]) {
            if (level > 99) {
                level = 99;
            }
            if (level < 0) {
                level = 0;
            }
            if (level < 10) {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[level]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[0]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[1]))->changeTexture(res, 0);
                if (field_0x2c[player]->search(lbl_8048DDD0[2])->isVisible() == true) {
                    field_0x2c[player]->search(lbl_8048DDD0[2])->hide();
                    field_0x2c[player]->search(lbl_8048DDD0[3])->hide();
                }
            } else {
                const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[level / 10]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[0]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[1]))->changeTexture(res, 0);
                res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B139C[level % 10]));
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[2]))->changeTexture(res, 0);
                static_cast<J2DPicture*>(field_0x2c[player]->search(lbl_8048DDD0[3]))->changeTexture(res, 0);
                if (!field_0x2c[player]->search(lbl_8048DDD0[2])->isVisible()) {
                    field_0x2c[player]->search(lbl_8048DDD0[2])->show();
                    field_0x2c[player]->search(lbl_8048DDD0[3])->show();
                }
            }
            field_0x294[player] = level;
        }
    } else {
        field_0x294[player] = 0;
    }
    if (field_0x234[player] > 0) {
        field_0x234[player]--;
    }
}

cls_804B13D4::~cls_804B13D4() {
    if (field_0x50 != NULL) {
        delete field_0x50;
        field_0x50 = NULL;
    }
    delete field_0x4c;
    field_0x4c = NULL;
    delete field_0x4;
    field_0x4 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x2c[i];
        field_0x2c[i] = NULL;
    }
    delete field_0x18;
    field_0x18 = NULL;
    delete field_0x54;
    field_0x54 = NULL;
    delete field_0x58;
    field_0x58 = NULL;
    delete field_0x5c;
    field_0x5c = NULL;
    delete field_0x6c;
    field_0x6c = NULL;
    delete field_0x70;
    field_0x70 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x74[i];
        field_0x74[i] = NULL;
        delete field_0x84[i];
        field_0x84[i] = NULL;
        delete field_0x94[i];
        field_0x94[i] = NULL;
        delete field_0xa4[i];
        field_0xa4[i] = NULL;
        delete field_0xb4[i];
        field_0xb4[i] = NULL;
        delete field_0xc4[i];
        field_0xc4[i] = NULL;
        delete field_0xd4[i];
        field_0xd4[i] = NULL;
        delete field_0xe4[i];
        field_0xe4[i] = NULL;
    }
}
