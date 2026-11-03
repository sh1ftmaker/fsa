#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/act_vaati_demo.h"

// The animation table of field_0x238: (cel, duration) pairs ended by a cel < 0.
static const s16 lbl_80464990[] = {0, 10, 1, 10, 2, 10, 3, 10, -1, -1};

static const s16* lbl_80539F08[2] = {lbl_80464990, NULL};

static const char* lbl_8049D930[4] = {
    "e_gufu_1.bti", "e_gufu_2.bti", "e_gufu_3.bti", "e_gufu_4.bti",
};

static const char lbl_80464A14[] = "kk_black.bti";

// .sdata2 floats: zero and the z height of mPos. Arrays, so the names survive.
static const f32 lbl_8053C6D0[1] = {0.0f};
static const f32 lbl_8053C704[1] = {20.0f};

cls_8049D940::cls_8049D940() : field_0x230(0), field_0x234(0) {
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x32c = 0;
    field_0x31c[0] = 0;
    field_0x31c[1] = 0;
    field_0x31c[2] = 0;
    field_0x31c[3] = 0;
}

cls_8049D940::~cls_8049D940() {
    field_0x31c[0] = 0;
    field_0x31c[1] = 0;
    field_0x31c[2] = 0;
    field_0x31c[3] = 0;
    field_0x32c = 0;
    fn_801EE434(field_0x314);
    field_0x314 = NULL;
    fn_801EE434(field_0x318);
    field_0x318 = NULL;
}

s32 cls_8049D940::vfunc_0x58() {
    return field_0x230;
}

void cls_8049D940::init() {
    cls_8049D140::init();
    for (s32 i = 0; i < 4; i++) {
        field_0x31c[i] = fn_804032E8('TIMG', lbl_8049D930[i]);
    }
    // 0x3F3F3F3F is the four-character type '????'.
    field_0x32c = lbl_8053AAF8->field_0xa4->getResource(0x3F3F3F3F, lbl_80464A14);
    mPos.z = lbl_8053C704[0];
    field_0x3c.x = field_0x3c.y = field_0x3c.z = lbl_8053C6D0[0];
    mDrawEnabled = false;
    field_0x238.fn_801D2608(lbl_80539F08, 1);
    field_0x238.fn_801D22DC(0, 2, false);
    field_0x238.fn_801D1F14(lbl_8053C6D0[0]);
    field_0x270.fn_801D2608(lbl_80539F08, 1);
    field_0x270.fn_801D22DC(0, 2, false);
    field_0x270.fn_801D1F14(lbl_8053C6D0[0]);
    field_0x308 = mPos;
}
