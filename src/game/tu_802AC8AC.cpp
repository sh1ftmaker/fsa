// A full-screen picture, "kk_black.bti" from main2D.arc (GameState_804B0FE0
// field_0x20): a black fade. tu_80134C04 creates the one instance with
// __nw__(0x1C, heap, 0) (0x801393E4) and the constructor fn_802ACBE8, which
// stores it in lbl_8053ACA0 (in tu_8022AD64's .sbss). Vtable 0x804A8390:
// only the destructor.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "JSystem/J2D/J2DPicture.h"

class cls_804A8390 {
public:
    cls_804A8390();
    virtual ~cls_804A8390();

    void fn_802AC8AC(s32 value);
    bool fn_802AC8B4();
    void fn_802AC980();
    void fn_802AC98C();
    void fn_802ACA64();
    void fn_802ACB58();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DPicture* mPicture;
    /* 0x08 */ u8 field_0x8[0xC - 0x8];
    // Passed to the picture's vtable slot 0x24 (setAlpha) by fn_802AC98C and
    // fn_802ACA64; fn_802AC8B4 steps it by 10 between 0 and 0xFF.
    /* 0x0C */ s32 mAlpha;
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ s32 field_0x14;
    /* 0x18 */ s32 field_0x18;
}; // Size: 0x1C

extern cls_804A8390* lbl_8053ACA0;

void cls_804A8390::fn_802AC8AC(s32 value) {
    field_0x18 = value;
}

bool cls_804A8390::fn_802AC8B4() {
    s32 state = lbl_8053AAF8->field_0x58;
    if (state == 1 || state == 5) {
        mAlpha += 10;
        if (mAlpha >= 0xFE) {
            mAlpha = 0xFF;
            if (lbl_8053AAF8->field_0x58 == 5) {
                field_0x14 = 10;
            }
            return true;
        }
        return false;
    }
    if (state == 4) {
        mAlpha -= 10;
        if (mAlpha < 0) {
            mAlpha = 0;
            return true;
        }
        return false;
    }
    if (state == 6) {
        if (field_0x14 == 0) {
            return true;
        }
        field_0x14--;
        return false;
    }
    return true;
}

void cls_804A8390::fn_802AC980() {
    field_0x10 = 2;
}

void cls_804A8390::fn_802AC98C() {
    if (field_0x10 > 0) {
        if (field_0x10 < 0xFD) {
            field_0x10 += 2;
        } else {
            field_0x10 = 0xFF;
        }
        lbl_8053AB10->fn_8013B490();
        mPicture->setBlackWhite(JUtility::TColor(0xFF), JUtility::TColor(0));
        mPicture->setAlpha(field_0x10);
        mPicture->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    }
}

void cls_804A8390::fn_802ACA64() {
    lbl_8053AB10->fn_8013B490();
    if (field_0x18 == 0) {
        mPicture->setBlackWhite(JUtility::TColor(0xFF), JUtility::TColor(0));
    } else {
        mPicture->setBlackWhite(JUtility::TColor(0xFFFFFFFF), JUtility::TColor(0xFFFFFF00));
    }
    mPicture->setAlpha(mAlpha);
    mPicture->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

void cls_804A8390::fn_802ACB58() {
    mAlpha = 0;
    field_0x18 = 0;
}

cls_804A8390::~cls_804A8390() {
    delete mPicture;
    mPicture = NULL;
}

cls_804A8390::cls_804A8390() {
    lbl_8053ACA0 = this;
    const ResTIMG* timg = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_black.bti"));
    mPicture = new J2DPicture(timg);
    mAlpha = 0;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x18 = 0;
}
