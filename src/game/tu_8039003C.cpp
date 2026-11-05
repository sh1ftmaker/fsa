#include "game/GameAudio.h"
#include "game/cls_804AF480.h"
#include "game/cls_804B1288.h"
#include "game/cls_804B1378.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "JSystem/JKernel/JKRArchive.h"

cls_804AF480* lbl_8053AD80;

s32 fn_8039003C() {
    if (lbl_8053AD80 != NULL) {
        if (lbl_8053AAF8->field_0x7f) {
            return fn_8040F474();
        }
        return fn_80414F6C();
    }
    return 6;
}

void cls_804AF480::fn_80390088() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053ADF8->fn_8040F480();
    } else {
        lbl_8053AE00->fn_80416BD0();
    }
}

void cls_804AF480::fn_803900C8() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053ADF8->fn_804108B4();
    } else {
        lbl_8053AE00->fn_804170B0();
    }
}

void cls_804AF480::fn_80390108() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053ADF8->fn_804111C4();
    }
}

void cls_804AF480::fn_8039013C() {
    if (lbl_8053AAF8->field_0x7f) {
        lbl_8053ADF8->fn_80411230();
    } else {
        lbl_8053AE00->fn_804170EC();
    }
}

cls_804AF480::~cls_804AF480() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_80529DEC.field_0xf8[0] = 0;
    lbl_80529DEC.field_0xf8[1] = 0;
    lbl_80529DEC.field_0xf8[2] = 0;
    lbl_80529DEC.field_0xf8[3] = 0;
    if (field_0x4) {
        delete field_0x4;
        field_0x4 = NULL;
    }
    if (field_0x8) {
        delete field_0x8;
        field_0x8 = NULL;
    }
    lbl_80529DEC.field_0x34->unmount();
    lbl_8053AD80 = NULL;
}

cls_804AF480::cls_804AF480() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053AD80 = this;
    field_0x4 = NULL;
    field_0x8 = NULL;
    lbl_8053AB10->field_0x2c->fn_80158B3C();
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
        lbl_8053AB10->field_0x20->fn_802F9714(res, 0, 2, 0, 0);
        res = lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        lbl_8053AB10->field_0x20->fn_802F93F4(res, 0, 2, 0, 0, false);
        res = lbl_8053AAF8->field_0xa4->getResource('SCH ', "gals_sch.szs");
        lbl_8053AB10->field_0x20->fn_802F9714(res, 0, 3, 0, 0);
        res = lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        lbl_8053AB10->field_0x20->fn_802F93F4(res, 0, 3, 0, 0, false);
        lbl_8053AB10->field_0x20->field_0x2364 = 1;
    }
    if (lbl_8053AAF8->field_0x7f) {
        field_0x4 = new cls_804B1288;
    } else {
        field_0x8 = new BattleStageSelect_804B1378;
    }
    if (lbl_8053AF80->field_0x58 != 0x100001A) {
        lbl_8053AF80->fn_80458FF0(0x100001A, true, false);
        lbl_8053AF80->fn_80458408(0x100001A, true);
        lbl_80529DEC.field_0x248 = 1;
    } else if (lbl_80529DEC.field_0x248 != 1) {
        lbl_8053AF80->fn_80458408(0x100001A, true);
        lbl_80529DEC.field_0x248 = 1;
    }
    if (!lbl_80529DEC.field_0xc->fn_804580DC(3)) {
        lbl_8053AF80->fn_80458090();
    }
}
