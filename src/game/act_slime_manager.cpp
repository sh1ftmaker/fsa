#include "game/GameAudio.h"
#include "game/act/act_slime_manager.h"
#include "game/Map.h"

const f32 lbl_8053F8E0 = 0.0f;

cls_804A5BD8::cls_804A5BD8() {
    field_0x340 = 0;
    field_0x344 = 0;
    field_0x345 = 0;
    field_0x346 = 0;
    field_0x348 = 0;
    field_0x34a = 0;
}

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

void cls_804A5BD8::fn_80208924() {
    field_0x346 = fn_8022461C(&mPos, field_0x4);
    field_0x348 = fn_80226850(&mPos, field_0x4);
}
