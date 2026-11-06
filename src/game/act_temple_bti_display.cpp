#include "game/GameAudio.h"
#include "game/act/act_temple_bti_display.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/Unk_8045A080.h"

static const char* sBtiNames[3][9] = {
    {
        "bg_tenkuu.bti",
        "tenkuunotou.bti",
        "koorinoshinden.bti",
        "yaminosinden.bti",
        "honounotou.bti",
        "pyramid.bti",
        "sabaku_sinden_l.bti",
        "ana.bti",
        "higasi_sinden.bti",
    },
    {
        "bg_tenkuu_ue.bti",
        "tenkuunotou_ue.bti",
        "koorinoshinden_kabuse.bti",
        "yaminosinden_ue.bti",
        "honounotou_ue.bti",
        "pyramid_kabuse.bti",
        "sabaku_sinden_l_ue.bti",
        "ana_ue.bti",
        "higasi_sinden_ue.bti",
    },
    {
        "bg_tenkuu_tobira.bti",
        "tenkuunotou_tobira.bti",
        "koorinoshinden_tobira.bti",
        "yaminosinden_tobira.bti",
        "honounotou_tobira.bti",
        "pyramid_tobira.bti",
        "sabaku_sinden_l_tobira.bti",
        "ana_tobira.bti",
        "higasi_sinden_tobira.bti",
    },
};

static const s32 sOffsets[9] = {
    -4, -3, 4, -2, -1, 3, 0, -1, -6,
};

static const f32 sFloatTable1[9] = {
    -5.0f, 0.0f, 2.0f, -3.0f, 0.0f, 2.0f, -3.5f, -2.0f, -6.0f,
};

static const f32 sFloatTable2[9] = {
    0.0f, 4.0f, -24.0f, 0.0f, 0.0f, -12.0f, 0.0f, 0.0f, 0.0f,
};

static const u32 sSoundTable[9] = {
    0x01000016, 0x01000015, 0x01000015, 0x01000011, 0x01000011,
    0x01000012, 0x01000012, 0x01000000, 0x0100000E,
};

void TempleBTIDisplay_8049CB04::init() {
    field_0x238 = field_0x90 & 0xF;
    field_0x234 = (field_0x90 >> 8) & 0xF;
    field_0x240 = 0.0f;
    field_0x23c = 0;
    field_0x244 = 0;
    field_0x245 = 0;
    field_0x246 = 0;
    if (field_0x234 == 2) {
        field_0x244 = 1;
    }
    if (getGameView_802D6A9C()->field_0x131f9 != 0) {
        vfunc_0x30(0xFF, true);
    }
    if (field_0x234 == 2) {
        if (vfunc_0x34()) {
            field_0x245 = 2;
            field_0x246 = 1;
        } else {
            field_0x246 = 2;
        }
    }
}

TempleBTIDisplay_8049CB04::~TempleBTIDisplay_8049CB04() {
    if (field_0x234 == 2) {
        if (gGameRoot_8053AAF8->field_0x4c != 9 && gGameRoot_8053AAF8->field_0x4c != 11) {
            GameView_804A9950* view = getGameView_802D6A9C();
            view->field_0x1328c = 0;
            gGameRoot_8053AAF8->fn_80138B7C();
        }
    }
}

void TempleBTIDisplay_8049CB04::update() {
    f32 y = mPos.y + 16.0f * (lbl_8049C3B8[0] + sOffsets[field_0x238]) + sFloatTable2[field_0x238] + (f32)lbl_8049C3B8[2];

    switch (field_0x234) {
    case 0:
        field_0x244 = 1;
        break;
    case 1:
        field_0x244 = 0;
        break;
    case 2:
        if (field_0x245 == 0) {
            u16 angle = 0;
            s32 dir = 0;
            f32 dist = 0.0f;
            s32 player = 0;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
            if (player >= 0) {
                if (getPlayerPos_8023E724(player)->y - y < 80.0f) {
                    vfunc_0x30(0xFF, true);
                    field_0x245 = 1;
                }
            }
        } else if (field_0x245 == 1) {
            field_0x240 -= 0.2f;
            if (field_0x240 < -24.0f) {
                field_0x244 = 0;
            }
            if (field_0x240 < -40.0f) {
                field_0x240 = -40.0f;
                field_0x245 = 2;
            }
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 1.0f, 0.0f, false);
            gGameRoot_8053AAF8->fn_80134DE4(1, 0);
        } else {
            field_0x240 = -40.0f;
            field_0x244 = 0;
        }
        break;
    }

    if (field_0x244 != 0) {
        JGeometry::TBox2f box(mPos.x - 32.0f, y - 32.0f, mPos.x + 32.0f, y + 32.0f);
        fn_80239914(&box, field_0x4, mSerial);
    }

    switch (field_0x246) {
    case 1:
        gAudioMgr_8053AF80->fn_80458FF0(sSoundTable[field_0x238], false, 0);
        field_0x246 = 0;
        break;
    case 2:
        gAudioMgr_8053AF80->fn_80458FF0(sSoundTable[field_0x238], true, 0);
        field_0x246 = 0;
        break;
    }
}

void TempleBTIDisplay_8049CB04::draw() {
    JGeometry::TVec3f pos = mPos;
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -1;
    params.field_0x24 = 1;
    params.field_0x21 = 1;

    fn_801EFD34(fn_804032E8('TIMG', sBtiNames[0][field_0x238]), &params, -1, 0.0f, 0.0f, 0.0f);

    if (field_0x234 != 1) {
        mPos.y += 16.0f * (lbl_8049C3B8[0] + sOffsets[field_0x238]) + field_0x240;
        if (field_0x240 < -24.0) {
            params.field_0x1d = 1;
        } else {
            params.field_0x1d = 0;
        }
        fn_801EFD34(fn_804032E8('TIMG', sBtiNames[2][field_0x238]), &params, -1, 0.0f, 0.0f, 0.0f);
        mPos.y = pos.y;
    }

    if (field_0x238 == 5 && field_0x234 == 0) {
        params.field_0x1d = 0;
    } else {
        params.field_0x1d = 2;
    }
    mPos.y += 16.0f * ((f32)lbl_8049C3B8[1] + sFloatTable1[field_0x238]);
    fn_801EFD34(fn_804032E8('TIMG', sBtiNames[1][field_0x238]), &params, -1, 0.0f, 0.0f, 0.0f);
    mPos.y = pos.y;
}
