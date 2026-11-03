#include "game/GameAudio.h"
#include "game/act/act_slime_manager.h"
#include "game/Map.h"
#include "game/ActorManager.h"
#include "game/act/tu_80209C14.h"

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

bool cls_804A5BD8::fn_80207E58() {
    bool result = false;
    switch (mState) {
    case 3:
        if (field_0x34a >= 10) {
            fn_80208244(6);
            result = true;
        } else {
            fn_80208244(2);
        }
        break;
    case 6:
        result = true;
        break;
    }
    return result;
}

void cls_804A5BD8::fn_80207ECC() {
    switch (mState) {
    case 1:
        fn_80208244(3);
        break;
    }
}

void cls_804A5BD8::fn_802088B4() {
    if (mState != 5) {
        fn_80208244(5);
    }
    for (s32 i = 0; i < 4; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(mSlots[i].mSerial);
        if (actor != NULL) {
            static_cast<cls_804A5D60*>(actor)->fn_80209D44();
        }
    }
}

void cls_804A5BD8::fn_80208924() {
    field_0x346 = fn_8022461C(&mPos, field_0x4);
    field_0x348 = fn_80226850(&mPos, field_0x4);
}
