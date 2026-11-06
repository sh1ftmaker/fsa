// A rupee-digit sprite pool: vtable 0x804B1E1C (only the destructor), the
// singleton is stored in lbl_8053AF50. It holds one J2DPicture per digit image
// "kk_rupyNum_00.bti".."kk_rupyNum_09.bti" plus "rupy_num_exclamation.bti"
// (main2D.arc, GameState_804B0FE0 field_0x20) and 128 queued sprites that
// fn_804556A4 draws through the digit's picture.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/GameState.h"
#include "JSystem/J2D/J2DPicture.h"
#include "game/cls_804B1E1C.h"

static const char* lbl_804B1DF0[11] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti", "kk_rupyNum_04.bti", "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti", "kk_rupyNum_07.bti", "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti", "rupy_num_exclamation.bti",
};

// FIXME: 99%. Regalloc only: the loop counter is r27 and the ResTIMG r28 in
// the original; swapped here.
cls_804B1E1C::cls_804B1E1C() {
    for (int i = 0; i < 11; i++) {
        const ResTIMG* timg = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1DF0[i]));
        mDigits[i] = new J2DPicture(timg);
    }
    fn_8045560C();
    lbl_8053AF50 = this;
}

cls_804B1E1C::~cls_804B1E1C() {
    for (int i = 0; i < 11; i++) {
        delete mDigits[i];
        mDigits[i] = NULL;
    }
    lbl_8053AF50 = NULL;
}

void cls_804B1E1C::fn_80455770(f32 scale, u8 digit, const Vec* pos, JUtility::TColor color) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    f32 baseSize = 48.0f * camera->field_0x858;
    Vec screen;
    camera->fn_8030C5F0(pos, false, false, &screen);
    for (int i = 0; i < 128; i++) {
        if (!mSprites[i].active) {
            mSprites[i].active = 1;
            mSprites[i].digit = digit;
            mSprites[i].size = scale * baseSize;
            mSprites[i].x = screen.x - mSprites[i].size / 2;
            mSprites[i].y = screen.y - mSprites[i].size / 2;
            mSprites[i].color.set(color);
            break;
        }
    }
}

void cls_804B1E1C::fn_804556A4() {
    gGameRoot2_8053AB10->fn_8013B490();
    for (int i = 0; i < 128; i++) {
        Sprite& s = mSprites[i];
        if (s.active == 1) {
            mDigits[s.digit]->setWhite(s.color);
            mDigits[s.digit]->draw(s.x, s.y, s.size, s.size, false, false, false);
        }
    }
}

void cls_804B1E1C::fn_8045560C() {
    for (int i = 0; i < 128; i++) {
        mSprites[i].active = 0;
    }
}
