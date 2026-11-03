#include "game/GameAudio.h"
#include "game/act/act_slime_manager.h"

const f32 lbl_8053F8E0 = 0.0f;

void cls_804A5BD8::vfunc_0x88() {
    field_0x3c.x = lbl_8053F8E0;
    field_0x3c.y = lbl_8053F8E0;
    field_0x3c.z = lbl_8053F8E0;
}

void cls_804A5BD8::fn_80207E24() {
    switch (mState) {
    case 4:
        fn_80208244(2);
        break;
    }
}

void cls_804A5BD8::fn_80207ECC() {
    switch (mState) {
    case 1:
        fn_80208244(3);
        break;
    }
}
