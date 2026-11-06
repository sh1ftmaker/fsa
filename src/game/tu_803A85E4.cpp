#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804AF878.h"
#include "game/tu_803E1668.h"
#include "game/tu_803F0A54.h"
#include "JSystem/JKernel/JKRArchive.h"

cls_804AF878* lbl_8053AD98;

cls_804AF878::cls_804AF878() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    field_0xc = 4;
    if (lbl_80529DEC.field_0x242 == 0) {
        lbl_80529DEC.field_0x242 = 1;
        gGameRoot2_8053AB10->field_0x20->fn_802FF798();
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_base_sch.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_schE.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0x7000, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F93F4(res, 0, 0, 0, 0, false);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "player_sch.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 2, 0x4000, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F93F4(res, 0, 2, 0, 0, false);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gals_sch.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 3, 0, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F93F4(res, 0, 3, 0, 0, false);
        gGameRoot2_8053AB10->field_0x20->field_0x2364 = 1;
    }
    field_0x4 = new cls_804B0B00();
    field_0x8 = new ConnectScreen_804B0F10();
    if (gAudioMgr_8053AF80->field_0x58 != 0x100001A) {
        gAudioMgr_8053AF80->fn_80458FF0(0x100001A, false, false);
        gAudioMgr_8053AF80->fn_80458408(0x100001A, true);
        lbl_80529DEC.field_0x248 = 1;
    } else if (lbl_80529DEC.field_0x248 != 1) {
        gAudioMgr_8053AF80->fn_80458408(0x100001A, true);
        lbl_80529DEC.field_0x248 = 1;
    }
    if (!lbl_80529DEC.field_0xc->fn_804580DC(3)) {
        gAudioMgr_8053AF80->fn_80458090();
    }
    lbl_8053AD98 = this;
}

cls_804AF878::~cls_804AF878() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    lbl_80529DEC.field_0x28->unmount();
    lbl_8053AD98 = NULL;
}

void cls_804AF878::fn_803A86A4() {
    field_0xc = 4;
    if (lbl_80529DEC.field_0x237 == 3 || lbl_80529DEC.field_0x237 == 4) {
        if (gGameRoot_8053AAF8->field_0x7f) {
            if (gGameRoot_8053AAF8->field_0x7e == 0) {
                field_0x4->fn_803EDBF0(2);
            } else {
                field_0x4->fn_803EDBF0(1);
            }
            if (lbl_80529DEC.field_0x237 == 3) {
                lbl_80529DEC.field_0x237 = 9;
            } else if (lbl_80529DEC.field_0x237 == 4) {
                lbl_80529DEC.field_0x237 = 7;
            } else {
                lbl_80529DEC.field_0x237 = 0;
            }
        } else {
            field_0x8->fn_803FC5E4(1);
            lbl_80529DEC.field_0x237 = 0;
        }
    } else {
        if (gGameRoot_8053AAF8->field_0x7f) {
            field_0x4->fn_803EDBF0(0);
        } else {
            field_0x8->fn_803FC5E4(0);
        }
        lbl_80529DEC.field_0x237 = 0;
    }
}

void cls_804AF878::fn_803A8664() {
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0x4->fn_803EDA2C();
    } else {
        field_0x8->fn_803FC46C();
    }
}

void cls_804AF878::fn_803A8600() {
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0x4->fn_803ED83C();
        field_0xc = field_0x4->field_0x360;
    } else {
        field_0x8->fn_803FC304();
        field_0xc = field_0x8->field_0x350;
    }
}

s32 fn_803A85E4() {
    if (lbl_8053AD98 != NULL) {
        return lbl_8053AD98->field_0xc;
    }
    return 4;
}
