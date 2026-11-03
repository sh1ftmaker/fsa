// A full-screen picture, "kk_black.bti" from main2D.arc (GameState_804B0FE0
// field_0x20): a black fade. tu_80134C04 creates the one instance with
// __nw__(0x1C, heap, 0) (0x801393E4) and the constructor fn_802ACBE8, which
// stores it in lbl_8053ACA0 (in tu_8022AD64's .sbss). Vtable 0x804A8390:
// only the destructor.

#include "game/GameAudio.h"
#include "game/GameState.h"
#include "JSystem/J2D/J2DPicture.h"

class cls_804A8390 {
public:
    cls_804A8390();
    virtual ~cls_804A8390();

    void fn_802AC8AC(s32 value);
    void fn_802AC980();
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

// FIXME: fn_802AC8B4 (the fade step, on lbl_8053AAF8->0x58), fn_802AC98C and
// fn_802ACA64 (draw through the picture's vtable slots 0x130, 0x24 and 0xEC)
// are not written yet.

void cls_804A8390::fn_802AC980() {
    field_0x10 = 2;
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
